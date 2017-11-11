
#define FS_NO_GLOBALS
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <DNSServer.h>            
#include <ESP8266WebServer.h>    
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <NtpClientLib.h>
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <PubSubClient.h>

#include "Free_Fonts.h" 
#include "SPI.h"
#include "TFT_eSPI.h"

#include "GfxUi.h"


#include "OneButton.h"

#include "WebResource.h"
WebResource webResource;

ProgressCallback _downloadCallback = downloadCallback;


OneButton button(D0, true);

int Clustertemp[8];
bool shouldSaveConfig = false;


char mqtt_server[40];
WiFiClient espClient;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
PubSubClient pubsubclient(espClient);

bool firmwareupdate = false;
bool dirty = false;
int currentdisplay = 0;
TFT_eSPI tft = TFT_eSPI();


GfxUi ui = GfxUi(&tft);

struct Weather {
    String FileName;
    String prop;
    String temperature; 
} CurrentWeather;


