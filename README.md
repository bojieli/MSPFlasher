How to Build
------------

* Use cygwin: type ```make``` and it will generate a ```flash.exe```. The executive built in this way will not run without ```cygwin1.dll```.
* Use Visual Studio (recommended): Create a new VS project, do NOT use Procompile Header (stdafx), add ```flash.c``` to the project and build.


How to Flash
------------

* If the executive is built with ```make```, open cygwin and goto the directory, type ```./flash```.
* If the executive is built with Visual Studio, double-click ```flash.exe``` in the folder.

If this is not the first time of flashing, please put ```id.txt``` in the current folder. This file contains flashed IDs which guanrantees random-generated IDs do not conflict.

When it prompts "Press ENTER to flash next", plug out the old chip and insert a new chip to be flashed, then press Enter.

When all chips have been completed, type Ctrl+C to exit. Keep the ```id.txt``` file for further use.


How to Change Product Type
--------------------------

The first two bytes of Product ID are Product Type and Product Version.

These two fields are hardcoded in ```flash.c``` and defined by C macros. Their values should not exceed 0xFF (255).

Product Type should be the same within one type of product. This field is kept for further use when we have more products to inter-connect.

Product Version is for each software or hardware version of the product. Please increment this field on each software/hardware update, so we can identify product version from product ID. After changing, you have to rebuild ```flash.exe```.

