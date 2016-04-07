#include <SD.h>

const int chipSelect = 10;
char filename[16]; // make it long enough to hold your longest file name, plus a null terminator

void setup()
  {
    Serial.begin(9600);  // Open serial communications and wait for port to open:
    
    Serial.print("Initializing SD card...");
    // make sure that the default chip select pin is set to 
    // output, even if you don't use it:
    pinMode(10, OUTPUT);
    
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    
    Serial.println("card initialized.");
    
    
    int n = 0;
    
    snprintf(filename, sizeof(filename), "data%03d.txt", n); // includes a three-digit sequence number in the file name
    
    /*  * Simple while loop that will finish after
        * filename in SD card does not exist anymore */
    while(SD.exists(filename)) {
      n++; //increment n .... ( number of files ) 
      snprintf(filename, sizeof(filename), "data%03d.txt", n);  // to modify our char[ ] array
    }
    // create a file with the manipulated char [ ] array. 
    File dataFile = SD.open(filename,FILE_READ);  
    Serial.println(n); // prints # of filename
    Serial.println(filename); // prints file name
    
    dataFile.close(); // close our file 
    //now filename[] contains the name of a file that doesn't exist
    
  } //end of setup 

  
void loop()
  {
    
 
  }//end of loop 

  
