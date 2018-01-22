#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H

#include <QObject>
#include <QModelIndex>
#include <QAbstractTableModel>
#include <QTableView>
#include <QJsonArray>
#include <QList>
#include <entry.h>

class EntryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EntryModel(QObject *parent = nullptr);

    static EntryModel* getInstance();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QTableView *tableview;
    void setTableConditions(QTableView *tv, QJsonArray arr);
    void formArray(QJsonArray array);

    //QList <Entry*> entrylist;

private:
    int rows;
    int columns;
    //QJsonArray array;
    QList <Entry*> entrylist;

    static EntryModel* model;

signals:

public slots:
};

#endif // ENTRYMODEL_H
