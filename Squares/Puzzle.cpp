#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {


    Puzzle::Puzzle(int index, int setIndex): Entity(s_Sprite, s_ObjectSize, s_BoundingBox, glm::vec2(s_InitOffset + s_Spacing * index, 0.0f)),
                                             m_Index(index), m_SetIndex(setIndex) {
            //undo icons 
            for(int i = 0; i < GetMaxTransformations(); ++i) {
                m_UndoIcons.emplace_back(new UndoIcon(glm::vec2(UndoIcon::s_Margin * ((1 - GetMaxTransformations()) / 2.0f + i) + x(), -110.0f), this));
            }

            std::vector<FractalElement> elements = g_Data.at(setIndex).puzzlesData.at(index).elements;

            //make a bunch of 1x1 fractals as starting point for all puzzles
            for(int row = 0; row < GetDimensions().y; ++row) {
                for(int col = 0; col < GetDimensions().x; ++col) {
                    FractalElement element = elements.at(row * GetDimensions().x + col);
                    if(element != 'E') {
                        glm::vec2 startCoords = Fractal::GetCoords({col, row}, 1, GetDimensions(), glm::vec2(x(), y()));
//                        m_Fractals.emplace_back(new Fractal({element}, {col, row}, glm::vec2(startCoords.x, startCoords.y), GetIndex()));
                        m_Fractals.emplace_back(new Fractal(GetTempSprite(1), {1, {col, row}}, startCoords, {this, &Puzzle::Transform}));
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

        if(m_MergeList.empty()) return; 

        std::vector<std::vector<FractalElement>> elementsList;
        
        for(int i = 0; i < m_MergeList.size(); ++i) {
          elementsList.push_back(std::vector<FractalElement>());
          FractalData data = m_MergeList.at(i);
          for(int k = 0; k < data.size * data.size; ++k) {
            elementsList.at(i).push_back('r');
          }
        }

       
        std::vector<Fractal*>::iterator iter = m_Fractals.begin();

        //this loop is the main issue, it seems
        while(iter != m_Fractals.end()) {
          Fractal* f = *iter;
          bool contained = false;

          for(int k = 0; k < m_MergeList.size(); ++k) {
            FractalData data = m_MergeList.at(k);
            if(Fractal::FindOverlapType(data, {f->GetSize(), f->GetIndex()}) == OverlapType::Enclose) {
              contained = true;
            }
            if(contained) break;
          }
          if(contained) iter = m_Fractals.erase(iter); //erase invalidates the iterator I pass in, but returns a new iterator that can be used
          else iter++;

        }



        //create new fractals using merge data
        //need to remove temp elements vector once code above works
        for(int i = 0; i < m_MergeList.size(); ++i) {
          FractalData data = m_MergeList.at(i);
          glm::vec2 startCoords = Fractal::GetCoords(data.index, data.size, GetDimensions(), {x(), y()});
          //m_Fractals.push_back(new Fractal(elementsList.at(i), data.index, startCoords, GetIndex()));
          m_Fractals.emplace_back(new Fractal(GetTempSprite(data.size), data, startCoords, {this, &Puzzle::Transform}));
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
        if(f->GetIndex() == index) return f;
      }
      return nullptr;
    }


    std::vector<Fractal*> Puzzle::SplitOverlappingWith(FractalData fractalData) {

      //finding all fractals that overlap the given fractal data
      std::vector<Fractal*> overlappingFractals;
      std::vector<Fractal*> allSplitFractals;

      for(Fractal* f: m_Fractals) {

        OverlapType type = Fractal::FindOverlapType({f->GetSize(), f->GetIndex()}, fractalData);

        std::cout << "Fractal index: " << f->GetIndex().x << ", " << f->GetIndex().y << std::endl;
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

        int size = f->GetSize();
        glm::ivec2 index = f->GetIndex();

        switch(size) {
          case 1:
            fData.push_back({1, f->GetIndex()}); 
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

      int targetSize = fractal->GetSize();
      glm::ivec2 targetIndex = fractal->GetIndex();


      for(FractalData data: fractalData) {
        //find start coordinates from fractaldata
        glm::vec2 startCoords = Fractal::GetCoordsForTarget(data.index, data.size, targetIndex, targetSize, GetDimensions(), glm::vec2(x(), y()));
        //create fractal at the calculated coordinates
        //with given elements
        Fractal* newFractal; 

        switch(data.size) {
          case 1: 
            newFractal = new Fractal(GetTempSprite(data.size), data, startCoords, {this, &Puzzle::Transform});
            break;
          case 2: 
            newFractal = new Fractal(GetTempSprite(data.size), data, startCoords, {this, &Puzzle::Transform});
            break;
          case 4: 
            newFractal = new Fractal(GetTempSprite(data.size), data, startCoords, {this, &Puzzle::Transform});
            break;
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


    void Puzzle::MergeFractals(FractalData data) {
      m_MergeList.push_back(data);
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
        glm::ivec2 indexA = fractalA->GetIndex();
        fractalA->SetIndex(fractalB->GetIndex());
        fractalB->SetIndex(indexA);

        fractalA->MoveTo({fractalB->x(), fractalB->y()});
        fractalB->MoveTo({fractalA->x(), fractalA->y()});

        //how to transform A to get back to original position
        glm::ivec2 newIndexA = fractalA->GetIndex();
        glm::ivec2 newIndexB = fractalB->GetIndex();

        if(newIndexA.y == newIndexB.y) { //horizontal transformation
            if(newIndexA.x < newIndexB.x) PushTransformation({TransformationType::TranslatePosX, {fractalA->GetSize(), fractalA->GetIndex()}});
            else PushTransformation({TransformationType::TranslateNegX, {fractalA->GetSize(), fractalA->GetIndex()}});
        }else{ //vertical transformation
            if(newIndexA.y < newIndexB.y) PushTransformation({TransformationType::TranslatePosY, {fractalA->GetSize(), fractalA->GetIndex()}});
            else PushTransformation({TransformationType::TranslateNegY, {fractalA->GetSize(), fractalA->GetIndex()}});
        }


        //////////////////////////write new element data after swapping///////////////////////////////
        std::vector<FractalElement> elementsA = GetElements({fractalA->GetSize(), fractalA->GetIndex()});

        int xShift = fractalB->GetIndex().x - fractalA->GetIndex().x;
        int yShift = fractalB->GetIndex().y - fractalA->GetIndex().y;
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;

        for(int row = fractalA->GetIndex().y; row < fractalA->GetIndex().y + fractalA->GetSize(); ++row) {
            for(int col = fractalA->GetIndex().x; col < fractalA->GetIndex().x + fractalA->GetSize(); ++col) {
                //replace value in fractal A with one in B
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col) = 
                    g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at((row + yShift)* dim.x + (col + xShift));
                //replace value in fractal B with cached A value in elementsA
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at((row + yShift)* dim.x + (col + xShift)) = 
                    elementsA.at((row - fractalA->GetIndex().y) * fractalA->GetSize() + (col - fractalA->GetIndex().x));
            }
        }


    }


    void Puzzle::RotateFractalCW(Fractal* fractal) {
        fractal->RotateBy(-1.5708);
        PushTransformation({TransformationType::RotateCCW, {fractal->GetSize(), fractal->GetIndex()}});

        std::vector<FractalElement> elements = GetElements({fractal->GetSize(), fractal->GetIndex()});
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;

        //to rotate ccw - swap col/row and flip over y (eg, reverse iteration over cols)
        for(int row = fractal->GetIndex().y; row < fractal->GetIndex().y + fractal->GetSize(); ++row) {
            for(int col = fractal->GetIndex().x; col < fractal->GetIndex().x + fractal->GetSize(); ++col) {
                int newCol = fractal->GetIndex().x + fractal->GetSize() - 1 - row;
                int newRow = col;
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col) = 
                    elements.at(newRow * fractal->GetSize() + newCol);
            }
        }

    }

    void Puzzle::RotateFractalCCW(Fractal* fractal) {
        fractal->RotateBy(1.5708);
        PushTransformation({TransformationType::RotateCW, {fractal->GetSize(), fractal->GetIndex()}});

        std::vector<FractalElement> elements = GetElements({fractal->GetSize(), fractal->GetIndex()});
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;

        //to rotate ccw - swap col/row and flip over x (eg, reverse iteration over rows)
        for(int row = fractal->GetIndex().y; row < fractal->GetIndex().y + fractal->GetSize(); ++row) {
            for(int col = fractal->GetIndex().x; col < fractal->GetIndex().x + fractal->GetSize(); ++col) {
                int newCol = row;
                int newRow = fractal->GetIndex().y + fractal->GetSize() - 1 - col;
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col) = 
                    elements.at(newRow * fractal->GetSize() + newCol);
            }
        }

    }

    void Puzzle::ReflectFractalX(Fractal* fractal) {
        fractal->ScaleTo({1.0f, -1.0f});
        PushTransformation({TransformationType::ReflectX, {fractal->GetSize(), fractal->GetIndex()}});

        std::vector<FractalElement> elements = GetElements({fractal->GetSize(), fractal->GetIndex()});
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;

        //flips rows
        for(int row = fractal->GetIndex().y; row < fractal->GetIndex().y + fractal->GetSize(); ++row) {
            for(int col = fractal->GetIndex().x; col < fractal->GetIndex().x + fractal->GetSize(); ++col) {
                int reverseRow = fractal->GetIndex().y + fractal->GetSize() - 1 - row; 
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col) = 
                    elements.at(reverseRow * fractal->GetSize() + col);
            }
        }

    }

    void Puzzle::ReflectFractalY(Fractal* fractal) {
        fractal->ScaleTo({-1.0f, 1.0f});
        PushTransformation({TransformationType::ReflectY, {fractal->GetSize(), fractal->GetIndex()}});

        std::vector<FractalElement> elements = GetElements({fractal->GetSize(), fractal->GetIndex()});
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;

        //flips cols
        for(int row = fractal->GetIndex().y; row < fractal->GetIndex().y + fractal->GetSize(); ++row) {
            for(int col = fractal->GetIndex().x; col < fractal->GetIndex().x + fractal->GetSize(); ++col) {
                int reverseCol = fractal->GetIndex().x + fractal->GetSize() - 1 - col;
                g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col) = 
                    elements.at(row * fractal->GetSize() + reverseCol);
            }
        }

    }


    void Puzzle::UndoLastTransformation() {

        struct TransformationData tData = PeekTransformation();

        Fractal* f = GetFractalWithIndex(tData.fractalData.index);

        //@todo: should only undo transformation after making sure fractal sizes are matching/correct
        if(tData.fractalData.size != f->GetSize()) return;

        Transform(tData.fractalData, tData.transformation);

        //the undo transformation pushes another transformation onto the transformation stack, 
        //so pop_back is called twice - the first undo removes the transoformation pushed onto the 
        //stack in the switch statement above, and the second is the actual transformation we want to undo
        PopTransformation();
        PopTransformation();
    }


    std::vector<FractalElement> Puzzle::GetElements(FractalData data) const {
        glm::ivec2 dim = g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).dimensions;
        std::vector<FractalElement> elements;
        for(int row = data.index.y; row < data.index.y + data.size; ++row) {
            for(int col = data.index.x; col < data.index.x + data.size; ++col) {
                elements.push_back(g_Data.at(GetSetIndex()).puzzlesData.at(GetIndex()).elements.at(row * dim.x + col));
            }
        }
        return elements;
    }


}
