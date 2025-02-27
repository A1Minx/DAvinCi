// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <vector>

class Model {
public:
    Model();


    void addLine(float x1, float y1, float x2, float y2);

    void addShape();


    const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& getLines() const;

private:
    // Store multiple lines as pairs of coordinates
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> lines;
};

#endif // MODEL_H
