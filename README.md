How to Build
------------

In cygwin, type ```make``` and it will generate a ```flash.exe```.

How to Flash
------------

In cygwin, type ```./flash```.

If this is not the first time of flashing, please put ```id.txt``` in the current folder. This file contains flashed IDs which guanrantees random-generated IDs do not conflict.

When it prompts "Press ENTER to flash next", plug out the old chip and insert a new chip to be flashed, then press Enter.

When all chips have been completed, type Ctrl+C to exit. Keep the ```id.txt``` file for further use.

