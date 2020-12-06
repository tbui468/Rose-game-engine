#include "Utility.h"

namespace rose {


float PointDistance(const glm::ivec2& start, const glm::ivec2& end) {
    float deltaXSqr = pow(start.x - end.x, 2);
    float deltaYSqr = pow(start.y - end.y, 2);
    return sqrt(deltaXSqr + deltaYSqr);
}

bool PointInRectangle(const glm::vec2& center, const glm::vec2& dim, const glm::vec2& point) {
    if(point.x < (center.x - dim.x / 2.0f)) return false;
    if(point.x > (center.x + dim.x / 2.0f)) return false;
    if(point.y > (center.y + dim.y / 2.0f)) return false;
    if(point.y < (center.y - dim.y / 2.0f)) return false;

    return true;
}

float Sigmoid(float t) {
    return 1.0f / (1.0f + exp(-15.0f * (t - 0.5f)));
}


}
