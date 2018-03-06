#include "HC_SR04.h"

#include <xdc/runtime/System.h>

#define STACKSIZE           512
#define PeriodEcho          30000
#define PeriodTriggerOff    15
//#define PeriodTriggerOn     1000000

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

bool            TaskFlag    =false;
bool            TimerMode   =true;

uint32_t CountPeriod        = 0;
uint32_t EchoCount          = 0;

//
//  Funciones privadas
//
    //
    //  Calculo de distancia según eventos
    //  El task necesita un Handle
    //
Void HCSR04Task(){
TASKLOOP:
    while(TaskFlag){
//01 Envia el pulso!
    Timer_setPeriodMicroSecs(TimerEcho, PeriodTriggerOff);
    GPIO_write(Trigger_Pin, Trigger_ON);
    TimerMode = true;
    Timer_start(TimerEcho);
   //Esperar semaforo
    Semaphore_pend(Measure_sem_Handle, BIOS_WAIT_FOREVER);
        //Lee el tiempo
        EchoCount = CountPeriod-Timer_getCount(TimerEcho);
        microS = (double)EchoCount/80;
        //Convierte en distancia
        distance = microS/58;
        Task_sleep(1);
//regresar a 01
    }
goto TASKLOOP;
}

//
// Interrupciones
//
void EchoEvent(unsigned int index){
GPIO_clearInt(Echo_Pin);
//Flanco de subida
    if(GPIO_read(Echo_Pin)){
        System_printf("re\n");
        //Activar el timer, el timer comienza en 0
        Timer_start(TimerEcho);
    }
    //Flanco de bajada
    else{
        //Desactivar el timer
        System_printf("fe\n");
            //*Leer el dato
        //Activar semaforo
        Semaphore_post(Measure_sem_Handle);
        TimerMode = true;   //Timer modo 1 porque se ha recibido el pulso!
    }
}
void TimerEvent(){
//Timer en Modo 1
Timer_stop(TimerEcho);
    if(TimerMode){
        //Se han alcanzado ya los 15uS
        GPIO_write(Trigger_Pin, Trigger_OFF);//Apagar el pulso
        Timer_setPeriodMicroSecs(TimerEcho, PeriodEcho);//Detiene el Timer
        TimerMode = false;  //Timer Modo 2
    }
}




//
//  Funciones Publicas
//
void Ultrasonic_init(index_PIN TriggerPin,index_PIN EchoPin){
    Task_Params         taskParams;
    Semaphore_Params    semParams;
    Trigger_Pin = TriggerPin;
    Echo_Pin    = EchoPin;

    //Registra las interrupciones
    GPIO_setCallback(EchoPin, EchoEvent);
    GPIO_enableInt(EchoPin);

    //Configura el timer
    CountPeriod = PeriodEcho*80;
    Timer_setPeriodMicroSecs(TimerEcho, PeriodTriggerOff);

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
