module;
#include <SDL.h>
#include <SDL_image.h>
export module Player;
import Structs;
import Player_Move_Stack;
import Virtuals;
import Animations_stuff;
import Weapons_Tools;
import Hitline;
import Hitbox;
//import Texture;
import<algorithm>;
import <vector>;

//import <iostream>;

export class player:public Moveable_obj
{
    double TWOPI = 6.2831853071795865;
public: 
    //pozycja i wektor przesuniecia (od moveable_obj) vec2d pos; vec2d mvec;
    int hp;
    //ustawienie na centrum ekranu, mob jako wartosc przemieszczenia wszystkich innych obiektow i cursor_vec jako przemieszczenie przez machanie kursorem
    vec2i center;//pozycja gracza na ekranie standardowa
    vec2d mov; ///move_world_wzgledem_center przesuneicie wzgledem center w ktorym znajduje sie gracz
    vec2i cursor_vec;//wektor przesuniecia dla center po przez ruch ekranu
    int range_cursor_mov; //maksymalne przesuniecie cursor_vec

    //zmienna do ustawienia predkosci postaci i stacki umozliwiajace przyjemne sterowanie klawiszami
    double vel = 4;
    Mstack sx;
    Mstack sy;

    double angle=0;///world angle
    double player_angle = 0;
    double psin = 0, pcos = 0, asin = 0, acos = 0;//wyliczenie cos i sin, mniej do kalkulacji
    bool focus;

    Animation *animation; //animacja dla walki
    Animation anim;
    Animation_frame* current_frame;//obecna klatka animacji i punkt na dloni
    Weapon *current_weapon;
    
    Hitline hitline;
    Hitbox hitbox;

    vec2d hand;
    vec2d seem_hand;
    vec2d seem_pos;

    void init(int x, int y, int centerx, int centery, Animation& whole_animation, Weapon& _weapon, double size = 100, int max_range_cursor_mov = 400, int _hp = 100)//size to max h
    {
        pos.x = x;
        pos.y = y;

        mvec.x = 0;
        mvec.y = 0;

        hp = _hp;
        
        center.x = centerx;
        center.y = centery;
        mov.x = center.x - pos.x;
        mov.y = center.y - pos.y;

        range_cursor_mov = max_range_cursor_mov;

        sx.vel = vel;
        sy.vel = vel;

        current_weapon = &_weapon;
        animation = &whole_animation;
        current_frame = &(*whole_animation.animations)[0];
        anim = whole_animation;

        //50 = h*scale

        double scale = 1;
        if((*whole_animation.animations)[0].h && size)
        scale = size / (double)(*whole_animation.animations)[0].h;
        
        hitline.init(current_weapon, angle);
       // hitbox.init(0, 0, current_frame->w, current_frame->h);
        double hw = (*whole_animation.animations)[0].dw2;
        double hh = (*whole_animation.animations)[0].dh2;

        hitbox.init(69, 12, 41, hw, hh, scale);
        hitbox.init(23, 24, 25, hw, hh, scale);
        hitbox.init(151, 25, 24, hw, hh, scale);

        animation->resize(scale);
    }

