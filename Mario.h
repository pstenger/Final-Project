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
  		Mario(int player=1);
  		~Mario();
  		static const int img_vel=6;
  		//Loads media
  		void loadMedia(SDL_Renderer*, string);
  		//called by LoadMedia to load specified texture
  		SDL_Texture* loadTexture( string path, SDL_Renderer* );
  		int display(SDL_Renderer*, int, int ); //runs funcs to display image and returns true if projectile thrown (used in main)
 		void handleevent(SDL_Event& e, SDL_Renderer*);  //checks key presses
  		void move(int, int);  //calculates xpos and ypos from xvel and yvel
 		void free();  
  		int get_xpos();
  		int get_ypos();
  		int get_width(); //return width of image
  		int get_height(); //return height of image
		SDL_Rect get_location(int);
		int attacking();  //returns true if character is attacking
		void damage(int, string);  //applies appropriate damage
		string facing(); //returns the direction the character is facing for reference on ranged attacks
        private: 
  		int xvel, yvel;
  		int xpos, ypos;
  		int mWidth; //width of image
  		int mHeight; //height of image
  		SDL_Texture* mTexture;
  		double jump, jump2; //keeps track of jump and midair jump
  		double t, t2;   //timer variables for jump
  		int controls; //1 for player 1 controls and 2 for player 2 controls
  		SDL_RendererFlip flip;
  		string stand; //variables represent file names for each animation/state
  		string anim;
		string crouch;
  		string attack1[8]; 
  		string attack;
		string jumpanim; //jump animation
  		string walk[8];
  		string attack2[8];
		int frame;
		SDL_Rect location;  //character's body
		SDL_Rect fist;  //character's fist
  		bool buttonpress, buttonpress2; //keeps track of whether either attack button has been pressed
		int health;
		double time; //keeps track of in-game time
		string damaged[8];  //damage animation
		double hity, hitx; //knocks character into air if damaged
		bool range; //whether or not ranged attack has been launched
};

Mario::Mario (int player){
        anim= "Stand"; //beginning animation is just standing
 	xvel =0;
  	yvel=0;
  	xpos=85;
  	ypos=230;
  	mWidth=0;
  	mHeight=0;
  	mTexture=NULL;
  	jump=0;
  	t=0;
  	t2=0;
  	jump2=0;
	hity=0;
	hitx=0;
  	controls=player;
  	flip=SDL_FLIP_NONE;
  	frame=0;
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
	health=0;
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
  	buttonpress=0; //keeps track of whether attack button is down or not
	buttonpress2=0; //keeps track of whether throw attack button is down
	time=0;
	if(player==1){  //sets beginning position of character
	xpos=85;
        ypos=230;
        } else {
             xpos=285;
             ypos=230;
	     flip=SDL_FLIP_HORIZONTAL;
	}
}

Mario::~Mario(){
  	free();
}

int Mario::get_width(){
 	 return(mWidth);
}
int Mario::get_height(){
  	return(mHeight);
}

int Mario::get_xpos(){
  	return(xpos);
}

int Mario::get_ypos(){
  	return(ypos);
}

