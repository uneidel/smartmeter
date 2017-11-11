void LoadSetup(){
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) 
    {
      //file exists, reading and loading
      Serial.println("reading config file");
      fs::File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) 
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) 
        {
          strcpy(mqtt_server, json["mqtt_server"]);                  
        } 
        else 
        {
          Serial.println("failed to load json config");
        }
      }
    }
  } 
  else 
  {
    Serial.println("failed to mount FS");
  }
}  

void setupNTP(){
  Serial.println("Connect NTP");
  NTP.begin("0.de.pool.ntp.org", 1, true);
  NTP.setInterval(63); 
}


void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;

}
void saveConfig(){
 
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
   
    fs::File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close(); 
}
void SplitBySpace(String input, String& cmd, String& cmdval){
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == " ") {
      cmd = input.substring(0, i);
      cmdval= input.substring(i+1);
      break;
    }
  }
}

String GetCurrentTime(bool wait) {
  String datetimestring = "";
  datetimestring = NTP.getTimeDateString();
  if (wait){
    while (datetimestring == "Time not set"){
      datetimestring = NTP.getTimeDateString();
      delay(100);
    }
  }
  return datetimestring;
}


void SetupOTA(){
 
  MDNS.begin(clientName);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  firmwareupdate = true;
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", clientName);
  tft.setFreeFont(FF24);  
  tft.fillScreen(TFT_BLACK);// Select the font
  tft.drawString("OTA", 100, 0, GFXFF);//
 
}


// callback called during download of files. Updates progress bar
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal) {
  Serial.println(String(bytesDownloaded) + " / " + String(bytesTotal));

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(240);

  int percentage = 100 * bytesDownloaded / bytesTotal;
  if (percentage == 0) {
    tft.drawString(filename, 120, 220);
  }
  if (percentage % 5 == 0) {
    tft.setTextDatum(TC_DATUM);
    tft.setTextPadding(tft.textWidth(" 888% "));
    tft.drawString(String(percentage) + "%", 120, 245);
    ui.drawProgressBar(10, 225, 240 - 20, 15, percentage, TFT_WHITE, TFT_BLUE);
  }

}


void downloadResources() {
  char id[5];

 
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/" + wundergroundIcons[i] + ".bmp", wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/mini/" + wundergroundIcons[i] + ".bmp", "/mini/" + wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 24; i++) {
    webResource.downloadFile("http://www.squix.org/blog/moonphase_L" + String(i) + ".bmp", "/moon" + String(i) + ".bmp", _downloadCallback);
  }
}

String getMeteoconIcon(String iconText) {
  if (iconText == "F") return "chanceflurries";
  if (iconText == "Q") return "chancerain";
  if (iconText == "W") return "chancesleet";
  if (iconText == "V") return "chancesnow";
  if (iconText == "S") return "chancetstorms";
  if (iconText == "clear-day" || iconText == "clear-night") return "clear";
  if (iconText == "cloudy") return "cloudy";
  if (iconText == "F") return "flurries";
  if (iconText == "fog") return "fog";
  if (iconText == "wind") return "hazy";
  if (iconText == "Y") return "mostlycloudy";
  if (iconText == "H") return "mostlysunny";
  if (iconText == "partly-cloudy-day" || iconText == "partly-cloudy-night") return "partlycloudy";
  if (iconText == "J") return "partlysunny";
  if (iconText == "sleet") return "sleet";
  if (iconText == "rain") return "rain";
  if (iconText == "snow") return "snow";
  if (iconText == "B") return "sunny";
  if (iconText == "0") return "tstorms";


  return "unknown";
}

