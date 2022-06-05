cmake_minimum_required(VERSION 3.22)
project(SNMP)

set(CMAKE_CXX_STANDARD 14)

include_directories("D:\\MSYS2\\mingw64\\include")

link_directories("D:\\MSYS2\\mingw64\\bin")

link_libraries(ws2_32)

add_executable(SNMP main.cpp SnmpMessage.h PDU.h Type.h ValueType.h Administ.h Utils.h Encode.h Decode.h VariableBindings.h)

target_link_libraries(SNMP ws2_32)
