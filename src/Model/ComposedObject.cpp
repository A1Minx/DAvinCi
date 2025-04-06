#include "ComposedObject.h"

ComposedObject::ComposedObject(std::string name, Object3D* parent)
    : name(name), parent(parent)
{
}

ComposedObject::~ComposedObject()
{
}

