#include <Encoder.h>

int RPWM = 3; //driver RPMW
int LPWM = 4; //driver LPMW
int L_EN = 7; //driver EN L
int R_EN = 8; //driver EN R
Encoder myEnc(22, 23);

//position
unsigned long oldPosition = 0;
unsigned long newPosition;

//distance
unsigned long dMax = 50000;
unsigned long dA;
unsigned long dC;

//speed
int velocity = 40; //start speed
float vMin = 3000;
float vMax;        
float vAvg;
float spOld = 0;
float spMax = 101;
int spList[36] = {42,44,48,51,54,58,62,65,70,73,78,
                  82,85,88,92,96,100,104,108,111,115,
                  119,123,127,131,134,139,142,147,151,
                  154,160,164,167,171,174};

struct speedData{
  int sp;
  int spPos;   
};

struct speedData spd;

int steps;

//time
int t = 1000;         //constant (milli)
int a = 100;          //acceleration time
float tA;               //time to max speed;
float tC;               //time in C section probably will increase
float tT;               //total time
unsigned long tn = 0; //time now

//indexes
int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (2500000);

  analogWriteFrequency(3, 19000);  //set pwm output frequency
  analogWriteFrequency(4, 19000);  //set pwm output frequency

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  digitalWrite(7, HIGH); //enable driver
  digitalWrite(8, HIGH); //enable driver

  pinMode(14, INPUT);     // input from rc teensy digital in 14-->>19
  //pinMode(15, INPUT);
  //pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}

struct speedData speedSearch(int speedList[], int speedMax){
  
  struct speedData sd;
  int lo = 0;
  int hi = 35;
  int mid;

  if (speedMax < speedList[lo]){
    sd.sp = speedList[lo];
    sd.spPos = lo;
    return (sd);
  }

  if (speedMax > speedList[hi]){
    sd.sp = speedList[hi];
    sd.spPos = hi;
    return (sd);
  }

  while (lo <= hi){
    mid = (int)((lo + hi) / 2);

    if (speedMax < speedList[mid]){
      hi = mid - 1;
    } else if (speedMax > speedList[mid]){
      lo = mid + 1;
    } else {
      sd.sp = speedList[mid];
      sd.spPos = mid;
      return(sd);
    }
  }

  if (speedList[lo] - speedMax < speedMax - speedList[hi]){
    sd.sp = speedList[lo];
    sd.spPos = lo;
    return(sd);
  } else {
    sd.sp = speedList[hi];
    sd.spPos = hi;
    return(sd);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  /*
  while (tn <= 100000) {
    newPosition = myEnc.read();
    if (millis() > tn + t) {
      i += 1;
      if (i == 5){
        i = 0;
        velocity += 1;
        Serial.print("Speed: ");
        Serial.println(velocity);
        analogWrite(RPWM, velocity);
      }
      tn = millis();
      //Serial.print("Time: ");
      //Serial.println(tn/1000);
      if (newPosition/100 != oldPosition/100) {
         Serial.println(newPosition-oldPosition);
         oldPosition = newPosition;
         //Serial.print("Pos: ");
         //Serial.println(newPosition);
      } 
    }
  }

  if (newPosition/100 != oldPosition/100) {
      oldPosition = newPosition;
      Serial.print("Pos: ");
      Serial.println(newPosition);
  }
    
  analogWrite(RPWM, 0);
 */

 
 
  //Serial.println(spOld);
  if (spOld != spMax){
    spOld = spMax;
    spd = speedSearch(spList, spMax);
    steps = spd.spPos;

    vMax = steps * 500 + vMin;
    vAvg = (vMin + vMax) / 2;
    tA = steps * a;       // time to maximum speed
    dA = vAvg * tA / 1000;
    dC = (int)(dMax - (2 * dA)); //dB = dA
    tC = (int)(dC / vMax * 1000); // time in C distance
    tT = (int)(2 * tA + tC);
  }

  if (millis() == 5000){
    Serial.print("Speed: ");
    Serial.println(spd.sp);
    Serial.print("steps: ");
    Serial.println(spd.spPos);
    Serial.print("vMax: ");
    Serial.println(vMax);
    Serial.print("vAvg: ");
    Serial.println(vAvg);
    Serial.print("tA: ");
    Serial.println(tA);
    Serial.print("dA: ");
    Serial.println(dA);
    Serial.print("dC: ");
    Serial.println(dC);
    Serial.print("tC: ");
    Serial.println(tC);
    Serial.print("tt: ");
    Serial.println(tT);
  }
}
