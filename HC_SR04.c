#include "HC_SR04.h"
#include <xdc/runtime/System.h>

#define STACKSIZE           512
#define PeriodEcho          30000
#define PeriodTriggerOff    15
#define Trigger_ON          1
#define Trigger_OFF         0

Task_Struct     UltrasonicTaskStruct;
Char            UltrasonicTaskStack[STACKSIZE];
Task_Handle     UltrasonicTaskHandle;

//Semaphore_Struct Enable_sem;
//Semaphore_Handle Enable_sem_Handle;

Semaphore_Struct Measure_sem;
Semaphore_Handle Measure_sem_Handle;

unsigned int    Trigger_Pin = 0;
unsigned int    Echo_Pin    = 0;
double          distance    = 0;
double          microS      = 0;
uint32_t        CountPeriod = 0;
uint32_t        EchoCount   = 0;
bool            TaskFlag    = false;
bool            once        = true;


//
//  Funciones privadas
//
    //
    //  Cálculo de distancia según eventos
    //  El task necesita un Handle
    //
Void HCSR04Task(){
TASKLOOP:
    while(TaskFlag){
//01 Envia el pulso!
        Timer_setPeriodMicroSecs(TimerTrigger, PeriodTriggerOff);
        GPIO_write(Trigger_Pin, Trigger_ON);
        Timer_start(TimerTrigger);
//      Esperar semaforo
        Semaphore_pend(Measure_sem_Handle, BIOS_WAIT_FOREVER);
//          Lee el tiempo
            EchoCount = CountPeriod-Timer_getCount(TimerEcho);
//          EchoCount = Timer_getCount(TimerEcho);
            microS = (double)EchoCount/80;
//          Convierte en distancia
            distance = microS/58;
            System_printf("%lu\n", (ULong)distance);
            System_flush();
            Task_sleep(250);
//Regresar a 01
    }
goto TASKLOOP;
}

//
// Interrupciones
//
void TimerEvent(){
Timer_stop(TimerTrigger);
    //Se han alcanzado ya los 15uS
    GPIO_write(Trigger_Pin, Trigger_OFF);           //Apagar el pulso
//        Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);//Detiene el Timer
}
void EchoEvent(unsigned int index){
    //Flanco de subida
    if(once == false){
        if(GPIO_read(Echo_Pin)){
            //Activar el timer, el timer comienza en 0
            Timer_start(TimerEcho);
        }
        //Flanco de bajada
        else{
//        Desactivar el timer
            Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);
//          Timer_stop(TimerEcho);
//            *Leer el dato
//        Activar semaforo
            Semaphore_post(Measure_sem_Handle);
        }
    }
    else{
        once = false;
    }
    GPIO_clearInt(Echo_Pin);
}

//
//  Funciones Publicas
//
void Ultrasonic_init(index_PIN TriggerPin, index_PIN EchoPin){
    Task_Params         taskParams;
    Semaphore_Params    semParams;
    Trigger_Pin = TriggerPin;
    Echo_Pin    = EchoPin;

    //Registra las interrupciones
    GPIO_setCallback(EchoPin, EchoEvent);
    GPIO_enableInt(EchoPin);

    //Configura el timer
    CountPeriod = PeriodEcho*80;
    Timer_setPeriodMicroSecs(TimerTrigger, PeriodTriggerOff);
    Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);

    //Configura el Task
        Task_Params_init(&taskParams);
            taskParams.stackSize = STACKSIZE;
            taskParams.stack = &UltrasonicTaskStack;
        Task_construct(&UltrasonicTaskStruct, (Task_FuncPtr)HCSR04Task, &taskParams, NULL);

    //Configurar semaforos
        //Crear un semáforo para sincronizar tareas
        Semaphore_Params_init(&semParams);
        Semaphore_construct(&Measure_sem, 0, &semParams);
        Measure_sem_Handle = Semaphore_handle(&Measure_sem);
}
void Ultrasonic_Enable(){
    //Activar el task!
    TaskFlag = true;
}
void Ultrasonic_Disable(){
    //Desactivar el task!
    TaskFlag = false;
}
centimeters Ultrasonic_Read(){
    //Leer el registro de distancia
    return distance;
}


