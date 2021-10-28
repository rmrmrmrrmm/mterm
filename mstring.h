#include <QString>
#include <string>

#ifndef MSTRING_H
#define MSTRING_H


namespace pterm{

class mstring
{
public:
    mstring(std::basic_string<uchar> str){
        this->str = str;
    }
    mstring(){
    }

    QString q_str(){
        return QString::fromStdString((char*)str.c_str());
    }

    int length(){
        return length(str);
    }

    static int length(std::basic_string<uchar> str){
        int l = 0;
        for(unsigned long i = 0; i < str.length(); i++){
            unsigned char c = str.at(i);
            if (c >= 0x80 && c <= 0xBF) {
            }else{
                l ++;
            }
        }
        return l;
    }

    static bool isMultiByte(uchar c){
        if (c >= 0x80 && c <= 0xBF) {
            return true;
        }
        return false;
    }

    int bytes(){
        return str.length();
    }

    void erase(int pos){
        int l = 0;
        for(unsigned long i = 0; i < str.length(); i++){
            unsigned char c = str.at(i);
            if (c >= 0x80 && c <= 0xBF) {
            }else{
                l ++;
                if(l == pos){
                    int m = 1;
                    for(unsigned long j = i + 1; j < str.length(); j++){
                        c = this->str.at(i);
                        if (c >= 0x80 && c <= 0xBF) {
                            m++;
                        }
                    }
                    str.erase(i, m);
                }

            }
        }
    }

    void insert(int pos, const std::basic_string<uchar> &str){
        int l = 0;
        for(unsigned long i = 0; i < this->str.length(); i++){
            unsigned char c = this->str.at(i);
            if (c >= 0x80 && c <= 0xBF) {
            }else{
                l ++;
                if(l == pos){
                    for(; i < this->str.length(); i++){
                        c = this->str.at(i);
                        if (!(c >= 0x80 && c <= 0xBF)) {
                            break;
                        }
                    }
                    this->str.insert(i, str);
                }

            }
        }
    }

    void insert(int pos, uchar c){
        int l = 0;
        for(unsigned long i = 0; i < this->str.length(); i++){
            unsigned char uc = this->str.at(i);
            if (uc >= 0x80 && uc <= 0xBF) {
            }else{
                l ++;
                if(l == pos){
                    for(; i < this->str.length(); i++){
                        uc = this->str.at(i);
                        if (!(uc >= 0x80 && uc <= 0xBF)) {
                            break;
                        }
                    }
                    this->str.insert(i, 1, c);
                }

            }
        }
    }

    void replace(int pos, int length, const std::basic_string<uchar> &str){
        for(int i = 0; i < length; i++)
            this->str.erase(pos + i);
        this->str.insert(pos, str);
    }

    void push(uchar c){
        str += c;
    }

    void push(std::basic_string<uchar> s){
        str += s;
    }

private:
    std::basic_string<uchar> str;
};

}

#endif // MSTRING_H
