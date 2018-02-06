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

QString Entry::getRessource() const
{
    return ressource;
}

void Entry::setRessource(const QString &value)
{
    ressource = value;
}

QDate Entry::getDate() const
{
    return date;
}

void Entry::setDate(const QDate &value)
{
    date = value;
}

QString Entry::toString(){
    return fname + " " + lname + " " + ressource + " " + date.toString();
}

