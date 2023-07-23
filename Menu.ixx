module;
#include <SDL.h>
#include <SDL_image.h>
export module Individual_Menu;
export import <vector>;
export import Player;
export import Wall;
import Animations_stuff;
import Intersection_response;
import Rotate_world;
import Weapons_Tools;
import Attack;
export import NPC;

import <iostream>;

export class Individual_Menu
{
	int WindowID;
	int screen_x, screen_y, screen_w, screen_h;
	int main_player_id;
	int Mx=0, My=0;

public:
	std::vector<player> *players;
	std::vector<NPC> *npc;
	std::vector<wall> *w;
	std::vector<Animation_frame> animations;//zrobic vec2 dimensional
	Animation whole_animation;
	std::vector<Weapon> weapons;
	std::vector<Attack> attack;

	player* main_player;//glowny gracz tego menu

	//SDL_Event e;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	

	Individual_Menu(std::vector<player>& _players, std::vector<NPC>& _npc, std::vector<wall>& _w, int scr_x = 0, int scr_y = 0, int scr_w = 1200, int scr_h = 800)
	{
		players = &_players;
		npc = &_npc;
		w = &_w;
		players->push_back({});
		main_player = &_players[_players.size() - 1];//UWAGA!!!!! NIE!!! PONIEWAZ VECTOR ZMIENIA DANE!!!!!!
		main_player_id = _players.size() - 1;
		screen_x = scr_x;
		screen_y = scr_y;
		screen_w = scr_w;
		screen_h = scr_h;
	}

	/*~Individual_Menu()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		for (int i = 0; i < animations.size(); i++)
		{
			animations[i].clean();
		}

		for (int i = 0; i < weapons.size(); i++)
		{
			weapons[i].clean();
		}

		IMG_Quit();
		SDL_Quit();
	}*/

	bool init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
			return false;
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		window = SDL_CreateWindow("RPG", screen_x, screen_y, screen_w, screen_h, NULL);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		WindowID = SDL_GetWindowID(window);

		animations.reserve(6);
		//animations[0].init(renderer, "image.png", 205, 15, 0);
		animations ={{},{},{}};
		animations[0].init(renderer, "image.png", 205, 15, 10);
		animations[1].init(renderer, "image2.png", 176, 8, -70);
		animations[2].init(renderer, "image3.png", 194, 15, -10);

		whole_animation.init(animations);

		weapons = { {} };

		weapons[0].init(renderer, 65, 486, 120 * players->size(), "weapon.png");
		main_player->init(0, 0, screen_w / 2, screen_h / 2, whole_animation, weapons[0], 100);//

		return true;
	}

	inline void calc_anims_and_hitboxes()
	{
		main_player->MousePos_cursorVec(Mx, My);
		main_player->updateAnimationsCoordinates_hitboxes();

		if (main_player->current_frame->hurt_frame)
			attack.push_back(main_player);

		for (int i = 0; i < attack.size(); i++)
		{
			attack[i].detect_hit(players, renderer);
		}

		attack.clear();

	}

	inline void calc_mvecs()//handle event and update anim and hits
	{
		main_player->transform_mvec();
		intersection_calc_mvec(*w, *main_player);

		for (int i = 0; i < npc->size(); i++)
		{
			(*npc)[i].set_mvec(*main_player);
			intersection_calc_mvec(*w, (*npc)[i]); //przeniesc te operacje na server
		}
	}

	inline void transform_SetSeem()//ex rotate or scale and setSeem
	{
		if (main_player->psin > 10)
			std::cout << "UWAGA!";

		for (int i = 0; i < w->size(); i++)
			(*w)[i].seem = rotate((*w)[i], *main_player);

		for (int i = 0; i < npc->size(); i++)
			(*npc)[i].seem = rotate((*npc)[i], *main_player);

		for (int i = 0; i < players->size(); i++)
		{
			if (i != main_player_id)
			{
				rotate((*players)[i], *main_player);
			}
		}
	}

	inline void move()
	{
		main_player->move();

		for (int i = 0; i < npc->size(); i++)//przeniesc te operacje na server
		(*npc)[i].move();
	}

	inline void render()
	{
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		for (int i = 0; i < w->size(); i++)///taka sama petla jest w intersection
			(*w)[i].draw(renderer, *main_player);

		for (int i = 0; i < npc->size(); i++)
			(*npc)[i].draw(renderer, *main_player);

		for (int i = 0; i < players->size(); i++)
		{
			if (main_player_id != i)
			(*players)[i].draw(renderer, *main_player);
		}

		main_player->draw_main(renderer);

		SDL_RenderPresent(renderer);
	}

	void main_loop(SDL_Event &e)
	{
		if (e.window.windowID == WindowID)
		{
			main_player->handleEvent(e);//set_mvec by that
			SDL_GetMouseState(&Mx, &My);
		}

		calc_anims_and_hitboxes();
		calc_mvecs();
		transform_SetSeem();//ex rotate or scale
		move();
		render();

		/*std::cout << "handx: " << main_player->current_frame->hand.x << " handy: " << main_player->current_frame->hand.x <<
			"weaponx: " << main_player->current_weapon->weapon_end.x << " weapony: " << main_player->current_weapon->weapon_end.y << std::endl;*/

		/*std::cout << "LINE: handx: " << main_player->hitline.hand.x << " handy: " << main_player->hitline.hand.y <<
			"weaponx: " << main_player->hitline.weapon.x << " weapony: " << main_player->hitline.weapon.y << std::endl;
		std::cout << main_player->pos.x << std::endl;*/
		//SDL_Delay(30);
	}

	//void start()
	//{
	//	if (init());
	//	main_loop();
	//}

	void clean()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		for (int i = 0; i < animations.size(); i++)
		{
			animations[i].clean();
		}

		for (int i = 0; i < weapons.size(); i++)
		{
			weapons[i].clean();
		}

		IMG_Quit();
		SDL_Quit();
	}
};

//if (true)//focus
//main_player->focus_on((*npc)[0]);//tricky dereference the pointer https://stackoverflow.com/questions/11587526/how-to-read-element-from-a-vector-pointer/11587545
