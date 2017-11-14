/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0: GUILLOTON Laurent
 * Version 1.1 - 2017-07-03: Création du sketch initial
 *
 * DESCRIPTION
 *
 * Sketch gérant l'envoi des datas d'un capteur de température et d'humidité (Grove) avec un lien NRF24L01 Mysensors v2.0
 *
 * For more information, please visit:
 * http://wiki.seeed.cc/Grove-TemptureAndHumidity_Sensor-High-Accuracy_AndMini-v1.0/
 *
 */

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

// ID du noeud
//#define MY_NODE_ID 108

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <MySensors.h>
#include <DHT.h>


// Sleep time between sensor updates (in milliseconds)
static const uint64_t UPDATE_INTERVAL = 120000;
static const uint64_t DHT_START_INTERVAL = 2000;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void presentation()
{
	// Send the sketch version information to the gateway
	sendSketchInfo("DHT TemperatureAndHumidity", "1.1");

	// Register all sensors to gw (they will be created as child devices)
	present(CHILD_ID_HUM, S_HUM);
	present(CHILD_ID_TEMP, S_TEMP);
}


void setup()
{
	dht.begin();
}


void loop()
{
	delay(DHT_START_INTERVAL);
	float Temperature = dht.readTemperature();
	float Humidity = dht.readHumidity();

	send(msgTemp.set(Temperature, 2));
	send(msgHum.set(Humidity, 1));

	// Sleep for a while to save energy
	sleep(UPDATE_INTERVAL);
}
