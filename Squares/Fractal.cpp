#include "Fractal.h"

namespace sqs {


    Fractal::Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& coords, int puzzleIndex):
        Fractal(MakeEntityData(elements, index, coords, puzzleIndex), elements, index, puzzleIndex) {}


    Fractal::Fractal(rose::EntityData e, const std::vector<FractalElement>& elements, const glm::ivec2& index, int puzzleIndex)
        : Entity(e.sprite, e.size, e.boundingBox, e.position), m_PuzzleIndex(puzzleIndex) {
            m_Elements = elements;
            m_Index = index;
            switch(elements.size()) {
                case 1: m_Size = 1; break;
                case 4: m_Size = 2; break;
                case 16: m_Size = 4; break;
                default: assert(false); break;
            }
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

    FractalElement Fractal::GetSubElement(const glm::ivec2& index) const {
        return GetSubElements(index, 1).front();
    }

    std::vector<FractalElement> Fractal::GetSubElements(const glm::ivec2& index, int subElementSize) const {
        std::vector<FractalElement> elements;

        for(int row = index.y; row < index.y + subElementSize; ++row) {
            for(int col = index.x; col < index.x + subElementSize; ++col) {
                elements.push_back(m_Elements.at(row * GetSize() + col));
            }
        }

        return elements;
    }

    void Fractal::WriteData(std::vector<PuzzleSetData>& data, int setIndex, int puzzleIndex) const {
        int width = data.at(setIndex).puzzlesData.at(puzzleIndex).dimensions.x;

        for(int row = 0; row < GetSize(); ++row) {
            for(int col = 0; col < GetSize(); ++col) {
                data.at(setIndex).puzzlesData.at(puzzleIndex).elements.at((GetIndex().y + row) * width + GetIndex().x + col) = GetSubElement({col, row});
            }
        }
    }

    void Fractal::RotateBy(float angle) {
        Entity::RotateBy(angle);

        if(GetSize() == 1) return;

        if(GetSize() == 2) {
            if(angle < 0) { //clockwise
                FractalElement topLeft = GetSubElement({0, 1});
                FractalElement topRight = GetSubElement({0, 0});
                FractalElement bottomLeft = GetSubElement({1, 1});
                FractalElement bottomRight = GetSubElement({1, 0});
                SetElements({topLeft, topRight, bottomLeft, bottomRight});
            }else{
                FractalElement topLeft = GetSubElement({1, 0});
                FractalElement topRight = GetSubElement({1, 1});
                FractalElement bottomLeft = GetSubElement({0, 0});
                FractalElement bottomRight = GetSubElement({0, 1});
                SetElements({topLeft, topRight, bottomLeft, bottomRight});
            }
            return;
        }

        if(GetSize() == 4) {
            if(angle < 0) { //clockwise
                FractalElement e00 = GetSubElement({0, 3});
                FractalElement e10 = GetSubElement({0, 2});
                FractalElement e20 = GetSubElement({0, 1});
                FractalElement e30 = GetSubElement({0, 0});

                FractalElement e01 = GetSubElement({1, 3});
                FractalElement e11 = GetSubElement({1, 2});
                FractalElement e21 = GetSubElement({1, 1});
                FractalElement e31 = GetSubElement({1, 0});

                FractalElement e02 = GetSubElement({2, 3});
                FractalElement e12 = GetSubElement({2, 2});
                FractalElement e22 = GetSubElement({2, 1});
                FractalElement e32 = GetSubElement({2, 0});

                FractalElement e03 = GetSubElement({3, 3});
                FractalElement e13 = GetSubElement({3, 2});
                FractalElement e23 = GetSubElement({3, 1});
                FractalElement e33 = GetSubElement({3, 0});

                SetElements({e00, e10, e20, e30,
                        e01, e11, e21, e31,
                        e02, e12, e22, e32,
                        e03, e13, e23, e33});
            }else{
                FractalElement e00 = GetSubElement({3, 0});
                FractalElement e10 = GetSubElement({3, 1});
                FractalElement e20 = GetSubElement({3, 2});
                FractalElement e30 = GetSubElement({3, 3});

                FractalElement e01 = GetSubElement({2, 0});
                FractalElement e11 = GetSubElement({2, 1});
                FractalElement e21 = GetSubElement({2, 2});
                FractalElement e31 = GetSubElement({2, 3});

                FractalElement e02 = GetSubElement({1, 0});
                FractalElement e12 = GetSubElement({1, 1});
                FractalElement e22 = GetSubElement({1, 2});
                FractalElement e32 = GetSubElement({1, 3});

                FractalElement e03 = GetSubElement({0, 0});
                FractalElement e13 = GetSubElement({0, 1});
                FractalElement e23 = GetSubElement({0, 2});
                FractalElement e33 = GetSubElement({0, 3});

                SetElements({e00, e10, e20, e30,
                        e01, e11, e21, e31,
                        e02, e12, e22, e32,
                        e03, e13, e23, e33});
            }
            return;
        }

    }

    void Fractal::ScaleTo(const glm::vec2& scale) {
        Entity::ScaleTo(scale);

        if(GetSize() == 1) return;

        if(GetSize() == 2) {
            if(scale.y < 0) { //x-axis reflection
                FractalElement topLeft = GetSubElement({0, 1});
                FractalElement topRight = GetSubElement({1, 1});
                FractalElement bottomLeft = GetSubElement({0, 0});
                FractalElement bottomRight = GetSubElement({1, 0});
                SetElements({topLeft, topRight, bottomLeft, bottomRight});
            }else{
                FractalElement topLeft = GetSubElement({1, 0});
                FractalElement topRight = GetSubElement({0, 0});
                FractalElement bottomLeft = GetSubElement({1, 1});
                FractalElement bottomRight = GetSubElement({0, 1});
                SetElements({topLeft, topRight, bottomLeft, bottomRight});
            }
            return;
        }

        if(GetSize() == 4) {
            if(scale.y < 0) { //x-axis reflection
                FractalElement e00 = GetSubElement({0, 3});
                FractalElement e10 = GetSubElement({1, 3});
                FractalElement e20 = GetSubElement({2, 3});
                FractalElement e30 = GetSubElement({3, 3});

                FractalElement e01 = GetSubElement({0, 2});
                FractalElement e11 = GetSubElement({1, 2});
                FractalElement e21 = GetSubElement({2, 2});
                FractalElement e31 = GetSubElement({3, 2});

                FractalElement e02 = GetSubElement({0, 1});
                FractalElement e12 = GetSubElement({1, 1});
                FractalElement e22 = GetSubElement({2, 1});
                FractalElement e32 = GetSubElement({3, 1});

                FractalElement e03 = GetSubElement({0, 0});
                FractalElement e13 = GetSubElement({1, 0});
                FractalElement e23 = GetSubElement({2, 0});
                FractalElement e33 = GetSubElement({3, 0});

                SetElements({e00, e10, e20, e30,
                        e01, e11, e21, e31,
                        e02, e12, e22, e32,
                        e03, e13, e23, e33});
            }else{ //y-axis reflection
                FractalElement e00 = GetSubElement({3, 0});
                FractalElement e10 = GetSubElement({2, 0});
                FractalElement e20 = GetSubElement({1, 0});
                FractalElement e30 = GetSubElement({0, 0});

                FractalElement e01 = GetSubElement({3, 1});
                FractalElement e11 = GetSubElement({2, 1});
                FractalElement e21 = GetSubElement({1, 1});
                FractalElement e31 = GetSubElement({0, 1});

                FractalElement e02 = GetSubElement({3, 2});
                FractalElement e12 = GetSubElement({2, 2});
                FractalElement e22 = GetSubElement({1, 2});
                FractalElement e32 = GetSubElement({0, 2});

                FractalElement e03 = GetSubElement({3, 3});
                FractalElement e13 = GetSubElement({2, 3});
                FractalElement e23 = GetSubElement({1, 3});
                FractalElement e33 = GetSubElement({0, 3});

                SetElements({e00, e10, e20, e30,
                        e01, e11, e21, e31,
                        e02, e12, e22, e32,
                        e03, e13, e23, e33});
            }
            return;
        }

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

        UpdateTextureData(m_Elements, GetIndex(), m_PuzzleIndex);
        UpdateSprite();
    }

    rose::EntityData Fractal::MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& pos, int puzzleIndex) {
        UpdateTextureData(elements, index, puzzleIndex);

        int size = sqrt(elements.size() + 1);
        float fWidth = s_UnitSize * size;
        float fHeight = s_UnitSize * size;

        glm::ivec2 texStart = GetTextureStart(index, puzzleIndex);

        glm::vec2 entitySize = glm::vec2(fWidth, fHeight);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, fWidth, fHeight);
        rose::EntityData e;

        e.sprite = { {texStart.x, texStart.y - (size - 1) * s_UnitSize}, {fWidth, fHeight}, rose::TextureType::Custom };
        e.size = entitySize;
        e.boundingBox = boundingBox;
        e.position = pos;

        return e;
    }


    void Fractal::UpdateSprite() {
        int size = GetSize();
        float fWidth = s_UnitSize * size;
        float fHeight = s_UnitSize * size;


        glm::ivec2 texStart = GetTextureStart(GetIndex(), GetPuzzleIndex());


        rose::Sprite sprite = { {texStart.x, texStart.y - (size - 1) * s_UnitSize}, {fWidth, fHeight}, rose::TextureType::Custom };
        Entity::SetSprite(sprite);
    }


    void Fractal::UpdateTextureData(const std::vector<FractalElement>& elements, const glm::ivec2& index, int puzzleNumber) {
        int size = sqrt(elements.size() + 1);

        float fWidth = s_UnitSize * size;
        float fHeight = s_UnitSize * size;

        //textures start from bottom left, but starting textures from top left to fit fractal order (left to right, top to bottom)
        glm::ivec2 texStart = GetTextureStart(index, puzzleNumber);

        std::vector<rose::SubTextureMapping> texMapping;

        for(int row = 0; row < size; ++row) {
            for(int col = 0; col < size; ++col) {
                texMapping.push_back({{texStart.x + col * s_UnitSize, texStart.y - row * s_UnitSize}, {0, 0}, {s_UnitSize, s_UnitSize}}); //fractal frame

                //elements are drawn inside fractal frame, so start point is offset by 1 and side length is reduced by 2 in each dimension
                FractalElement element = elements.at(row * size + col);

                switch(element) {
                    case 'r':
                        texMapping.push_back({{texStart.x + col * s_UnitSize + 1, texStart.y - row * s_UnitSize + 1}, 
                                {s_UnitSize + 1, 1}, {s_UnitSize - 2, s_UnitSize - 2}});
                        break;
                    case 'b':
                        texMapping.push_back({{texStart.x + col * s_UnitSize + 1, texStart.y - row * s_UnitSize + 1}, 
                                {s_UnitSize + 1, s_UnitSize + 1}, {s_UnitSize - 2, s_UnitSize - 2}});
                        break;
                    case 'g':
                        texMapping.push_back({{texStart.x + col * s_UnitSize + 1, texStart.y - row * s_UnitSize + 1}, 
                                {s_UnitSize + 1, s_UnitSize * 2 + 1}, {s_UnitSize - 2, s_UnitSize - 2}});
                        break;
                }
            }
        }

        rose::Application::GetApplication()->SetCustomTexture(texMapping);
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
