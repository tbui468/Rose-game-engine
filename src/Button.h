#ifndef BUTTON_H
#define BUTTON_H

#include "Entity.h"
#include "renderer/Texture.h"

namespace rose {

class Button : public Entity {
    public:
        Button() = default;
        Button(const std::string& sprite, const glm::vec2& pos) : Entity(sprite, pos) {}
        virtual ~Button() {}
    private:
};

}

#endif //BUTTON_H
