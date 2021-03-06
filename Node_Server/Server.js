var express = require("express");
var mysql = require("mysql");
var path = require("path");
var isempty = require('is-empty');
var bodyParser = require("body-parser");
var expressValidator = require("express-validator");
var app = express();
//--------------------------------------------Parser
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));


app.use(function (req, res, next) { // Test
    console.log('Time:', Date.now());
    next();
});

app.get('/hello', function (req, res) {// pfad localhost:8080/hello
    res.send("Hello World");
});
//-----------------------------------------DELETE methods
app.post("/sql_delete", function (req, res) {
    console.log("DELETE req: " + JSON.stringify(req.body));

    var Entid;
    selectEntid_SQL(req.body, function (err, cbEntid) {
        if (err) throw console.log("Select Entid ERROR" + err);
        console.log("CALLBACK Entid in DELETE: " + cbEntid)
        Entid = cbEntid;
        if (Entid == "EMPTY") {
            res.send("ERROR: entry not found")
        } else {
            query = "DELETE FROM Borrowed " +
                "WHERE Entid = '" + Entid + "';";

            con.query(query, function (err, result) {
                if (err) throw err;
                console.log("SQL result DELETE: " + result);
                console.log("DELETE DONE")
                res.send("DELETE DONE")
            });
        }
    });
});

//-------------------------------------------SQL_Get von Client nach Datum -> ganze Liste
app.get('/sql_get', function (req, res) {
    console.log("Get String: " + JSON.stringify(req.query.date));
    var date = req.query.date;
    console.log("searched Date" + date);

    var sqlquery = "SELECT * FROM Persons as Per JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep " +
        "on Bor.Pid = Per.Pid and Bor.Rid = Res.Rid and Dep.Did = Per.Did " +
        "WHERE ('" + date + "'>= DATE(datefrom)) AND ('" + date + "'<=DATE(dateto));"
    // return table
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent" + result);
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        console.log(y);
        res.send(y)
    });
});
//----------------------------------------SQL GET Table
app.get('/sql_get/department', function (req, res) {

    var sqlquery = "SELECT section FROM Department";

    // return table
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent" + result);
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        console.log(y);
        res.send(y)
    });
});

app.get('/sql_get/persons', function (req, res) {

    var sqlquery = "select p.firstName, p.lastName, d.section from Persons as p join Department as d on p.Did = d.Did";

    // return table
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent" + result);
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        console.log(y);
        res.send(y)
    });
});

app.get('/sql_get/resources', function (req, res) {

    var sqlquery = "SELECT * FROM Resources";

    // return table
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent" + result);
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        console.log(y);
        res.send(y)
    });
});
//----------------------------------------SQL_Post von Client
app.post("/sql_post/entry", function (req, res) {
    var Pid;
    var Rid;
    var Did;
    console.log("POST ENTRY req: " + JSON.stringify(req.body));

    isTimeareaFree(req.body, function (err, isFree) {
        if (isFree == true) {
            console.log("isFree: keine reservierungen vorhanden!")
        } else {
            let datefrom = isFree[0].datefrom;
            let dateto = isFree[0].dateto;
            console.log("isFree: bereits reserviert!: "+datefrom+"-"+dateto);
            res.send(isFree);
            return false;
        }

        isEntryDouble(req.body, function (err, isDouble) {

            if (isDouble == true) {
                console.log("Entry already exists!!")
                res.send("POST ENTRY FAILED")
                return false;
            }
            else {
                console.log("New Entry!!")
            }

            isDepartmentDouble(req.body, function (err, cbDid) { //if double + sqlinsert + return ID
                if (err) throw console.log("isDespartmentDouble failed: " + err)
                console.log("isDepartmentDouble: " + cbDid)
                Did = cbDid //Department ID zum Client

                isResourceDouble(req.body, function (err, cbRid) { //if double + sqlinsert + return ID
                    if (err) throw console.log("isResourceDouble failed: " + err)
                    console.log("isResourceDouble: " + cbRid)
                    Rid = cbRid //Resources ID zum Client

                    isPersonDouble(req.body, Did, function (err, cbPid) { //if double + sqlinsert + return ID
                        if (err) throw console.log("isPersonDouble failed: " + err)
                        console.log("isPersonDouble: " + cbPid)
                        Pid = cbPid //Personen ID zum Client

                        insertSQL_Borrowed(req.body, Pid, Rid) // Termin mit Foreign Keys
                        res.send("POST ENTRY DONE")

                    });
                });
            });
        });
    });
});

