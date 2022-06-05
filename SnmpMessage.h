//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_SNMPMESSAGE_H
#define SNMP_SNMPMESSAGE_H

#include<iostream>
#include "PDU.h"
#include "Type.h"

using namespace std;

class SnmpMessage {
private:
  string version;
  string community;
  PDU snmpPdu;
public:
  SnmpMessage() {}

  SnmpMessage(const string &version, const string &community, const PDU &snmpPdu) : version(t.version[stoi(version)]),
                                                                                    community(community),
                                                                                    snmpPdu(snmpPdu) {}

  void toString() {
    cout << "VERSION:" << version << "\tCOMMUNITY:" << community << "\tPDU:" << endl;
    snmpPdu.toString();
  }

  const string &getVersion() const {
    return version;
  }

  int getVersionId() {
    for (int i = 0; i < t.version.size(); i++) {
      if (t.version[i] == this->version) {
        return i;
      }
    }
    return 0;
  }

  const string &getCommunity() const {
    return community;
  }

  const PDU &getSnmpPdu() const {
    return snmpPdu;
  }

  void setVersion(const string &version) {
    SnmpMessage::version = t.version[stoi(version)];
  }

  void setCommunity(const string &community) {
    SnmpMessage::community = community;
  }

  void setSnmpPdu(const PDU &snmpPdu) {
    SnmpMessage::snmpPdu = snmpPdu;
  }
};

#endif //SNMP_SNMPMESSAGE_H
