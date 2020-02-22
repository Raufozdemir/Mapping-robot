// ARDUİNO MEGA
//------------ HC - SR04
int trigPin = 40;  // HC - SR04 trig bacağının bağlı olduğu dijital pin
int echoPin = 41;  // HC - SR04 echo bacağının bağlı olduğu dijital pin
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//------------ STEP MOTOR
const int stepPin = 6;
const int dirPin = 7;
const int stepEN = 2;
const int stepms1 = 3;
const int stepms2 = 4;
const int stepms3 = 5;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//İşlamler için gerekli olan değişkenler
unsigned long toplam_uzaklik = 0;   //Ortalama almadan önce tüm uzakliğin toplandiği değişken
unsigned long uzaklik_ortalama = 0; //Ortalama Uzaklığın toplandığı Kodlar
int sure;                           //Mesafe ölçülmesinde kullanılan değişken.
int uzaklik;                        //Ultrasonik cevabın süreye çevrilmiş hali
String tmpMesaj="";                 //Bu mesaj seri ekrana yazmak için kullanılacak değişken 
String STR_olcumSonucu;             //Ölçüm sonucunun STRİNG Hali
String STR_motorAcisi;              //Motorun açısının STRİNG Hali
int INT_motorAcisi=1;               //Motor açısı INTEGER 
String STR_yollanacakVeri;          //Seri port ve NRF ile yollacaca veri
int ortalama_olcum_last=30;
//----------------------------------

const int in1 = 11;     // Motor sürücümüze bağladığımız pinleri tanımlıyoruz
const int in2 = 10;  
const int in3 = 9;     // Motor sürücümüze bağladığımız pinleri tanımlıyoruz
const int in4 = 8;
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(48, 49); // CE, CSN
const byte address[][6] = {"00001", "00002"};
String veri="bos",veri2="Bitti";
int sayac=0;

void setup() {
  pinDurumlari_Ayarlama();
  stepMotor_Driver_RES();
  Serial.begin(9600);
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address[0]);//00001
  radio.openReadingPipe(0, address[1]);//00002
  radio.setPALevel(RF24_PA_MAX);
}
void loop() {
  delay(5);
  radio.startListening();
   while(radio.available()) {
    
    char text[32] = "";
    radio.read(&text, sizeof(text));
    String gelenVeri=String(text);
   
    if(gelenVeri==":s")
    {
      radio.stopListening();
      //Ölçüm yap
      INT_motorAcisi=0;
      while(INT_motorAcisi<370)
      {
        STR_olcumSonucu = OrtalamaOlcumSonucu(15);
        STR_motorAcisi=String(INT_motorAcisi);
        STR_yollanacakVeri="-"+STR_motorAcisi+"*"+STR_olcumSonucu+"_";
        seri0PortPrint(STR_yollanacakVeri);
        haberlesme_veri(STR_yollanacakVeri);
        //NRF ile yazma eksik
        
        step_Motor_Cevir();

        if(INT_motorAcisi==360)
        {
        STR_yollanacakVeri="!e";
        seri0PortPrint(STR_yollanacakVeri);
        haberlesme_veri(STR_yollanacakVeri);
        }
        
        INT_motorAcisi+=20;
        
      }
      radio.startListening();
    }
    else if(gelenVeri=="?l"){
      motorsol();
      motorsol();
      sayac=0;
    }
     else if(gelenVeri=="?r"){
      motorsag();
      motorsag();
      sayac=0;
    }
     else if(gelenVeri=="?u"){
      motorileri();
      motorileri();
      sayac=0;
    }
    
    else{
      Serial.println("Üzgünüm ters giden birşeyler var");
    }
  }
}


/*-------------------Dc Motor Driver---------------------*/
void motorileri(){
  if (sayac<1)
 {
  motorHizKontrolileri(200);
  sayac++;
  Serial.println(sayac);
  delay(150);
 }

 else
 {
  motorHizKontrolileri(0);
  Serial.println(sayac);
 }
}

void motorsag(){
  if (sayac<1)
 {
  motorHizKontrolsag(200);
  sayac++;
  Serial.println(sayac);
  delay(150);
 }
 else
 {
  motorHizKontrolsag(0);
  Serial.println(sayac);
 }
}

void motorsol(){
  if (sayac<1)
 {
  motorHizKontrolsol(200);
  sayac++;
  Serial.println(sayac);
  delay(150);
 }

 else
 {
  motorHizKontrolsol(0);
  Serial.println(sayac);
 }
}

