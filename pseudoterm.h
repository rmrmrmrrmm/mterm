#ifndef PSEUDOTERM_H
#define PSEUDOTERM_H

#include <string>
#include <sstream>

namespace pterm{

    class PseudoTerm{
        public:
            PseudoTerm();
            ~PseudoTerm();

            void keyPressed(int keycode);
            void keyReleased(int keycode);

        private:
            int amaster;
            int aslave;
            int childp;
            int forkPty();

            std::stringstream is, os;

    };

}
#endif  //PSEUDOTERM_H
