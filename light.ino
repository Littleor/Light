#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ESP8266WebServer.h>
#define pin 2
int angle = 90;
int speed = 90;
int delay_time = 200;
int back_delay_time = 200;
char ssid[] = "Little";
char pass[] = "XiaoMing";
Servo servo;
ESP8266WebServer server(80);

IPAddress ip(192, 168, 12, 44);  //Node static IP
IPAddress dns(192, 168, 12, 1);
IPAddress gateway(192, 168, 12, 1);
IPAddress subnet(255, 255, 255, 0);

void open();
void close();
void init_basic();
void init_wifi();
void init_servo();
void init_server();

void handleRoot();
void handleOpen();
void handleClose();
void handleSetting();
void handleNotFound();

void Led();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  init_basic();
  init_wifi();
  init_servo();
  init_server();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  server.handleClient();
}

void open() {
  servo.write(angle + speed);
  delay(delay_time);
  servo.write(angle - speed);
  delay(back_delay_time);
  servo.write(angle);
}
void close() {
  servo.write(angle - speed);
  delay(delay_time);
  servo.write(angle + speed);
  delay(back_delay_time);
  servo.write(angle);
}
void init_basic() {
  //Serial.begin(115200);
  //Serial.println("\n Now start");
}
void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  while (WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("Connect to wifi");
    WiFi.begin(ssid, pass);
    delay(5000);
  }
}
void init_servo() {
  servo.attach(2);
  servo.write(angle);
}
void init_server() {
  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.on("/setting", handleSetting);
  server.onNotFound(handleNotFound);
  server.begin();
}


void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "The service of web is ok");
  Led();
}

void handleOpen() {


  open();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Light is open");
  Led();

}
void handleClose() {
  //Serial.println("close");
  close();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Light is close");
  Led();
}
void handleSetting() {
  String message;
  String a;
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + " : " +  server.arg(i) + "\n";
    if (server.argName(i).equals("speed")) {
      speed = server.arg(i).toInt();
      a += "\n speed = " + server.arg(i).toInt();
    }
    else if (server.argName(i).equals("delay")) {
      delay_time = server.arg(i).toInt();
      a += "\n delay= " + server.arg(i).toInt();
    }
    else if (server.argName(i).equals("delay_back")) {
      back_delay_time = server.arg(i).toInt();
      a += "\n delay_delay= " + server.arg(i).toInt();
    }
    else {}
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", message);
  Led();




}
void handleNotFound() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "404");
}

void Led() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

}
