//
// Created by Hypocrisy on 2022/6/1.
//

#ifndef SNMP_ENCODE_H
#define SNMP_ENCODE_H

typedef unsigned char byte;

#include <cstring>
#include "VariableBindings.h"
#include "Utils.h"
#include <vector>
#include "numeric"
#include "sstream"


Utils utils;
using namespace std;

class Encode {
public:
  vector<byte> getLengthCode(int length) {
    vector<byte> lengthByte = utils.intToBytes(length);
    if (length >= 127) {
      vector<byte> l_byte = utils.intToBytes(lengthByte.size());
      l_byte[0] |= (0x80);
      l_byte.insert(l_byte.end(), lengthByte.begin(), lengthByte.end());
      lengthByte = l_byte;
    }
    return lengthByte;
  }

  vector<byte> getBooleanCode(VariableBindings var) {
    //  编码 TLV
    if (var.getValue() == vt.T) {
      return vector<byte>{0x01, 0x01, 0xFF};
    } else {
      return vector<byte>{0x01, 0x01, 0x00};
    }
  }

  vector<byte> getIntegerCode(VariableBindings var) {
    //  编码 V
    vector<byte> v_byte = utils.intToBytes(stoi(var.getValue()));
    //  编码TL
    vector<byte> tl_byte{vt.getTypeByte(var.getValueType()), (byte) v_byte.size()};
    //  合并TLV
    tl_byte.insert(tl_byte.end(), v_byte.begin(), v_byte.end());;
    return tl_byte;
  }

  vector<byte> getOctStringCode(VariableBindings var) {
    //  编码V
    vector<byte> v_byte;
    string p = var.getValue();
    for (int i = 0; i < p.length(); i++) {
      v_byte.push_back(p[i]);
    }
    //  编码TL
    vector<byte> tl_byte;
    tl_byte.push_back(vt.getTypeByte(var.getValueType()));
    tl_byte.push_back((byte) v_byte.size());
    //  合并TLV
    tl_byte.insert(tl_byte.end(), v_byte.begin(), v_byte.end());
    return tl_byte;
  }

  vector<byte> getNullCode() {
    //  编码TLV
    return vector<byte>{0x05, 0x00};
  }

  vector<byte> getOIDCode(VariableBindings var) {
    //  编码 V
    vector<string> str_oID;
    str_oID = utils.split(var.getObjectId(), '.');
    vector<byte> v_byte;
    v_byte.push_back((byte) (stoi(str_oID[0]) * 40 + stoi(str_oID[1])));
    int temp[str_oID.size() - 2];
    vector<byte> temp_v_byte;
    for (int i = 0; i < str_oID.size() - 2; i++) {
      temp[i] = stoi(str_oID[i + 2]);
      if (temp[i] > 127) {
        while (temp[i] != 0) {
          temp_v_byte.push_back((byte) (temp[i] % 128));
          temp[i] /= 128;
        }
        for (int j = temp_v_byte.size() - 1; j >= 0; j--) {
          if (j != temp_v_byte.size() - 1)
            temp_v_byte[j] = (byte) (temp_v_byte[j] & 0x7F);
          else
            temp_v_byte[j] = (byte) (temp_v_byte[j] | 0x80);
          v_byte.push_back(temp_v_byte[j]);
        }
      }
      if (temp_v_byte.size() == 0)
        v_byte.push_back((byte) temp[i]);
    }
    //  编码 TLV
    vector<byte> tlv_byte(utils.intToBytes(6));
    //  加入T
    for (int i = 0; i < getLengthCode(v_byte.size()).size(); i++) {
      tlv_byte.push_back(getLengthCode(v_byte.size())[i]);
    }
    //  加入V
    for (int i = 0; i < v_byte.size(); i++) {
      tlv_byte.push_back(v_byte[i]);
    }
    return tlv_byte;
  }

