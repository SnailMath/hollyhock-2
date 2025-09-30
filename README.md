This is a __fork__ of the original hollyhock launcher by The6P4C (https://github.com/Stellaris-code/hollyhock-fork/).
My intention was to modify it a little. I changed the following things:
- BIN:       I added the ability to load prograrms from binary files instead of .hhk files, the extrypoint is the start of the file and the load location is somwhere in the file,
             look at the linker.ld file in the app_template folder.
- EXEC003:   I added the ability to run programs located in program variables. This was the function of the old "LoaderIIb", which is now obsolete.
             I didn't want to miss the feel of typing in programs byte per byte by hand (only if you want to) and sending them over the serial cable.
- Exam mode: The Launcher is disabled in exam mode to prevent cheating. (Even though I would recommend going back to the original firmware for exams.) 
             The loader checks the color of the battery logo to test, if the exam mode is on or off.
- Strings:   I changed a few texts in the OS, the Imaginary Unit button used to load the program was renamed to 'Hollyhock-Launcher'. The version text (usually "classpad II")
             Was changed to "hollyhock" by The6P4C, I changed it to "hollyhock-2".
- Backslash: I will change the backslash character from the symbol `¥` to the symbol `\`.

# DISCLAIMER
- I AM NOT RESPONSIBLE FOR ANY DAMAGE YOU MAY DO TO YOUR CALCULATOR (fx-CP400, fx-CP400+E, fx-CG500 or whatever other model).  
- I AM NOT LIABLE IF YOU BRICK OR OTHERWISE DAMAGE YOUR CALCULATOR.  
- CASIO WILL NOT FIX YOUR CALCULATOR IF IT SHOWS EVIDENCE OF TAMPERING, I.E. FLASHING WITH MODIFIED FIRMWARE IMAGES OR MESSING WITH DEBUG MENUS.  
- I WILL NOT FIX YOUR CALCULATOR.  
- IF YOU CANNOT FIX YOUR CALCULATOR YOURSELF, DO NOT USE THIS TOOLKIT.  
- **BY USING THESE TOOLS, YOU AGREE TO TAKE FULL RESPONSIBILITY FOR ANY DAMAGE THAT MAY OCCUR.**  
- **YOU HAVE BEEN WARNED**.

# Compatibility
Hollyhock is compatible with the fx-CP400, fx-CP400+E and fx-CG500.  
As of 2024, new fx-CP400 hardware revisions (maybe fx-CP400+E and fx-CG500 too?) started to show up. These calculators do not boot on OSes older than v2.01.7002, such as 2.01.2000 (which is used by hollyhock). Please follow at least one of the following procedures to ensure you have a compatible model before installing.

> **Note:** There is a private beta of hollyhock v3 available for testing on our Discord server. It should suport ABS2022 / OS 2.01.7002 / newer calculators. If you are interested in trying out new features or helping with development, please join [our Discord](https://discord.gg/y8Mr8tD) and ask for access to the beta.

### ABS_Date
- With your calculator powered off, open the debug menu by pressing and holding [=] + [EXP] + [Clear/ON] until you see a "Factory use only" screen
- Quickly press [z] + [left] before the screen disappears
- At the bottom of the screen, you should see your `ABS_Date`listed
- Quickly take note of it or take a picture, as this screen will disappear afteer a few seconds
- Your `OS_Date` is not important here
- Calculators with an `ABS_Date` of `2012/09/04 15:11` are known to work with older OS versions and Hollyhock
- Calculators with an `ABS_Date` of `2022/10/28 13:16` are known to boot to a black screen on OS versions older than `2.01.7002`, and therefore will not work with Hollyhock for now
- If your calculator has an ABS_Date different from those, it's something we haven't seen yet and would love to hear about!

### Serial Number
After a bit of research with affected calculators, we've noticed a pattern in their serial numbers  
- Calculators with a new serial number format (starting with `L777A` or `C805A`, with an _Ex (x being a number) suffix in the end) should check their serial numbers second to last digit before the _Ex suffix
  - Calculators which have a `C` there seem to always have an ABS_Date of 2022, and they don't work on OSes older than 2.01.7002. That means no hollyhock on them for now
  - Calculators which have a `B` there seem to always have a 2012 ABS_Date, and should work with hollyhock
- Calculators with an old serial number format (starting with `777A`, `805A` or `804A`, no suffix) seem to always have a 2012 ABS_Date, and should work with hollyhock
- The serial number is located in a label near the batteries, you need to remove the battery cover to find it
- If your serial number doesn't fit any of the above categories, it's something we haven't seen yet and would love to hear about!

Examples:
```plaintext
Known uncomopatible SNs:
- L777A*******5CB_E4
- L777A*******0CE_E0
- L777A*******8CB_E3
- L777A*******9CE_E0

Known compatible SNs:
- L777A*******3BA_E3
- L777A*******9BA_E2
- C805A*******3BA_E2
- 777A**********2
- 804A**********6
```

(if unsure, check the ABS_Date version)

### Notes
- If you install an OS older than 2.01.7002, hollyhock or not, on a 2022 ABS_Date calculator, it'll show nothing on the screen - it'll remain black when you try to turn it on
  - At this state, update mode and the OS Error screen should still work normally. On the calculator hold down the `EXP`, `^` and `Clear` keys. With those keys pressed, momentarily press the RESET button on the back of the calculator. Keep the three front buttons pressed until the screen displays the `OS ERROR` message.
    - You may need to remove the batteries and put them back in in order for this to work
    - There have been reports that adding `Shift` to the above key combination may work to get to the OS Error screen if the normal key combo doesn't work
  - At this point, you can download the latest official updater from CASIO and use it to update your calculator. Updating to 2.01.7002 is confirmed to make the calculator work again.
- No matter what, by installing hollyhock, upgrading or downgrading the OS of your calculator or even just following the above procedures to check the ABS_Date, you do so at your own risk. YOU AGREE TO TAKE FULL RESPONSIBILITY FOR ANY DAMAGE THAT MAY OCCUR.
- It would be really helpful if you could submit your calculators serial number and ABS_Date (on the comments field) to [MyCalcs](https://my.calcs.quest/), specially if it's one of the 2022 ABS_Date models (but actually no matter your model), and let us know!
  

# Installation (simple)
This is how you install hollyhock-2 onto your calculator:
- check the compatibility section above to ensure your calculator is compatible
- download the newest prebuild release of hollyhock-2: https://github.com/SnailMath/hollyhock-2/releases
- copy the folder Snail2021 from the .zip file in the release onto the desktop.
- open the folder Snail2021 on the desktor and double click on Snail2021.exe
- follow the instructions on screen:
  - Download and install ResourceHacker (scroll down, click on _EXE install_): http://angusj.com/resourcehacker/
  - Download the official updater version 02.01.2 (click the green download button): https://tiplanet.org/forum/archives_voir.php?id=1044960
  - Follow the instructions in the updater, but don't connect the calculator when it asks you to.
- at this point, return back to the first windown (The Snail2021.exe)
- click enter until it asks you to connect the calculator.
- You can close the updater window now.
- When you click enter the last time in the Snail2021.exe window, the updater will open again, but this time it is the modified version.
- Connect the calculator via USB.
- On the calculator hold down the `EXP`, `^` and `Clear` keys. With those keys pressed, momentarily press the RESET button on the back of the calculator. Keep the three front buttons pressed until the screen displays the `OS ERROR` message. Don't worry, there is no actual error, this is just the update mode.
- On the computer, Click on OK in the updater. 
- __DO NOT TOUCH__ the calculator until the update is finished.
- When the updater is finished, unplug it and plug it in again
- Select USB Storage and copy the file run.bin and all .hhk and .bin files (from the folder Snail2021) directly onto the calculator.
- On the computer, click on eject (probably on the bottom right) and eject the calculator.

To start a program, go to the Menu, go to the right, select System, click on the gear on the top left and click on `Imaginary Unit` (I will probably rename this to 'Start hollyhock' or 'Start program' soon.)

Select the program and click OK.


# hollyhock
An SDK to write your own applications for the CASIO fx-CP400 graphics calculator.

This is by no means a finished project. Like the disclaimer says, use at your own risk! Breaking changes are not out of the question, and should be 100% expected.

I (@The6P4C) have burned out *hard* on this project, and won't be maintaining it any further. These tools, patches and SDK work only for OS v02.01.2000.0000. The tools are easily updated, but the patches and SDK are **much** more difficult (and half the reason I'm so burned out).

~~Please contact @ps100000 regarding this project.~~ If you have any questions, you can visit our discord https://discord.gg/y8Mr8tD or comment on YouTube.

## User Guides
1. [Compatibility](#Compatibility)
2. [Install hollyhock-2](#installation-simple)
3. [Using add-on software](doc/user/using.md)
4. [Developing applications](doc/user/developing.md)
5. [Writing programs without a PC](doc/user/exec.md)
6. [Patching your firmware (from source)](doc/user/patching.md) (complicated, you do not need this, just scroll up to section [Install](#installation-simple)

