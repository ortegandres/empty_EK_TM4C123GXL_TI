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
//#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include "Board.h"
#include "HC_SR04.h"

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

//double x = 0;
//uint32_t h= 0;
//
//Void triggerFunc(UArg arg0, UArg arg1)
//{
//    while (1)
//    {
//        if(h == 80000000)
//        {
//            x = Ultrasonic_read();
//            System_printf(" %lu cm\n", (ULong)x);
//            System_flush();
//            h = 0;
//        }
//        h++;
//    }
//}

int main(void)
{
//    Task_Params taskParams;
    Board_initGeneral();
    Board_initGPIO();

//    Task_Params_init(&taskParams);
//    taskParams.stackSize = TASKSTACKSIZE;
//    taskParams.stack = &task0Stack;
//    Task_construct(&task0Struct, (Task_FuncPtr)triggerFunc, &taskParams, NULL);

//    Ultrasonic_init(Board_LED1, Board_PA2);

    System_printf("Ultrasonic HC-SR04 \n");
    System_flush();

    BIOS_start();

    return (0);
}
