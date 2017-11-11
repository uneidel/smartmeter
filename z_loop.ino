#define KITKAT (1000UL * 60)
#define WEATHER (1000UL *60 *60)  // 1 Hour

unsigned long rolltime = millis() + KITKAT;
unsigned long rollTemptime = millis() + KITKAT;

void loop() {

  button.tick();
  if (dirty){ tft.fillScreen(TFT_BLACK); }
  switch (currentdisplay){
    case 0:
      ShowTimeAndTemp();
      break;
    case 1:
       ShowLinearGaugeTemp();
    case 2:
       ShowLinearGaugeMemory();
  }
  
  pubsubclient.loop();
  
  
  if (firmwareupdate){ httpServer.handleClient();}


  
  
  
 
}

void ShowLinearGaugeTemp(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(FF24);         
    tft.drawString("Temperature", 10, 0, GFXFF);
    plotLinear("Swarm1", 20, 80, Clustertemp[0]);
    plotLinear("Swarm2", 90, 80, Clustertemp[1]);
    plotLinear("Swarm3", 160, 80, Clustertemp[2]);
    plotLinear("Swarm4", 230, 80, Clustertemp[3]);
    dirty = false;
  }
}
void ShowLinearGaugeMemory(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(FF24);         
    tft.drawString("Memory           ", 10, 0, GFXFF);
    plotLinear("Swarm1", 20, 80, Clustertemp[4]);
    plotLinear("Swarm2", 90, 80, Clustertemp[5]);
    plotLinear("Swarm3", 160, 80, Clustertemp[6]);
    plotLinear("Swarm4", 230, 80, Clustertemp[7]);
    dirty = false;
  }
}
void ShowTimeAndTemp(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
     String s = GetCurrentTime(true);
     s = s.substring(0, s.indexOf(" "));
     s = s.substring(0, s.lastIndexOf(":"));
    tft.setFreeFont(FF24);  
    tft.fillScreen(TFT_BLACK);// Select the font
    tft.setTextDatum(BC_DATUM);
    tft.drawString(s, 150, 50, GFXFF);//
    ui.drawBmp(CurrentWeather.FileName, 200, 100);
    tft.drawString(CurrentWeather.temperature, 50, 200);
    //tft.setFreeFont(FF7);
    //tft.drawString(CurrentWeather.prop, 50, 200);
    dirty = false;
 }
}
void PublishMessage(char* message)
{
  PubSubConnect();
  Serial.println(message);
  pubsubclient.publish(OUT_FEED,message);
}


