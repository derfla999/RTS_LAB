#include "App.h"
#include "TinyTimber.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"
#include <stdlib.h>
#include "print.h"
#include <time.h>

extern App app;
extern Can can0;
extern Serial sci0;
extern Background background;
// int a = 0;

void receiver(App *self, int unused)
{
  CANMsg msg;
  CAN_RECEIVE(&can0, &msg);
  SCI_WRITE(&sci0, "Can msg received: ");
  SCI_WRITE(&sci0, msg.buff);
}

int john[32] = {0, 2, 4, 0,
                0, 2, 4, 0,
                4, 5, 7,
                4, 5, 7,
                7, 9, 7, 5, 4, 0,
                7, 9, 7, 5, 4, 0,
                0, -5, 0,
                0, -5, 0};

int period[25] = {
    2024, 1911, 1803, 1702, 1607,
    1516, 1431, 1351, 1275, 1203,
    1136, 1072, 1012, 955, 901,
    851, 803, 758, 715, 675,
    637, 601, 568, 536, 506};

/*
void reader(App *self, int c) {
  SCI_WRITE(&sci0, "Rcv: \'");
  SCI_WRITECHAR(&sci0, c);
  SCI_WRITE(&sci0, "\'\n");
}
*/

void find_min_max(int *min, int *max)
{
  *min = john[0];
  *max = john[0];
  for (int i = 1; i < 32; i++)
  {
    if (john[i] < *min)
    {
      *min = john[i];
    }
    if (john[i] > *max)
    {
      *max = john[i];
    }
  }
}

int x(int k)
{
  return k + 10;
}

void print_periods_for_key(int key)
{
  print("Key: %d\n", key);

  for (int i = 0; i < 32; i++)
  {
    int k = john[i] + key;
    int p = period[x(k)];
    print("%d ", p);
  }
  print("\n");
}

void startApp(App *self, int arg)
{

  int min, max;

  CANMsg msg;

  CAN_INIT(&can0);
  SCI_INIT(&sci0);
  SCI_WRITE(&sci0, "Hello, hello...\n");

 // find_min_max(&min, &max);
  //print("Min: %d, Max: %d\n", min, max);

  //print("k=0 -> %d\n", period[x(0)]);
  test_wcet(self, 0);
  msg.msgId = 1;
  msg.nodeId = 1;
  msg.length = 6;
  msg.buff[0] = 'H';
  msg.buff[1] = 'e';
  msg.buff[2] = 'l';
  msg.buff[3] = 'l';
  msg.buff[4] = 'o';
  msg.buff[5] = 0;
  CAN_SEND(&can0, &msg);

  //AFTER(USEC(500), self, tone_generator, 0);
  //AFTER(USEC(1300), &background, background_load, 0);
}
/*
void readInput(App *self, int c)---------
{
  int num;

  if (c != 'e')
  {
    self->array[self->counter] = c;
    self->counter++;
  }
  else
  {
    num = atoi(self->array);
    SCI_WRITE(&sci0, num);
  }

  SCI_WRITE(&sci0, "Rcv: \'");
  SCI_WRITECHAR(&sci0, c);
  SCI_WRITE(&sci0, num);
}*/
int sumArray(int *array)
{
  int a = 0;
  for (int i = 0; i < 3; i++)
  {
    a += array[i];
  }
  return a;
}


void increase_load(Background *self, int unused)
{
  if (self->background_loop_range < 8000)
  {
    self->background_loop_range += 500;
  }
  print("background loop range: %d\n", self->background_loop_range);
}

void test_wcet(App *self, int unused)
{
  float max =0.0f;
  float avg =0.0f;
  
  for (int i = 0; i < 500; i++)
  {
    Time start = CURRENT_OFFSET();
    //background_load_measure(self);
    for(int j =0; j<1000; j++){
      tone_generator(self,0);
    }  
    //tone_generator_measure(self);
    Time end = CURRENT_OFFSET();

    float total_nsec = (float)(end - start) * 10000.0f; // Convert to nanoseconds
    float current_avg = total_nsec / 1000.0f;

    
    if(current_avg > max){
      max = current_avg; 
    }
    avg += current_avg;
    

  }
  avg /= 500.0;
  
  print("Max WCET: %d NSEC\n", (int)max);
  print("Avg WCET: %d NSEC\n", (int)avg);
} 
void tone_generator_measure(App *self){
  volatile int i;
  for(i = 0; i < 500; i++){
    
  }
}
void decrease_load(Background *self, int unused)
{
  if (self->background_loop_range > 1000)
  {
    self->background_loop_range -= 500;
  }
  print("background loop range: %d\n", self->background_loop_range);
}

