#ifndef ENTRY_H
#define ENTRY_H

#define NEWENTRY 0
#define ADD 1


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
    QDateTime datefrom;
    QDateTime dateto;

};

#endif // ENTRY_H
