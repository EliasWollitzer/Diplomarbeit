#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H

#include <QObject>

class EntryModel : public QObject
{
    Q_OBJECT
public:
    explicit EntryModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ENTRYMODEL_H