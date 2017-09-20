#include <iostream>
#include "des.h"
#include "aes.h"
#include "tea.h"
using namespace std;

int main(int argc, char* argv[])
{
	main_des();
	main_aes();
	main_tea();
	//main_tea_test();
	return 0;
}