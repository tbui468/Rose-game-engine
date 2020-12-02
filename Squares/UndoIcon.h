#ifndef UNDO_ICON_H
#define UNDO_ICON_H


#include "Rose.h"

namespace sqs {

class Puzzle;

class UndoIcon: public rose::Entity {
    public:
        UndoIcon(const glm::ivec2& pos, Puzzle* puzzle);
        virtual ~UndoIcon() {}
        Puzzle* GetPuzzle() const { return m_PuzzleHandle; }
    public:
        inline static float s_Margin {24.0f};
    private:
        Puzzle* m_PuzzleHandle {nullptr};
        inline static rose::Sprite s_Sprite {{32, 32}, {8, 8}, rose::TextureType::Default};
        inline static glm::vec2 s_ObjectSize {8.0f, 8.0f};
        inline static glm::vec4 s_BoundingBox {0.0f, 0.0f, 8.0f, 8.0f};
};

}


#endif //UNDO_ICON_H
