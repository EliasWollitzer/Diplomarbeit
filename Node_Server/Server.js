var express = require('express');
var app = express();
var path = require('path');
var mysql = require('mysql');

var bodyParser = require("body-parser");

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
    extended: true
  })); 
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