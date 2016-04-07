#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SoftwareSerial.h>

double magx;

double magy;

double magz;

// Choose two Arduino pins to use for software serial
// The GPS Shield uses D2 and D3 by default when in DLINE mode
int RXPin = 2;
int TXPin = 3;

const int button1Pin = 10;

const int led1 = 5;
const int led2 = 6;
const int led3 = 7;


union XYZBuffer {struct {short x,y,z;} value; byte buff[6];};
void changeEndian(union XYZBuffer*xyz){for(int i=0;i<6;i+=2){byte t=xyz->buff[i];xyz->buff[i]=xyz->buff[i+1];xyz->buff[i+1]=t;}}
void readXYZ(int device,union XYZBuffer*xyz){Wire.requestFrom(device,6);long start=millis();while(!Wire.available()&&(millis()-start)<100);if(millis()-start<100){for(int i=0;i<6;i++)xyz->buff[i]=Wire.read();}}
void setupAccel(int device){Wire.beginTransmission(device);Wire.write(0x00);Wire.endTransmission();Wire.requestFrom(device,1);while(!Wire.available());byte ch=Wire.read();Wire.beginTransmission(device);Wire.write(0x2d);Wire.write(0x08);Wire.endTransmission();Wire.beginTransmission(device);Wire.write(0x38);Wire.write(0x84);Wire.endTransmission();}
void readAccel(int device,union XYZBuffer*xyz){Wire.beginTransmission(device);Wire.write(0x32);Wire.endTransmission();readXYZ(device,xyz);}
void setupCompass(int device){Wire.beginTransmission(device);Wire.write(10);Wire.endTransmission();Wire.requestFrom(device,2);while(!Wire.available());char ch=Wire.read();Serial.print(ch);ch=Wire.read();Serial.println(ch);Wire.beginTransmission(device);Wire.write(0x00);Wire.write(0x70);
Wire.endTransmission();Wire.beginTransmission(device);Wire.write(0x01);Wire.write(0xA0);Wire.endTransmission();Wire.beginTransmission(device);Wire.write(0x02);Wire.write(0x00);Wire.endTransmission();delay(6);}
void readCompass(int device,union XYZBuffer*xyz){readXYZ(device,xyz);changeEndian(xyz);Wire.beginTransmission(device);Wire.write(0x03);Wire.endTransmission();}
void setupGyro(int device){Wire.beginTransmission(device);Wire.write(0x00);Wire.endTransmission();Wire.requestFrom(device,1);while(!Wire.available());byte ch=Wire.read();}
void readGyro(int device,union XYZBuffer*xyz){Wire.beginTransmission(device);Wire.write(0x1d);Wire.endTransmission();readXYZ(device,xyz);changeEndian(xyz);}
void output(union XYZBuffer xyz){Serial.print(xyz.value.x);Serial.print(",");Serial.print(xyz.value.y);Serial.print(",");Serial.print(xyz.value.z);}


// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;


// Create a software serial port called "gpsSerial"
// This is how we will communicate with the GPS
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud, used for debugging on a computer.
  Serial.begin(9600);
  Wire.begin();
  setupCompass(0x1e);
  setupAccel(0x53);
  setupGyro(0x68);

  magx = 0;

  magy = 0;

  magz = 0;

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  pinMode(8, OUTPUT);
  if (!SD.begin(8)) {
    Serial.println("Card failed, or not present. Try formatting your card.");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  pinMode(button1Pin,INPUT);
  
  pinMode(led1,OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3,OUTPUT);
  
  digitalWrite(led2,HIGH);
}

void loop()
{
  int button1State = digitalRead(button1Pin);
  int led1State = digitalRead(led1);
  int led2State = digitalRead(led2);
  int led3State = digitalRead(led3);
  
  if(button1State == LOW && led2State == HIGH && led3State == LOW)
  {
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    delay(500);
  }
  else if(button1State == LOW && led2State ==LOW  && led3State ==HIGH)
  {
    digitalWrite(led2,HIGH);
    digitalWrite(led3,LOW);
    delay(500);
  }
   if(gpsSerial.available())
   {
      digitalWrite(led1,HIGH);
    }
    else 
    {
      digitalWrite(led1,LOW);
     }
  
  if(led2State == HIGH)
  
  { 
          union XYZBuffer compass,gyro,accel;
          readAccel(0x53,&accel);
          
          magx = accel.value.x;
          magy = accel.value.y;
          magz = accel.value.z;
           // Orientation();
         
          
           //saveData();
          if(gpsSerial.available() >= 6 && gpsSerial.read() == '$' && gpsSerial.read() == 'G' && gpsSerial.read() == 'P' && gpsSerial.read() == 'G' && gpsSerial.read() == 'G' && gpsSerial.read() == 'A'){
            File dataFile = SD.open("datalog.txt", FILE_WRITE);
              char c = 0;
            File sensor = SD.open("sensor.txt",FILE_WRITE);
              sensor.print(magx);
              sensor.print(" ");
              sensor.print(magy);
              sensor.print(" ");
              sensor.print(magz);
              sensor.println();
              sensor.close();
              //getValues();
              
              
              dataFile.print("$GPGGA");
              Serial.print("$GPGGA");
           
            do{
              //Prints the data from the GPS onto the SD card and a Serial Monitor.
                if (gpsSerial.available())
                {
                  c = gpsSerial.read();
                  dataFile.write(c);
                  Serial.write(c);
                }
              }while (c != '\n');
          dataFile.println();
          Serial.println();
          dataFile.close();
           delay(1000);       
          }
  }
  else
  {
    
  }
 
  
}
 
 
 void saveData(){
  File dataFile = SD.open("sensor.txt",FILE_WRITE);
  dataFile.write("Mag of X:");
  dataFile.print(magx);
  dataFile.println();
  dataFile.write("Mag of y:");
  dataFile.print(magy);
  dataFile.println();
  dataFile.write("Mag of Z:");
  dataFile.print(magz);
  dataFile.println();
  dataFile.close();
}
