//This is the derived class for the character link
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
  static const int img_vel=10;
  //Loads media
  bool loadMedia(SDL_Renderer*);
  //Frees media and shuts down SDL
  SDL_Texture* loadTexture( string path, SDL_Renderer* );
  void display(SDL_Renderer*, int, int ); //runs funcs to display image
  void handleevent(SDL_Event& e, SDL_Renderer*);
  void move(int, int);
  void free();
  int get_xpos();
  int get_ypos();
  int get_width(); //return width of image
  int get_height(); //return height of image
 private: 
  string imagename;  //character image
  int xvel, yvel;
  int xpos, ypos;
  int mWidth; //width of image
  int mHeight; //height of image
  SDL_Texture* mTexture;
  int jump, jump2; //keeps track of jump and midair jump
  int t, t2;   //timer variables for jump
  int controls; //1 for player 1 controls and 2 for player 2 controls
  SDL_RendererFlip flip;
  int attack1count;  //counter to keep track of attack cycle
  
};

Mario::Mario (int player){
  imagename= "mario.png";
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
  controls=player;
  flip=SDL_FLIP_NONE;
  attack1count=0;
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

void Mario::handleevent(SDL_Event& e, SDL_Renderer* gRenderer){
  //make sure event is key press
  if(controls==1){ //if player 1
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
      //Adjust the velocity
      switch( e.key.keysym.sym ){
        case SDLK_UP: 
	  if(jump>0 && jump2>0){
	    break;
	  } else if(jump>0){
	    jump2=15;
	    t2=0;
	    break;
	  } else {
	    jump=15;
	    t=0;
	    t2=0;
	    break;
	  }
        case SDLK_DOWN:
	  imagename="mariocrouch.png";
	  loadMedia(gRenderer);
	  break;
	case SDLK_LEFT: 
	  xvel -= img_vel; 
	  if(flip==SDL_FLIP_NONE){
	    flip=SDL_FLIP_HORIZONTAL;
	  }
	  break;
	case SDLK_RIGHT: 
	  xvel += img_vel; 
	  if(flip==SDL_FLIP_HORIZONTAL){
	    flip=SDL_FLIP_NONE;
	  }
	  break;
        case SDLK_COMMA:
	  if(attack1count==0){
	    attack1count=20;
	    imagename="marioattack1_1.png";
	    loadMedia(gRenderer);
	  } else if(attack1count==10){
	    imagename="marioattack1_2.png";
            loadMedia(gRenderer);
	  } else if (attack1count==0){
	    imagename="mario.png";
            loadMedia(gRenderer);
	  }
	  break;
      }
      } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){ //if key was released
    //Adjust the velocity
      switch( e.key.keysym.sym )
      {
      case SDLK_DOWN:
	imagename="mario.png";
	loadMedia(gRenderer);
	break;
      case SDLK_LEFT: 
        xvel += img_vel; 
        break;
      case SDLK_RIGHT: 
        xvel -= img_vel; 
        break;
      
      case SDLK_COMMA:
	imagename="mario.png";
	loadMedia(gRenderer);
	break;
      }
    }
  } else if (controls==2){  //if player 2
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
      //Adjust the velocity
      switch( e.key.keysym.sym ){
      case SDLK_w:
	if(jump>0 && jump2>0){
	  break;
	} else if(jump>0){
	  jump2=20;
	  t2=0;
	  break;
	} else {
	  jump=15;
	  t=0;
	  t2=0;
	  break;
	}
      case SDLK_s:
	imagename="mariocrouch.png";
	loadMedia(gRenderer);
	break;
      case SDLK_a:
	xvel -= img_vel;
	if(flip==SDL_FLIP_NONE){
	  flip=SDL_FLIP_HORIZONTAL;
	}

	break;
      case SDLK_d:
	xvel += img_vel;
	if(flip==SDL_FLIP_HORIZONTAL){
	  flip=SDL_FLIP_NONE;
	}
	break;
      case SDLK_g:
	if(attack1count==0){
	  attack1count=20;
	  imagename="marioattack1_1.png";
	  loadMedia(gRenderer);
	} else if(attack1count==10){
	  imagename="marioattack1_2.png";
	  loadMedia(gRenderer);
	} else if (attack1count==0){
	  imagename="mario.png";
	  loadMedia(gRenderer);
	}

	break;

      }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){ //if key was released
      //Adjust the velocity
      switch( e.key.keysym.sym )
	{
	case SDLK_s:
	  imagename="mario.png";
	  loadMedia(gRenderer);
	  break;
	case SDLK_a:
	  xvel += img_vel;
	  break;
	case SDLK_d:
	  xvel -= img_vel;
	  break;
	case SDLK_g:
	  imagename="mario.png";
	  loadMedia(gRenderer);
	  attack1count=0;
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
    ypos+=10;
  }
    
  //If it went too far to the left or right
  if( ( xpos < 85 ) || ( xpos+mWidth > SCREEN_WIDTH-85 ) ){
      //Move back
      ypos += 10;
    }
}

void Mario::free(){
  if(mTexture != NULL){
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth=0;
    mHeight=0;
  }
}

bool Mario::loadMedia(SDL_Renderer* gRenderer){

  //Loading success flag
  bool success = true;

  //Load default surface
  gImage = loadTexture( imagename, gRenderer );
  if( gImage == NULL){

      printf( "Failed to load default image!\n" );
      success = false;
    }
  return success;
}


SDL_Texture* Mario::loadTexture( string path, SDL_Renderer* gRenderer ){
  free();
  //The final optimized image
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedTexture = IMG_Load( path.c_str() );
  if( loadedTexture == NULL ){

      cout<<"Unable to load image"<<IMG_GetError()<<"\n";
    }
  else {
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

void Mario::display (SDL_Renderer* gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT ){
  move(SCREEN_WIDTH, SCREEN_HEIGHT);    
  SDL_Rect renderQuad = { xpos, ypos-jump, mWidth, mHeight};
  //calculate jump values to keep track of position while in the air
          if(jump>0){
	  jump=15+3*t-.1*t*t+jump2;
	  t++;
	  if (jump2>0){
	    jump2=20+3*t2-.1*t2*t2;
	    t2++;
	  }
        }
	
	  if(attack1count>0){
	    attack1count--;
	  }
	//render the image with appropriate flip and no rotation
	SDL_RenderCopyEx( gRenderer, gImage, NULL, &renderQuad, 0, NULL, flip);
	//Update the surface
        SDL_RenderPresent( gRenderer );

	 
}
#endif
