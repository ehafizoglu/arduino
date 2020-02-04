#include <dht11.h>

String agAdi = "XXX";
String agSifresi =  "XXXX";

String statikIP = "192.168.1.35";
String gateway =  "192.168.1.1";
String netmask =  "255.255.255.0";



String ip = "184.106.153.149"; //Thingspeak ip adresi
float sicaklik, nem;

int dht11Pin = 2;

dht11 DHT11;

void setup()
{


  Serial.begin(115200);
  Serial.println("Bağlantı süreci başlatıldı.");

  Serial.println("AT");
  delay(2000);
  //  Serial.println(Serial.readString());

  if (!Serial.find("OK"))

  {
    Serial.println("AT komutundan OK dönmedi. Denemeye başla ve OK Dönene Kadar Devam Et.");
  }


  while (!Serial.find("OK")) {
    Serial.println("AT");
    delay(2000);
  }

  Serial.println("OK Komutu Alındı");
  Serial.println("AT+CWMODE=1");
  while (!Serial.find("OK")) {                                   //Ayar yapılana kadar bekliyoruz.
    Serial.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  delay(2000);
  Serial.println("Client olarak ayarlandı");
  delay(2000);
  Serial.println("DHCP modu disable ediliyor...");
  Serial.println("AT+CWDHCP=1,0");
  delay(2000);
  while (!Serial.find("OK")) {                                   //Ayar yapılana kadar bekliyoruz.
    Serial.println("AT+CWDHCP=1,0");
    Serial.println("DHCP Ayarı Yapılıyor....");
  }
  delay(2000);
  Serial.println("DHCP modu Disable edildi...");

  delay(2000);
  Serial.println("IP statik olarak \"" + statikIP + "\" olarak ayarlanacak...");
  Serial.println("AT+CIPSTA_CUR=\"" + statikIP + "\",\"" + gateway + "\",\"" + netmask + "\"");
  delay(2000);
  while (!Serial.find("OK")) {                                   //Ayar yapılana kadar bekliyoruz.
    Serial.println("AT+CIPSTA_CUR=\"" + statikIP + "\",\"" + gateway + "\",\"" + netmask + "\"");
    Serial.println("IP Statik Olarak ayarlanıyor....");
  }
  delay(2000);
  Serial.println("Ip Statik olarak ayarlandı...");
  delay(2000);
  Serial.println("Aga Baglaniliyor...");
  Serial.println("AT+CWJAP=\"" + agAdi + "\",\"" + agSifresi + "\""); //Ağımıza bağlanıyoruz.
  while (!Serial.find("WIFI CONNECTED")) {                                   //Ayar yapılana kadar bekliyoruz.
    Serial.println("AT+CWJAP=\"" + agAdi + "\",\"" + agSifresi + "\"");
    Serial.println("Ağa Bağlanılmaya çalışılıyor....");
  }
  delay(1000);
  Serial.println("Ağa bağlanıldı...");
  delay(2000);
  // Serial.println("AT+CIPSTA?");
  //  Serial.println(Serial.readString());
  //  Serial.println("");


}

void loop() {
  //loop içinde loop oldu


  Serial.println("Thingspeak'e bağlanıyoruz...");
  Serial.println("AT+CIPSTART=\"TCP\",\"" + ip + "\",80");       //Thingspeak'e bağlanıyoruz.

  //  while(Serial.find("ERROR")) {                                    //Bağlantı hatası kontrolü yapıyoruz.
  //   Serial.println("AT+CIPSTART=\"TCP\",\"" + ip + "\",80");
  //  Serial.println("Thingspeak'e bağlanmaya çalışıyoruz...");
  // }

 // Serial.println(Serial.readString());
  if (Serial.find("OK") || Serial.find("ALREADY CONNECTED") ) {                                    //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("Thingspeak'e bağlandık...");

    DHT11.read(dht11Pin);
    sicaklik = (float)DHT11.temperature;
    nem = (float)DHT11.humidity;
    Serial.println("Sıcaklık değeri: " + String(sicaklik));
    Serial.println("Nem değeri: " + String(nem));

    String veri = "GET https://api.thingspeak.com/update?api_key=85LU6BYHB7A7H5A4";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.
    veri += "&field1=";
    veri += String(sicaklik);
    veri += "&field2=";
    veri += String(nem);                                        //Göndereceğimiz nem değişkeni
//    veri += " HTTP/1.1";
    veri += "\r\n\r\n";

    //Serial.println("");
    Serial.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
    Serial.println(veri.length());
    delay(30000);

    // Serial.println(Serial.readString());

    if (Serial.find(">") || Serial.find("OK")) {    //ESP8266 hazır olduğunda içindeki komutlar çalışıyor. || işareti or anlamında
     Serial.print(veri);                                          //Veriyi gönderiyoruz.
      Serial.println(veri);
     // Serial.println("Host: https://api.thingspeak.com");
      delay(30000);
    }


    else Serial.println("Veri Gönderilemedi");


  }


  else

    Serial.println("Thingspeak'e bağlanamadık. Tekrar Dene...");




  Serial.println("Baglantı kapatılmaya çalışılacak.");
  Serial.println("AT+CIPCLOSE");                                //Bağlantıyı kapatıyoruz
  delay(30000);                                               //Yeni veri gönderimi için 1 dakika bekliyoruz.

   // Serial.println(Serial.readString());
    if (!Serial.find("CLOSED")) {                                
      Serial.println("Bağlantı kapatılamadı...");
      delay(30000);
    }



else       Serial.println("Bağlantı kapatıldı...");
    
}
