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


    std::vector<Fractal*> Puzzle::SplitOverlappingWith(FractalData fractalData) {

        //finding all fractals that overlap the given fractal data
        std::vector<Fractal*> overlappingFractals;
        std::vector<Fractal*> allSplitFractals;

        for(Fractal* f: m_Fractals) {

            OverlapType type = Fractal::FindOverlapType({f->m_Size, f->m_Index}, fractalData);

            std::cout << "Fractal index: " << f->m_Index.x << ", " << f->m_Index.y << std::endl;
            std::cout << "Overlap type: ";

            switch(type) {
                case OverlapType::None:
                    std::cout << "None" << std::endl;
                    break;
                case OverlapType::Equal:
                    std::cout << "Equal" << std::endl;
                    allSplitFractals.push_back(f);
                    return allSplitFractals;
                    break;
                case OverlapType::Within:
                    std::cout << "Within" << std::endl;
                    allSplitFractals.push_back(f);
                    break;
                case OverlapType::Enclose:
                    std::cout << "Enclose" << std::endl;
                    overlappingFractals.push_back(f);
                    break;
                case OverlapType::Partial:
                    std::cout << "Partial" << std::endl;
                    overlappingFractals.push_back(f);
                    break;
            }
        }

        std::cout << std::endl;


        //all fractals with no intersections are already filtered out above
        for(Fractal* f: overlappingFractals) {
            std::vector<FractalData> fData;

            int size = f->m_Size;
            glm::ivec2 index = f->m_Index;

            switch(size) {
                case 1:
                    fData.push_back({1, f->m_Index}); 
                    break;
                case 2:
                    fData.push_back({1, index}); 
                    fData.push_back({1, {index.x + 1, index.y}}); 
                    fData.push_back({1, {index.x, index.y + 1}}); 
                    fData.push_back({1, {index.x + 1, index.y + 1}}); 
                    break;
                case 4:
                    FractalData fdArr[4];
                    fdArr[0] = {2, index};
                    fdArr[1] = {2, {index.x + 2, index.y}};
                    fdArr[2] = {2, {index.x, index.y + 2}};
                    fdArr[3] = {2, {index.x + 2, index.y + 2}};
                    for(int i = 0; i < 4; ++i) {
                        OverlapType type = Fractal::FindOverlapType(fdArr[i], fractalData);
                        if(type == OverlapType::None || type == OverlapType::Equal || type == OverlapType::Within) { //2x2 fractal doesn't need to be split anymore
                            fData.push_back(fdArr[i]);
                        }else{ //need to split 2x2 further into 1x1s
                            glm::ivec2 fdIndex = fdArr[i].index; 
                            fData.push_back({1, fdIndex}); 
                            fData.push_back({1, {fdIndex.x + 1, fdIndex.y}}); 
                            fData.push_back({1, {fdIndex.x, fdIndex.y + 1}}); 
                            fData.push_back({1, {fdIndex.x + 1, fdIndex.y + 1}}); 
                        }
                    }
                    break;
            }

            for(Fractal* f: SplitFractal(f, fData)) allSplitFractals.push_back(f);
        }

        return allSplitFractals;
    }


    std::vector<Fractal*> Puzzle::SplitFractal(Fractal* fractal, const std::vector<FractalData>& fractalData) {

        std::vector<Fractal*> newFractals;

        for(FractalData data: fractalData) {
            glm::vec2 startCoords = Fractal::GetCoordsForTarget(data.index, data.size, fractal->m_Index, fractal->m_Size, m_Dimensions, glm::vec2(x(), y()));
            Fractal* newFractal; 

            switch(data.size) {
                case 1: 
                    newFractal = new Fractal(CalculateSpriteData({data.size, data.index}, m_Index), data, startCoords, {this, &Puzzle::Transform});
                    break;
                case 2: 
                    newFractal = new Fractal(CalculateSpriteData({data.size, data.index}, m_Index), data, startCoords, {this, &Puzzle::Transform});
                    break;
                case 4: 
                    newFractal = new Fractal(CalculateSpriteData({data.size, data.index}, m_Index), data, startCoords, {this, &Puzzle::Transform});
                    break;
                default:
                    assert(false);
                    break;
            }

            m_Fractals.push_back(newFractal);
            newFractals.push_back(newFractal);
        }

        m_Fractals.erase(GetFractalIterator(fractal));
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

        if(topLeft && topLeft->m_Size == size) fc.TopLeft = topLeft;
        if(topRight && topRight->m_Size == size) fc.TopRight = topRight;
        if(bottomLeft && bottomLeft->m_Size == size) fc.BottomLeft = bottomLeft;
        if(bottomRight && bottomRight->m_Size == size) fc.BottomRight = bottomRight;

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
            if(newIndexA.x < newIndexB.x) PushTransformation({TransformationType::TranslatePosX, {fractalA->m_Size, fractalA->m_Index}});
            else PushTransformation({TransformationType::TranslateNegX, {fractalA->m_Size, fractalA->m_Index}});
        }else{ //vertical transformation
            if(newIndexA.y < newIndexB.y) PushTransformation({TransformationType::TranslatePosY, {fractalA->m_Size, fractalA->m_Index}});
            else PushTransformation({TransformationType::TranslateNegY, {fractalA->m_Size, fractalA->m_Index}});
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
                int newCol = fractal->m_Size - 1 - row;
                int newRow = col;
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
                int newCol = row;
                int newRow = fractal->m_Size - 1 - col;
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

        //@todo: should only undo transformation after making sure fractal sizes are matching/correct
        if(tData.fractalData.size != f->m_Size) return;

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


    void Puzzle::UpdateTextureData(FractalData data) const {
        /*
        //@temp: making it all red to test
        //@reference
          struct SubTextureMapping {
          glm::ivec2 DesTexCoords; //to custom texture
          glm::ivec2 SrcTexCoords; //from default texture
          glm::ivec2 TexDimensions;
          };

          std::vector<rose::SubTextureMapping> texMapping;
          //frame
          texMapping.push_back({{0, 0}, {0, 0}, {Fractal::s_UnitSize, Fractal::s_UnitSize}}); 
          texMapping.push_back({{1, 1}, {Fractal::s_UnitSize + 1, 1}, {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2}});
          texMapping.push_back({{Fractal::s_UnitSize, Fractal::s_UnitSize}, {0, 0}, {Fractal::s_UnitSize, Fractal::s_UnitSize}}); 
          texMapping.push_back({{Fractal::s_UnitSize + 1, Fractal::s_UnitSize + 1}, {Fractal::s_UnitSize + 1, Fractal::s_UnitSize * 2 + 1}, {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2}});
          rose::Application::GetApplication()->SetCustomTexture(texMapping);

          return;*/

        //textures start from bottom left, but starting textures from top left to fit fractal order (left to right, top to bottom)
        glm::ivec2 texStart =  glm::ivec2(data.index.x * Fractal::s_UnitSize + m_Index * 256, 256 - (data.index.y + 1) * Fractal::s_UnitSize); 

        std::vector<rose::SubTextureMapping> texMapping;

        for(int row = 0; row < data.size; ++row) { for(int col = 0; col < data.size; ++col) {
            //pushing on fractal frame
            texMapping.push_back({{texStart.x + col * Fractal::s_UnitSize, texStart.y - row * Fractal::s_UnitSize}, {0, 0}, {Fractal::s_UnitSize, Fractal::s_UnitSize}}); 

            //elements are drawn inside fractal frame, so start point is offset by 1 and side length is reduced by 2 in each dimension
            switch(GetElementAt(col + data.index.x, row + data.index.y)) {
                case 'r':
                    texMapping.push_back({{texStart.x + col * Fractal::s_UnitSize + 1, texStart.y - row * Fractal::s_UnitSize + 1}, 
                            {Fractal::s_UnitSize + 1, 1}, {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2}});
                    break;
                case 'b':
                    texMapping.push_back({{texStart.x + col * Fractal::s_UnitSize + 1, texStart.y - row * Fractal::s_UnitSize + 1}, 
                            {Fractal::s_UnitSize + 1, Fractal::s_UnitSize + 1}, {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2}});
                    break;
                case 'g':
                    texMapping.push_back({{texStart.x + col * Fractal::s_UnitSize + 1, texStart.y - row * Fractal::s_UnitSize + 1}, 
                            {Fractal::s_UnitSize + 1, Fractal::s_UnitSize * 2 + 1}, {Fractal::s_UnitSize - 2, Fractal::s_UnitSize - 2}});
                    break;
            }
        }}

        rose::Application::GetApplication()->SetCustomTexture(texMapping);
    }


    rose::Sprite Puzzle::CalculateSpriteData(FractalData data, int puzzleIndex) {
        glm::ivec2 texStart =  glm::ivec2(data.index.x * Fractal::s_UnitSize + puzzleIndex * 256, 256 - (data.index.y + 1) * Fractal::s_UnitSize); 
        glm::vec2 start = {texStart.x + Fractal::s_UnitSize * data.index.x, texStart.y - Fractal::s_UnitSize * data.index.y};
        glm::vec2 size = {Fractal::s_UnitSize * data.size , Fractal::s_UnitSize * data.size};
        return {start, size, rose::TextureType::Custom };
    }


}
