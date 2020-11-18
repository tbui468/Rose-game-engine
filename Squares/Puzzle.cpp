#include "Rose.h"
#include "Puzzle.h"
#include "Fractal.h"

namespace sqs {

Puzzle::Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index) :
        Entity(sprite, size, boundingBox, pos) {
            m_Index = index; 
            m_FractalGrid.fill(nullptr);

            rose::Sprite fractalSprite = {{0, 0}, {32, 32}};
            glm::vec2 fractalSize = glm::vec2(32.0f, 32.0f);
            glm::vec4 fractalBoundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);

            for(int row = 0; row < 2; ++row) {
                for(int col = 0; col < 2; ++col) {
                    m_FractalGrid.at(row * 2 + col) = new Fractal(fractalSprite, fractalSize, fractalBoundingBox, 
                                    glm::vec2(x() + col * 48.0f - 24.0f, y() + row * 48.0f - 24.0f));
                }
            }
        }


Puzzle::~Puzzle() {
    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) delete m_FractalGrid.at(i);
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

    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) m_FractalGrid.at(i)->MoveBy(glm::vec2(deltaX, deltaY));
    }
}

void Puzzle::MoveBy(const glm::vec2& shift) {
    Entity::MoveBy(shift);
    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) m_FractalGrid.at(i)->MoveBy(shift);
    }
}


void Puzzle::OnAnimationEnd() {
    Entity::OnAnimationEnd();
    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) m_FractalGrid.at(i)->OnAnimationEnd();
    }
}
void Puzzle::OnAnimationUpdate(float t) {
    Entity::OnAnimationUpdate(t);
    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) m_FractalGrid.at(i)->OnAnimationUpdate(t);
    }
}

void Puzzle::Draw() {
    Entity::Draw();
    for(int i = 0; i < 4; ++i) {
        if(m_FractalGrid.at(i)) m_FractalGrid.at(i)->Draw();
    }
}

}
