//The class for instantiating projectiles. This takes as input the
//characters x and y positions, which way they are facing and which
//character they are
#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
//Texture wrapper class
class Projectile{
 	public:
  	//Initializes variables
  		Projectile(int xcord=0, int ycord=0, string facing="right", string character="Mario");

		//Deallocates memory
  		~Projectile();
		//Loads image at specified path
  		void Load( string path, SDL_Renderer* );

		//Deallocates texture
  		void free();

		//Renders texture at given point
  		void render( int x, int y, SDL_Renderer* );

		//Gets image dimensions
  		int getWidth();
  		int getHeight();
  		int OffScreen();
		SDL_Rect get_location();
  		void update(); //update xpos
 	private:
		//The actual hardware texture
  		SDL_Texture* mTexture;
  		int xpos;
  		int ypos;
		//Image dimensions
  		int mWidth;
  		int mHeight;
  		string direction; //direction character is facing
		SDL_Rect location;
  		SDL_RendererFlip flip; //whether or not to flip way projectile points
};

Projectile::Projectile(int xcord, int ycord, string facing, string character)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	if(character=="Megaman" || character=="Link" || character=="Pacman"){ //have different image sizes so different offsets
	  ypos=ycord+40;
	} else {
	  ypos=ycord+25;
	}
	if(facing=="right"){  
	  xpos=xcord+50;
	  flip=SDL_FLIP_NONE;
	} else {
	  xpos=xcord;
	  flip=SDL_FLIP_HORIZONTAL;
	}
	direction=facing;
}

Projectile::~Projectile()
{
	free();
}

SDL_Rect Projectile::get_location(){
  	return (location);
}
void Projectile::Load(string path, SDL_Renderer* gRenderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{

	
	  newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;

		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture=newTexture;
       
}

void Projectile::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Projectile::render(int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Renderer* gRenderer){
  	//SDL_RenderClear(gRenderer);
  if(xpos>640 || xpos<0 || ypos>480 || ypos<0){ //free if leaves screen
    		free();
  	}
  	SDL_Rect renderQuad;
  	renderQuad.x=xpos;  
  	renderQuad.y=ypos;
  	renderQuad.w=mWidth;
  	renderQuad.h=mHeight;
  	SDL_RenderCopyEx( gRenderer, mTexture, NULL, &renderQuad, 0, NULL, flip);
  	SDL_RenderPresent(gRenderer);
}
int Projectile::getWidth()
{
	return mWidth;
}

int Projectile::getHeight()
{
  	return mHeight;
}
int Projectile::OffScreen()
{
  if(xpos<-100 || xpos>1280){
    return 1;
  } else {
    return 0;
  }
}
void Projectile::update(){
        if(direction=="left"){//velocity
	        xpos=xpos-15;
    
  	} else if (direction=="right"){ //move right if facing right
    		xpos=xpos+15;
  
  	}
      
  	location.x=xpos;
  	location.y=ypos;
  	location.w=mWidth;
  	location.h=mHeight;
}
#endif
