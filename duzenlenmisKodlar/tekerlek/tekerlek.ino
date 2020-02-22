const int in1 = 11;     // Motor sürücümüze bağladığımız pinleri tanımlıyoruz
const int in2 = 10;  
const int in3 = 9;     // Motor sürücümüze bağladığımız pinleri tanımlıyoruz
const int in4 = 8;
 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

 int sayac=0;
void setup() 
 
{


  
Serial.begin(9600);
pinMode(in1, OUTPUT); 
pinMode(in2, OUTPUT);
}
void loop() 

{
  motorsag();
}

void motorileri(){
  if (sayac<1)
 {
  motorHizKontrolileri(130);
  sayac++;
  Serial.println(sayac);
  delay(500);
 }

 else
 {
  motorHizKontrolileri(0);
  Serial.println(sayac);
  delay(500);
 }
 sayac=0;
}

void motorsag(){
  if (sayac<1)
 {
  motorHizKontrolsag(100);
  sayac++;
  Serial.println(sayac);
  delay(500);
 }
 else
 {
  motorHizKontrolsag(0);
  Serial.println(sayac);
  delay(500);
 }
}

void motorsol(){
  if (sayac<1)
 {
  motorHizKontrolsol(100);
  sayac++;
  Serial.println(sayac);
  delay(500);
 }

 else
 {
  motorHizKontrolsol(0);
  Serial.println(sayac);
  delay(500);
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
