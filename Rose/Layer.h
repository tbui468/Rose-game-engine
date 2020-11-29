#ifndef LAYER_H
#define LAYER_H

#include "Globals.h"

namespace rose {


class Layer {
    public:
        Layer(){}
        virtual ~Layer() {}
        virtual bool Update(double deltaTime, const std::array<bool, g_MaxKeys>& keys, 
                            const std::array<bool, g_MaxMouseButtons>& mouse , const glm::vec2& mousePos) = 0;
        virtual void Draw() = 0;
    private:
};

}

#endif //LAYER_H
