#include <Encoder.h>

int RPWM = 3; //driver RPMW
int LPWM = 4; //driver LPMW
int L_EN = 7; //driver EN L
int R_EN = 8; //driver EN R
Encoder myEnc(22, 23);

//position
unsigned long oldPosition = 0;
long newPosition;

//distance
long dMax = 100000;
long dA;
long dC;

//speed
int velocity = 40; //start speed
float vMin = 3000;
float vMax;
float vAvg;
float spOld = 0;
float spMax = 100;
int spListRight[36] = {42, 44, 48, 51, 54, 58, 62, 65, 70, 73, 78,
                       82, 85, 88, 92, 96, 100, 104, 108, 111, 115,
                       119, 123, 127, 131, 134, 139, 142, 147, 151,
                       154, 160, 164, 167, 171, 174
                      };

int spListLeft[36] = {46, 49, 52, 55, 58, 61, 65, 68, 72, 75, 79,
                      83, 87, 91, 95, 99, 103, 107, 110, 114, 118,
                      122, 126, 130, 134, 137, 141, 145, 149, 153,
                      157, 161, 165, 169, 173, 176
                     };

struct speedData {
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
unsigned long tStart = 0;

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

struct speedData speedSearch(int speedList[], int speedMax) {

  struct speedData sd;
  int lo = 0;
  int hi = 35;
  int mid;

  if (speedMax < speedList[lo]) {
    sd.sp = speedList[lo];
    sd.spPos = lo;
    return (sd);
  }

  if (speedMax > speedList[hi]) {
    sd.sp = speedList[hi];
    sd.spPos = hi;
    return (sd);
  }

  while (lo <= hi) {
    mid = (int)((lo + hi) / 2);

    if (speedMax < speedList[mid]) {
      hi = mid - 1;
    } else if (speedMax > speedList[mid]) {
      lo = mid + 1;
    } else {
      sd.sp = speedList[mid];
      sd.spPos = mid;
      return (sd);
    }
  }

  if (speedList[lo] - speedMax < speedMax - speedList[hi]) {
    sd.sp = speedList[lo];
    sd.spPos = lo;
    return (sd);
  } else {
    sd.sp = speedList[hi];
    sd.spPos = hi;
    return (sd);
  }
}

void goRight() {

  Serial.println("here");

  tStart = millis();
  tn = millis();
  i = 0;
  analogWrite(RPWM, spListRight[i]);
  newPosition = myEnc.read();

  while ((tn <= (tStart + tT) && newPosition <= dMax)
         || newPosition <= dMax) {

    newPosition = myEnc.read();

    if (tn <= (tStart + tA)) {
      if (millis() > tn + a) {
        analogWrite(RPWM, spListRight[++i]);
        tn = millis();
        //Serial.print("Pos: ");
        //Serial.println(newPosition);
      }
    }

    if ((tn > (tStart + tA))
        && (tn < (tStart + tA + tC))) {
      tn = millis();
    }

    if ((tn >= (tStart + tA + tC)) && (newPosition >= dMax - (dC + dA)) && i > 0) {
      if (millis() > tn + a) {
        analogWrite(RPWM, spListRight[--i]);
        tn = millis();
        //Serial.print("Pos: ");
        //Serial.println(newPosition);
      }
    }
  }
  analogWrite(RPWM, 0);
  //Serial.print("time: ");
  //Serial.println(tn - tStart);
}

void goLeft() {

  tStart = millis();
  tn = millis();
  i = 0;
  analogWrite(LPWM, spListLeft[i]);
  newPosition = myEnc.read();

  while ((tn <= (tStart + tT) && newPosition >= 0)
         || newPosition >= 0) {

    newPosition = myEnc.read();

    if (tn <= (tStart + tA)) {
      if (millis() > tn + a) {
        analogWrite(LPWM, spListLeft[++i]);
        tn = millis();
        //Serial.print("Pos: ");
        //Serial.println(newPosition);
      }
    }

    if ((tn > (tStart + tA))
        && (tn < (tStart + tA + tC))) {
      tn = millis();
    }

    if ((tn >= (tStart + tA + tC)) && (newPosition <= dA) && i > 0) {
      if (millis() > tn + a) {
        analogWrite(LPWM, spListLeft[--i]);
        tn = millis();
        //Serial.print("Pos: ");
        //Serial.println(newPosition);
      }
    }
  }
  analogWrite(LPWM, 0);
  //Serial.print("time: ");
  //Serial.println(tn - tStart);
}

void setVariables() {
  spOld = spMax;
  spd = speedSearch(spListRight, spMax);
  steps = spd.spPos;

  vMax = steps * 500 + vMin;
  vAvg = (vMin + vMax) / 2;
  tA = steps * a;       // time to maximum speed
  dA = vAvg * tA / 1000;
  dC = (int)(dMax - (2 * dA)); //dB = dA
  tC = (int)(dC / vMax * 1000); // time in C distance
  tT = (int)(2 * tA + tC);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (spOld != spMax) {
    setVariables();
  }

  if (tStart == 0) {
    goRight();
    newPosition = myEnc.read();
    Serial.print("Pos: ");
    Serial.println(newPosition);

    goLeft();
    newPosition = myEnc.read();
    Serial.print("Pos: ");
    Serial.println(newPosition);

  }

  /*
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);

    newPosition = myEnc.read();

    if (newPosition / 100 != oldPosition / 100) {
    oldPosition = newPosition;
    Serial.println("*************************");
    Serial.print("Pos: ");
    Serial.println(newPosition);
    Serial.print("Start: ");
    Serial.println(tStart);
    Serial.print("End: ");
    Serial.println(millis());
    Serial.print("Duration: ");
    Serial.println(millis() - tStart);
    Serial.println("*************************");
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
    Serial.print("i: ");
    Serial.println(i);
    }
  */
}
