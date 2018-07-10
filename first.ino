#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>


const char *ssid = "prometheus_wlink";  //ENTER YOUR WIFI SETTINGS
const char *password = "7550882670";

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
   JsonObject& JSONencoder = JSONbuffer.createObject();  
        
   JSONencoder["moisture"] = "56";
   JSONencoder["temperature"] = "56";
   JSONencoder["humidity"] = "80";
              
   char JSONmessageBuffer[300];
   JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   Serial.println(JSONmessageBuffer);


   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.100.72:8090/yoESP");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
   int httpCode = http.POST(JSONmessageBuffer);   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
  }else{
 
    Serial.println("Error in WiFi connection");   
 
 }

  delay(30000);



  
    /**if (WiFi.status() == WL_CONNECTED) {
      StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
      JsonObject& JSONencoder = JSONbuffer.createObject();  

      JSONencoder["sensorType"] = "Temperature";
 
      JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
      values.add(20); //Add value to array
      values.add(21); //Add value to array
      values.add(23); //Add value to array
   
      JsonArray& timestamps = JSONencoder.createNestedArray("timestamps"); //JSON array
      timestamps.add("10:10"); //Add value to array
      timestamps.add("10:20"); //Add value to array
      timestamps.add("10:30"); //Add value to array

      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      Serial.println(JSONmessageBuffer);

      HTTPClient http;    //Declare object of class HTTPClient
 
      http.begin("http://127.0.0.1:5000/postjson");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
   
      int httpCode = http.POST(JSONmessageBuffer);   //Send the request
      String payload = http.getString();                                        //Get the response payload
   
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
   
      http.end();  //Close connection
   
    } else {
   
      Serial.println("Error in WiFi connection");
   
    }
   
    delay(30000);  //Send a request every 30 seconds
    **/
  
       
   
}

