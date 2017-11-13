#define KITKAT (1000UL * 60)
#define DISABLE_LED (1000UL *60 *30)  // 30 Min

unsigned long rolltime = millis() + KITKAT;
unsigned long ROLLDISABLELED = millis() + DISABLE_LED;

void loop() {

  button.tick();

  switch (currentdisplay){
    case 0:
        DisableLED();
        dirty = false;
        break;
    case 1:
      EnableLED();
      ROLLDISABLELED += DISABLE_LED;
      ShowTimeAndTemp();
      dirty = false;
      break;
    case 2:
       EnableLED();
       ShowLinearGaugeTemp();
       ROLLDISABLELED += DISABLE_LED;
       dirty = false;
       break;
    case 3:
        EnableLED();
       ShowLinearGaugeMemory();
       ROLLDISABLELED += DISABLE_LED;
       dirty = false;
       break;
  }
  
  pubsubclient.loop();
  
  
  if (firmwareupdate){ httpServer.handleClient();}

  if((long)(millis() - ROLLDISABLELED) >= 0 || currentdisplay != 0) {
     DisableLED();
  }
}

void DisableLED(){
   pinMode(D3,OUTPUT);
   digitalWrite(D3,0);
}
void EnableLED(){
  pinMode(D3,OUTPUT);
  digitalWrite(D3,1);
}
void ShowLinearGaugeTemp(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
     tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(FF24);         
    tft.drawString("Temperature", 10, 0, GFXFF);
    plotLinear("Swarm1", 20, 80, Clustertemp[0]);
    plotLinear("Swarm2", 90, 80, Clustertemp[1]);
    plotLinear("Swarm3", 160, 80, Clustertemp[2]);
    plotLinear("Swarm4", 230, 80, Clustertemp[3]);
    
  }
}
void ShowLinearGaugeMemory(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
     tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(FF24);         
    tft.drawString("Memory           ", 10, 0, GFXFF);
    plotLinear("Swarm1", 20, 80, Clustertemp[4]);
    plotLinear("Swarm2", 90, 80, Clustertemp[5]);
    plotLinear("Swarm3", 160, 80, Clustertemp[6]);
    plotLinear("Swarm4", 230, 80, Clustertemp[7]);
    
  }
}
void ShowTimeAndTemp(){
  if((long)(millis() - rolltime) >= 0 || dirty == true) {
     rolltime += KITKAT;
     tft.fillScreen(TFT_BLACK);
     String s = GetCurrentTime(true);
     s = s.substring(0, s.indexOf(" "));
     s = s.substring(0, s.lastIndexOf(":"));
    tft.setFreeFont(FF24);  
    tft.fillScreen(TFT_BLACK);// Select the font
    tft.setTextDatum(BC_DATUM);
    tft.drawString(s, 150, 50, GFXFF);//
    ui.drawBmp(CurrentWeather.FileName, 200, 100);
    tft.drawString(CurrentWeather.temperature, 50, 200);
    
 }
}
void PublishMessage(char* message)
{
  PubSubConnect();
  Serial.println(message);
  pubsubclient.publish(OUT_FEED,message);
}


