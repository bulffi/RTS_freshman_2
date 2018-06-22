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
	static std::vector<std::tuple<std::string,bool,bool>> guest_list;//name nation team
};
