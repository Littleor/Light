#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ESP8266WebServer.h>
#define pin 2
int angle = 90;
int speed = 35;
int delay_time = 200;
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
  delay(delay_time);
  servo.write(angle);
}
void close() {
  servo.write(angle - speed);
  delay(delay_time);
  servo.write(angle + speed);
  delay(delay_time);
  servo.write(angle);
}
void init_basic() {
  //Serial.begin(115200);
  //Serial.println("\n Now start");
}
void init_wifi() {
  while (WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("Connect to wifi");
    WiFi.begin(ssid, pass);
    delay(6000);
  }
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  //Serial.println("\n Now is ok,and status is:");
  //Serial.println(WiFi.localIP());
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
  server.send(200, "text/plain", "This is a web for 531's Light");
  Led();
}

void handleOpen() {

  //Serial.println("open");

  open();
  server.send(200, "text/plain", "Light is open");
  Led();

}
void handleClose() {
  //Serial.println("close");
  close();
  server.send(200, "text/plain", "Light is close");
  Led();
  

  
}
void handleSetting() {
  String message;

  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + " : " +  server.arg(i) + "\n";
    if (server.argName(i).equals("speed")) {
      speed = server.arg(i).toInt();
    }
    else if (server.argName(i).equals("speed")) {
      delay_time = server.arg(i).toInt();
    }
    else {}
  }

  //Serial.println(message);
  server.send(200, "text/plain", message);
    Led();
  Led();
}
void handleNotFound() {
  server.send(200, "text/plain", "404");
}

void Led(){
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  
  }
