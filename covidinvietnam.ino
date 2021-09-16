//Lib
#include <ESP8266WiFi.h>        //Use ESP8266 functions                                              
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Wifi
const char* ssid = "SDvn";              //WIFI SSID Name
const char* password =  "169kimma";        //WIFI Password

//Thingspeak
const char* host = "api.thingspeak.com";  //We read the data from this host                                   
const int httpPortRead = 80;                                                                 
const char* url1 = "/apps/thinghttp/send_request?api_key=9LT0AV70S3YH176X";     //Cases
const char* url2 = "/apps/thinghttp/send_request?api_key=UC9LR9XNTQZACAYN";   //Today
const char* url3 = "/apps/thinghttp/send_request?api_key=UOOFKD50MIQ5IYFX";   //Deaths

//Create a WiFi client and http client
WiFiClient client;
HTTPClient http; 

//Here I keep the numbers that I got
String Cases,Today,Deaths,Data_Raw,Data_Raw1,Data_Raw2,Data_Raw3;


void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
  delay(1000);                                                                  
  WiFi.begin(ssid, password); 
                                                  
  Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
  display.display();
  display.clearDisplay();
}


void loop() {
    // Wait a few seconds between measurements.
    delay(10000);
    Datacase();
    if (Cases != Data_Raw1) {
      Cases = Data_Raw1;
      Datatoday();
      Today=Data_Raw2;
      }
    Datadeath();
    if (Deaths != Data_Raw3) {
      Deaths = Data_Raw3;
    }
     
while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }
      displayOled();

}


void displayOled() {

    //Datacase()
    //Datatoday()
    //Datadeath()
    //else //If we can't connect to the HTTP
     // {
      //  Serial.printf("[HTTP} Unable to connect\n");
      //}
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0,0);
    display.println("Cases:");
    display.setCursor(0,16);
    display.println("Today:");
    display.setCursor(0,32);
    display.println("Deaths:");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,8);
    display.println(Cases);
    display.setCursor(0,24);
    display.println(Today);
    display.setCursor(0,40);
    display.println(Deaths);
    display.display();
}

void Datacase() {
  //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw1 = http.getString();   //Here we store the raw data string
                                           
              //Cases=Data_Raw1;
              //Serial.print("Cases: ");  //I choosed to display it on the serial monitor to help you debug
              //Serial.println(Cases);
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();                                                                 
      }
          else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      } 
  }
void Datatoday() {
  //Reading 2: Reading of today
    if( http.begin(host,httpPortRead,url2))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw2 = http.getString();   //Here we store the raw data string
                                           
              //Today=Data_Raw2;
              //Serial.print("Today: ");
              //Serial.println(Today);                    
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();                                                                 
      }
          else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      } 
}
void Datadeath() {
  //Reading 3: Reading of deaths
    if( http.begin(host,httpPortRead,url3))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw3 = http.getString();   //Here we store the raw data string
                                   
              //Deaths=Data_Raw3;
              //Serial.print("Deaths: ");
              //Serial.println(Deaths);
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();                                                                 
      }
          else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      } 
}
