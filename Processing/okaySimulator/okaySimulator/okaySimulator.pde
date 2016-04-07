
import processing.opengl.*;
String [] data;
int offset_x = -4;
int offset_y = -10;
int SIZE = 300, SIZEX = 400;
int counter = 0;

  
    

void setup()
{
  size(SIZEX, SIZE, P3D);
  
  
}

void draw()
{
  translate(SIZEX/2, SIZE/2, -400);
  background(0);

  data = loadStrings("SE004.TXT");
  float [] fx = new float[data.length];
  float [] fz = new float[data.length];
  
  for (int i = 0;  i <data.length; i++)
    {
      String [] fields = split(data[i] , ";");
      
      float x = float(fields[0]);
      float y = float(fields[1]);
      float z = float(fields[2]);
      fx[i] = y;
      fz[i] = x;
    }
    //to avoid error
    if(counter < data.length-1)
     {   
         
         println(counter);
        
        //for sensor stick
        buildShape(-(fz[counter]-offset_x)*HALF_PI/256,-(fx[counter]-offset_y)*HALF_PI/256);
        //for dr.spielier data
        //buildShape(fz[counter],fx[counter]);
     }  
     else if (counter >data.length)
     {
        buildShape(0,0);
     }
    //increment array counter
    counter ++; 
}

void buildShape(float rotz, float rotx)
{
  
  pushMatrix();
  scale(6,6,14);
  rotateZ(rotz);
    rotateX(rotx);
    fill(255);
     stroke(0);
     box(60, 10, 10);
     fill(0, 255, 0);
     box(10, 9, 40);
     translate(0, -10, 20);
     fill(255, 0, 0);
     //box(5, 12, 10);  
  popMatrix();
}
