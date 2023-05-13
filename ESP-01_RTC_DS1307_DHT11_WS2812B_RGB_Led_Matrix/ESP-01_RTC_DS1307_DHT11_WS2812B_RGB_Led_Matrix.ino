// REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/Tbg493zyqu0
// - DHT11 DHT22 Library: https://github.com/3tawi/EspDHT
// - DS1307ESP Library: https://github.com/3tawi/DS1307ESPTime
// - Adafruit_NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
// - Getting Started ESP01 : https://youtu.be/QRnPRKbtEGU
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects


#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <DS1307ESPTime.h>
#include "EspDHT.h"
#include "atawi8x6.h"
#include "page.h"

#define DHTPIN 2  
#define PIN         0 
#define NUMPIXELS 256 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
GFXcanvas1 canvasF(33, 8);
DS1307ESPTime rtc;
EspDHT dht;

String  Ssid, Pass;
const char* Apssid = "NeoPixel";
const char* Appassword = "3tawi-GP";
  IPAddress ip(192,168,1,31);
  IPAddress dns(192, 168,1,1);
  IPAddress gateway(192, 168,1,1);
  IPAddress gatewayap(192,168,4,1);
  IPAddress subnet(255, 255, 255, 0);

File myfile;
float temp, humi;
int Tz = 1; // Time Zone
bool flasher = false;
bool wifiok = false;
bool Modeclock = true;
String Message = "Great Projects wish you a very nice day. Hope you have a great time ahead.";
long savednum = 0 ,passnum = 0; 
int xps=32, msg = 1;
uint32_t lastTime, prevTime; 
String text, textmsg, textip;
int sp0=30, sp1=300, sp2=5, sp3=15, sp4, sp5, sp6, sp7;
long     hueShift;
long     value;
int      dx1, dx2, dx3, dx4, dy1, dy2, dy3, dy4, sx1, sx2, sx3, sx4;
int      r, g, b;
uint8_t  lo;
float rs1, rs2, rs3, rs4,
      cx1, cx2, cx3, cx4,
      cy1, cy2, cy3, cy4;
float  ag1, ag2, ag3, ag4;
static const int8_t PROGMEM sinetab[256] = {
     0,   2,   5,   8,  11,  15,  18,  21, 24,  27,  30,  33,  36,  39,  42,  45,
    48,  51,  54,  56,  59,  62,  65,  67, 70,  72,  75,  77,  80,  82,  85,  87,
    89,  91,  93,  96,  98, 100, 101, 103, 105, 107, 108, 110, 111, 113, 114, 116,
   117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126,
   127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118,
   117, 116, 114, 113, 111, 110, 108, 107, 105, 103, 101, 100,  98,  96,  93,  91,
    89,  87,  85,  82,  80,  77,  75,  72, 70,  67,  65,  62,  59,  56,  54,  51,
    48,  45,  42,  39,  36,  33,  30,  27, 24,  21,  18,  15,  11,   8,   5,   2,
     0,  -3,  -6,  -9, -12, -16, -19, -22, -25, -28, -31, -34, -37, -40, -43, -46,
   -49, -52, -55, -57, -60, -63, -66, -68, -71, -73, -76, -78, -81, -83, -86, -88,
   -90, -92, -94, -97, -99,-101,-102,-104, -106,-108,-109,-111,-112,-114,-115,-117,
  -118,-119,-120,-121,-122,-123,-124,-124, -125,-126,-126,-127,-127,-127,-127,-127,
  -128,-127,-127,-127,-127,-127,-126,-126, -125,-124,-124,-123,-122,-121,-120,-119,
  -118,-117,-115,-114,-112,-111,-109,-108, -106,-104,-102,-101, -99, -97, -94, -92,
   -90, -88, -86, -83, -81, -78, -76, -73, -71, -68, -66, -63, -60, -57, -55, -52,
   -49, -46, -43, -40, -37, -34, -31, -28, -25, -22, -19, -16, -12,  -9,  -6,  -3
};
 
