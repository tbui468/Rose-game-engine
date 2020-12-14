#include "Fractal.h"

namespace sqs {


    //Fractal::Fractal(rose::Sprite sprite, FractalData data, const glm::vec2& coords, void (Puzzle::*func)(FractalData, TransformationType)):
    Fractal::Fractal(rose::Sprite sprite, FractalData data, const glm::vec2& coords, CallbackData callback):
        Entity(sprite, {s_UnitSize * data.size, s_UnitSize * data.size}, {0.0f, 0.0f, s_UnitSize * data.size, s_UnitSize * data.size}, coords),
        m_Callback(callback), m_Data(data) {}



    void Fractal::Transform(TransformationType type) { 
        (m_Callback.puzzle->*m_Callback.transformCallback)(m_Data, type); 
        m_ShouldRecreate = true;
    }

    //return type is read as follows:
    //f1 is Within f2 (f2 is larger)
    //f1 Partially overlaps f2 (and vice versa)
    //f1 Encloses f2 (f1 is larger)
    //f1 is Equal to f2 (f1 and f2 are equal in size and have same index)
    OverlapType Fractal::FindOverlapType(FractalData f1, FractalData f2) {

        if(f1.size == f2.size && f1.index.x == f2.index.x && f1.index.y == f2.index.y) return OverlapType::Equal;

        //check if f1 is fully inside f2
        bool f1InsideX = f1.index.x >= f2.index.x && f1.index.x + f1.size <= f2.index.x + f2.size;
        bool f1InsideY = f1.index.y >= f2.index.y && f1.index.y + f1.size <= f2.index.y + f2.size;
        if(f1InsideX && f1InsideY) return OverlapType::Within;

        //check if f2 is fully inside f1 (should split)
        bool f2InsideX = f2.index.x >= f1.index.x && f2.index.x + f2.size <= f1.index.x + f1.size;
        bool f2InsideY = f2.index.y >= f1.index.y && f2.index.y + f2.size <= f1.index.y + f1.size;
        if(f2InsideX && f2InsideY) return OverlapType::Enclose;

        bool noXOverlap = f1.index.x + f1.size <= f2.index.x || f2.index.x + f2.size <= f1.index.x;
        bool noYOverlap = f1.index.y + f1.size <= f2.index.y || f2.index.y + f2.size <= f1.index.y;

        if(noXOverlap || noYOverlap) return OverlapType::None;

        return OverlapType::Partial; //should split

    }

    bool Fractal::Contains(const glm::ivec2& index) const {
        if(index.x < GetIndex().x) return false;
        if(index.y < GetIndex().y) return false;
        if(index.x >= GetIndex().x + GetSize()) return false;
        if(index.y >= GetIndex().y + GetSize()) return false;
        return true;
    }


    rose::Edge Fractal::EdgeCollision(float pointX, float pointY) const {
        glm::vec2 point = {pointX * rose::g_Scale, pointY * rose::g_Scale};

        float hWidth = m_BoundingBox.z / 2.0f;
        float hHeight = m_BoundingBox.w / 2.0f;

        glm::vec2 vertBox = { hWidth / 2.0f, hHeight };
        glm::vec2 horiBox = { hWidth, hHeight / 2.0f };

        if(rose::PointInRectangle({m_Pos.x - hWidth, m_Pos.y}, vertBox, point)) return rose::Edge::Left;
        if(rose::PointInRectangle({m_Pos.x + hWidth, m_Pos.y}, vertBox, point)) return rose::Edge::Right;
        if(rose::PointInRectangle({m_Pos.x, m_Pos.y + hHeight}, horiBox, point)) return rose::Edge::Top;
        if(rose::PointInRectangle({m_Pos.x, m_Pos.y - hHeight}, horiBox, point)) return rose::Edge::Bottom;

        return rose::Edge::None;
    }

