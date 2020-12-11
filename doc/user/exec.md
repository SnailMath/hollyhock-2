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

Before we can run this, we need to translate this assembly code in hexadecimal machine code. You can either remember everything in the user manual or [print out page 164 - 174](https://www.google.com/search?q=%22sh-4-32-bit-cpu-core-architecture-stmicroelectronics.pdf%22+site%3Ast.com).

The values in the manual are binary, we just need to convert it to hexadecimal. According to the manual, `nop` is `0000000000001001`. This is `0009` in hex. 
`rts` is `0000000000001011`, which is `000B`. So let's add that to our code.
```
0009 'nop //do nothing
0009 'nop //do nothing

000B 'rts //return subroutine
0009 'nop //this is after a jump (the rts in this case), so we need to put a nop here.
```
Now we can do the very first test. Save it (using the floppy icon), turn the calculator off and on again (shift clear,  clear) and go to the menu, go to `System`, 
click on the gear in the top left corner, click on `Hollyhock-2 Launcher` and select your program in the dropdown menu. Click on Run. Because your program returns
immiately, you should see the message `The program has finished execution` immidiatly. (If it locks up, you typed something wrong. Hit reset on the back or take out and reinsert the batteries, fix the error and try again.)

Now we add the backing up of important registers. If we want to use register r8-r14, we would need to back them up. Because we want to call subroutines, we need
to back up the process register `pr`. This is a system register used by jsr to store the return address, that is retrieved by rts. Let's say, we also want to use r8 and r9, so I will back them up as well. All registers are restored at the end, make sure to restore them in the opposite order of backing them up.
```
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9
4F22 'sts.l pr, @-r15 //Back up pr

0009 'nop //Main code

4F26 'lds.l @r15+, pr //Restore pr
69F6 'mov.l @r15+, r9 //Restore r9
68F6 'mov.l @r15+, r8 //Restore r8
000B 'rts //return subroutine
0009 'nop //delay slot
```
At this point I want to introduce you to the delay slot. The instruction __after__ a _delayed branch_ (see datasheet) gets executed __before__ the branch is taken.
So to make things simpler, we could move the `mov.l @r15+, r8` after the `rts` into the delay slot, it will still get executed before the jump is taken.
The example below will do exactly the same as the example above.
```
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9
4F22 'sts.l pr, @-r15 //Back up pr

0009 'nop //Main code

4F26 'lds.l @r15+, pr //Restore pr
69F6 'mov.l @r15+, r9 //Restore r9
000B 'rts //return subroutine
68F6 'mov.l @r15+, r8 //Restore r8 //delay slot (gets executed before the return)
```
Now it would be great, if we could do something else than just backing up and restoring registers. Print text for example.
The simplest way is the function `Debug_PrintString(const char *string, bool invert)`. It is simpler than it looks. 

We need to do three things:
- Set the cursor using the function `Debug_SetCursorPosition(int x, int y)`
- Print the text using the function `Debug_PrintString(const char *string, bool invert)`
- Make the change on the screen visible using the function `LCD_Refresh()`
We need to load the address of these functions in a register before we can call `jsr`. Because all addresses are 32 bit, we can't load this with the `mov #imm, rn` instruction, we have to use `mov.l` to load the value from a different position. Just look at the following code, it should make sense:
```
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9

4F22 'sts.l pr, @-r15 //Back up pr
0009 'nop 


     ' //Main code
D802 'mov.l setCursor, r8   ------------+
D903 'mov.l printString, r9 ---------+  |
                                     '  |
                                     '  |
4F26 'lds.l @r15+, pr //Restore pr   |  |
69F6 'mov.l @r15+, r9 //Restore r9   |  |
                                     '  |
000B 'rts //return subroutine        |  |
68F6 'mov.l @r15+, r8 //Restore r8   |  |
                                     '  |
'setCursor:                          |  |
8002E430 '<--------------------------|--+
'printString:                        |
8002DA0C '<--------------------------+

```
The addresses after the code are behind the return instruction, so the CPU doesn't try to execute data. The `mov.l` instructions need the correct offset
so the processor knows where the data is. I would suggest grouping everything in groups of 32 bits, so 2 instructions or 1 address. This makes it easyer to calculate the offset. Start counting the groupd after the mov.l instruction until you reach the data. Start with 0. So 00 is the group `lds.l ...`, 01 is `rts...`, 02 is `setCursor: 8002E430` and 03 is `printString: 8002DA0C`. Make sure to calculate this new every time you cange something.

One thing: Where did I get the addresses from? Well, it just happens that I know these. But to make this simpler, you can replace them with `#S` and `#PS`. The launcher will replace this automatically while loading. [Here](functions.md) is a full list of all the functions you can replace with this short combinations.

The next thing is to actually call the functions. But before we do that, we have to give arguments to them. Let's say, we want to print to position (0,1), so we
have to put 0 in r4 and 1 in r5 before calling setCursor:
```
E400 'mov #0, r4 //x=0
E500 'mov #1, r5 //y=1
480B 'jsr @r8 //setCursor is in r8
0009 'nop //(delay slot)

```
The arguments for the print are a little more complicated. We need a text to print. We can put that after the addresses between `"` so the Launcher copies it directly. Because strings need to be terminated with 0x00 we have to add `00` after the text. To load the address of the text, we have to use `mova text, r0` and `mov r0, r4` because we can only mova in r0, not in r4. We will give printString 0 as the argument in r5 using `mov #0, r5`, we could use #1 to invert the text color. 
```
     ' //back up everything
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9

4F22 'sts.l pr, @-r15 //Back up pr
0009 'nop 


     ' //Load subroutine addresses
D806 'mov.l setCursor, r8   ------------+
D907 'mov.l printString, r9 ---------+  |  //Make sure to recalculate these offsets!!!
                                     '  |
                                     '  |
     ' //Set the Cursor to (0,1)     |  |
E400 'mov #0, r4 //x=0               |  |
E501 'mov #1, r5 //y=1               |  |
                                     '  |
480B 'jsr @r8 //setCursor is in r8   |  |
0009 'nop //(delay slot)             |  |
                                     '  |
                                     '  |
     ' //Print the text              |  |
C706 'mova text, r0 -----------------------+
6403 'mov r0, r4                     |  |  |
   'The addr ofthe text is now in r4 |  |  |
                                     '  |  |
490B 'jsr @r9 //print is in r9       |  |  |
E500 'mov #0, r5 //0 in r5 (this gets executed before the jsr, because it is in a delay slot)
                                     '  |  |
                                     '  |  |
     ' //restore everything          |  |  |
4F26 'lds.l @r15+, pr //Restore pr   |  |  |
69F6 'mov.l @r15+, r9 //Restore r9   |  |  |
                                     '  |  |
000B 'rts //return subroutine        |  |  |
68F6 'mov.l @r15+, r8 //Restore r8   |  |  |
                                     '  |  |
'setCursor:                          |  |  |
#S '<--------------------------------|--+  | //You can use these symbols so you don't need to remember the long addresses.
'printString:                        |     |
#PS '<-------------------------------+     |
'text:                                     |
"Hello World" 00 '<------------------------+
```
This program runs and exits immidiately, but no text is on the screen. This is because the print subroutine changes only the content of the VRAM ant this has to
be sent to the screen, before we can see it. This is what the function `LCD_Refresh()` does. We can use `#R` instead of the address `8003733E`.
So we just need to call `LCD_Refresh()` after the `printStirng`.
```
     ' //back up everything
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9

4F22 'sts.l pr, @-r15 //Back up pr
0009 'nop 


     ' //Load subroutine addresses
D808 'mov.l setCursor, r8   ------------+
D909 'mov.l printString, r9 ---------+  |  //Make sure to recalculate these offsets!!!
                                     '  |
                                     '  |
     ' //Set the Cursor to (0,1)     |  |
E400 'mov #0, r4 //x=0               |  |
E501 'mov #1, r5 //y=1               |  |
                                     '  |
480B 'jsr @r8 //setCursor is in r8   |  |
0009 'nop //(delay slot)             |  |
                                     '  |
                                     '  |
     ' //Print the text              |  |
C708 'mova text, r0 -----------------------+
6403 'mov r0, r4                     |  |  |
   'The addr ofthe text is now in r4 |  |  |
                                     '  |  |
490B 'jsr @r9 //print is in r9       |  |  |
E500 'mov #0, r5 //0 in r5 (this gets executed before the jsr, because it is in a delay slot)
                                     '  |  |
                                     '  |  |
     ' //Refresh the LCD             |  |  |
D205 'mov.l refresh, r2 ----------------------+
420B 'jsr @r2                        |  |  |  |
                                     '  |  |  |
0009 'nop (delay slot)               |  |  |  |
0009 'nop (make it an even number of instructions)
                                     '  |  |  |
                                     '  |  |  |
     ' //restore everything          |  |  |  |
4F26 'lds.l @r15+, pr //Restore pr   |  |  |  |
69F6 'mov.l @r15+, r9 //Restore r9   |  |  |  |
                                     '  |  |  |
000B 'rts //return subroutine        |  |  |  |
68F6 'mov.l @r15+, r8 //Restore r8   |  |  |  |
                                     '  |  |  |
'setCursor:                          |  |  |  |
#S '<--------------------------------|--+  |  | //You can use these symbols so you don't need to remember the long addresses.
'printString:                        |     |  |
#PS '<-------------------------------+     |  |
'refresh:                                  |  |
#R '<--------------------------------------|--+
'text:                                     |
"Hello World" 00 '<------------------------+
```
Mayby you are wondering why I stored setCursor and printString in r8 and r9, which we had to back up first. Well, r8-r15 are preserved during function calls,
so we can call the subroutine multiple times without needing to use mov.l multiple times. As an example, I will add a second text at position 2,15 with inverted colors. I will also add the hollyhock description at the beginning:
```
'Hello World
'This is a simple Hello World program
'SnailMath
'1.0.0

     ' //back up everything
2F86 'mov.l r8, @-r15 //Back up r8
2F96 'mov.l r9, @-r15 //Back up r9

4F22 'sts.l pr, @-r15 //Back up pr
0009 'nop 


     ' //Load subroutine addresses
D80C 'mov.l setCursor, r8
D90D 'mov.l printString, r9 //Make sure to recalculate these offsets when you change something!!!


     ' //Set the Cursor to (0,1)
E400 'mov #0, r4 //x=0
E501 'mov #1, r5 //y=1

480B 'jsr @r8 //setCursor is in r8
0009 'nop //(delay slot)


     ' //Print the text
C70C 'mova text, r0
6403 'mov r0, r4 //The addr ofthe text is now in r4

490B 'jsr @r9 //print is in r9
E500 'mov #0, r5 //0 in r5 (this gets executed before the jsr, because it is in a delay slot)


     ' //Set the Cursor to (2,15)
E402 'mov  #2, r4 //x=2
E50F 'mov #15, r5 //y=15=0x0F

480B 'jsr @r8 //setCursor is in r8
0009 'nop //(delay slot)


     ' //Print the other text
C70B 'mova text2, r0
6403 'mov r0, r4 //The addr ofthe text is now in r4

490B 'jsr @r9 //print is in r9
E501 'mov #1, r5 //1 in r5 (invert color) (this gets executed before the jsr, because it is in a delay slot)



     ' //Refresh the LCD
D205 'mov.l refresh, r2
420B 'jsr @r2

0009 'nop (delay slot)
0009 'nop (make it an even number of instructions)


     ' //restore everything
4F26 'lds.l @r15+, pr //Restore pr
69F6 'mov.l @r15+, r9 //Restore r9

000B 'rts //return subroutine
68F6 'mov.l @r15+, r8 //Restore r8

'setCursor:
  #S
'printString:
  #PS
'refresh:
  #R
'text:
  "Hell"
  "o Wo"
  "rld"00
'text2: 
  "Snai"
  "lMat"
  "h!"0000
```
When you start this Program in the Hollyhock-2 Launcher, it is copied to RAM at address 0x8CFF0000 and it gets executed as a subroutine.

Just type it into your calculator and try it out yourself. Can you add a call to #W to wait until the clear key is pressed?
