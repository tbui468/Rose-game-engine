#ifndef BUTTON_H
#define BUTTON_H

#include "Quad.h"

namespace sqs {

class Button : public Quad {
    public:
        Button() = default;
        Button(CartCoords pos, uint32_t depth) {
            SetPosition(pos);
            SetDepth(depth);
        }
        virtual ~Button() {}
        bool OnClick() {
            std::cout << "I'm a button " << std::endl;
            return true;
        }

    private:
};

}

#endif //BUTTON_H
