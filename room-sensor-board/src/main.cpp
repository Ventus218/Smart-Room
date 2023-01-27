#include <Arduino.h>
#include <Scheduler.h>
#include <SamplingSubsystem.h>
#include <Pir.h>
#include <Photoresistor.h>
#include <LedSubsystem.h>
#include <Led.h>
#include <ServiceSubsystem.h>

#define PIN_PIR 48
#define PIN_PHOTORESISTOR 9
#define PIN_LED 1

Scheduler sched;

void setup() {
  Logger::init(9600, LogLevel::INFO);
  sched.init(1000);

  SamplingSubsystem* samplingSys = new SamplingSubsystem(new Pir(PIN_PIR), new Photoresistor(PIN_PHOTORESISTOR));
  samplingSys->init(1000);

  LedSubsystem* ledSys = new LedSubsystem(new Led(PIN_LED), samplingSys);
  ledSys->init(1000);

  ServiceSubsystem* serviceSys = new ServiceSubsystem(samplingSys);
  serviceSys->init(1000);

  sched.addTask(samplingSys);
  sched.addTask(ledSys);
  sched.addTask(serviceSys);
}

void loop() {
  sched.schedule();
}