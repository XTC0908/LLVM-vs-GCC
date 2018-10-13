#include <stdio.h>
#include "system.h"
#include "includes.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"

#define DEBUG 1

#define HW_TIMER_PERIOD 100 /* 100ms */

/* Button Patterns */

#define GAS_PEDAL_FLAG      0x08
#define BRAKE_PEDAL_FLAG    0x04
#define CRUISE_CONTROL_FLAG 0x02
/* Switch Patterns */

#define TOP_GEAR_FLAG       0x00000002
#define ENGINE_FLAG         0x00000001

/* LED Patterns */

#define LED_RED_0 0x00000001 // Engine
#define LED_RED_1 0x00000002 // Top Gear

#define LED_GREEN_0 0x0001 // Cruise Control activated
#define LED_GREEN_2 0x0002 // Cruise Control Button
#define LED_GREEN_4 0x0010 // Brake Pedal
#define LED_GREEN_6 0x0040 // Gas Pedal

/*
 * Definition of Tasks
 */

#define TASK_STACKSIZE 2048

OS_STK StartTask_Stack[TASK_STACKSIZE]; 
OS_STK ControlTask_Stack[TASK_STACKSIZE]; 
OS_STK VehicleTask_Stack[TASK_STACKSIZE];
OS_STK SwitchIO_Stack[TASK_STACKSIZE];
OS_STK ButtonIO_Stack[TASK_STACKSIZE];

// Task Priorities
 
#define STARTTASK_PRIO     5
#define VEHICLETASK_PRIO  10
#define CONTROLTASK_PRIO  12
#define SWITCHIO_PRIO  14
#define BUTTONIO_PRIO  16

// Task Periods

#define CONTROL_PERIOD  300
#define VEHICLE_PERIOD  300

/*
 * Definition of Kernel Objects 
 */

// Mailboxes
OS_EVENT *Mbox_Throttle;
OS_EVENT *Mbox_Velocity;

// Semaphores

OS_EVENT *Vehicle_Semaphore;
OS_EVENT *Control_Semaphore;

// SW-Timer

OS_TMR *Vehicle_Timer;
OS_TMR *Control_Timer;

/*
 * Types
 */
enum active {on, off};

enum active gas_pedal = off;
enum active brake_pedal = off;
enum active top_gear = off;
enum active engine = off;
enum active cruise_control = off; 

/*
 * Global variables
 */
int delay; // Delay of HW-timer 
INT16U led_green = 0; // Green LEDs
INT32U led_red = 0;   // Red LEDs
INT32U switch_flag = 0;
INT8U button_flag = 0;


void VehicleCallback(OS_TMR *ptmr, void *p_arg)
{
    OSSemPost(Vehicle_Semaphore);
}

int buttons_pressed(void)
{
  return ~IORD_ALTERA_AVALON_PIO_DATA(DE2_PIO_KEYS4_BASE);    
}

int switches_pressed(void)
{
  return IORD_ALTERA_AVALON_PIO_DATA(DE2_PIO_TOGGLES18_BASE);    
}

void ControlCallback(OS_TMR *ptmr, void *p_arg)
{
    OSSemPost(Control_Semaphore);
}


void ButtonIO(void* pdata) 
{  
    while(1)
    {
        int tmp;
        tmp = buttons_pressed();
        switch(tmp&0x0e)
        {
            case 8:
                button_flag = GAS_PEDAL_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_GREENLED9_BASE,0x40);//GAS_CONTROL LEDG6
                break;
            case 4:
                button_flag = BRAKE_PEDAL_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_GREENLED9_BASE,0x10);//BRAKE_CONTROL LEDG4
                break;
            case 2:
                button_flag = CRUISE_CONTROL_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_GREENLED9_BASE,0x04);//CRUISE_CONTROL LEDG2
                break;
        }
        OSTimeDlyHMSM(0,0,0,20); 
    }
}

void SwitchIO(void* pdata) 
{
    printf("switch\n");
    while(1)
    {
        int tmp;
        tmp = switches_pressed();
        switch(tmp&0x00000003)
        {
            case 1: 
                switch_flag = ENGINE_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,tmp&0x1);//ENGINE LEDR0
                break;
            case 2: 
                switch_flag = TOP_GEAR_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,tmp&0x2);//GEER LEDR1
                break;
            case 3:
                switch_flag = ENGINE_FLAG|TOP_GEAR_FLAG;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,tmp&0x3);//ENGINE&GEER LEDR0&LEDR1
                break;
            case 0:
                switch_flag = 0;
                IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,tmp&0x0);
                break;
        }
        OSTimeDlyHMSM(0,0,0,20); 
    }
}