string Mario::facing(){
  if (flip==SDL_FLIP_NONE){
    return("right");
  } else {
    return("left");
  }
}
void Mario::damage(int style, string direction){ //style takes in type of attack and direction takes in which direction attack is from
  if(anim!="Crouch"){//crouch is block equivalent so can't be damaged
    if (style==1){ //melee
      health=health+1;
    } else if (style==2){ //ranged
      health=health+2;
    }
    if(direction=="left"){
      xvel=-5; //throws player some distance
    } else {
      xvel=5;
    }
    cout<<health<<endl;
    anim="Damage"; //invokes damage animation
    hity=1; //allows to go into air when hit
    
    t=0;  //t used to represent time in equation get character's position in air
  }
}
int Mario::attacking(){
  if(buttonpress==1){  //used in main to determine if character is in state of attack
    return 1;
  } else {
    return 0;
  }
}
SDL_Rect Mario::get_location(int x){
  if(x==0){ //if x is 0 use the rectangle if mario's body
    return (location);
  } else {
    return (fist);  //otherwise use rectangle of his fist
  }
}
void Mario::handleevent(SDL_Event& e, SDL_Renderer* gRenderer){
  //make sure event is key press
  	if(controls==1 && anim!="BigHit"){ //if player 1
    		if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
      		
      			switch( e.key.keysym.sym ){
        			case SDLK_UP: 
				        if(!buttonpress && !buttonpress2){ //attack animations should continue while jumping
				                anim="Jump";
				        }
				  	if(jump==0){//can't jump if already in the air. jump is added to ypos
					        jump=1; //allows jump later in program
	    					t=0; //resets t
	  		  		}
	  			  	break;
        			case SDLK_DOWN:
				        anim="Crouch";
				        break;
				case SDLK_LEFT: 
				        if(!buttonpress && anim!="Damage"){ //can't move out of damage animation or attack animation
	    					xvel -= img_vel; 
	  				}
	  				if(flip==SDL_FLIP_NONE){
					        flip=SDL_FLIP_HORIZONTAL; //change direction character is facing
	  				}
	  				if(jump<=0 && !buttonpress && anim!="Damage" && !buttonpress2){
					        anim="Walk";  //walk animation if not in air
	  				}
	  				break;
				case SDLK_RIGHT: 
	  				if(!buttonpress && anim!="Damage"){   
	    					xvel += img_vel; 
	  				}
	  				if(flip==SDL_FLIP_HORIZONTAL){
	    					flip=SDL_FLIP_NONE;
	  				}
	  				if(jump<=0 && !buttonpress && anim!="Damage" && !buttonpress2 ){
	    					anim="Walk";
	  				}
	  				break;
        			case SDLK_COMMA:
	  			        
				        if(anim!="Damage" && !buttonpress2){  //can't interrupt ranged attack or damage animation
		       			        anim="Attack1";
	  		       	 	        buttonpress=1;

				        }
					break;
				 case SDLK_PERIOD:
					if(!buttonpress && anim!="Damage"){
                                                anim="Throw";
						buttonpress2=1;
					}
                                        break;

      				}
      		} else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){ //if key was released
    
        		switch( e.key.keysym.sym ){
      				case SDLK_DOWN:
				        anim="Stand";  //when crouch is released
				        break;
      				case SDLK_LEFT: 
				        if(xvel!=0 && flip==SDL_FLIP_HORIZONTAL){  //if key pressed but in the middle of animation, don't want to decrease xvel bc it is already 0
						xvel += img_vel;
					} 
					if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){  //if on ground 
	  					anim="Stand";
					}
					frame=0; //reset frame
					break;
      				case SDLK_RIGHT: 
        				if(xvel!=0 && flip==SDL_FLIP_NONE){	
						xvel -= img_vel;
					} 
					if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){
	  					anim="Stand";
					}
					frame=0;
					break;
      
      				case SDLK_COMMA:
				      
	 				break;
      				case SDLK_PERIOD:
				                                          
                                        break;

			}
    		}
  	} else if (controls==2 && anim!="BigHit"){  //if player 2
    		if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
   
   			switch( e.key.keysym.sym ){
      				case SDLK_w:
				        if(!buttonpress && !buttonpress2){
				                anim="Jump";
				        }
					if(jump==0){
	  					jump=1;
	  					t=0;
					}
					break;
      				case SDLK_s:
				        anim="Crouch";
					break;
      				case SDLK_a:
					if(!buttonpress && anim!="Damage"){
	  					xvel -= img_vel;
					}
        				if(flip==SDL_FLIP_NONE){ //change way facing appropriately
	  					flip=SDL_FLIP_HORIZONTAL;
					}
					if(jump<=0 && !buttonpress && anim!="Damage" && !buttonpress2){
	  					anim="Walk";
					}
					break;
      				case SDLK_d:
				        if(!buttonpress && anim!="Damage"){
	  					xvel += img_vel;
					}
					if(flip==SDL_FLIP_HORIZONTAL){
	  					flip=SDL_FLIP_NONE;
					}
					if(jump<=0 && !buttonpress && anim!="Damage" && !buttonpress2){
          					anim="Walk";
        				}
					break;
      				case SDLK_g:
				  	if(anim!="Damage" && !buttonpress2){
				        	anim="Attack1";
						buttonpress=1;
				       
				  	}
					break;
				case SDLK_h:
					if(!buttonpress && anim!="Damage"){
						anim="Throw";
						buttonpress2=1;
					}
					break;
      			}
    		} else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){ //if key was released
			switch( e.key.keysym.sym ){
				case SDLK_s:
				        anim="Stand";
				        break;
				case SDLK_a:
 					if(xvel!=0 && flip==SDL_FLIP_HORIZONTAL){
	  					xvel += img_vel;
					}
	  				if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){
            					anim="Stand";
          				}
					frame=0;
	  				break;
				case SDLK_d:
	  				if(xvel!=0 && flip==SDL_FLIP_NONE){
						xvel -= img_vel;
					}
	 				if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){
	    					anim="Stand";
	  				}
					frame=0;
	  				break;
				case SDLK_g:
	  				break;
				case SDLK_h:
					break;
			}

    		}
  	}
}

