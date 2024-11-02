/* 
#AUTHOR Дмитрий Витальевич Мельник
 #MAIL i.x.c.o.n@yandex.ru 
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок.  
*/
#include <Servo.h>
Servo myservo;

#define PINECHOONE 6 //A1
#define PINTRIGONE 7 //A0

#define PINECHOTWO 4 //A3
#define PINTRIGTWO 5 //A2

#define PINZUMMER A2

#define PINRELE A3

#define PINLEDONE A0 //4
#define PINLEDTWO A1 //5

uint32_t timer = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;
uint32_t timer3 = 0;

float DistanceONE = 0;
float DistanceTWO = 0;

float MinDistanceONE = 5.0;
float MinDistanceTWO = 5.0;

bool StatusRele = false;
bool StatusZummer = false;
bool StatusDistanceONE = false;
bool StatusDistanceTWO = false;
bool StatusBlinc = false;

//Analiz
bool StatusEchoONE = false;
bool StatusEchoTWO = false;
bool StatusPoezd = false;
int TimerSbros = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("Start OK");
  PlayZummer();
  confing();

}

void loop() {
  if(millis() - timer >= 250){
    timer = millis();
    PrintDistance();
    Control();
  }

  if(millis() - timer1 >= 250){
    timer1 = millis();

  }

  if(millis() - timer2 >= 500){
    timer2 = millis();
    if(StatusBlinc){
      Blinc();
    }
    else{
      BlincStop();
    }
    if(StatusZummer){
      tone(PINZUMMER, 1000, 250);
    }
    if(StatusRele){
      myservo.write(90);
    }
    else{
      myservo.write(0);
    }


  }

  if(millis() - timer3 >= 1000){
    timer3 = millis();
    if(StatusEchoONE && StatusEchoTWO  && !StatusDistanceONE && !StatusDistanceTWO){
      TimerSbros = TimerSbros + 1000;
      Serial.println("Поезд проехал");
      if((TimerSbros > 5000) && !StatusDistanceONE && !StatusDistanceTWO){
        StatusEchoONE = false;
        StatusEchoTWO = false;
        TimerSbros = 0;
        Serial.println("Сброс таймера, поезд проехал");
      }
    }
  }
}

void confing(){
  pinMode(PINECHOONE, INPUT);
  pinMode(PINTRIGONE, OUTPUT);
  pinMode(PINECHOTWO, INPUT);
  pinMode(PINTRIGTWO, OUTPUT);
  pinMode(PINLEDONE, OUTPUT);
  pinMode(PINLEDTWO, OUTPUT);
  pinMode(PINZUMMER, OUTPUT);
  digitalWrite(PINZUMMER, LOW);

  StatusRele = false;
  digitalWrite(PINLEDONE, HIGH);
  digitalWrite(PINLEDTWO, HIGH);

  myservo.attach(PINRELE);
  myservo.write(0);

}

float readDistanceCMONE(){
  digitalWrite(PINTRIGONE, LOW);
  delayMicroseconds(2);
  digitalWrite(PINTRIGONE, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINTRIGONE, LOW);
  int duration = pulseIn(PINECHOONE, HIGH, 20000);
  return duration * 0.034 / 2;
}

float readDistanceCMTWO(){
  digitalWrite(PINTRIGTWO, LOW);
  delayMicroseconds(2);
  digitalWrite(PINTRIGTWO, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINTRIGTWO, LOW);
  int duration = pulseIn(PINECHOTWO, HIGH, 20000);
  return duration * 0.034 / 2;
}

void PrintDistance(){
    DistanceONE = readDistanceCMONE();
    DistanceTWO = readDistanceCMTWO();
    /*
    Serial.print("Дальномер 1: ");
    Serial.print(DistanceONE);
    Serial.println("см.");
    Serial.print("Дальномер 2: ");
    Serial.print(DistanceTWO);
    Serial.println("см.");
    */
}

void Control(){
  if(MinDistanceONE > DistanceONE){
    StatusDistanceONE = true;
  }
  else{
    StatusDistanceONE = false;
  }
  if(MinDistanceTWO > DistanceTWO){
    StatusDistanceTWO = true;
  }
  else{
    StatusDistanceTWO = false;
  }

  if(!StatusEchoONE && StatusDistanceONE){
    StatusEchoONE = true;
    Serial.println("Сработал первый датчик поезда");
  }

  if(!StatusEchoTWO && StatusDistanceTWO){
    StatusEchoTWO = true;
    Serial.println("Сработал второй датчик  поезда");
  }

  



  if(StatusEchoONE || StatusEchoTWO){
    StatusPoezd = true;
  }
  else{
    StatusPoezd = false;
  }

  //StatusEchoONE
  //StatusEchoTWO

  if(StatusPoezd){
    StatusBlinc = true;
    StatusRele = true;
    StatusZummer = true;
  }
  else{
    StatusBlinc = false;
    StatusRele = false; 
    StatusZummer = false; 
  }

}

void Blinc(){
  if(digitalRead(PINLEDONE)){
    digitalWrite(PINLEDONE, LOW);
    digitalWrite(PINLEDTWO, HIGH);
  }
  else{
    digitalWrite(PINLEDONE, HIGH);
    digitalWrite(PINLEDTWO, LOW);
  }
}

void BlincStop(){
  digitalWrite(PINLEDONE, HIGH);
  digitalWrite(PINLEDTWO, HIGH);
}

void PlayZummer(){
  /*
  digitalWrite(PINZUMMER, HIGH);
  delay(300);
  digitalWrite(PINZUMMER, LOW);
  */
  tone(PINZUMMER, 1000, 250);
}

//tone(PINTONE, 1000, 500);
//notone();
