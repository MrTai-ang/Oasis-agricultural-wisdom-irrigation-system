var express = require('express');//引入express模块
var app = express();
// 当前时间包
const sd = require('silly-datetime');

//解决跨域问题
const cors=require('cors');





const mqtt = require('mqtt')
//存储订阅的消息
var message={}

// 第一步：引入mysql模块
let mysql = require("mysql");

// 连接参数
const db_config={
    host     : 'localhost', // 主机地址
    user     : 'root',      // 用户名
    password : '123456',    // 用户密码
    database : 'nodejs',    // 数据库名
    port     : 3306,         // 端口号，默认3306 
    timezone : 'utc' // 解决时区时间统一的问题
}
var temp = new Array()

// 第二步：创建数据库连接池，这里参数会很多
let pool=mysql.createPool(db_config)


// 第三步：正式发起数据库连接
pool.getConnection(function(err,connect){//通过getConnection()方法进行数据库连接
    if(err){
        console.log(`mysql链接失败${err}`);
    }else{
    }
})



const host = '47.113.146.63'
const port = '1883'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const connectUrl = `mqtt://${host}:${port}`
const client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  username: 'emqx',
  password: 'public',
  reconnectPeriod: 1000,
})


//订阅主题
// const subtopic = 'inTopic'
const subtopic = 'device/date'
// 订阅主题土壤数据
const subtopic2 = 'device/date2'
// 订阅主题NPK数据
const subtopic3 = 'device/date3'
// 订阅主题不同土壤湿度传感器的数据
const subtopic4 = 'device/date4'
//发布主题
const pubtopic = 'outTopic'
client.on('connect', () => {
  console.log('Connected')
  client.subscribe([subtopic], () => {
    console.log(`Subscribe to topic '${subtopic}'`)
  })
  client.subscribe([subtopic2], () => {
    console.log(`Subscribe to topic '${subtopic2}'`)
  })
  client.subscribe([subtopic3], () => {
    console.log(`Subscribe to topic '${subtopic3}'`)
  })
  client.subscribe([subtopic4], () => {
    console.log(`Subscribe to topic '${subtopic4}'`)
  })
  client.publish(pubtopic, 'nodejs mqtt test', { qos: 0, retain: false }, (error) => {
    if (error) {
      console.error(error)
    }
  })
})



