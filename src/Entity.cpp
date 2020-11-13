#include "Entity.h"

namespace rose {

Entity::Entity(CartCoords pos, uint32_t depth) {
    SetPosition(pos);
    SetDepth(depth);
}

void Entity::MoveTo(CartCoords _pos) {
    m_FromPos = {x, y};
    m_ToPos = {_pos.x, _pos.y};
}
void Entity::FadeTo(float _alpha) {
    m_FromAlpha = alpha;
    m_ToAlpha = _alpha;
}
void Entity::ScaleTo(CartCoords _scale) {
    m_FromScale = {xScale, yScale};
    m_ToScale = _scale;
}
void Entity::RotateTo(float _angle) {
    m_FromAngle = angle;
    m_ToAngle = _angle;
}
void Entity::OnAnimationUpdate(float _t) {
    x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * _t;
    y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * _t;
    xScale = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * _t;
    yScale = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * _t;
    angle = m_FromAngle + (m_ToAngle - m_FromAngle) * _t;
    alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * _t;
}
void Entity::OnAnimationEnd() {
    x = m_ToPos.x;
    y = m_ToPos.y;
    m_FromPos.x = x;
    m_FromPos.y = y;
    xScale = m_ToScale.x;
    yScale = m_ToScale.y;
    m_FromScale.x = xScale;
    m_FromScale.y = yScale;
    angle = m_ToAngle;
    m_FromAngle = angle;
    alpha = m_ToAlpha;
    m_FromAlpha = alpha;
}


bool Entity::PointCollision(float pointX, float pointY) const {
    float left = x + m_BoundingBox.x - m_BoundingBox.w * .5f;
    float right = x + m_BoundingBox.x + m_BoundingBox.w * .5f;
    float upper = y + m_BoundingBox.y - m_BoundingBox.h * .5f;
    float lower = y + m_BoundingBox.y + m_BoundingBox.h * .5f;
    if(pointX < left) return false;
    if(pointY < upper) return false;
    if(pointX > right) return false;
    if(pointY > lower) return false;
    return true;
}

void Entity::SetBoundingBox(float _x, float _y, float _w, float _h) {
    m_BoundingBox.x = _x;
    m_BoundingBox.y = _y;
    m_BoundingBox.w = _w;
    m_BoundingBox.h = _h;
}

void Entity::SetDepth(uint32_t _depth) {
    depth = _depth;
}
void Entity::SetPosition(CartCoords _coords) {
    x = _coords.x;
    y = _coords.y;
    m_FromPos = {_coords.x, _coords.y};
    m_ToPos = {_coords.x, _coords.y};
}

}
