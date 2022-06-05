//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_ADMINIST_H
#define SNMP_ADMINIST_H

#include<winsock2.h>
#include<iostream>
#include "SnmpMessage.h"
#include "PDU.h"
#include "VariableBindings.h"
#include "Encode.h"
#include "Decode.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")

class Administ {
private:
  SnmpMessage snmp;
  sockaddr_in dstAddr;
  Encode encode;
  Decode decode;
public:
  Administ() {}
  Administ(SnmpMessage snmp, string ip) {
    try {
      this->dstAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
      this->snmp = snmp;
    } catch (exception e) {
      cerr << "ip or snmp error" << endl;
    }
  }
  int run() {
    vector<byte> temp (encode.getSnmpCode(snmp));
    const byte *sendSnmp = &temp.at(0);
    byte recvSnmp[1472]; // UDP数据报的数据区最大长度为1472字节
    WSADATA WSAData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &WSAData) != 0)
      return 0;

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == clientSocket) {
      cout << "socket error!";
      return 0;
    }
    int len = sizeof(dstAddr);
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_port = htons(161);
    dstAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    cout << "Send SNMP Message:" << endl;
    snmp.toString();
    utils.showPacket(encode.getSnmpCode(snmp));

    if (sendto(clientSocket,
               (char *) sendSnmp,
               encode.getSnmpCode(snmp).size(),
               0,
               (sockaddr *) &dstAddr,
               len) != -1) {
      cout << "send success" << endl;
    }
    try {
      int recvData_len = recvfrom(clientSocket, (char *) recvSnmp, sizeof(recvSnmp), 0, (sockaddr *) &dstAddr, &len);
      if (recvData_len > 0) {
        cout << "recv success" << endl;
        vector<byte> recvData(recvSnmp,recvSnmp + recvData_len);
        cout << "Receive SNMP Response:" << endl;
        SnmpMessage res = decode.getSnmpMessage(recvData);
        res.toString();
        utils.showPacket(recvData);
      }
    } catch (exception e) {
      cout << endl << "等待代理响应超时！" << endl;
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
  }
};
#endif //SNMP_ADMINIST_H
