//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_UTILS_H
#define SNMP_UTILS_H

#include "string"
#include "math.h"
//添加随机数种子，作用：利用当前系统时间生成随机数，以此来防止每一次的随机数相同
#include <ctime> //添加头文件
#include "vector"

using namespace std;

class Utils {
public:
  vector<string> C{
      "0", "1", "2", "3", "4", "5", "6", "7",
      "8", "9", "a", "b", "c", "d", "e", "f"
  };

  //  随机数（1-100）
  string getNum() {
    srand((unsigned int) time(NULL));
    int randomNum = rand() % 100 + 1;
    return to_string(randomNum);
  }

  //  字符串分割方法
  vector<string> split(const string &s, char delim) {
    vector<string> tokens;
    tokens.clear();
    auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
      for (size_t i = pos; i < s.size(); i++) {
        if (s[i] != delim) return i;
      }
      return string::npos;
    };
    size_t lastPos = string_find_first_not(0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != string::npos) {
      tokens.emplace_back(s.substr(lastPos, pos - lastPos));
      lastPos = string_find_first_not(pos);
      pos = s.find(delim, lastPos);
    }
    return tokens;
  }

  /**
   * 将int型整数转换为字节数组
   *
   * @param num int整数
   * @return 字节指针
   */
  vector<byte> intToBytes(int num) {
    int n = 0;
    vector<byte> result;
    if (num <= 255 && num >= 0) {
      return vector<byte>{(byte) (num)};
    } else if (num <= 65535 && num > 256) {
      vector<byte> result(2);
      for (int i = 1; i >= 0; i--) {
        result[i] = (byte) (num >> n);
        n = n + 8;
      }
      return result;
    } else if (num <= 16777215 && num > 65536) {
      vector<byte> result(3);
      for (int i = 2; i >= 0; i--) {
        result[i] = (byte) (num >> n);
        n = n + 8;
      }
      return result;
    } else if (num > 16777216) {
      vector<byte> result(4);
      for (int i = 3; i >= 0; i--) {
        result[i] = (byte) (num >> n);
        n = n + 8;
      }
      return result;
    }
  }

  /**
   * 将字节数转换为整型
   *
   * @param data 字节数据
   * @return 转换后的整型
   */
  int bytesToInt(vector<byte> data) {
    vector<byte> bytes{0, 0, 0, 0};
    bytes.erase(bytes.begin() + (4 - data.size()));
    bytes.insert(bytes.begin() + (4 - data.size()), data.begin(), data.end());
    return bytes[0] << 24 | (bytes[1] & 0xFF) << 16 | (bytes[2] & 0xFF) << 8 | (bytes[3] & 0xFF);
  }

  /**
   * 将字节数组转换为十六进制字符串
   *
   * @param bytes 字节数组
   * @return 转换后的十六进制字符串
   */
  vector<string> byteToHex(vector<byte> bytes) {
    vector<string> hex(bytes.size());
    for (int i = 0; i < bytes.size(); i++) {
      int h = (bytes[i] >> 4) & 0xf;
      int l = bytes[i] & 0xf;
      hex[i] = C[h] + C[l];
    }
    return hex;
  }

  /**
   * 将字节数组转换为十进制数组
   *
   * @param bytes 字节数组
   * @return 返回转换后的十进制数组
   */
  vector<int> byteToDec(vector<byte> bytes) {
    vector<int> dec(bytes.size());
    int i = 0;
    for (byte b : bytes) {
      dec[i++] = b & 0xff;
    }
    return dec;
  }

  /**
   * 分别以十进制、十六进制、二进制显示数据包
   *
   * @param data 数据包编码
   */
  void showPacket(vector<byte> data) {
    cout << "BER-TLV(DEC) = ";
    for (int i = 0; i < byteToDec(data).size(); i++) {
      cout << byteToDec(data)[i] << " ";
    }
    cout << endl;
    cout << "BER-TLV(HEX) = ";
    for (int i = 0; i < byteToHex(data).size(); i++) {
      cout << byteToHex(data)[i] << " ";
    }
    cout << endl;
  }
};

#endif //SNMP_UTILS_H
