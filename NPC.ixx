module;
#include <SDL.h>
export module NPC;
import Structs;
import Player;
import Virtuals;

//import <iostream>;

//export class NPC :public Moveable_obj
//{
//public:
//	int hp;
//    int w, h;
//    double angle;//w ktorym kierunku odwrocony
//    double vel;
//
//    vec2d seem;//how palyers see it, nawiazuje do wygladu po rotate_world w tansform w menu
//
//    virtual void draw(SDL_Renderer* renderer, player& player){}
//
//    //ponizej do wywalenia i przerzucenia na inna klase
//
//    virtual void set_mvec(player& player){}
//
//    virtual void move(){}
//};
//
//export class Human :public NPC
//{
//public:
//    void draw(SDL_Renderer* renderer, player& player)
//    {
//        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
//        SDL_Rect r({ static_cast<int>(seem.x - 10 + player.mov.x), static_cast<int>(seem.y - 10 + player.mov.y), w, h });
//        SDL_RenderDrawRect(renderer, &r);
//        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
//        SDL_RenderDrawLine(renderer, seem.x + player.mov.x, seem.y + player.mov.y,
//            seem.x + w / 2 * sin(angle - player.angle) + player.mov.x, seem.y + h / 2 * cos(angle - player.angle) + player.mov.y);
//    }
//
//    void set_mvec(player& player)
//    {
//        mvec.x = player.pos.x - pos.x;
//        mvec.y = player.pos.y - pos.y;
//
//        angle = atan2(mvec.x, mvec.y);
//        double leng = sqrt(mvec.x * mvec.x + mvec.y * mvec.y);
//        if (leng)
//        {
//            mvec.x = mvec.x / leng * vel;
//            mvec.y = mvec.y / leng * vel;
//        }
//        else
//        {
//            mvec.x = 0;
//            mvec.y = 0;
//        }
//    }
//
//    void move()
//    {
//        pos.x += mvec.x;
//        pos.y += mvec.y;
//    }
//};

export class NPC :public Moveable_obj
{
public:
    int hp;
    int w, h;

    double angle;//w ktorym kierunku odwrocony
    double vel;

    vec2d seem;//how palyers see it, nawiazuje do wygladu po rotate_world w tansform w menu

    NPC(int x, int y, int _hp = 100, double _angle = 0, double _vel = 1, int _w = 20, int _h = 20)
        :hp(_hp), angle(_angle), vel(_vel), w(_w), h(_h)
    {
        pos.x = x;
        pos.y = y;
    };

    void draw(SDL_Renderer* renderer, player& player)
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
        SDL_Rect r({ static_cast<int>(seem.x - 10 + player.mov.x), static_cast<int>(seem.y - 10 + player.mov.y), w, h });
        SDL_RenderDrawRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
        SDL_RenderDrawLine(renderer, seem.x + player.mov.x, seem.y + player.mov.y,
            seem.x + w / 2 * sin(angle - player.angle) + player.mov.x, seem.y + h / 2 * cos(angle - player.angle) + player.mov.y);
    }

    //ponizej do wywalenia i przerzucenia na inna klase

    void set_mvec(player& player)
    {
        mvec.x = player.pos.x - pos.x;
        mvec.y = player.pos.y - pos.y;

        angle = atan2(mvec.x, mvec.y);

        double leng = sqrt(mvec.x * mvec.x + mvec.y * mvec.y);
        if (leng)
        {
            mvec.x = mvec.x / leng * vel;
            mvec.y = mvec.y / leng * vel;
        }
        else
        {
            mvec.x = 0;
            mvec.y = 0;
        }

    }

    void move()
    {
        pos.x += mvec.x;
        pos.y += mvec.y;
    }
};