
#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {


    Puzzle::Puzzle(int index, int setIndex): Entity(s_Sprite, s_ObjectSize, s_BoundingBox, glm::vec2(s_InitOffset + s_Spacing * index, 0.0f)),
                                             m_Index(index), m_SetIndex(setIndex) {
            m_Dimensions = g_Data.at(setIndex).puzzlesData.at(index).dimensions;
            m_MaxTransformations = 5; //temp: should load this value in from default puzzle data

            //undo icons 
            for(int i = 0; i < m_MaxTransformations; ++i) {
                m_UndoIcons.emplace_back(new UndoIcon(glm::vec2(UndoIcon::s_Margin * ((1 - m_MaxTransformations) / 2.0f + i) + x(), -110.0f), this));
            }

            std::vector<FractalElement> elements = g_Data.at(setIndex).puzzlesData.at(index).elements;

            //make a bunch of 1x1 fractals as starting point for all puzzles
            for(int row = 0; row < m_Dimensions.y; ++row) {
                for(int col = 0; col < m_Dimensions.x; ++col) {
                    FractalElement element = elements.at(row * m_Dimensions.x + col);
                    if(element != 'E') {
                        glm::vec2 startCoords = Fractal::GetCoords({col, row}, 1, m_Dimensions, glm::vec2(x(), y()));
                        m_Fractals.emplace_back(new Fractal({element}, {col, row}, glm::vec2(startCoords.x, startCoords.y), GetIndex()));
                    }
                }
            }
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

        for(Fractal* f: m_Fractals) {
            if(f) f->OnAnimationEnd();
        }
        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->OnAnimationEnd();
        }

        for(std::vector<Fractal*> mergeList: m_MergeLists) {
            Fractal* newF = CreateFromMergeList(mergeList);

            for(Fractal* f: mergeList) {
                std::vector<Fractal*>::iterator fIter = GetFractalIterator(f);
                m_Fractals.erase(fIter);
                delete f;
            }
            mergeList.clear();
        }

        m_MergeLists.clear();

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
            if(f->GetIndex() == index) return f;
        }
        return nullptr;
    }

    std::vector<Fractal*> Puzzle::GetOverlappingFractals(const FractalData& fractalData) {
        std::vector<Fractal*> overlappingFractals;
        for(Fractal* f: m_Fractals) {
            bool contains = false;
            for(int row = fractalData.index.y; row < fractalData.index.y + fractalData.size; ++row) {
                for(int col = fractalData.index.x; col < fractalData.index.x + fractalData.size; ++col) {
                    if(f->Contains({col, row})) contains = true;
                    if(contains) break;
                }
                if(contains) break;
            }
            if(contains) overlappingFractals.push_back(f);
        }

        return overlappingFractals;
    }

    std::vector<Fractal*> Puzzle::SplitFractal(Fractal* fractal, const std::vector<FractalData>& fractalData) {

        std::vector<Fractal*> newFractals;

        int targetSize = fractal->GetSize();
        glm::ivec2 targetIndex = fractal->GetIndex();

        /*
           for(int row = 0; row < 2; ++row) {
           for(int col = 0; col < 2; ++col) {
           std::cout << (int)fractal->GetSubElement({col, row}) << std::endl;
           }
           }*/

        for(FractalData data: fractalData) {
            //find start coordinates from fractaldata
            glm::vec2 startCoords = Fractal::GetCoordsForTarget(data.index, data.size, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
            //create fractal at the calculated coordinates
            //with given elements
            Fractal* newFractal; 

            switch(data.size) {
                case 1: {
                            std::vector<FractalElement> elements;
                            elements.push_back(fractal->GetSubElement({data.index.x - targetIndex.x, data.index.y - targetIndex.y}));
                            newFractal = new Fractal(elements, data.index, startCoords, GetIndex());
                            break;
                        }
                case 2: {
                            std::vector<FractalElement> elements;

                            for(int row = 0; row < 2; ++row) {
                                for(int col = 0; col < 2; ++col) {
                                    elements.push_back(fractal->GetSubElement({data.index.x + col - targetIndex.x, data.index.y + row - targetIndex.y}));
                                }
                            }

                            newFractal = new Fractal(elements, data.index, startCoords, GetIndex());

                            break;
                        }
                case 4: {
                            std::vector<FractalElement> elements;
                            for(int row = 0; row < 4; ++row) {
                                for(int col = 0; col < 4; ++col) {
                                    elements.push_back(fractal->GetSubElement({data.index.x + col - targetIndex.x, data.index.y + row - targetIndex.y}));
                                }
                            }

                            newFractal = new Fractal(elements, data.index, startCoords, GetIndex());

                            break;
                        }
                default:
                        assert(false);
                        break;
            }

            m_Fractals.push_back(newFractal);
            newFractals.push_back(newFractal);
        }

        std::vector<Fractal*>::iterator iter = GetFractalIterator(fractal);
        m_Fractals.erase(iter);
        delete fractal; 

        return newFractals;

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

    FractalCorners Puzzle::FindFractalCorners(float mousex, float mousey) const {
        FractalCorners fc;

        Fractal* closestF = GetClosestFractal(mousex, mousey);

        if(!closestF) return fc;


        //determine which corner that fractal belongs to
        glm::ivec2 index = closestF->GetIndex();
        int size = closestF->GetSize();
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

        if(topLeft && topLeft->GetSize() == size) fc.TopLeft = topLeft;
        if(topRight && topRight->GetSize() == size) fc.TopRight = topRight;
        if(bottomLeft && bottomLeft->GetSize() == size) fc.BottomLeft = bottomLeft;
        if(bottomRight && bottomRight->GetSize() == size) fc.BottomRight = bottomRight;

        return fc;
    }


    void Puzzle::MergeFractals(std::vector<Fractal*> mergeList) {
        m_MergeLists.push_back(mergeList);
    }

    Fractal* Puzzle::CreateFromMergeList(const std::vector<Fractal*>& mergeList) {
        //temp: checking out what fractals in her
        //find index and size of merged fractal from subfractals in mergeList
        int smallestCol = 100; //100 is larger than any fractal will ever be
        int smallestRow = 100; //"
        int largestCol = 0; //== largestRow since only using square fractals

        for(Fractal* f: mergeList) {
            glm::ivec2 index = f->GetIndex();
            int size = f->GetSize();
            if(index.x < smallestCol) smallestCol = index.x;
            if(index.y < smallestRow) smallestRow = index.y;
            if((index.x + size - 1) > largestCol) largestCol = index.x + size - 1;

        }

        int size = largestCol - smallestCol + 1;
        glm::ivec2 index = {smallestCol, smallestRow};
        glm::vec2 coords = Fractal::GetCoords(index, size, GetDimensions(), glm::vec2(x(), y()));

        //a size 1 fractal should never be formed, but putting it here for completeness
        switch(size) {
            case 1: {
                        std::vector<FractalElement> elements;
                        elements.push_back(mergeList.front()->GetSubElement({0,0}));
                        m_Fractals.emplace_back(new Fractal(elements, index, coords, GetIndex()));
                        return m_Fractals.back();
                    }
            case 2: {
                        std::vector<FractalElement> elements;
                        for(int row = 0; row < 2; ++row) {
                            for(int col = 0; col < 2; ++col) {
                                Fractal* subF = nullptr;
                                for(Fractal* f: mergeList) {
                                    if(f->Contains({index.x + col, index.y + row})) {
                                        subF = f;
                                        break;
                                    }
                                }

                                //get the individual elements
                                glm::ivec2 subIndex = subF->GetIndex();
                                elements.push_back(subF->GetSubElement({index.x + col - subIndex.x, index.y + row - subIndex.y}));
                            }
                        }

                        m_Fractals.emplace_back(new Fractal(elements, index, coords, GetIndex()));
                        return m_Fractals.back();
                    }
            case 4: {
                        std::vector<FractalElement> elements;
                        for(int row = 0; row < 4; ++row) {
                            for(int col = 0; col < 4; ++col) {
                                Fractal* subF;
                                for(Fractal* f: mergeList) {
                                    //check if f contains index {col, row}
                                    if(f->Contains({index.x + col, index.y + row})) {
                                        subF = f;
                                        break;
                                    }
                                }

                                //get the individual elements
                                glm::ivec2 subIndex = subF->GetIndex();
                                elements.push_back(subF->GetSubElement({index.x + col - subIndex.x, index.y + row - subIndex.y}));
                            }
                        }
                        m_Fractals.emplace_back(new Fractal(elements, index, coords, GetIndex()));
                        return m_Fractals.back();
                    }
            default: {
                         assert(false);
                         return nullptr;
                         break;
                     }
        }
    }



    void Puzzle::SwapFractals(Fractal* fractalA, Fractal* fractalB) {
        glm::ivec2 indexA = fractalA->GetIndex();
        fractalA->SetIndex(fractalB->GetIndex());
        fractalB->SetIndex(indexA);

        fractalA->MoveTo({fractalB->x(), fractalB->y()});
        fractalB->MoveTo({fractalA->x(), fractalA->y()});

        //how to transform A to get back to original position
        glm::ivec2 newIndexA = fractalA->GetIndex();
        glm::ivec2 newIndexB = fractalB->GetIndex();

        if(newIndexA.y == newIndexB.y) { //horizontal transformation
            if(newIndexA.x < newIndexB.x) m_TransformationStack.push_back({TransformationType::TranslatePosX, fractalA->GetIndex(), fractalA->GetSize()});
            else m_TransformationStack.push_back({TransformationType::TranslateNegX, fractalA->GetIndex(), fractalA->GetSize()});
        }else{ //vertical transformation
            if(newIndexA.y < newIndexB.y) m_TransformationStack.push_back({TransformationType::TranslatePosY, fractalA->GetIndex(), fractalA->GetSize()});
            else m_TransformationStack.push_back({TransformationType::TranslateNegY, fractalA->GetIndex(), fractalA->GetSize()});
        }

        fractalA->WriteData(g_Data, GetSetIndex(), GetIndex());
        fractalB->WriteData(g_Data, GetSetIndex(), GetIndex());

    }


    void Puzzle::RotateFractalCW(Fractal* fractal) {
        fractal->RotateBy(-1.5708);
        m_TransformationStack.push_back({TransformationType::RotateCCW, fractal->GetIndex(), fractal->GetSize()});

        fractal->WriteData(g_Data, GetSetIndex(), GetIndex());
    }

    void Puzzle::RotateFractalCCW(Fractal* fractal) {
        fractal->RotateBy(1.5708);
        m_TransformationStack.push_back({TransformationType::RotateCW, fractal->GetIndex(), fractal->GetSize()});

        fractal->WriteData(g_Data, GetSetIndex(), GetIndex());
    }

    void Puzzle::ReflectFractalX(Fractal* fractal) {
        fractal->ScaleTo({1.0f, -1.0f});
        m_TransformationStack.push_back({TransformationType::ReflectX, fractal->GetIndex(), fractal->GetSize()});

        fractal->WriteData(g_Data, GetSetIndex(), GetIndex());
    }

    void Puzzle::ReflectFractalY(Fractal* fractal) {
        fractal->ScaleTo({-1.0f, 1.0f});
        m_TransformationStack.push_back({TransformationType::ReflectY, fractal->GetIndex(), fractal->GetSize()});

        fractal->WriteData(g_Data, GetSetIndex(), GetIndex());
    }


    void Puzzle::UndoLastTransformation() {

        struct TransformationData tData = m_TransformationStack.back();

        Fractal* f = GetFractalWithIndex(tData.fractalIndex);

        //temp: should only undo transformation after making sure fractal sizes are matching/correct
        if(tData.fractalSize != f->GetSize()) return;

        Fractal* otherF;
        glm::ivec2 index = f->GetIndex();

        switch(tData.transformation) {
            case TransformationType::TranslatePosX:
                otherF = GetFractalWithIndex(glm::ivec2(index.x + f->GetSize(), index.y)); 
                if(otherF && f->GetSize() == otherF->GetSize()) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslateNegX:
                otherF = GetFractalWithIndex(glm::ivec2(index.x - f->GetSize(), index.y)); 
                if(otherF && f->GetSize() == otherF->GetSize()) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslatePosY:
                otherF = GetFractalWithIndex(glm::ivec2(index.x, index.y + f->GetSize())); 
                if(otherF && f->GetSize() == otherF->GetSize()) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslateNegY:
                otherF = GetFractalWithIndex(glm::ivec2(index.x, index.y - f->GetSize())); 
                if(otherF && f->GetSize() == otherF->GetSize()) SwapFractals(f, otherF);
                break;
            case TransformationType::RotateCW:
                RotateFractalCW(f);
                break;
            case TransformationType::RotateCCW:
                RotateFractalCCW(f);
                break;
            case TransformationType::ReflectX: 
                ReflectFractalX(f);
                break;
            case TransformationType::ReflectY:
                ReflectFractalY(f);
                break;
        }

        //the undo transformation pushes another transformation onto the transformation stack, 
        //so pop_back is called twice - the first undo removes the transoformation pushed onto the 
        //stack in the switch statement above, and the second is the actual transformation we want to undo
        m_TransformationStack.pop_back();
        m_TransformationStack.pop_back();
    }


}
