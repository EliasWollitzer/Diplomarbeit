#ifndef ENTRY_H
#define ENTRY_H

#include <mainwindow.h>
#include <QObject>

class Entry : public QObject
{
    Q_OBJECT
public:
    Entry();

    QString getFname() const;
    void setFname(const QString &value);

    QString getLname() const;
    void setLname(const QString &value);

    QString getRessource() const;
    void setRessource(const QString &value);

    QDate getDate() const;
    void setDate(const QDate &value);

    QString toString();

signals:

public slots:



private:
    QString fname;
    QString lname;
    QString ressource;
    QDate date;
};

#endif // ENTRY_H
