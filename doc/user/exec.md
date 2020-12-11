# Writing programs without a PC
You can write and execute programs on the calculator without the use of a pc. 
This can be useful if you just want to try something out really fast or it can also be fun to type out a lot of hex values by hand (if you are in to that sort of thing...).

## 1. Create the folder hhk
Hollyhock-2 only searches for variables in the folder hhk, so you have to create it if you haven't already.
- go to `Main` or `Program`
- click on the gear icon in the top left
- click on 'Variable Manager`
- click `Edit`
- click `Create Folder`
- type in `hhk`
- click OK
Now you've created the folder hhk.

## 2. Create a program variable.
You need to create a variable for the program.
- In the Menu, go to Program
- In the dropdown `Folder` select `hhk`
- click on `Edit` -> `New File` __or__ click the 2nd logo with the blank page near the top of the screen
- type in any name you wish (8 characters max)
- in the dropdown `Type` select `Program(Text)`
- click OK

## 3. Give the program a name
You can define a name for your program, a description, the author's name and a version number. To do this, write a `'` at the beginning of the first four lines and
add the info directly after that like this:
```
'Example Program
'This is the program description
'SnailMath
'1.0.1
```
Replace the program name, description, author and version number with your own. I would suggest using your GitHub username as the author's name.

Click on the icon with the floppy disk near the top of the screen to save the changes. 
(The file is only saved in RAM until you turn the calculator off and on again, so I would turn it off and on again after clicking on the floppy icon, by pressing shift and clear and than clear again.)

## 4. Writing the program
Now you can write a program. You can find Info about the SH4 Processor here:
[SH4 Datasheet pdf](https://www.google.com/search?q=%22sh-4-32-bit-cpu-core-architecture-stmicroelectronics.pdf%22+site%3Ast.com)

Just a few infos about the processor, I think this will help:
- The SH4 CPU is a 32 bit CPU
- 16 registers (r0 to r15)
- r15 is used as a stack pointer (so r0-r14 remain)
- r8-r15 are backed up during subroutine calles
- r0-r7 are lost during subroutine calls
- functions or subroutines need the arguments in r4, r5, r6 and r7 (if there are more, they need to go on the stack.)
- functions return their value in r0
- the sh4 uses a delay slot. You don't know what that means? Just use a nop (no operation) after every jump, subroutine call or return.
(The instruction directly following a delayed jump will be executed before the actual jump, even so it is written after the jump. If this is a `nop` this does not matter.)
- there is a register called pr which holds the return address, if you want to call subroutines yourself, you need to back this up and resore this, see the following example.

__"Hello World" example__

First I start with the rough structure. The text after `//` is a comment, but hollyhock will ignore everything after the `'`
```
'nop //do nothing
'nop //do nothing

'rts //return subroutine
'nop /this is after a jump (the rts in this case), so we need to put a nop here.
```
This program does nothing and will return immediately. 

Before we can run this, we need to translate this assembly code in hexadecimal machine code. You can either remember everything in the user manual or print out page 164 - 174.

The values in the manual are binary, we just need to convert it to hexadecimal. According to the manual, `nop` is `0000000000001001`. This is `0009` in hex. 
`rts` is `0000000000001011`, which is `000B`. So let's add that to our code.
```
0009 'nop //do nothing
0009 'nop //do nothing

000B 'rts //return subroutine
0009 'nop /this is after a jump (the rts in this case), so we need to put a nop here.
```
Now we can do the very first test. Save it (using the floppy icon), turn the calculator off and on again (shift clear,  clear) and go to the menu, go to `System`, 
click on the gear in the top left corner, click on `Hollyhock-2 Launcher` and select your program in the dropdown menu. Click on Run. Because your program returns
immiately, you should see the message `The program has finished execution` immidiatly.

TO BE CONTINUED



Because I didn't wrote an assembler, you have to do that yourself.

Values in hexadecimal (0x8CFF0000)
