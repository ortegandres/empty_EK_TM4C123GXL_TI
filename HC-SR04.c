#include <xdc/std.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <xdc/cfg/global.h>
#include "Board.h"
#include "HC-SR04.h"

//#define TriggerPin      Board_LED1      // PF3
#define TriggerOn       Board_LED_ON
#define TriggerOff      Board_LED_OFF
//#define TriggerIntPin   Board_BUTTON1   // PF0
//#define EchoPin         Board_PA2   // PA2
#define PeriodEcho          300000
#define PeriodTriggerOff    15
#define PeriodTriggerOn     1000000

uint32_t CountPeriod = 0;
uint32_t EchoCount = 0;
//uint32_t pTriggerPin = 0;
//uint32_t pEchoPin = 0;
unsigned int pTriggerPin = 0;
unsigned int pEchoPin = 0;
bool EchoTime = true;
double distance = 0;
double microS = 0;

void TriggerOnInt(unsigned int index)
{
    GPIO_write(pTriggerPin, TriggerOn);
    Timer_setPeriodMicroSecs(TimerTriggerOff, PeriodTriggerOff);
    Timer_start(TimerTriggerOff);
//    GPIO_clearInt(pTriggerPin);
}

void TriggerOffInt(unsigned int index)
{
    GPIO_write(pTriggerPin, TriggerOff);
    Timer_stop(TimerTriggerOff);
}

void EchoInt(unsigned int index)
{
    if(EchoTime == false){

        Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);
        Timer_start(TimerEcho);
        GPIO_write(Board_LED0, Board_LED_ON);
    }
    else if(EchoTime == true){
        EchoCount = CountPeriod - Timer_getCount(TimerEcho);
        Timer_stop(TimerEcho);
        microS = EchoCount/80;
        distance = microS/58;
//        System_printf(" %lu cm\n", (ULong)distance);
        GPIO_write(Board_LED0, Board_LED_OFF);
    }
    EchoTime ^= 1;
//    GPIO_clearInt(pEchoPin);
}

//  -------------------------------------------------------------

void Ultrasonic_init(unsigned int TriggerPin, unsigned int EchoPin){
    pTriggerPin = TriggerPin;
    pEchoPin = EchoPin;
    GPIO_setCallback(TriggerPin, TriggerOnInt);
    GPIO_enableInt(TriggerPin);
    GPIO_setCallback(EchoPin, EchoInt);
    GPIO_enableInt(EchoPin);
    CountPeriod = PeriodEcho*80;
    Timer_setPeriodMicroSecs(TimerTriggerOn, PeriodTriggerOn);
    Timer_setPeriodMicroSecs(TimerTriggerOff, PeriodTriggerOff);
    Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);
}
void Ultrasonic_setPeriod(PeriodMicrosec microsec){
    Timer_setPeriodMicroSecs(TimerTriggerOn, microsec);
}
double Ultrasonic_read(void){
    return distance;
}
