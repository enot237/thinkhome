#include <DHT_U.h>
#include <DHT.h>
boolean auto_mode = false; //Автоматический режим [on|off]
int incomingByte = 0;   // переменная для хранения полученного байта
const int phototransistor_pin = 3; //пин подключения датчика света
const int room_lamp_pin = 8; // Пин подключения лампы
boolean room_lamp = false; //переменная состояния лампы
const int room_light_pin = 11;//ШИМ пин первого канала (комната)
boolean room_light = false; //переменная состояния света в комнате
int room_light_volume = 0; //яркость освещения комнаты
int table_light_pin = 10;//ШИМ пин второго канала (стол)
boolean table_light = false; //переменная состояния света на столе
int table_light_volume = 0; //яркость освещения на столе
#define DHTPIN 2 // номер пина, к которому подсоединен датчик
DHT dht(DHTPIN, DHT22); //объявление пина с датчиком dht22

void setup() {   
    pinMode(phototransistor_pin, INPUT);
    dht.begin(); //запуск dht
    Serial.begin(9600); // устанавливаем последовательное соединение
}
void lightning(int pin, int volume){ //int pin=pwm pin, int volume=max volume
  for(int i=0;i<volume;i++){
    analogWrite(pin, i); //slow lightning
    delay(5);}}
void turning_off(int a, int c){ //a=pwm pin, c=max volume
  for(int i=c;i>=0;i--){
    analogWrite(a, i);//slow fogging
    delay(5);}}
void print_state(int a){ //int a = selector room|table
  if(a=='1'){
    Serial.print(room_light);
    Serial.print("|");
    Serial.println(room_light_volume);
    
  }
  else if(a=='2'){
    Serial.print(table_light);
    Serial.print("|");
    Serial.println(table_light_volume);
  }
  else if(a=='4'){
    Serial.print(room_lamp);
    Serial.print("|");
    Serial.println("3");
  }
}
void set_volume(int pin, int volume){
  if(pin=='1'){
    if(room_light){
   if(volume>room_light_volume){
      for(room_light_volume;room_light_volume<volume;room_light_volume++){
        analogWrite(room_light_pin, room_light_volume);
        delay(3);
      }
    }
    else if(volume<room_light_volume){
      for(room_light_volume;room_light_volume>volume;room_light_volume--){
        analogWrite(room_light_pin, room_light_volume);
        delay(3);
  }}}
  else{
    room_light_volume = volume;}
  }
  else if(pin=='2'){
    if(table_light){
    if(volume>table_light_volume){
      for(table_light_volume;table_light_volume<volume;table_light_volume++){
        analogWrite(table_light_pin, table_light_volume);
        delay(3);
      }
    }
    else if(volume<table_light_volume){
      for(table_light_volume;table_light_volume>volume;table_light_volume--){
        analogWrite(table_light_pin, table_light_volume);
        delay(3);
      }
    }
  }
  else{
    table_light_volume = volume;}
  }
}


void loop() {
  if (Serial.available() > 0) {  //если есть доступные данные
        incomingByte = Serial.read();
        if(incomingByte=='1'){//CHANGE STATE OF LIGHTNING
          delay(20);
          if (Serial.available() > 0) {  //если есть доступные данные
              int pin_to_change = Serial.read();
              if(digitalRead(phototransistor_pin)==1){ //если света недостаточно
              Serial.println("C");//changed
              if(pin_to_change=='1'){
                if(room_light==false){
                  lightning(room_light_pin, room_light_volume);
                  room_light=true;
                }
                else{
                  turning_off(room_light_pin, room_light_volume);
                  room_light=false;
                }
              }
              else if(pin_to_change=='2'){
                if(table_light==false){
                  lightning(table_light_pin, table_light_volume);
                  table_light=true;
                }
                else{
                  turning_off(table_light_pin, table_light_volume);
                  table_light=false;
                }
              }
              else if(pin_to_change=='4'){
                if(room_lamp==false){
                  digitalWrite(room_lamp_pin, HIGH);
                  room_lamp=true;
                }
                else{
                  digitalWrite(room_lamp_pin, LOW);
                  room_lamp=false;
                }
              }
              
        }
        else{ //если света в комнате ДОСТАТОЧНО
          Serial.println("state can't be changed");
          }
        }}

        
        else if(incomingByte=='2'){//SET THE VOLUME OF LIGHTNING 
          Serial.println("VS");//volume seted
          delay(20);
          if(Serial.available()>0){
            int line_to_set_the_volume = Serial.read();//select a line room|table
            int volume=0;
                if(Serial.available()>0){
                  volume = volume+(Serial.read()-48)*100;delay(100);}
                  if(Serial.available()>0){
                  volume = volume+(Serial.read()-48)*10;delay(100);}
                  if(Serial.available()>0){
                  volume = volume+(Serial.read()-48);delay(100);}
                  set_volume(line_to_set_the_volume, volume);
              
            }
            
          }
          else if(incomingByte=='3'){//GET STATE OF LINE
             delay(20);
            if(Serial.available()>0){
              int a = Serial.read();
              print_state(a);
            }
          }
          else if(incomingByte=='4'){//SEND WEATHER DATA FROM DHT22
                    float h = dht.readHumidity();
                    float t = dht.readTemperature();
                    Serial.print(t);
                    Serial.print("|");
                    Serial.println(h);
          }
          else if(incomingByte == '5'){
             if(digitalRead(phototransistor_pin)==0){ //если СВЕТА ДОСТАТОЧНО
              Serial.println("1");}
             else{
              Serial.println("0");} //если света МАЛО.
            
             }
          else if(incomingByte == '6'){//проверка авто-режима
            if(auto_mode==false){
              Serial.println("0");}
              else{
              Serial.println("1");}
          }
          else if(incomingByte=='7'){//смена состояния авто-режима (on/off automode)
            auto_mode=!auto_mode;
            Serial.println("AM"); //automode changed
          }
          }
        
        
    
       
        if(digitalRead(phototransistor_pin)==0){ //света много
        if(table_light){
        turning_off(table_light_pin, table_light_volume);
        table_light=false;}
        if(room_light){
        turning_off(room_light_pin, room_light_volume);
        room_light=false;}
        delay(20);
       }
       else if(digitalRead(phototransistor_pin)==1 && auto_mode){ //света мало и авто-режим
        if(table_light==false){
          lightning(table_light_pin, 255);
          table_light_volume = 255;
          table_light=true;}
        if(room_light==false){
        lightning(room_light_pin, 255);
        room_light_volume = 255;
        room_light=true;}
        delay(20);
        }
       }


