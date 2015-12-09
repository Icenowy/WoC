#include "SDL.h"

#include <iostream>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Unable to initialize SDL video." << std::endl;
		return 1;
	}
	atexit(SDL_Quit);
}
