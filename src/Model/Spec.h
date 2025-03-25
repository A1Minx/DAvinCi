#ifndef SPEC_H_INCLUDED
#define SPEC_H_INCLUDED

#include <cstddef>
#include <string>
class Spec 
{
protected:
    int id;
    float color[4];
    std::string name;

public:
    Spec(std::string name, float color[4], int id) : id(id), name(name) {
        for (int i = 0; i < 4; i++) {
            this->color[i] = color[i];
        }
    };

    virtual ~Spec() = default;
    virtual float* getColor() { return color; };
    virtual std::string getName() { return name; };
    int getID() const { return id; }
};


class PointSpec : public Spec 
{
public:
    PointSpec(std::string name, float color[4], float size, int id) : Spec(name, color, id), size(size) {}
    virtual ~PointSpec() = default;
    float getSize() const { return size; }
protected:
    float size;
};


class LineSpec : public Spec 
{
public:
    LineSpec(std::string name, float color[4], float width, int id) : Spec(name, color, id), width(width) {}
    virtual ~LineSpec() = default;
    float getWidth() const { return width; }

protected:
    float width;
};

#endif // SPEC_H_INCLUDED