app.post("/sql_post/persons", function (req, res) { // nur mit department möglich
    var Pid;
    var Did;
    console.log("POST PERSONS req: " + JSON.stringify(req.body));

    selectDid_SQL(req.body, function (err, cbDid) {
        if (err) throw console.log("Select DID ERROR" + err);
        console.log("CALLBACK DID in Insert Persons: " + cbDid)
        Did = cbDid

        isPersonDouble(req.body, Did, function (err, cbPid) { //if double + sqlinsert
            if (err) throw console.log("isPersonDouble failed: " + err)
            console.log("isPersonDouble: " + cbPid)
            Pid = cbPid
            res.send("Insert Done") //Personen ID zum Client
        });
    });
});

app.post("/sql_post/borrowed", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Borrowed(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/resources", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Resources(req.body, function(){
        res.send("Insert Done");
    });
    
});
app.post("/sql_post/department", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Department(req.body, function(){
        res.send("Insert Done");
    });
    
});

app.post("/sql_post/delete_data", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    deleteSQL_all(req.body, function(){
        res.send("All deleted")
    })
});
//-----------------------------------------PATCH von Client: SQL_Update
app.post("/sql_update/borrowed", function (req, res) {
    var Entid
    console.log("PATCH req: " + JSON.stringify(req.body));

    selectEntid_SQL(req.body, function (err, cbEntid) {//Vergleich mit bestehenden einträgen -> Entid
        if (err) throw console.log("Select Entid ERROR" + err);
        console.log("CALLBACK Entid in Update borrowed: " + cbEntid)
        Entid = cbEntid;
        if (Entid == 'EMPTY') {
            res.send("UPDATE failed: entry not found")
        } else {
            isTimeareaFreeForUpdate(req, function(cb){
                console.log(JSON.stringify(cb))
                if(cb){
                    updateSQL_Borrowed(req.body, Entid);
                    res.send("UPDATE done");
                }else{
                    res.send("Time collision")
                }

            })
        }

    });
});

//----------------------------------------- SQL_Connection
var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "root",
    database: "data",
    dateStrings: true
});

con.connect(function (err) {
    if (err) throw err;
    console.log("Connected!")
});
//------------------------------------------select methods: freien Termin suchen
var isTimeareaFree = function (req, callback) { // true -> bereich ist frei | false -> bereich bereits reserviert
    var query;
    var datefrom;
    var dateto;

    query = "SELECT COUNT(Entid) as anz FROM Persons as Per JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep " +
        "on Bor.Pid = Per.Pid and Bor.Rid = Res.Rid and Dep.Did = Per.Did " +
        "WHERE not((Res.resource != '"+ JSON.stringify(req.resource).slice(1, -1) + "') OR (('" + JSON.stringify(req.dateto).slice(1, -1) + "') <= datefrom) OR (dateto <= ('" + JSON.stringify(req.datefrom).slice(1, -1) + "')));";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL COUNT(datefrom): " + result);
        var x = JSON.stringify(result);
        var y = JSON.parse(x);
        var z = y[0].anz
        console.log("isTimeareaFree: SQL result ANZ: " + y[0].anz);

        if ((z) == 0) {//Wenn Zeitbereich frei
            console.log("Zeitbereich ist frei: " + req.datefrom + " " + req.dateto)

            callback(null, true)
            return callback

        } else {//Wenn Zeitbereich reserviert
            query = "SELECT datefrom,dateto FROM Persons as Per JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep " +
                "on Bor.Pid = Per.Pid and Bor.Rid = Res.Rid and Dep.Did = Per.Did " +
                "WHERE not((Res.resource != '"+ JSON.stringify(req.resource).slice(1, -1) + "') OR (('" + JSON.stringify(req.dateto).slice(1, -1) + "') <= datefrom) OR (dateto <= ('" + JSON.stringify(req.datefrom).slice(1, -1) + "')));";

            con.query(query, function (err, result) {
                if (err) throw err;
                console.log("Table Sent" + result);
                var x = JSON.stringify(result)
                var y = JSON.parse(x)
                console.log(y)
                datefrom = y[0].datefrom;
                dateto = y[0].dateto;
                var result = 
                [{"isFree" : "0"},
                {"datefrom": ""+datefrom+""},
                {"dateto":""+dateto+""},
                {"count":y.length}]
                console.log("Termine bereits vorhanden: " + datefrom + " " + dateto)

                callback(null, result)
                return callback
            });
        }
    });
}

