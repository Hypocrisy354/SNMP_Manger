//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_VALUETYPE_H
#define SNMP_VALUETYPE_H

#include "map"
#include <iostream>

using namespace std;

class ValueType {
public:
  /**
   * BOOLEAN 布尔类型
   */
  string BOOLEAN = "Boolean";
  /**
   * INTEGER 整数类型
   */
  string INTEGER = "Integer";
  /**
   * OCTET STRING 字符串类型
   */
  string OCTET_STRING = "Octet String";
  /**
   * NULL 空类型
   */
  string NUL = "Null";
  /**
   * OBJECT_IDENTIFIER 对象标识符类型
   */
  string OBJECT_IDENTIFIER = "Object Identifier";
  /**
   * SEQUENCE 序列类型
   */
  string SEQUENCE = "Sequence";
  /**
   * COUNTER 计数器类型
   */
  string COUNTER = "Counter";
  /**
   * IPADDRESS IP地址类型
   */
  string IPADDRESS = "IpAddress";
  /**
   * TIMETICKS 时间戳类型
   */
  string TIMETICKS = "TimeTicks";
  /**
   * BOOLEAN类型的True
   */
  string T = "True";
  /**
   * BOOLEAN类型的False
   */
  string F = "False";

  /**
   * 存放 数据类型 与其 序号 的映射
   */
  map<int, string> value{
      {1, BOOLEAN},
      {2, INTEGER},
      {4, OCTET_STRING},
      {5, NUL},
      {6, OBJECT_IDENTIFIER},
      {48, SEQUENCE},
      {64, IPADDRESS},
      {65, COUNTER},
      {67, TIMETICKS}
  };
  /**
   * 存放 数据类型 与其 字节 的映射
   */
  map<string, byte> typeValue{
      {BOOLEAN, 0x01},
      {INTEGER, 0x02},
      {OCTET_STRING, 0x04},
      {NUL, 0x05},
      {OBJECT_IDENTIFIER, 0x06},
      {SEQUENCE, 0x30},
      {IPADDRESS, 0x40},
      {COUNTER, 0x41},
      {TIMETICKS, 0x43},
  };

  /**
   * 获取每种类型对应的byte
   *
   * @param type 类型
   * @return 类型对应的byte
   */
  byte getTypeByte(string type) {
    return typeValue[type];
  }

  /**
   * 根据类型序号获取对应的Type
   *
   * @param num 类型序号
   * @return 序号对应的类型
   */
  string getTypeByNum(string num) {
    return value[stoi(num)];
  }
};

#endif //SNMP_VALUETYPE_H
