#ifndef KIND_TAGLIST_H
#define KIND_TAGLIST_H

#include <vector>
#include "kind_tag.h"

class Kind_TagList
{
private:
    std::vector<Kind_Tag> kind_tag_list;
public:
    Kind_TagList();
    int get_kind_num() const;
    bool add_kind(QString kindname);
    bool delete_kind(QString kindname);
    Kind_Tag get_kind(QString kindname) const;
    std::vector<Kind_Tag> get_allkinds() const;

};

#endif // KIND_TAGLIST_H
