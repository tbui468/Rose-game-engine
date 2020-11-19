#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {

Puzzle::Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index) :
        Entity(sprite, size, boundingBox, pos) {
            m_Index = index; 

            const rose::Sprite fractalSprite = {{0, 0}, {32, 32}};
            const glm::vec2 fractalSize = glm::vec2(32.0f, 32.0f);
            const glm::vec4 fractalBoundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);

            for(int row = 0; row < 2; ++row) {
                for(int col = 0; col < 2; ++col) {
                    m_Fractals.emplace_back(new Fractal(fractalSprite, fractalSize, fractalBoundingBox, 
                                            glm::vec2(x() + col * 48.0f - 24.0f, y() - row * 48.0f - 24.0f), {col, row}));
                }
            }
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
}
void Puzzle::OnAnimationUpdate(float t) {
    Entity::OnAnimationUpdate(t);
    for(Fractal* f: m_Fractals) {
        if(f) f->OnAnimationUpdate(t);
    }
}

void Puzzle::Draw() const {
    Entity::Draw();
    for(Fractal* f: m_Fractals) {
        if(f) f->Draw();
    }
}

Fractal* Puzzle::GetFractal(const glm::ivec2& index) const {
    for(Fractal* f: m_Fractals) {
        if(f->GetIndex() == index) return f;
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
    //animate the four subfractals
}


}
