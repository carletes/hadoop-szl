//
// base64.cc
//

#include <hadoop-szl/base64.h>


using std::string;


int Base64Escape(const unsigned char *src, int szsrc, char *dest, int szdest);
int Base64Unescape(const char *src, int szsrc, char *dest, int szdest);
int CalculateBase64EscapedLen(int input_len, bool do_padding = true);


bool
Base64Decode(const string& src, string* dest)
{
    int len = src.size();
    char* buf = new char[len + 1];
    len = Base64Unescape(src.data(), len, buf, len);
    *dest = string(buf);
    dest->resize(len);
    delete[] buf;

    return true;
}

bool
Base64Encode(const string& src, string* dest)
{
    int len = CalculateBase64EscapedLen(src.size());
    char* buf = new char[len + 1];
    len = Base64Escape(reinterpret_cast<const unsigned char*>(src.data()),
                       src.size(),
                       buf,
                       len);
    *dest = string(buf);
    dest->resize(len);
    delete[] buf;

    return true;
}
