//#include <algorithm>

#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {


float PointDistance(const glm::ivec2& start, const glm::ivec2& end) {
    float deltaXSqr = pow(start.x - end.x, 2);
    float deltaYSqr = pow(start.y - end.y, 2);
    return sqrt(deltaXSqr + deltaYSqr);
}

Puzzle::Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index) :
        Entity(sprite, size, boundingBox, pos) {
            m_Index = index; 
            m_Dimensions = {4 , 4}; //temp: hard coding dimensions for now

          
            std::vector<FractalElement> elements0;
            elements0.emplace_back(FractalElement::Red);
            elements0.emplace_back(FractalElement::Green);
            elements0.emplace_back(FractalElement::Blue);
            elements0.emplace_back(FractalElement::Empty);
            Grid<FractalElement> grid0(elements0);
            glm::vec2 startCoords0 = Fractal::GetCoords({0, 0}, grid0.GetSize(), m_Dimensions, glm::vec2(x(), y()));
            m_Fractals.emplace_back(new Fractal(grid0, {0, 0}, glm::vec2(startCoords0.x, startCoords0.y), GetIndex()));

            std::vector<FractalElement> elements1;
            elements1.emplace_back(FractalElement::Red);
            elements1.emplace_back(FractalElement::Red);
            elements1.emplace_back(FractalElement::Red);
            elements1.emplace_back(FractalElement::Red);
            Grid<FractalElement> grid1(elements1);
            glm::vec2 startCoords1 = Fractal::GetCoords({2, 0}, grid1.GetSize(), m_Dimensions, glm::vec2(x(), y()));
            m_Fractals.emplace_back(new Fractal(grid1, {2, 0}, glm::vec2(startCoords1.x, startCoords1.y), GetIndex()));

            std::vector<FractalElement> elements2;
            elements2.emplace_back(FractalElement::Blue);
            elements2.emplace_back(FractalElement::Blue);
            elements2.emplace_back(FractalElement::Blue);
            elements2.emplace_back(FractalElement::Blue);
            Grid<FractalElement> grid2(elements2);
            glm::vec2 startCoords2 = Fractal::GetCoords({0, 2}, grid2.GetSize(), m_Dimensions, glm::vec2(x(), y()));
            m_Fractals.emplace_back(new Fractal(grid2, {0, 2}, glm::vec2(startCoords2.x, startCoords2.y), GetIndex()));

            std::vector<FractalElement> elements3;
            elements3.emplace_back(FractalElement::Green);
            elements3.emplace_back(FractalElement::Green);
            elements3.emplace_back(FractalElement::Green);
            elements3.emplace_back(FractalElement::Green);
            Grid<FractalElement> grid3(elements3);
            glm::vec2 startCoords = Fractal::GetCoords({2, 2}, grid3.GetSize(), m_Dimensions, glm::vec2(x(), y()));
            m_Fractals.emplace_back(new Fractal(grid3, {2, 2}, glm::vec2(startCoords.x, startCoords.y), GetIndex()));
/*

            for(int row = 0; row < gridSize; row += fractalSize) {
                for(int col = 0; col < gridSize; col += fractalSize) {
                    glm::vec2 startCoords = Fractal::GetCoords({col, row}, fractalSize, glm::ivec2(gridSize, gridSize), glm::vec2(x(), y()));
                    m_Fractals.emplace_back(new Fractal(elements0, {col, row}, glm::vec2(startCoords.x, startCoords.y), GetIndex()));
                }
            }*/
        }


Puzzle::~Puzzle() {
    for(Fractal* f: m_Fractals) {
        if(f) delete f;
    }
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
}

void Puzzle::MoveBy(const glm::vec2& shift) {
    Entity::MoveBy(shift);
    for(Fractal* f: m_Fractals) {
        if(f) f->MoveBy(shift);
    }
}


