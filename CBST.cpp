#include "CBST.h"
#include <iostream>

enum class MY_TYPE
{
	TYPE_1, //0 
};

enum class OTHER_MY_TYPE
{
	TYPE_1,
};

int main(void)
{
	unsigned char value = (unsigned char)MY_TYPE::TYPE_1; //class를 도입하면, 자료형 바꿔줘야함.


	CBST<int, int> bstint;
	bstint.insert(make_bstpair(100, 0));
	bstint.insert(make_bstpair(150, 0));
	bstint.insert(make_bstpair(50, 0));
	bstint.insert(make_bstpair(25, 0));
	bstint.insert(make_bstpair(125, 0));
	bstint.insert(make_bstpair(175, 0));
	CBST<int, int>::iterator Iter = bstint.begin();
	Iter = bstint.find(50);
	Iter = bstint.erase(Iter);

	for (Iter; Iter != bstint.end(); ++Iter)
	{
		std::wcout << L"first   : " << Iter->first << std::endl;
		std::wcout << L"sencond : " << Iter->second << std::endl;
	}
	return 0;
}