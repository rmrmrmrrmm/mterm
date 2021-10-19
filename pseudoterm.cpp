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
    if(::mkfifo("/tmp/FifoTest",0666)==-1){
        perror("mkfifo");
    }
    forkPty();
}

PseudoTerm::~PseudoTerm()
{
    ::remove("/tmp/FifoTest");
    close(amaster);
    delete(childp);
}

void PseudoTerm::keyPressed(int keycode){
    char buf = KeyUtil::translateKey(keycode);
    if(buf != -1)
    ::write(amaster, &buf, 1);
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
            ::exit(0);
    }

    ::close(aslave);

    switch((childp = childptr::getInst(childp, ::fork()))->getPid()){
        case -1:
            exit(-1);
        case 0:
            int nread;
            char buf[256];
            if((pip=open("/tmp/FifoTest", O_RDWR  | O_NONBLOCK))==-1){
                perror("open");
                    exit(-1);
            }
            for(;;){
                if ((nread = ::read(amaster, buf, 127)) <= 0){
                    break;
                }
                buf[nread] = 0;
                ::write(pip, &nread, 1);
                ::write(pip, buf, nread + 1);
            }
            std::cout << "exit: " << nread << std::endl;
            ::close(pip);
            ::close(amaster);
            ::exit(0);
    }

    return 0;
}

}