void Puzzle::OnAnimationEnd() {
    Entity::OnAnimationEnd();
    for(Fractal* f: m_Fractals) {
        if(f) f->OnAnimationEnd();
    }


    if(m_FractalCorners.TopLeft) {
    
        glm::ivec2 index = m_FractalCorners.TopLeft->GetIndex();
        int size = m_FractalCorners.TopLeft->GetSize();
        //temp:
        //glm::vec2 coords = glm::vec2(0.0f, 0.0f);
        glm::vec2 coords = Fractal::GetCoords(index, size * 2, GetDimensions(), glm::vec2(x(), y()));
        std::vector<FractalElement> elements;
        for(int i = 0; i < size * size * 4; ++i) {
            elements.push_back(FractalElement::Green);
        }

        Grid<FractalElement> grid(elements);

        m_Fractals.emplace_back(new Fractal(grid, index, coords, GetIndex()));

        std::vector<Fractal*>::iterator topLeft = GetFractalIterator(m_FractalCorners.TopLeft);
        m_Fractals.erase(topLeft);
        delete m_FractalCorners.TopLeft; 
        m_FractalCorners.TopLeft = nullptr;

        std::vector<Fractal*>::iterator topRight = GetFractalIterator(m_FractalCorners.TopRight);
        m_Fractals.erase(topRight);
        delete m_FractalCorners.TopRight; 
        m_FractalCorners.TopRight = nullptr;

        std::vector<Fractal*>::iterator bottomLeft = GetFractalIterator(m_FractalCorners.BottomLeft);
        m_Fractals.erase(bottomLeft);
        delete m_FractalCorners.BottomLeft; 
        m_FractalCorners.BottomLeft = nullptr;

        std::vector<Fractal*>::iterator bottomRight = GetFractalIterator(m_FractalCorners.BottomRight);
        m_Fractals.erase(bottomRight);
        delete m_FractalCorners.BottomRight; 
        m_FractalCorners.BottomRight = nullptr;


    }
}
void Puzzle::OnAnimationUpdate(float t) {
    Entity::OnAnimationUpdate(t);
    for(Fractal* f: m_Fractals) {
        if(f) f->OnAnimationUpdate(t);
    }
}

void Puzzle::Draw() const {
//    Entity::Draw();
    for(Fractal* f: m_Fractals) {
        if(f) f->Draw();
    }
}

Fractal* Puzzle::GetFractal(const glm::ivec2& index) const {
    for(Fractal* f: m_Fractals) {
        if(f->GetIndex() == index || f->Contains(index)) return f;
    }
    return nullptr;
}

void Puzzle::SwapFractals(Fractal* fractalA, Fractal* fractalB) {
    glm::ivec2 indexA = fractalA->GetIndex();
    fractalA->SetIndex(fractalB->GetIndex());
    fractalB->SetIndex(indexA);

    fractalA->MoveTo({fractalB->x(), fractalB->y()});
    fractalB->MoveTo({fractalA->x(), fractalA->y()});
}

