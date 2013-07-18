# 编译运行

1. 新建 Visual Studio 工程
2. 选择控制台工程，不要使用预编译头（stdafx）
3. 将 flash.c 加入工程
4. 选择 Release 模式，编译
5. 将 resource 文件夹复制到 Release 文件夹
6. 双击运行 Release 文件夹中的 flash.exe

在 cygwin 下编译的方法（不推荐）：make，生成 flash.exe，这样生成的程序需要有 cygwin1.dll 才能运行。

# 烧写说明

如果不是第一次烧写，需要把 id.txt 文件放在与 flash.exe 相同的目录中。这个文件包括所有已经烧写过的ID，用于保证随机生成的ID不冲突。

当命令行提示 "Press ENTER to flash next" 时，换上新的一片待烧写芯片，回车，就会开始烧写。

烧写完成后，按 Ctrl+C 退出，保存好 id.txt 文件以便下次烧写使用。

# Card ID

Card ID consists of 9 bytes range 0..255.

The first two bytes are Product Type and Product Version. These two fields are hardcoded in ```flash.c``` and defined by C macros.

Product Type should be the same within one type of product. This field is kept for further use when we have more products to inter-connect.

Product Version is for each software or hardware version of the product. Please increment this field on each software/hardware update, so we can identify product version from product ID. After changing, you have to rebuild ```flash.exe```.

Bytes 3~8 are random bytes.

Byte 9 is checksum. In C,

```id[8] = id[0] ^ id[1] ^ ... ^ id[7]```

Therefore the following condition holds for all Card IDs, which can be used to validate a card ID:

```id[0] ^ id[1] ^ ... ^ id[8] == 0```

