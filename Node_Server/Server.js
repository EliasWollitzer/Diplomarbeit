var express = require("express");
var mysql = require("mysql");
var path = require("path");
var bodyParser = require("body-parser");
var expressValidator = require("express-validator");
var urlencodedParser = bodyParser.urlencoded({extendet: true});
var app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
    extended: true
  }));
   
  app.use(function (req, res, next) { // Test
    console.log('Time:', Date.now());
    next();
  });

app.get('/hello', function(req, res){// pfad localhost:8080/hello
    res.send("Hello World");
});
//SQL_Get von Client
app.get('/sql_get',function(req, res){
    console.log("Get String: "+JSON.stringify(req.query));

    var sqlquery = "SELECT * FROM serverTable";
    
    con.query(sqlquery, function (err, result) {
        if (err) throw err;
        console.log("Table Sent");
        var x = JSON.stringify(result)
        var y = JSON.parse(x)
        res.send(y)
    });

});
app.listen(30000);
// SQL_Connection
var con = mysql.createConnection({
    host:"localhost",
    user:"root",
    password:"root",
    database:"data"
});

con.connect(function(err){
    if(err) throw err;
    console.log("Connected!")
});

