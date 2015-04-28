//This is the derived class for the character mario
#ifndef MEGAMAN_H
#define MEGAMAN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"  //base class
#include <stdio.h>
#include <iostream>
using namespace std;

class Megaman: public Character {
	public:
  		Megaman(int player);
  		~Megaman();
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
  		string attack1[6]; 
  		string attack;
		string jumpanim; //jump animation
  		string walk[10];
  		string attack2[8];
		SDL_Rect location;  //character's body
		SDL_Rect fist;  //character's fist
      
		double time; //keeps track of in-game time
		string damaged[8];  //damage animation
		bool range; //whether or not ranged attack has been launched
	       
};

Megaman::Megaman (int player) : Character(player){
     
	range=0;
  	//initializes file variables to appropriate file names
	stand="megaman.png";
 	attack1[1]="megamanattack1_1.png";
  	attack1[0]="megamanattack1_2.png";
  	attack1[2]="megamanattack1_3.png";
  	attack1[3]="megamanattack1_4.png";
  	attack1[4]="megamanattack1_5.png";
	attack1[5]="megamanattack1_6.png";
  	jumpanim="megamanjump.png";
  	walk[0]="megamanwalk1.png";
  	walk[1]="megamanwalk2.png";
  	walk[2]="megamanwalk3.png";
  	walk[3]="megamanwalk4.png";
  	walk[4]="megamanwalk5.png";
  	walk[5]="megamanwalk6.png";
  	walk[6]="megamanwalk7.png";
  	walk[7]="megamanwalk8.png";
	walk[8]="megamanwalk9.png";
	walk[9]="megamanwalk10.png";
	crouch="megamancrouch.png";
	damaged[0]="megamandmg1.png";
	damaged[1]="megamandmg2.png";
	damaged[2]="megamandmg3.png";
	damaged[3]="megamandmg4.png";   
	damaged[4]="megamandmg5.png";
	damaged[5]="megamandmg6.png";
	damaged[6]="megamandmg7.png";
	damaged[7]="megamandmg8.png";
	attack2[0]="megamanthrow1.png";
	attack2[1]="megamanthrow2.png";
 	attack2[2]="megamanthrow3.png";
 	attack2[3]="megamanthrow4.png";
 	attack2[4]="megamanthrow5.png";
 	attack2[5]="megamanthrow6.png";
 	attack2[6]="megamanthrow7.png";
 	attack2[7]="megamanthrow8.png";
	time=0;
	if(player==2){  //sets beginning position of character (overrides base class default bc megaman sprite size different)
          xpos=90;
          ypos=200;
      
        } else {
          xpos=400;
          ypos=200;
	  flip=SDL_FLIP_HORIZONTAL;
        }

}

Megaman::~Megaman(){
  	free();
}

SDL_Rect Megaman::get_location(int x){
  if(x==0){ //if x is 0 use the rectangle if mario's body
    return (location);
  } else {
    return (fist);  //otherwise use rectangle of his fist
  }
}

void Megaman::loadMedia(SDL_Renderer* gRenderer, string filename){

        gImage=loadTexture(filename, gRenderer); //call load texture with appropriate image name
  
}
void Megaman::win(SDL_Renderer* gRenderer){
  SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight};
  loadMedia(gRenderer, "megamanwin.png");
  SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
}
void Megaman::lost(SDL_Renderer* gRenderer){
  SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight};
  loadMedia(gRenderer, "megamanlost.png");
  SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
}

int Megaman::display (SDL_Renderer* gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT ){
  move(SCREEN_WIDTH, SCREEN_HEIGHT);    //update x and y positions
  location.x=xpos+30;// set hit box for character
  location.y=ypos+30-jump;
  location.w=mWidth-40;
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
    fist.h=mHeight-20;
  }
  SDL_Rect renderQuad = { xpos, ypos-jump-hity, mWidth, mHeight};

  //calculate jump values to keep track of position while in the air
  if(jump>0 && anim!="Damage"){
    if(!buttonpress && !buttonpress2 && anim!="Respawn"){//once attack animations stop
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
    if(frame>10){
      frame=0;
      buttonpress=0;
    }
    loadMedia(gRenderer, attack1[frame/2]);
    frame++;
  } else if(anim=="Damage"){
    if(frame>2){
      frame=0;
    }
    loadMedia(gRenderer, damaged[frame/2]);
    if(frame==0 || frame%2!=0){//stop animation as mario is lieing down
      frame++;
    }

    t++;
    hity=3*t-.3*t*t;  //knock into air with damage
    if(hity<=0){//if hit goes below zero in an iteration
      hity=0;
      xvel=0;
      frame=0;
      anim="Stand";
      jump=0;
      t=0;
    }
  } else if(anim=="Crouch"){
    loadMedia(gRenderer, crouch);
  }else if (anim=="Walk"){
    if(frame>18){
      frame=0;
    }    
    loadMedia(gRenderer, walk[frame/2]);
    
    frame++;
    
  } else if (anim=="Jump"){
    loadMedia(gRenderer, jumpanim);
  } else if (anim=="Throw"){
    if(frame==8){
      range=1;
    }
    if(frame>14){
      frame=0;
      buttonpress2=0; //notifies key presses that other actions can now happen
    }
    loadMedia(gRenderer, attack2[frame/2]);
    frame++;

  } else if(anim=="Respawn"){
    ypos=200;
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
