#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Fractal: public rose::Entity {
        public:
            Fractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, const glm::ivec2& index);
            virtual ~Fractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index);
        private:
            glm::ivec2 m_Index {0, 0};
    };

}


#endif //FRACTAL_H
