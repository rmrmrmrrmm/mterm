#include "pseudoterm.h"

#include "fcntl.h"
#include "pty.h"
#include "utmp.h"
#include "stdlib.h"
#include "keyutil.h"
#include <sys/stat.h>

#include "iostream"

namespace pterm{

PseudoTerm::PseudoTerm()
{
    initPipe();
    forkPty();
}

PseudoTerm::~PseudoTerm()
{
    close(pip);
    ::remove(FIFONAME.c_str());
    close(amaster);
    delete(childp);
}

void PseudoTerm::keyPressed(int keycode){
    char buf = KeyUtil::translateKey(keycode);
    if(buf != -1)
    ::write(amaster, &buf, 1);
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

int PseudoTerm::forkPty(){

    if(::openpty(&amaster, &aslave, nullptr, NULL, nullptr) < 0){
              exit(1);
    }

    switch(::fork()){
        case -1:
            ::exit(1);

        case 0:
            close(amaster);
            if(login_tty(aslave) < 0) {
                ::exit(1);
            }
            ::execlp("bash", "");
            ::exit(1);
    }

    ::close(aslave);


    switch((childp = childptr::getInst(childp, ::fork()))->getPid()){
        case -1:
            exit(-1);
        case 0:
            int nread;
            char buf[256];
            for(;;){
                if ((nread = ::read(amaster, buf, 127)) <= 0){
                    break;
                }
                buf[nread] = 0;
                ::write(getPipe(), &nread, 1);
                ::write(getPipe(), buf, nread + 1);
            }
            std::cout << "exit: " << nread << std::endl;
            ::exit(0);
    }

    return 0;
}

}
