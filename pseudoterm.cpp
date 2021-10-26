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
    ::remove(FIFONAME.c_str());
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

char* PseudoTerm::getTname(){
    return tname;
}

void PseudoTerm::initTermios(){
    //手元のターミナルと同一の設定 取り急ぎ
    //term.c_cflag |= ;
    term.c_iflag |= BRKINT | IGNPAR | ICRNL | IXON | IMAXBEL | IUTF8;
    term.c_oflag |= OPOST | ONLCR;
    term.c_lflag |= ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE;
    term.c_cc[VINTR] = 0x03;
    term.c_cc[VQUIT] = 0x1c;
    term.c_cc[VERASE] = 0x7f;
    term.c_cc[VKILL] = 0x15;
    term.c_cc[VEOF] = 0x04;
    //term.c_cc[VEOL] = undef;
    //term.c_cc[VEOL2] = undef;
    //term.c_cc[VSWITCH] = undef;
    term.c_cc[VSTART] = 0x11;
    term.c_cc[VSTOP] = 0x13;
    term.c_cc[VSUSP] = 0x1A;
    term.c_cc[VREPRINT] = 0x12;
    term.c_cc[VWERASE] = 0x17;
    term.c_cc[VLNEXT] = 0x16;
    term.c_cc[VDISCARD] = 0x0f;
}

void PseudoTerm::setWinSize(winsize ws){
    tws.ws_col = ws.ws_col;
    tws.ws_row = ws.ws_row;
}

int PseudoTerm::forkPty(){
    tws.ws_col = 80;
    tws.ws_row = 24;

    initTermios();

    if(::openpty(&amaster, &aslave, tname, &term, &tws) < 0){
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
            unsigned char buf[128];
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
