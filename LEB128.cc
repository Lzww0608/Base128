#include "LEB128.h"

std::vector<uint8_t> LEB128::encodeUnsigned(const std::vector<uint8_t>& value) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> temp = value;
    bool more = true;
    
    while (more) {
        uint8_t byte = temp[0] & 0x7F;
        
        // 右移7位
        for (size_t i = 0; i < temp.size() - 1; i++) {
            temp[i] = (temp[i] >> 7) | (temp[i+1] << 1);
        }
        temp.back() >>= 7;
        
        // 检查是否还有更多字节
        more = false;
        for (uint8_t b : temp) {
            if (b != 0) {
                more = true;
                byte |= 0x80;
                break;
            }
        }
        
        result.push_back(byte);
    }
    
    return result;
}

std::vector<uint8_t> LEB128::encodeSigned(const std::vector<uint8_t>& value) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> temp = value;
    bool more = true;
    
    while (more) {
        uint8_t byte = temp[0] & 0x7F;
        
        // 右移7位
        for (size_t i = 0; i < temp.size() - 1; i++) {
            temp[i] = (temp[i] >> 7) | (temp[i+1] << 1);
        }
        temp.back() >>= 7;
        
        // 检查符号位
        bool isNegative = (temp.back() & 0x80) != 0;
        
        if ((isNegative && temp.back() == 0xFF && (byte & 0x40) != 0) ||
            (!isNegative && temp.back() == 0 && (byte & 0x40) == 0)) {
            more = false;
        } else {
            byte |= 0x80;
        }
        
        result.push_back(byte);
    }
    
    return result;
}

std::vector<uint8_t> LEB128::decodeUnsigned(const uint8_t* data, size_t* bytesRead) {
    std::vector<uint8_t> result(8, 0); // 假设最大8字节
    size_t count = 0;
    size_t shift = 0;
    
    while (true) {
        uint8_t byte = data[count++];
        
        // 将当前字节的低7位移位到结果中
        for (size_t i = 0; i < result.size(); i++) {
            if (shift >= 8*i && shift < 8*(i+1)) {
                result[i] |= (byte & 0x7F) << (shift - 8*i);
            }
            if (shift + 7 >= 8*i && shift + 7 < 8*(i+1)) {
                result[i] |= (byte & 0x7F) >> (8*(i+1) - (shift + 7));
            }
        }
        
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
    }
    
    if (bytesRead) {
        *bytesRead = count;
    }
    
    // 移除前导零
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    
    return result;
}

std::vector<uint8_t> LEB128::decodeSigned(const uint8_t* data, size_t* bytesRead) {
    std::vector<uint8_t> result(8, 0); // 假设最大8字节
    size_t count = 0;
    size_t shift = 0;
    uint8_t byte;
    
    do {
        byte = data[count++];
        
        // 将当前字节的低7位移位到结果中
        for (size_t i = 0; i < result.size(); i++) {
            if (shift >= 8*i && shift < 8*(i+1)) {
                result[i] |= (byte & 0x7F) << (shift - 8*i);
            }
            if (shift + 7 >= 8*i && shift + 7 < 8*(i+1)) {
                result[i] |= (byte & 0x7F) >> (8*(i+1) - (shift + 7));
            }
        }
        
        shift += 7;
    } while (byte & 0x80);
    
    // 符号扩展
    if (byte & 0x40) {
        for (size_t i = (shift-1)/8 + 1; i < result.size(); i++) {
            result[i] = 0xFF;
        }
    }
    
    if (bytesRead) {
        *bytesRead = count;
    }
    
    // 移除不必要的字节
    while (result.size() > 1 && 
           ((result.back() == 0 && (result[result.size()-2] & 0x80) == 0) ||
            (result.back() == 0xFF && (result[result.size()-2] & 0x80) != 0))) {
        result.pop_back();
    }
    
    return result;
}

size_t LEB128::getSizeUnsigned(const std::vector<uint8_t>& value) {
    size_t size = 0;
    std::vector<uint8_t> temp = value;
    
    do {
        // 右移7位
        for (size_t i = 0; i < temp.size() - 1; i++) {
            temp[i] = (temp[i] >> 7) | (temp[i+1] << 1);
        }
        temp.back() >>= 7;
        
        size++;
    } while (!std::all_of(temp.begin(), temp.end(), [](uint8_t b) { return b == 0; }));
    
    return size;
}

size_t LEB128::getSizeSigned(const std::vector<uint8_t>& value) {
    size_t size = 0;
    std::vector<uint8_t> temp = value;
    bool more = true;
    
    while (more) {
        // 右移7位
        for (size_t i = 0; i < temp.size() - 1; i++) {
            temp[i] = (temp[i] >> 7) | (temp[i+1] << 1);
        }
        temp.back() >>= 7;
        
        size++;
        
        bool isNegative = (temp.back() & 0x80) != 0;
        bool allZero = std::all_of(temp.begin(), temp.end(), [](uint8_t b) { return b == 0; });
        bool allOnes = std::all_of(temp.begin(), temp.end(), [](uint8_t b) { return b == 0xFF; });
        
        if ((isNegative && allOnes) || (!isNegative && allZero)) {
            more = false;
        }
    }
    
    return size;
}
