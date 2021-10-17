#ifndef KEYUTIL_H
#define KEYUTIL_H

#include <string>
#include <stdio.h>
#include <iostream>

namespace pterm{

class KeyUtil
{
public:
    static std::string translateKey(int QtKeyCode){
        std::string str = std::to_string(QtKeyCode);
        std::cout << "pressed:" << str << std::endl;
        return str;
    }
};

}

#endif // KEYUTIL_H
