#ifndef __SERVICESUBSYSTEM__
#define __SERVICESUBSYSTEM__

#include <Task.h>
#include <StateSystem.h>
#include <SamplingSubsystem.h>
#include <WiFi.h>
#include <PubSubClient.h>

enum class ServiceSubsystemState { INIT, SENDING };

class ServiceSubsystem: public Task, public StateSystem<ServiceSubsystemState> {
private:
  SamplingSubsystem* samplingSys;
  
  /* wifi network info */
  const char* ssid = "iPhone XR di Alessandro";
  const char* wifiPassword = "Ventus218";

  /* MQTT server address */
  const char* mqttServer = "broker.mqtt-dashboard.com";
  const int mqttServerPort = 1883;
  const char* clientName = "room-sensor-board";

  /* MQTT topic */
  const char* topic = "esiot-2022/aleventu/sensor-board";

  /* MQTT client management */
  WiFiClient wifiClient;
  PubSubClient* mqttClient;

  void setupWiFi();
  void reconnect();
  void sendMesurements();

public:
  ServiceSubsystem(SamplingSubsystem* samplingSys);
  void init(int period);  
  void tick();

  String getSystemName();
  String stateToString(ServiceSubsystemState s);
};

#endif