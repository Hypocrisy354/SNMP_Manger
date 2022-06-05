#include<winsock2.h>
#include<iostream>
#include "SnmpMessage.h"
#include "PDU.h"
#include "VariableBindings.h"
#include "Administ.h"
#include "Utils.h"

using namespace std;

bool view() {
  string ip, community, type, num, oid, valuetype, value, flag;
  VariableBindings var;
  PDU pdu;
  SnmpMessage snmp;
  Utils utils;

  cout << "请填写SNMP代理信息" << endl;
  cout << "1.IP地址" << endl;
  cin >> ip;
  cout << "2.团体名称(community)" << endl;
  cin >> community;
  cout << "----SNMP报文配置菜单----" << endl;
  cout << "Version：    0：version-1" << endl;
  cout << "Pdu Type：   0：get-request    1：get-next-request  3：set-request" << endl;
  cout
      << "Value Type： 1：BOOLEAN             2：INTEGER       4：OCTET STRING    5：NULL\n6：OBJECT_IDENTIFIER  64：IPADDRESS    65：COUNTER        67：TIMETICKS"
      << endl;
  cout << "请按照提示填写，选择SNMP Message相关配置：" << endl;
  cout << "版本号(Version): 0" << endl;
  snmp.setVersion("0");
  snmp.setCommunity(community);
  cout << "请求类型(Pdu Type):" << endl;
  cin >> type;
  pdu.setPduType(type);
  num = utils.getNum();  //1-100的随机数
  cout << "请求标识(Request Id):" << num << endl;
  pdu.setRequestId(num);
  cout << "OID:";
  cin >> oid;
  var.setObjectId(oid);
  if (type == "3") {
    cout << "Value Type:";
    cin >> valuetype;
    var.setValueType(valuetype);
    if (valuetype == "1") {
      cout << "Value:(T/F)";
      cin >> value;
      var.setValue("T" == value ? "True" : "False");
    } else if (valuetype == "5") {
      cout << "Value:   NULL" << endl;
    } else {
      cout << "Value:";
      cin >> value;
      var.setValue(value);
    }
  } else {
    cout << "Value Type:5" << endl;
    cout << "Value:   NULL" << endl;
  }
  pdu.setVariableBindings(var);
  snmp.setSnmpPdu(pdu);
  Administ ad(snmp, ip);
  ad.run();

  cout << "是否继续：(T/F)" << endl;
  cin >> flag;
  if (flag == "T" || flag == "t") {
    cout << "\n";
    return true;
  } else if (flag == "F" || flag == "f") {
    cout << "\n";
    return false;
  } else {
    cout << "输入错误！" << endl;
    return false;
  }
}

int main() {
  bool state = true;
  while (state) {
    state = view();
  }
  return 0;
}

