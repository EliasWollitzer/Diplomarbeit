var express = require("express");
var mysql = require("mysql");
var path = require("path");
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
//-------------------------------------------SQL_Get von Client nach Datum -> ganze Liste
app.get('/sql_get', function (req, res) {
    console.log("Get String: " + JSON.stringify(req.query.date));
    var date = req.query.date;
    var datesp = date.split("-");
    console.log("searched Date" + datesp);

    var sqlquery = "SELECT * FROM Persons as Per JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep " +
        "on Bor.Pid = Per.Pid and Bor.Rid = Res.Rid and Dep.Did = Per.Did " +
        "WHERE '" + datesp[0] + "'= year(Bor.datefrom) AND '" + datesp[1] + "'= month(Bor.datefrom) AND '" + datesp[2] + "'= day(Bor.datefrom)";;

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
//----------------------------------------SQL GET Resource Table
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
app.post("/sql_post/persons", function (req, res) {
    let Pid;
    console.log("POST PERSONS req: " + JSON.stringify(req.body));

    isPersonDouble(req.body, function (err,cbPid) { //if double + sqlinsert
        if (err) throw console.log("isPersonDouble failed: "+err) 
        console.log("isPersonDouble: "+cbPid)
        res.send(""+cbPid) //Personen ID zum Client
    });
});

app.post("/sql_post/borrowed", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Borrowed(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/resources", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Resources(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/department", function (req, res) {
    console.log("POST req: " + JSON.stringify(req.body));

    insertSQL_Department(req.body);
    res.send("Insert Done");
});
//-----------------------------------------PATCH von Client: SQL_Update
app.patch("/sql_update/borrowed", function (req, res) {
    console.log("PATCH req: " + JSON.stringify(req.body));

    updateSQL_Borrowed(req.body);//!!!!!!!!!!!!!!!!description
    res.send("Update Done");
});
//----------------------------------------- SQL_Connection
var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "root",
    database: "data"
});

con.connect(function (err) {
    if (err) throw err;
    console.log("Connected!")
});
//------------------------------------------select methods: ID suchen

var isPersonDouble = function (req, callback) { //wenn Person doppelt = 1
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
        console.log("SQL result ANZ: " + y[0].anz);

        if ((z) == 0) {//Wenn Name neu
            console.log("Person neu: " + req.firstName + " " + req.lastName)
            insertSQL_Persons(req);

            selectPid_SQL(req, function (err, cbPid) {
                if (err) throw console.log("Select PID ERROR" + err);;
                console.log("CALLBACK PID in PERSONNEW: " + cbPid);
                Pid = cbPid;
                callback(null, Pid)
            });

        } else {//Wenn Name doppelt
            console.log("Person doppelt: " + req.firstName + " " + req.lastName)

            selectPid_SQL(req, function (err, cbPid) {
                if (err) throw console.log("Select PID ERROR" + err);
                console.log("CALLBACK PID in PERSONDOUBLE: " + cbPid)
                Pid = cbPid;
                callback(null, Pid)
            });
        }
    });
}

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
//------------------------------------------patch methods
function updateSQL_Borrowed(req) { // Nur Termin
    var query;
    console.log("SQL_Update_Borrowed: " + JSON.stringify(req));

    query = "UPDATE Borrowed" +
        " SET datefrom = '" + JSON.stringify(req.datefrom).slice(1, -1) + "', " +
        "dateto = '" + JSON.stringify(req.dateto).slice(1, -1) + "', " +
        "description = '" + JSON.stringify(req.description).slice(1, -1) + "' " +
        "WHERE Entid = " + JSON.stringify(req.Entid).slice(1, -1) + ";"

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("Update Datefrom, Dateto, description at ID: " + JSON.stringify(req.Entid) + " done");
    });
}
//------------------------------------------insert methods
function insertSQL_Persons(req) {
    var query;
    console.log("SQL_insert_Persons: " + JSON.stringify(req));

    query = "INSERT INTO Persons (Did,firstName,lastName)" +
        " VALUES ('" + JSON.stringify(req.Did).slice(1, -1) + "','" +
        JSON.stringify(req.firstName).slice(1, -1) + "','" +
        JSON.stringify(req.lastName).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Person: " + JSON.stringify(req.firstName) + " done");
    });
}
function insertSQL_Borrowed(req) {
    var query;
    console.log("SQL_insert_Borrowed: " + JSON.stringify(req));

    query = "INSERT INTO Borrowed (Pid,Rid,description,datefrom,dateto)" +
        " VALUES ('" + JSON.stringify(req.Pid).slice(1, -1) + "','" +
        JSON.stringify(req.Rid).slice(1, -1) + "','" +
        JSON.stringify(req.description).slice(1, -1) + "','" +
        JSON.stringify(req.datefrom).slice(1, -1) + "','" +
        JSON.stringify(req.dateto).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Borrow: " + JSON.stringify(req.description) + " done");
    });
}
function insertSQL_Resources(req) {
    var query;
    console.log("SQL_insert_Resources: " + JSON.stringify(req));

    query = "INSERT INTO Resources (resource)" +
        " VALUES ('" + JSON.stringify(req.resource).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Resources: " + JSON.stringify(req.resource) + " done");
    });
}
function insertSQL_Department(req) {
    var query;
    console.log("SQL_insert_Department: " + JSON.stringify(req));

    query = "INSERT INTO Department (section)" +
        " VALUES ('" + JSON.stringify(req.section).slice(1, -1) + "')";

    console.log("SQLQuery: " + query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Department: " + JSON.stringify(req.section) + " done");
    });
}
//
app.listen(30000);

