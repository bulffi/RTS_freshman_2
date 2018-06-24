#pragma once
#include<string>
class imformation
{
public:
	static std::string myname;
	static bool am_i_host;
	static bool am_i_in_room;
	static std::string my_host_name;
	static bool my_nation;
	static bool my_team;
	static int my_number;
	static int map_index;
	static int XMAX;
	static int YMAX;
	static int X_SIZE;
	static int Y_SIZE;
	static int X_BASE1;
	static int Y_BASE1;
	static int X_BASE2;
	static int Y_BASE2;
	static int X_BASE3;
	static int Y_BASE3;
	static int X_BASE4;
	static int Y_BASE4;
	//¿ó³µµÄÍßÆ¬×ø±ê
	static int  X_MINE1;
	static int  Y_MINE1;
	static int  X_MINE2;
	static int  Y_MINE2;
	static int  X_MINE3;
	static int  Y_MINE3;
	static int  X_MINE4;
	static int  Y_MINE4;
	static std::vector<std::tuple<std::string,bool,bool>> guest_list;//name nation team
};
