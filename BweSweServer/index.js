var server = require('./server');
var handler = require('./requestHandler');
var database = require('./database');

function RuleCreator(path, method, callback){
    this.path = path;
    this.method = method;
    this.callback = callback;
}

var rule = [
    new RuleCreator('/', 'GET', handler.getTest),
    new RuleCreator('/', 'POST', handler.postTest)
]

handler.connectDB(database);

server.serverStart(rule);