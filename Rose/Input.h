#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>

#include "InputType.h"

namespace rose {


class Input {
    public:
        static glm::ivec2 GetMousePos();
        static InputType GetInput();
};


}


#endif //INPUT_H
