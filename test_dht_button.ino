#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTTYPE DHT11

const char* ssid = "MacBook";
const char* password = "golu2008a";

int ledpin = 5; 
int button = 4;
int buttonState=0;

WiFiServer server(80);

const int DHTPin = 0;

DHT dht(DHTPin, DHTTYPE);

static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

void setup() 
{
  pinMode(ledpin, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(ledpin, LOW);
  Serial.begin(115200);
  delay(10);
  dht.begin();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);

  Serial.println(WiFi.localIP());
}

void loop() 
{
  buttonState=digitalRead(button); 
  if (buttonState == 1)
  {
  digitalWrite(ledpin, HIGH); 
  delay(600);
  digitalWrite(ledpin, LOW);   
  }

  WiFiClient client = server.available();

  if (client) 
  {
    Serial.println("New client");
    boolean blank_line = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        if (c == '\n' && blank_line) 
        {
          float h = dht.readHumidity();
          float t = dht.readTemperature();
          float f = dht.readTemperature(true);
          if (isnan(h) || isnan(t) || isnan(f)) 
          {
            Serial.println("Failed to read from DHT sensor!");
            
          }
          else
          {
            float hic = dht.computeHeatIndex(t, h, false);
            dtostrf(hic, 6, 2, celsiusTemp);
            float hif = dht.computeHeatIndex(f, h);
            dtostrf(hif, 6, 2, fahrenheitTemp);
            dtostrf(h, 6, 2, humidityTemp);
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          client.println("<!DOCTYPE HTML>");
          client.println("<html>\n");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n");
          client.println("<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n");
          client.println("<title>ESP8266 Weather Report</title>\n");
          client.println("<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n");
          client.println("body{margin-top: 50px;}\n");
          client.println("h1 {margin: 50px auto 30px;}\n");
          client.println(".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n");
          client.println(".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n");
          client.println(".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n");
          client.println(".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n");
          client.println(".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n");
          client.println(".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n");
          client.println(".button-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 74px;text-align: left;}\n");
          client.println(".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n");
          client.println(".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n");
          client.println(".data{padding: 10px;}\n");
          client.println("</style>\n");
          client.println("</head>\n");
          client.println("<body background='https://image.made-in-china.com/202f0j00mazUIHBlsLqY/Natural-Reed-Aroma-Wicker-for-Automatic-Air-Freshener-Spray.jpg'>\n");
          client.println("<div id=\"webpage\">\n");
          client.println("<h1>Welcome to Spray System</h1>");
          client.println("<div class=\"data\">\n");
          client.println("<div class=\"side-by-side temperature-icon\">\n");
          client.println("<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n");
          client.println("width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n");
          client.println("<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n");
          client.println("c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n");
          client.println("c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n");
          client.println("c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n");
          client.println("c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n");
          client.println("</svg>\n");
          client.println("</div>\n");
          client.println("<div class=\"side-by-side temperature-text\">Temperature</div>\n");
          client.println("<div class=\"side-by-side temperature\">");
          client.println(celsiusTemp);
          client.println("<span class=\"superscript\">&#8451</span></div>\n");
          client.println("</div>\n");
          client.println("<div class=\"data\">\n");
          client.println("<div class=\"side-by-side humidity-icon\">\n");
          client.println("<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n");
          client.println("<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n");
          client.println("c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n");
          client.println("</svg>\n");
          client.println("</div>\n");
          client.println("<div class=\"side-by-side humidity-text\">Humidity</div>\n");
          client.println("<div class=\"side-by-side humidity\">");
          client.println(humidityTemp);
          client.println("<span class=\"superscript\">%</span></div>\n");
          client.println("</div>\n");

          client.println("<br><br>");
          client.println("<a href=\"/LED=ON\"\"><button><div class=\"side-by-side button-text\">Spray</div></button></a>");
          client.println("</div>\n");
          client.println("</body></html>");
          
          break;
        }
        if (c == '\n') 
        {
          blank_line = true;
        }
        else if (c != '\r') 
        {
          blank_line = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
