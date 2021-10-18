#ifndef KEYUTIL_H
#define KEYUTIL_H

#include <string>
#include <stdio.h>
#include <iostream>

#include <Qt>

namespace pterm{

class KeyUtil
{
public:
    static char translateKey(int QtKeyCode){

        if(QtKeyCode >= 32 && QtKeyCode <= 126){
            if(QtKeyCode >= 65 && QtKeyCode <= 90)
                return ::tolower(QtKeyCode);
            return QtKeyCode;
        }

        if(QtKeyCode == Qt::Key_Backspace){
            return 0x08;
        }
        if(QtKeyCode == Qt::Key_Return){
            return 0x0d;
        }

        //std::string str = std::to_string(QtKeyCode);
        //std::cout << "pressed:" << str << std::endl;
        return -1;
    }
};

}

#endif // KEYUTIL_H
