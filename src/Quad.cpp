#include "Quad.h"

namespace sqs {

void Quad::MoveTo(CartCoords _pos) {
    m_FromPos = {x, y};
    m_ToPos = _pos;
}
void Quad::FadeTo(float _alpha) {
    m_FromAlpha = alpha;
    m_ToAlpha = _alpha;
}
void Quad::ScaleTo(CartCoords _scale) {
    m_FromScale = {xScale, yScale};
    m_ToScale = _scale;
}
void Quad::RotateTo(float _angle) {
    m_FromAngle = angle;
    m_ToAngle = _angle;
}
void Quad::OnAnimationUpdate(float _t) {
    x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * _t;
    y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * _t;
    xScale = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * _t;
    yScale = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * _t;
    angle = m_FromAngle + (m_ToAngle - m_FromAngle) * _t;
    alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * _t;
}
void Quad::OnAnimationEnd() {
    x = m_ToPos.x;
    y = m_ToPos.y;
    xScale = m_ToScale.y;
    yScale = m_ToScale.x;
    angle = m_ToAngle;
    alpha = m_ToAlpha;
}


bool Quad::PointCollision(int pointX, int pointY) const {
    float _x = static_cast<float>(pointX);
    float _y = static_cast<float>(pointY);
    if(_x < m_BoundingBox.x) return false;
    if(_y < m_BoundingBox.y) return false;
    if(_x > m_BoundingBox.x + m_BoundingBox.w) return false;
    if(_y > m_BoundingBox.y + m_BoundingBox.h) return false;
    return true;
}

void Quad::SetBoundingCoords(float _x, float _y, float _w, float _h) {
    m_BoundingBox.x = _x;
    m_BoundingBox.y = _y;
    m_BoundingBox.w = _w;
    m_BoundingBox.h = _h;
}

void Quad::SetDepth(uint32_t _depth) {
    depth = _depth;
}
void Quad::SetPosition(CartCoords _coords) {
    x = _coords.x;
    y = _coords.y;
}

}