client.on('message', (topic, payload) => {

    if(topic==subtopic)
{
  console.log('Received Message:', subtopic, payload.toString())

 
   //处理json数据
   var obj=JSON.parse(payload.toString())
   console.log('处理的数据空气：',obj.msg1,obj.msg2,obj.msg3)
   var humidity=obj.msg1
   var temperature=obj.msg2
   var light=obj.msg3
   //时间
   const time = sd.format(new Date(), 'YYYY-MM-DD HH:mm');
   console.log('time:',time);
 
// 第三步：正式发起数据库连接
pool.getConnection(function(err,connect){//通过getConnection()方法进行数据库连接
    if(err){
        console.log(`mysql链接失败${err}`);
    }else{
        let insertSql2 = `insert into kongqi(humidity,temperature,light,time) values ('${humidity}','${temperature}','${light}','${time}')`
        console.log(insertSql2)
        connect.query(insertSql2,function(err,result){
            if(err){
                console.log(`SQL error:${err}`)
            }else{
                console.log('插入成功');
                //打印数据库里的数据,测试用
                console.log(result);
                connect.release();//释放连接池中的数据库连接
            }
        });
    }
  })

}
else if(topic==subtopic2)
{
    console.log('Received Message:', subtopic2, payload.toString())
    //土壤温度、点导率、含盐量
    var obj=JSON.parse(payload.toString())
    console.log('处理的数据土壤数据：',obj.msg1,obj.msg2,obj.msg3)
    var temp=obj.msg1
    var electricity=obj.msg2
    var salt=obj.msg3
    // 第三步：正式发起数据库连接
pool.getConnection(function(err,connect){//通过getConnection()方法进行数据库连接
    if(err){
        console.log(`mysql链接失败${err}`);
    }else{
        let insertSql2 = `insert into soil(temp,electricity,salt) values ('${temp}','${electricity}','${salt}')`
        console.log(insertSql2)
        connect.query(insertSql2,function(err,result){
            if(err){
                console.log(`SQL error:${err}`)
            }else{
                console.log('插入成功');
                //打印数据库里的数据,测试用
                console.log(result);
                connect.release();//释放连接池中的数据库连接
            }
        });
    }
  })
}
else if(topic==subtopic3)
{
    console.log('Received Message:', topic, payload.toString())
    //N\P\K
    var obj=JSON.parse(payload.toString())
    console.log('处理的数据土壤数据：',obj.msg1,obj.msg2,obj.msg3)
    var N=obj.msg1
    var P=obj.msg2
    var K=obj.msg3
    // 第三步：正式发起数据库连接
pool.getConnection(function(err,connect){//通过getConnection()方法进行数据库连接
    if(err){
        console.log(`mysql链接失败${err}`);
    }else{
        let insertSql2 = `insert into soilnpk(N,P,K) values ('${N}','${P}','${K}')`
        console.log(insertSql2)
        connect.query(insertSql2,function(err,result){
            if(err){
                console.log(`SQL error:${err}`)
            }else{
                console.log('插入成功');
                //打印数据库里的数据,测试用
                console.log(result);
                connect.release();//释放连接池中的数据库连接
            }
        });
    }
  })
}
else if(topic==subtopic4)
{
    console.log('Received Message:', topic, payload.toString())
    //土壤湿度1、土壤湿度2、ph
    var obj=JSON.parse(payload.toString())
    console.log('处理的数据土壤数据：',obj.msg1,obj.msg2,obj.msg3)
    var hum1=obj.msg1
    var hum2=obj.msg2
    var ph=obj.msg3
    // 第三步：正式发起数据库连接
pool.getConnection(function(err,connect){//通过getConnection()方法进行数据库连接
    if(err){
        console.log(`mysql链接失败${err}`);
    }else{
        let insertSql2 = `insert into soilhum(hum1,hum2,ph) values ('${hum1}','${hum2}','${ph}')`
        console.log(insertSql2)
        connect.query(insertSql2,function(err,result){
            if(err){
                console.log(`SQL error:${err}`)
            }else{
                console.log('插入成功');
                //打印数据库里的数据,测试用
                console.log(result);
                connect.release();//释放连接池中的数据库连接
            }
        });
    }
  })
}


})
//获取最近十条空气数据
exports.getlastedRecord=(req,res)=>
{
     
     
    console.log('打印近10条数据：')
    let searchsql='select * from kongqi order by ids desc limit 10'
    pool.query(searchsql,(err,results)=>{
        if(err) return res.status.json(err)
        res.status(200).json(results)
    })
    console.log('请求一次最近的数据')
    
 
}
// 获取最近十条土壤数据
exports.getlastedsoil=(req,res)=>
{
     
     
    console.log('打印近10条数据：')
    let searchsql='select * from soil order by ids desc limit 10'
    pool.query(searchsql,(err,results)=>{
        if(err) return res.status.json(err)
        res.status(200).json(results)
    })
    console.log('请求一次最近的数据')
    
 
}
// 获取最近十条NPK数据
exports.getlastednpk=(req,res)=>
{
     
     
    console.log('打印近10条数据：')
    let searchsql='select * from soilnpk order by ids desc limit 1'
    pool.query(searchsql,(err,results)=>{
        if(err) return res.status.json(err)
        res.status(200).json(results)
    })
    console.log('请求一次最近的数据')
    
 
}
// 获取最近十条土壤湿度数据
exports.getlastedsoilhum=(req,res)=>
{
     
     
    console.log('打印近10条数据：')
    let searchsql='select * from soilhum order by ids desc limit 10'
    pool.query(searchsql,(err,results)=>{
        if(err) return res.status.json(err)
        res.status(200).json(results)
    })
    console.log('请求一次最近的数据')
    
 
}


app.use(cors()) 


const userRouter = require('./api_router')
app.use('/api/user', userRouter)



//定义端口，此处所用为3000端口，可自行更改
var server = app.listen(3000,function(){
  console.log('runing 3000...');
})
