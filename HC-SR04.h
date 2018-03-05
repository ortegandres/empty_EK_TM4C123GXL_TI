//#include <xdc/std.h>
//#include <xdc/runtime/xdc.h>
//#include <xdc/runtime/Types.h>

#ifndef HC_SR04_H_
#define HC_SR04_H_

typedef uint32_t PeriodMicrosec;
void Ultrasonic_init(unsigned int TriggerPin, unsigned int EchoPin);
void Ultrasonic_setPeriod(PeriodMicrosec microsec);
double Ultrasonic_read(void);

void TriggerOffInt(unsigned int index);
void TriggerOnInt(unsigned int index);
void EchoInt(unsigned int index);

#endif /* HC_SR04_H_ */
