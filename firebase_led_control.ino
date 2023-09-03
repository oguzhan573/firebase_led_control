#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <DHT.h>

//1. Firebase veritabanı adresini, Token bilgisini ve ağ adresi bilgilerinizi giriniz.
#define FIREBASE_HOST "" // http:// veya https:// olmadan yazın  without http or https://
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define DHTPIN D5     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
int ledler= D2;
float temp,hum;


//2. veritabanim adında bir firebase veritabanı nesnesi oluşturuyoruz
FirebaseData veritabanim;


void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Ağ Bağlantısı Oluşturuluyor");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP adresine bağlanıldı: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  //3. Firebase bağlantısı başlatılıyor

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();

  //4. Ağ bağlantısı kesilirse tekrar bağlanmasına izin veriyoruz
  Firebase.reconnectWiFi(true);
 pinMode(ledler,OUTPUT);
}

void loop()
{     temp= dht.readTemperature();
        hum=dht.readHumidity();
      Serial.print(" %, Temp: ");
      Serial.print(temp);
       Serial.print(" %, nem: ");
      Serial.print(hum);

  if(Firebase.getString(veritabanim, "/led")) //Alınacak veri tipine göre getInt, getBool, getFloat, getDouble, getString olarak kullanılabilir.
  {
    //bağlantı başarılı ve veri geliyor ise
    //Serial.print("String tipinde veri alımı başarılı, veri = ");
    //Serial.println(veritabanim.stringData());
    if (veritabanim.stringData()=="1")
    {
      digitalWrite(ledler,HIGH);
      Serial.println(veritabanim.stringData());
    }
    else 
    {
      
      digitalWrite(ledler,LOW);
      Serial.println(veritabanim.stringData());
      }
    
    

  }
  else{
    //hata varsa hata mesajı ve nedeni yazdırılıyor

    Serial.print("Str verisi çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }
 //firebase veritabanına veri göndermek için Firebase.setInt komutu kullanılabilir.
if(Firebase.setInt(veritabanim, "/sicaklik", temp))
 {
    //bağlantı başarılı ve veri geliyor ise
     Serial.println("sicaklik veri gönderimi başarılı");
 
  }
  else
  {
    //hata varsa hata mesajı ve nedeni yazdırılıyor

    Serial.print("Int tipindeki veri gönderilemedi, ");
   Serial.println(veritabanim.errorReason());
  }
  if(Firebase.setInt(veritabanim,"/nem",hum))
  {
    Serial.println("nem veri gönderimi başarılı");
  }
  else
  {

    Serial.print("Int tipindeki veri gönderilemedi, ");
   Serial.println(veritabanim.errorReason());
  }

}
