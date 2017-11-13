
void setup() {
  Serial.begin(115200);  
  WiFiManager wifiManager;
 
  setupDisplay();
  setupNTP();
  pinMode(D0,OUTPUT);
  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  button.attachLongPressStart(longClick);
  
  //wifiManager.resetSettings();
  LoadSetup();
  WiFi.hostname(clientName);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_mqtt_server("mqtt", "MQTT Server", mqtt_server, 50);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.autoConnect("Setup smartmeter");
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  //strcpy(bot_token, custom_bot_id.getValue());
  strcpy(mqtt_server, custom_mqtt_server.getValue());
   if (shouldSaveConfig) 
   {
      saveConfig();
   }  
  
  Serial.print("MQTT Server: ");
  Serial.println(mqtt_server);
  SetupPubSub();
  PubSubConnect();

   downloadResources();
 
}
void PubSubConnect() {
  while (!pubsubclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (pubsubclient.connect(clientName,WILL_FEED,1,false, clientName)) {
      
      pubsubclient.subscribe(DEVICE_FEED);
      pubsubclient.subscribe(CLUSTER_FEED);
      pubsubclient.subscribe(WEATHER_FEED);
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(pubsubclient.state());
      delay(1000);
    }
  }
}


void setupDisplay() {
  tft.begin();
  tft.setRotation(3); 
  tft.fillScreen(TFT_BLACK);

}

void SetupPubSub(){
  pubsubclient.setServer(mqtt_server, 1883);
  pubsubclient.setCallback(pubsubCallback);
}


void pubsubCallback(char* topic, byte* payload, unsigned int length) {
  String json = String((char *)payload);
    
  
  if (strcmp(topic,DEVICE_FEED)==0) {
    String json = String((char *)payload);  
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    String action = root["action"];
    String cmdval = root["cmdval"];
    Serial.print("action: ");Serial.println(action);
    Serial.print("cmdval: "); Serial.println(cmdval);
    if (action == "firmwareupdate"){  SetupOTA();  }
    if (action == "showmessage") {

      
    }
  }
  else if (strcmp(topic,CLUSTER_FEED)==0) {
    String json = String((char *)payload);
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& cluster = jsonBuffer.parseObject(json);
    Clustertemp[0] = cluster["s1t"];
    Clustertemp[1] = cluster["s2t"];
    Clustertemp[2] = cluster["s3t"];
    Clustertemp[3] = cluster["s4t"];
    Clustertemp[4] = cluster["s1m"];
    Clustertemp[5] = cluster["s2m"];
    Clustertemp[6] = cluster["s3m"];
    Clustertemp[7] = cluster["s4m"];
  }
  else if (strcmp(topic,WEATHER_FEED)==0) {
    String json = String((char *)payload);
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& weather = jsonBuffer.parseObject(json);
    CurrentWeather.FileName = getMeteoconIcon(weather["precipType"]) + ".bmp";
    CurrentWeather.temperature =  (weather["temperature"].as<String>()) + " C";
    CurrentWeather.prop = (weather["recipprob"].as<String>()) + "%";
  }
}





