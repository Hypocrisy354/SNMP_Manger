//
// Created by Hypocrisy on 2022/5/29.
//

#ifndef SNMP_VARIABLEBINDINGS_H
#define SNMP_VARIABLEBINDINGS_H

#include <string>
#include <iostream>
#include "ValueType.h"

using namespace std;
ValueType vt;

class VariableBindings {
private:
  string objectId;
  string valueType;
  string value;

public:
  VariableBindings() {
    this->valueType = vt.NUL;
    this->value = "";
  }

  VariableBindings(const string &objectId, const string &valueType, const string &value)
      : objectId(objectId), valueType(
      vt.getTypeByNum(valueType)),
        value(value) {
  }

  void toString() {
    cout << "objectId:" << this->objectId << "\tvalueType:" << this->valueType << "\tvalue:" << this->value << endl;
  }

  const string &getObjectId() const {
    return objectId;
  }

  const string &getValueType() const {
    return valueType;
  }

  const string &getValue() const {
    return value;
  }

  void setObjectId(const string &objectId) {
    VariableBindings::objectId = objectId;
  }

  void setValueType(const string &valueType) {
    VariableBindings::valueType = vt.getTypeByNum(valueType);
  }

  void setValue(const string &value) {
    VariableBindings::value = value;
  }
};

#endif //SNMP_VARIABLEBINDINGS_H
