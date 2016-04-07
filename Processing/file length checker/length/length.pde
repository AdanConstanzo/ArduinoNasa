
String [] data;
int counter = 0;
void setup()
{
  
  
   data = loadStrings("open.csv");
   for (int i = 0;  i <data.length; i++)
    {
      counter ++; 
      println(counter);
    }
}

