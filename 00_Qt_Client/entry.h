#ifndef ENTRY_H
#define ENTRY_H

#include <mainwindow.h>


class Entry : public QObject
{
    Q_OBJECT
public:
    Entry();



    QString toString();

    QString getFirstName() const;
    void setFirstName(const QString &value);

    QString getLastName() const;
    void setLastName(const QString &value);

    QString getRessource() const;
    void setRessource(const QString &value);

    QDateTime getDatefrom() const;
    void setDatefrom(const QDateTime &value);

    QDateTime getDateto() const;
    void setDateto(const QDateTime &value);

    QString getEntlID() const;
    void setEntlID(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    QString getSection() const;
    void setSection(const QString &value);

signals:

public slots:



private:
    QString firstName;
    QString lastName;
    QString ressource;
    QString section;
    QString description;
    QString entlID;
    QDateTime datefrom;
    QDateTime dateto;

};

#endif // ENTRY_H
