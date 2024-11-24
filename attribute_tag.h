#ifndef ATTRIBUTE_TAG_H
#define ATTRIBUTE_TAG_H
#include "tag.h"
//#include <QString>

//属性标签有以下三种
enum TaskPriority {
    Urgent = 0,
    Important = 1,
    Normal = 2
};

class Attribute_Tag : public Tag
{
private:
    TaskPriority priority;//优先级
public:
    Attribute_Tag(TaskPriority _priority = TaskPriority::Normal);
    TaskPriority get_priority() const;
    void set_priority(TaskPriority _priority);

//signals:
};

#endif // ATTRIBUTE_TAG_H
