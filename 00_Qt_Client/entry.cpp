#include "entry.h"

Entry::Entry()
{    

}

QString Entry::getFname() const
{
    return fname;
}

void Entry::setFname(const QString &value)
{
    fname = value;
}

QString Entry::getLname() const
{
    return lname;
}

void Entry::setLname(const QString &value)
{
    lname = value;
}

QString Entry::getRessoure() const
{
    return ressoure;
}

void Entry::setRessoure(const QString &value)
{
    ressoure = value;
}

QDate Entry::getDate() const
{
    return date;
}

void Entry::setDate(const QDate &value)
{
    date = value;
}

