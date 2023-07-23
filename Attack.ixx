module;
#include <SDL.h>
export module Attack;
import Player;
import Hitline;

import <iostream>;

export struct Attack
{
	player* from;

	Attack(player* _player)
	{
		from = _player;
	}

	void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
	{
		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}

	void detect_hit(std::vector<player>* to, SDL_Renderer* renderer)
	{
		double Ax = from->hitline.weapon.x + from->pos.x;
		double Ay = from->hitline.weapon.y + from->pos.y;
		double Bx = from->hitline.hand.x + from->pos.x;
		double By = from->hitline.hand.y + from->pos.y;

		double LAB = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
		double Dx = (Bx - Ax) / LAB;
		double Dy = (By - Ay) / LAB;

		double mx = std::max(Ax, Bx);
		double my = std::max(Ay, By);
		double mix = std::min(Ax, Bx);
		double miy = std::min(Ay, By);

		for (int i = 0; i < to->size(); i++)
		{
			if (&(*to)[i] != from)
			{
				for (int j = 0; j < (*to)[i].hitbox.part.size(); j++)
				{
					double Cx = (*to)[i].hitbox.part[j].pos.x + (*to)[i].pos.x;
					double Cy = (*to)[i].hitbox.part[j].pos.y + (*to)[i].pos.y;

					double t = Dx * (Cx - Ax) + Dy * (Cy - Ay);

					double Ex = t * Dx + Ax;
					double Ey = t * Dy + Ay;

					if (mx < Ex)
					{
						Ex = mx;
					}
					else if (mix > Ex)
					{
						Ex = mix;
					}
					if (my < Ey)
					{
						Ey = my;
					}
					else if (miy > Ey)
					{
						Ey = miy;
					}

					//E is the point of AB closest the circle center(Cx, Cy)
					double LEC = sqrt((Ex - Cx) * (Ex - Cx) + (Ey - Cy) * (Ey - Cy));


					/*std::cout << i << " weaponx: " << Ax << " weapony: " << Ay << std::endl;
					std::cout << i << " hitboxx: " << Cx << " hitboxy: " << Cy << std::endl;
					std::cout << i << " handx: " << Bx << " handy: " << By << std::endl;*/
					if (LEC <= (*to)[i].hitbox.part[j].r)
					{
						(*to)[i].hp--;
						//std::cout << i << " hp: " << (*to)[i].hp << std::endl;
						/*double px = Ax * 2;
						double py = By * 2;
						SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
						SDL_RenderDrawLine(renderer, Ax+px, Ay+py, Bx+px, By+py);
						DrawCircle(renderer, (*to)[i].hitbox.part[j].pos.x, (*to)[i].hitbox.part[j].pos.y, (*to)[i].hitbox.part[j].r);*/
					}
				}
			}
		}
	}
};