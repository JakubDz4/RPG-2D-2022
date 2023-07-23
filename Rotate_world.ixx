export module Rotate_world;
import Structs;
import Virtuals;
import Player;
import Wall;
import <math.h>;

import <iostream>;

export vec4d rotate(wall &w, player& player)
{
    double sn = sin(player.angle);
    double cs = cos(player.angle);

    vec2d R, G;
    R.x = w.r.x - player.pos.x;///odejmujemy od pozycji pozycje player by miec wektor od punktu 0.0
    R.y = w.r.y - player.pos.y;
    G.x = w.g.x - player.pos.x;
    G.y = w.g.y - player.pos.y;

    double tmpr = R.x;
    R.x = R.x * cs - R.y * sn;///obracamy punkty 
    R.y = R.y * cs + tmpr * sn;

    double tmpg = G.x;
    G.x = G.x * cs - G.y * sn;
    G.y = G.y * cs + tmpg * sn;

    R.x += player.pos.x;///dodajemy przedtem odejmowana pozycje
    R.y += player.pos.y;
    G.x += player.pos.x;
    G.y += player.pos.y;

    //w.change_wall_by_rotate(R.x, R.y, G.x, G.y);///robi to co constructor, zmienia vectory nomalne etc

    return vec4d(R.x, R.y, G.x, G.y);
}

export vec2d rotate(Moveable_obj& npc, player& player)
{
    vec2d R = npc.pos;

    if (player.angle)
    {
        double sn = sin(player.angle);
        double cs = cos(player.angle);


        R = R - player.pos;///odejmujemy od pozycji pozycje player by miec wektor od punktu 0.0

        double tmpr = R.x;
        R.x = R.x * cs - R.y * sn;///obracamy punkty 
        R.y = R.y * cs + tmpr * sn;

        R = R + player.pos;///dodajemy przedtem odejmowana pozycje
    }


    return R;
}

export void rotate(player& change_player, player& player)
{
    change_player.seem_hand = change_player.hand;
    //change_player.seem_weapon_end = change_player.hitline.weapon;
    change_player.seem_pos = change_player.pos ;

    if (player.angle)
    {
        double sn = sin(player.angle);
        double cs = cos(player.angle);

        change_player.seem_pos = change_player.seem_pos - player.pos;
        double tmp = change_player.seem_pos.x;
        change_player.seem_pos.x = change_player.seem_pos.x * cs - change_player.seem_pos.y * sn;
        change_player.seem_pos.y = change_player.seem_pos.y * cs + tmp * sn;
        change_player.seem_pos = change_player.seem_pos + player.pos;

        tmp = change_player.seem_hand.x;
        change_player.seem_hand.x = change_player.seem_hand.x * cs - change_player.seem_hand.y * sn;
        change_player.seem_hand.y = change_player.seem_hand.y * cs + tmp * sn;
    }
    if (change_player.angle)
    {
        double sn = sin(-change_player.angle);
        double cs = cos(-change_player.angle);
        double tmp = change_player.seem_hand.x;
        change_player.seem_hand.x = change_player.seem_hand.x * cs - change_player.seem_hand.y * sn;
        change_player.seem_hand.y = change_player.seem_hand.y * cs + tmp * sn;
    }
}