#ifndef WINDOWBUFFER_H
#define WINDOWBUFFER_H

#include "mstring.h"
#include <iostream>
#include <QString>
#include <QVector>

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
        if(value < 0){
            row = 0;
        }
        if(value > row){
            LF(value - row);
        }

    }

    void incCol(){
        incCol(1);
    }
    void incCol(int value){
        getCurrent()->resize(getCurrent()->letterLength() + value);
    }

    void decCol(){
        decCol(1);
    }
    void decCol(int value){
        getCurrent()->resize(getCurrent()->letterLength() - value);
    }
    /*
    int getCol(){
        return col;
    }
    void setCol(int value){
        col = value < 0 ? 0 : value;
        if(!isValid(col, row)){
            at(row)->resize(col, ' ');
        }
    }
    */

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
        if(value > getCurrent()->letterLength()){
            offset = 0;
            getCurrent()->resize(value, ' ');
        } else{
            offset = value - getCurrent()->letterLength();
        }
    }

    int getOffsetY(){
        return offsetY;
    }
    void setOffsetY(int value){
        if(value > row){
            offsetY = 0;
            incRow(value - row);
        } else{
            offsetY = value - winHeight + 1;// value - ((row + 1 - winHeight) & 0xff);
        }
    }

    void CR(){
        offset = -getCurrent()->letterLength();
    }
    void LF(){
        offset = 0;
        offsetY = 0;
        row++;
        clear(row);
        setTopRow(row >= winHeight ? (row + 1 - winHeight) & 0xff : 0);
        head = row < 255 ? 0 : row & 0xff;
    }
    void LF(int n){
        offset = 0;
        for(int i = 0; i < n; i++){
            row++;
            clear(row);
        }
        setTopRow(row >= winHeight ? (row + 1 - winHeight) & 0xff : 0);
        head = row < 255 ? 0 : row & 0xff;
    }

    int getTopRow(){
        return top;
    }
    void setTopRow(int n){
        top = n;
    }
    void incTopRow(){
        if(isValid((top + winHeight) & 0xff))
            top = ((top - 1 + winHeight) & 0xff) == head ? top : top + 1;
    }
    void incTopRow(int n){
        for(int i = 0; i < n; i++)
        incTopRow();
    }
    void decTopRow(){
        top = (top&0xff) == head ? top : top - 1;
    }
    void decTopRow(int n){
        for(int i = 0; i < n; i++)
        decTopRow();
    }

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

    bool isValid(int row){
        return array[row & 0xff].length() > 0;
    }

    bool isValid(int x, int y){
        return array[y & 0xff].length() > x;
    }

    mstring *at(int index){
        return &array[index & 0xff];
    }
    mstring *getCurrent(){
        return at(row + offsetY);
    }
    void popBack(){
        getCurrent()->erase(getCurrent()->letterLength() - 1 + getOffset());
    }

    void append(std::basic_string<uchar> input, unsigned long *index){
        if(offset < 0){
            getCurrent()->replace(getCursorX(), input, index);
            offsetFront();
        } else{
            getCurrent()->push(input, index);
        }
    }

    void setBell(){
        bell = BELL;
    }

    bool isBell(){
        return (bell = bell <= 1 ? 0 : bell - 1) > 0;
    }

    QVector<QString> print(){
        QVector<QString> ret;
        for(int i = 0; i < winHeight; i++){
            //if(!isValid(top + i)){
            //    at(top + i)->resize(0, ' ');
            //}
            ret.append(at(top + i)->q_str());
        }
        return ret;
    }

    int getCursorX(){
        return getCurrent()->letterLength() + offset;
    }

    int getCursorY(){
        return (row + offsetY)&0xff;
    }

    int getRenderCursorY(){
        return (row - top + offsetY)&0xff;
    }

    void setCursor(int x, int y){
        setOffsetY(y);
        setOffset(x);
    }

    void clear(int index){
        at(index)->resize(0);
    }
    void clearWindow(){
        LF(winHeight);
    }

private:
    mstring array[256];
    int BELL = 2;
    int row = 0, offset=0, offsetY = 0, head = 0, top = 0, winWidth = 80, winHeight = 30, bell = 0;
};

}

#endif // WINDOWBUFFER_H
