#include "pseudoterm.h"

#include "fcntl.h"
#include "pty.h"
#include "utmp.h"
#include "stdlib.h"
#include <sys/stat.h>
#include <math.h>

#include "iostream"

namespace pterm{

PseudoTerm::PseudoTerm()
{
    initPipe();
    forkPty();
}

PseudoTerm::~PseudoTerm()
{
    delete(childp);
}

void PseudoTerm::keyPressed(int keycode){
    _Char buf;
    buf.keycode = keycode;
    for(int i = 4; i >= 2; i--){
        if(buf.keycode >= std::pow(256, i - 1)){
            ::write(amaster, buf.char_4, i);
            return;
        }
    }
    ::write(amaster, buf.char_4, 1);
}

void PseudoTerm::initPipe(){
    if(::mkfifo(FIFONAME.c_str(),0666)==-1){
        perror("mkfifo");
    }
    if((pip=open(FIFONAME.c_str(), O_RDWR  | O_NONBLOCK))==-1){
        perror(("open" + FIFONAME).c_str());
        exit(1);
    }
}

int PseudoTerm::getPipe(){
    return pip;
}

bool PseudoTerm::isPipeExists(){
    struct stat st;
    return stat(FIFONAME.c_str(), &st) == 0;
}

int PseudoTerm::forkPty(){

    if(::openpty(&amaster, &aslave, tname, nullptr, nullptr) < 0){
        perror("openpty");
        exit(EXIT_FAILURE);
    }

    switch(::fork()){
        case -1:
            ::exit(1);

        case 0:
            close(amaster);
            if(login_tty(aslave) < 0) {
                ::exit(1);
            }

            ::execlp("bash", "", nullptr);
            ::exit(0);
    }

    ::close(aslave);

    switch((childp = childptr::getInst(childp, ::fork()))->getPid()){
        case -1:
            ::perror("forkMaster");
            exit(1);
        case 0:
            int nread;
            char buf[128];
            for(;;){
                if ((nread = ::read(amaster, buf, 127)) <= 0){
                    break;
                }
                buf[nread] = 0;
                ::write(getPipe(), &nread, 1);
                ::write(getPipe(), buf, nread + 1);
            }
            ::remove(FIFONAME.c_str());
            ::exit(0);
    }

    return 0;
}

}
