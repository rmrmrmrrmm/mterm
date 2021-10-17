#include "pseudoterm.h"

#include "unistd.h"
#include "pty.h"
#include "utmp.h"
#include "stdlib.h"
#include "signal.h"
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
    if(childp!=0)::kill(childp, SIGTERM);;
}

void PseudoTerm::keyPressed(int keycode){
    std::string s = KeyUtil::translateKey(keycode);
    //::write(amaster, s.c_str(), s.length());
}

int PseudoTerm::forkPty(){
    int nread;
    char buf[256];
    int pip[2];
    std::string str;

    if(::openpty(&amaster, &aslave, nullptr, NULL, nullptr) < 0){
              exit(1);
    }

    switch(childp = ::fork()){
        case -1:
            ::exit(1);

        case 0:
            //child
            close(amaster);
            if(login_tty(aslave) < 0) {
                ::exit(1);
            }
            ::execlp("bash", "");
            ::exit(0);
    }

    //parent
    close(aslave);

    if(::pipe(pip)== -1){
        exit(-1);
    }

    switch(childp = ::fork()){
        case -1:
            exit(-1);
        case 0:
            ::close(pip[0]);
            for(;;){
                if ((nread = ::read(amaster, buf, 256)) <= 0) break;
                if (::write(pip[1], buf, nread) != nread) break;
            }
            ::exit(0);
    }

    ::close(pip[1]);

    /*
    //0x08 BS 0x0d CR
    str = "ls /usr/bin | more";
    ::write(amaster, str.c_str(), str.length());
    str = 0x0d;
    ::write(amaster, str.c_str(), str.length());
    sleep(1);
    str = "q";
    //str+= 0x0d;
    ::write(amaster, str.c_str(), str.length());

    sleep(1);
    str = "ls /usr/bin | more";
    ::write(amaster, str.c_str(), str.length());
    str = 0x0d;
    ::write(amaster, str.c_str(), str.length());
    sleep(1);
    str = "q";
    //str+= 0x0d;
    ::write(amaster, str.c_str(), str.length());
    //str = 0x0d;
    //::write(amaster, str.c_str(), str.length());
    str = "";

    sleep(3);
    for(int i = 0; i < 3; i++){
        ::read(pip[0], buf, 256);
        str.assign(buf);
        std::cout << str << std::endl;
        str = "";
        sleep(1);
    }
*/

    return childp;
}

}
