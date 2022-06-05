//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_TYPE_H
#define SNMP_TYPE_H

#include <string>
#include "map"

using namespace std;

class Type {

public:
  /**
   * PDU数据包的类型
   *
   * get-request 类型
   * get-next-request 类型
   * set-request 类型
   * get-response 类型
   * 4种状态
   */
  map<int, string> pduType{
      {0, "get-request"},
      {1, "get-next-request"},
      {2, "get-response"},
      {3, "set-request"}
  };
  /**
   * SNMP的Version的版本
   *
   * "version-1 (0)", "version-2 (1)", "version-3 (2)"
   * 3个版本
   */
  map<int, string> version{
      {0, "version-1"},
      {1, "version-2"},
      {2, "version-3"}
  };

  /**
   * Response中的ErrorStatus的类型
   *
   * "noError (0)"    ：正常
   * "tooBig (1)"     ：应答太大无法装入一个 SNMP 报文
   * "noSuchName (2)" ：操作指明了一个不存在的变量
   * "badValue (3)"   ：Set 操作结果是一个无效值或无效语法
   * "readOnly (4)"   ：管理站试图修改一个只读变量
   * "genError (5)"   ：其他差错
   * 6种错误状态
   */
  map<int, string> status{
      {0, "noError"},
      {1, "tooBig"},
      {2, "noSuchName"},
      {3, "badValue"},
      {4, "readOnly"},
      {5, "genError"}
  };
};

#endif //SNMP_TYPE_H
