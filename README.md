
Ryan Hooley, Phil Stenger, Ryan Hesse

SUPER SMASH BROS.

Link to Git repository: https://github.com/pstenger/Final-Project

USER MANUAL

Section 1: (Special libraries needed)

SDL2

SDL2_image

SDL2_ttf

Section 2: (compiling and running the code)

To compile the code on a student machine:

1.Type these two lines into the command line:


setenv PATH /afs/nd.edu/user14/csesoft/new/bin:$PATH

setenv LD_LIBRARY_PATH/afs/nd.edu/user14/csesoft/new/lib:$LD_LIBRARY_PATH


2.  Type “make” to compile the program with the makefile.


3.  Type “main” to run the executable “main” which has been created by running the makefile in Step 2. The code should now run, with the character selection menu 
coming up first

    4. When the game is over, or when you want to quit, exit out of the game window.

Section 3: How to play


Step one: pick your fighters.

    The Character Selection screen will pop up first, here each player chooses a character with player 1’s choices appearing on the left and player 2’s on the right 
using the mouse to click in the correct square. When you mouse over a character, an hd image of the selected character will show up. Once both players have clicked 
on a character, the battle will launch. 


Step two: Battle

The objective of the game is to knock the other player off of the stage 3 times. Do this by hitting them with either short or long ranged attacks that knock them 
backwards. 

On the bottom left and right there are color coded numbers keeping count of HP. A player’s HP starts at 0 every time they spawn, and rises when they are hit. The 
higher a player’s HP, the further back attacks will knock them, making it easier to be hit off the stage. Once one player is knocked off the stage 3 times, the 
losing player takes a shameful stance, while the winner strikes a victory pose. Being in the crouch position allows a character to block any incoming attack.







Player 1 controls: 

A : move left

D: move right

W: jump

S: crouch

G: use close range attack

H: use projectile attack


Player 2 controls: 

Left arrow: move left

Right arrow: move right

Up arrow: jump

Down arrow: crouch

, key: use close range attack

. key: use projectile attack


Section 4: Known Bugs

1.In the character selection menu, if the mouse is started on one of the pictures when the program starts it will show up as mario until the mouse is removed from the picture


2.The characters and score flash on the screen when they are refreshed


3.The the running animation does not take effect if you hold down a side button while in the air. To get the walk animation back you need to release the key and then press it down again.


4.Projectiles disappear momentarily when collide with characters.


5.Characters shift position slightly when they jump (this is the result of different sized jump sprites).


6.Pressing side arrows on winning screen makes characters change the way they are facing.


7.Occasional movement glitches


8.If a player jumps back towards the stage after falling off, they could potentially stop their fall by entering the stage from the side and then being stuck at a lower y position than they should be.


9.First animation a player does will be executed more quickly than usual.


10.Occasional hit detection issues (sometimes hits or doesn’t hit when it should/shouldn’t)




Section 5: Sprite/Image Resources

1.Background/Stage image : http://smashbros-miiverse.com/stages/final-destination-wii-u
2.True Type Font file: http://www.superluigibros.com/mario-bros-fonts
3. Pacman Sprites: lisnovski.deviantart.com
4. Megaman, Link, and Mario Sprites: gengarlink10.deviantart.com





