#ifndef TAG_H
#define TAG_H

#include <QString>
class Tag
{
private:
    QString name;
public:
    Tag(QString _name = "");
    QString getname() const;
    void setname(QString _name);


//signals:
};

#endif // TAG_H
