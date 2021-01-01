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
		//׼����ؼ�����
		sensorManager = ASensorManager_getInstance();
		accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
		sensorEventQueue = ASensorManager_createEventQueue(sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);
	}

	int start(int time = (1000L / 60) * 1000)
	{
		//�����ǵ�Ӧ�û�ý���ʱ�����ǿ�ʼ��ؼ��ټơ�
		if (accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(sensorEventQueue,
				accelerometerSensor);
			//������Ҫÿ���� 60 ���¼�(������)��
			ASensorEventQueue_setEventRate(sensorEventQueue,
				accelerometerSensor, time);
		}
	}

	void stop()
	{
		//�����ǵ�Ӧ�ó���ʧȥ����ʱ�����ǻ�ֹͣ��ؼ��ټơ�
		//����ڲ�ʹ��ʱ����ʹ�õ�ء�
		if (accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(sensorEventQueue,
				accelerometerSensor);
		}
	}

	void process()
	{
		//��������������ݣ���������
		//if (ident == LOOPER_ID_USER) {
		if (accelerometerSensor != NULL) {
			ASensorEvent event;
			while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
				//LOGI("������accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
			}
		}
	}
};


#endif //SENSOR_HPP_20200208001347