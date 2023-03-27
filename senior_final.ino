#include <analogWrite.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <DHT.h>
//TX 16
//RX 17
#include <HardwareSerial.h>

//rgb彩色灯
#define led_red 21   
#define led_green 19
#define led_blue 22
//设置继电器引脚
#define RELAY_PIN 5
//二号继电器，绑蓝色胶带的是2号
#define RELAY_PIN2 18

#define DHTPIN 14  //DHT11   
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);  

//蜂鸣器
#define beep_pin 33
char g_i=0;

//StaticJsonDocument<200> sensor_json;
StaticJsonDocument<200> sensor_json1;//  //空气数据
StaticJsonDocument<200> sensor_json2;//  土壤数据
StaticJsonDocument<200> sensor_json3;   //土壤pku
StaticJsonDocument<200> sensor_json4;  //两块土地的湿度
String realmsg="";
String realmsg2="";
String realmsg3="";
String realmsg4="";
int LED=2;

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "1234";
const char* password = "123690tyq";
const char* mqttServer = "47.113.146.63";//服务器ip

float h;//dht11采集的空气湿度
float t;//dht11 采集的温度
int value;//光照强度
int valueh1;//土壤湿度
int valueh2;//土壤湿度
//土壤传感器请求指令
unsigned char item[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x08,0x44, 0x0C};
float linshi_d[10]; //保存测到的土壤数据

//设置灌溉模式
int set_auto=1;//默认自动灌溉
int set_1water=0;//默认不灌溉1号农田
int set_guangai2=0;//默认不灌溉2号农田
int setled=0;//默认不开灯
int set_sign1=0;//默认不对一号继电器操作
int set_sign2=0;//默认不对2号农田操作
int set_off=1;//切换为手动模式时先关继电器等待操作


//发布的消息字段
typedef struct {
  int msg1;
  int msg2;
  int msg3;
}msg;
int count=0;//发布消息间隔
// 如以上MQTT服务器无法正常连接，请前往以下页面寻找解决方案
// http://www.taichi-maker.com/public-mqtt-broker/
//------------------全局变量定义-----------------
//msg mymsg={123,45,6};//定义消息结构体
msg mymsg={h,t,6};//定义消息结构体

 
Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//------------------全局变量定义-----------------


void tickerCount(){//Ticker回调函数
  count++;
  //数据测试
  //空气数据
sensor_json1["msg1"] = h;//空气湿度
sensor_json1["msg2"] =t;//空气温度
sensor_json1["msg3"] = value;//光强
serializeJson(sensor_json1,realmsg);
//  //土壤数据
sensor_json2["msg1"] = linshi_d[1];  //土壤温度
sensor_json2["msg2"] =linshi_d[2];   //电导率
sensor_json2["msg3"] = linshi_d[7];  //含盐量
serializeJson(sensor_json2,realmsg2);
//  //  土壤npk
sensor_json3["msg1"] = linshi_d[4];    //n
sensor_json3["msg2"] =linshi_d[5];     //p
sensor_json3["msg3"] = linshi_d[6];    //k
serializeJson(sensor_json3,realmsg3);
//  //两块土壤的湿度
sensor_json4["msg1"] = valueh1;  //土壤湿度传感器1测的的湿度
sensor_json4["msg2"] =valueh2;   //土壤湿度传感器2测的的湿度
sensor_json4["msg3"] =linshi_d[3];    //土壤ph
serializeJson(sensor_json4,realmsg4);
}

 //发布
