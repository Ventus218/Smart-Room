#include <Arduino.h>
#include <Scheduler.h>
#include <Logger.h>
#include <InputSubsystem.h>
#include <RollerSubsystem.h>
#include <ServoMotor.h>
#include <LightSubsystem.h>
#include <Led.h>

#define PIN_SERVO 11
#define PIN_LED 8

Scheduler sched;

void setup() {
  Logger::init(9600, LogLevel::INFO);
  sched.init(1000);

  InputSubsystem* inputSys = new InputSubsystem();
  inputSys->init(1000);

  RollerSubsystem* rollerSys = new RollerSubsystem(new ServoMotor(PIN_SERVO), inputSys);
  rollerSys->init(1000);

  LightSubsystem* lightSys = new LightSubsystem(new Led(PIN_LED), inputSys);
  lightSys->init(1000);

  sched.addTask(inputSys);
  sched.addTask(rollerSys);
  sched.addTask(lightSys);
}

void loop() {
  sched.schedule();
}