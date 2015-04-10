#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
//Texture wrapper class
class Background{
 public:
  //Initializes variables
  Background();

//Deallocates memory
  ~Background();
//Loads image at specified path
  void Load( string path, SDL_Renderer* );

//Deallocates texture
  void free();

//Renders texture at given point
  void render( int x, int y, SDL_Renderer* );

//Gets image dimensions
  int getWidth();
  int getHeight();

 private:
//The actual hardware texture
  SDL_Texture* mTexture;

//Image dimensions
  int mWidth;
  int mHeight;
};

Background::Background()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Background::~Background()
{
	
	free();
}

void Background::Load(string path, SDL_Renderer* gRenderer)
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

void Background::free()
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

void Background::render(int x, int y, SDL_Renderer* gRenderer){
  //SDL_RenderClear(gRenderer);
  SDL_Rect renderQuad={x, y, mWidth, mHeight};
  SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
  SDL_RenderPresent(gRenderer);
}
int Background::getWidth()
{
	return mWidth;
}

int Background::getHeight()
{
  return mHeight;
}
#endif