void motorHizKontrolileri(int hiz)
{
 for( int i =hiz; i<hiz+2;i++)
 {
  analogWrite(in1,0);
  analogWrite(in2,i);
  analogWrite(in3,0);
  analogWrite(in4,i);
  Serial.print("HIZ: ");
  Serial.println(i);
  delay(2);
 }
} 

void motorHizKontrolsag(int hiz)
{
 for( int i =hiz; i<hiz+2;i++)
 {
  analogWrite(in1,i);
  analogWrite(in2,0);
  analogWrite(in3,0);
  analogWrite(in4,i);
  Serial.print("HIZ: ");
  Serial.println(i);
  delay(2);
 }
} 

void motorHizKontrolsol(int hiz)
{
 for( int i =hiz; i<hiz+2;i++)
 {
  analogWrite(in1,0);
  analogWrite(in2,i);
  analogWrite(in3,i);
  analogWrite(in4,0);
  Serial.print("HIZ: ");
  Serial.println(i);
  delay(2);
 }
}

/*---------------------Step Motor Ve Ölçümler--------------*/

void haberlesme_veri(String hab_veri){
  int a=hab_veri.length()+1;
  char text[a];
  hab_veri.toCharArray(text, hab_veri.length()+1);
  radio.write(&text, sizeof(text));
  Serial.println(text);
  }

void haberlesmeAyar(){
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}
 


int olcumAl() {
  //Bu fonksiyon kesinlikte LOOP içerisinde çağırılmayacak.
  digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */
  sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  uzaklik = sure / 29.1 / 2; /* ölçülen sure uzaklığa çevriliyor */
  delay(100);
  return uzaklik;
}


String OrtalamaOlcumSonucu(int ort_olc_say) {
  //Bu fonksiyonun içerisinde 10 adet ölçümün ortalamasını alıyoruz ve ortalama sonucunu gönderiyoruz.
  toplam_uzaklik = 0;
  for (int i = 0; i < ort_olc_say; i++) {
    int tmp_uzaklik = olcumAl(); //Gelen ölçümleri toplamak
    toplam_uzaklik = toplam_uzaklik + tmp_uzaklik;
  }
  uzaklik_ortalama = toplam_uzaklik / ort_olc_say;
  if(uzaklik_ortalama>70 && ortalama_olcum_last+10<uzaklik_ortalama && ortalama_olcum_last-10<uzaklik_ortalama)
  {
    toplam_uzaklik=0;
    uzaklik_ortalama=0;
    delay(1);
    OrtalamaOlcumSonucu(ort_olc_say);
  }
  ortalama_olcum_last=uzaklik_ortalama;
  String str_uzaklik_ortalama = String(uzaklik_ortalama);
  return str_uzaklik_ortalama;
}

void step_Motor_Cevir() {
  // Step Motorun 1 derece dönmesini sağlayan blok
  digitalWrite(stepEN, LOW);
  //delayMicroseconds(1000);
  for (int i = 0; i < 15*20; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  delay(1);
  digitalWrite(stepEN,HIGH);
  seri0PortPrint("Step motor 1 derece döndü");
  
}

void pinDurumlari_Ayarlama() {
  //HC - SR 04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //<<<<<<<<<<<<<<<<<<<<

  //A4988
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepEN, OUTPUT);
  pinMode(stepms1, OUTPUT);
  pinMode(stepms2, OUTPUT);
  pinMode(stepms3, OUTPUT);
  //<<<<<<<<<<<<<<<<<<<<<<
  tmpMesaj="Pin Durumları ayarlandı";
  
}

void stepMotor_Driver_RES() {
  digitalWrite(stepEN, LOW);
  digitalWrite(stepms1, LOW);
  digitalWrite(stepms2, LOW);
  digitalWrite(stepms3, LOW);
  digitalWrite(stepEN, HIGH);
  seri0PortPrint("Step Motor Ölçek ayarı yapıldı. Full Step MOD");
  
}

void seri0HaberlesmeAyar(int boutRate){
  //Fonksiyon setup içerisinde kullanılaca

  Serial.begin(boutRate);
  Serial.println("Cihaz Çalışmaya Başladı.");
  
}

void seri0PortPrint(String mesaj){
  Serial.println(mesaj);
}
