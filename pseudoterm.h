#ifndef PSEUDOTERM_H
#define PSEUDOTERM_H

#include <string>
#include <sstream>
#include <termios.h>
#include <sys/ioctl.h>
#include "unistd.h"
#include "signal.h"

namespace pterm{

    class PseudoTerm {

        public:
            PseudoTerm();
            ~PseudoTerm();

            void keyPressed(int keycode);
            void keyReleased(int keycode);
            void initPipe();
            int getPipe();
            bool isPipeExists();
            char *getTname();
            void setWinSize(winsize ws);

        private:
            int amaster;
            int aslave;
            int pip = -1;

            char *tname;
            struct ::termios term;
            struct ::winsize tws;
            const std::string FIFONAME = "/tmp/mterm-" + std::to_string(getpid());

            int forkPty();
            void initTermios();

            class childptr{
                private:
                childptr *prev;
                int pid;

                childptr(childptr *ptr, int pid){
                    prev = ptr;
                    this->pid = pid;
                };
                public:
                ~childptr(){
                    if(prev!=nullptr)
                        delete prev;
                    ::kill(pid, SIGTERM);
                };
                static childptr* getInst(childptr*ptr, int pid){
                    return new childptr(ptr, pid);
                }
                int getPid(){
                    return pid;
                }
            };
            childptr *childp = nullptr;

            typedef union{
                int keycode;
                unsigned char char_4[4];
            } _Char;
    };

}
#endif  //PSEUDOTERM_H
