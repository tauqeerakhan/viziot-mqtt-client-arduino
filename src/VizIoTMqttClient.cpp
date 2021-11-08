/**
   @file       VizIoTMqttClient.h
   @author     Trunov Alexandr
   @license    This project is released under the MIT License (MIT)
   @copyright  Copyright (c) 2018 Trunov Alexandr
   @date       November  2018
*/

#include <Arduino.h>
#include "VizIoTMqttClient.h";


VizIoTMqttClient::VizIoTMqttClient(PubSubClient mqttClient) {
  this->_mqttClient = mqttClient;
  this->_keyAndPassIsOk = false;

  this->_isListenCommands = false;
  // Set up a handler for receiving data
  this->_mqttClient.setCallback([this] (char* topic, byte * payload, unsigned int length) {
    this->_callback(topic, payload, length);
  });
}


byte VizIoTMqttClient::config(String key, String pass) {
  return this->config(key, pass, DEFAULT_HOST, DEFAULT_PORT);
}


byte VizIoTMqttClient::config(String deviceKey, String devicePass, String mqttHost, int mqttPort) {

  // Set the address of the MQTT broker
  this->_host = mqttHost;
  this->_port = mqttPort;
  this->_mqttClient.setServer(this->_host.c_str(), this->_port);

  deviceKey.trim();
  deviceKey.toUpperCase();
  devicePass.trim();

  if (deviceKey.length() == 16 && devicePass.length() == 20) {
    this->_deviceKey = deviceKey;
    this->_devicePass = devicePass;
    this->_topicForPublish = String("/devices/") + _deviceKey + "/packet";
    this->_topicForSubscribe = String("/devices/") + _deviceKey + "/param/+";
    this->_clientId = "arduinoClient-" + _deviceKey;
    this->_keyAndPassIsOk = true;
    return VIZIOT_CONFIG_OK;
  } else {
    this->_keyAndPassIsOk = false;
    return VIZIOT_KEY_OR_PASS_NOT_CORRECT_FORMAT;
  }
}

bool VizIoTMqttClient::connected() {
  return this->_mqttClient.connected();
}

bool VizIoTMqttClient::connect() {
  while (this->connected() == false) {
    // Attempt to connect
    if (this->connectToBroker()) {
      // Connected
      this->subscribe();
      return true;
    } else {
      //failed,   статус= this->_mqttClient.state()
      // try to connect in 5 seconds //
      delay(5000);
    }
  }
}

void VizIoTMqttClient::closeConnection() {
  if (this->connected()) {
    this->_mqttClient.disconnect();
  }
}

bool VizIoTMqttClient::sendJsonString(String jsonString) {
  while (!this->connected()) {
    this->reconnect();
  }

  if (jsonString.length() > 6) {
    return this->_mqttClient.publish(this->_topicForPublish.c_str(), jsonString.c_str());
  } else {
    return false; // jsonString is clearly not JSON
  }

}

void VizIoTMqttClient::reconnect() {
  while (!this->connected()) {
    // Attempt to connect MQTT:
    if (this->connectToBroker()) {
      this->subscribe();
    } else {
      //      failed,статус= this->_mqttClient.state()
      //       attempt to connect in 5 seconds
       delay(5000);
    }
  }
}

void VizIoTMqttClient::loop() {
  // Check the connection to the Broker
  if (!this->connected()) {
    this->reconnect();
  }
  this->_mqttClient.loop();
}
bool VizIoTMqttClient::subscribe() {
  if (this->connected()) {
    if (this->_isListenCommands) {
      if (this->_mqttClient.subscribe(this->_topicForSubscribe.c_str())) {
        //// Subscribed to the topic
        return true;
      } else {
        //Error! could not subscribe to the topic _topicForSubscribe
        return false;
      }
    }
  } else {
    return false;
  }
}
bool VizIoTMqttClient::connectToBroker() {
  if (this->_keyAndPassIsOk == true && !this->connected()) {
    return this->_mqttClient.connect(this->_clientId.c_str(), this->_deviceKey.c_str(), this->_devicePass.c_str());
  } else {
    return false;
  }
}
// Handle the event of receiving data
void VizIoTMqttClient::_callback(char* topic, byte* payload, unsigned int length) {
  //          if (compareTo(topic, (char*)topicLed.c_str())) {
  //            if ((char)payload[0] == '1') {
  //              statusLed = 1;
  //              digitalWrite(LED_ESP, LOW);
  //            } else {
  //              statusLed = 0;
  //              digitalWrite(LED_ESP, HIGH);
  //            }
  //          snprintf(msg, sizeof(msg), "{\"led\":\"%c\"}", (char)payload[0]);
  //          this->_mqttClient.publish(topicSendData.c_str(), msg);
  //          }
  //  this->callbackUser(String parameter, String value);
  String topicString = String(topic);
  byte value = ((char)payload[0] == '1') ? 1 : 0 ;
  int indexLastSlash = topicString.lastIndexOf('/');
  String parameter = "";
  if (indexLastSlash != -1) {
    parameter = topicString.substring(indexLastSlash + 1);
  }
  if (this->_isListenCommands && parameter.length() > 0){
    this->callbackUser(parameter, value);
  }
}
bool VizIoTMqttClient::listenCommand(VIZIOT_CALLBACK_SIGNATURE) {
  this->_isListenCommands = true;
  this->callbackUser = callbackUser;
  if(this->connected()) {
    return this->subscribe();
  }else{
    return false;
  }
}





