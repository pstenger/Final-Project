#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class Character{
  public:
    Character();
    //Loads media 
    virtual bool loadMedia(SDL_Renderer*)=0;
    //Frees media and shuts down SDL
    virtual SDL_Texture* loadTexture( string path, SDL_Renderer*)=0;
    virtual void display (SDL_Renderer*, int , int)=0;
    virtual void handleevent(SDL_Event& e, SDL_Renderer*)=0;
    virtual void free()=0;
 protected:
    //The window we'll be rendering to
    //SDL_Window* gWindow;
    //int SCREEN_WIDTH;
    //int SCREEN_HEIGHT;
    //The surface contained by the window
    SDL_Texture* gScreenSurface;
    //SDL_Renderer* gRenderer =NULL;
    //Current displayed image
    SDL_Texture* gImage;
};

Character::Character (){
  //gWindow=NULL;
  //SCREEN_WIDTH=1000;
  //SCREEN_HEIGHT=1000;
  gScreenSurface=NULL;
  gImage=NULL;
}

#endif
