#include "entrymodel.h"


EntryModel* EntryModel::model = nullptr;
QString EntryModel::url = "http://10.8.250.22:30000/";


EntryModel::EntryModel(QObject *parent) : QAbstractTableModel(parent), entrylist()
{

}

EntryModel* EntryModel::getInstance(){
    if(model == nullptr){
        model = new EntryModel();
    }
    return model;
}

int EntryModel::rowCount(const QModelIndex & /*parent*/) const
{
   return rows;
}

int EntryModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns;
}

QVariant EntryModel::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if (role != Qt::DisplayRole)
    {
       return QVariant();
    }

    switch(c){
        case 0: return entrylist.at(r)->getRessource();
                break;
        case 1: return entrylist.at(r)->getDatefrom().time().toString();
                break;
        case 2: return entrylist.at(r)->getDateto().time().toString();
                break;

    }

    return "";
}

QVariant EntryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal){
        return QVariant();
    }

    return this->getColumnName(section);
}



QString EntryModel::getColumnName(int section) const{

    switch (section) {
    case 0:
        return "Ressource";
    case 1:
        return "From";
    case 2:
        return "To";
    }
    return "";
}

void EntryModel::http_post_entry(Entry* e){

}

void EntryModel::http_get_list(QString date){

    QEventLoop eventLoop;
    QUrl localURL(QString(url + "sql_get"));
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    qu.addQueryItem("date", date.toLatin1());
    qDebug() << "http_get_list: " << qu.toString();

    localURL.setQuery(qu);
    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET



    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL (finished()), this , SLOT(init_list()));

    eventLoop.exec();
}

void EntryModel::http_get_ressources(){

    QEventLoop eventLoop;
    QUrl localURL(QString(url + "get_ressources"));
    QNetworkAccessManager mgr;

    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET



    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL (finished()), this , SLOT(init_ressources()));

    eventLoop.exec();
}

void EntryModel::init_ressources(){

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray arr = json.array();

    for(int i = 0; i < arr.size(); i++){
        QJsonObject o = arr.at(i).toObject();
        ressourceList.append(o.value("").toString());        //ERROR
    }
}

void EntryModel::init_list(){

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray arr = json.array();

    //qDebug() << "array: " << arr;

    rows = arr.size();
    columns = 3;


    qDebug() << &entrylist << endl;

    entrylist.clear();

    for(int i = 0; i < arr.size(); i++){
        Entry* e = new Entry();

        QJsonObject o = arr.at(i).toObject();

        e->setFirstName(o.value("firstName").toString());
        e->setLastName(o.value("lastName").toString());
        e->setRessource(o.value("resource").toString());
        e->setSection(o.value("section").toString());
        e->setEntlID(o.value("Entlid").toString());

        QDateTime datefrom = this->stringToDateTime(o.value("datefrom").toString());
        QDateTime dateto = this->stringToDateTime(o.value("dateto").toString());
        e->setDatefrom(datefrom);
        e->setDateto(dateto);

        entrylist.push_back(e);
        //qDebug() << "formArray" << e->toString();
    }

    for(int i = 0; i < entrylist.size(); i++){
        qDebug() << "formArray" << entrylist[i]->toString();
    }

}

QDateTime EntryModel::stringToDateTime(QString sdate){

    int y =  sdate.mid(0,4).toInt();
    int m = sdate.mid(5,2).toInt();
    int d = sdate.mid(8,2).toInt();
    int h = sdate.mid(11,2).toInt();
    int min = sdate.mid(14,2).toInt();
    int s = sdate.mid(17,2).toInt();

    QDateTime* date = new QDateTime();
    QDate da;
    da.setDate(y,m,d);
    QTime* time = new QTime(h,min,s,0);
    date->setDate(da);
    date->setTime(*time);

    return *date;
}

QString EntryModel::dateToString(QDate d){
    return "" + d.day() + d.month() + d.year();
}
