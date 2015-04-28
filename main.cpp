//driver program to run the functions as well as the  menu
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"
#include "Link.h"
#include "Projectile.h"
#include "Mario.h"
#include "Background.h"
#include "Megaman.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <SDL2/SDL_ttf.h>
#include "Pac.h"
using namespace std;
bool init();
void close();
void loadstage();
SDL_Window* gWindow =NULL;
SDL_Renderer* gRenderer=NULL;
int SCREEN_WIDTH=1280;
int SCREEN_HEIGHT=800;
SDL_Texture* background;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 8;

Character * character1; //player 1 and player 2
Character * character2;
Megaman megaman1(1);  //instantiates an on object for each player as player 1 or 2
Pac pacman1(1);
Link link1(1);
Mario mario1(1);
Megaman megaman2(2);
Pac pacman2(2);
Link link2(2);
Mario mario2(2);
enum LButtonSprite
{
        mario8 = 4, //8bit or hd images assigned number code for reference on which is picked
	mariohd = 5,
	linkhd = 6,
	link8 = 7,
	pac8 = 0,
	pachd = 1,
	megahd = 2,
	mega8 = 3,
	BUTTON_SPRITE_TOTAL = 8
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Deallocates texture
  		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
  	void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
  		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

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

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
  int handleEvent( SDL_Event* e, int i );
	
		//Shows button sprite
		void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init1();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close1();

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
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
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
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
	mTexture = newTexture;
	return mTexture != NULL;
}


void LTexture::free()
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

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	
	mCurrentSprite = mario8;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

int LButton::handleEvent( SDL_Event* e, int i )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;
		
		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the bu
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside && i < 2)
		{
			mCurrentSprite = mario8;
		} else if ( !inside && (i==2||i==3)) { 
			    mCurrentSprite = link8;
		}else if ( !inside && (i==4||i==5)) { 
			    mCurrentSprite = pac8;
		}else if ( !inside && (i==6||i==7)) { 
			    mCurrentSprite = mega8;
		}
		//Mouse is inside button
	        else if (e->type !=SDL_MOUSEBUTTONUP && i == 0) {
		  	mCurrentSprite = mariohd;
			if (e->type ==SDL_MOUSEBUTTONDOWN){
			  return(1);
			}
		} else if (e->type !=SDL_MOUSEBUTTONUP && (i == 2)) {
		  mCurrentSprite = linkhd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(2);
		  }
		} else if (e->type !=SDL_MOUSEBUTTONUP && (i == 4)) {
		  mCurrentSprite = pachd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(3);
		  }
		} else if (e->type !=SDL_MOUSEBUTTONUP && (i == 6)) {
		  mCurrentSprite = megahd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(4);
		  }
		}
		else if (e->type !=SDL_MOUSEBUTTONUP && i == 1) {
		  	mCurrentSprite = mariohd;
			if (e->type ==SDL_MOUSEBUTTONDOWN){
			  return(5);
			}
		} else if (e->type !=SDL_MOUSEBUTTONUP && ( i==3)) {
		  mCurrentSprite = linkhd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(6);
		  }
		} else if (e->type !=SDL_MOUSEBUTTONUP && (i==5)) {
		  mCurrentSprite = pachd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(7);
		  }
		} else if (e->type !=SDL_MOUSEBUTTONUP && (i==7)) {
		  mCurrentSprite = megahd;
		  if (e->type ==SDL_MOUSEBUTTONDOWN){
		    return(8);
		  }
		}
		 
	      
	}
}

	
void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

bool init1()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Character Select", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !gButtonSpriteSheetTexture.loadFromFile( "button4.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = BUTTON_WIDTH;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
			
		}

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 4 ].setPosition( 0, BUTTON_HEIGHT );
		gButtons[ 5 ].setPosition( SCREEN_WIDTH-BUTTON_WIDTH, BUTTON_HEIGHT );
		gButtons[ 6 ].setPosition( 0, SCREEN_HEIGHT - 2*BUTTON_HEIGHT );
		gButtons[ 7 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - 2*BUTTON_HEIGHT );
	}

	return success;
}

