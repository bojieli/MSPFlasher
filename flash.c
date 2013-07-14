#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Product ID
 * | Product Type | Product Version | Random ID | XOR checksum |
 * |    1 byte    |     1 byte      |  6 bytes  |    1 byte    |
 */
#define PRODUCT_TYPE 1
#define PRODUCT_VERSION 1

#define RESOURCE_DIR "resource/"
#define HEXFILE RESOURCE_DIR "RFID_G2001.txt"
#define IDFILE "id.txt"
#define GENHEXFILE RESOURCE_DIR "flash.txt"
#define BUF_SIZE 16384
#define ID_SIZE 9
#define LINE_LEN 18

#define bool char
#define true 1
#define false 0

static char int2hex(int n) {
    if (n >= 0 && n <= 9)
        return n + '0';
    if (n >= 10 && n <= 15)
        return n - 10 + 'A';
    printf("int2hex: Invalid value %d\n", n);
    return -1;
}

static int hex2int(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    printf("hex2int: Invalid char 0x%x\n", hex);
    return -1;
}

unsigned char *set = NULL;
int alloc_size = 0;
int used_size = 0;

void set_add(unsigned char* id) {
    if (used_size + ID_SIZE > alloc_size) {
        alloc_size = (alloc_size + ID_SIZE) * 2;
        set = (unsigned char*)realloc(set, alloc_size);
        if (set == NULL) {
            printf("FATAL: No memory\n");
            exit(1);
        }
    }
    memcpy(set + used_size, id, ID_SIZE);
    used_size += ID_SIZE;
}

bool set_isin(unsigned char* id) {
    unsigned char *cur = set;
    while (cur < set + used_size) {
        if (memcmp(cur, id, ID_SIZE) == 0)
            return true;
        cur += ID_SIZE;
    }
    return false;
}

FILE* my_fopen(const char* file, const char* mode) {
    FILE *fp = fopen(file, mode);
    if (fp == NULL) {
        printf("%s does not exist\n", file);
        exit(1);
    }
    return fp;
}

void generate_id(unsigned char* id) {
    id[0] = PRODUCT_TYPE & 0xFF;
    id[1] = PRODUCT_VERSION & 0xFF;
    
    int i;
    for (i=2; i<ID_SIZE-1; i++)
        id[i] = rand() & 0xFF;

    id[ID_SIZE-1] = 0;
    for (i=0; i<ID_SIZE-1; i++)
        id[ID_SIZE-1] ^= id[i];
}

int main() {
    int i;

    FILE *fphex = my_fopen(HEXFILE, "r");
    char hex[BUF_SIZE] = {0};
    int hex_file_len = fread(hex, 1, BUF_SIZE, fphex);
    for (i=0; i<hex_file_len; i++)
        if (hex[i] == '\n')
            break;
    if (i == hex_file_len) {
        printf("Invalid hexfile\n");
        return 1;
    }
    int hex_id_pos = i+1;
    fclose(fphex);

    FILE *fpid = fopen(IDFILE, "r");
    if (fpid == NULL) {
        printf("ID file not found, creating a new one\n");
    } else {
        int counter = 0;
        while (1) {
            char line[LINE_LEN+2];
            int bytes = fread(line, 1, LINE_LEN+2, fpid);
            if (bytes < LINE_LEN+2)
                break;
            if (line[LINE_LEN] != '\r' || line[LINE_LEN+1] != '\n') {
                printf("Invalid ID file\n");
                return 1;
            }

            unsigned char id[ID_SIZE];
            for (i=0; i<ID_SIZE; i++)
                id[i] = (hex2int(line[i*2]) << 4) + hex2int(line[i*2+1]);
            set_add(id);
            ++counter;
        }
        fclose(fpid);
        printf("read %d existing IDs from file\n", counter);
    }

    srand((int)time(NULL));
    while (1) {
        unsigned char id[ID_SIZE];
        do {
            generate_id(id);
        } while (set_isin(id));

        for (i=0; i<ID_SIZE; i++) {
            hex[hex_id_pos + i*3] = int2hex(id[i] >> 4);
            hex[hex_id_pos + i*3 + 1] = int2hex(id[i] & 0xF);
            hex[hex_id_pos + i*3 + 2] = ' ';
        }
        printf("\nPress ENTER to flash next...");
        getchar();
        printf("ID to flash: ");
        fwrite(hex + hex_id_pos, 3, ID_SIZE, stdout);
        printf("\n\n");

        FILE *gen = my_fopen(GENHEXFILE, "w");
        fwrite(hex, 1, hex_file_len, gen);
        fclose(gen);
        
        char line[LINE_LEN+2];
        for (i=0; i<ID_SIZE; i++) {
            line[i*2] = int2hex(id[i] >> 4);
            line[i*2 + 1] = int2hex(id[i] & 0xF);
        }
        line[LINE_LEN] = '\r';
        line[LINE_LEN+1] = '\n';

        FILE *fpid = my_fopen(IDFILE, "a");
        fwrite(line, 1, LINE_LEN+2, fpid);
        fclose(fpid);

        system(RESOURCE_DIR "MSP430Flasher.exe -i USB -n MSP4302001 -w " GENHEXFILE " -v");
        printf("====== Flash complete. ======\n");
    }
}
