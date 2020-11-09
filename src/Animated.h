#ifndef ANIMATED_H
#define ANIMATED_H

namespace sqs {

class Animated {
    public:
        Animated() = default;
        virtual ~Animated() = default;
        void MoveTo(CartCoords pos) {
            m_FromPos = m_Pos;
            m_ToPos = pos;
        }
        void FadeTo(float alpha) {
            m_FromAlpha = m_Alpha;
            m_ToAlpha = alpha;
        }
        void ScaleTo(CartCoords scale) {
            m_FromScale = m_Scale;
            m_ToScale = scale;
        }
        virtual void OnUpdate(float t) {
            m_Pos.x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * t;
            m_Pos.y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * t;
            m_Scale.x = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * t;
            m_Scale.y = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * t;
            m_Alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * t;
        }
        virtual void OnEnd(float t) {
            m_Pos.x = m_ToPos.x;
            m_Pos.y = m_ToPos.y;
            m_Scale.x = m_ToScale.y;
            m_Scale.y = m_ToScale.x;
            m_Alpha = m_ToAlpha;
        }
    private:
        CartCoords m_Pos;
        CartCoords m_FromPos;
        CartCoords m_ToPos;
        float m_Alpha;
        float m_FromAlpha;
        float m_ToAlpha;
        Scale m_Scale;
        Scale m_FromScale;
        Scale m_ToScale;
};

}

#endif //ANIMATED_H
