#ifndef DATA_H
#define DATA_H

#include <vector>
#include <glm/glm.hpp>

namespace sqs {

    typedef char FractalElement;

    enum class TransformationType {
        None,
        TranslatePosX,
        TranslateNegX,
        TranslatePosY,
        TranslateNegY,
        RotateCW,
        RotateCCW,
        ReflectX,
        ReflectY
    };

    struct PuzzleData {
        glm::ivec2 dimensions {-1, -1};
        std::vector<FractalElement> elements;
    };

    struct PuzzleSetData {
        std::vector<PuzzleData> puzzlesData;
    };

    struct FractalData {
        int size {0};
        glm::ivec2 index {-1, -1};
    };

    struct TransformationData {
        TransformationType transformation {TransformationType::None};
        FractalData fractalData {0, {-1 ,-1}};
    };


    extern std::vector<PuzzleSetData> g_Data;
    const extern std::vector<PuzzleSetData> g_DefaultData;

}


#endif //DATA_H
