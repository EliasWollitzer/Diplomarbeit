#include "entrymodel.h"


EntryModel* EntryModel::model = nullptr;
QString EntryModel::url = "http://10.8.250.21:30000/";


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

    if(entrylist.isEmpty()){
        //return QVariant();
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
    qDebug("Funktion HTTPPost");
    //HTTP

    QEventLoop eventLoop;
    QUrl localURL(QString(url + "sql_post/entry"));
    QNetworkRequest request(localURL);
    QNetworkAccessManager mgr;

    QUrlQuery qu;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    qu.addQueryItem("firstName", e->getFirstName().toLatin1());
    qu.addQueryItem("lastName", e->getLastName().toLatin1());
    qu.addQueryItem("resource", e->getRessource().toLatin1());
    qu.addQueryItem("section", e->getSection().toLatin1());
    qu.addQueryItem("description", e->getDescription().toLatin1());
    qu.addQueryItem("datefrom", this->dateTimeToString(e->getDatefrom()).toLatin1());
    qu.addQueryItem("dateto", this->dateTimeToString(e->getDateto()).toLatin1());

    curDate = e->getDatefrom().date();

    qDebug()<<"Query:"<< qu.toString();
    QNetworkReply *reply = mgr.post(request, qu.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL(finished(QNetworkReply*)), this, SLOT(http_get_list()));


    eventLoop.exec();
}

void EntryModel::http_add_component(QList<QString> list){
    qDebug("addComponent");

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QUrlQuery qu;


    QString u = "sql_post/";

    switch(list.size()){
        case RESSOURCE:     u += "resources";
                            qu.addQueryItem("resource",list.at(0).toLatin1());
                            break;

        case DEPARTMENT:    u += "department";
                            qu.addQueryItem("section", list.at(0).toLatin1());
                            break;

        case PERSON:        u += "persons";
                            qu.addQueryItem("firstName",list.at(0).toLatin1());
                            qu.addQueryItem("lastName", list.at(1).toLatin1());
                            qu.addQueryItem("department", list.at(3).toLatin1());
                            break;
    }
    QNetworkRequest request(url + u);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    qDebug()<<"Query:"<< qu.toString();

    QNetworkReply *reply = mgr.post(request, qu.toString(QUrl::FullyEncoded).toUtf8());
    qDebug() << "add" << "vorfinished";

    connect(reply, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    eventLoop.exec();

}

void EntryModel::http_get_list(){
/*
    //ADD
    Entry* e = new Entry();
    e->setFirstName("firstname");
    e->setLastName("lastName");
    e->setDatefrom(QDateTime::currentDateTime());
    e->setDateto(QDateTime::currentDateTime());
    e->setDescription("description");
    e->setRessource("ressource");ect: No such signal QNetworkReplyHttpImpl::finished(QNetworkRepl
    e->setSection("section");
    entrylist.append(e);
*/
    QString date = this->dateToString(curDate);

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
    QUrl localURL(QString(url + "sql_get/resources"));
    QNetworkAccessManager mgr;

    QNetworkRequest request(localURL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(request); // GET

    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(reply, SIGNAL (finished()), this , SLOT(init_ressources()));



    eventLoop.exec();
}

void EntryModel::init_ressources(){

    ressourceList.clear();

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();

    qDebug(reply->readAll());
    //reply->deleteLater();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray arr = json.array();

    for(int i = 0; i < arr.size(); i++){
        QJsonObject o = arr.at(i).toObject();
        ressourceList.append(o.value("resource").toString());
    }
}

void EntryModel::init_list(){

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray response_data = reply->readAll();

    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray arr = json.array();

    qDebug() << "array: " << reply->readAll();

    rows = arr.size();
    columns = 3;

    entrylist.clear();

    Entry* e;

    for(int i = 0; i < arr.size(); i++){
        e = new Entry();

        QJsonObject o = arr.at(i).toObject();

        e->setFirstName(o.value("firstName").toString());
        e->setLastName(o.value("lastName").toString());
        e->setRessource(o.value("resource").toString());
        e->setSection(o.value("section").toString());

        QDateTime datefrom = this->stringToDateTime(o.value("datefrom").toString());
        QDateTime dateto = this->stringToDateTime(o.value("dateto").toString());
        e->setDatefrom(datefrom);
        e->setDateto(dateto);

        entrylist.push_back(e);
        //qDebug() << "formArray" << e->toString();
    }

    for(int i = 0; i < entrylist.size(); i++){
        qDebug() << "init_list" << entrylist[i]->toString();
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
    return QString::number(d.year()) + "-" + QString::number(d.month()) + "-" + QString::number(d.day());
}


QString EntryModel::dateTimeToString(QDateTime dt){
    return QString::number(dt.date().year()) + "-" +  QString::number(dt.date().month()) + "-" + QString::number(dt.date().day()) + " " + QString::number(dt.time().hour()) + ":" + QString::number(dt.time().minute()) + ":" + QString::number(dt.time().second());

}
