//
// Created by Hypocrisy on 2022/6/4.
//

#ifndef SNMP__DECODE_H_
#define SNMP__DECODE_H_

class Decode {
public:
  SnmpMessage getSnmpMessage(vector<byte> snmpData) {
    SnmpMessage snmp_message;
    PDU pdu;
    VariableBindings var;

    //  解码SNMP的version和community
    vector<byte> pduData(getSnmp(snmpData, snmp_message));
    vector<byte> varData;
    vector<byte> temp(getPdu(pduData, pdu));
    if ((pduData[0] & 0xff) == 162) {
      //  解码PDU的Request ID、Error status、Error index
      varData.assign(temp.begin(), temp.end());
      //  解码Variable bindings的Name、Value
      getVar(varData, var);
      //  合并
      pdu.setVariableBindings(var);
      snmp_message.setSnmpPdu(pdu);
    } else if ((pduData[0] & 0xff) == 164) {
      cout << "响应为Trap" << endl;
    }
    return snmp_message;
  }

  vector<byte> getSnmp(vector<byte> snmpData, SnmpMessage &snmp_message) {
    //  一、第一步解码SNMP的Version、Community
    //  1、获取Version
    int version = snmpData[4] & 0xFF;
    snmp_message.setVersion(to_string(version));
    //  2.获取Community
    int communityLength = snmpData[6] & 0xFF;
    vector<byte> communityData(communityLength);
    communityData.assign(snmpData.begin() + 7, snmpData.begin() + 7 + communityLength);
    string p;
    for (int i = 0; i < communityData.size(); i++) {
      p.push_back(communityData[i]);
    }
    snmp_message.setCommunity(p);
    //  获取数据包整体长度
    int length = (snmpData[1] & 0xFF) + 2;
    vector<byte> pduData(length - 7 - communityLength);
    //  截取PDU部分数据,继续解码
    pduData.assign(
        snmpData.begin() + 7 + communityLength,
        snmpData.begin() + 7 + communityLength + pduData.size());
    return pduData;
  }

  vector<byte> getPdu(vector<byte> pduData, PDU &pdu) {
    //  二、第二步解码PDU的Request ID、Error status、Error index
    //  1、获取Request ID
    int requestId = pduData[4] & 0xFF;
    pdu.setRequestId(to_string(requestId));
    //  2、获取Error status
    int errorStatus = pduData[7] & 0xFF;
    pdu.setErrorStatus(to_string(errorStatus));
    //  3、获取Error index
    int errorIndex = pduData[10] & 0xFF;
    pdu.setErrorIndex(to_string(errorIndex));
    //  截取Variable bindings部分数据，继续解码
    vector<byte> varData(pduData.size() - 11);
    varData.assign(pduData.begin() + 11, pduData.begin() + 11 + varData.size());
    return varData;
  }

  void getVar(vector<byte> varData, VariableBindings &var) {
    //  三、第三步解码Variable bindings的Name、Value
    vector<byte> varDatatemp(varData.size());
    varDatatemp.assign(varData.begin() + 4, varData.end());
    //  1、获取Name
    int nameLength = varDatatemp[1] & 0xFF;
    vector<byte> nameData(nameLength);
    nameData.assign(varDatatemp.begin() + 2, varDatatemp.begin() + 2 + nameData.size());
    var.setObjectId(getOID(nameData));
    //  2、获取Value
    int valuelength = varDatatemp.size() - (2 + nameLength);
    vector<byte> valueData(valuelength);
    valueData.assign(
        varDatatemp.begin() + (2 + nameLength),
        varDatatemp.begin() + (2 + nameLength) + valuelength);
    int typeNum = valueData[0] & 0xFF;
    string type = vt.getTypeByNum(to_string(typeNum));
    int datalength = valueData[1] & 0xFF;
    vector<byte> data(datalength);
    data.assign(valueData.begin() + 2, valueData.begin() + 2 + datalength);
    if (type == vt.BOOLEAN) {
      var.setValueType(to_string(typeNum));
      var.setValue(getBoolean(data));
    } else if (type == vt.INTEGER) {
      var.setValueType(to_string(typeNum));
      var.setValue(getInteger(data));
    } else if (type == vt.OCTET_STRING) {
      var.setValueType(to_string(typeNum));
      var.setValue(getString(data));
    } else if (type == vt.NUL) {
      var.setValueType(to_string(typeNum));
      var.setValue(getNull(data));
    } else if (type == vt.OBJECT_IDENTIFIER) {
      var.setValueType(to_string(typeNum));
      var.setValue(getOID(data));
    } else if (type == vt.SEQUENCE) {
      var.setValueType(to_string(typeNum));
      var.setValue(getSequence(data));
    } else if (type == vt.IPADDRESS) {
      var.setValueType(to_string(typeNum));
      var.setValue(getIpAddress(data));
    } else if (type == vt.COUNTER) {
      var.setValueType(to_string(typeNum));
      var.setValue(getCounter(data));
    } else if (type == vt.TIMETICKS) {
      var.setValueType(to_string(typeNum));
      var.setValue(getTimeTicks(data));
    } else { cout << "read value error!" << endl; }
  }

  string getBoolean(vector<byte> data) {
    return utils.bytesToInt(data) == 0 ? vt.F : vt.T;
  }

  string getInteger(vector<byte> data) {
    return to_string(utils.bytesToInt(data));
  }

  string getString(vector<byte> data) {
    string str;
    for (int i = 0; i < data.size(); i++) {
      str.push_back(data[i]);
    }
    return str;
  }

  string getNull(vector<byte> data) { return "null"; }

  string getOID(vector<byte> data) {
    byte y = (byte) (data[0] % 40);
    byte x = (byte) (data[0] / 40);
    string oID = to_string(x) + "." + to_string(y);
    for (int i = 1; i < data.size(); i++)
      oID += "." + to_string(data[i]);
    return oID;
  }

  string getSequence(vector<byte> data) { return "null"; }

  string getIpAddress(vector<byte> data) {
    string str;
    vector<int> temp(utils.byteToDec(data));
    for (int i = 0; i < temp.size(); i++) {
      str.push_back(temp[i]);
      if (i < 3) {
        str.push_back('.');
      }
    }
    return str;
  }

  string getCounter(vector<byte> data) { return to_string(utils.bytesToInt(data)); }

  string getTimeTicks(vector<byte> data) { return to_string(utils.bytesToInt(data)); }
};

#endif //SNMP__DECODE_H_