void Mario::move(int SCREEN_WIDTH, int SCREEN_HEIGHT){
  //Move up or down
  	ypos += yvel;

  //Move left or right
  	xpos += xvel;
  //continue falling if under stage
 	if( ypos > 230){
    		ypos+=4;
  	}
    
  //If it went too far to the left or right
  	if( ( xpos < 85 ) || ( xpos+mWidth > SCREEN_WIDTH-85 ) ){
      		ypos +=4;
    	}
}

void Mario::free(){
  	int i;
  	if(mTexture != NULL){
	    	SDL_DestroyTexture( mTexture );
    		mTexture = NULL;
   		mWidth=0;
    		mHeight=0;
  	}
}

void Mario::loadMedia(SDL_Renderer* gRenderer, string filename){

        gImage=loadTexture(filename, gRenderer); //call load texture with appropriate image name
  
}


SDL_Texture* Mario::loadTexture( string path, SDL_Renderer* gRenderer ){
  	free();
  //The final optimized image
  	SDL_Texture* newTexture = NULL;

  //Load image at specified path
  	SDL_Surface* loadedTexture = IMG_Load( path.c_str() );
  	if( loadedTexture == NULL ){

      		cout<<"Unable to load image"<<IMG_GetError()<<"\n";
    	} else {
    //Convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedTexture);
		if( newTexture == NULL ) {
      			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    		} else {
      //get width and height of image
      			mWidth = loadedTexture->w;
      			mHeight =loadedTexture->h;
    		}

    //Get rid of old loaded surface
    		SDL_FreeSurface( loadedTexture );
  	}
  	mTexture= newTexture;
  	return  mTexture; 
}

int Mario::display (SDL_Renderer* gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT ){
        move(SCREEN_WIDTH, SCREEN_HEIGHT);    //update x and y positions
	location.x=xpos+20;// set hit box for character
	location.y=ypos+20;
	location.w=mWidth-30;
	location.h=mHeight-30;
	if(flip==SDL_FLIP_NONE){//set hitbox for fist. dependent on the way character is facing
	  	fist.x=xpos+30; 
	  	fist.y=ypos+20;
	  	fist.w=mWidth-40;
	  	fist.h=mHeight-40;
	} else if(flip==SDL_FLIP_HORIZONTAL){
	  	fist.x=xpos+10;
          	fist.y=ypos+20;
          	fist.w=mWidth-40;
          	fist.h=mHeight-40;
	}
 	SDL_Rect renderQuad = { xpos+hitx, ypos-jump-hity, mWidth, mHeight};

  //calculate jump values to keep track of position while in the air
   	if(jump>0){
	        if(!buttonpress && !buttonpress2){//once attack animations stop 
	              anim="Jump";
	        }
     		t++;
     		jump=7*t-.3*t*t;
		if(jump<0){//if next step takes below 0
		  jump=0;
		}
   	} 
      
   	if(buttonpress==0 && frame==0 && jump==0 && anim!="Walk" && anim!="Damage" && buttonpress2==0 && anim!="Crouch"){
	        anim="Stand"; //default state 
   	}  
   	if (anim=="Stand"){
      		loadMedia(gRenderer, stand);
    	} else if(anim=="Attack1"){
      		loadMedia(gRenderer, attack1[frame/2]);
      		frame++;
      		if(frame>18){
    	  		frame=0;
			buttonpress=0;
      		}
	} else if(anim=="Damage"){
		 loadMedia(gRenderer, damaged[frame/2]);
		 if(frame==0 || frame%14!=0){//stop animation as mario is lieing down
		   frame++;
		 }
		
		t++; 
		hity=3*t-.3*t*t;  //knock into air with damage
		 if(hity<=0){//if hit goes below zero in an iteration
		     hity=0;
		     xvel=0;
		     frame=0;
		     anim="Stand";
		 }	     
	} else if(anim=="Crouch"){
	  loadMedia(gRenderer, crouch);
	}else if (anim=="Walk"){
     		loadMedia(gRenderer, walk[frame/2]);
     		frame++;
     		if(frame>14){
       			frame=0;
     		}
   	} else if (anim=="Jump"){
     		loadMedia(gRenderer, jumpanim);
   	} else if (anim=="Throw"){
		 loadMedia(gRenderer, attack2[frame/2]);
                frame++;
                if(frame>14){
                        frame=0;
			range=1;  //end of animation so projectile launched
			buttonpress2=0; //notifies key presses that other actions can now happen
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
