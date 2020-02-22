#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8); // CE, CSN
const byte address[][6] = {"00001", "00002"};
String veri="bos",veri2="Bitti";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[0]);//00001
  radio.openReadingPipe(1, address[1]);//00002
  radio.setPALevel(RF24_PA_MAX);
}
void loop() {
  delay(5);
  radio.startListening();
  while (!radio.available()){
    radio.read(&veri,sizeof(veri));
     Serial.println(veri);
    if (veri.equals("basla")) {
      for(int i = 0; i <= 10; i ++){
        Serial.write("İşlem Yapılıyor = "+i);
      }
    }
    else {
      Serial.write("Veri Alınmadı ---- Hata");
    }
  }
  delay(5);
  radio.stopListening();
  radio.write(&veri2, sizeof(veri2));
  Serial.write("Veri Gönderildi");
}
