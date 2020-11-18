#include "Fractal.h"

namespace sqs {

    Fractal::Fractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos)
        :Entity(sprite, size, boundingBox, pos) {
    }

}
