#include "kind_taglist.h"

//程序初始化时,种类标签列表中初始建立4中标签
Kind_TagList::Kind_TagList()
{
    kind_tag_list.clear();
    kind_tag_list.push_back(Kind_Tag("学习任务"));
    kind_tag_list.push_back(Kind_Tag("工作任务"));
    kind_tag_list.push_back(Kind_Tag("生活任务"));
    kind_tag_list.push_back(Kind_Tag("其他"));
}

int Kind_TagList::get_kind_num() const
{
    return kind_tag_list.size();
}

Kind_Tag Kind_TagList::get_kind(QString kindname) const// 返回名为kindname的任务标签
{
    /*int n = kind_tag_list.size();
    for(int i = 0 ; i < n; ++i){
        if(kind_tag_list[i].getname() == kindname) return kind_tag_list[i];
    }*/
    for(const Kind_Tag& kind_tag : kind_tag_list){
        if(kind_tag.getname() == kindname) return kind_tag;
    }
    return Kind_Tag("无此项");//设置特殊标签,表示没有这类任务
}

std::vector<Kind_Tag> Kind_TagList::get_allkinds() const //返回所有任务标签
{
    std::vector<Kind_Tag> res;
    res.assign(kind_tag_list.begin(), kind_tag_list.end());
    return res;
}

bool Kind_TagList::add_kind(QString kindname)
{
    for(const Kind_Tag& kind_tag : kind_tag_list){
        if(kind_tag.getname() == kindname) return false;
    }
    kind_tag_list.push_back(Kind_Tag(kindname));
    return true;
}

bool Kind_TagList::delete_kind(QString kindname)
{
    if(kindname == "学习任务" || kindname == "工作任务" || kindname == "生活任务" || kindname == "其他") return false;
    std::vector<Kind_Tag>::iterator it = kind_tag_list.begin();
    for (; it != kind_tag_list.end();) {
        if (it->getname() == kindname) {
            it = kind_tag_list.erase(it);
            return true;
        }
        else {
            ++it;
        }
    }
    return false;
}
