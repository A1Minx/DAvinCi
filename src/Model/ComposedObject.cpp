#include "ComposedObject.h"

ComposedObject::ComposedObject(int id, std::string name, std::shared_ptr<ComposedObject> parent, int parent_id)
    : id(id), name(name), parent(parent), parent_id(parent_id)
{
}

ComposedObject::~ComposedObject()
{
}