void pubmsg( const String &topicString, const  String &messageString){

  char publishTopic[topicString.length() + 1];  
  strcpy(publishTopic, topicString.c_str());
  char publishMsg[messageString.length() + 1];   
  strcpy(publishMsg, messageString.c_str());
  
  // 实现ESP8266向主题发布信息
  if(mqttClient.publish(publishTopic, publishMsg)){
    Serial.println("Publish Topic:");Serial.println(publishTopic);
    Serial.println("Publish message:");Serial.println(publishMsg);    
  } else {
    Serial.println("Message Publish Failed."); 
  }
}
void setup() {
//  pinMode(LED, OUTPUT);     // 设置板上LED引脚为输出模式
//  digitalWrite(LED, LOW);  // 启动后关闭板上LED
  Serial.begin(9600);               // 启动串口通讯
  Serial2.begin(9600);
 
  dht.begin(); 
  //光强
  pinMode(4, INPUT);
  pinMode(35, INPUT);

  //将继电器设为输出模式
  pinMode(RELAY_PIN,OUTPUT);
   pinMode(RELAY_PIN2,OUTPUT);

//  rgb彩灯
  pinMode(led_red,OUTPUT);  //设置引脚为输出模式
  pinMode(led_green,OUTPUT);
  pinMode(led_blue,OUTPUT);
  analogWrite(led_red,0);     //首先，先把所有的灯都关闭 共阳极
  analogWrite(led_green,0);   
  analogWrite(led_blue,0);
//蜂鸣器设子引脚为输出模式
  pinMode(beep_pin, OUTPUT);


  // 设置定时周期为2s，回调函数为tickerCount
//这个没用，测试用的
//  ticker.attach(2, tickerCount); //每过2s调用其回调函数
  
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  
  // 连接WiFi
  connectWifi();
  
  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
 
  // 连接MQTT服务器
  connectMQTTserver();

  while (!Serial)
  ;
}
 
void loop() {

      //DHT11读取温湿度
   h= dht.readHumidity();   
   t= dht.readTemperature(); 
//  Serial.print("相对湿度: ");   
//  Serial.print(h);
//  Serial.println(" %RH");
//  Serial.print("温度: ");
//  Serial.print(t);
//  Serial.println(" °C");

  //光强，有光的情况下输出0，光弱输出1，光弱时光敏传感器模块的DO-LED熄灭。
//  顺时针调整模块上的十字旋钮会使敏感度降低，顺时针调整敏感度增强
//  Serial.print("DO：");
// Serial.print(digitalRead(4));
 value=analogRead(35);
// Serial.print("||AO：");
// Serial.println(value);

 //土壤湿度传感器
  valueh1=analogRead(32);
// Serial.print("土壤湿度：");
// Serial.println(valueh1);
// 
 valueh2=analogRead(34);
// Serial.print("土壤湿度：");
// Serial.println(valueh2);






//连接服务器并操作
  if (mqttClient.connected()) { // 如果开发板成功连接服务器
//   if (count>0)
{
      
  //发布信息
  //这里给web发消息
  //发送realmsg
      pubmsg("device/date",realmsg);
      realmsg="";
      
  //发送realmsg2
   pubmsg("device/date2",realmsg2);
      realmsg2="";

//发送realmsg3
   pubmsg("device/date3",realmsg3);
      realmsg3="";

//发送realmsg4
   pubmsg("device/date4",realmsg4);
      realmsg4="";      
//发送给android的主题其他数据


  tickerCount();

  
    }
  mqttClient.loop();          // 处理信息以及心跳  

  } else {                      // 如果开发板未能成功连接服务器
    connectMQTTserver();        // 则尝试连接服务器
  }

//补光
//setColor();

//灌溉功能
  guangai();

//土壤pku传感器
soil_senior();
//土壤营养不良时触发蜂鸣器报警
buzzer();


 delay(500);

}
 