void close1()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main (){ 
   int a, b, c;
  a=0;
  b=0;
  c=0;
  bool quit = false;
	//Start up SDL and create window
	if( !init1() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
		  //	bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
				  if( e.type == SDL_QUIT || (a!=0 && b!=0) )
					{
						quit = true;
					}
					
					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
					  
					  if ((a==0) || (b==0)){ 
					    c=gButtons[ i ].handleEvent( &e, i );
					    
					  }
					  if (c==1||c==2||c==3||c==4) {
					        a=c;
					  }
					  if (c==5||c==6||c==7||c==8) {
					      b=c;
					  }
				}
			}
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render buttons
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render();
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close1();
	SCREEN_WIDTH=640;
	SCREEN_HEIGHT=480;
	string rangename1, rangename2, player1name, player2name;
	// assign character to the right object
	if (a==1) {
	  character1=&mario2;
	  player1name="Mario";
	  rangename1="mariorange.png";
	} else if (a==2) {
	  character1=&link2;
	  player1name="Link";
	  rangename1="linkrange.png";
	} else if (a==3) {
	  character1=&pacman2;
	  player1name="Pacman";
	  rangename1="pacrange.png";
	} else if (a==4) {
	  character1=&megaman2;
	  player1name="Megaman";
	  rangename1="megamanrange.png";
	}
	if (b==5) {
	  character2=&mario1;
	   player2name="Mario";
	  rangename2="mariorange.png";
	} else if (b==6) {
	  character2=&link1;
	   player2name="Link";
	  rangename2="linkrange.png";
	} else if (b==7) {
	  character2=&pacman1;
	   player2name="Pacman";
	  rangename2="pacrange.png";
	} else if (b==8) {
	  character2=&megaman1;
	  player2name="Megaman";
	  rangename2="megamanrange.png";
	}

  Background background;
  SDL_Rect* intersection;
  SDL_Rect rect1; //rectangles representing position of characters
  SDL_Rect rect2;
  SDL_Rect rect1_hit; //rectangle representing position of characters' fist (for attacks)
  SDL_Rect rect2_hit;
  string health1="0";
  string health2="0";
  SDL_Rect rectproj;  //rectangle of projectile
  int proj; //provides different names for objects
  string projstring; //string version for naming
  ostringstream ss; //for use converting string and int
  vector<Projectile> range1;  //vectors for each players projectiles
  vector<Projectile> range2;
  vector<Projectile>::iterator iter; //iterator
  string char1=rangename1; //for determinng the file type for projectile
  string char2=rangename2;
  init(); //initialize window
  background.Load("background.png",gRenderer);
  SDL_Event e;
   TTF_Init(); //initialize text
  TTF_Font *font =TTF_OpenFont("font.ttf", 35);
  if(font==NULL){
    cout<<"Failed"<<endl;
  }
  SDL_Color color1={1, 235, 71}; //set colors for health text
  SDL_Color color2={0, 14, 215};
  SDL_Surface* health1surf=TTF_RenderText_Solid(font, health1.c_str(), color1); //surface and texture for health text
  SDL_Surface* health2surf=TTF_RenderText_Solid(font, health2.c_str(), color2);
  SDL_Texture* health1tex=SDL_CreateTextureFromSurface(gRenderer, health1surf);
  SDL_Texture* health2tex=SDL_CreateTextureFromSurface(gRenderer, health2surf);
  int health1W=0;
  int health1H=0;
  int health2W=0;
  int health2H=0;
  SDL_QueryTexture(health1tex, NULL, NULL, &health1W, &health1H);
  SDL_QueryTexture(health2tex, NULL, NULL, &health2W, &health2H);
  SDL_Rect health1rect={540, 400, health1W, health1H};  //rectangles for text
  SDL_Rect health2rect={100, 400, health2W, health2H};
  if(!quit){
    while( !quit ){
        //Handle events on queue
      while( SDL_PollEvent( &e ) != 0 ){
          //User requests quit
	if( e.type == SDL_QUIT ){
	  quit = true;
	}
      //check for key presses
	character1->handleevent(e, gRenderer);
	character2->handleevent(e, gRenderer);
      }
      background.render(SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);
      SDL_RenderPresent(gRenderer);
      if(character1->dead()){//if dead, appropriate animations
	character1->lost(gRenderer);
	character2->win(gRenderer);
	SDL_RenderPresent(gRenderer);

      } else if (character2->dead()){
	character2->lost(gRenderer);
	character1->win(gRenderer);
	SDL_RenderPresent(gRenderer);
      } else {
	if(character1->display(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)){//display returns 1 if it reaches the end of the projectile launch animation
      
	  ss<<proj;//converting proj to string for use in dynamically naming objects
	  projstring=ss.str();
	  Projectile projstring(character1->get_xpos(), character1->get_ypos(), character1->facing(), player1name);
	  range1.push_back(projstring);   //puts new projectile object into vector with current x and y pos of character
	  proj++; //makes proj, and therefore next object name different
	}
    
    
	if(character2->display(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)){  //same as above but for player 2
      
	  ss<<proj;
	  projstring=ss.str();
	
	  Projectile projstring(character2->get_xpos(), character2->get_ypos(), character2->facing(), player2name);      
	  range2.push_back(projstring);
	  proj++;
	}
    
	rect1=character1->get_location(0); //character positions
	rect2=character2->get_location(0);
	rect1_hit=character1->get_location(1); //fist positions
	rect2_hit=character2->get_location(1);

	for(iter=range1.begin(); iter!=range1.end(); iter++){  //iterates through projectile vectors, comparing to character location to determine if it is a hit
	  iter->Load(char1, gRenderer);  //displays projectile
	  rectproj=iter->get_location();  //sets rectangle of projectile for use in collision detection
	  if(SDL_HasIntersection(&rectproj, &rect2)){
	    character2->damage(2, character1->facing());    
	  } else {
	    iter->render(SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);  //renders projectile if it doesn't hit character
	  }
	  iter->update();  //updates position of projectile
	}
	for(iter=range2.begin(); iter!=range2.end(); iter++){ //same as above for player 2
	  iter->Load(char2, gRenderer);	
	  rectproj=iter->get_location();
	  if(SDL_HasIntersection(&rectproj, &rect1)){
	    character1->damage(2, character2->facing());
	   
	  } else {
	    iter->render(SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);
	  }
	  iter->update();
	}

	if (character1->attacking()==1){ //check for melee attack
	  if(SDL_HasIntersection(&rect1_hit, &rect2)){ //check for intersection
	    character2->damage(1, character1->facing()); //if intersection do damage
	  }
	}
	if (character2->attacking()){
	  if(SDL_HasIntersection(&rect1, &rect2_hit)){
	    character1->damage(1, character2->facing());
	  }
	}
	ss.clear();
	ss.str("");
	ss<<(character1->gethealth());
	health1=ss.str();
	ss.clear();
	ss.str("");
	ss<<(character2->gethealth());
	health2=ss.str();
	ss.clear();
	ss.str("");
	health1surf=TTF_RenderText_Solid(font, health1.c_str(), color1);
	health2surf=TTF_RenderText_Solid(font, health2.c_str(), color2);
	health1tex=SDL_CreateTextureFromSurface(gRenderer, health1surf);
	health2tex=SDL_CreateTextureFromSurface(gRenderer, health2surf);
	SDL_QueryTexture(health1tex, NULL, NULL, &health1W, &health1H);
	SDL_QueryTexture(health2tex, NULL, NULL, &health2W, &health2H);
	SDL_RenderCopy(gRenderer, health1tex, NULL, &health1rect);
	SDL_RenderCopy(gRenderer, health2tex, NULL, &health2rect);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer); //clear renders for next loop
      }
    }
    background.free();
    character1->free();
    character2->free();
    close();
    SDL_DestroyTexture(health1tex);
    SDL_FreeSurface(health1surf);
    SDL_DestroyTexture(health2tex);
    SDL_FreeSurface(health2surf);
    TTF_CloseFont(font);
  }
  return 0;
}


bool init(){
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    cout<<"SDL could not initialize! SDL Error"<<SDL_GetError()<<"\n";
    success=false;
  } else {
    //create window
    gWindow = SDL_CreateWindow( "Super Smash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT , SDL_WINDOW_SHOWN );
    if( gWindow == NULL ){
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success=false;
    } else{

      //set gRenderer
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

void close(){//close/destroy window/renderer 
  SDL_DestroyRenderer( gRenderer);
  SDL_DestroyWindow( gWindow );
  gWindow=NULL;
  gRenderer= NULL;
  SDL_Quit();
}
	