uint16_t XY( uint8_t x, uint8_t y)
{
  if( x >= 32 || x < 0 || y >= 8 || y < 0) return -1;
  uint16_t i;
  if( x & 0x01) {
      i = (x * 8) + 7 - y;
  } else {
      i = (x * 8) + y;
  }
  return i;
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

void getWifi()
{
  char user[30], pasw[30];
  int lengthSsid = Ssid.length() + 1;
  int lengthPass = Pass.length() + 1;
  Ssid.toCharArray(user, lengthSsid);
  Pass.toCharArray(pasw, lengthPass);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(user, pasw);
  int xc = 0;
  while (WiFi.status() != WL_CONNECTED && xc < 10) {
    delay(500);
    xc++;
  }
  if (WiFi.status() == WL_CONNECTED) { 
    textip = "CONNECTED To: ";
    textip += Ssid;
    textip += " IP Address: http://";
    textip += WiFi.localIP().toString().c_str();
    WiFi.softAPdisconnect(true);
    wifiok = true;
    } else { 
    WiFi.softAPConfig(ip, gatewayap, subnet);
    WiFi.softAP(Apssid, Appassword);
    textip = "CONNECTED To: ";
    textip += Apssid;
    textip += " Password: ";
    textip += Appassword;
    textip += " IP Address: http://";
    textip += WiFi.softAPIP().toString().c_str();
    wifiok = false;
    }
    msg = 8, getmesg();
}
void Colorplat(int cpla) {
  switch (cpla) { 
  case 0:
  rs1 = 4.1, rs2 = 6.3, rs3 = 10.2, rs4 = 11.1;
  cx1 = 4.1, cy1 = 2.2;
  cx2 = 5.8, cy2 = 1.7;
  cx3 = 5.8, cy3 = 3.5;
  cx4 = 1.1, cy4 = -1.5;
  sp4 = 3, sp5 = 2;
    break;
  case 1:
  rs1 = 4.1, rs2 = 6.3, rs3 = 10.2, rs4 = 11.1;
  cx1 = 4.1, cy1 = 2.2;
  cx2 = 5.8, cy2 = 1.7;
  cx3 = 5.8, cy3 = 3.5;
  cx4 = 1.1, cy4 = -1.5;
  sp4 = 1, sp5 = 2;
    break;
  case 2:
  rs1 = 4.1, rs2 = 6.3, rs3 = 10.2, rs4 = 11.1;
  cx1 = 4.1, cy1 = 2.2;
  cx2 = 5.8, cy2 = 1.7;
  cx3 = 5.8, cy3 = 3.5;
  cx4 = 1.1, cy4 = -1.5;
  sp4 = 2, sp5 = 3;
    break;
  case 3:
  rs1 = 8.1, rs2 = 8.5, rs3 = 15.4, rs4 = 17.1;
  cx1 = 8.1, cy1 = 4.3;
  cx2 = 5.8, cy2 = 3.3;
  cx3 = 11.7, cy3 = 7.0;
  cx4 = 2.1, cy4 = -1.5;
  sp4 = 2, sp5 = 3;
    break;
  case 4:
  rs1 = 12.1, rs2 = 12.5, rs3 = 20.4, rs4 = 22.1;
  cx1 = 12.1, cy1 = 6.3;
  cx2 = 8.8, cy2 = 4.8;
  cx3 = 16.9, cy3 = 10.5;
  cx4 = 3.1, cy4 = -2.1;
  sp4 = 2, sp5 = 3;
    break;
  case 5:
  rs1 = 16.3, rs2 = 23.0, rs3 = 40.8, rs4 = 44.2;
  cx1 = 16.1, cy1 = 8.7;
  cx2 = 11.6, cy2 = 6.5;
  cx3 = 23.4, cy3 = 14.0;
  cx4 = 4.1, cy4 = -2.9;
  sp4 = 2, sp5 = 3;
    break;
  case 6:
  rs1 = 24.6, rs2 = 34.5, rs3 = 15.4, rs4 = 66.4;
  cx1 = 34.2, cy1 = 23.4;
  cx2 = 35.2, cy2 = 19.9;
  cx3 = 33.8, cy3 = 42.0;
  cx4 = 12.2, cy4 = -5.6;
  sp4 = 2, sp5 = 3;
    break;
  case 7:
  rs1 = 32.6, rs2 = 46.0, rs3 = 20.4, rs4 = 88.4;
  cx1 = 32.2, cy1 = 17.4;
  cx2 = 23.2, cy2 = 13.0;
  cx3 = 23.4, cy3 = 28.0;
  cx4 = 8.2, cy4 = -5.6;
  sp4 = 2, sp5 = 3;
    break;
  case 8:
  rs1 = 65.2, rs2 = 92.0, rs3 = 40.8, rs4 = 176.8;
  cx1 = 64.4, cy1 = 34.8;
  cx2 = 46.4, cy2 = 26.0;
  cx3 = 46.8, cy3 = 56.0;
  cx4 = 16.4, cy4 = -11.2;
  sp4 = 2, sp5 = 3;
    break;
  case 9:
  rs1 = 97.8, rs2 = 138.0, rs3 = 61.2, rs4 = 265.2;
  cx1 = 96.6, cy1 = 52.2;
  cx2 = 69.6, cy2 = 39.0;
  cx3 = 70.2, cy3 = 84.0;
  cx4 = 24.6, cy4 = -16.8;
  sp4 = 2, sp5 = 3;
    break;
  case 10:
  rs1 = 130.4, rs2 = 184.0, rs3 = 81.6, rs4 = 353.6;
  cx1 = 128.8, cy1 = 69.6;
  cx2 = 92.8, cy2 = 52.0;
  cx3 = 93.6, cy3 = 112.0;
  cx4 = 32.8, cy4 = -22.4;
  sp4 = 3, sp5 = 4;
    break;
  default:
  rs1 = 130.4, rs2 = 184.0, rs3 = 81.6, rs4 = 353.6;
  cx1 = 128.8, cy1 = 69.6;
  cx2 = 92.8, cy2 = 52.0;
  cx3 = 93.6, cy3 = 112.0;
  cx4 = 32.8, cy4 = -22.4;
  sp4 = 5, sp5 = 6;
    break;
  }
}
void getColor(long hue) {
  // Hue
  hue %= 1791; // -1791 to +1791
  if (hue < 0)
    hue += 1791;      //     0 to +1791
  lo = hue & 255;     // Low byte  = primary/secondary color mix
  switch (hue >> 8) { // High byte = sextant of colorwheel
  case 0:
    r = 255;
    g = 255 - lo;
    b = 255 - lo;
    break; // W to R
  case 1:
    r = 255;
    g = lo;
    b = 0;
    break; // R to Y
  case 2:
    r = 255 - lo;
    g = 255;
    b = 0;
    break; // Y to G
  case 3:
    r = 0;
    g = 255;
    b = lo;
    break; // G to C
  case 4:
    r = 0;
    g = 255 - lo;
    b = 255;
    break; // C to B
  case 5:
    r = lo;
    g = 0;
    b = 255;
    break; // B to M
  default:
    r = 255;
    g = lo;
    b = 255;
    break; // M to W
  }
}
void getmesg()
{
    xps = 32;
    switch(msg)
    {
    case 0 :
        gettemphumi();
        textmsg = Message;
        msg++;
        break;
    case 1 :
        textmsg = rtc.getTime("%H:%M");
        msg++;
        break;
    case 2 :
        textmsg = "Temperature: ";
        textmsg += temp;
        textmsg += "°C";
        msg++;
        break;
    case 3 :
        textmsg = rtc.getTime("%H:%M");
        msg++;
        break;
    case 4 :
        textmsg = "Humidity: ";
        textmsg += humi;
        textmsg += "%";
        msg++;
        break;
    case 5 :
        textmsg = rtc.getTime("%H:%M");
        msg++;
        break;
    case 6 :
        textmsg = rtc.getTime("%A, %d %B %Y");
        msg++;
        break;
    case 7 :
        textmsg = rtc.getTime("%H:%M");
        msg = 0;
        break;
    case 8 :
        textmsg = textip;
        msg = 0;
        break;
    }
}
  void gettemphumi() {
  dht.readSensor();
  humi = dht.getHumidity();
  temp = dht.getTemperature();
}
  void getdata() {
  ag1   += 0.01;
  ag2   -= 0.03;
  ag3   += 0.5;
  ag4   -= 0.7;
  hueShift ++;
}
void getblat() {
  sx1 = (int)(cos(ag1) * rs1 + cx1);
  sx2 = (int)(cos(ag2) * rs2 + cx2);
  sx3 = (int)(cos(ag3) * rs3 + cx3);
  sx4 = (int)(cos(ag4) * rs4 + cx4);
  dy1 = (int)(sin(ag1) * rs1 + cy1);
  dy2 = (int)(sin(ag2) * rs2 + cy2);
  dy3 = (int)(sin(ag3) * rs3 + cy3);
  dy4 = (int)(sin(ag4) * rs4 + cy4);
  hueShift = xps*10;
  for(int y=0; y<8; y++) {
  dx1 = sx1; dx2 = sx2; dx3 = sx3; dx4 = sx4;
  for(int x=0; x<32; x++) {
  int c0 = canvasF.getPixel(x, y);
  if (c0 != 0) {
      value = hueShift
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((dx1 * dx1 + dy1 * dy1) >> sp4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((dx2 * dx2 + dy2 * dy2) >> sp4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((dx3 * dx3 + dy3 * dy3) >> sp5))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((dx4 * dx4 + dy4 * dy4) >> sp5));
  getColor(value*3);
  pixels.setPixelColor(XY(x, y), pixels.Color(r,g,b));
    } else {  
  pixels.setPixelColor(XY(x, y), pixels.Color(0,0,0));
  }
      dx1--; dx2--; dx3--; dx4--;
    }
    dy1--; dy2--; dy3--; dy4--;
  }
    pixels.show();
}
void scroll_text(String txt) {
  uint16_t text_length = txt.length() * 7; 
    canvasF.fillScreen(0);
    canvasF.setFont(&atawi8x6);
    canvasF.setCursor(xps, 0);
    canvasF.print(txt);
    canvasF.setFont();
     xps--;
     if (xps < -text_length) {getmesg();}
  }
void getTim() {
  if (flasher) {
    canvasF.fillScreen(0);
    canvasF.setFont(&atawi8x6);
    canvasF.setCursor(1, 0);
    canvasF.print(rtc.getTime("%H:%M"));
    canvasF.setFont();
  } else  { canvasF.fillRect(15, 0, 2, 8, 0x0000); }
}
  void handleRoot() {
 server.send(200, "text/html", MAIN_page); //Send web page
}
void handleRestesp() {  
    handleRoot();
    delay(1000);     
    resetFunc();
}
void handlesaveTime() { 
    handleRoot();
    SaveStting();
    writeTime();
}
void handlentpTime() {
    handleRoot();
  if (wifiok) { 
  configTime(Tz * 3600, 0, "145.238.203.10", "145.238.203.14");
    while(!time(nullptr)){
        delay(100);
    }
  }
}
void handlezoneTime() {
    handleRoot();
    Tz = server.arg("locatz").toInt(); 
    configTime(Tz * 3600, 0, "", "");
}
void handlelocaltime() { 
    handleRoot();
    rtc.setTime(server.arg("locadt").toInt());  
    configTime(Tz * 3600, 0, "", "");
}
void handleMyTime() { 
    handleRoot(); 
    String hbuf = server.arg("htmie"); 
    String dbuf = server.arg("ddate");
    text = hbuf[0];
    text += hbuf[1];
    int h = text.toInt(); 
    text = hbuf[3];
    text += hbuf[4];
    int m = text.toInt(); 
    text = hbuf[6];
    text += hbuf[7];
    int s = text.toInt();
    
    text = dbuf[0];
    text += dbuf[1];
    text += dbuf[2];
    text += dbuf[3];
    int yr = text.toInt(); 
    text = dbuf[5];
    text += dbuf[6];
    int mo = text.toInt();
    text = dbuf[8];
    text += dbuf[9];
    int dd = text.toInt();
    rtc.setTime(s, m, h, dd, mo, yr);
}
void handleMode() { 
    handleRoot();
    Modeclock = !Modeclock;
    canvasF.fillScreen(0);
  }
void handleSpe0() {
    handleRoot();
    sp0 = server.arg("Speed0").toInt();
  }
void handleSpe1() {
    handleRoot();
    sp1 = server.arg("Speed1").toInt();
  }
void handleColor() { 
    handleRoot();
    sp2 = server.arg("Color0").toInt();
    Colorplat(sp2);
  }
void handleBright() {
    handleRoot();
    sp3 = server.arg("Bright").toInt();
    pixels.setBrightness(sp3);
  }
void handleSpe4() { 
    handleRoot();
    sp4 = server.arg("Speed4").toInt();
  }
void handleSpe5() { 
    handleRoot();
    sp5 = server.arg("Speed5").toInt();
  }
void handleMesg() { 
    handleRoot();
    Message = server.arg("Message");
    myfile = LittleFS.open("/Message.txt", "w");
    if(myfile){ 
      myfile.print(Message);
    }
    myfile.close();
    msg = 0, getmesg();
  }
void handleWifi() { 
  handleRoot(); 
  Ssid = server.arg("usname");
  Pass = server.arg("pssw");
  getWifi();
  if (wifiok) Savewifi();
}

void readSttWifi() {
  myfile = LittleFS.open("/Stt-wifi.txt", "r");
  String textwifi = "";
  if(myfile) {
    while (myfile.available()) {
      int savednum = myfile.read();
      textwifi += (char)savednum;
     }
  }
  myfile.close();
  readsource(textwifi);
}
void Savewifi() {
  myfile = LittleFS.open("/Stt-wifi.txt", "w");
  if (myfile) {
    String textw =  "\nSsid=";
           textw += Ssid;
           textw += "\nPass=";
           textw += Pass;
           textw += "\n";
    myfile.print(textw);
  }
  myfile.close();
}
void readSttspeed() {
  myfile = LittleFS.open("/Sttingsp.txt", "r");
  String text = "";
  if(myfile) {
    while (myfile.available()) {
      int savednum = myfile.read();
      text += (char)savednum;
     }
  }
  myfile.close();
  readsource(text);
}
void SaveStting() {
  myfile = LittleFS.open("/Sttingsp.txt", "w");
  if(myfile){ 
    String text =  "nmymsge=";
           text += Message;
           text +=  "speed0=";
           text += sp0;
           text += "\nspeed1=";
           text += sp1;
           text += "\nspeed2=";
           text += sp2;
           text += "\nspeed3=";
           text += sp3;
           text += "\nmytimz=";
           text += Tz;
           text += "\nmAuto=";
           text += Modeclock;
           text += "\n";
    myfile.print(text);
  }
  myfile.close();
}
void readsource(String source) {
  String line = "";
  int k = source.length();
  for(int j=0; j<k; j++) {
    if (source[j] == '\n') {
      readLine(line);
      line = "";
      } else
    line += source[j];
  }
}
    void readLine(String Line) {
      if (Line.startsWith("Ssid=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Ssid = Line;
        }
      }
      else if (Line.startsWith("Pass=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Pass = Line;
        }
      }
      else if (Line.startsWith("mymsge=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Message = Line;
        }
      }
      else if (Line.startsWith("speed0=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp0 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed1=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp1 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed2=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp2 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed3=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp3 = Line.toInt();
        }
      }
      else if (Line.startsWith("mytimz=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Tz = Line.toInt();
        }
      }
      else if (Line.startsWith("mAuto=")) {
        Line = Line.substring(6);
        if (Line.length() > 0) {
          Modeclock = Line.toInt();
        }
      }
  }
void setup() {
  rtc.DSbegin(1, 3);
  updateTime();
  LittleFS.begin();
  readSttWifi();
  WiFi.mode(WIFI_AP_STA);
  getWifi();
  readSttspeed();
  pixels.begin(); 
  canvasF.setTextWrap(false); 
  canvasF.fillScreen(0);
  server.on("/", handleRoot); 
  server.on("/MesBright", handleBright);
  server.on("/MesColor", handleColor); 
  server.on("/MesMode", handleMode);  
  server.on("/readtemhu", handlestate); 
  server.on("/MesSpeed0", handleSpe0);
  server.on("/MesSpeed1", handleSpe1); 
  server.on("/MesSpeed4", handleSpe4); 
  server.on("/MesSpeed5", handleSpe5);
  server.on("/myMesg", handleMesg);  
  server.on("/Mywifi", handleWifi); 
  server.on("/ntptime", handlentpTime); 
  server.on("/mytimezon", handlezoneTime); 
  server.on("/localdatime", handlelocaltime);
  server.on("/restime", handleMyTime);
  server.on("/savetime", handlesaveTime); 
  server.on("/restesp", handleRestesp);
  server.begin();
  Colorplat(sp2); 
  pixels.setBrightness(sp3);
  pixels.show(); 
  delay(2000);
  dht.setup(DHTPIN, EspDHT::DHT11);
  gettemphumi();
}
void loop() {
  server.handleClient(); 
   if(millis() - lastTime >= sp0 && !Modeclock) {
      scroll_text(textmsg);
      getblat();
      lastTime = millis();
    } 
   if(millis() - lastTime >= 1000 && Modeclock) {
      getTim();
      flasher = !flasher; 
      lastTime = millis();
    }
   if(millis() - prevTime >= sp1 && !Modeclock) {
      getdata();
      prevTime = millis();
    }
   if(millis() - prevTime >= sp1 && Modeclock) {
      getdata();
      getblat();
      prevTime = millis();
    }
}
void handlestate() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += temp;
  content += "</analog><analog>";
  content += humi;
  content += "</analog><analog>";
  content += Tz;
  content += "</analog><analog>";
  content += Modeclock;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
void updateTime()
{
  rtc.DSgetdatime(); 
  configTime(Tz * 3600, 0, "", "");
} 
void writeTime()
{
  configTime(0, 0, "", ""); 
  rtc.DSsetdatime();
  configTime(Tz * 3600, 0, "", "");
  Serial.println("Time write");
}
