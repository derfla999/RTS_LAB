#ifndef _APP_H
#define _APP_H

#include "TinyTimber.h"


#define DAC (*(volatile unsigned char *)0x4000741C)
#define MAX_VOLUME 30
#define MIN_VOLUME 1
#define BASE_VOLUME 5

typedef struct
{
  Object super;
  int example;
  char input[20];
  int inputIndex;
  int median[3];
  int medianIndex;
  int historyCounter;
  int sum;
  volatile unsigned char volume;
  volatile unsigned char muted;

  volatile unsigned char tone_state;

} App;

#define initApp() \
  {initObject(), 1234, {0}, 0, {0, 0, 0}, 0, 0, 0, BASE_VOLUME, 0, 0}

extern int john[32];
extern int period[25];

typedef struct
{
  Object super;
  int loop_toggle;
  int background_loop_range;

} Background;

#define initBackground() \
  {initObject(), 1000, 1000}

extern Background background;

void reader(App *, int);
void receiver(App *, int);
void startApp(App *, int);
void find_min_max(int *min, int *max);
int x(int k);

void print_periods_for_key(int key);
void tone_generator(App *, int);
void background_load(Background *, int);
void increase_load(Background *, int);
void decrease_load(Background *, int);
void togglebg(Background *, int);
void test_wcet(App *, int);
void tone_generator_measure(App *);

//test step 4 
void background_load_measure(Background *self);
void measure_background(Background *self);
void init_cycle_counter(void);

#endif
