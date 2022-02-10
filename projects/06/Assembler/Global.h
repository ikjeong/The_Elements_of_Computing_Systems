/**
    Global Constants and Header
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <map>

enum class CommandType { nothing = 0, address = 1, compute = 2, label = 3 };
enum class Error { fileOpen = 100, functionCall = 200};

#endif