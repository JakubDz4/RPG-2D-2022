module;
#include <SDL.h>
#include <SDL_image.h>
export module Animations_stuff;
import Structs;
import Weapons_Tools;
import <vector>;
import <string>;

export struct Animation_frame
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;
	vec2d hand;//punkt na rece gdzie bedzie trzymana bron/narzedzie,
	double degree;//vertical - kat pod jakim bron jest trzymana (0 czyli rownolegle do ekranu, na + gdy nad ekranem (postac celuje w gore)
	double angle;//horizontal- kat pod jakim bron jest odrwocona wzgledem ekranu (czy prosto: | czy pod katem: \)
	int w, h, w2, h2;
	double dw2, dh2;
	bool hurt_frame, vertical;//hitbox - czy obliczac intrsekcje z hitline, vertical - ktora wersja tekstury broni, jezeli true to ta wertykalna
	vec2d weapon_nvec;//znormalizowany vector broni

	void init(SDL_Renderer* renderer, std::string path, int handx, int handy, double _angle = 0,
		double _degree = 0, bool _hurt_frame = true, bool _vertical = false)
	{
		texture = IMG_LoadTexture(renderer, path.c_str());
		surface = IMG_Load(path.c_str());

		angle = _angle;
		degree = _degree;
		hurt_frame = _hurt_frame;
		vertical = _vertical;

		SDL_QueryTexture(texture, NULL, NULL, &w2, &h2);

		w = w2;
		h = h2;
		dw2 = w / 2;
		dh2 = h / 2;
		w2 = round(dw2);
		h2 = round(dh2);
		
		hand.x = handx - dw2;
		hand.y = handy - dh2;

		weapon_nvec.x = sin(angle*3.14159265359 / 180);
		weapon_nvec.y = -cos(angle*3.14159265359 / 180);

		/*weapon_nvec.x = hand.x * 1000;
		weapon_nvec.y = hand.y * 1000;

		double wx = weapon_nvec.x * cos(angle) - weapon_nvec.y * sin(angle);
		double wy = weapon_nvec.y * cos(angle) + weapon_nvec.x * sin(angle);
		double wlen = sqrt(wx * wx + wy * wy);

		weapon_nvec.x = wx/wlen;
		weapon_nvec.y = wy/wlen;*/
	}

	void resize(double scale)
	{
		w = round(w*scale);
		dw2 *= scale;
		w2 = round(dw2);

		h = round(h*scale);
		dh2 *= scale;
		h2 = round(dh2);

		hand.x *= scale;
		hand.y *= scale;
	}

	void clean()
	{
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

};

export struct Frame
{
	bool running;
	int frame;
	int max_frame;
	int one_frame_time;
	int frame_nums;
	int ret;

	void init(int _frame_nums, int _one_frame_time = 20)
	{
		running = false;
		frame = 0;
		max_frame = _frame_nums * _one_frame_time;
		one_frame_time = _one_frame_time;
		frame_nums = _frame_nums;
		ret = 0;
	}

	int result()
	{
		if (running || frame)
		{
			if (running == false && frame == 0)
			{
				ret = 0;
				return ret;
			}
			else if (frame == 0 && running)
			{
				frame = max_frame;
			}

			if (frame % one_frame_time == 0)
			{
				ret++;
				if (ret == frame_nums)
					ret = 0;
			}
			frame--;
		}
		return ret;
	}
};

export struct Animation
{
	std::vector<Animation_frame>* animations = NULL;
	//Animation_frame* current_frame;//obecna klatka animacji i punkt na dloni
	Frame frame;

	void init(std::vector<Animation_frame>& _animations)
	{
		animations = &_animations;
		//current_frame = &(*animations)[0];// jezeli jako argument bylby std::vector<Animation>* _animations
		frame.init(_animations.size());
	}

	//*(tab+i) == tab[i]
	Animation_frame * ret_currentFrame()
	{
		return &(*animations)[frame.result()];
	}

	void resize(double scale)
	{
		for (int i = 0; i < animations->size(); i++)
		{
			(*animations)[i].resize(scale);
		}
	}
};