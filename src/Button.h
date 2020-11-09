#ifndef BUTTON_H
#define BUTTON_H

#include "Quad.h"

namespace sqs {

class Button : public Quad {
    public:
        Button() = default;
        Button(CartCoords pos, uint32_t depth);
        virtual ~Button() {}
        bool OnClick();
};

}

#endif //BUTTON_H
