//
// Created by Hypocrisy on 2022/5/29.
//
#ifndef SNMP_PDU_H
#define SNMP_PDU_H

#include<iostream>
#include "Type.h"
#include "VariableBindings.h"

using namespace std;
Type t;

class PDU {
private:
  string pduType;
  string requestId;
  string errorStatus;
  string errorIndex;
  VariableBindings variableBindings;
public:
  PDU() {
    this->pduType = t.pduType[2];
    this->errorStatus = t.status[0];
    this->errorIndex = "0";
  }

  PDU(const string &pduType, const string &requestId, const VariableBindings &variableBindings) : pduType(
      t.pduType[stoi(pduType)]),
                                                                                                  requestId(
                                                                                                      requestId),
                                                                                                  variableBindings(
                                                                                                      variableBindings) {
    this->errorStatus = t.status[0];
    this->errorIndex = "0";
  }

  PDU(const string &pduType, const string &requestId, const string &errorStatus, const string &errorIndex,
      const VariableBindings &variableBindings) : pduType(t.pduType[stoi(pduType)]), requestId(requestId),
                                                  errorStatus(t.status[stoi(errorStatus)]),
                                                  errorIndex(errorIndex), variableBindings(variableBindings) {}

  void toString() {
    cout << pduType << "\trequest-Id:" << requestId << "\terror-Status:" << errorStatus << "\terror-Index:"
         << errorIndex << "\tvariable-Bindings:" << endl;
    variableBindings.toString();
  }

  const string &getPduType() const {
    return pduType;
  }

  const string &getRequestId() const {
    return requestId;
  }

  const string &getErrorStatus() const {
    return errorStatus;
  }

  const string &getErrorIndex() const {
    return errorIndex;
  }

  const VariableBindings &getVariableBindings() const {
    return variableBindings;
  }

  void setPduType(const string &pduType) {
    PDU::pduType = t.pduType[stoi(pduType)];
  }

  void setRequestId(const string &requestId) {
    PDU::requestId = requestId;
  }

  void setErrorStatus(const string &errorStatus) {
    PDU::errorStatus = t.status[stoi(errorStatus)];
  }

  void setErrorIndex(const string &errorIndex) {
    PDU::errorIndex = errorIndex;
  }

  void setVariableBindings(const VariableBindings &variableBindings) {
    PDU::variableBindings = variableBindings;
  }
};

#endif //SNMP_PDU_H
