#include <vector>
#include <string>

enum OptType
{
	kEncrypt,
	kDecrypt
};

void Log(const std::string msg);
void LogWarn(const std::string msg);
void LogError(const std::string msg);

void gen_key(std::vector<unsigned char>& main_key);
std::string int_to_hexstr(const int int_value);
void str_to_vect(const std::string& str, std::vector<unsigned char>& vect);
void print_vect(const std::vector<unsigned char>& vect, const std::string& msg);
void memset_vect(std::vector<unsigned char>& vect, int value, int begin, int size = 0);
void memcpy_vect(std::vector<unsigned char>& des_vect, const std::vector<unsigned char>& src_vect, int size = 0);
void sub_vect(const std::vector<unsigned char>& in_vect, int begin, int size, std::vector<unsigned char>& out_vect);
void vect_xor(std::vector<unsigned char>& des_vect, const std::vector<unsigned char> src_vect);
void concat_vect(std::vector<unsigned char>& des_vect, const std::vector<unsigned char>& src_vect);