void Puzzle::SplitFractal(Fractal* fractal) {

    //find fractal in m_Fractals and delete it
    //instantiate four subfractals and add them to m_Fractals
    glm::ivec2 index = fractal->GetIndex();
    int subFractalSize = fractal->GetSize() / 2;
    glm::vec2 startCoords0 = Fractal::GetCoordsForTarget(glm::ivec2(index.x, index.y), subFractalSize, index, fractal->GetSize(), m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 startCoords1 = Fractal::GetCoordsForTarget(glm::ivec2(index.x + subFractalSize, index.y) , subFractalSize, index, fractal->GetSize(), m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 startCoords2 = Fractal::GetCoordsForTarget(glm::ivec2(index.x, index.y + subFractalSize) , subFractalSize, index, fractal->GetSize(), m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 startCoords3 = Fractal::GetCoordsForTarget(glm::ivec2(index.x + subFractalSize, index.y + subFractalSize) , subFractalSize, index, fractal->GetSize(), m_Dimensions, glm::vec2(x(), y()));

    //temp: green fractals
    std::vector<FractalElement> elements;
    for(int i = 0; i < subFractalSize * subFractalSize; ++i) {
        elements.emplace_back(FractalElement::Green);
    }

    Grid<FractalElement> grid(elements);


    //todo: this code only works for 2x2 - how to generalize to allow splitting 4x4
    Fractal* fractal0 = new Fractal(grid, {index.x, index.y}, glm::vec2(startCoords0.x, startCoords0.y), GetIndex());
    Fractal* fractal1 = new Fractal(grid, {index.x + subFractalSize, index.y}, glm::vec2(startCoords1.x, startCoords1.y), GetIndex());
    Fractal* fractal2 = new Fractal(grid, {index.x, index.y + subFractalSize}, glm::vec2(startCoords2.x, startCoords2.y), GetIndex());
    Fractal* fractal3 = new Fractal(grid, {index.x + subFractalSize, index.y + subFractalSize}, glm::vec2(startCoords3.x, startCoords3.y), GetIndex());
   
    m_Fractals.push_back(fractal0);
    m_Fractals.push_back(fractal1);
    m_Fractals.push_back(fractal2);
    m_Fractals.push_back(fractal3);
    
    glm::vec2 endCoords0 = Fractal::GetCoords(glm::ivec2(index.x, index.y), subFractalSize, m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 endCoords1 = Fractal::GetCoords(glm::ivec2(index.x + subFractalSize, index.y), subFractalSize, m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 endCoords2 = Fractal::GetCoords(glm::ivec2(index.x, index.y + subFractalSize), subFractalSize, m_Dimensions, glm::vec2(x(), y()));
    glm::vec2 endCoords3 = Fractal::GetCoords(glm::ivec2(index.x + subFractalSize, index.y + subFractalSize), subFractalSize, m_Dimensions, glm::vec2(x(), y()));
    
    fractal0->MoveTo(endCoords0);
    fractal1->MoveTo(endCoords1);
    fractal2->MoveTo(endCoords2);
    fractal3->MoveTo(endCoords3);

    std::vector<Fractal*>::iterator iter = GetFractalIterator(fractal);
    m_Fractals.erase(iter);
    delete fractal; 
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
            float closestDis = PointDistance(glm::vec2(closestF->x(), closestF->y()), glm::vec2(mousex, mousey));
            float thisDis = PointDistance(glm::vec2(f->x(), f->y()), glm::vec2(mousex, mousey));
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

    if(topLeft && topLeft->GetSize() == size) fc.TopLeft = topLeft;
    if(topRight && topRight->GetSize() == size) fc.TopRight = topRight;
    if(bottomLeft && bottomLeft->GetSize() == size) fc.BottomLeft = bottomLeft;
    if(bottomRight && bottomRight->GetSize() == size) fc.BottomRight = bottomRight;

    return fc;
}

void Puzzle::FormFractal(FractalCorners fc) {
    glm::ivec2 targetIndex = fc.TopLeft->GetIndex();
    int subFractalSize = fc.TopLeft->GetSize();
    int targetSize = subFractalSize * 2;

    glm::vec2 topLeftEndCoords = Fractal::GetCoordsForTarget(fc.TopLeft->GetIndex(), subFractalSize, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
    fc.TopLeft->MoveTo(topLeftEndCoords);
    glm::vec2 topRightEndCoords = Fractal::GetCoordsForTarget(fc.TopRight->GetIndex(), subFractalSize, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
    fc.TopRight->MoveTo(topRightEndCoords);
    glm::vec2 bottomLeftEndCoords = Fractal::GetCoordsForTarget(fc.BottomLeft->GetIndex(), subFractalSize, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
    fc.BottomLeft->MoveTo(bottomLeftEndCoords);
    glm::vec2 bottomRightEndCoords = Fractal::GetCoordsForTarget(fc.BottomRight->GetIndex(), subFractalSize, targetIndex, targetSize, m_Dimensions, glm::vec2(x(), y()));
    fc.BottomRight->MoveTo(bottomRightEndCoords);

    m_FractalCorners.TopLeft = fc.TopLeft;
    m_FractalCorners.TopRight = fc.TopRight;
    m_FractalCorners.BottomLeft = fc.BottomLeft;
    m_FractalCorners.BottomRight = fc.BottomRight;
}


}
