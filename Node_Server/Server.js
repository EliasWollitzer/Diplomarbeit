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
/*
var con = mysql.createConnection({
    host:'localhost',
    user:'root',
    password:'root',
    database:'new_schema'
});

con.connect(function(err) {
    if (err) throw err;
    console.log("connectet");
});
*/

app.listen(30000);