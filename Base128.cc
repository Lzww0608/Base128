#include "Base128.h"

// Base128编码表 - 使用可打印的ASCII字符
const char Base128::encoding_table[] = {
    '0','1','2','3','4','5','6','7','8','9',
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    '!','@','#','$','%','^','&','*','(',')','[',']','{',
    '}','|',';',':',',','.','<','>','/','?','`','~','+',
    '=','_','-',' ','"','\''
};

// Base128解码表
const int Base128::decoding_table[128] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 0-15
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 16-31
    79,-1,80,73,74,75,76,81,77,78,76,86,72,87,73,84, // 32-47
    0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,           // 48-63
    -1,10,11,12,13,14,15,16,17,18,19,20,21,22,       // 64-77
    23,24,25,26,27,28,29,30,31,32,33,34,35,          // 78-90
    -1,-1,-1,-1,88,-1,                               // 91-96
    36,37,38,39,40,41,42,43,44,45,46,47,48,          // 97-109
    49,50,51,52,53,54,55,56,57,58,59,60,61,          // 110-122
    -1,-1,-1,85,-1                                   // 123-127
};

std::string Base128::encode(const std::vector<uint8_t>& data) {
    return encode(data.data(), data.size());
}

std::string Base128::encode(const uint8_t* data, size_t length) {
    std::string encoded;
    encoded.reserve((length * 8 + 6) / 7); // 预分配空间

    uint32_t buffer = 0;
    int bits = 0;

    for(size_t i = 0; i < length; i++) {
        buffer = (buffer << 8) | data[i];
        bits += 8;
        
        while(bits >= 7) {
            bits -= 7;
            encoded += encoding_table[(buffer >> bits) & 0x7F];
        }
    }

    // 处理剩余的位
    if(bits > 0) {
        encoded += encoding_table[(buffer << (7 - bits)) & 0x7F];
    }

    return encoded;
}

std::vector<uint8_t> Base128::decode(const std::string& encoded) {
    return decode(encoded.c_str(), encoded.length());
}

std::vector<uint8_t> Base128::decode(const char* encoded, size_t length) {
    std::vector<uint8_t> decoded;
    decoded.reserve(length * 7 / 8);

    uint32_t buffer = 0;
    int bits = 0;

    for(size_t i = 0; i < length; i++) {
        int value = decoding_table[encoded[i] & 0x7F];
        if(value == -1) continue; // 跳过无效字符
        
        buffer = (buffer << 7) | value;
        bits += 7;
        
        while(bits >= 8) {
            bits -= 8;
            decoded.push_back((buffer >> bits) & 0xFF);
        }
    }

    return decoded;
}

bool Base128::isValid(const std::string& encoded) {
    return isValid(encoded.c_str(), encoded.length());
}

bool Base128::isValid(const char* encoded, size_t length) {
    for(size_t i = 0; i < length; i++) {
        if(encoded[i] > 127 || decoding_table[encoded[i]] == -1) {
            return false;
        }
    }
    return true;
}