/*
 * ISR for HW Timer
 */
alt_u32 alarm_handler(void* context)
{
  OSTmrSignal(); /* Signals a 'tick' to the SW timers */
  
  return delay;
}

static int b2sLUT[] = {0x40, //0
                 0x79, //1
                 0x24, //2
                 0x30, //3
                 0x19, //4
                 0x12, //5
                 0x02, //6
                 0x78, //7
                 0x00, //8
                 0x18, //9
                 0x3F, //-
};

/*
 * convert int to seven segment display format
 */
int int2seven(int inval){
    return b2sLUT[inval];
}

/*
 * output current velocity on the seven segement display
 */
void show_velocity_on_sevenseg(INT8S velocity){
  int tmp = velocity;
  int out;
  INT8U out_high = 0;
  INT8U out_low = 0;
  INT8U out_sign = 0;

  if(velocity < 0){
     out_sign = int2seven(10);
     tmp *= -1;
  }else{
     out_sign = int2seven(0);
  }

  out_high = int2seven(tmp / 10);
  out_low = int2seven(tmp - (tmp/10) * 10);
  
  out = int2seven(0) << 21 |
            out_sign << 14 |
            out_high << 7  |
            out_low;
  IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_HEX_LOW28_BASE,out);
}

/*
 * shows the target velocity on the seven segment display (HEX5, HEX4)
 * when the cruise control is activated (0 otherwise)
 */
void show_target_velocity(INT8U target_vel)
{
}

/*
 * indicates the position of the vehicle on the track with the four leftmost red LEDs
 * LEDR17: [0m, 400m)
 * LEDR16: [400m, 800m)
 * LEDR15: [800m, 1200m)
 * LEDR14: [1200m, 1600m)
 * LEDR13: [1600m, 2000m)
 * LEDR12: [2000m, 2400m]
 */
void show_position(INT16U position)
{
  if (position < 4000) 
     IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00020000);// 0-400
  else if (position < 8000)
      IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00010000); // 400-800
    else if (position < 12000)
        IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00008000);  // 800-1200
      else if (position < 16000)
          IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00004000);  // 1200-1600
        else if (position < 20000)
            IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00002000);  // 1600-2000
          else
              IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_REDLED18_BASE,0x00001000);  // 2000-2400
}

/*
 * The function 'adjust_position()' adjusts the position depending on the
 * acceleration and velocity.
 */
 INT16U adjust_position(INT16U position, INT16S velocity,
                        INT8S acceleration, INT16U time_interval)
{
  INT16S new_position = position + velocity * time_interval / 1000
    + acceleration / 2  * (time_interval / 1000) * (time_interval / 1000);

  if (new_position > 24000) {
    new_position -= 24000;
  } else if (new_position < 0){
    new_position += 24000;
  }
  
  show_position(new_position);
  return new_position;
}
 
/*
 * The function 'adjust_velocity()' adjusts the velocity depending on the
 * acceleration.
 */
INT16S adjust_velocity(INT16S velocity, INT8S acceleration,  
               enum active brake_pedal, INT16U time_interval)
{
  INT16S new_velocity;
  INT8U brake_retardation = 200;

  if (brake_pedal == off)
    new_velocity = velocity  + (float) (acceleration * time_interval) / 1000.0;
  else {
    if (brake_retardation * time_interval / 1000 > velocity)
      new_velocity = 0;
    else
      new_velocity = velocity - brake_retardation * time_interval / 1000;
  }
  
  return new_velocity;
}

/*
 * The task 'VehicleTask' updates the current velocity of the vehicle
 */
