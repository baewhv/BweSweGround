var mysql = require('mysql');

// 클라이언트 정보 설정.

var client = mysql.createConnection({
    host : 'localhost',
    user : 'root',
    password : 'qwer1234',
    database : 'userdb'
});

//연결.
client.connect(function(error){
    if(error){
        console.log(error);
    }
});

function login(res, data){
    var query = 'select * from userdb.userinfo where userid=? and password=?';
    
    client.query(query, [data.userid, data.password], function(error, result, fields){
        
        if(error){
            res.send('쿼리 검색 실패 : ' + error);
        } else{
            if(result.length > 0){
                var Message = {
                    result : "success",
                    id : result[0].userid,
                    password : result[0].password,
                    nickname : result[0].nickname
                };
                console.log(Message);
                res.send(Message);
            }else{
                res.send('로그인 실패!!!');
            }

        }
        }
    );
}

function register(res, data){
    var query = 'select * from userdb.userinfo where userid=?';
    client.query(query, data.userid, function(error, results){
            if(results.length > 0){
                console.log('이미 사용중인 ID입니다.');
                var Message = {
                    result : "false",
                    message : "이미 사용중인 ID입니다.",
                };
                res.send(Message);
            } else{
                query = 'insert into userdb.userinfo set?'
                client.query(query, data,function(error, results){
                        if(error)
                        {
                            var Message = {
                                result : "false",
                                message : "실패",
                                error : error
                            };
                            res.send(Message);
                        }else{
                            var Message = {
                                result : "true",
                                message : "성공!",
                            };
                            res.send(Message);
                        }
                    }
                )
            }
    });
}

module.exports = {
    login : login,
    register : register,
};