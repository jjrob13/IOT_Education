#ifndef _SENSOR_CONFIG_H
#define _SENSOR_CONFIG_H
//#include "Sensor.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <exception>
//For JSON creation and parsing
#include "include/rapidjson/document.h"
#include "include/rapidjson/istreamwrapper.h"
#include "include/rapidjson/ostreamwrapper.h"
#include "include/rapidjson/writer.h"
#include <boost/assign.hpp>
#include <boost/function.hpp>
#include "Sensor.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "UltrasonicSensor.h"
#include "ServoController.h"

using namespace rapidjson;
using namespace std;
using namespace boost::assign;

#define SENSOR_KEY "sensors"
#define SERVO_KEY "servos"
#define ULTRASONIC_SENSOR_KEY "ultrasonic_sensor"
#define LIGHT_SENSOR_KEY "light_sensor"
#define TOUCH_SENSOR_KEY "touch_sensor"
#define ID_KEY "id"
#define TRIG_KEY "trig"
#define ECHO_KEY "echo"
#define PIN_KEY "pin"

class SensorConfig{
public:
	//NOTE: This is where support for additional sensors will be added
	/*
	Routine description:
	Take in a rapidjson Value object (just think of it as json) and return
	the sensor object represented by that json

	Note: sensor_json should contain only a single key corresponding to the 
	sensor type that is being created
	*/
	static Sensor * allocAndInitSensor(const Value & sensor_json)
	{
		if(sensor_json.MemberCount() != 1){
			throw logic_error("Should only be one sensor present.");
		}
		
		//Sensor specific initialization routines.  Add code here to add support
		//for additional sensors
		if(sensor_json.HasMember(ULTRASONIC_SENSOR_KEY)) {
			int trig = sensor_json[ULTRASONIC_SENSOR_KEY][TRIG_KEY].GetInt();
			int echo = sensor_json[ULTRASONIC_SENSOR_KEY][ECHO_KEY].GetInt();
			int id = sensor_json[ULTRASONIC_SENSOR_KEY][ID_KEY].GetInt();
			return new UltrasonicSensor(trig, echo, id);
		}

		if(sensor_json.HasMember(TOUCH_SENSOR_KEY)) {
			int pin = sensor_json[TOUCH_SENSOR_KEY][PIN_KEY].GetInt();
			int id = sensor_json[TOUCH_SENSOR_KEY][ID_KEY].GetInt();
			return new TouchSensor(pin, id);
		}

		if(sensor_json.HasMember(LIGHT_SENSOR_KEY)) {
			int pin = sensor_json[LIGHT_SENSOR_KEY][PIN_KEY].GetInt();
			int id = sensor_json[LIGHT_SENSOR_KEY][ID_KEY].GetInt();
			return new LightSensor(pin, id);
		}

		//Add your code for new sensors here

		throw logic_error("Unknown sensor type");
		return NULL;

	}

	SensorConfig()
	{
		this->json_doc.SetObject();
	}


	~SensorConfig()
	{
		//TODO: Implement if necessary
	}

