#include "PuzzleSelector.h"

namespace sqs {

PuzzleSelector::PuzzleSelector(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
    Entity(sprite, size, boundingBox, pos) {
        rose::Sprite iconsprite = {{32, 32}, {8, 8}};
        glm::vec2 iconsize = {8.0f, 8.0f};
        glm::vec4 iconbox = {0.0f, 0.0f, 8.0f, 8.0f};
        float margin = 24.0f;
        int iconCount = 8;
        float halfLength = (iconCount - 1) * margin / 2.0f;
        for(int i = 0; i < iconCount; ++i) {
            m_PuzzleIconList.emplace_back(new PuzzleIcon(iconsprite, iconsize, iconbox, glm::vec2(-halfLength + margin * i, y())));
        }
    }


PuzzleSelector::~PuzzleSelector() {
    for(PuzzleIcon* icon: m_PuzzleIconList) {
        if(icon) delete icon;
    }
}


void PuzzleSelector::MoveTo(const glm::vec2& pos) {
    Entity::MoveTo(pos);

    float deltaX = pos.x - x();
    float deltaY = pos.y - y();
    for(PuzzleIcon* icon : m_PuzzleIconList) {
        if(icon) icon->MoveTo(glm::vec2(icon->x() + deltaX, icon->y() + deltaY));
    }
}


void PuzzleSelector::OnAnimationEnd() {
    Entity::OnAnimationEnd();
    for(PuzzleIcon* icon: m_PuzzleIconList) {
        if(icon) icon->OnAnimationEnd();
    }
}

void PuzzleSelector::OnAnimationUpdate(float t) {
    Entity::OnAnimationUpdate(t);
    for(PuzzleIcon* icon: m_PuzzleIconList) {
        if(icon) icon->OnAnimationUpdate(t);
    }
}

void PuzzleSelector::Draw() {
    //selector just organizes icons, so it doesn't need to be drawn
    for(PuzzleIcon* icon: m_PuzzleIconList) {
        if(icon) icon->Draw();
    }
}


}
