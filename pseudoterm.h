#ifndef PSEUDOTERM_H
#define PSEUDOTERM_H

#include <string>
#include <sstream>
#include "unistd.h"
#include "signal.h"

namespace pterm{

    class PseudoTerm {

        public:
            PseudoTerm();
            ~PseudoTerm();

            void keyPressed(int keycode);
            void keyReleased(int keycode);

        private:
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
            int amaster;
            int aslave;
            int pip;
            childptr *childp = nullptr;
            int forkPty();
    };

}
#endif  //PSEUDOTERM_H
