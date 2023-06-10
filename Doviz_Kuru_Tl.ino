#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

const char* ssid     = "your-ssid";
const char* password = "your-password";

void setup() {
  Serial.begin(115200); //Seri monitörü başlatır
  //Wifi bağlantısı kurma
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi bağlandı!");
}

void loop() {
  // HTTPS isteği gönder
    WiFiClientSecure client;
    client.setInsecure(); 
    HTTPClient https;
  https.begin(client, "https://api.genelpara.com/embed/doviz.json"); // API adresini buraya yazın
  int httpCode = https.GET();

  // Eğer istek başarılıysa, cevabı bir değişkende sakla
  String response;
  if (httpCode == HTTP_CODE_OK) {
    response = https.getString();
  }

  // İstek başarısız olduysa, hata mesajını yazdır
  else {
    Serial.printf("Hata %d\n", httpCode);
  }

  // İstekten dönen veriyi yazdır
  Serial.println(response);

DynamicJsonDocument doc(1536);

DeserializationError error = deserializeJson(doc, response);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}
  
//Değeler değişkenlere atanıp seri monitöre yazdırılır  
JsonObject USD = doc["USD"];
const char* USD_satis = USD["satis"];
const char* USD_alis = USD["alis"]; 

JsonObject EUR = doc["EUR"];
const char* EUR_satis = EUR["satis"];
const char* EUR_alis = EUR["alis"]; 
  
JsonObject GA = doc["GA"];
const char* GA_satis = GA["satis"];
const char* GA_alis = GA["alis"];   

Serial.print("Dolar satış:");
Serial.print(USD_satis);
Serial.println(" TL");

Serial.print("EURO satış:");
Serial.print(EUR_satis);
Serial.println(" TL");
  
Serial.print("Altın satış:");
Serial.print(GA_satis);
Serial.println(" TL");  

  https.end();
  // Bir sonraki istek için bekleme süresi
  delay(10000); // 1 dakika
}
