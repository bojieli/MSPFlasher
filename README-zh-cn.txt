MSPFlasher 说明
---------------

编译运行
--------

* 新建 Visual Studio 工程
* 选择控制台工程，不要使用预编译头（stdafx）
* 将 flash.c 加入工程
* 选择 Release 模式，编译
* 将 resource 文件夹复制到 Release 文件夹
* 双击运行 Release 文件夹中的 flash.exe


烧写说明
--------

如果不是第一次烧写，需要把 id.txt 文件放在与 flash.exe 相同的目录中。这个文件包括所有已经烧写过的ID，用于保证随机生成的ID不冲突。

当命令行提示 "Press ENTER to flash next" 时，换上新的一片待烧写芯片，回车，就会开始烧写。

烧写完成后，按 Ctrl+C 退出，保存好 id.txt 文件以便下次烧写使用。

