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
    virtual void loadMedia(SDL_Renderer*, string)=0;
    //Frees media and shuts down SDL
    virtual SDL_Texture* loadTexture( string path, SDL_Renderer*)=0;
    virtual int display (SDL_Renderer*, int , int)=0;
    virtual void handleevent(SDL_Event& e, SDL_Renderer*)=0;
    virtual void free()=0;
    virtual int get_xpos()=0;
    virtual int get_ypos()=0;
    virtual int get_width()=0; 
    virtual int get_height()=0; 
    virtual SDL_Rect get_location(int)=0;
    virtual int attacking()=0;  //returns true if character is attacking
    virtual void damage(int, string)=0; 
    virtual string facing()=0;
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
