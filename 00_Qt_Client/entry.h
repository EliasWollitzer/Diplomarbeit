#ifndef ENTRY_H
#define ENTRY_H

#include <mainwindow.h>
#include <QObject>

class Entry : public QObject
{
    Q_OBJECT
public:
    Entry();

signals:

public slots:
    QString get_fname();
    void set_fname(QString &value);

    QString get_lname();
    void set_lname(QString &value);

    QString get_resource();
    void set_resource(QString &value);

    QDate get_date();
    void set_date(QDate &value);


private:
    QString fname;
    QString lname;
    QString resoure;
    QDate date;
};

#endif // ENTRY_H
