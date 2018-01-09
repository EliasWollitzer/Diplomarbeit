#include "entry.h"

//halt die ffresse
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
