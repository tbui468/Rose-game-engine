#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Fractal: public rose::Entity {
        public:
            Fractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
            virtual ~Fractal() {}
        private:
    };

}


#endif //FRACTAL_H