    void handleEvent(SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {
                case SDLK_UP:case SDLK_w: sy.add_min(); break;
                case SDLK_DOWN:case SDLK_s: sy.add_plus(); break;
                case SDLK_LEFT:case SDLK_a: sx.add_min(); break;
                case SDLK_RIGHT:case SDLK_d: sx.add_plus(); break;
                case SDLK_x: angle -= 0.174532925; if (angle < 0) angle += TWOPI; break; //1 degree = 0.0174532925 radians
                case SDLK_z: angle += 0.174532925; if (angle > TWOPI) angle -= TWOPI; break;
            }
        }
        //If a key was released
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {
                case SDLK_UP:case SDLK_w: sy.del_min(); break;
                case SDLK_DOWN:case SDLK_s: sy.del_plus(); break;
                case SDLK_LEFT:case SDLK_a: sx.del_min(); break;
                case SDLK_RIGHT:case SDLK_d: sx.del_plus(); break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            animation->frame.running = true;
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            animation->frame.running = false;
        }
    }

    void MousePos_cursorVec(int &Mx, int &My)
    { 
        int MX = Mx - center.x;
        int MY = My - center.y;

        if (range_cursor_mov > fabs(MX))
        {
            cursor_vec.x = -MX;
        }
        else
        {
            if (MX < 0)
                cursor_vec.x = range_cursor_mov;
            else cursor_vec.x = -range_cursor_mov;
        }

        if (range_cursor_mov > fabs(MY))
        {
            cursor_vec.y = -MY;
        }
        else
        {
            if (MY < 0)
                cursor_vec.y = range_cursor_mov;
            else cursor_vec.y = -range_cursor_mov;

        }

        player_angle = atan2(MX, -MY);
        psin = sin(player_angle);
        pcos = cos(player_angle);
        if (angle)
        {
            asin = sin(angle);
            acos = cos(angle);
        }
    }

    void updateAnimationsCoordinates_hitboxes()
    {
        current_frame = animation->ret_currentFrame();//&(*animations)[frame.result()];
        current_weapon->calc_weapon_end(current_frame->weapon_nvec, current_frame->degree);
        
        hitbox.calc_player_angle(pcos, psin);
        if (angle)
        hitbox.calc_Angle(asin, acos);

        hitline.calc_hand_weapon(current_frame, pcos, psin);
        hand = hitline.hand;
        if(angle)
        hitline.calc_hand_weapon_byAngle(asin,acos);
    }

    void transform_mvec()
    {
        mvec.x = sx.get();
        mvec.y = sy.get();

        //normalizing mvec
        if (mvec.y && mvec.x)
        {
            double leng = sqrt(mvec.x * mvec.x + mvec.y * mvec.y);
            if (leng)
            {
                mvec.x = vel * mvec.x / leng;
                mvec.y = vel * mvec.y / leng;
            }
        }

        //updating mvec to angle
        if (angle)
        {
            double tmp = mvec.x;
            mvec.x = mvec.x * acos + mvec.y * asin;///obracamy punkty 
            mvec.y = mvec.y * acos - tmp * asin;
        }

        
    }

    void move()//move and calc mov odpowaida za ruch myszki
    {
        pos.x += mvec.x;
        pos.y += mvec.y;
        mov.x = center.x - pos.x + cursor_vec.x;
        mov.y = center.y - pos.y + cursor_vec.y;
    }

    void draw_main(SDL_Renderer* renderer)
    {
        int x = center.x + cursor_vec.x;//przesuniecie ktore liczy w sobie central punkt i ruch myszki
        int y = center.y + cursor_vec.y;

        SDL_RendererFlip flip = SDL_FLIP_NONE;

        player_angle *= 57.2957795131; //180/pi

        //bron
        SDL_Rect r({ static_cast<int>(round(hand.x)) - current_weapon->center.x + x, static_cast<int>(round(hand.y)) - current_weapon->center.y + y, //rzmieszczenie broni
            current_weapon->w_hor, current_weapon->h_hor });
        SDL_RenderCopyEx(renderer, current_weapon->texture_horizontal, NULL, &r, player_angle+current_frame->angle, &current_weapon->center, flip);
        //

        //gracz
        r = { x - current_frame->w2 , y - current_frame->h2 , current_frame->w, current_frame->h };
        SDL_RenderCopyEx(renderer, current_frame->texture, NULL, &r, player_angle, NULL, flip);
        //

        //hitbox
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0x00);
       // SDL_RenderDrawLine(renderer, hand.x + x, hand.y + y, hitline.weapon.x + x, hitline.weapon.y + y);
        for (int i = 0; i < hitbox.part.size(); i++)
        DrawCircle(renderer, round(hitbox.part[i].pos.x + x), round(hitbox.part[i].pos.y + y), round(hitbox.part[i].r));
        //
    }

    void draw(SDL_Renderer* renderer, player& _player)
    {
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        double new_angle = player_angle + (_player.angle - angle) * 57.2957795131;// *360 / TWOPI - angle * 360 / TWOPI;//57.2957795131
        double x = _player.mov.x + seem_pos.x;
        double y = _player.mov.y + seem_pos.y;

        //bron, rzmieszczenie broni
        SDL_Rect r({ static_cast<int>(seem_hand.x - current_weapon->center.x + x),
            static_cast<int>(seem_hand.y - current_weapon->center.y + y), current_weapon->w_hor, current_weapon->h_hor });

        SDL_Texture* tmptext = SDL_CreateTextureFromSurface(renderer, current_weapon->surface_horizontal);

        SDL_RenderCopyEx(renderer, tmptext, NULL, &r, new_angle + current_frame->angle, &current_weapon->center, flip);
        SDL_DestroyTexture(tmptext);
        //

        //hand
        /*r = { static_cast<int>(seem_hand.x + x - 10), static_cast<int>(seem_hand.y + y - 10), 20, 20 };
        SDL_RenderDrawRect(renderer, &r);*/
        //
        
        //hitbox
        //DrawCircle(renderer, pos.x+x, pos.y+y, hitbox.part[0].r);
        //
        
        //gracz
        r = { static_cast<int>(x - current_frame->w2), static_cast<int>(y - current_frame->h2), current_frame->w, current_frame->h };

        tmptext = SDL_CreateTextureFromSurface(renderer, current_frame->surface);

        SDL_RenderCopyEx(renderer, tmptext, NULL, &r, new_angle, NULL, flip);

        SDL_DestroyTexture(tmptext);
        SDL_RenderDrawRect(renderer, &r);
        //

        //hitbox
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0x00);
      //  SDL_RenderDrawLine(renderer, hitline.hand.x + x, hitline.hand.y + y, hitline.weapon.x + x, hitline.weapon.y + y);
        DrawCircle(renderer, hitbox.part[0].pos.x + x, hitbox.part[0].pos.y + y, hitbox.part[0].r);
        //

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

    /*void focus_on(Moveable_obj& focus_obj)
    {
        angle = -atan2(focus_obj.pos.x - pos.x , pos.y - focus_obj.pos.y);
    }*/
};

