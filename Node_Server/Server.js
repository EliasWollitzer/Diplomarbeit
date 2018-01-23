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
//-------------------------------------------SQL_Get von Client
app.get('/sql_get', function (req, res) {
    console.log("Get String: " + JSON.stringify(req.query.date));
    var date = req.query.date;
    var datesp = date.split("-");
    console.log("searched Date"+ datesp);

    var sqlquery = "SELECT firstName FROM Persons as Per JOIN Borrowed as Bor JOIN Resources AS Res JOIN Department as Dep "+
    "on Bor.Pid = Per.Pid and Bor.Rid = Res.Rid and Dep.Did = Per.Did "+
    "WHERE '"+datesp[0]+"'= year(Bor.datefrom) AND '"+datesp[1]+"'= month(Bor.datefrom) AND '"+datesp[2]+"'= day(Bor.datefrom)";;
    
// return table
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent"+result);
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        res.send(y)
    });
});
//----------------------------------------SQL_Post von Client
app.post("/sql_post/persons", function (req, res) {
    console.log("POST req: "+JSON.stringify(req.body));

    insertSQL_Persons(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/borrowed", function (req, res) {
    console.log("POST req: "+JSON.stringify(req.body));

    insertSQL_Borrowed(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/resources", function (req, res) {
    console.log("POST req: "+JSON.stringify(req.body));

    insertSQL_Resources(req.body);
    res.send("Insert Done");
});
app.post("/sql_post/department", function (req, res) {
    console.log("POST req: "+JSON.stringify(req.body));

    insertSQL_Department(req.body);
    res.send("Insert Done");
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
//------------------------------------------insert methods
function insertSQL_Persons(req) {
    var query;
    console.log("SQL_insert_Persons: " + JSON.stringify(req));

    query = "INSERT INTO Persons (Did,firstName,lastName)"+
    " VALUES ('"+JSON.stringify(req.Did).slice(1, -1)+"','" +
                JSON.stringify(req.firstName).slice(1, -1) + "','" +
                JSON.stringify(req.lastName).slice(1, -1) + "')";

    console.log("SQLQuery: "+query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Person: "+JSON.stringify(req.firstName)+" done");
    });
}
function insertSQL_Borrowed(req) {
    var query;
    console.log("SQL_insert_Borrowed: " + JSON.stringify(req));

    query = "INSERT INTO Borrowed (Pid,Rid,description,datefrom,dateto)"+
    " VALUES ('"+JSON.stringify(req.Pid).slice(1, -1)+"','"+
                JSON.stringify(req.Rid).slice(1, -1)+ "','"+
                JSON.stringify(req.description).slice(1, -1) + "','"+
                JSON.stringify(req.datefrom).slice(1, -1)+"','"+
                JSON.stringify(req.dateto).slice(1, -1)+"')";

    console.log("SQLQuery: "+query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Borrow: "+JSON.stringify(req.description)+" done");
    });
}
function insertSQL_Resources(req) {
    var query;
    console.log("SQL_insert_Resources: " + JSON.stringify(req));

    query = "INSERT INTO Resources (resource)"+
    " VALUES ('"+JSON.stringify(req.resource).slice(1, -1) + "')";

    console.log("SQLQuery: "+query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Resources: "+JSON.stringify(req.resource)+" done");
    });
}
function insertSQL_Department(req) {
    var query;
    console.log("SQL_insert_Department: " + JSON.stringify(req));

    query = "INSERT INTO Department (section)"+
    " VALUES ('"+JSON.stringify(req.section).slice(1, -1) + "')";

    console.log("SQLQuery: "+query);
    con.query(query, function (err, result) {
        if (err) throw err;
        console.log("INSERT Department: "+JSON.stringify(req.section)+" done");
    });
}
//
app.listen(30000);

