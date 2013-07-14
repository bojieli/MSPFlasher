default:
	gcc -Wall -O2 -g -o flash flash.c

clean:
	@rm ./flash
	@rm id.txt
	@rm resource/flash.txt
	@rm -rf Log/
