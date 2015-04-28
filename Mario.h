//This is the derived class for the character mario
#ifndef MARIO_H
#define MARIO_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"  //base class
#include <stdio.h>
#include <iostream>
using namespace std;

class Mario: public Character {
	public:
  		Mario(int player);
  		~Mario();
  		//Loads media
  		void loadMedia(SDL_Renderer*, string);
  		//called by LoadMedia to load specified texture
  		int display(SDL_Renderer*, int, int ); //runs funcs to display image and returns true if projectile thrown (used in main)
		SDL_Rect get_location(int);
		int attacking();  //returns true if character is attacking
		void damage(int, string);  //applies appropriate damage
		void win(SDL_Renderer*);
                void lost(SDL_Renderer*);
      
        private:     
  		string stand; //variables represent file names for each animation/state
		string crouch;
  		string attack1[8]; 
  		string attack;
		string jumpanim; //jump animation
  		string walk[8];
  		string attack2[8];
		SDL_Rect location;  //character's body
		SDL_Rect fist;  //character's fist
      
		double time; //keeps track of in-game time
		string damaged[8];  //damage animation
		bool range; //whether or not ranged attack has been launched
	       
};

Mario::Mario (int player) : Character(player){
     
	range=0;
  	//initializes file variables to appropriate file names
	stand="mario.png";
 	attack1[1]="marioattack1_1.png";
  	attack1[0]="marioattack1_2.png";
  	attack1[2]="marioattack1_3.png";
  	attack1[3]="marioattack1_4.png";
  	attack1[4]="marioattack1_5.png";
	attack1[5]="marioattack1_6.png";
	attack1[6]="marioattack1_7.png";
	attack1[7]="marioattack1_8.png";
	attack1[8]="marioattack1_9.png";
	attack1[9]="marioattack1_10.png";
  	jumpanim="mariojump.png";
  	walk[0]="mariowalk1.png";
  	walk[1]="mariowalk2.png";
  	walk[2]="mariowalk3.png";
  	walk[3]="mariowalk4.png";
  	walk[4]="mariowalk5.png";
  	walk[5]="mariowalk6.png";
  	walk[6]="mariowalk7.png";
  	walk[7]="mariowalk8.png";
	crouch="mariocrouch.png";
	damaged[0]="mariodmg1.png";
	damaged[1]="mariodmg2.png";
	damaged[2]="mariodmg3.png";
	damaged[3]="mariodmg4.png";   
	damaged[4]="mariodmg5.png";
	damaged[5]="mariodmg6.png";
	damaged[6]="mariodmg7.png";
	damaged[7]="mariodmg8.png";
	attack2[0]="mariothrow1.png";
	attack2[1]="mariothrow2.png";
 	attack2[2]="mariothrow3.png";
 	attack2[3]="mariothrow4.png";
 	attack2[4]="mariothrow5.png";
 	attack2[5]="mariothrow6.png";
 	attack2[6]="mariothrow7.png";
 	attack2[7]="mariothrow8.png";
	time=0;
}

Mario::~Mario(){
  	free();
}

SDL_Rect Mario::get_location(int x){
  if(x==0){ //if x is 0 use the rectangle if mario's body
    return (location);
  } else {
    return (fist);  //otherwise use rectangle of his fist
  }
}

void Mario::loadMedia(SDL_Renderer* gRenderer, string filename){

        gImage=loadTexture(filename, gRenderer); //call load texture with appropriate image name
  
}
void Mario::win(SDL_Renderer* gRenderer){
  SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight};
  loadMedia(gRenderer, "mariowin.png");
  SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
}
void Mario::lost(SDL_Renderer* gRenderer){
  SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight};
  loadMedia(gRenderer, "mariolost.png");
  SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
}


int Mario::display (SDL_Renderer* gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT ){
  move(SCREEN_WIDTH, SCREEN_HEIGHT);    //update x and y positions
  location.x=xpos+20;// set hit box for character
  location.y=ypos+20-jump;
  location.w=mWidth-30;
  location.h=mHeight-30;
  if(flip==SDL_FLIP_NONE){//set hitbox for fist. dependent on the way character is facing
    fist.x=xpos+30;
    fist.y=ypos+20-jump;
    fist.w=mWidth-40;
    fist.h=mHeight-40;
  } else if(flip==SDL_FLIP_HORIZONTAL){
    fist.x=xpos+10;
    fist.y=ypos+20-jump;
    fist.w=mWidth-40;
    fist.h=mHeight-40;
  }
  SDL_Rect renderQuad = { xpos, ypos-jump-hity, mWidth, mHeight};

  //calculate jump values to keep track of position while in the air
  if(jump>0 && anim!="Damage"){
    if(!buttonpress && !buttonpress2){//once attack animations stop
      anim="Jump";
    }
    t++;
    jump=10*t-.3*t*t;
    if(jump<0){//if next step takes below 0
      jump=0;
    }
  }

  if(buttonpress==0 && jump==0 && anim!="Walk" && anim!="Damage" && buttonpress2==0 && anim!="Crouch" && anim!="Respawn"){
    anim="Stand"; //default state
  }
  if (anim=="Stand"){
    loadMedia(gRenderer, stand);
  } else if(anim=="Attack1"){
    if(frame>18){
      frame=0;
      buttonpress=0;
    }

    loadMedia(gRenderer, attack1[frame/2]);
    frame++;
  } else if(anim=="Damage"){
    if(frame>4){
      frame=0;
    }
    loadMedia(gRenderer, damaged[frame/2]);
    if(frame==0 || frame%4!=0){//stop animation as mario is lieing down
      frame++;
    }

    t++;
    hity=3*t-.3*t*t;  //knock into air with damage
    if(hity<=0){//if hit goes below zero in an iteration
      hity=0;
      xvel=0;
      frame=0;
      anim="Stand";
      t=0;
      jump=0;
    }
  } else if(anim=="Crouch"){
    loadMedia(gRenderer, crouch);
  }else if (anim=="Walk"){
    if(frame>14){
      frame=0;
    }

    loadMedia(gRenderer, walk[frame/2]);
    frame++;
  } else if (anim=="Jump"){
    loadMedia(gRenderer, jumpanim);
  } else if (anim=="Throw"){
    if(frame>14){
      frame=0;
      range=1;  //end of animation so projectile launched
      buttonpress2=0; //notifies key presses that other actions can now happen
    }
 
   loadMedia(gRenderer, attack2[frame/2]);
    frame++;
  } else if(anim=="Respawn"){
    ypos=230;
    xpos=320;
    health=0;
    t++;
    jump=150-.3*t*t;
    if(jump<=0){//if next step takes below 0
      jump=0;
      anim="Stand";
    }
  }

  time=time+.25;


  //render the image with appropriate flip and no rotation
  SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
  //Update the surface
  SDL_RenderPresent( gRenderer );
  if(range){
    range=0;
    return 1;
  } else {
    return 0;
  }

}
			
#endif
