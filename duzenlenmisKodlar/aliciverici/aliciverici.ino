//ARDUİNO UNO

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define solBUT 2
#define ileriBUT 3
#define sagBUT 4
#define durumBUT 5


//Kodların bu kısmı haritalama robotunda uno için yazılmıştır




RF24 radio(7, 8); // CE, CSN
const byte address[][6] = {"00001", "00002"};
String STR_yollanacakVeri;
bool oku_durum = false;
String veri = "";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[1]);//00001
  radio.openReadingPipe(0, address[0]);//00002
  radio.setPALevel(RF24_PA_MAX);

  pinMode(durumBUT, INPUT);
  pinMode(solBUT, INPUT);
  pinMode(ileriBUT, INPUT);
  pinMode(sagBUT, INPUT);



}
void loop() {


  if (oku_durum == false)
  {
    //Eğer Arduino Megadan bana veri gelmiyorsa
    //Ben kumanda modundayım.
    if (digitalRead(solBUT) == 1 )
    {
      veri = "?l";
      delay(5);
      radio.stopListening();
      STR_yollanacakVeri = veri;
      haberlesme_veri(STR_yollanacakVeri);

      delay(5);
    }
    else if (digitalRead(ileriBUT) == 1)
    {
      veri = "?u";
      delay(5);
      radio.stopListening();
      STR_yollanacakVeri = veri;
      haberlesme_veri(STR_yollanacakVeri);

      delay(5);
    }
    else if (digitalRead(sagBUT) == 1)
    {
      veri = "?r";
      delay(5);
      radio.stopListening();
      STR_yollanacakVeri = veri;
      haberlesme_veri(STR_yollanacakVeri);

      delay(5);
    }

    else if (digitalRead(durumBUT) == 1)
    {
      veri = ":s";
      
      delay(5);
      radio.stopListening();
      STR_yollanacakVeri = veri;
      haberlesme_veri(STR_yollanacakVeri);
      oku_durum = true;

      delay(5);
    }
    else
    {

    }

  }

  else
  {
    //Seri kanaldan haber alınacak
    //Durum değişme değişkeni beklenecek.
    radio.startListening();
    while (radio.available()) {

      char text[32] = "!e";
      radio.read(&text, sizeof(text));
      String gelenVeri = String(text);
      Serial.print("Gelen Veri: "); Serial.println(text);
      if(gelenVeri=="!e")
      {
        oku_durum = false;
        radio.stopListening();
        
      }

    }
  }

  delay(100);






}

void haberlesme_veri(String hab_veri) {
  int a = hab_veri.length() + 1;
  char text[a];
  hab_veri.toCharArray(text, hab_veri.length() + 1);
  radio.write(&text, sizeof(text));
  Serial.println(text);
}
