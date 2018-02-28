#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#include <xdc/runtime/Types.h>
#include <ti/sysbios/timers/timer64/Timer.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>

// #include <ti/drivers/UART.h>
#include "Board.h"

#define TASKSTACKSIZE   512

#define TriggerPin      Board_LED1      // PF3
#define TriggerOn       Board_LED_ON
#define TriggerOff      Board_LED_OFF
#define TriggerIntPin   Board_BUTTON1   // PF0
#define EchoPin         Board_BUTTON0   // PF4

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

Clock_Struct clk1Struct;
Clock_Handle clk2Handle;

Semaphore_Struct sem0Struct, sem1Struct;
Semaphore_Handle semHandle;

uint32_t time1 = 0;
uint32_t time2 = 0;
uint32_t time3 = 0;
uint32_t EchoCount = 0;
bool EchoTime = false;

double distance = 0;
double microS = 0;

void TriggerOffInt(unsigned int index);
void TriggerOnInt(unsigned int index);
void EchoInt(unsigned int index);
void EchoOffInt(unsigned int index);

void TriggerOnInt(unsigned int index)
{
    GPIO_write(TriggerPin, TriggerOn);
    Timer_setPeriodMicroSecs(TimerTriggerOff, 15);
    Timer_start(TimerTriggerOff);
    System_printf("Trigger On \n");
    GPIO_clearInt(TriggerIntPin);
}

void TriggerOffInt(unsigned int index)
{
    GPIO_write(TriggerPin, TriggerOff);
//    Timer_setPeriodMicroSecs(TimerEcho, 1000000); // 10seg
//    Timer_start(TimerEcho);
//    GPIO_write(Board_LED0, Board_LED_OFF);
//    System_printf("Trigger Off \n");
}

void EchoInt(unsigned int index)
{
    if(EchoTime == false){
        Timer_start(TimerEcho);
        GPIO_write(Board_LED0, Board_LED_ON);
    }
    else if(EchoTime == true){
        EchoCount = Timer_getPeriod(TimerEcho);
        Timer_stop(TimerEcho);
        Timer_setPeriodMicroSecs(TimerEcho, 30000); // 10seg
//        Timer_setNextTick(TimerEcho,0);
        microS = (EchoCount/80)*1000;
        distance = (microS / 58);
        System_printf(" %lu \n", (ULong)distance);
        GPIO_write(Board_LED0, Board_LED_OFF);
    }
    EchoTime = (EchoTime+1)%2;
//    EchoCount = Timer_getCount(TimerEcho);
//    time2++;
//    Timer_setNextTick(TimerEcho,0);
//    Timer_setPeriodMicroSecs(TimerEcho, 1000000); // 10seg
//    Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);
//    System_printf(" %lu \n", (ULong)EchoCount);
//    GPIO_write(Board_LED0, Board_LED_ON);
    GPIO_clearInt(EchoPin);
//        Semaphore_post(semHandle);
}

Void triggerFunc(UArg arg0, UArg arg1)
{
    while (1)
    {
//        System_printf("Distancia es:  %lu cm\n", (ULong)EchoCount);
//        System_printf("Tiempo es:  %lu mS\n", (ULong)Timer_getCount(myTimer2));
//        if(Timer_getCount(myTimer2) < 50000000){
////            GPIO_write(TriggerPin, TriggerOn);
//            System_printf("----Tiempo es:  %lu mS\n", (ULong)Timer_getCount(myTimer));
//        }
//        Timer_stop(myTimer2);
//        Timer_setPeriod(myTimer2,0);
//        Timer_start(myTimer2);
//        GPIO_write(TriggerPin, TriggerOff);
//        GPIO_toggle(TriggerPin);
//        time1 = Clock_getTicks();
////        Task_sleep(1000);
        System_flush();
    }
}

int main(void)
{
//    Semaphore_Params semParams;
//    Semaphore_Params_init(&semParams);
//    semParams.mode = Semaphore_Mode_BINARY;
////    semParams.event = evtHandle;
////    semParams.eventId = Event_Id_01;
//    Semaphore_construct(&sem0Struct, 0, &semParams);
//    semHandle = Semaphore_handle(&sem0Struct);

//    Clock_Params clkParams;
//    Clock_Params_init(&clkParams);
//    clkParams.period = 0;
//    clkParams.startFlag = TRUE;

//    Clock_construct(&clk1Struct, (Clock_FuncPtr)EchoInt, 10000, &clkParams);
//    Clock_construct(&clk1Struct, (Clock_FuncPtr)TriggerInt, 100000, &clkParams);
//    Clock_start(clk2Handle);

    Task_Params taskParams;
    Board_initGeneral();
    Board_initGPIO();

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)triggerFunc, &taskParams, NULL);

//    Idealmente --> 1 MHz - 1000000 Hz
//    Timer_Params timerparams, ti_sysbios_family_arm_lm4_Timer1Params;
//    Timer_Params_init(&timerparams);
//    timerparams.period = 0;
//    timerparams.startMode = Timer_StartMode_USER;

//    Timer_Params_init(&timerparams2);
//    ti_sysbios_family_arm_lm4_Timer1Params.period = 0;
//    ti_sysbios_family_arm_lm4_Timer1Params.startMode = Timer_StartMode_USER;

//    Timer_start(myTimer);
//    Timer_start(myTimer2);

    Timer_setPeriodMicroSecs(TimerTriggerOn, 3000000);
    Timer_setPeriodMicroSecs(TimerTriggerOff, 15);
    Timer_setPeriodMicroSecs(TimerEcho, 30000);

    time1 = 0;
    time2 = 0;
    time3 = 0;

    GPIO_setCallback(TriggerIntPin, TriggerOnInt);
    GPIO_enableInt(TriggerIntPin);

    GPIO_setCallback(EchoPin, EchoInt);
    GPIO_enableInt(EchoPin);

    System_printf("Ultrasonic HC-SR04 \n");
    System_flush();

    BIOS_start();

    return (0);
}
