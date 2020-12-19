#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {


    Puzzle::Puzzle(int index, int setIndex): Entity(s_Sprite, s_ObjectSize, s_BoundingBox, glm::vec2(s_InitOffset + s_Spacing * index, 0.0f)),
    m_Index(index), 
    m_SetIndex(setIndex), 
    m_Dimensions(g_Data.at(setIndex).puzzlesData.at(index).dimensions) {

        //undo icons 
        for(int i = 0; i < GetMaxTransformations(); ++i) {
            m_UndoIcons.emplace_back(new UndoIcon(glm::vec2(UndoIcon::s_Margin * ((1 - GetMaxTransformations()) / 2.0f + i) + x(), -110.0f), this));
        }

        //make a bunch of 1x1 fractals as starting point for all puzzles
        for(int row = 0; row < m_Dimensions.y; ++row) {
            for(int col = 0; col < m_Dimensions.x; ++col) {
                UpdateTextureData({1, {col, row}});
                if(GetElementAt(col, row) != 'E') {
                    glm::vec2 startCoords = Fractal::GetCoords({col, row}, 1, m_Dimensions, glm::vec2(x(), y()));
                    m_Fractals.emplace_back(new Fractal(CalculateSpriteData({1, {col, row}}, m_Index), {1, {col, row}}, startCoords, {this, &Puzzle::Transform}));
                }
            }
        }

        rose::Application::GetApplication()->SetCustomTexture(m_TextureMap);
        m_TextureMap.clear();

    }


    Puzzle::~Puzzle() {
        for(Fractal* f: m_Fractals) {
            if(f) delete f;
        }

        for(UndoIcon* i: m_UndoIcons) {
            if(i) delete i;
        }

        //shouldn't need to delete pointers from m_MergeLists bc all those are inside m_Fractals (which are deleted above)
    }

    void Puzzle::Open() {
        m_IsOpen = true;
    }

    void Puzzle::Close() {
        m_IsOpen = false;
    }

    void Puzzle::MoveTo(const glm::vec2& pos) {
        Entity::MoveTo(pos);

        float deltaX = pos.x - x();
        float deltaY = pos.y - y();

        for(Fractal* f: m_Fractals) {
            if(f) f->MoveBy(glm::vec2(deltaX, deltaY));
        }

        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->MoveBy(glm::vec2(deltaX, deltaY));
        }
    }

    void Puzzle::MoveBy(const glm::vec2& shift) {
        Entity::MoveBy(shift);
        for(Fractal* f: m_Fractals) {
            if(f) f->MoveBy(shift);
        }

        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->MoveBy(shift);
        }
    }


    void Puzzle::OnAnimationEnd() {
        Entity::OnAnimationEnd();


        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->OnAnimationEnd();
        }

        std::vector<Fractal*>::iterator iter = m_Fractals.begin();

        while(iter != m_Fractals.end()) {
            Fractal* f = *iter;

            switch(f->GetAnimationEndEvent()) {
                case AnimationEndEvent::Destroy:
                    iter = m_Fractals.erase(iter);
                    delete f;
                    break;
                case AnimationEndEvent::Recreate:
                    iter = m_Fractals.erase(iter);
                    glm::vec2 startCoords = Fractal::GetCoords(f->m_Index, f->m_Size, m_Dimensions, {x(), y()});
                    iter = m_Fractals.insert(iter, new Fractal(CalculateSpriteData({f->m_Size, f->m_Index}, m_Index), 
                                             {f->m_Size, f->m_Index}, startCoords, {this, &Puzzle::Transform}));
                    delete f;
                    iter++;
                    break;
                case AnimationEndEvent::None:
                    f->OnAnimationEnd();
                    iter++;
                    break;
                default:
                    assert(false);
                    break;
            }
        }

        //create new fractals using merge data
        for(int i = 0; i < m_MergeList.size(); ++i) {
            FractalData data = m_MergeList.at(i);
            glm::vec2 startCoords = Fractal::GetCoords(data.index, data.size, m_Dimensions, {x(), y()});
            m_Fractals.emplace_back(new Fractal(CalculateSpriteData({data.size, data.index}, m_Index), data, startCoords, {this, &Puzzle::Transform}));
        }

        m_MergeList.clear();

        rose::Application::GetApplication()->SetCustomTexture(m_TextureMap);
        m_TextureMap.clear();

    }

    void Puzzle::OnAnimationUpdate(float t) {
        Entity::OnAnimationUpdate(t);
        for(Fractal* f: m_Fractals) {
            if(f) f->OnAnimationUpdate(t);
        }
        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->OnAnimationUpdate(t);
        }
    }

    void Puzzle::Draw() const {
        //    Entity::Draw(); //puzzle doesn't need/have a visual element
        for(Fractal* f: m_Fractals) {
            if(f) f->Draw();
        }

        for(int i = 0; i < GetTransformationCount(); ++i) {
            UndoIcon* icon = m_UndoIcons.at(i);
            if(icon) icon->Draw();
        }
    }


    Fractal* Puzzle::GetFractalContaining(const glm::ivec2& index) const {
        for(Fractal* f: m_Fractals) {
            if(f->Contains(index)) return f;
        }
        return nullptr;
    }


    Fractal* Puzzle::GetFractalWithIndex(const glm::ivec2& index) const {
        for(Fractal* f: m_Fractals) {
            if(f->m_Index == index) return f;
        }
        return nullptr;
    }


    std::vector<Fractal*> Puzzle::FindFractalsInsideOf(FractalData data, const std::vector<Fractal*>& fractals) const {
        std::vector<Fractal*> properSubsetFractals;

        for(Fractal* f: fractals) {
            OverlapType type = Fractal::FindOverlapType({f->m_Size, f->m_Index}, data);
            if(type == OverlapType::Within || type == OverlapType::Equal) {
                properSubsetFractals.push_back(f);
            }
        }

        return properSubsetFractals;
    }

    std::vector<Fractal*> Puzzle::FindFractalsPartialIntersectionWith(const std::vector<FractalData>& dataList, const std::vector<Fractal*>& fractals) const {
        std::vector<Fractal*> overlappingFractals;

        for(Fractal* f: fractals) {
            for(FractalData fData: dataList) {
                OverlapType type = Fractal::FindOverlapType({f->m_Size, f->m_Index}, fData);
                if(type == OverlapType::Partial || type == OverlapType::Enclose) {
                    overlappingFractals.push_back(f);
                    break; //don't want to add it again if already overlapping with one target fractal data
                }
            }
        }

        return overlappingFractals;
    }



    std::vector<Fractal*> Puzzle::SplitFractals(const std::vector<Fractal*>& fractals) {

        std::vector<Fractal*> splitFractals;

        for(Fractal* f: fractals) {
            if(f->m_Size > 1) {

                //split f into quarters of half size in each dimension
                for(int yIndex = f->m_Index.y; yIndex < f->m_Index.y + f->m_Size; yIndex += f->m_Size/2) {
                    for(int xIndex = f->m_Index.x; xIndex < f->m_Index.x + f->m_Size; xIndex += f->m_Size/2) {
                        glm::vec2 startCoords = Fractal::GetCoordsForTarget({xIndex, yIndex}, f->m_Size/2, f->m_Index, f->m_Size, m_Dimensions, {x(), y()});
                        FractalData data = {f->m_Size/2, {xIndex, yIndex}};
                        Fractal* newFractal = new Fractal(CalculateSpriteData(data, m_Index), data, startCoords, {this, &Puzzle::Transform});
                        m_Fractals.push_back(newFractal);
                        splitFractals.push_back(newFractal);
                    }
                }

                m_Fractals.erase(GetFractalIterator(f));
                delete f;
            }
        }

        return splitFractals;
    }


    std::vector<Fractal*> Puzzle::SplitFractals(const std::vector<Fractal*>& fractals, const std::vector<FractalData>& dataList) {

        std::vector<Fractal*> splitFractals;

        for(Fractal* f: fractals) {
            switch(f->m_Size) {
                case 1:
                    //don't need to split 1x1 fractal
                    break;
                case 2:
                    //split f into quarters of size 1
                    for(int yIndex = f->m_Index.y; yIndex < f->m_Index.y + 2; yIndex += 1) {
                        for(int xIndex = f->m_Index.x; xIndex < f->m_Index.x + 2; xIndex += 1) {
                            glm::vec2 startCoords = Fractal::GetCoordsForTarget({xIndex, yIndex}, 1, f->m_Index, 2, m_Dimensions, {x(), y()});
                            FractalData data = {1, {xIndex, yIndex}};
                            Fractal* newFractal = new Fractal(CalculateSpriteData(data, m_Index), data, startCoords, {this, &Puzzle::Transform});
                            m_Fractals.push_back(newFractal);
                            splitFractals.push_back(newFractal);
                        }
                    }

                    m_Fractals.erase(GetFractalIterator(f));
                    delete f;
                    break;
                case 4:
                    //split f into quarters of size 2, and possibly splitting those 2x2s into 1x1s (some or all of the 2x2s)
                    for(int yIndex = f->m_Index.y; yIndex < f->m_Index.y + 4; yIndex += 2) {
                        for(int xIndex = f->m_Index.x; xIndex < f->m_Index.x + 4; xIndex += 2) {
                            //check if the 2x2 needs further splitting using dataList
                            bool splitAgain = false;
                            for(FractalData fData: dataList) {
                                if(Fractal::FindOverlapType({2, {xIndex, yIndex}}, fData) == OverlapType::Partial) {
                                    splitAgain = true;
                                    break;
                                }
                            }

                            if(!splitAgain) {
                                glm::vec2 startCoords = Fractal::GetCoordsForTarget({xIndex, yIndex}, 2, f->m_Index, 4, m_Dimensions, {x(), y()});
                                FractalData data = {2, {xIndex, yIndex}};
                                Fractal* newFractal = new Fractal(CalculateSpriteData(data, m_Index), data, startCoords, {this, &Puzzle::Transform});
                                m_Fractals.push_back(newFractal);
                                splitFractals.push_back(newFractal);
                            }else{
                                for(int yIndexSub = yIndex; yIndexSub < yIndex + 2; ++yIndexSub) {
                                    for(int xIndexSub = xIndex; xIndexSub < xIndex + 2; ++xIndexSub) {
                                        glm::vec2 startCoords = Fractal::GetCoordsForTarget({xIndexSub, yIndexSub}, 1, f->m_Index, 4, m_Dimensions, {x(), y()});
                                        FractalData data = {1, {xIndexSub, yIndexSub}};
                                        Fractal* newFractal = new Fractal(CalculateSpriteData(data, m_Index), data, startCoords, {this, &Puzzle::Transform});
                                        m_Fractals.push_back(newFractal);
                                        splitFractals.push_back(newFractal);
                                    }
                                }
                            }
                        }
                    }

                    m_Fractals.erase(GetFractalIterator(f));
                    delete f;
                    break;
                default:
                    assert(false);
                    break; 
            }
        }

        return splitFractals;
    }


    std::vector<Fractal*>::iterator Puzzle::GetFractalIterator(Fractal* fractal) {
        std::vector<Fractal*>::iterator iter = m_Fractals.end();

        for(std::vector<Fractal*>::iterator i = m_Fractals.begin(); i < m_Fractals.end(); ++i) {
            if(&(*(*i)) == &(*fractal)) iter = i;
        }

        return iter;
    }


    Fractal* Puzzle::GetClosestFractal(float mousex, float mousey) const {
        Fractal* closestF = nullptr;

        for(Fractal* f: m_Fractals) {
            if(!closestF) {
                closestF = f;
            }else{
                float closestDis = rose::PointDistance(glm::vec2(closestF->x(), closestF->y()), glm::vec2(mousex, mousey));
                float thisDis = rose::PointDistance(glm::vec2(f->x(), f->y()), glm::vec2(mousex, mousey));
                if(thisDis < closestDis) closestF = f;
            }
        }

        return closestF;
    }

    //find the four fractals surrounding the give mouse position (used to merge into a larger fractal of twice the size)
    //one or all of the four will be a nullptr if sizes are mismatched or are anchor fractals
    FractalCorners Puzzle::FindFractalCorners(float mousex, float mousey) const {
        FractalCorners fc;

        Fractal* closestF = GetClosestFractal(mousex, mousey);

        if(!closestF) return fc;


        //determine which corner that fractal belongs to
        glm::ivec2 index = closestF->m_Index;
        int size = closestF->m_Size;
        Fractal* topLeft = nullptr;
        Fractal* topRight = nullptr;
        Fractal* bottomLeft = nullptr;
        Fractal* bottomRight = nullptr;

        if(closestF->x() - mousex < 0) { //closest fractal is on left of mouse
            if(closestF->y() - mousey < 0) { //closest fractal is below mouse
                bottomLeft = closestF;
                bottomRight = GetFractalWithIndex(glm::ivec2(index.x + size, index.y));
                topLeft = GetFractalWithIndex(glm::ivec2(index.x, index.y - size));
                topRight = GetFractalWithIndex(glm::ivec2(index.x + size, index.y - size));
            }else{
                topLeft = closestF;
                topRight = GetFractalWithIndex(glm::ivec2(index.x + size, index.y));
                bottomLeft = GetFractalWithIndex(glm::ivec2(index.x, index.y + size));
                bottomRight = GetFractalWithIndex(glm::ivec2(index.x + size, index.y + size));
            }
        }else{ //closest fractal is on right side of mouse
            if(closestF->y() - mousey < 0) { //closest fractal is below mouse
                bottomRight = closestF;
                bottomLeft = GetFractalWithIndex(glm::ivec2(index.x - size, index.y));
                topLeft = GetFractalWithIndex(glm::ivec2(index.x - size, index.y - size));
                topRight = GetFractalWithIndex(glm::ivec2(index.x, index.y - size));
            }else{
                topRight = closestF;
                topLeft = GetFractalWithIndex(glm::ivec2(index.x - size, index.y));
                bottomLeft = GetFractalWithIndex(glm::ivec2(index.x - size, index.y + size));
                bottomRight = GetFractalWithIndex(glm::ivec2(index.x, index.y + size));
            }
        }

        if(topLeft && topLeft->m_Size == size && !IsAnchor(topLeft)) fc.TopLeft = topLeft;
        if(topRight && topRight->m_Size == size && !IsAnchor(topRight)) fc.TopRight = topRight;
        if(bottomLeft && bottomLeft->m_Size == size && !IsAnchor(bottomLeft)) fc.BottomLeft = bottomLeft;
        if(bottomRight && bottomRight->m_Size == size && !IsAnchor(bottomRight)) fc.BottomRight = bottomRight;

        return fc;
    }



    void Puzzle::MergeFractals(std::vector<Fractal*> fractals) {
        assert(!fractals.empty());
        //also find index and size while iterating through fractals
        glm::ivec2 smallestIndex = { 100, 100};
        glm::ivec2 largestIndex = {-1, -1}; //this value is exclusive

        for(Fractal* f: fractals) {
            f->SetAnimationEndEvent(AnimationEndEvent::Destroy);

            //find size and index of final merged fractal
            if(largestIndex.x == -1 || largestIndex.y == -1) {
                smallestIndex = f->m_Index;
                largestIndex = {f->m_Index.x + f->m_Size, f->m_Index.y + f->m_Size};
            }else{
                if(f->m_Index.x < smallestIndex.x) smallestIndex.x = f->m_Index.x;
                if(f->m_Index.y < smallestIndex.y) smallestIndex.y = f->m_Index.y;
                if(f->m_Index.x + f->m_Size > largestIndex.x) largestIndex.x = f->m_Index.x + f->m_Size;
                if(f->m_Index.y + f->m_Size > largestIndex.y) largestIndex.y = f->m_Index.y + f->m_Size;
            }
        }

        int xSize = largestIndex.x - smallestIndex.x;
        int ySize = largestIndex.y - smallestIndex.y;
        assert(xSize == ySize);

        m_MergeList.push_back({xSize, smallestIndex}); //xSize == ySize
    }


    void Puzzle::Transform(FractalData data, TransformationType type) {
        Fractal* fractalA = GetFractalWithIndex(data.index);
        Fractal* fractalB;
        switch(type) {
            case TransformationType::TranslatePosX:
                fractalB = GetFractalWithIndex({data.index.x + data.size, data.index.y});
                SwapFractals(fractalA, fractalB);
                break;
            case TransformationType::TranslateNegX:
                fractalB = GetFractalWithIndex({data.index.x - data.size, data.index.y});
                SwapFractals(fractalA, fractalB);
                break;
            case TransformationType::TranslatePosY:
                fractalB = GetFractalWithIndex({data.index.x, data.index.y + data.size});
                SwapFractals(fractalA, fractalB);
                break;
            case TransformationType::TranslateNegY:
                fractalB = GetFractalWithIndex({data.index.x, data.index.y - data.size});
                SwapFractals(fractalA, fractalB);
                break;
            case TransformationType::RotateCW:
                RotateFractalCW(fractalA);
                break;
            case TransformationType::RotateCCW:
                RotateFractalCCW(fractalA);
                break;
            case TransformationType::ReflectX:
                ReflectFractalX(fractalA);
                break;
            case TransformationType::ReflectY:
                ReflectFractalY(fractalA);
                break;
            default:
                assert(false); //should always be a valid transformation
                break;
        }

    }

    //@todo: split into TranslateLeft, TranslateDown, etc
    void Puzzle::SwapFractals(Fractal* fractalA, Fractal* fractalB) {

        fractalA->MoveTo({fractalB->x(), fractalB->y()});
        fractalB->MoveTo({fractalA->x(), fractalA->y()});

        fractalA->SetAnimationEndEvent(AnimationEndEvent::Recreate);
        fractalB->SetAnimationEndEvent(AnimationEndEvent::Recreate);



        //how to transform A to get back to original position
        glm::ivec2 newIndexB = fractalA->m_Index;
        glm::ivec2 newIndexA = fractalB->m_Index;

        if(newIndexA.y == newIndexB.y) { //horizontal transformation
            if(newIndexA.x < newIndexB.x) PushTransformation({TransformationType::TranslatePosX, {fractalB->m_Size, fractalB->m_Index}});
            else PushTransformation({TransformationType::TranslateNegX, {fractalB->m_Size, fractalB->m_Index}});
        }else{ //vertical transformation
            if(newIndexA.y < newIndexB.y) PushTransformation({TransformationType::TranslatePosY, {fractalB->m_Size, fractalB->m_Index}});
            else PushTransformation({TransformationType::TranslateNegY, {fractalB->m_Size, fractalB->m_Index}});
        }


        //////////////////////////write new element data after swapping///////////////////////////////
        std::vector<FractalElement> elementsA = GetElements({fractalA->m_Size, fractalA->m_Index});

        int xShift = fractalB->m_Index.x - fractalA->m_Index.x;
        int yShift = fractalB->m_Index.y - fractalA->m_Index.y;

        for(int row = fractalA->m_Index.y; row < fractalA->m_Index.y + fractalA->m_Size; ++row) {
            for(int col = fractalA->m_Index.x; col < fractalA->m_Index.x + fractalA->m_Size; ++col) {
                //replace value in fractal A with one in B
                SetElementAt(col, row, GetElementAt(col + xShift, row + yShift));
                //replace value in fractal B with cached A value in elementsA
                SetElementAt(col + xShift, row + yShift, elementsA.at((row - fractalA->m_Index.y) * fractalA->m_Size + (col - fractalA->m_Index.x)));
            }
        }

        //update texture data based on updated element data
        UpdateTextureData({fractalA->m_Size, fractalA->m_Index});
        UpdateTextureData({fractalB->m_Size, fractalB->m_Index});

    }


    void Puzzle::RotateFractalCW(Fractal* fractal) {
        fractal->RotateBy(-1.5708);
        fractal->SetAnimationEndEvent(AnimationEndEvent::Recreate);
        PushTransformation({TransformationType::RotateCCW, {fractal->m_Size, fractal->m_Index}});

        std::vector<FractalElement> elements = GetElements({fractal->m_Size, fractal->m_Index});

        //to rotate ccw - swap col/row and flip over y (eg, reverse iteration over cols)
        for(int row = 0; row < fractal->m_Size; ++row) {
            for(int col = 0; col < fractal->m_Size; ++col) {
                int newCol = row;
                int newRow = fractal->m_Size - 1 - col;
                SetElementAt(fractal->m_Index.x + col, fractal->m_Index.y + row, elements.at(newRow * fractal->m_Size + newCol));
            }
        }

        UpdateTextureData({fractal->m_Size, fractal->m_Index});

    }


    void Puzzle::RotateFractalCCW(Fractal* fractal) {
        fractal->RotateBy(1.5708);
        fractal->SetAnimationEndEvent(AnimationEndEvent::Recreate);
        PushTransformation({TransformationType::RotateCW, {fractal->m_Size, fractal->m_Index}});

        std::vector<FractalElement> elements = GetElements({fractal->m_Size, fractal->m_Index});

        //to rotate ccw - swap col/row and flip over x (eg, reverse iteration over rows)
        for(int row = 0; row < fractal->m_Size; ++row) {
            for(int col = 0; col < fractal->m_Size; ++col) {
                int newCol = fractal->m_Size - 1 - row;
                int newRow = col;
                SetElementAt(fractal->m_Index.x + col, fractal->m_Index.y + row, elements.at(newRow * fractal->m_Size + newCol));
            }
        }

        UpdateTextureData({fractal->m_Size, fractal->m_Index});

    }


    void Puzzle::ReflectFractalX(Fractal* fractal) {
        fractal->ScaleTo({1.0f, -1.0f});
        fractal->SetAnimationEndEvent(AnimationEndEvent::Recreate);
        PushTransformation({TransformationType::ReflectX, {fractal->m_Size, fractal->m_Index}});

        std::vector<FractalElement> elements = GetElements({fractal->m_Size, fractal->m_Index});

        //flips rows
        for(int row = 0; row < fractal->m_Size; ++row) {
            for(int col = 0; col < fractal->m_Size; ++col) {
                int newCol = col;
                int newRow = fractal->m_Size - 1 - row;
                SetElementAt(fractal->m_Index.x + col, fractal->m_Index.y + row, elements.at(newRow * fractal->m_Size + newCol));
            }
        }

        UpdateTextureData({fractal->m_Size, fractal->m_Index});
    }


    void Puzzle::ReflectFractalY(Fractal* fractal) {
        fractal->ScaleTo({-1.0f, 1.0f});
        fractal->SetAnimationEndEvent(AnimationEndEvent::Recreate);
        PushTransformation({TransformationType::ReflectY, {fractal->m_Size, fractal->m_Index}});

        std::vector<FractalElement> elements = GetElements({fractal->m_Size, fractal->m_Index});

        //flips cols
        for(int row = 0; row < fractal->m_Size; ++row) {
            for(int col = 0; col < fractal->m_Size; ++col) {
                int newCol = fractal->m_Size - 1 - col;
                int newRow = row;
                SetElementAt(fractal->m_Index.x + col, fractal->m_Index.y + row, elements.at(newRow * fractal->m_Size + newCol));
            }
        }

        UpdateTextureData({fractal->m_Size, fractal->m_Index});

    }


    void Puzzle::UndoLastTransformation() {

        struct TransformationData tData = PeekTransformation();

        Fractal* f = GetFractalWithIndex(tData.fractalData.index);

        assert(tData.fractalData.size == f->m_Size);

        Transform(tData.fractalData, tData.transformation);

        //the undo transformation pushes another transformation onto the transformation stack, 
        //so pop_back is called twice - the first undo removes the transoformation pushed onto the 
        //stack in the switch statement above, and the second is the actual transformation we want to undo
        PopTransformation();
        PopTransformation();
    }


    std::vector<FractalElement> Puzzle::GetElements(FractalData data) const {
        std::vector<FractalElement> elements;
        for(int row = data.index.y; row < data.index.y + data.size; ++row) {
            for(int col = data.index.x; col < data.index.x + data.size; ++col) {
                elements.push_back(GetElementAt(col, row));
            }
        }
        return elements;
    }


    bool Puzzle::IsAnchor(Fractal* fractal) const {
        std::vector<FractalElement> elements = GetElements({fractal->m_Size, fractal->m_Index});
        for(FractalElement e: elements) {
            switch(e) {
                case 'R': return true; break; 
                case 'G': return true; break; 
                case 'B': return true; break; 
                default: break;
            }
        }

        return false;
    }


    //pushes texture data to m_TextureMap.  The actual updating of texture on GPU side is called in Puzzle::OnAnimationEnd()
    void Puzzle::UpdateTextureData(FractalData data) {
        //textures start from bottom left, but starting textures from top left to fit fractal order (left to right, top to bottom)
        glm::ivec2 texStart =  {data.index.x * Fractal::s_UnitSize + m_Index * 256, 256 - Fractal::s_UnitSize * (data.index.y + 1)};

        for(int row = 0; row < data.size; ++row) {  //when data.size is > 1, problems occur.  Why???
            for(int col = 0; col < data.size; ++col) {
                glm::vec2 subFractalTexCoords = {texStart.x + col * Fractal::s_UnitSize + 1, texStart.y - row * Fractal::s_UnitSize + 1}; //smaller than frame 
                glm::vec2 colorTexSize = {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2};
                //pushing on fractal frame
                m_TextureMap.push_back({{texStart.x + col * Fractal::s_UnitSize, texStart.y - row * Fractal::s_UnitSize}, 
                                       {0, 0}, {Fractal::s_UnitSize, Fractal::s_UnitSize}}); 

                //elements are drawn inside fractal frame, so start point is offset by 1 and side length is reduced by 2 in each dimension
                switch(GetElementAt(data.index.x + col, data.index.y + row)) {
                    case 'r':
                        m_TextureMap.push_back({subFractalTexCoords, {0 + 1, Fractal::s_UnitSize + 1}, colorTexSize});
                        break;
                    case 'b':
                        m_TextureMap.push_back({subFractalTexCoords, {0 + 1, Fractal::s_UnitSize * 2 + 1}, colorTexSize});
                        break;
                    case 'g':
                        m_TextureMap.push_back({subFractalTexCoords, {0 + 1, Fractal::s_UnitSize * 3 + 1}, colorTexSize});
                        break;
                    case 'R':
                        m_TextureMap.push_back({subFractalTexCoords, {Fractal::s_UnitSize + 1, Fractal::s_UnitSize + 1}, colorTexSize});
                        break;
                    case 'B':
                        m_TextureMap.push_back({subFractalTexCoords, {Fractal::s_UnitSize + 1, Fractal::s_UnitSize * 2 + 1}, colorTexSize});
                        break;
                    case 'G':
                        m_TextureMap.push_back({subFractalTexCoords, {Fractal::s_UnitSize + 1, Fractal::s_UnitSize * 3 + 1}, colorTexSize});
                        break;
                    case 'e':
                        //leave central area empty
                        break;
                    default:
                        break;
                }
            }
        }

    }


    rose::Sprite Puzzle::CalculateSpriteData(FractalData data, int puzzleIndex) {
        glm::vec2 start = glm::ivec2(data.index.x * Fractal::s_UnitSize + puzzleIndex * 256, 256 - Fractal::s_UnitSize * (data.index.y + data.size));
        glm::vec2 size = {Fractal::s_UnitSize * data.size , Fractal::s_UnitSize * data.size};
        return { start, size, rose::TextureType::Custom };
    }


    bool Puzzle::IsCleared() const {
        return ColorsConnected('R') && ColorsConnected('G') && ColorsConnected('B');
    }

    //perform a depth-first search on the given color to check connectedness
    //anchor color is uppercase, and will check if all upcase chars are connected by lowercase chars of the same letter
    bool Puzzle::ColorsConnected(char anchorColor) const {
        std::vector<glm::ivec2> anchorIndices;
        std::vector<DFSMark> visited {false};
        for(int row = 0; row < m_Dimensions.y; ++row) {
            for(int col = 0; col < m_Dimensions.x; ++col) {
                FractalElement e = GetElementAt(col, row);
                if(e == anchorColor) {
                    anchorIndices.push_back({col, row});
                }
                if(e == anchorColor || e == (char)tolower(anchorColor)) {
                    visited.push_back(DFSMark::Unvisited);
                }else{
                    visited.push_back(DFSMark::Disconnected);
                }
            }
        }

        if(anchorIndices.empty()) return true; 


        glm::ivec2 startIndex = anchorIndices.at(0);

        DFS(startIndex, visited);

        for(glm::ivec2 index: anchorIndices) {
            if(visited.at(index.y * m_Dimensions.x + index.x) == DFSMark::Unvisited) return false;
        }

        return true;
    }


    //check if index is inside puzzle dimensions
    bool Puzzle::InsideGrid(glm::ivec2 index) const {
        if(index.x < 0) return false;
        if(index.y < 0) return false;
        if(index.x >= m_Dimensions.x) return false;
        if(index.y >= m_Dimensions.y) return false;
        return true;
    }

    void Puzzle::DFS(const glm::ivec2 index, std::vector<DFSMark>& visited) const {
        visited.at(index.y * m_Dimensions.x + index.x) = DFSMark::Visited;

        if(InsideGrid({index.x + 1, index.y}) && visited.at(index.y * m_Dimensions.x + index.x + 1) == DFSMark::Unvisited) DFS({index.x + 1, index.y}, visited);
        if(InsideGrid({index.x - 1, index.y}) && visited.at(index.y * m_Dimensions.x + index.x - 1) == DFSMark::Unvisited) DFS({index.x - 1, index.y}, visited);
        if(InsideGrid({index.x, index.y + 1}) && visited.at((index.y + 1) * m_Dimensions.x + index.x) == DFSMark::Unvisited) DFS({index.x, index.y + 1}, visited);
        if(InsideGrid({index.x, index.y - 1}) && visited.at((index.y - 1) * m_Dimensions.x + index.x) == DFSMark::Unvisited) DFS({index.x, index.y - 1}, visited);
    }


}
