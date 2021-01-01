/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sensor.hpp

 2018-11-23 18:17:43
 2020-02-08 00:13:47

*/
#ifndef SENSOR_HPP_20200208001347
#define SENSOR_HPP_20200208001347

#include <cgl/public.h>

namespace cgl{
namespace {
namespace {

}//end namespace
}//end namespace
}//end namespace cgl

#include <android/sensor.h>

class sensor
{
public:
	ASensorManager* sensorManager;
	ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;
public:
	int create()
	{
		//准备监控加速器
		sensorManager = ASensorManager_getInstance();
		accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
		sensorEventQueue = ASensorManager_createEventQueue(sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);
	}

	int start(int time = (1000L / 60) * 1000)
	{
		//当我们的应用获得焦点时，我们开始监控加速计。
		if (accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(sensorEventQueue,
				accelerometerSensor);
			//我们想要每秒获得 60 个事件(在美国)。
			ASensorEventQueue_setEventRate(sensorEventQueue,
				accelerometerSensor, time);
		}
	}

	void stop()
	{
		//当我们的应用程序失去焦点时，我们会停止监控加速计。
		//这可在不使用时避免使用电池。
		if (accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(sensorEventQueue,
				accelerometerSensor);
		}
	}

	void process()
	{
		//如果传感器有数据，立即处理。
		//if (ident == LOOPER_ID_USER) {
		if (accelerometerSensor != NULL) {
			ASensorEvent event;
			while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
				//LOGI("传感器accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
			}
		}
	}
};


#endif //SENSOR_HPP_20200208001347