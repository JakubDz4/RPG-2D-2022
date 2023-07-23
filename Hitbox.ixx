export module Hitbox;
import Structs;
import Hitline;
import <vector>;

struct Circle_part
{
	vec2d oryg_pos;
	vec2d pos;

	double r;
	int hp;

	Circle_part(vec2d p, double _r, int _hp) :pos(p), r(_r), hp(_hp) {}

	Circle_part(double x, double y, double _r, int _hp)
	{
		r = _r;
		hp = _hp;
		oryg_pos.x = x + r;
		oryg_pos.y = y + r;
		pos = oryg_pos;
	}
};

export class Hitbox
{
public:
	std::vector<Circle_part> part;
	//Hitline* hitedByline''

	void init(vec2d pos, double w, double h, int hp = 50)
	{
		double size = 0;
		if (w > h)
			size = h;
		else size = w;

		part.push_back({ pos, size / 2, hp });
	}

	void init(double x, double y, double r, double w2, double h2, double scale = 1, int hp = 50)
	{
		part.push_back({ (x - w2) * scale, (y - h2) * scale, r * scale, hp });
	}

	void calc_player_angle(double& cs, double& sn)
	{
		for (int i = 0; i < part.size(); i++)
		{
			part[i].pos.x = part[i].oryg_pos.x * cs - part[i].oryg_pos.y * sn;
			part[i].pos.y = part[i].oryg_pos.y * cs + part[i].oryg_pos.x * sn;
		}
	}

	void calc_Angle(double& asp, double& ac)
	{
		double  as = -asp;
		double tmp;

		for (int i = 0; i < part.size(); i++)
		{
			tmp = part[i].pos.x;
			part[i].pos.x = tmp * ac - part[i].pos.y * as;
			part[i].pos.y = part[i].pos.y * ac + tmp * as;
		}
	}
};