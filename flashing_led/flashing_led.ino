#include <ESP8266WiFi.h>
int time_on = 500, time_off = 500;

const char* ssid = "ALIREZA 1130";
const char* password = "alireza8181";
int ledPin = D4;
WiFiServer server(80);


// int str_to_int(char arr[]) {
//   int num = 0;
//   while (arr++ != 0) {
//     num *= 10;

//     num = *arr - 48;
//   }
//   return num;
// }


int find_on_value(String req) 
{
  int on_index = req.indexOf("On=");
  on_index += 2;
  char on_str[5] = { 0 };

  int i = 0;
  while (req[++on_index] != '&')
  {
    on_str[i++] = req[on_index];
  }
  String on_s = (String)(on_str);
  Serial.println(on_s);
  int on = on_s.toInt();
  return on;
}


int find_off_value(String req) 
{
  int off_index = req.indexOf("Off=");
  
  off_index += 3;
  char off_str[5] = { 0 };

  int i = 0;
  while (req[++off_index] != ' ')
  {
    off_str[i++] = req[off_index];
  }

  String off_s = (String)(off_str);
  int off = off_s.toInt();
  return off;
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
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();

  if (!client)
  {
    digitalWrite(ledPin, LOW);
    delay(time_on);
    digitalWrite(ledPin, HIGH);
    delay(time_off);    
    return;
  }


  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
    delay(1);

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("On=") != -1 && request.indexOf("Off=") != -1)
  {
    time_on = find_on_value(request);
    Serial.print("time on:(");
    Serial.print(time_on, DEC);
    Serial.println(")");
    time_off = find_off_value(request);
    Serial.print("time off:(");
    Serial.print(time_off, DEC);
    Serial.println(")");
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><style>");
  client.println("  .form-example {background-color: rgb(226, 239, 243);height: 100%;width: 100%;text-align: center;position: absolute;top: 50%;left: 50%;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);}");
  client.println("  .input-container {background-color: rgb(210, 238, 178); border: 3px solid #4CAF50; padding: 20px; border-radius: 20px; margin: 0; width: 500px; position: absolute; top: 50%; left: 50%; -ms-transform: translate(-50%, -50%); transform: translate(-50%, -50%);}");
  client.println("  label {color: #333; font-weight: bold; display: block; text-align: left; padding: 0px 50px;}");
  client.println("  input[type=\"text\"] {padding: 7px 10px; margin: 8px 0; box-sizing: border-box; border: 2px solid #ccc; border-radius: 10px; width: 80%;}");
  client.println("  input[type=\"submit\"] {background-color: #4CAF50; color: white; padding: 10px 20px; margin: 8px 0; border: none; border-radius: 4px; cursor: pointer;}");
  client.println("</style></head>");
  client.println("<body>");
  client.println("<form action=\"\" method=\"get\" class=\"form-example\">");
  client.println("  <div class=\"input-container\">");
  client.println("    <label for=\"ON\">Time(ms) on: </label>");
  client.println("    <input type=\"text\" name=\"On\" id=\"On\" required><br><br>");
  client.println("    <label for=\"OFF\">Time(ms) off: </label>");
  client.println("    <input type=\"text\" name=\"Off\" id=\"off\" required><br><br>");
  client.println("    <input type=\"submit\" value=\"Send\">");
  client.println("  </div></form></body></html>");
  
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

}
