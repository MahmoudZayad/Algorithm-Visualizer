
#pragma once

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "grid.h"

#include <vector>
#include <tuple>

class RenderWindow 
{	
	public:
		RenderWindow();
		void drawWindow(SDL_Renderer* renderer, Grid &g);
		void drawGrid(SDL_Renderer* renderer, Grid &grid);
		void destroySDL(SDL_Renderer* renderer, SDL_Window* window);

		
	private:
	void drawGridLines(SDL_Renderer* renderer, Grid &g);
	// SDL_Window* window;
	// SDL_Renderer* renderer;

};






// /*
//  * Make textures for all button texts and return a vector to be used to place them on SDL_Rects
// */
// std::vector<SDL_Texture*> makeButtonTextures(SDL_Renderer* renderer, TTF_Font* font, std::vector<char*> buttonText) {
//     std::vector<SDL_Texture*> textures;
    
//     for (auto &text : buttonText) {
//         textures.push_back(makeStringTexture(renderer, font, text));
//     }

//     return textures;
// }