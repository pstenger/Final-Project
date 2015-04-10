//simple driver program to run the functions
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"
//#include "Link.h"
#include "Mario.h"
#include "Background.h"
using namespace std;
bool init();
void close();
void loadstage();
SDL_Window* gWindow =NULL;
SDL_Renderer* gRenderer=NULL;
int SCREEN_WIDTH=640;
int SCREEN_HEIGHT=480;
SDL_Texture* background;
int main (){ 
   Mario mario1;
  Character * character1= &mario1;
  Background background;
  //Start up SDL and create window
  
  init();
  background.Load("background.png",gRenderer);
  character1->loadMedia(gRenderer); 
  bool quit = false;
  SDL_Event e;
  while( !quit ){
        //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ){
          //User requests quit
      if( e.type == SDL_QUIT ){
            quit = true;
      }
      character1->handleevent(e, gRenderer);
    }
   
    background.render(0, 0, gRenderer);
    character1->display(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT); 
    SDL_RenderClear(gRenderer);
  }
  background.free();
  character1->free();
  close();
  return 0;
}

bool init(){
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    cout<<"SDL could not initialize! SDL Error"<<SDL_GetError()<<"\n";
    success=false;
  } else {
    gWindow = SDL_CreateWindow( "Super Smash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT , SDL_WINDOW_SHOWN );
    if( gWindow == NULL ){
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
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
  return success; 
}
}

void close(){
  SDL_DestroyRenderer( gRenderer);
  //Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow=NULL;
  gRenderer= NULL;
  //Quit SDL subsystems
  SDL_Quit();
}