  vector<byte> getIpAddressCode(VariableBindings var) {
    //  编码 V
    vector<string> str_Ip;
    str_Ip = utils.split(var.getValue(), '.');
    vector<byte> v_byte(str_Ip.size());
    for (int i = 0; i < v_byte.size(); i++) {
      v_byte[i] = (byte) stoi(str_Ip[i]);
    }
    //  编码TLV
    vector<byte> tlv_byte{vt.getTypeByte(var.getValueType()), (byte) v_byte.size()};
    tlv_byte.insert(tlv_byte.end(), v_byte.begin(), v_byte.end());
    return tlv_byte;
  }

  vector<byte> getCounterCode(VariableBindings var) {
    //  编码V
    vector<byte> v_byte(utils.intToBytes(stoi(var.getValue())));
    //  编码 TLV
    vector<byte> tlv_byte{vt.getTypeByte(var.getValueType()), (byte) v_byte.size()};
    tlv_byte.insert(tlv_byte.end(), v_byte.begin(), v_byte.end());
    return tlv_byte;
  }

  vector<byte> getTimeTicksCode(VariableBindings var) {
    //  编码V
    vector<byte> v_byte(utils.intToBytes(stoi(var.getValue())));
    //  编码TLV
    vector<byte> tlv_byte{vt.getTypeByte(var.getValueType()), (byte) v_byte.size()};
    tlv_byte.insert(tlv_byte.end(), v_byte.begin(), v_byte.end());
    return tlv_byte;
  }

  vector<byte> getVariableBindingsCode(VariableBindings var) {
    //  编码VariableBindings
    //  1.编码OID
    vector<byte> chief(getOIDCode(var));
    //  2.编码Value
    if (var.getValueType() == vt.BOOLEAN) {
      vector<byte> _1(getBooleanCode(var));
      chief.insert(chief.end(), _1.begin(), _1.end());
    } else if (var.getValueType() == vt.INTEGER) {
      vector<byte> _2(getIntegerCode(var));
      chief.insert(chief.end(), _2.begin(), _2.end());
    } else if (var.getValueType() == vt.OCTET_STRING) {
      vector<byte> _4(getOctStringCode(var));
      chief.insert(chief.end(), _4.begin(), _4.end());
    } else if (var.getValueType() == vt.NUL) {
      vector<byte> _5(getNullCode());
      chief.insert(chief.end(), _5.begin(), _5.end());
    } else if (var.getValueType() == vt.OBJECT_IDENTIFIER) {
      vector<byte> _6(getOIDCode(var));
      chief.insert(chief.end(), _6.begin(), _6.end());
    } else if (var.getValueType() == vt.COUNTER) {
      vector<byte> _65(getCounterCode(var));
      chief.insert(chief.end(), _65.begin(), _65.end());
    } else if (var.getValueType() == vt.IPADDRESS) {
      vector<byte> _64(getIpAddressCode(var));
      chief.insert(chief.end(), _64.begin(), _64.end());
    } else if (var.getValueType() == vt.TIMETICKS) {
      vector<byte> _67(getTimeTicksCode(var));
      chief.insert(chief.end(), _67.begin(), _67.end());
    } else { cout << "请输入正确的编码对象编号！" << endl; }

    //  3.合并var整体的TLV
    vector<byte> tlv_byte_1{vt.getTypeByte(vt.SEQUENCE), (byte) chief.size()};
    chief.insert(chief.begin(), tlv_byte_1.begin(), tlv_byte_1.end());
    vector<byte> tlv_byte_2{vt.getTypeByte(vt.SEQUENCE), (byte) chief.size()};
    chief.insert(chief.begin(), tlv_byte_2.begin(), tlv_byte_2.end());
    return chief;
  }

