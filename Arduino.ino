#ifdef ESP32
#include <WiFi.h>
#else
 #include <ESP8266WiFi.h>
 #endif
 #include<WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
 #include <ArduinoJson.h>
 #include <Servo.h>
 Servo myservo;
 const char* ssid = "quiero chettos";
 const char* password= "churro12";
 #define BOTtoken "5625150420:AAFO-2WLIXn9Vg2O60ZbiDWXgwqip2_aoqY"
 #define CHAT_ID "1768307042"
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 100;
unsigned long lastTimeBotRan;
void handleNewMessages(int numNewMessages){
 
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario no Autorizado", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start" or text == "7iniciar") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Utiliza los comandos C:.\n\n";
      welcome += "/Encender \n";
      welcome += "/Apagar \n";
      bot.sendMessage(chat_id, welcome, "");
      myservo.attach(15);
      myservo.write(0);
    }

  
    if (text == "/Encender"){
      bot.sendMessage(chat_id, "listo,cruck","");
      myservo.write(180);
      delay(5000);
      myservo.write(0);
    }
    
    if (text == "/Apagar") {
      bot.sendMessage(chat_id, "Valor = 0", "");
      myservo.write(0);
      myservo.detach();
    }
  }
}

void setup() {
  Serial.begin(115200);
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
