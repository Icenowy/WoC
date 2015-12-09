#include "SDL2/SDL.h"

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "../vm/vcpu.h"
#include "../vpool/vcpu_pool.h"

#define TITLE "WoC: SimpleWars"

uint32_t p1[0x4000], p2[0x4000];

size_t read_player (char *filename, uint32_t *mem) {
	FILE *fp;
	if (! (fp = std::fopen (filename, "r"))) {
		std::cerr << "Cannot open player " << filename << std::endl;
		std::exit(1);
	}
	size_t res = std::fread(mem, sizeof(uint32_t), 0x4000, fp);
	std::fclose(fp);
	return res;
}

int main (int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " Player1 Player2" << std::endl;
		return 1;
	}

	int delay = 50;

	if ( getenv("WOC_DELAY") != NULL) {
		delay = atoi(getenv("WOC_DELAY"));
	}

	std::srand (std::time (NULL));

	size_t sp1 = read_player (argv[1], p1);
	size_t sp2 = read_player (argv[2], p2);

	vcpu_pool *pool = new vcpu_pool;

	size_t pos_p1 = 0x4000 + std::rand() % (0x4000 - sp1 + 1);
	size_t pos_p2 = 0x8000 + std::rand() % (0x4000 - sp2 + 1);

	if ( getenv("WOC_POS_P1") != NULL) {
		pos_p1 = atoi(getenv("WOC_POS_P1"));
	}
	if ( getenv("WOC_POS_P2") != NULL) {
		pos_p2 = atoi(getenv("WOC_POS_P2"));
	}

	for(size_t i = 0; i < sp1; i++) {
		pool->mem[pos_p1 + i] = p1[i];
		pool->mem_color[pos_p1 + i] = 1;
	}

	for(size_t i = 0; i < sp2; i++) {
		pool->mem[pos_p2 + i] = p2[i];
		pool->mem_color[pos_p2 + i] = 2;
	}

	vcpu *cpu_p1 = pool->add_vcpu (pos_p1, 0x2000, 0xE000, 1, 3);
	vcpu *cpu_p2 = pool->add_vcpu (pos_p2, 0xC000, 0x0000, 2, 4);

	if (SDL_VideoInit (NULL) < 0) {
		std::cerr << "Unable to initialize SDL video." << std::endl;
		return 1;
	}
	atexit (SDL_VideoQuit);

	SDL_Window *mainWindow = SDL_CreateWindow (TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		512, 512, 0);
	SDL_ShowWindow (mainWindow);

	SDL_Surface *mainSurface = SDL_GetWindowSurface (mainWindow);

	SDL_Renderer *renderer = SDL_CreateSoftwareRenderer (mainSurface);

	bool win = false;
	bool exit = false;

	char title[1001],title_prev[1001];

	strcpy (title, TITLE);

	do {
		if (!win) {
			for (int i = 0; i < 256; i++) {
				for (int j = 0; j < 256; j++) {
					uint8_t r, g, b;
					switch(pool->mem_color[j * 256 + i]) {
					case 1:
						r = 255; g = 192; b = 192;
						break;
					case 2:
						r = 192; g = 192; b = 255;
						break;
					case 3:
						r = 255; g = 0; b = 0;
						break;
					case 4:
						r = 0; g = 0; b = 255;
						break;
					default:
						r = 255; g = 255; b = 255;
						break;
					}
					SDL_SetRenderDrawColor (renderer, r, g, b, 0);
					SDL_RenderDrawPoint (renderer, 2*i + 0, 2*j + 0);
					SDL_RenderDrawPoint (renderer, 2*i + 1, 2*j + 0);
					SDL_RenderDrawPoint (renderer, 2*i + 0, 2*j + 1);
					SDL_RenderDrawPoint (renderer, 2*i + 1, 2*j + 1);
				}
			}

			SDL_UpdateWindowSurface (mainWindow);

			pool->nexti ();

			strcpy (title_prev, title);
			strcpy (title, TITLE);

			if (cpu_p1->IN) strcat (title, " [P1 IN]");
			if (cpu_p2->IN) strcat (title, " [P2 IN]");

			if (strcmp (title, title_prev)) SDL_SetWindowTitle (mainWindow, title);

			std::vector<vcpu *>::iterator beg = pool->pool.begin(), en = pool->pool.end();

			if (std::find (beg, en, cpu_p1) == en) {
				std::cerr << "Player 2 wins!" << std::endl;
				strcpy (title, TITLE);
				strcat (title, " Player 2 wins!");
				SDL_SetWindowTitle (mainWindow, title);
				win = true;
			}
			if (std::find (beg, en, cpu_p2) == en) {
				std::cerr << "Player 1 wins!" << std::endl;
				strcpy (title, TITLE);
				strcat (title, " Player 1 wins!");
				SDL_SetWindowTitle (mainWindow, title);
				win = true;
			}
		}
		SDL_Delay (delay);
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE || ev.type == SDL_QUIT) {
				exit = true;
			}
		}
	} while (!exit);

	return 0;
}
