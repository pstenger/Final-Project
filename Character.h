#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class Character{
	public:
    		Character(int player);
    		//Loads media
    		static const int img_vel=8; 
    		virtual void loadMedia(SDL_Renderer*, string)=0;
    		//Frees media and shuts down SDL
    		SDL_Texture* loadTexture( string path, SDL_Renderer*);
    		virtual int display (SDL_Renderer*, int , int)=0;
		virtual void win(SDL_Renderer*)=0;
		virtual void lost(SDL_Renderer*)=0;
		int dead();
    		void handleevent(SDL_Event& e, SDL_Renderer*);
    		void free();
    		int get_xpos();
    		int get_ypos();
    		int get_width(); 
    		int get_height(); 
    		virtual SDL_Rect get_location(int)=0;
    		int attacking();  //returns true if character is attacking
    		void damage(int, string); 
    		string facing();
    		int gethealth();
		void move(int, int);  //calculates xpos and ypos from xvel and yvel
 	protected:
    		//The window we'll be rendering to
    		SDL_Texture* mTexture;
    		//The surface contained by the window
    		SDL_Texture* gScreenSurface;
    		int mWidth, mHeight;
    		//Current displayed image
    		SDL_Texture* gImage;
    		SDL_RendererFlip flip;
    		int xpos, ypos;
    		int controls;
    		string anim;
    		int xvel, yvel;
    		int health;
    		int hity;
    		int t;
    		double jump;
    		bool buttonpress, buttonpress2; //keeps track of whether either attack button has been pressed
    		int lives;
    		int frame;
};

Character::Character (int player) : controls(player){

  	gScreenSurface=NULL;
  	gImage=NULL;
  	mWidth=0;
  	mHeight=0;
  	mTexture=NULL;
  	controls=player;
  	anim="Stand";
  	health=0;
  	hity=0;
  	xvel=0;
  	yvel=0;
  	flip=SDL_FLIP_NONE;
  	t=0;
  	jump=0;
  	lives=3;
  	frame=0;
  	buttonpress=0;
  	buttonpress2=0;
  	if(player==1){  //sets beginning position of character
    		xpos=85;
    		ypos=230;
  	} else {
    		xpos=285;
    		ypos=230;
    		flip=SDL_FLIP_HORIZONTAL;
  	}

}

int Character::gethealth(){
        return health;
}
int Character::dead(){
  if(lives<=0){
    return 1;
  } else {
    return 0;
  }
}
 
void Character::damage(int style, string direction){ //style takes in type of attack and direction takes in which direction attack is from
  	if(anim!="Crouch"){//crouch is block equivalent so can't be damaged
    		if (style==1){ //melee
      			health=health+1;
    		} else if (style==2){ //ranged
      			health=health+2;
    		}
    		if(direction=="left"){
      			if(health>75){
        			xvel=-7;
      			} else if (xvel>150){
        			xvel=-10;
      			} else {
        			xvel=-1;
      			}
    		} else {
      			if(health>75){
        			xvel=7;
      			} else if (xvel>150){
        			xvel=10;
      			} else {
        			xvel=1;
      			}
    		}
    		//cout<<health<<endl;
    		anim="Damage"; //invokes damage animation
   	 	hity=1; //allows to go into air when hit
    		frame=0;
    		t=0;  //t used to represent time in equation get character's position in air
  	}
}

void Character::free(){
  	int i;
  	if(mTexture != NULL){
    	SDL_DestroyTexture( mTexture );
    	mTexture = NULL;
    	mWidth=0;
    	mHeight=0;
  	}
}

SDL_Texture* Character::loadTexture( string path, SDL_Renderer* gRenderer ){
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

int Character::attacking(){
  if(buttonpress==1){  //used in main to determine if character is in state of attack
    return 1;
  } else {
    return 0;
  }
}

void Character::move(int SCREEN_WIDTH, int SCREEN_HEIGHT){
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
  if(ypos>480){
    lives--;
    t=0;
    anim="Respawn";
    frame=0;
    xvel=0;
  }
}

void Character::handleevent(SDL_Event& e, SDL_Renderer* gRenderer){
  //make sure event is key press
  if(controls==1 && anim!="Damage"){ //if player 1
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
	if(xvel!=0 && flip==SDL_FLIP_HORIZONTAL){  //if key pressed but in the middle of animation, don't want to decre\
ase xvel bc it is already 0
  xvel += img_vel;
	}
	if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){  //if on ground
	  anim="Stand";
	}

	break;
      case SDLK_RIGHT:
	if(xvel!=0 && flip==SDL_FLIP_NONE){
	  xvel -= img_vel;
	}
	if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){
	  anim="Stand";
	}

	break;

      case SDLK_COMMA:

	break;
      case SDLK_PERIOD:

	break;
      }
    }
  } else if (controls==2 && anim!="Damage"){  //if player 2
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

	break;
      case SDLK_d:
	if(xvel!=0 && flip==SDL_FLIP_NONE){
	  xvel -= img_vel;
	}
	if(jump<=0 && anim!="Damage" && anim!="Throw" && anim!="Attack1"){
	  anim="Stand";
	}

	break;
      case SDLK_g:
	break;
      case SDLK_h:
	break;
      }

    }
  }
}

int Character::get_width(){
  return(mWidth);
}
int Character::get_height(){
  return(mHeight);
}

int Character::get_xpos(){
  return(xpos);
}

int Character::get_ypos(){
  return(ypos);
}

string Character::facing(){
  if (flip==SDL_FLIP_NONE){
    return("right");
  } else {
    return("left");
  }
}

#endif