// 连接MQTT服务器并订阅信息
void connectMQTTserver(){
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "esp8266-" + WiFi.macAddress();
 
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str())) { 
    Serial.println("MQTT Server Connected.");
    Serial.println("Server Address:");
    Serial.println(mqttServer);
    Serial.println("ClientId: ");
    Serial.println(clientId);
    subscribeTopic(); // 订阅指定主题
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
  }   
}
// 收到信息后的回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Received [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  Serial.print("Message Length(Bytes) ");
  Serial.println(length);
  //

  int temp=0;
  while(*topic){
    temp++;
    topic++;
  }

  Serial.println("消息长度:");
  Serial.println(temp);

   
      //判断灌溉模式
   if((char)topic[7]=='a'&&(char)topic[8]=='u'){

     if ((char)topic[13]=='1') {    
                set_auto=1;
                Serial.println("收到信息灌溉自动模式");
        }
     else if ((char)topic[13]=='0') {    
                set_auto=0;
                Serial.println("收到信息灌溉手动模式");
                
                set_off=1;
        }

          }
    if((char)topic[14]=='g'&&(char)topic[15]=='1'){       
                
                set_sign1=1;//对1号农田操作                
                Serial.println("对一号农田进行自动灌溉模式操作");
                if((char)topic[18]=='1')
                {//打开一号继电器
                
                  
                    set_1water=1;
                      Serial.print("set_1water:");
                      Serial.print(set_1water);
                      Serial.println("1号继电器赋值on");
                }
                  else if((char)topic[18]=='0')
                {
                    //关闭一号继电器
                      set_1water=0;
                      Serial.print("set_1water:");
                      Serial.print(set_1water);
                      Serial.println("1号继电器赋值off");
      
                }
            }
     else if((char)topic[14]=='g'&&(char)topic[15]=='2'){       
               
                set_sign2=1;//对2号农田操作                  
 
                if((char)topic[18]=='1')
                {//打开一号继电器
                    set_guangai2=1;
                      Serial.println("2号继电器赋值on");
                
                
                
                
                }
                  else if((char)topic[18]=='0')
                {
                    //关闭一号继电器
                      set_guangai2=0;
                      Serial.println("2号继电器赋值off");
      
                }
         
    }

  //判断补光灯
    else if((char)topic[7]=='b'&&(char)topic[8]=='l')
  {
     if((char)topic[13]=='1')
     {
      //补紫光
      Serial.println("补紫光");
      setled=1;

      analogWrite(led_red,106);  
      analogWrite(led_green,90);   
      analogWrite(led_blue,205);
      
      }
     else if((char)topic[13]=='0')
     {
      
          Serial.println("关灯");
      setled=0;
          analogWrite(led_red,0);     //把所有的灯都关闭 共阳极
          analogWrite(led_green,0);   
          analogWrite(led_blue,0);
      
      }
  }
    else if((char)topic[7]=='r'&&(char)topic[8]=='e')
  {
     if((char)topic[12]=='1')
     {
      //补红光
          Serial.println("补红光");
      setled=2;
        //补红光
        analogWrite(led_red,255);  
        analogWrite(led_green,0);   
        analogWrite(led_blue,0);
      }
     else if((char)topic[12]=='0')
     {
      setled=0;
          Serial.println("关灯");
              //关灯
          analogWrite(led_red,0);     //把所有的灯都关闭 共阳极
          analogWrite(led_green,0);   
          analogWrite(led_blue,0);
      }
  }
    
  
}
 
// 订阅指定主题
void subscribeTopic(){
 
 
  
  // 通过串口监视器输出是否成功订阅主题1以及订阅的主题1名称
  if(mqttClient.subscribe("device/dev/123")){
    Serial.println("Subscrib Topic:");
    Serial.println("device/action");
  } else {
    Serial.print("Subscribe Fail...");
  }  
 
  // 通过串口监视器输出是否成功订阅主题2以及订阅的主题2名称
     
}
 
// ESP8266连接wifi
void connectWifi(){
 
  WiFi.begin(ssid, password);
 
  //等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");  
  Serial.println(""); 
}



//水泵
void guangai()
{
 if(set_auto==1)
  {
            // 一号灌溉器
              Serial.println("水泵自动灌溉模式");
              if(valueh1>3900)  //1020为模拟值
              {
               digitalWrite(RELAY_PIN, HIGH);//继电器1开，放水水泵开
                Serial.println("继电器1开，放水！");
               
                }
              else
            {
     
              digitalWrite(RELAY_PIN, LOW);//继电器1关
              Serial.println("继电器1关！");
                }

//             二号灌溉器
               if(valueh2>3900)  
              {
              digitalWrite(RELAY_PIN2, HIGH);//继电器2开，放水水泵开
              Serial.println("继电器2开，放水！");
              }
              else
              {
     
              digitalWrite(RELAY_PIN2, LOW);//继电器2关
              Serial.println("继电器2关！");
              }
  }
  else if(set_auto==0)
  {
  //先把继电器都关闭
  Serial.println("水泵手动灌溉模式");
    if(set_off==1) //保证只在切换模式时触发一次
    {   Serial.println("先全部关闭继电器");  
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(RELAY_PIN2, LOW);
    }
//   选择对哪一个继电器操作
      if(set_sign1==1)
   {
     set_off=0;
     Serial.print("set_1water:");
      Serial.println(set_1water);
     Serial.println("对水泵1操作");
    if(set_1water==1)
        {
          Serial.println("水泵1手动灌溉中");
          //打开一号继电器
             digitalWrite(RELAY_PIN, HIGH);//继电器1开，放水水泵开 
             set_1water=1;     
          }
         else if(set_1water==0)
         {
         Serial.println("水泵1手动灌溉结束");
       //关闭一号继电器
           digitalWrite(RELAY_PIN, LOW);//继电器1关，放水水泵关
           set_sign1=0;
          }
   }
//   
      if(set_sign2==1)
   {
     set_off=0;
     Serial.print("set_guangai2:");
      Serial.println(set_guangai2);
     Serial.println("对水泵2操作");
    if(set_guangai2==1)
        {
          Serial.println("水泵1手动灌溉中");
          //打开一号继电器
             digitalWrite(RELAY_PIN2, HIGH);//继电器1开，放水水泵开 
             set_guangai2=1;     
          }
         else if(set_guangai2==0)
         {
         Serial.println("水泵1手动灌溉结束");
       //关闭一号继电器
           digitalWrite(RELAY_PIN2, LOW);//继电器1关，放水水泵关
           set_sign2=0;
          }
   }
  if(set_sign2==0&&set_sign1==0)
   {
     Serial.println("没响应");
   }
        
    
  } 
   
}