  vector<byte> getPduCode(vector<byte> var, PDU pdu) {
    vector<byte> chief(var);
    // 编码PDU
    // 1、编码Error status和Error index
    //    编码"TLV
    vector<byte> tlv_byte_1{0x02, 0x01, 0x00, 0x02, 0x01, 0x00};
    chief.insert(chief.begin(), tlv_byte_1.begin(), tlv_byte_1.end());
    // 2.编码Request ID
    //   编码V
    vector<byte> v_byte{(byte) stoi(pdu.getRequestId())};
    //   编码TLV
    vector<byte> tlv_byte_2{vt.getTypeByte(vt.INTEGER), (byte) v_byte.size()};
    tlv_byte_2.insert(tlv_byte_2.end(), v_byte.begin(), v_byte.end());
    chief.insert(chief.begin(), tlv_byte_2.begin(), tlv_byte_2.end());
    //  3.编码PDU的type
    //    编码T
    vector<byte> t_byte;
    if (t.pduType[0] == pdu.getPduType()) {
      vector<byte> tem_0(utils.intToBytes(160));
      t_byte.insert(t_byte.end(), tem_0.begin(), tem_0.end());
    } else if (t.pduType[1] == pdu.getPduType()) {
      vector<byte> tem_1(utils.intToBytes(161));
      t_byte.insert(t_byte.end(), tem_1.begin(), tem_1.end());
    } else if (t.pduType[3] == pdu.getPduType()) {
      vector<byte> tem_3(utils.intToBytes(163));
      t_byte.insert(t_byte.end(), tem_3.begin(), tem_3.end());
    } else { cout << "err" << endl; };
    //    编码L
    vector<byte> l_byte{(byte) chief.size()};
    t_byte.insert(t_byte.end(), l_byte.begin(), l_byte.end());
    // 合并
    chief.insert(chief.begin(), t_byte.begin(), t_byte.end());
    return chief;
  }

  vector<byte> getSnmpMessageCode(vector<byte> pdu, SnmpMessage snmp) {
    vector<byte> chief = pdu;
    //  编码SNMPMessage
    //  1.编码Community
    //    编码V
    vector<byte> v_byte_1;
    string p = snmp.getCommunity();
    for (int i = 0; i < p.length(); i++) {
      v_byte_1.push_back(p[i]);
    }
    //    编码L
    vector<byte> l_byte_1(utils.intToBytes(v_byte_1.size()));
    l_byte_1.insert(l_byte_1.end(), v_byte_1.begin(), v_byte_1.end());
    //    编码T
    vector<byte> t_byte_1(utils.intToBytes(4));
    //    合并TLV
    t_byte_1.insert(t_byte_1.end(), l_byte_1.begin(), l_byte_1.end());
    //  合并community和pdu
    chief.insert(chief.begin(), t_byte_1.begin(), t_byte_1.end());
    //  2.编码Version
    //    编码V
    vector<byte> v_byte_2(utils.intToBytes(snmp.getVersionId()));
    //    编码L
    vector<byte> l_byte_2(utils.intToBytes(v_byte_2.size()));
    l_byte_2.insert(l_byte_2.end(), v_byte_2.begin(), v_byte_2.end());
    //    编码T
    vector<byte> t_byte_2(utils.intToBytes(2));
    t_byte_2.insert(t_byte_2.end(), l_byte_2.begin(), l_byte_2.end());
    //  合并version和community以及pdu
    chief.insert(chief.begin(), t_byte_2.begin(), t_byte_2.end());
    //  3.编码整体TLV
    //    编码T
    vector<byte> l_byte_3(utils.intToBytes(chief.size()));
    //    编码L
    vector<byte> t_byte_3(utils.intToBytes(48));
    t_byte_3.insert(t_byte_3.end(), l_byte_3.begin(), l_byte_3.end());
    //    合并TLV
    chief.insert(chief.begin(), t_byte_3.begin(), t_byte_3.end());
    return chief;
  }

  vector<byte> getSnmpCode(SnmpMessage snmp_message) {
    PDU pdu = (PDU) snmp_message.getSnmpPdu();
    VariableBindings variable_bindings = (VariableBindings) pdu.getVariableBindings();
    vector<byte> chief(getSnmpMessageCode(getPduCode(getVariableBindingsCode(variable_bindings), pdu), snmp_message));
    return chief;
  }
};

#endif //SNMP_ENCODE_H
