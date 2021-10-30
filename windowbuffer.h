#ifndef WINDOWBUFFER_H
#define WINDOWBUFFER_H

#include "mstring.h"
#include <iostream>

namespace pterm {

class WindowBuffer{
public:
    WindowBuffer(){

    }
    ~WindowBuffer(){
    }

    void incRow(){
        incRow(1);
    }
    void incRow(int value){
        row += value;
    }
    void decRow(){
        decRow(1);
    }
    void decRow(int value){
        row = row - value < 0 ? 0 : row - value;
    }
    int getRow(){
        return row;
    }
    void setRow(int value){
        row = value < 0 ? 0 : value;
    }

    void incCol(){
        incCol(1);
    }
    void incCol(int value){
        col += value;
    }
    void decCol(){
        decCol(1);
    }
    void decCol(int value){
        col = col - value < 0 ? 0 : col - value;
    }
    int getCol(){
        return col;
    }
    void setCol(int value){
        col = value < 0 ? 0 : value;
    }

    void offsetBack(){
        offsetBack(1);
    }
    void offsetBack(int value){
        offset -= value;
    }
    void offsetFront(){
        offsetFront(1);
    }
    void offsetFront(int value){
        offset += value;
    }
    int getOffset(){
        return offset;
    }
    void setOffset(int value){
        offset = value;
    }

    void CR(){
        offset = -col;
    }
    void LF(){
        col = offset = 0;
        row++;
    }

    int getCursorX(){
        return col + offset;
    }

    int getCursorY(){
        return row;
    }

    int getTopRow(){
        return top;
    }
    void setTopRow(int value);

    int getWinWidth(){
        return winWidth;
    }
    int getWinHeight(){
        return winHeight;
    }
    void setWinSize(int width, int height){
        winWidth = width;
        winHeight = height;
    }

    mstring *at(int index){
        return &array[index & 0xff];
    }
    mstring *getCurrent(){
        return &array[row & 0xff];
    }

    mstring *print();



private:
    mstring array[256];
    int col = 0, row = 0, offset=0, top = 0, winWidth = 80, winHeight = 30;
};

}

#endif // WINDOWBUFFER_H
