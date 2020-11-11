#ifndef BUTTON_H
#define BUTTON_H

#include "Entity.h"

namespace rose {

class Button : public Entity {
    public:
        Button() = default;
        Button(CartCoords pos, uint32_t depth) : Entity(pos, depth) {}
        virtual ~Button() {}
    private:
};

}

#endif //BUTTON_H
