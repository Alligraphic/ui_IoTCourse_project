#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* host = "https://iotprojectui.pythonanywhere.com/getValues/";

HTTPClient http;
WiFiClientSecure client2;


int time_on = 500, time_off = 500;

const char* ssid = "ALIREZA 1130";
const char* password = "alireza8181";
int ledPin = D4;
WiFiServer server(80);


int find_on_value(String req) {
  int on_index = req.indexOf("On=");
  on_index += 2;
  char on_str[5] = { 0 };

  int i = 0;
  while (req[++on_index] != '&') {
    on_str[i++] = req[on_index];
  }
  String on_s = (String)(on_str);
  Serial.println(on_s);
  int on = on_s.toInt();
  return on;
}


int find_off_value(String req) {
  int off_index = req.indexOf("Off=");

  off_index += 3;
  char off_str[5] = { 0 };

  int i = 0;
  while (req[++off_index] != ' ') {
    off_str[i++] = req[off_index];
  }

  String off_s = (String)(off_str);
  int off = off_s.toInt();
  return off;
}


void set_values() {
  http.begin(client2, host);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    Serial.println("");
    time_on = find_on_value(payload);
    Serial.print("time on:(");
    Serial.print(time_on, DEC);
    Serial.println(")");
    time_off = find_off_value(payload);
    Serial.print("time off:(");
    Serial.print(time_off, DEC);
    Serial.println(")");
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println(password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  Serial.println("");
  Serial.println("WiFi connected");

  client2.setInsecure(); //the magic line, use with caution
  // client2.connect(host, httpsPort);
}


int counter = 0;
void loop() 
{
  digitalWrite(ledPin, LOW);
  counter += time_on;
  delay(time_on);
  digitalWrite(ledPin, HIGH);
  counter += time_off;
  delay(time_off);

  if (counter >= 10000)
  {
    set_values();
    counter = 0;
  }

}
