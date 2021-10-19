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

    //widgetのデフォルトで処理されてしまうキーたち
    static bool isRegistered(int key){
        if(Qt::Key_Home <= key && key <= Qt::Key_PageDown)
            return true;
        return false;
    }
};

}

#endif // KEYUTIL_H
