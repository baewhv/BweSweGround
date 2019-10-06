var http = require('http');
var express = require('express');
var bodyParser = require('body-parser');
var cookieParser = require('cookie-parser');


function setRouter(app, rule){
    //배열 루프
    for(var ix in rule){
        if (rule[ix].method === 'GET'){
            app.get(rule[ix].path, rule[ix].callback);
        } else if (rule[ix].method === 'POST'){
            app.post(rule[ix].path, rule[ix].callback);
        } else if (rule[ix].method === 'PUT'){
            app.put(rule[ix].path, rule[ix].callback);
        } else if (rule[ix].method === 'DELETE'){
            app.delete(rule[ix].path, rule[ix].callback);
        }
    }
}

function serverStart(rule){
    var app = express();
    app.use(bodyParser({extended : false}));
    app.use(cookieParser());

    setRouter(app, rule);
    http.createServer(app).listen(3000);
    console.log('서버 실행 중');
}

module.exports = {
    serverStart : serverStart
}