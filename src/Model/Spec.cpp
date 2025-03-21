#include "Spec.h"

Spec::Spec(std::string name, float color[4], int id) : name(name), id(id) {
    for (int i = 0; i < 4; i++) {
        this->color[i] = color[i];
    }
}

PointSpec::PointSpec(std::string name, float color[4], float size, int id) : Spec(name, color, id), size(size) {}

LineSpec::LineSpec(std::string name, float color[4], float width, int id) : Spec(name, color, id), width(width) {}

