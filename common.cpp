#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include "common.h"
using namespace std;

void LogError(const string msg)
{
	cout << "ERROR: " << msg << endl;
}

void LogWarn(const string msg)
{
	cout << "WARN : " << msg << endl;
}

void Log(const string msg)
{
	cout << "INFO : " << msg << endl;
}

void gen_key(vector<unsigned char>& main_key)
{
	for (auto i = 0; i < main_key.size(); i++)
	{
		main_key[i] = rand() % 255;
	}
}

void print_vect(const vector<unsigned char>& vect, const string& msg)
{
	cout << "START  : " << msg << endl;
	cout << "ASCII  : ";
	for (auto i=0; i<vect.size(); i++)
	{
		cout << vect[i];
	}
	cout << endl;

	cout << "Base64 : ";
	int tmp = 0;
	for (auto i=0; i<vect.size(); i++)
	{
		cout << int_to_hexstr(vect[i]);
	}
	cout << endl;
	cout << "END    : " << msg << endl;
	cout << endl;
}

string int_to_hexstr(const int int_value)
{
	string result;
	if (int_value <= 255 && int_value >= 0)
	{
		auto int_to_hexchar = [](int v){
			if (v < 10)
			{
				return to_string(v);
			}
			else
			{
				return string(1,(char)(v - 10 + 'A'));
			}
		};
		int tmp = int_value / 16;
		result += int_to_hexchar(tmp);
		tmp = int_value % 16;
		result += int_to_hexchar(tmp);
	}
	else
	{
		LogError("int_value range error! int_value = " + to_string(int_value));
	}
	return result;
}

void str_to_vect(const string& str, vector<unsigned char>& vect)
{
	vect.clear();
	for (auto i=0; i<str.size(); i++)
	{
		vect.push_back(str[i]);
	}
}

void memset_vect(vector<unsigned char>& vect, int value, int begin, int size)
{
	int end = begin + size;
	if (size == 0)
	{
		end = vect.size();
	}
	for (auto i=begin; i<end; i++)
	{
		if (i < vect.size())
		{
			vect[i] = value;
		}
		else
		{
			vect.push_back(value);
		}
	}
}

void memcpy_vect(vector<unsigned char>& des_vect, const vector<unsigned char>& src_vect, int size)
{
	if (size==0)
	{
		size = src_vect.size();
	}

	for (auto i=0; i<size; i++)
	{
		des_vect[i] = src_vect[i];
	}
}

void sub_vect(const vector<unsigned char>& in_vect, int begin, int size, vector<unsigned char>& out_vect)
{
	if (begin<0 || begin>=in_vect.size())
	{
		LogError("sub_vect range error 1! begin = " + to_string(begin) + " vect size = " + to_string(in_vect.size()));
	}
	else
	{
		int end = begin + size;
		if (size == 0 || end > in_vect.size())
		{
			if (end>in_vect.size())
			{
				LogWarn("sub_vect range error 2! begin = " + to_string(begin) + " size = " + to_string(size) + " vect size = " + to_string(in_vect.size()));
			}
			end = in_vect.size();
		}

		out_vect.clear();
		for (auto i=begin; i<end; i++)
		{
			out_vect.push_back(in_vect[i]);
		}
	}
}

void vect_xor(vector<unsigned char>& des_vect, const vector<unsigned char> src_vect)
{
	if (des_vect.size() != src_vect.size())
	{
		LogError("des_vect.size!=src_vect.size");
	}
	else
	{
		for (auto i=0; i<src_vect.size(); i++)
		{
			des_vect[i] ^= src_vect[i];
		}
	}
}

void concat_vect(vector<unsigned char>& des_vect, const vector<unsigned char>& src_vect)
{
	for (auto i=0; i<src_vect.size(); i++)
	{
		des_vect.push_back(src_vect[i]);
	}
}