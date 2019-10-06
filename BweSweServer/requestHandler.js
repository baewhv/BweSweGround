var mysqlDB;

function connectDB(database){
    mysqlDB = database;
}
function register(req, res){
    var data = {
        userid : req.body.userid,
        nickname : req.body.nickname,
        password : req.body.password
    };
    console.log(data);
    mysqlDB.register(res, data);
};

function login(req, res){
    var data = {
        userid : req.body.userid,
        password : req.body.password,
    };
    mysqlDB.login(res, data);
};


module.exports = {
    connectDB : connectDB,
    register : register,
    login : login,
}