/*********
  Gourav Das
  Complete project details at https://hackernoon.com/cloud-home-automation-series-part-2-use-aws-device-shadow-to-control-esp32-with-arduino-code-tq9a37aj
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include <AWS_IOT.h>
#include "WiFi.h"
#include <ArduinoJson.h>  //Download latest ArduinoJson 6 Version library only by Benoît Blanchon from Manage Libraries


AWS_IOT shadow;
#define ledPin 2  // LED Pin

const char* WIFI_SSID = "toya1234568280563";                                 //Replace with your WiFi Name
const char* WIFI_PASSWORD = "70510424";                                  // Replace with your WiFi Password
char HOST_ADDRESS[] = "a3i2k6sho7daw8-ats.iot.us-east-1.amazonaws.com";  //AWS IoT Custom Endpoint Address
char CLIENT_ID[] = "ESP32";
char SHADOW_GET[] = "$aws/things/myWindowBlinds2023/shadow/get/accepted";
char SENT_GET[] = "$aws/things/myWindowBlinds2023/shadow/get";
char SHADOW_UPDATE[] = "$aws/things/myWindowBlinds2023/shadow/update";

int status = WL_IDLE_STATUS;
int msgReceived = 0;
int wifi_i = 0;
int aws_iot_connect_i = 0;
int shadow_connect = 1;
int shadow_publish = 1;
int shadow_subscribe = 1;
int connect_AWS_i = 0;
int shadow_subscribe_i = 0;
int shadow_publish_i = 0;
char payload[512];
char reportpayload[512];
char rcvdPayload[512];

void mySubCallBackHandler(char* topicName, int payloadLen, char* payLoad) {
  strncpy(rcvdPayload, payLoad, payloadLen);
  rcvdPayload[payloadLen] = 0;
  msgReceived = 1;
}

void setup_wifi() {
  wifi_i = 0;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    wifi_i++;
    Serial.println("Connecting to WiFi..");
    if (wifi_i > 100) {
      //spanie!
    }
  }
  Serial.println(WiFi.localIP());
}

void AWS_core_connect() {
  shadow_connect = 1;
  aws_iot_connect_i = 0;
  while (shadow_connect != 0)  //Connect to AWS IoT COre
  {
    aws_iot_connect_i++;
    if (aws_iot_connect_i > 100) {
      //spanie
    }
    shadow_connect = shadow.connect(HOST_ADDRESS, CLIENT_ID);
    //Serial.println("Connected to AWS");
    delay(1000);
  }
}

void subscribe_to_Accept_get() {
  shadow_subscribe = 1;
  shadow_subscribe_i = 0;
  while (shadow_subscribe != 0)  //Subscribe to Accepted GET Shadow Service
  {
    shadow_subscribe_i++;
    if (shadow_subscribe_i > 20) {
      //sapnie
        AWS_core_connect();
    }
    shadow_subscribe = shadow.subscribe(SHADOW_GET, mySubCallBackHandler);
    //Serial.println("Subscribe Successfull");
  }
}
void updateShadow (int power)
{ 
  sprintf(reportpayload,"{\"state\": {\"reported\": {\"power\": \"%d\"}}}",power);
  delay(3000);   
    if(shadow.publish(SHADOW_UPDATE,reportpayload) == 0)
      {       
        Serial.print("Publish Message:");
        Serial.println(reportpayload);
      }
    else
      {
        Serial.println("Publish failed");
        Serial.println(reportpayload);   
      }  
} 

void send_empty_string() {
  shadow_publish = 1;
  shadow_publish_i = 0;
  while (shadow_publish != 0) {
    delay(3000);
    shadow_publish_i++;
    if (shadow_publish_i > 100) {
      //sapnie
    }
    shadow_publish = shadow.publish(SENT_GET, "{}");
    //Serial.print("Empty String Published\n");
  }
}



void setup() {
  Serial.begin(9600);
  setup_wifi();
  AWS_core_connect();
  subscribe_to_Accept_get();
  send_empty_string();
  
}
void loop() {
  if (msgReceived == 1) {
    msgReceived = 0;
    //Serial.print("Received Message:");
    //Serial.println(rcvdPayload);
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, rcvdPayload);
    if (doc.isNull()) { /* Test if parsing succeeds. */
      Serial.println("parseObject() failed");
      //spanie
    } /* Test if parsing succeeds. */
    int status = doc["state"]["desired"]["st"];
    float slider = doc["state"]["desired"]["sl"];
    int sliderApply = doc["state"]["desired"]["sa"];
    Serial.println(status);
    Serial.println(slider);
    Serial.println(sliderApply);

  status = WL_IDLE_STATUS;
  msgReceived = 0;
  wifi_i = 0;
  aws_iot_connect_i = 0;
  shadow_connect = 1;
  shadow_publish = 1;
  shadow_subscribe = 1;
  connect_AWS_i = 0;
  shadow_subscribe_i = 0;
  shadow_publish_i = 0;
  char payload[512];
  char reportpayload[512];
  char rcvdPayload[512];

  //setup_wifi();
  //AWS_core_connect();
  subscribe_to_Accept_get();
  send_empty_string();

    //updateShadow(power);
  }
  //SPANIE
}