var isTimeareaFreeForUpdate = function(req, callback){
    var test = {}
    test.dateto = req.body.datetoNew
    test.datefrom = req.body.datefromNew
    test.resource = req.body.resource
    console.log(JSON.stringify(req.body))
    console.log("test: " + JSON.stringify(test))
    isTimeareaFree(test, function(err, isFree){
        console.log(JSON.stringify(isFree))
        console.log(JSON.stringify(req.body))
        //console.log("TEST: " + req.body.dateto + isFree[2].dateto + req.body.datefrom + isFree[1].datefrom)
        if(isFree == true){
            console.log("isFree")
            callback(true)
        }else{
            if(isFree[3].count == 1){
                if(((req.body.dateto + ":00") == isFree[2].dateto) && ((req.body.datefrom + ":00") == isFree[1].datefrom)){
                    console.log("count is 1")
                    callback(true)
                }else{
                    callback(false)
                }
            }else{
                console.log("mehrere")
                callback(false)
            }  
        }
    })
}
//------------------------------------------select methods: ID suchen
var isEntryDouble = function (req, callback) { // true -> is double | false -> is new
    var query;
    var x;
    var y;
    var z;

    query = "SELECT COUNT(datefrom) as anz FROM Borrowed " +
        "WHERE datefrom = '" + JSON.stringify(req.datefrom).slice(1, -1) + "'" + " and dateto = '" + JSON.stringify(req.dateto).slice(1, -1) + "' and " +
        "description = '" + JSON.stringify(req.description).slice(1, -1) + "';";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL COUNT(datefrom): " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        z = y[0].anz
        console.log("B: SQL result ANZ: " + y[0].anz);


        if ((z) == 0) {//Wenn entry neu
            console.log("Eintrag neu: " + req.firstName + " " + req.lastName)

            callback(null, false)
            return callback

        } else {//Wenn entry doppelt
            console.log("Entry doppelt: " + req.firstName + " " + req.lastName)

            callback(null, true)
            return callback
        }
    });
}

