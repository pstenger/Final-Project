#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class Character{
  public:
    Character();
    virtual bool init()=0;
    //Loads media 
    virtual bool loadMedia()=0;
    //Frees media and shuts down SDL
    virtual void close()=0;
    virtual SDL_Surface* loadSurface( string path)=0;
    virtual void runfunc ()=0;
  protected:
    //The window we'll be rendering to
    SDL_Window* gWindow;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    //The surface contained by the window
    SDL_Surface* gScreenSurface;

    //Current displayed image
    SDL_Surface* gImage;
};

Character::Character (){
  gWindow=NULL;
  SCREEN_WIDTH=640;
  SCREEN_HEIGHT=480;
  gScreenSurface=NULL;
  gImage=NULL;
}

#endif