void togglebg(Background *self, int unused)
{
  if (self->loop_toggle == 0)
  {
    self->loop_toggle = 1;
    SCI_WRITE(&sci0, "Background loop toggled ON\n");
  }
  else
  {
    self->loop_toggle = 0;
    SCI_WRITE(&sci0, "Background loop toggled OFF\n");
  }
}

void sortArray(int *array)
{
  int temp;
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2 - i; j++)
    {
      if (array[j] > array[j + 1])
      {
        temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
}

void reader(App *self, int c)
{
  int inputValue;

  switch (c)
  {
  case 'e':
    int temp[3];
    self->input[self->inputIndex] = '\0';
    self->inputIndex = 0;
    inputValue = atoi(self->input);
    print_periods_for_key(inputValue);

    self->median[self->medianIndex] = inputValue;
    if (self->historyCounter < 3)
    {
      self->historyCounter++;
    }

    self->medianIndex = (self->medianIndex + 1) % 3;
    self->sum = sumArray(self->median);
    temp[0] = self->median[0];
    temp[1] = self->median[1];
    temp[2] = self->median[2];

    sortArray(temp);

    print("Entered integer: %d\n", inputValue);
    print("sum: %d\n", self->sum);
    if (self->historyCounter == 1)
    {
      print("median: %d\n", inputValue);
    }
    else if (self->historyCounter == 2)
    {
      int a = self->median[0];
      int b = self->median[1];

      if (a > b)
      {
        int t = a;
        a = b;
        b = t;
      }

      print("median: %d\n", (a + b) / 2);
    }
    else
    {
      print("median: %d\n", temp[1]);
    }
    break;
  case 'f':
    print("the 3-history has been erased\n");
    self->medianIndex = 0;
    self->sum = 0;
    self->median[0] = 0;
    self->median[1] = 0;
    self->median[2] = 0;
    self->historyCounter = 0;
    // a = 0;
    break;
  case '+':
    if (self->volume < MAX_VOLUME)
      self->volume++;
    print("volume: %d\n", self->volume);
    break;

  case '-':
    if (self->volume > MIN_VOLUME)
      self->volume--;
    print("volume: %d\n", self->volume);
    break;

  case 'm':
    self->muted ^= 1;
    print("muted: %d\n", self->muted);
    break;

  case 'u':
    ASYNC(&background, increase_load, 0);
    break;

  case 'd':
    ASYNC(&background, decrease_load, 0);
    break;

    case 't':
    ASYNC(&background, togglebg, 0);
   
    break;

  default:

    self->input[self->inputIndex++] = c;
    break;
  }
}

int main()
{
  INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
  INSTALL(&can0, can_interrupt, CAN_IRQ0);
  TINYTIMBER(&app, startApp, 0);
  return 0;
}

void tone_generator(App *self, int unused)
{

  if (self->muted)
  {
    DAC = 0; // stäng av ljud
  }
  else if (self->tone_state)
  {
    DAC = self->volume;
  }
  else
  {
    DAC = 0;
  }

  self->tone_state ^= 1; // xor

  //AFTER(USEC(500), self, tone_generator, 0);
  
}

/*void background_load(Background *self, int unused)
{
  volatile int i;

  if (self->loop_toggle)
  {
     for (i = 0; i < self->background_loop_range; i++)
    {}
  }
  
  AFTER(USEC(1300), self, background_load, 0);
}
*/



// test step 4


void background_load_measure(Background *self)
{
  volatile int i;

  
  
    for (i = 0; i < self->background_loop_range; i++)
    {

    }
  

  
}