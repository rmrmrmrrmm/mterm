#include "pseudoterm.h"

#include "fcntl.h"
#include "pty.h"
#include "utmp.h"
#include "stdlib.h"
#include "keyutil.h"

#include "iostream"

namespace pterm{

PseudoTerm::PseudoTerm()
{
    forkPty();
}

PseudoTerm::~PseudoTerm()
{
    close(amaster);
    delete(childp);
}

void PseudoTerm::keyPressed(int keycode){
    char buf = KeyUtil::translateKey(keycode);
    if(buf != -1)
    ::write(amaster, &buf, 1);
}

int PseudoTerm::forkPty(){
    int pip[2];

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

    if(::pipe(pip)== -1){
        exit(-1);
    }

    switch((childp = childptr::getInst(childp, ::fork()))->getPid()){
        case -1:
            exit(-1);
        case 0:
            int nread;
            char buf[256];
            ::close(pip[0]);
            for(;;){
                if ((nread = ::read(amaster, buf, 255)) <= 0) break;
                buf[nread] = 0;
                if (::write(pip[1], buf, nread + 1) != nread + 1) break;
            }
            ::close(pip[1]);
            ::close(amaster);
            ::exit(0);
    }

    ::close(pip[1]);



    switch((childp = childptr::getInst(childp, ::fork()))->getPid()){
        case -1:
            exit(-1);
        case 0:
            char buf[256];
            for(;;){
                ::read(pip[0], buf, 256);
               std::cout << buf << std::ends << std::flush;
            }
            ::close(pip[0]);
            ::close(amaster);
            ::exit(0);
    }
    ::close(pip[0]);

    return 0;
}

}
