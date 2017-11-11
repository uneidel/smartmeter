
#define TFT_GREY 0x5AEB


const String WUNDERGRROUND_API_KEY = "<WUNDERGROUND KEY HERE>";

#define clientName "smartmeter"
const char* WILL_FEED="/devices";
const char* OUT_FEED="/office/smartmeterout";
const char* DEVICE_FEED = "/devices/smartmeter";
const char* CLUSTER_FEED = "cluster/temperature";
const char* WEATHER_FEED = "cgn/weather";


// List, so that the downloader knows what to fetch
String wundergroundIcons [] = {"chanceflurries","chancerain","chancesleet","chancesnow","clear","cloudy","flurries","fog","hazy","mostlycloudy","mostlysunny","partlycloudy","partlysunny","rain","sleet","snow","sunny","tstorms","unknown"};