	void loadConfigFile(const char * filename)
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		this->json_doc.ParseStream(isw);

	}

	void saveConfigFile(const char * filename)
	{
		ofstream ofs(filename);
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		this->json_doc.Accept(writer);
	}

	/*
	Routine description:
	Populate the sensors vector and servo_map with Sensor objects and ServoController objects
	as specified in the this->json_doc configuration file.

	NOTE: This routine will allocate memory and the user is responsible for freeing said memory.
	This arises from the inability to have vector<Sensor>, as Sensor is an abstract class.
	
	Memory is currently freed in the cleanup_sensors_and_servos routine in edison_config

	*/
	void createSensorsAndServos(vector<Sensor *> & sensors,
		map<int, ServoController *> & servo_map){
		
		parseAndCreateSensors(sensors);
		parseAndCreateServos(servo_map);
	}

	void parseAndCreateSensors(vector<Sensor*> & sensors)
	{
		if(!this->json_doc.HasMember(SENSOR_KEY)){
			return; //Nothing to parse
		}

		const Value & json_sensors = this->json_doc[SENSOR_KEY];
		assert(json_sensors.IsArray());
		for(auto it = json_sensors.Begin(); it != json_sensors.End(); it++)
		{
			//Create the sensor object and create it
			Sensor * new_sensor = allocAndInitSensor(*it);

			//add the new sensor to the sensors vector
			sensors.push_back(new_sensor);
		}
	}

	void parseAndCreateServos(map<int, ServoController *> & servo_map)
	{
		//TODO: Implement

	}

	//NOTE: For new sensor support, one of these should be implemented as well for
	//ease of configuration creation
	void addUltrasonicSensor(int trig, int echo, int id)
	{
		/*
		{'ultrasonic_sensor' : {'echo' : 1, 'trig' : 2, 'id' : 3}}
		*/
		Value sensor_json(kObjectType); //Create json object.  top level
		Value sensor_config(kObjectType); //pin info
		myAddMember(sensor_config, TRIG_KEY, trig);
		myAddMember(sensor_config, ECHO_KEY, echo);
		myAddMember(sensor_config, ID_KEY, id);
		myAddMember(sensor_json, ULTRASONIC_SENSOR_KEY, sensor_config);
	
		addSensor(sensor_json); //Add to the current configuration that we are building
	}

	//NOTE: For new sensor support, one of these should be implemented as well for
	//ease of configuration creation
	void addTouchSensor(int pin, int id)
	{
		Value sensor_json(kObjectType); //Create json object.  top level
		Value sensor_config(kObjectType); //pin info

		myAddMember(sensor_config, PIN_KEY, pin);
		myAddMember(sensor_config, ID_KEY, id);
		myAddMember(sensor_json, TOUCH_SENSOR_KEY, sensor_config);
	
		addSensor(sensor_json); //Add to the current configuration that we are building
	}

	void addLightSensor(int pin, int id)
	{
		Value sensor_json(kObjectType); //Create json object.  top level
		Value sensor_config(kObjectType); //pin info

		myAddMember(sensor_config, PIN_KEY, pin);
		myAddMember(sensor_config, ID_KEY, id);
		myAddMember(sensor_json, LIGHT_SENSOR_KEY, sensor_config);
	
		addSensor(sensor_json); //Add to the current configuration that we are building

	}

	void addServo(int pin){
		//overload
		addServo(pin, pin);
	}

	void addServo(int pin, int id)
	{
		//create the servos array if it does not exist in the json_doc
		if(!this->json_doc.HasMember(SERVO_KEY)){
			Value servo_array(kArrayType);
			this->json_doc.AddMember(SERVO_KEY, servo_array,
				this->json_doc.GetAllocator());
		}

		Value servo_obj(kObjectType);
		myAddMember(servo_obj, PIN_KEY, pin);
		myAddMember(servo_obj, ID_KEY, id);
		
		//add servo to the array
		this->json_doc[SERVO_KEY].PushBack(servo_obj, this->json_doc.GetAllocator());
	}

	void addSensor(Value & sensor_json)
	{
		//create the sensors array if it does not exist in the json_doc
		if(!this->json_doc.HasMember(SENSOR_KEY)){
			Value sensor_array(kArrayType);
			this->json_doc.AddMember(SENSOR_KEY, sensor_array,
				this->json_doc.GetAllocator());
		}
		this->json_doc[SENSOR_KEY].PushBack(sensor_json, this->json_doc.GetAllocator());
		
	}

	//These are all helper functions to avoid the verbosity of the rapidjson lib
	void myAddMember(Value & json_obj, const char * key, string & str_value)
	{
		json_obj.AddMember(Value(key, strlen(key), this->json_doc.GetAllocator()),
					Value(str_value.c_str(), str_value.size(), this->json_doc.GetAllocator()),
					this->json_doc.GetAllocator());
	}

	void myAddMember(Value & json_obj, const char * key, int i)
	{
		json_obj.AddMember(Value(key, strlen(key), this->json_doc.GetAllocator()), Value(i), this->json_doc.GetAllocator());

	}

	void myAddMember(Value & json_obj, const char * key, Value & val)
	{
		json_obj.AddMember(Value(key, strlen(key), this->json_doc.GetAllocator()), val, this->json_doc.GetAllocator());

	}



protected:
	Document json_doc;

};
#endif

