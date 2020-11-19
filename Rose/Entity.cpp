#include <memory>

#include "Entity.h"
#include "Application.h"
#include "Globals.h"
#include "renderer/Renderer.h"

namespace rose {

Entity::Entity(const Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos) {
    m_SpriteData = sprite;
    m_Size = g_Scale * size;
    m_BoundingBox = g_Scale * boundingBox;
    m_Pos = g_Scale * pos;
}

void Entity::MoveTo(const glm::vec2& _pos) {
    m_FromPos = m_Pos;
    m_ToPos = g_Scale * _pos;
}

void Entity::MoveBy(const glm::vec2& shift) {
    m_FromPos = m_Pos;
    m_ToPos = {m_Pos.x + g_Scale * shift.x, m_Pos.y + g_Scale * shift.y};
}

void Entity::FadeTo(float _alpha) {
    m_FromAlpha = m_Alpha;
    m_ToAlpha = _alpha;
}
void Entity::ScaleTo(const glm::vec2& _scale) {
    m_FromScale = {m_Scale.x, m_Scale.y};
    m_ToScale = _scale;
}
void Entity::RotateTo(float _angle) {
    m_FromAngle = m_Angle;
    m_ToAngle = _angle;
}
void Entity::OnAnimationUpdate(float _t) {
    m_Pos.x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * _t;
    m_Pos.y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * _t;
    m_Scale.x = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * _t;
    m_Scale.y = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * _t;
    m_Angle = m_FromAngle + (m_ToAngle - m_FromAngle) * _t;
    m_Alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * _t;
}
void Entity::OnAnimationEnd() {
    m_Pos.x = m_ToPos.x;
    m_Pos.y = m_ToPos.y;
    m_FromPos.x = m_Pos.x;
    m_FromPos.y = m_Pos.y;
    m_Scale.x = m_ToScale.x;
    m_Scale.y = m_ToScale.y;
    m_FromScale.x = m_Scale.x;
    m_FromScale.y = m_Scale.y;
    m_Angle = m_ToAngle;
    m_FromAngle = m_Angle;
    m_Alpha = m_ToAlpha;
    m_FromAlpha = m_Alpha;
}

void Entity::Draw() const {
    Application* app = Application::GetApplication();
    std::shared_ptr<Renderer> renderer = app->GetRenderer();
    renderer->AddEntity(this);

//    RenderCommand::AddQuad(this);
}

bool Entity::LeftTap(InputType input, float x, float y) const {
    if(input != InputType::LeftDown) return false;
    if(!PointCollision(x, y)) return false;
    return true;
}

bool Entity::PointCollision(float pointX, float pointY) const {
    if(pointX < m_Pos.x + m_BoundingBox.x - m_BoundingBox.z * .5f) return false;
    if(pointY < m_Pos.y + m_BoundingBox.y - m_BoundingBox.w * .5f) return false;
    if(pointX > m_Pos.x + m_BoundingBox.x + m_BoundingBox.z * .5f) return false;
    if(pointY > m_Pos.y + m_BoundingBox.y + m_BoundingBox.w * .5f) return false;
    return true;
}


glm::mat4 Entity::GetModelMatrix() const {
    glm::mat4 rotated = glm::rotate(glm::mat4(1.0f), m_Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale.x * m_Size.x, m_Scale.y * m_Size.y, 1.0f));
    glm::mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos.x, m_Pos.y, 0.0f));
    return translated * scaled * rotated;
}


}
