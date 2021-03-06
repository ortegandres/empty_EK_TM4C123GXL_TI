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
#include <xdc/runtime/Error.h>
#include "Board.h"
#include "HC_SR04.h"

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

unsigned int    trigger[]  = {Board_LED1};
unsigned int    echo[]     = {Board_PA2};

int main(void)
{
    Board_initGeneral();
    Board_initGPIO();

    Ultrasonic_init(trigger, echo, 1);
    Ultrasonic_Enable();

    System_printf("Ultrasonic HC-SR04 \n");
    System_flush();

    BIOS_start();

    return (0);
}
