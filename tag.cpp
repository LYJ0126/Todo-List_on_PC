#include "tag.h"

Tag::Tag(QString _name)
    :name(_name)
{}

QString Tag::getname() const
{
    return name;
}

void Tag::setname(QString _name)
{
    name = _name;
}
