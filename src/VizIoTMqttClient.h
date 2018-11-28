/**
   @file       VizIoTMqttClient.h
   @author     Trunov Alexandr
   @license    This project is released under the MIT License (MIT)
   @copyright  Copyright (c) 2018 Trunov Alexandr
   @date       November  2018
*/
#ifndef VizIoTMqttClient_h
#define VizIoTMqttClient_h,

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

/** https://github.com/knolleary/pubsubclient */
#include <PubSubClient.h>

#define DEFAULT_HOST     "viziot.com"
#define DEFAULT_PORT     48651

#define VIZIOT_CONFIG_OK    						1
#define VIZIOT_KEY_OR_PASS_NOT_CORRECT_FORMAT		2	//'Ключ или пароль устройства записаны в неправильном формате'


#ifdef ESP8266
#include <functional>
#define VIZIOT_CALLBACK_SIGNATURE std::function<void(String, byte)> callbackUser
#else
#define VIZIOT_CALLBACK_SIGNATURE void (*callbackUser)(String, byte)
#endif

class VizIoTMqttClient
{
  public:
    VizIoTMqttClient(PubSubClient mqttClient);
    byte config(String deviceKey, String devicePass);
    byte config(String deviceKey, String devicePass, String mqttHost, int mqttPort);
    bool connect();
    void loop();
    void closeConnection();
    bool sendJsonString(String jsonString);
    void reconnect();
    bool connected();
    VIZIOT_CALLBACK_SIGNATURE;
    bool listenCommand(VIZIOT_CALLBACK_SIGNATURE);

  private:
    void _callback(char* topic, byte* payload, unsigned int length);
    bool _keyAndPassIsOk;
    bool _isListenCommands;
    String _deviceKey;
    String _devicePass;
    String _host;
    bool subscribe();
    bool connectToBroker();
    int _port;
    String _topicForPublish;
    String _topicForSubscribe;
    String _clientId;
    PubSubClient _mqttClient;
};
#endif
