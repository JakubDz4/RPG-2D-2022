export module Hitline;
import Structs;
import Weapons_Tools;
import Animations_stuff;

import<iostream>;

export class Hitline//player
{
public:
	vec2d *weapon_end;
	/*vec2d *hand;
	double* angle;*/
	double* angle;

	vec2d weapon;
	vec2d hand;

	void init(Weapon* cur_weapon, double& _angle)
	{
		weapon_end = &cur_weapon->weapon_end;
		/*hand = &cur_animation->hand;
		angle = &cur_animation->angle;*/
		angle = &_angle;
	}

	void calc_hand_weapon(Animation_frame* cur_animation, double& cs, double& sn)
	{
		weapon.x = weapon_end->x * cs - weapon_end->y * sn;
		weapon.y = weapon_end->y * cs + weapon_end->x * sn;
		
		hand.x = cur_animation->hand.x * cs - cur_animation->hand.y * sn;
		hand.y = cur_animation->hand.y * cs + cur_animation->hand.x * sn;

		weapon.x += hand.x;
		weapon.y += hand.y;
	}

	void calc_hand_weapon_byAngle(double& asp, double& ac)
	{
		double  as = -asp;

		double tmp = hand.x;
		hand.x = hand.x * ac - hand.y * as;
		hand.y = hand.y * ac + tmp * as;

		tmp = weapon.x;
		weapon.x = weapon.x * ac - weapon.y * as;
		weapon.y = weapon.y * ac + tmp * as;
	}
};