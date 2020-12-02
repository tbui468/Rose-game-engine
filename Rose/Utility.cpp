#include "Utility.h"

namespace rose {


float PointDistance(const glm::ivec2& start, const glm::ivec2& end) {
    float deltaXSqr = pow(start.x - end.x, 2);
    float deltaYSqr = pow(start.y - end.y, 2);
    return sqrt(deltaXSqr + deltaYSqr);
}


}
