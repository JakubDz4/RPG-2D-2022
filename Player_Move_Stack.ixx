export module Player_Move_Stack;

export struct Mstack
{
	int val[2];
	double vel;

	Mstack()
	{
		val[0] = 0;
		val[1] = 0;
	}

	void add_min()
	{
		val[1] = val[0];
		val[0] = -vel;
	}

	void del_min()
	{
		if (val[0]<0) 
		{
			val[0] = val[1];
			val[1] = 0;
		}
		else if (val[1] < 0)
		{
			val[1] = 0;
		}
	}

	void add_plus()
	{
		val[1] = val[0];
		val[0] = vel;
	}

	void del_plus()
	{
		if (val[0] > 0)
		{
			val[0] = val[1];
			val[1] = 0;
		}
		else if (val[1] > 0)
		{
			val[1] = 0;
		}
	}

	int get()
	{
		return val[0];
	}
};

