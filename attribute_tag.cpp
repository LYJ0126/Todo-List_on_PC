#include "attribute_tag.h"

Attribute_Tag::Attribute_Tag(TaskPriority _priority)
    :Tag("Attribute_Tag")
{priority = _priority;}

TaskPriority Attribute_Tag::get_priority() const
{
    return priority;
}

void Attribute_Tag::set_priority(TaskPriority _priority)
{
    priority = _priority;
}
