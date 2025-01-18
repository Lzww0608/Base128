#ifndef BASE128_H
#define BASE128_H

#include <string>
#include <vector>

class Base128 {
public:
    // 将二进制数据编码为Base128字符串
    static std::string encode(const std::vector<uint8_t>& data);
    static std::string encode(const uint8_t* data, size_t length);

    // 将Base128字符串解码为二进制数据
    static std::vector<uint8_t> decode(const std::string& encoded);
    static std::vector<uint8_t> decode(const char* encoded, size_t length);

    // 检查Base128字符串是否有效
    static bool isValid(const std::string& encoded);
    static bool isValid(const char* encoded, size_t length);

private:
    // Base128编码表
    static const char encoding_table[];
    // Base128解码表
    static const int decoding_table[];
};

#endif // BASE128_H
