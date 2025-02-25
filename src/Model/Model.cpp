// Model.cpp
#include "Model.h"

Model::Model() {
}

void Model::addLine(float x1, float y1, float x2, float y2) {
    // Add a new line to the vector
    lines.push_back({{x1, y1}, {x2, y2}});
}

const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& Model::getLines() const {
    return lines;
}