//土壤pku传感器
void soil_senior()
{
  String data = "";
  char buff[128]; // 定义存储传感器数据的数组
  String info[21];
Serial.println("输入的指令为：");
  for (int i = 0; i < 8; i++)
  {                         // 发送测温命令
    Serial2.write(item[i]); // write输出
    Serial.print(",");
    Serial.print(item[i]);//测试
  }
  delay(100); // 等待测温数据返回
  data = "";
Serial.println();
Serial.println("输出的指令为：");
  while (Serial2.available())
  {                                                   //从串口中读取数据
    unsigned char in = (unsigned char)Serial2.read(); // read读取
     Serial.print(in, HEX);
     Serial.print(',');
    data += in;
    data += ',';
  }
  Serial.println(";");
  Serial.println();
  if (data.length() > 0)
  { //先输出一下接收到的数据
     Serial.print(data.length());
     Serial.println();
     Serial.println(data);
    int commaPosition = -1;
    // 用字符串数组存储
    for (int i = 0; i < 21; i++)
    {
      commaPosition = data.indexOf(',');
      if (commaPosition != -1)
      {
        info[i] = data.substring(0, commaPosition);
        data = data.substring(commaPosition + 1, data.length());
      }
      else
      {
        if (data.length() > 0)
        {
          info[i] = data.substring(0, commaPosition);
        }
      }
    }
  }

 

   linshi_d[0] = (info[3].toInt() * 256 + info[4].toInt()) / 10.0;
   linshi_d[1] = (info[5].toInt() * 256 + info[6].toInt()) / 10.0;
   linshi_d[2] = (info[7].toInt() * 256 + info[8].toInt()) / 10.0;
   linshi_d[3] = (info[9].toInt() * 256 + info[10].toInt()) / 10.0;
   linshi_d[4] = (info[11].toInt() * 256 + info[12].toInt()) / 10.0;
   linshi_d[5] = (info[13].toInt() * 256 + info[14].toInt()) / 10.0;
   linshi_d[6] = (info[15].toInt() * 256 + info[16].toInt()) / 10.0;
   linshi_d[7] = (info[17].toInt() * 256 + info[18].toInt()) / 10.0;
//   Serial.print("水分:");
//   Serial.println(linshi_d[0]);
//   Serial.print("土壤温度:");
//   Serial.println(linshi_d[1]);
//   Serial.print("电导率:");
//   Serial.println(linshi_d[2]);
//   Serial.print("酸碱度:");
//   Serial.println(linshi_d[3]);
//   Serial.print("N含量:");
//   Serial.print(linshi_d[4]);
//   Serial.println("mg/kg");
//   Serial.print("P含量:");
//   Serial.print(linshi_d[5]);
//   Serial.println("mg/kg");
//   Serial.print("K含量:");
//   Serial.print(linshi_d[6]);
//   Serial.println("mg/kg");
//   Serial.print("含盐量:");
//   Serial.println(linshi_d[7]);
  
}

//蜂鸣器报警

void buzzer()
{
  // 土壤营养不良时，蜂鸣器报警，NPK和PH
  if(linshi_d[4]<0.3||linshi_d[3]>9.1){
    //营养不良报警
    g_i=1;
  digitalWrite(beep_pin,g_i);
 
  }
  else{
//    停止报警
        g_i=0;
  digitalWrite(beep_pin,g_i);
    }
}
