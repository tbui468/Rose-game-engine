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

    struct TransformationData {
        TransformationType Transformation {TransformationType::None};
        glm::ivec2 FractalIndex {-1, -1};
        int FractalSize {0};
    };


    struct FractalData {
        int size {0};
        glm::ivec2 index {-1, -1};
    };



    extern std::vector<PuzzleSetData> g_Data;

}


#endif //DATA_H
