var http = require('http');
var express = require('express');
var bodyParser = require('body-parser');
var cookieParser = require('cookie-parser');


function setRouter(app, handle){
    for (var ix in handle){
        switch(handle[ix].method){
            case 'get':
            app.get(handle[ix].path, handle[ix].callback);
            break;
            case 'post':
            app.post(handle[ix].path, handle[ix].callback);
            break;
            case 'put':
            app.put(handle[ix].path, handle[ix].callback);
            break;
            case 'delete':
            app.delete(handle[ix].path, handle[ix].callback);
            break;
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
    setRouter : setRouter,
    serverStart : serverStart
}