void VehicleTask(void* pdata)
{ 
  INT8U err;  
  void* msg;
  INT8U* throttle; 
  INT8S acceleration;  /* Value between 40 and -20 (4.0 m/s^2 and -2.0 m/s^2) */
  INT8S retardation;   /* Value between 20 and -10 (2.0 m/s^2 and -1.0 m/s^2) */
  INT16U position = 0; /* Value between 0 and 20000 (0.0 m and 2000.0 m)  */
  INT16S velocity = 0; /* Value between -200 and 700 (-20.0 m/s amd 70.0 m/s) */
  INT16S wind_factor;   /* Value between -10 and 20 (2.0 m/s^2 and -1.0 m/s^2) */

  printf("Vehicle task created!\n");

  while(1)
    {
      err = OSMboxPost(Mbox_Velocity, (void *) &velocity);

      OSSemPend(Vehicle_Semaphore, 0, &err);

      /* Non-blocking read of mailbox: 
       - message in mailbox: update throttle
       - no message:         use old throttle
      */
      msg = OSMboxPend(Mbox_Throttle, 1, &err); 
      if (err == OS_NO_ERR) 
         throttle = (INT8U*) msg;
      /* Retardation : Factor of Terrain and Wind Resistance */
      if (velocity > 0)
         wind_factor = velocity * velocity / 10000 + 1;
      else 
         wind_factor = (-1) * velocity * velocity / 10000 + 1;
         
      if (position < 4000) 
         retardation = wind_factor; // even ground
      else if (position < 8000)
          retardation = wind_factor + 15; // traveling uphill
        else if (position < 12000)
            retardation = wind_factor + 25; // traveling steep uphill
          else if (position < 16000)
              retardation = wind_factor; // even ground
            else if (position < 20000)
                retardation = wind_factor - 10; //traveling downhill
              else
                  retardation = wind_factor - 5 ; // traveling steep downhill
                  
      acceleration = *throttle / 2 - retardation;     
      position = adjust_position(position, velocity, acceleration, 300); 
      velocity = adjust_velocity(velocity, acceleration, brake_pedal, 300); 
      printf("Position: %dm\n", position / 10);
      printf("Velocity: %4.1fm/s\n", velocity /10.0);
      printf("Throttle: %dV\n", *throttle / 10);
      show_velocity_on_sevenseg((INT8S) (velocity / 10));
    }
} 
 
/*
 * The task 'ControlTask' is the main task of the application. It reacts
 * on sensors and generates responses.
 */

void ControlTask(void* pdata)
{
  INT8U err;
  INT8U throttle = 0; /* Value between 0 and 80, which is interpreted as between 0.0V and 8.0V */
  void* msg;
  INT16S* current_velocity;
  INT16S  former_velocity;
  INT16S  cruise_velocity;
  INT8U   cruise_velocity_flag = 1;
  printf("Control Task created!\n");
  while(1)
    {
                          
      //control law

      switch (button_flag)
      {
          case GAS_PEDAL_FLAG:
            if(engine == on)
            {
                gas_pedal = on;
                brake_pedal = off;
                cruise_control = off;
            }
          break;        
          case BRAKE_PEDAL_FLAG:
            gas_pedal = off;
            brake_pedal = on;
            cruise_control = off;
          break;
          case CRUISE_CONTROL_FLAG:
            if((top_gear == on)&&(engine == on))
            {
                gas_pedal = off;
                brake_pedal = off;
                cruise_control = on;
                printf("cruise on\n");
            }
          break;      
          default:
            gas_pedal = off;
              brake_pedal = off;
              cruise_control = off;
          break;
      }
      
      switch(switch_flag)
      {
          case ENGINE_FLAG:
            engine = on;
            top_gear = off;
            cruise_control = off; 
          break;
          case TOP_GEAR_FLAG:
            top_gear = on;
            if(*current_velocity == 0) 
            {
                engine = off;
                gas_pedal = off;  
                cruise_control = off; 
            }
          break;  
          case ENGINE_FLAG|TOP_GEAR_FLAG:
            engine = on;
            top_gear = on;
          break;
          case 0:
            engine = off;
            top_gear = off;
            cruise_control = off; 
          break;
      }
      
      if(engine == on)
      {
          if((cruise_control == on)&&former_velocity >= 200)
          {      
              IOWR_ALTERA_AVALON_PIO_DATA(DE2_PIO_GREENLED9_BASE,0x05);           
              if(cruise_velocity_flag == 1) 
              {
                cruise_velocity = former_velocity;
                cruise_velocity_flag = 0;
              }
              if(*current_velocity > cruise_velocity)
              {
                if(throttle >= 20)
                {
                  throttle -= 20;
                }
                else
                {
                  throttle = 0;
                }
              }
              else if(*current_velocity < cruise_velocity)
              {
                if(throttle <= 60)
                {
                  throttle += 20;
                }
                else
                {
                  throttle = 80;
                }
              }
          }
          else
          {
            cruise_velocity_flag = 1;
          }
          former_velocity = *current_velocity;    
            
          if((gas_pedal == on)&&(throttle <= 80))
          {      
            throttle += 20;
          }
      }
      
      msg = OSMboxPend(Mbox_Velocity, 0, &err);
      current_velocity = (INT16S*) msg;
      
      err = OSMboxPost(Mbox_Throttle, (void *) &throttle);
      
      OSSemPend(Control_Semaphore, 0, &err);
      OSTimeDlyHMSM(0,0,0, CONTROL_PERIOD);
    
    }
}

/* 
 * The task 'StartTask' creates all other tasks kernel objects and
 * deletes itself afterwards.
 */ 

