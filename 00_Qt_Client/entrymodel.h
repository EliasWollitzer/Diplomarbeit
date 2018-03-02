#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H

#include <QObject>
#include <QModelIndex>
#include <QAbstractTableModel>
#include <QTableView>
#include <QJsonArray>
#include <QList>
#include <entry.h>

#define RESSOURCE 1
#define DEPARTMENT 2
#define PERSON 3


class EntryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    static EntryModel* getInstance();

    //TableModel functions
    int rows;
    int columns;
    static QString url;
    QList <QString> ressourceList;
    QDate curDate;


    int rowCount(const QModelIndex &parent = QModelIndex()) const ;     //Pushes rows to the model
    int columnCount(const QModelIndex &parent = QModelIndex()) const;   //Pushes columns to the model

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //Sets data in the model
    QVariant headerData(int section, Qt::Orientation orientation, int role) const ; //Sets column names


    void http_get_list();   //Sends a GET request with the date to the server
    void http_get_ressources();
    void http_post_entry(Entry* e);
    void http_add_component(QList <QString> list);
    void http_update_entry(Entry* e, Entry* oldEntry);
    void http_delete_entry(Entry* e);

    QDateTime stringToDateTime(QString sdate);
    QString dateToString(QDate d);
    QString dateTimeToString(QDateTime dt);

    Entry* getEntryAt(int index);

private:

    explicit EntryModel(QObject *parent = nullptr); //Private constructor for singleton
    static EntryModel* model;   //Singleton

    QList <Entry*> entrylist;   //EntryList

    QString getColumnName(int section) const;

signals:

private slots:
    void init_list();   //Converts JSON to List of Entrys
    void init_ressources();
    void update_successful();
    void delete_successful();
};

#endif // ENTRYMODEL_H