var isPersonDouble = function (req, Did, callback) {
    var query;
    var Pid;
    var x;
    var y;
    var z;
    query = "SELECT COUNT(firstName) as anz FROM Persons " +
        "WHERE firstName = '" + JSON.stringify(req.firstName).slice(1, -1) + "'" + " and lastName = '" + JSON.stringify(req.lastName).slice(1, -1) + "';";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL COUNT(firstName): " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        z = y[0].anz
        console.log("P: SQL result ANZ: " + y[0].anz);

        if ((z) == 0) {//Wenn Name neu
            console.log("Person neu: " + req.firstName + " " + req.lastName)
            console.log("DID in isDouble: " + Did)
            insertSQL_Persons(req, Did);

            selectPid_SQL(req, function (err, cbPid) {
                if (err) throw console.log("Select PID ERROR" + err);;
                console.log("CALLBACK PID in PERSONNEW: " + cbPid);
                Pid = cbPid;
                callback(null, Pid)
                return callback
            });

        } else {//Wenn Name doppelt
            console.log("Person doppelt: " + req.firstName + " " + req.lastName)

            selectPid_SQL(req, function (err, cbPid) {
                if (err) throw console.log("Select PID ERROR" + err);
                console.log("CALLBACK PID in PERSONDOUBLE: " + cbPid)
                Pid = cbPid;
                callback(null, Pid)
                return callback
            });
        }
    });
}
var isResourceDouble = function (req, callback) {
    var query;
    var Rid;
    var x;
    var y;
    var z;
    query = "SELECT COUNT(resource) as anz FROM Resources " +
        "WHERE resource = '" + JSON.stringify(req.resource).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL COUNT(resource): " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        z = y[0].anz
        console.log("R: SQL result ANZ: " + y[0].anz);

        if ((z) == 0) {//Wenn Resource neu
            console.log("Resource neu: " + req.resource)
            insertSQL_Resources(req);
            selectRid_SQL(req, function (err, cbRid) {
                if (err) throw console.log("Select RID ERROR" + err);
                console.log("CALLBACK RID in ResourceNew: " + cbRid)
                Rid = cbRid;
                callback(null, Rid)
            });

        } else {//Wenn Resource doppelt
            console.log("Resource doppelt: " + req.resource)
            selectRid_SQL(req, function (err, cbRid) {
                if (err) throw console.log("Select RID ERROR" + err);
                console.log("CALLBACK RID in ResourceDouble: " + cbRid)
                Rid = cbRid;
                callback(null, Rid)
            });
        }
    });
}
var isDepartmentDouble = function (req, callback) {
    var query;
    var Did;
    var x;
    var y;
    var z;
    query = "SELECT COUNT(section) as anz FROM Department " +
        "WHERE section = '" + JSON.stringify(req.section).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL COUNT(department): " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        z = y[0].anz
        console.log("D:SQL result ANZ: " + y[0].anz);

        if ((z) == 0) {//Wenn Department neu
            console.log("Department neu: " + req.section)
            insertSQL_Department(req);
            selectDid_SQL(req, function (err, cbDid) {
                if (err) throw console.log("Select DID ERROR" + err);
                console.log("CALLBACK DID in DepartmentNew: " + cbDid)
                Did = cbDid;
                callback(null, Did)
            });

        } else {//Wenn Department doppelt
            console.log("Department doppelt: " + req.section)
            selectDid_SQL(req, function (err, cbDid) {
                if (err) throw console.log("Select DID ERROR" + err);
                console.log("CALLBACK DID in DepartmentDouble: " + cbDid)
                Did = cbDid;
                callback(null, Did)
            });
        }
    });
}
//------------------------------------ SELECT ID methods
var selectPid_SQL = function (req, cb) { // return Pid 
    var query;
    var x;
    var y;
    var Pid;
    console.log("SQL_Select_Persons: " + JSON.stringify(req));

    query = "SELECT Pid from Persons " +
        "WHERE firstName = '" + JSON.stringify(req.firstName).slice(1, -1) + "' AND " +
        "lastName = '" + JSON.stringify(req.lastName).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL result: " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        Pid = y[0].Pid
        console.log("SQL Pid: " + y[0].Pid);
        cb(null, Pid);
    });
}
var selectRid_SQL = function (req, cb) { // return Rid
    var query;
    var x;
    var y;
    var Pid;
    console.log("SQL_Select_Resources: " + JSON.stringify(req));

    query = "SELECT Rid from Resources " +
        "WHERE resource = '" + JSON.stringify(req.resource).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL result: " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        Rid = y[0].Rid
        console.log("SQL Rid: " + y[0].Rid);
        cb(null, Rid);
    });
}
var selectDid_SQL = function (req, cb) { // return Rid
    var query;
    var x;
    var y;
    var Did;
    console.log("SQL_Select_Department: " + JSON.stringify(req));

    query = "SELECT Did from Department " +
        "WHERE section = '" + JSON.stringify(req.section).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("SQL result: " + result);
        x = JSON.stringify(result);
        y = JSON.parse(x);
        Did = y[0].Did
        console.log("SQL Did: " + y[0].Did);
        cb(null, Did);
    });
}
var selectEntid_SQL = function (req, cb) { // return Entid 
    var query;
    var x;
    var y;
    var Entid;
    console.log("SQL_Select_Entid: " + JSON.stringify(req));

    query = "SELECT Entid FROM Persons as Per " +
        "JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep" +
        " on Bor.Pid = Per.Pid" +
        " and Bor.Rid = Res.Rid" +
        " and Dep.Did=Per.Did " +
        "WHERE datefrom = '" + JSON.stringify(req.datefrom).slice(1, -1) + "' AND " +
        "dateto = '" + JSON.stringify(req.dateto).slice(1, -1) + "' AND " +
        "resource = '" + JSON.stringify(req.resource).slice(1, -1) + "' AND " +
        "description = '" + JSON.stringify(req.description).slice(1, -1) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err

        if (isempty(result)) {
            console.log("selectEntid: no entry found")
            cb(null, "EMPTY")
            return cb
        } else {
            x = JSON.stringify(result);
            y = JSON.parse(x);
            Entid = y[0].Entid
            console.log("SQL result: " + result);
            console.log("SQL Entid: " + Entid);
            cb(null, Entid);
        }
    });
}
//------------------------------------------patch methods
function updateSQL_Borrowed(req, Entid) { // Nur Termin
    var query;
    console.log("SQL_Update_Borrowed: " + JSON.stringify(req));

    console.log("Entid in Update: " + Entid)
    query = "UPDATE Borrowed" +
        " SET datefrom = '" + JSON.stringify(req.datefromNew).slice(1, -1) + "', " +
        "dateto = '" + JSON.stringify(req.datetoNew).slice(1, -1) + "', " +
        "description = '" + JSON.stringify(req.descriptionNew).slice(1, -1) + "' " +
        "WHERE Entid = '" + JSON.stringify(Entid) + "';"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("Update Datefrom, Dateto, description at ID: " + JSON.stringify(Entid) + " done");
    });
}
//------------------------------------------insert methods
function insertSQL_Persons(req, Did) {
    var query;
    var SQLDid;
    console.log("SQL_insert_Persons: " + JSON.stringify(req));
    if (req.Did == null) {
        SQLDid = Did
        console.log("DID: " + SQLDid)
    } else {
        SQLDid = JSON.stringify(req.Did).slice(1, -1)
        console.log("DIDandere: " + SQLDid)
    }

    query = "INSERT INTO Persons (Did,firstName,lastName)" +
        " VALUES ('" + SQLDid + "','" +
        JSON.stringify(req.firstName).slice(1, -1) + "','" +
        JSON.stringify(req.lastName).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Person: " + JSON.stringify(req.firstName) + " done");
    });
}
function insertSQL_Borrowed(req, Pid, Rid) {
    var query;
    var SQLPid;
    var SQLRid;

    if (req.Pid == null)
        SQLPid = Pid
    else
        SQLPid = JSON.stringify(req.Pid).slice(1, -1)

    if (req.Rid == null)
        SQLRid = Rid
    else
        SQLRid = JSON.stringify(req.Rid).slice(1, -1)

    console.log("SQL_insert_Borrowed: " + JSON.stringify(req));

    query = "INSERT INTO Borrowed (Pid,Rid,description,datefrom,dateto)" +
        " VALUES ('" + SQLPid + "','" +
        SQLRid + "','" +
        JSON.stringify(req.description).slice(1, -1) + "','" +
        JSON.stringify(req.datefrom).slice(1, -1) + "','" +
        JSON.stringify(req.dateto).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Borrow: " + JSON.stringify(req.description) + " done");
    });
}
function insertSQL_Resources(req,cb) {
    var query;
    console.log("SQL_insert_Resources: " + JSON.stringify(req));

    query = "INSERT INTO Resources (resource)" +
        " VALUES ('" + JSON.stringify(req.resource).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Resources: " + JSON.stringify(req.resource) + " done");
        cb()
        return
    });
}
function insertSQL_Department(req,cb) {
    var query;
    console.log("SQL_insert_Department: " + JSON.stringify(req));

    query = "INSERT INTO Department (section)" +
        " VALUES ('" + JSON.stringify(req.section).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Department: " + JSON.stringify(req.section) + " done");
        cb()
        return
    });
}
function deleteSQL_all(req,cb){
    var query
    console.log("SQL_delete_All: " + JSON.stringify(req))
    
    if(req.test == "test"){
        query = "DELETE FROM Borrowed"
        con.query(query, function (err, result) {
            if (err) throw err;
            console.log(err)
            query = "DELETE FROM Department" 
            con.query(query, function (err, result){
                if(err) throw err;
                console.log(err)
                query = "DELETE FROM Persons"
                con.query(query, function (err, result){
                    if(err) throw err;
                    console.log(err)
                    query = "DELETE FROM Resources"
                    con.query(query,function(err, result){
                        if(err) throw err;
                        console.log(err)
                        cb()
                        return
                    })
                })
            })
        });
    }
}
//
app.listen(30000);

