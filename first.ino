#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <String.h>

const char *ssid = "prometheus_wlink";  //ENTER YOUR WIFI SETTINGS
const char *password = "7550882670";

bool state = false;

ESP8266WebServer server(80);

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

  
  server.on("/body",handleBody);
  server.begin();
  Serial.println("Server Listening");

}

void loop() {
  // put your main code here, to run repeatedly:
   
  server.handleClient();

  if (state==1){
    Serial.println("It's ready.");

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

    
    state = false;
    }
  
}

void handleBody(){
     if (server.hasArg("plain")== false){ //Check if body received
 
            server.send(200, "text/plain", "Body not received");
            return;   
      }
 
      String message = server.arg("plain");
             message += "\n";
      server.send(200, "text/plain", message);
      Serial.println(state); // here
      Serial.println(message);
      state = true;
      Serial.println(state);
      
  }


