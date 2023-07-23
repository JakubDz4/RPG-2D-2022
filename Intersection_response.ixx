export module Intersection_response;
import Wall;
//import Player;
import Structs;
import Virtuals;
import <vector>;
import <algorithm>;

export void intersection_result(const vec2dw& p, Moveable_obj& player)
{
    double przesx = player.pos.x + player.mvec.x;
    double przesy = player.pos.y + player.mvec.y;
    double prec = 0.00001;

    //ograniczenie istersekcji do zasiegu vectora przesuneicia gracza
    if (p.x - std::max(przesx, player.pos.x) < prec && p.x - std::min(przesx, player.pos.x) > -prec && 
        p.y - std::max(przesy, player.pos.y) < prec && p.y - std::min(przesy, player.pos.y) > -prec)
    {
        if (((przesx - p.x > -prec && player.pos.x - p.x < prec) || (przesx - p.x <prec && player.pos.x - p.x >-prec)) &&
            ((przesy - p.y > -prec && player.pos.y - p.y < prec) || (przesy - p.y < prec && player.pos.y - p.y>-prec)))
            ///czy gracz leci w kierunku sciany
        {
            //pokaz punkt intersekcji
            /*SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
            int x = p.x - 5;
            int y = p.y - 5;
            SDL_Rect rec({ x, y ,10,10 });
            SDL_RenderDrawRect(renderer, &rec);*/

            //vector od cx,cy do konczacego sie mvec
            vec2d A = { przesx - p.x,przesy - p.y };
            // A*norm/|norm|
            double proj_val = fabs(A.x * p.w->norm.x + A.y * p.w->norm.y);// /1

            vec2d tmp1 = { (player.mvec.x + p.w->norm.x) , (player.mvec.y + p.w->norm.y) };
            vec2d tmp2 = { (player.mvec.x - p.w->norm.x) , (player.mvec.y - p.w->norm.y) };

            //jezeli wektora po normalnej wzgelem lini jest wieksza wzgledem normalnej o odwroconych znakach, to znaczy ze wlasciwe
            if (tmp1.x * tmp1.x + tmp1.y * tmp1.y > tmp2.x * tmp2.x + tmp2.y * tmp2.y)
            {
                player.mvec.x = p.w->norm.x * proj_val;
                player.mvec.y = p.w->norm.y * proj_val;
            }
            else
            {
                player.mvec.x = p.w->norm.x * -proj_val;
                player.mvec.y = p.w->norm.y * -proj_val;
            }

            tmp1 = { A.x + p.w->norm_w.x , A.y + p.w->norm_w.y };
            tmp2 = { A.x - p.w->norm_w.x , A.y - p.w->norm_w.y };

            if (tmp1.x * tmp1.x + tmp1.y * tmp1.y < tmp2.x * tmp2.x + tmp2.y * tmp2.y)
            {
                player.mvec.x += p.w->norm_w.x;
                player.mvec.y += p.w->norm_w.y;
            }
            else
            {
                player.mvec.x -= p.w->norm_w.x;
                player.mvec.y -= p.w->norm_w.y;
            }

            player.mvec.x += p.x - player.pos.x;
            player.mvec.y += p.y - player.pos.y;
            // pos.x = cx;
            // pos.y = cy;
        }
    }
}

export void intersection_point(wall& w, std::vector<vec2dw>& inter_vec, Moveable_obj& player)//vec2d& r, vec2d& g, const vec2d& norm, const vec2d& norm_w)
{
    double przesx = player.pos.x + player.mvec.x;
    double przesy = player.pos.y + player.mvec.y;
    double prec = 0.00001;

    double A1, A2, B1, B2, C1, C2;
    A1 = przesy - player.pos.y;
    A2 = w.g.y - w.r.y;
    B1 = player.pos.x - przesx;
    B2 = w.r.x - w.g.x;
    C1 = A1 * player.pos.x + B1 * player.pos.y;
    C2 = A2 * w.r.x + B2 * w.r.y;

    if (A1 / B1 == A2 / B2)///a == a
    {
        if (C1 == C2)
            return;///linie sie pokrywaja
        return;///linie sa rownolegle ale sie nie pokrywaja
    }

    double denominator = (A1 * B2 - A2 * B1);
    if (!denominator)
        return;

    double cx, cy;
    cx = (B2 * C1 - B1 * C2) / denominator;
    //ograniczenie intereskcji do rozmiaru lini(sciany), by ponad nia nie bylo intersekcji
    if (cx - std::max(w.r.x, w.g.x) <= prec && cx - std::min(w.r.x, w.g.x) >= -prec)
    {
        cy = (A1 * C2 - A2 * C1) / denominator;
        if (cy - std::max(w.r.y, w.g.y) <= prec && cy - std::min(w.r.y, w.g.y) >= -prec)
        {
            //ograniczenie istersekcji do zasiegu vectora przesuneicia gracza
            if (cx - std::max(przesx, player.pos.x) < prec && cx - std::min(przesx, player.pos.x) > -prec && 
                cy - std::max(przesy, player.pos.y) < prec && cy - std::min(przesy, player.pos.y) > -prec)
            {
                if (((przesx - cx > -prec && player.pos.x - cx < prec) || (przesx - cx <prec && player.pos.x - cx >-prec)) &&
                    ((przesy - cy > -prec && player.pos.y - cy < prec) || (przesy - cy < prec && player.pos.y - cy>-prec)))
                    ///czy gracz leci w kierunku sciany
                {
                    inter_vec.push_back({ cx, cy, w });
                    return;
                }
            }
            else return;
        }
        else return;
    }
    else return;
}

export vec2dw closest_intersection(std::vector<vec2dw>& p, Moveable_obj& player)//, vec2d& pos)
{
    if (p.size() == 1)
        return p[0];

    int num = 0;

    vec2dw max = p[0];
    max.x -= player.pos.x;
    max.y -= player.pos.y;
    for (int i = 1; i < p.size(); i++)
    {
        double x = p[i].x - player.pos.x;
        double y = p[i].y - player.pos.y;
        if (x * x + y * y < max.x * max.x * max.y * max.y)
        {
            num = i;
            max = p[i];
            max.x -= player.pos.x;
            max.y -= player.pos.y;
        }
    }
    return p[num];
}

export void intersection_calc_mvec(std::vector<wall>& w, Moveable_obj& p)
{
    std::vector<vec2dw> inters_points;

    int i = 0;//ograniczenie petli
    while (i != 10)
    {
        for (int i = 0; i < w.size(); i++)///sprawdzamy dla kazdej sciany czy wystepuje intersekcja z wektorem przesuniecia
        {
            intersection_point(w[i], inters_points, p);
        }

        if (inters_points.size())///jezeli istnieje chociaz 1 interekcja to:
        {
            vec2dw closest_inters_point = closest_intersection(inters_points, p);///obliczamy najblizsza interekcje do gracza
            intersection_result(closest_inters_point, p);///obliczamy i zmieniamy odpowiednio wektor przesuniecia
            inters_points.clear();
        }
        else break;

        i++;
    }
}