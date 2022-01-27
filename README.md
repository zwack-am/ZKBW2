# ZKBW2 - Zwack's Keyboard Wedge 2
A platformio program for an Adafruit Trinket M0 and an RC522 RFID reader

This code allows you to "type" a password by reading a suitably formatted token on the reader.

This code has been released into the Public Domain by me

Most discussion about this code has been happening at the [Dangerous Things Forum](https://forum.dangerousthings.com/t/improved-keyboard-wedge/12642).  

The License is the Unlicense, putting the code into the public domain.  If you have a feature request or bug report then please open an issue.

If you have a code enhancement and are willing to put it into the Public Domain then please send me a pull request.  If you wish your contributions to be acknowledged feel free to add your name or tag to the README as part of the pull request.

While the development has switched to [platformio](https://platformio.org/) it shouldn't be too hard to use.  If you install platformio on your system you can either use the platformio core to compile and upload this `pio run -t upload` or whichever IDE you are used to using. I am using CodeBlocks, but you can use whatever you like.

Releases are also released as UF2 files.  If you are using a UF2 bootloader (the Tirinket m0 has one for example) then you can just press the reset button twice and wait for the green light.  At that point your computer will have a drive called TRINKET_BOOT and you can simply drop the trinket.uf2 file onto that drive, when it is finished copying the trinket will reset and will be running that program.

The releases use the standard config.h as included in the source code.

If you wish to use a custom configuration then I am afraid you will have to build the code for yourself.
