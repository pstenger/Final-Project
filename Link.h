//This is the derived class for the character link
#ifndef LINK_H
#define LINK_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"  //base class
#include <stdio.h>
#include <iostream>
using namespace std;

class Link: public Character {
 public:
  Link();
  ~Link();
  static const int img_vel=10;
  bool init();
  //Loads media
  bool loadMedia();
  //Frees media and shuts down SDL
  void close();
  SDL_Texture* loadTexture( string path );
  void runfunc(); //runs funcs to display image
  void handleevent(SDL_Event& e);
  void move();
  void free();
 private: 
  string imagename;  //character image
  int xvel, yvel;
  int xpos, ypos;
  int mWidth;
  int mHeight;
  SDL_Texture* mTexture;
};

Link :: Link (){
  imagename= "link.png";
  xvel =0;
  yvel=0;
  xpos=25;
  ypos=900;
  mWidth=0;
  mHeight=0;
  mTexture=NULL;
}

Link::~Link(){
  free();
}

void Link::handleevent(SDL_Event& e){
  //make sure event is key press
  if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
      //Adjust the velocity
      switch( e.key.keysym.sym ){
	case SDLK_UP: yvel -= img_vel; break; //top of screen is 0 so - goes up
	case SDLK_DOWN: yvel += img_vel; break;
	case SDLK_LEFT: xvel -= img_vel; break;
	case SDLK_RIGHT: xvel += img_vel; break;
        }
  } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){ //if key was released
      //Adjust the velocity
      switch( e.key.keysym.sym )
        {
	case SDLK_UP: yvel += img_vel; break;
	case SDLK_DOWN: yvel -= img_vel; break;
	case SDLK_LEFT: xvel += img_vel; break;
	case SDLK_RIGHT: xvel -= img_vel; break;
        }
    }


}

void Link::move(){
  //Move up or down
  ypos += yvel;

  //If it went too far up or down
  if( ( ypos < 0 ) || ( ypos > SCREEN_HEIGHT ) )
    {
      //Move back
      ypos -= yvel;
    }
  //Move left or right
  xpos += xvel;

  //If it went too far to the left or right
  if( ( xpos < 0 ) || ( xpos > SCREEN_WIDTH ) )
    {
      //Move back
      xpos -= xvel;
    }
}

void Link::free(){
  if(mTexture != NULL){
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth=0;
    mHeight=0;
  }
}

bool Link::init() {
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    cout<<"SDL could not initialize! SDL Error"<<SDL_GetError()<<"\n";
    success=false;
  } else {
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT , SDL_WINDOW_SHOWN );
    if( gWindow == NULL ){
	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	success=false;
      } else{

	    //Get window surface
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL ){
	  printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	  success = false;
	} else {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//Initialize PNG loading
	int imgFlags= IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ){
	    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
	    success = false;
	  }

      } 
      
    }
  }  
  return success;
}


bool Link::loadMedia(){

  //Loading success flag
  bool success = true;

  //Load default surface
  gImage = loadTexture( imagename );
  if( gImage == NULL){

      printf( "Failed to load default image!\n" );
      success = false;
    }
  return success;
}

void Link::close(){
  free();
  SDL_DestroyRenderer( gRenderer);
  //Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow=NULL;
  gRenderer= NULL;
  //Quit SDL subsystems
  SDL_Quit();

}


SDL_Texture* Link::loadTexture( string path ){
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
      mWidth = loadedTexture->w;
      mHeight =loadedTexture->h;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedTexture );
  }
  mTexture= newTexture;
  return  mTexture; 
}

void Link::runfunc (){

  //Start up SDL and create window
  if( !init() ){
    printf( "Failed to initialize!\n" );
  } else {
    //Load media
    if( !loadMedia() ){
      printf( "Failed to load media!\n" );
    } else{
      bool quit = false;
      SDL_Event e;
      while( !quit ){
	//Handle events on queue
	while( SDL_PollEvent( &e ) != 0 ){
	  //User requests quit
	  if( e.type == SDL_QUIT ){
	    quit = true;
	
	  } 
	  handleevent(e);
	}
	move();    
	SDL_RenderClear( gRenderer );
	SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight};
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad);
	//Update the surface
        SDL_RenderPresent( gRenderer );

	 
	

      }
    }
    //Free resources and close SDL
    close();
  }
}

#endif
