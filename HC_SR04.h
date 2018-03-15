#ifndef _HC_SR04_
#define _HC_SR04_

    #include <stdbool.h>
    #include <xdc/std.h>
    #include <ti/sysbios/BIOS.h>
    #include <ti/sysbios/knl/Clock.h>
    #include <ti/sysbios/knl/Task.h>
    #include <ti/sysbios/knl/Semaphore.h>

    #include <ti/drivers/GPIO.h>
    #include <ti/drivers/PIN.h>
    #include <ti/sysbios/family/arm/lm4/Timer.h>
    #include <xdc/cfg/global.h>
    #include "Board.h"

//
//    Macros para la conversion de datos
//
    #define     CmToInches(DISTANCE)    {DISTANCE/2.54;}
    #define     CmToMeters(DISTANCE)    {DISTANCE/100;}

    typedef unsigned int    index_PIN;
    typedef float           centimeters;

    extern    void        Ultrasonic_init(index_PIN, index_PIN);
    extern    void        Ultrasonic_Enable();
    extern    void        Ultrasonic_Disable();
    extern    centimeters Ultrasonic_Read();    //Centimetros
#endif
