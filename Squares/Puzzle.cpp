//#include <algorithm>

#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {



    //Puzzle::Puzzle(FractalElement* elements, const glm::ivec2& dimensions, const glm::vec2& pos, int index) 
    Puzzle::Puzzle(FractalElement* elements, const glm::ivec2& dimensions, int index) 
        : Entity(GetSprite(), GetObjectSize(), GetBoundingBox(), glm::vec2(GetInitOffset() + GetSpacing() * index, 0.0f)) {
            m_Index = index;
            m_Dimensions = dimensions;
            m_MaxTransformations = 5; //temp: should load this value in from default puzzle data

            //undo icons 
            for(int i = 0; i < m_MaxTransformations; ++i) {
                m_UndoIcons.emplace_back(new UndoIcon(glm::vec2(UndoIcon::s_Margin * ((1 - m_MaxTransformations) / 2.0f + i) + x(), -110.0f), this));
            }

            //make a bunch of 1x1 fractals as starting point for all puzzles
            for(int row = 0; row < dimensions.y; ++row) {
                for(int col = 0; col < dimensions.x; ++col) {
                    FractalElement element = elements[row * dimensions.x + col];
                    if(element != FractalElement::Block) {
                        glm::vec2 startCoords = BaseFractal::GetCoords({col, row}, 1, m_Dimensions, glm::vec2(x(), y()));
                        m_Fractals.emplace_back(new Fractal<int>((int)element, {col, row}, glm::vec2(startCoords.x, startCoords.y), GetIndex()));
                    }
                }
            }
        }


    Puzzle::~Puzzle() {
        for(BaseFractal* f: m_Fractals) {
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

        for(BaseFractal* f: m_Fractals) {
            if(f) f->MoveBy(glm::vec2(deltaX, deltaY));
        }

        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->MoveBy(glm::vec2(deltaX, deltaY));
        }
    }

    void Puzzle::MoveBy(const glm::vec2& shift) {
        Entity::MoveBy(shift);
        for(BaseFractal* f: m_Fractals) {
            if(f) f->MoveBy(shift);
        }

        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->MoveBy(shift);
        }
    }


    void Puzzle::OnAnimationEnd() {
        Entity::OnAnimationEnd();
        for(BaseFractal* f: m_Fractals) {
            if(f) f->OnAnimationEnd();
        }
        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->OnAnimationEnd();
        }

        for(std::vector<BaseFractal*> mergeList: m_MergeLists) {
            std::cout << mergeList.size() << std::endl;
            CreateFromMergeList(mergeList);
            for(BaseFractal* f: mergeList) {
                std::vector<BaseFractal*>::iterator fIter = GetFractalIterator(f);
                m_Fractals.erase(fIter);
                delete f;
            }
            mergeList.clear();
        }

        m_MergeLists.clear();
    }

    void Puzzle::OnAnimationUpdate(float t) {
        Entity::OnAnimationUpdate(t);
        for(BaseFractal* f: m_Fractals) {
            if(f) f->OnAnimationUpdate(t);
        }
        for(UndoIcon* i: m_UndoIcons) {
            if(i) i->OnAnimationUpdate(t);
        }
    }

    void Puzzle::Draw() const {
        //    Entity::Draw(); //puzzle doesn't need/have a visual element
        for(BaseFractal* f: m_Fractals) {
            if(f) f->Draw();
        }
        for(int i = 0; i < GetTransformationCount(); ++i) {
            UndoIcon* icon = m_UndoIcons.at(i);
            if(icon) icon->Draw();
        }
    }

    BaseFractal* Puzzle::GetFractal(const glm::ivec2& index) const {
        for(BaseFractal* f: m_Fractals) {
            if(f->GetIndex() == index) {
                if(Fractal<int>* fractal = dynamic_cast<Fractal<int>*>(f)) {
                    if(fractal->Contains(index)) return f;
                }
                if(Fractal<glm::imat2>* fractal = dynamic_cast<Fractal<glm::imat2>*>(f)) {
                    if(fractal->Contains(index)) return f;
                }
                if(Fractal<glm::imat4>* fractal = dynamic_cast<Fractal<glm::imat4>*>(f)) {
                    if(fractal->Contains(index)) return f;
                }
            }
        }
        return nullptr;
    }


    std::vector<BaseFractal*> Puzzle::SplitFractal(BaseFractal* fractal, const std::vector<FractalData>& fractalData) {

        std::vector<BaseFractal*> newFractals;

        int targetSize = GetFractalSize(fractal);
        glm::ivec2 targetIndex = fractal->GetIndex();

        for(FractalData data: fractalData) {
            //find start coordinates from fractaldata
            glm::vec2 startCoords = BaseFractal::GetCoordsForTarget(data.index, data.size, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
            //create fractal at the calculated coordinates
            //with given elements
            BaseFractal* newFractal; 

            switch(data.size) {
                case 1: {
                            int mat = GetSubElementsI(fractal, {data.index.x - targetIndex.x, data.index.y - targetIndex.y});
                            newFractal = new Fractal<int>(mat, data.index, startCoords, GetIndex());
                            break;
                        }
                case 2: {
                            int arr[4];

                            for(int row = 0; row < 2; ++row) {
                                for(int col = 0; col < 2; ++col) {
                                    arr[row * 2 + col] = GetSubElementsI(fractal, {data.index.x + col - targetIndex.x, data.index.y + row - targetIndex.y});
                                }
                            }

                            glm::imat2 mat = glm::make_mat2(arr);
                            newFractal = new Fractal<glm::imat2>(mat, data.index, startCoords, GetIndex());

                            break;
                        }
                case 4: {
                            int arr[16];

                            for(int row = 0; row < 4; ++row) {
                                for(int col = 0; col < 4; ++col) {
                                    arr[row * 4 + col] = GetSubElementsI(fractal, {data.index.x + col - targetIndex.x, data.index.y + row - targetIndex.y});
                                }
                            }

                            glm::imat4 mat = glm::make_mat4(arr);

                            newFractal = new Fractal<glm::imat4>(mat, data.index, startCoords, GetIndex());

                            break;
                        }
                default:
                        assert(false);
                        break;
            }

            m_Fractals.push_back(newFractal);
            newFractals.push_back(newFractal);
        }

        std::vector<BaseFractal*>::iterator iter = GetFractalIterator(fractal);
        m_Fractals.erase(iter);
        delete fractal; 

        return newFractals;

    }


    std::vector<BaseFractal*>::iterator Puzzle::GetFractalIterator(BaseFractal* fractal) {
        std::vector<BaseFractal*>::iterator iter = m_Fractals.end();

        for(std::vector<BaseFractal*>::iterator i = m_Fractals.begin(); i < m_Fractals.end(); ++i) {
            if(&(*(*i)) == &(*fractal)) iter = i;
        }

        return iter;
    }


    BaseFractal* Puzzle::GetClosestFractal(float mousex, float mousey) const {
        BaseFractal* closestF = nullptr;

        for(BaseFractal* f: m_Fractals) {
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

        BaseFractal* closestF = GetClosestFractal(mousex, mousey);

        if(!closestF) return fc;


        //determine which corner that fractal belongs to
        glm::ivec2 index = closestF->GetIndex();
        int size = GetFractalSize(closestF);
        BaseFractal* topLeft = nullptr;
        BaseFractal* topRight = nullptr;
        BaseFractal* bottomLeft = nullptr;
        BaseFractal* bottomRight = nullptr;

        if(closestF->x() - mousex < 0) { //closest fractal is on left of mouse
            if(closestF->y() - mousey < 0) { //closest fractal is below mouse
                bottomLeft = closestF;
                bottomRight = GetFractal(glm::ivec2(index.x + size, index.y));
                topLeft = GetFractal(glm::ivec2(index.x, index.y - size));
                topRight = GetFractal(glm::ivec2(index.x + size, index.y - size));
            }else{
                topLeft = closestF;
                topRight = GetFractal(glm::ivec2(index.x + size, index.y));
                bottomLeft = GetFractal(glm::ivec2(index.x, index.y + size));
                bottomRight = GetFractal(glm::ivec2(index.x + size, index.y + size));
            }
        }else{ //closest fractal is on right side of mouse
            if(closestF->y() - mousey < 0) { //closest fractal is below mouse
                bottomRight = closestF;
                bottomLeft = GetFractal(glm::ivec2(index.x - size, index.y));
                topLeft = GetFractal(glm::ivec2(index.x - size, index.y - size));
                topRight = GetFractal(glm::ivec2(index.x, index.y - size));
            }else{
                topRight = closestF;
                topLeft = GetFractal(glm::ivec2(index.x - size, index.y));
                bottomLeft = GetFractal(glm::ivec2(index.x - size, index.y + size));
                bottomRight = GetFractal(glm::ivec2(index.x, index.y + size));
            }
        }

        if(topLeft && GetFractalSize(topLeft) == size) fc.TopLeft = topLeft;
        if(topRight && GetFractalSize(topRight) == size) fc.TopRight = topRight;
        if(bottomLeft && GetFractalSize(bottomLeft) == size) fc.BottomLeft = bottomLeft;
        if(bottomRight && GetFractalSize(bottomRight) == size) fc.BottomRight = bottomRight;

        return fc;
    }


    void Puzzle::MergeFractals(std::vector<BaseFractal*> mergeList) {
        m_MergeLists.push_back(mergeList);
    }

    void Puzzle::CreateFromMergeList(const std::vector<BaseFractal*>& mergeList) {
        //temp: checking out what fractals in her
        //find index and size of merged fractal from subfractals in mergeList
        int smallestCol = 100; //100 is larger than any fractal will ever be
        int smallestRow = 100; //"
        int largestCol = 0; //== largestRow since only using square fractals

        for(BaseFractal* f: mergeList) {
            glm::ivec2 index = f->GetIndex();
            int size = GetFractalSize(f);
            if(index.x < smallestCol) smallestCol = index.x;
            if(index.y < smallestRow) smallestRow = index.y;
            if((index.x + size - 1) > largestCol) largestCol = index.x + size - 1;
        }

        int size = largestCol - smallestCol + 1;
        glm::ivec2 index = {smallestCol, smallestRow};
        glm::vec2 coords = BaseFractal::GetCoords(index, size, GetDimensions(), glm::vec2(x(), y()));

        //a size 1 fractal should never be formed, but putting it here for completeness
        switch(size) {
            case 1: {
                        int mat = GetSubElementsI(mergeList.front(), {0,0});
                        m_Fractals.emplace_back(new Fractal<int>(mat, index, coords, GetIndex()));
                        break;
                    }
            case 2: {
                        int arr[4];

                        for(int row = 0; row < 2; ++row) {
                            for(int col = 0; col < 2; ++col) {
                                BaseFractal* subF = nullptr;
                                for(BaseFractal* f: mergeList) {
                                    //check if f contains index {col, row}
                                    if(Contains(f, {index.x + col, index.y + row})) {
                                        subF = f;
                                        break;
                                    }
                                }

                                //get the individual elements
                                glm::ivec2 subIndex = subF->GetIndex();
                                int subSize = GetFractalSize(subF);
                                arr[row * 2 + col] = GetSubElementsI(subF, {index.x + col - subIndex.x, index.y + row - subIndex.y});
                            }
                        }

                        glm::imat2 mat = glm::make_mat2(arr);

                        m_Fractals.emplace_back(new Fractal<glm::imat2>(mat, index, coords, GetIndex()));
                        break;
                    }
            case 4: {
                        int arr[16];

                        for(int row = 0; row < 4; ++row) {
                            for(int col = 0; col < 4; ++col) {
                                BaseFractal* subF;
                                for(BaseFractal* f: mergeList) {
                                    //check if f contains index {col, row}
                                    if(Contains(f, {index.x + col, index.y + row})) {
                                        subF = f;
                                        break;
                                    }
                                }

                                //get the individual elements
                                glm::ivec2 subIndex = subF->GetIndex();
                                int subSize = GetFractalSize(subF);
                                arr[row * 4 + col] = GetSubElementsI(subF, {index.x + col - subIndex.x, index.y + row - subIndex.y});
                            }
                        }

                        glm::imat4 mat = glm::make_mat4(arr);

                        m_Fractals.emplace_back(new Fractal<glm::imat4>(mat, index, coords, GetIndex()));
                        break;
                    }
            default: {
                         assert(false);
                         break;
                     }
        }
    }



    void Puzzle::SwapFractals(BaseFractal* fractalA, BaseFractal* fractalB) {
        glm::ivec2 indexA = fractalA->GetIndex();
        fractalA->SetIndex(fractalB->GetIndex());
        fractalB->SetIndex(indexA);

        fractalA->MoveTo({fractalB->x(), fractalB->y()});
        fractalB->MoveTo({fractalA->x(), fractalA->y()});

        //how to transform A to get back to original position
        glm::ivec2 newIndexA = fractalA->GetIndex();
        glm::ivec2 newIndexB = fractalB->GetIndex();

        if(newIndexA.y == newIndexB.y) { //horizontal transformation
            if(newIndexA.x < newIndexB.x) m_TransformationStack.push_back({TransformationType::TranslatePosX, fractalA->GetIndex(), GetFractalSize(fractalA)});
            else m_TransformationStack.push_back({TransformationType::TranslateNegX, fractalA->GetIndex(), GetFractalSize(fractalA)});
        }else{ //vertical transformation
            if(newIndexA.y < newIndexB.y) m_TransformationStack.push_back({TransformationType::TranslatePosY, fractalA->GetIndex(), GetFractalSize(fractalA)});
            else m_TransformationStack.push_back({TransformationType::TranslateNegY, fractalA->GetIndex(), GetFractalSize(fractalA)});
        }

    }

    void Puzzle::RotateFractalCW(BaseFractal* fractal) {
        fractal->RotateBy(-1.5708);
        m_TransformationStack.push_back({TransformationType::RotateCCW, fractal->GetIndex(), GetFractalSize(fractal)});
    }

    void Puzzle::RotateFractalCCW(BaseFractal* fractal) {
        fractal->RotateBy(1.5708);
        m_TransformationStack.push_back({TransformationType::RotateCW, fractal->GetIndex(), GetFractalSize(fractal)});
    }

    void Puzzle::ReflectFractalX(BaseFractal* fractal) {
        fractal->ScaleTo({1.0f, -1.0f});
        m_TransformationStack.push_back({TransformationType::ReflectX, fractal->GetIndex(), GetFractalSize(fractal)});
    }

    void Puzzle::ReflectFractalY(BaseFractal* fractal) {
        fractal->ScaleTo({-1.0f, 1.0f});
        m_TransformationStack.push_back({TransformationType::ReflectY, fractal->GetIndex(), GetFractalSize(fractal)});
    }


    void Puzzle::UndoTransformation() {

        struct TransformationData tData = m_TransformationStack.back();

        BaseFractal* f = GetFractal(tData.FractalIndex);

        //temp: should only undo transformation after making sure fractal sizes are matching/correct
        if(tData.FractalSize != GetFractalSize(f)) return;

        BaseFractal* otherF;
        glm::ivec2 index = f->GetIndex();

        switch(tData.Transformation) {
            case TransformationType::TranslatePosX:
                otherF = GetFractal(glm::ivec2(index.x + GetFractalSize(f), index.y)); 
                if(otherF && GetFractalSize(f) == GetFractalSize(otherF)) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslateNegX:
                otherF = GetFractal(glm::ivec2(index.x - GetFractalSize(f), index.y)); 
                if(otherF && GetFractalSize(f) == GetFractalSize(otherF)) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslatePosY:
                otherF = GetFractal(glm::ivec2(index.x, index.y + GetFractalSize(f))); 
                if(otherF && GetFractalSize(f) == GetFractalSize(otherF)) SwapFractals(f, otherF);
                break;
            case TransformationType::TranslateNegY:
                otherF = GetFractal(glm::ivec2(index.x, index.y - GetFractalSize(f))); 
                if(otherF && GetFractalSize(f) == GetFractalSize(otherF)) SwapFractals(f, otherF);
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
