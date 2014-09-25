//Header Files
#include <iostream>
//header for SDL2 functionality
#include <SDL.h>


//Global variables go here
//Pointer to our SDL Windows
SDL_Window*window;

//Constants to control window cration
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

//Global functions
void InitWindow(int width, int height, bool fullscreen){
	//Create a window
	window = SDL_CreateWindow(
		"Lab 1",		//Window title
		SDL_WINDOWPOS_CENTERED,	//x position, centered
		SDL_WINDOWPOS_CENTERED, //y position, centered
		width,			//width, in pixels
		height,			//height, in pixels
		SDL_WINDOW_OPENGL	//flags
		);

}

//Used to cleanup once we exit
void CleanUp(){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//Main Method - Entry Point
int main(int argc, char*arg[]){
	//init everything - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return -1;
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	CleanUp();

	return 0;
}