/*   ~player() rozwizac problem (gdy dodajemy do vectora aktywuje sie destruktor
   {
       SDL_DestroyTexture(texture);
   }*/

   /*SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0x00);
   int x = center.x + cursor_vec.x;//przesuniecie ktore liczy w sobie central punkt i ruch myszki
   int y = center.y + cursor_vec.y;

   SDL_RendererFlip flip = SDL_FLIP_NONE;

   //int hx = current_frame->hand.x * cos(player_angle) - current_frame->hand.y * sin(player_angle);//pozycja dloni po obroceniu
   //int hy = current_frame->hand.y * cos(player_angle) + current_frame->hand.x * sin(player_angle);
   player_angle *= 360 / TWOPI;

   //bron
   SDL_Rect r({ static_cast<int>(hitline.seem_hand.x) - current_weapon->center.x + x, static_cast<int>(hitline.seem_hand.y) - current_weapon->center.y + y, //rzmieszczenie broni
       current_weapon->w_hor, current_weapon->h_hor });
   SDL_RenderCopyEx(renderer, current_weapon->texture_horizontal, NULL, &r, player_angle + current_frame->angle, &current_weapon->center, flip);
   //

   //gracz
   r = { x - current_frame->w2 , y - current_frame->h2 , current_frame->w, current_frame->h };
   SDL_RenderCopyEx(renderer, current_frame->texture, NULL, &r, player_angle, NULL, flip);
   //

   //bron_end_point
   SDL_RenderDrawLine(renderer, hitline.seem_hand.x + x, hitline.seem_hand.y + y,
       hitline.seem_hand.x + hitline.seem_weapon.x + x, hitline.seem_hand.y + hitline.seem_weapon.y + y);
   //
   */




   //punkt na rece
   /*SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
   int hx = current_frame->hand.x * cos(player_angle) - current_frame->hand.y * sin(player_angle);
   int hy = current_frame->hand.y * cos(player_angle) + current_frame->hand.x * sin(player_angle);
   r.x = x + hx - 5;
   r.y = y + hy - 5;
   r.w = 10; r.h = 10;
   SDL_RenderDrawRect(renderer, &r);*/
   //

   //wektor kierunkowy
   /*SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
   double t1 = mvec.x;
   double t2 = mvec.y;
   double tmp = t1;
   t1 = t1 * cos(angle) - t2 * sin(angle);///obracamy punkty
   t2 = t2 * cos(angle) + tmp * sin(angle);
   SDL_RenderDrawLine(renderer, x, y, t1 + x, t2 + y);*/
   //


//do playerdraw
/*double sn = sin(-angle);
        double cs = cos(-angle);
        double tmp = seem_hand.x;
        seem_hand.x = seem_hand.x * cs - seem_hand.y * sn;
        seem_hand.y = seem_hand.y * cs + tmp * sn;*/