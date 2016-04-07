#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

 //varibales for xyz accel
  double magx; 
  double magy;
  double magz;
  
  //varaibles for xyz gyro
  double gmagx;
  double gmagy;
  double gmagz;

  // Choose two Arduino pins to use for software serial
  // The GPS Shield uses D2 and D3 by default when in DLINE mode
  int RXPin = 2;
  int TXPin = 3;


////////////////////////////////////////////SENSOR FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////SENSOR FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////
             
 

 

  //sd pin
  const int chipSelect = 8;
  //new file char arrays
 
  char filename2[16];



// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default


// Create a software serial port called "gpsSerial"
// This is how we will communicate with the GPS

  

  
/////////////////////////////////////VOID SETUP () /////////////////////////////////////////////////////////
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
          
          gmagx = 0;
          
          gmagy = 0;
          
          gmagz = 0;
      
        
          
          Serial.print("Initializing SD card...");
          // see if the card is present and can be initialized:
          pinMode(8, OUTPUT);
          if (!SD.begin(8)) {
            Serial.println("Card failed, or not present. Try formatting your card.");
            // don't do anything more:
            return;
          }
          Serial.println("card initialized.");
          
        
          
         
          
         
          
          
          
          
          /////////////////////////////////////////////INCREMENT FILE CODE! //////////////////////////////////////////////////////
        
                  int j = 0;
                 
                  snprintf(filename2 , sizeof(filename2), "se%03d.txt" , j);
               
                  while(SD.exists(filename2))
                  {
                    j++;
                    snprintf(filename2 , sizeof(filename2), "se%03d.txt" , j); 
                  }
                  
            
                  File sensor = SD.open(filename2, FILE_WRITE);
                 
                  sensor.close();
                  Serial.println(j);
                  Serial.println(filename2);
                  
          
          /////////////////////////////////////////////INCREMENT FILE CODE! //////////////////////////////////////////////////////
          
          
        }//end of setup



////////////////////////////////////////////////VOID LOOP() //////////////////////////////////////////////////////////////
        void loop()
        {
          
        
          union XYZBuffer compass,gyro,accel;
                  readAccel(0x53,&accel);
                  readGyro(0x68,&gyro);
                  
                  gmagx = gyro.value.x;
                  gmagy = gyro.value.y;
                  gmagz = gyro.value.z;
                  
                  magx = accel.value.x;
                  magy = accel.value.y;
                  magz = accel.value.z;
         
          
           File sensor = SD.open(filename2,FILE_WRITE);
                    //prints out every second of time inizialized 
                   
                      sensor.print(magx);
                      Serial.print(magx);
                      
                      Serial.print(" ");
                      sensor.print(";");
                      Serial.print(";");
                      sensor.print(magy);
                      Serial.print(magy);
                    
                      Serial.print(" ");
                      sensor.print(";");
                      Serial.print(";");
                      sensor.print(magz);
                      Serial.print(magz);
                     
                      Serial.print(" ");
                      sensor.print(";");
                      Serial.print(";");
                     sensor.println();
                     Serial.println();
                      
                      sensor.close();
         
        
        }//end of void loop () 
 
 