    rose::Corner Fractal::CornerCollision(float pointX, float pointY) const {
        glm::vec2 point = {pointX * rose::g_Scale, pointY * rose::g_Scale};

        float hWidth = m_BoundingBox.z / 2.0f;
        float hHeight = m_BoundingBox.w / 2.0f;
        glm::vec2 cornerBox = {hWidth/2.0f, hHeight/2.0f}; //make corner collision boxes 1/4 of the width/height

        if(rose::PointInRectangle({m_Pos.x - hWidth, m_Pos.y + hHeight}, cornerBox, point)) return rose::Corner::TopLeft;
        if(rose::PointInRectangle({m_Pos.x + hWidth, m_Pos.y + hHeight}, cornerBox, point)) return rose::Corner::TopRight;
        if(rose::PointInRectangle({m_Pos.x - hWidth, m_Pos.y - hHeight}, cornerBox, point)) return rose::Corner::BottomLeft;
        if(rose::PointInRectangle({m_Pos.x + hWidth, m_Pos.y - hHeight}, cornerBox, point)) return rose::Corner::BottomRight;
        //base off of s_UnitMargin and m_BoundingBox
        //rose::PointInRectangle(start, end, point)
        return rose::Corner::None;
    }

    void Fractal::OnAnimationEnd() {
        Entity::OnAnimationEnd();
        //setting fractal angles  and scale back to 0 to avoid saving states
        m_Angle = 0.0f;
        m_ToAngle = 0.0f;
        m_FromAngle = 0.0f;
        m_FromScale = {1.0f, 1.0f};
        m_Scale = {1.0f, 1.0f};
        m_ToScale = {1.0f, 1.0f};

        //UpdateTextureData(m_Elements, GetIndex(), m_PuzzleIndex); //@todo: move this function to puzzle class
        //        UpdateTextureData(m_Elements, GetIndex(), 0); //temp puzzle index of 0
        //       UpdateSprite(); //@todo: this shouldn't be necessary - when OnAnimationEnd() is called, transformed fractals should be destroyed and a new fractal created
    }


    void Fractal::UpdateSprite() { //@todo: move to puzzle
        int size = GetSize();
        float fWidth = s_UnitSize * size;
        float fHeight = s_UnitSize * size;


        //glm::ivec2 texStart = GetTextureStart(GetIndex(), GetPuzzleIndex()); //putting in temp puzzle index of 0
        glm::ivec2 texStart = GetTextureStart(GetIndex(), 0);


        rose::Sprite sprite = { {texStart.x, texStart.y - (size - 1) * s_UnitSize}, {fWidth, fHeight}, rose::TextureType::Custom };
        Entity::SetSprite(sprite); //@todo: shouldn't need this anymore - will destroy and recreate fractal when animation is over, and will initialize with new sprite
    }



    glm::ivec2 Fractal::GetTextureStart(const glm::ivec2& index, int puzzleNumber) {
        return glm::ivec2(index.x * s_UnitSize + puzzleNumber * 256, 256 - (index.y + 1) * s_UnitSize); 
    }


    glm::vec2 Fractal::GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
            const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {
        glm::vec2 targetCenter = GetCoords(targetIndex, targetSize, puzzleDim, puzzlePos);

        float targetHalfWidth = size * s_UnitSize * (targetSize / size - 1) * 0.5f; //distance between centers of the fractals on left/right edges (or top/bottom)

        glm::vec2 targetTopLeft = glm::vec2(targetCenter.x - targetHalfWidth, targetCenter.y + targetHalfWidth); //center of top left fractal

        //using target center, start from targetIndex location and 
        glm::ivec2 relativeIndex = glm::ivec2(index.x - targetIndex.x, index.y - targetIndex.y);
        return glm::vec2(targetTopLeft.x + relativeIndex.x * s_UnitSize, targetTopLeft.y - relativeIndex.y * s_UnitSize);
    }

    glm::vec2 Fractal::GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {
        float puzzleWidth = (puzzleDim.x - 1) * (s_UnitSize + s_UnitMargin); //this gives us width/height if all fractals are 1x1
        float puzzleHeight = (puzzleDim.y - 1) * (s_UnitSize + s_UnitMargin);
        glm::vec2 topLeftGrid = glm::vec2(puzzlePos.x - puzzleWidth / 2.0f, puzzlePos.y + puzzleHeight/ 2.0f); //this gives center of topleft fractal
        glm::vec2 topLeftFractal = glm::vec2(topLeftGrid.x + index.x * (s_UnitSize + s_UnitMargin), 
                topLeftGrid.y - index.y * (s_UnitSize + s_UnitMargin));
        float fractalWidth = (size - 1) * (s_UnitSize + s_UnitMargin);
        return glm::vec2(topLeftFractal.x + fractalWidth / 2.0f, topLeftFractal.y - fractalWidth / 2.0f);
    }




}
