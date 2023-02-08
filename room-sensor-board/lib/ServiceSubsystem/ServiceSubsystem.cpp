#include <ServiceSubsystem.h>

ServiceSubsystem::ServiceSubsystem(SamplingSubsystem* samplingSys): StateSystem(ServiceSubsystemState::INIT) {
  this->samplingSys = samplingSys;
  this->mqttClient = new PubSubClient(this->wifiClient);
}

void ServiceSubsystem::setupWiFi() {

  log(String("Connecting to ") + String(ssid), LogLevel::INFO);
  
  delay(1500); // let everything settle

  WiFi.mode(WIFI_STA);
  WiFi.begin(this->ssid, this->wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log(String("Connecting to ") + String(ssid), LogLevel::INFO);
  }

  log(F("WiFi connected."), LogLevel::INFO);
}

void ServiceSubsystem::reconnect() {
  while (!this->mqttClient->connected()) {
    if (this->mqttClient->connect(this->clientName)) {
      log(F("MQTT connection established."), LogLevel::INFO);
    } else {
      log(F("Failed to establish MQTT connection. Trying again in 5 seconds..."), LogLevel::NOTICE);
      delay(5000);
    }
  }
}

void ServiceSubsystem::sendMesurements() {
  String msg;

  if (!this->mqttClient->connected()) {
    reconnect();
  }
  
  msg = "{\n\
          \"motion\": " + String(this->samplingSys->getMotion()) + ",\n\
          \"lightLevel\": " + String(this->samplingSys->getLightLevel()) + "\n\
        }";
  this->mqttClient->loop();
  if (this->mqttClient->publish(this->topic, msg.c_str())) {
    log(F("Data sent to service"), LogLevel::INFO);
  } else {
    log(F("Something went wrong, data not sent to service"), LogLevel::NOTICE);
  }
}

void ServiceSubsystem::init(int period) {
  Task::init(period);

  this->setupWiFi();
  this->mqttClient->setServer(this->mqttServer, this->mqttServerPort);
  this->reconnect();

  transitionTo(ServiceSubsystemState::SENDING);
}

void ServiceSubsystem::tick() {
  switch (getState()) {
    case ServiceSubsystemState::SENDING:
      this->sendMesurements();
      break;
    case ServiceSubsystemState::INIT:
      log(F("should not tick in INIT state"), LogLevel::WARNING);
      break;
  }
}

String ServiceSubsystem::getSystemName() {
  return F("ServiceSubsystem");
}

String ServiceSubsystem::stateToString(ServiceSubsystemState s) {
  String str;

  switch(s) {
    case ServiceSubsystemState::INIT: 
      str = F("INIT");
      break;
    case ServiceSubsystemState::SENDING: 
      str = F("SENDING");
      break;
  }
  return str;
}