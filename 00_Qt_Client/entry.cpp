#include "entry.h"

Entry::Entry()
{    

}


QString Entry::toString(){
    return firstName + lastName + description + ressource + datefrom.toString() + dateto.toString();
}

QString Entry::getFirstName() const
{
    return firstName;
}

void Entry::setFirstName(const QString &value)
{
    firstName = value;
}

QString Entry::getLastName() const
{
    return lastName;
}

void Entry::setLastName(const QString &value)
{
    lastName = value;
}

QString Entry::getRessource() const
{
    return ressource;
}

void Entry::setRessource(const QString &value)
{
    ressource = value;
}

QDateTime Entry::getDatefrom() const
{
    return datefrom;
}

void Entry::setDatefrom(const QDateTime &value)
{
    datefrom = value;
}

QDateTime Entry::getDateto() const
{
    return dateto;
}

void Entry::setDateto(const QDateTime &value)
{
    dateto = value;
}

QString Entry::getDescription() const
{
    return description;
}

void Entry::setDescription(const QString &value)
{
    description = value;
}

QString Entry::getSection() const
{
    return section;
}

void Entry::setSection(const QString &value)
{
    section = value;
}
