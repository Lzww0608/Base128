#ifndef LEB128_H
#define LEB128_H

#include <cstdint>
#include <vector>
#include <string>

class LEB128 {
public:
    // 编码无符号大整数为LEB128格式
    static std::vector<uint8_t> encodeUnsigned(const std::vector<uint8_t>& value);
    
    // 编码有符号大整数为LEB128格式 
    static std::vector<uint8_t> encodeSigned(const std::vector<uint8_t>& value);

    // 从LEB128格式解码为无符号大整数
    static std::vector<uint8_t> decodeUnsigned(const uint8_t* data, size_t* bytesRead);
    
    // 从LEB128格式解码为有符号大整数
    static std::vector<uint8_t> decodeSigned(const uint8_t* data, size_t* bytesRead);

    // 计算编码后的字节数
    static size_t getSizeUnsigned(const std::vector<uint8_t>& value);
    static size_t getSizeSigned(const std::vector<uint8_t>& value);
};

#endif // LEB128_H
