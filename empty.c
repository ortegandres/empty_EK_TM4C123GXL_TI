#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include "Board.h"
#include "HC-SR04.h"

#define TASKSTACKSIZE   512
//#define TriggerPin      Board_LED1      // PF3
//#define TriggerOn       Board_LED_ON
//#define TriggerOff      Board_LED_OFF
//#define TriggerIntPin   Board_BUTTON1   // PF0
////#define EchoPin         Board_BUTTON0   // PF4
//#define EchoPin         Board_PA2   // PA2
//#define PeriodEcho          30000
//#define PeriodTriggerOff    15

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
double x = 0;
uint32_t h= 0;
//uint32_t CountPeriod = 0;
//uint32_t EchoCount = 0;
//
//bool EchoTime = true;

//double distance = 0;
//double microS = 0;
//void TriggerOffInt(unsigned int index);
//void TriggerOnInt(unsigned int index);
//void EchoInt(unsigned int index);

//void TriggerOnInt(unsigned int index)
//{
//    GPIO_write(TriggerPin, TriggerOn);
//    Timer_setPeriodMicroSecs(TimerTriggerOff, PeriodTriggerOff);
//    Timer_start(TimerTriggerOff);
//    GPIO_clearInt(TriggerIntPin);
//}

//void TriggerOffInt(unsigned int index)
//{
//    GPIO_write(TriggerPin, TriggerOff);
//    Timer_stop(TimerTriggerOff);
//}

//void EchoInt(unsigned int index)
//{
//    if(EchoTime == false){
//        Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);
//        Timer_start(TimerEcho);
//        GPIO_write(Board_LED0, Board_LED_ON);
//    }
//    else if(EchoTime == true){
//        EchoCount = CountPeriod - Timer_getCount(TimerEcho);
//        Timer_stop(TimerEcho);
//        microS = EchoCount/80;
//        distance = microS/58;
//        System_printf(" %lu cm\n", (ULong)distance);
//        GPIO_write(Board_LED0, Board_LED_OFF);
//    }
//    EchoTime ^= 1;
//    GPIO_clearInt(EchoPin);
//}

Void triggerFunc(UArg arg0, UArg arg1)
{
    while (1)
    {
        if(h == 80000000){
            x = Ultrasonic_read();
            System_printf(" %lu cm\n", (ULong)x);
            h = 0;
        }
        System_flush();
        h++;
    }
}

int main(void)
{
    Task_Params taskParams;
    Board_initGeneral();
    Board_initGPIO();

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)triggerFunc, &taskParams, NULL);

    Ultrasonic_init(Board_LED1, Board_PA2);
//    Timer_setPeriodMicroSecs(TimerTriggerOff, PeriodTriggerOff);
//    Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);

//    CountPeriod = PeriodEcho*80;

//    GPIO_setCallback(TriggerIntPin, TriggerOnInt);
//    GPIO_enableInt(TriggerIntPin);
//    GPIO_setCallback(EchoPin, EchoInt);
//    GPIO_enableInt(EchoPin);

    System_printf("Ultrasonic HC-SR04 \n");
    System_flush();

    BIOS_start();

    return (0);
}
