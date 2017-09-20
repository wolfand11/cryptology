#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include "common.h"
#include "tea.h"
using namespace std;

void process_message(uint32_t* in_out_data_piece, uint32_t* main_key, OptType opt_type)
{
	uint32_t delta = 0x9e3779b9;                     /* a key schedule constant */
	uint32_t v0 = in_out_data_piece[0], v1 = in_out_data_piece[1], sum = 0, i;   /* set up */
	uint32_t k0 = main_key[0], k1 = main_key[1], k2 = main_key[2], k3 = main_key[3];   /* cache key */
	if (opt_type==OptType::kEncrypt)
	{
		for (i=0; i<32; i++)
		{
			sum += delta;
			v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
			v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		}
	}
	else if (opt_type==OptType::kDecrypt)
	{
		sum = 0xC6EF3720;
		for (i=0; i<32; i++)
		{
			v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
			v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
			sum -= delta;
		}
	}
	in_out_data_piece[0] = v0;
	in_out_data_piece[1] = v1;
}

void do_crypt(const vector<unsigned char>& in_data, OptType opt_type, int piece_size, const vector<unsigned char>& main_key, vector<unsigned char>& out_data)
{
	out_data.clear();

	int block_count = 0;
	int number_of_blocks = in_data.size() / piece_size + (in_data.size() % piece_size ? 1 : 0);
	uint32_t tmp_inout_data_piece[2];
	uint32_t tmp_main_key[4];
	assign_t_with_vect<uint32_t>(main_key, (uint32_t*)tmp_main_key, 4);
	vector<unsigned char> in_data_piece(piece_size, 0);
	vector<unsigned char> out_data_piece(piece_size, 0);
	vector<unsigned char> tmp_data_piece(piece_size, 0);
	vector<unsigned char> init_vector(piece_size, 0);
	for (auto i=0; i<init_vector.size(); i++)
	{
		init_vector[i] = i % 255;
	}
	while (true)
	{
		sub_vect(in_data, block_count * piece_size, piece_size, in_data_piece);
		block_count++;
		if (block_count == number_of_blocks)
		{
			if (opt_type == OptType::kEncrypt)
			{
				int padding = piece_size - in_data.size() % piece_size;
				if (padding < piece_size)
				{
					memset_vect(in_data_piece, padding, piece_size - padding, padding);
				}

				vect_xor(in_data_piece, init_vector);
				assign_t_with_vect<uint32_t>(in_data_piece, (uint32_t*)tmp_inout_data_piece, 2);
				process_message(tmp_inout_data_piece, tmp_main_key, opt_type);
				assign_vect_with_t(out_data_piece, tmp_inout_data_piece, 2);
				memcpy_vect(init_vector, out_data_piece);
				concat_vect(out_data, out_data_piece);

				if (padding==piece_size)
				{
					memset_vect(in_data_piece, padding, 0, padding);
					vect_xor(in_data_piece, init_vector);
					assign_t_with_vect<uint32_t>(in_data_piece, (uint32_t*)tmp_inout_data_piece, 2);
					process_message(tmp_inout_data_piece, tmp_main_key, opt_type);
					assign_vect_with_t(out_data_piece, tmp_inout_data_piece, 2);
					memcpy_vect(init_vector, out_data_piece);
					concat_vect(out_data, out_data_piece);
				}
			}
			else
			{
				memcpy_vect(tmp_data_piece, in_data_piece);
				assign_t_with_vect<uint32_t>(in_data_piece, (uint32_t*)tmp_inout_data_piece, 2);
				process_message(tmp_inout_data_piece, tmp_main_key, opt_type);
				assign_vect_with_t(out_data_piece, tmp_inout_data_piece, 2);
				vect_xor(out_data_piece, init_vector);
				memcpy_vect(init_vector, tmp_data_piece);
				int padding = out_data_piece[piece_size-1];
				if (padding < piece_size)
				{
					concat_vect(out_data, out_data_piece);
				}
			}

			break;
		}
		else
		{
			if (opt_type == OptType::kEncrypt)
			{
				vect_xor(in_data_piece, init_vector);
				assign_t_with_vect<uint32_t>(in_data_piece, (uint32_t*)tmp_inout_data_piece, 2);
				process_message(tmp_inout_data_piece, tmp_main_key, opt_type);
				assign_vect_with_t(out_data_piece, tmp_inout_data_piece, 2);
				memcpy_vect(init_vector, out_data_piece);
				concat_vect(out_data, out_data_piece);
			}
			else 
			{
				memcpy_vect(tmp_data_piece, in_data_piece);
				assign_t_with_vect<uint32_t>(in_data_piece, (uint32_t*)tmp_inout_data_piece, 2);
				process_message(tmp_inout_data_piece, tmp_main_key, opt_type);
				assign_vect_with_t(out_data_piece, tmp_inout_data_piece, 2);
				vect_xor(out_data_piece, init_vector);
				memcpy_vect(init_vector, tmp_data_piece);
				concat_vect(out_data, out_data_piece);
			}
		}
	}
}

void main_tea()
{
	cout << endl;
	Log("TEA-START-------");

	// gen main key
	// tea key length 128
	vector<unsigned char> main_key(16, 0);
	gen_key(main_key);
	print_vect(main_key, "main_key");

	// gen sub_keys

	string plain_text = "my name is wolfand11! i'm a programmer. this is a test text for crypt example!";
	////string plain_text = "1111111100000000";
	vector<unsigned char> plain_data;
	str_to_vect(plain_text, plain_data);
	print_vect(plain_data, "plain_data");

	vector<unsigned char> cipher_data;
	do_crypt(plain_data, OptType::kEncrypt,  8, main_key, cipher_data);
	print_vect(cipher_data, "cipher_data");

	vector<unsigned char> plain_data_from_cipher;
	do_crypt(cipher_data, OptType::kDecrypt, 8, main_key, plain_data_from_cipher);
	print_vect(plain_data_from_cipher, "plain_data_from_cipher");
	Log("TEA-END---------\n");
}

void main_tea_test()
{
	vector<unsigned char> main_key(16, 0);
	gen_key(main_key);
	uint32_t tmp_main_key[4];
	assign_t_with_vect<uint32_t>(main_key, (uint32_t*)tmp_main_key, 4);
	print_vect(main_key, "test-a");

	//vector<unsigned char> out_data_piece(16, 0);
	//assign_vect_with_t(out_data_piece, (uint32_t*)tmp_main_key, 4);
	//print_vect(out_data_piece, "test-b");
}