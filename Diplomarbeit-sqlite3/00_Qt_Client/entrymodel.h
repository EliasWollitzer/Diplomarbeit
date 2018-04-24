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
    static EntryModel* getInstance();

    //TableModel functions
    int rows;
    int columns;

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;     //Pushes rows to the model
    int columnCount(const QModelIndex &parent = QModelIndex()) const;   //Pushes columns to the model

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //Sets data in the model
    QVariant headerData(int section, Qt::Orientation orientation, int role) const ; //Sets column names


    void http_get_list(QString date);   //Sends a GET request with the date to the server


private:

    explicit EntryModel(QObject *parent = nullptr); //Private constructor for singleton
    static EntryModel* model;   //Singleton

    QList <Entry*> entrylist;   //EntryList

    QString getColumnName(int section) const;

signals:

private slots:
    void init_list();   //Converts JSON to List of Entrys
};

#endif // ENTRYMODEL_H
