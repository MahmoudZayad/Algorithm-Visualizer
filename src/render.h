
#pragma once

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "grid.h"
#include <iostream>
#include <vector>
#include <tuple>

class RenderWindow 
{	
	public:
		RenderWindow();
		SDL_Renderer* getRenderer();
		SDL_Window* getWindow();
		void render(Grid& grid, ImGuiIO& io);
		void destroySDL();

		void setupImGuiContext();
		void setupRenderPlatform();
		void startImGuiFrame();
		void destroyImGui();
		
		void drawGrid(Grid& grid);
		
	private:
		void drawGridLines(Grid &g);
		
		SDL_Window* window;
		SDL_Renderer* renderer;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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