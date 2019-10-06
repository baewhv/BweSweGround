var server = require('./server');
var handler = require('./requestHandler');
var database = require('./database');

function RuleCreator(path, method, callback){
    this.path = path;
    this.method = method;
    this.callback = callback;
}

var rule = [
    new RuleCreator('/register', 'POST', handler.register),
    new RuleCreator('/login', 'POST', handler.login),
]
handler.connectDB(database);
server.serverStart(rule);