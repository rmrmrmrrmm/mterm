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
    static int translateKey(int QtKeyCode){

        if(QtKeyCode >= 0 && QtKeyCode <= 26){
            return QtKeyCode;
        }

        if(QtKeyCode >= 32 && QtKeyCode <= 126){
            return QtKeyCode;
        }

        if(QtKeyCode == Qt::Key_Backspace){
            return 0x08;
        }
        if(QtKeyCode == Qt::Key_Return){
            return 0x0d;
        }
        if(QtKeyCode == Qt::Key_Escape){
            return 0x1B;
        }
        if(QtKeyCode == Qt::Key_Return){
            return 0x0d;
        }

        //std::string str = std::to_string(QtKeyCode);
        //std::cout << "pressed:" << str << std::endl;
        perror("error at translating QtKeyCode");
        return 0;
    }
};

}

#endif // KEYUTIL_H
