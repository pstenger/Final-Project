//simple driver program to run the functions
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
#include <sstream>
#include <SDL2/SDL_ttf.h>
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
  Megaman megaman(1);
  Link link1(2);
  Character * character1= &megaman;
  Character * character2=&link1;
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
  string char1="megamanrange.png"; //for determinng the file type for projectile
  string char2="linkrange.png";
  init(); //initialize window
  background.Load("background.png",gRenderer);
  bool quit = false;
  SDL_Event e;
  TTF_Init(); //initialize text
  TTF_Font *font =TTF_OpenFont("font.ttf", 35);
  if(font==NULL){
    cout<<"Failed"<<endl;
  }
  SDL_Color color1={255, 0, 0};
  SDL_Color color2={0, 14, 215};
  SDL_Color color3={0, 0, 0};
  SDL_Surface* health1surf=TTF_RenderText_Solid(font, health1.c_str(), color1);
  SDL_Surface* health2surf=TTF_RenderText_Solid(font, health2.c_str(), color2);
  SDL_Texture* health1tex=SDL_CreateTextureFromSurface(gRenderer, health1surf);
  SDL_Texture* health2tex=SDL_CreateTextureFromSurface(gRenderer, health2surf);
  int health1W=0;
  int health1H=0;
  int health2W=0;
  int health2H=0;
  SDL_Surface* end1surf=TTF_RenderText_Solid(font, "Player 2 Wins", color3);
  SDL_Texture* end1tex=SDL_CreateTextureFromSurface(gRenderer, end1surf);
  SDL_QueryTexture(end1tex, NULL, NULL, &health1W, &health1H);
  SDL_Rect end1rect={170, 240, 0, 0};
  SDL_Surface* end2surf=TTF_RenderText_Solid(font, "Player 1 Wins", color3);
  SDL_Texture* end2tex=SDL_CreateTextureFromSurface(gRenderer, end2surf);
  SDL_QueryTexture(end2tex, NULL, NULL, &health1W, &health1H);
  SDL_Rect end2rect={170, 240, 0, 0};
  SDL_QueryTexture(health1tex, NULL, NULL, &health1W, &health1H);
  SDL_QueryTexture(health2tex, NULL, NULL, &health2W, &health2H);
  SDL_Rect health1rect={100, 400, health1W, health1H};
  SDL_Rect health2rect={540, 400, health2W, health2H};
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
    if(character1->dead()){
      character1->lost(gRenderer);
      character2->win(gRenderer);
      SDL_RenderCopy(gRenderer, end1tex, NULL, &end1rect);
      SDL_RenderPresent(gRenderer);
      SDL_RenderClear(gRenderer); //clear renders for next loop
    } else if (character2->dead()){
      character2->lost(gRenderer);
      character1->win(gRenderer);
      SDL_RenderCopy(gRenderer, end2tex, NULL, &end2rect);
      SDL_RenderPresent(gRenderer);
      SDL_RenderClear(gRenderer); //clear renders for next loop
    } else {
      if(character1->display(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)){//display returns 1 if it reaches the end of the projectile launch animation
      
	ss<<proj;//converting proj to string for use in dynamically naming objects
	projstring=ss.str();
	ss.clear();
	ss.str("");
	Projectile projstring(character1->get_xpos(), character1->get_ypos(), character1->facing(), "Megaman");
	//projstring.Load(char1, gRenderer);
	range1.push_back(projstring);   //puts new projectile object into vector with current x and y pos of character
	proj++; //makes proj, and therefore next object name different
      }
    
    
      if(character2->display(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)){  //same as above but for player 2
      
	ss<<proj;
	projstring=ss.str();
	ss.clear();
	ss.str("");
	Projectile projstring(character2->get_xpos(), character2->get_ypos(), character2->facing(), "Link");      
	//projstring.Load(char2, gRenderer);
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
