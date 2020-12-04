#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Fractal: public rose::Entity {
        public:
            Fractal() {}
            virtual ~Fractal() {}
            virtual int GetSize() const = 0;
            virtual int GetPuzzleIndex() const = 0;
            virtual const std::vector<FractalElement>& GetElements() const = 0;
            virtual void SetElements(std::vector<FractalElement> elements) = 0;
            virtual void UpdateSprite() = 0;
            virtual bool Contains(const glm::ivec2& index) const = 0;
            virtual const std::vector<FractalElement>& GetSubElements(const glm::ivec2& index, int size) const = 0; //size == width == height
        public:
            static rose::EntityData MakeEntityData(const T& elements, const glm::ivec2& index, const glm::vec2& pos, int puzzleNumber);
            static void UpdateTextureData(const T& elements, const glm::ivec2& index, int puzzleNumber);
            static glm::ivec2 GetTextureStart(const glm::ivec2& index, int puzzleNumber);
        private:
    };


}

#endif // FRACTAL_H
