
#pragma once

#include <SDL2/SDL.h>


#include "menu.h"
#include "dropdown.h"
#include "grid.h"

#include <vector>
#include <tuple>

class RenderWindow 
{	
	public:
		RenderWindow();
		void drawWindow(Menu &menu, Grid &g);
		void drawMenu(Menu &menu);
		void drawMenuBar(Menu &Menu);
		void drawButton(Button &button);
		void updateDropDown(DropDown &dropDown);
		void drawComboBox(DropDown &dropDown);
		void drawGrid(Grid &grid);

		void destroySDL();

		
	private:
	void drawGridLines(Grid &g);
	SDL_Texture* makeStringTexture(std::string text);
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
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