void StartTask(void* pdata)
{
  INT8U err;
  void* context;

  static alt_alarm alarm;     /* Is needed for timer ISR function */
  
  /* Base resolution for SW timer : HW_TIMER_PERIOD ms */
  delay = alt_ticks_per_second() * HW_TIMER_PERIOD / 1000; 
  printf("delay in ticks %d\n", delay);

  /* 
   * Create Hardware Timer with a period of 'delay' 
   */
  if (alt_alarm_start (&alarm,
      delay,
      alarm_handler,
      context) < 0)
      {
          printf("No system clock available!n");
      }

  /* 
   * Create and start Software Timer 
   */
    Vehicle_Timer = OSTmrCreate(0,
    VEHICLE_PERIOD,
    OS_TMR_OPT_PERIODIC,
     (OS_TMR_CALLBACK)VehicleCallback,
    (void *)0,
    "VehiclePeriod",
    &err);
    
    Control_Timer = OSTmrCreate(0,
    CONTROL_PERIOD,
    OS_TMR_OPT_PERIODIC,
     (OS_TMR_CALLBACK)ControlCallback,
    (void *)0,
    "ControlPeriod",
    &err);
    
    OSTmrStart(Vehicle_Timer, &err); 
    OSTmrStart(Control_Timer, &err); 
    
  /*
   * Creation of Kernel Objects
   */
  Vehicle_Semaphore = OSSemCreate(0);
  Control_Semaphore = OSSemCreate(0);
  
  // Mailboxes
  Mbox_Throttle = OSMboxCreate((void*) 0); /* Empty Mailbox - Throttle */
  Mbox_Velocity = OSMboxCreate((void*) 0); /* Empty Mailbox - Velocity */
   
  /*
   * Create statistics task
   */

  OSStatInit();

  /* 
   * Creating Tasks in the system 
   */


  err = OSTaskCreateExt(
       ControlTask, // Pointer to task code
       NULL,        // Pointer to argument that is
                    // passed to task
       &ControlTask_Stack[TASK_STACKSIZE-1], // Pointer to top
                             // of task stack
       CONTROLTASK_PRIO,
       CONTROLTASK_PRIO,
       (void *)&ControlTask_Stack[0],
       TASK_STACKSIZE,
       (void *) 0,
       OS_TASK_OPT_STK_CHK);

  err = OSTaskCreateExt(
       VehicleTask, // Pointer to task code
       NULL,        // Pointer to argument that is
                    // passed to task
       &VehicleTask_Stack[TASK_STACKSIZE-1], // Pointer to top
                             // of task stack
       VEHICLETASK_PRIO,
       VEHICLETASK_PRIO,
       (void *)&VehicleTask_Stack[0],
       TASK_STACKSIZE,
       (void *) 0,
       OS_TASK_OPT_STK_CHK);
       
    err = OSTaskCreateExt(
         SwitchIO, // Pointer to task code
         NULL,      // Pointer to argument that is
                    // passed to task
         (void *)&SwitchIO_Stack[TASK_STACKSIZE-1], // Pointer to top
                             // of task stack 
         SWITCHIO_PRIO,
         SWITCHIO_PRIO,
         (void *)&SwitchIO_Stack[0],
         TASK_STACKSIZE,
         (void *) 0,  
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
         
     err = OSTaskCreateExt(
         ButtonIO, // Pointer to task code
         NULL,      // Pointer to argument that is
                    // passed to task
         (void *)&ButtonIO_Stack[TASK_STACKSIZE-1], // Pointer to top
                             // of task stack 
         BUTTONIO_PRIO,
         BUTTONIO_PRIO,
         (void *)&ButtonIO_Stack[0],
         TASK_STACKSIZE,
         (void *) 0,  
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  
  printf("All Tasks and Kernel Objects generated!\n");

  /* Task deletes itself */

  OSTaskDel(OS_PRIO_SELF);
}

/*
 *
 * The function 'main' creates only a single task 'StartTask' and starts
 * the OS. All other tasks are started from the task 'StartTask'.
 *
 */

int main(void) {

  printf("Lab: Cruise Control\n");
 
  OSTaskCreateExt(
     StartTask, // Pointer to task code
         NULL,      // Pointer to argument that is
                    // passed to task
         (void *)&StartTask_Stack[TASK_STACKSIZE-1], // Pointer to top
                             // of task stack 
         STARTTASK_PRIO,
         STARTTASK_PRIO,
         (void *)&StartTask_Stack[0],
         TASK_STACKSIZE,
         (void *) 0,  
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  
         
  OSStart();
  
  return 0;
}
