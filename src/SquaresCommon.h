#ifndef SQUARES_COMMON_H
#define SQUARES_COMMON_H


namespace sqs {

struct CartCoords {
    float x = 0.0f;
    float y = 0.0f;
};

struct CartCoordsi {
    int x = 0;
    int y = 0;
};

using Scale = CartCoords;

struct PolarCoords {
    float r = 0.0f;
    float a = 0.0f;
};

struct Color {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

struct Rectangle {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
};

}

#endif //SQUARES_COMMON_H

