#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include "MQTTClient.h"
#include <cstdio>
#include "ADXL345/adxl345.h"

#define CPU_TEMP "/sys/class/thermal/thermal_zone0/temp"

#define ADDRESS     "mqtt://192.168.1.100:1883"
#define CLIENTID    "rpi1"
#define AUTHMETHOD  "avijays"
#define AUTHTOKEN   "avijays"
#define TOPIC       "ee513/CPUTemp"
#define QOS         0
#define TIMEOUT     10000L

using namespace std;

float getCPUTemperature()
{
    int cpuTemp;
    fstream fs;
    fs.open(CPU_TEMP, fstream::in);
    fs >> cpuTemp;
    fs.close();
    return (((float)cpuTemp)/1000);
}

void printAccValues(const acc_xyz* ptr) {
    cout << "Acceleration (X,Y,Z): " << ptr->acc_x << "," << ptr->acc_y << "," << ptr->acc_z << endl;
}

int main()
{
    ADXL345 acc(1, 0x53);

    char str_payload[100];
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;

    MQTTClient_willOptions willOptions = MQTTClient_willOptions_initializer;
    willOptions.message = "Device Offline"; // Message to be sent when the client disconnects
    willOptions.qos = 1; // Quality of Service for the will message
    willOptions.retained = 0; // Whether the will message should be retained
    willOptions.topicName = "topic/will"; // Will message topic

    opts.will = &willOptions;

    int rc;
    if((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS)
    {
        cout << "Failed to connect, return code " << rc << endl;
        return -1;
    }


    sprintf(str_payload, "{\"d\":{\"CPUTemp\": %f }}", getCPUTemperature());
    pubmsg.payload = str_payload;
    pubmsg.payloadlen = strlen(str_payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
        " seconds for publication of " << str_payload <<
        " \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    cout << "Message with token " << (int)token << " delivered." << endl;

    char str_payload2[100];
    acc_xyz values = acc.readXYZ();
    sprintf(str_payload2, "{\"d\":{\"acc_X\": %d, \"acc_Y\": %d, \"acc_Z\": %d }}", values.acc_x,values.acc_y,values.acc_z);
    pubmsg.payload = str_payload2;
    pubmsg.payloadlen = strlen(str_payload2);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, "ee513/Acc", &pubmsg, &token);
    cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
        " seconds for publication of " << str_payload2 <<
        " \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    cout << "Message with token " << (int)token << " delivered." << endl;

    while(1)
    {

    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;

    // acc_xyz values;

    // acc.writeRegister(REG_DATA_FORMAT, 0x8);
    // acc.calibrateSensor();

    // while(1)
    // {
    //     values = acc.readXYZ();
    //     printAccValues(&values);
    //     sleep(1);
    // }
    // return 0;
}