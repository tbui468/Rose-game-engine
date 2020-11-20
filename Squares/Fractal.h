#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Fractal: public rose::Entity {
        public:
            Fractal(int size, const glm::ivec2& index, const glm::vec2& pos);
            virtual ~Fractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            void SetIndex(const glm::ivec2& index);
        private:
            Fractal(rose::EntityData e);
            rose::EntityData MakeEntityData(int size, const glm::ivec2& index, const glm::vec2& pos) const;
        public:
            glm::ivec2 GetIndex() const { return m_Index; }
            int GetSize() const { return m_Size; }
        private:
            glm::ivec2 m_Index {0, 0};
            int m_Size {0};
    };

}


#endif //FRACTAL_H
