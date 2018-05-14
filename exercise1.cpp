#include<string>
#include<iostream>
#include<cstdlib>
int main()
{
	int cnt=0;
	std::string s;
	while(std::cin>>s)
		if(s=="that")
			++cnt;
	std::cout<<cnt;
	system("pause");
	return 0;
}