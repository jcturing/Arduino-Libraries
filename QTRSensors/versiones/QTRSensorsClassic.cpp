/*
  QTRSensors.cpp - Arduino library for using Pololu QTR reflectance
    sensors and reflectance sensor arrays: QTR-1A, QTR-8A, QTR-1RC, and
    QTR-8RC.  The object used will determine the type of the sensor (either
    QTR-xA or QTR-xRC).  Then simply specify in the constructor which
    Arduino I/O pins are connected to a QTR sensor, and the read() method
    will obtain reflectance measurements for those sensors.  Smaller sensor
    values correspond to higher reflectance (e.g. white) while larger
    sensor values correspond to lower reflectance (e.g. black or a void).

    * QTRSensorsRC should be used for QTR-1RC and QTR-8RC sensors.
    * QTRSensorsAnalog should be used for QTR-1A and QTR-8A sensors.
*/

/*
 * Written by Ben Schmidel et al., October 4, 2010.
 * Copyright (c) 2008-2012 Pololu Corporation. For more information, see
 *
 *   http://www.pololu.com
 *   http://forum.pololu.com
 *   http://www.pololu.com/docs/0J19
 *
 * You may freely modify and share this code, as long as you keep this
 * notice intact (including the two links above).  Licensed under the
 * Creative Commons BY-SA 3.0 license:
 *
 *   http://creativecommons.org/licenses/by-sa/3.0/
 *
 * Disclaimer: To the extent permitted by law, Pololu provides this work
 * without any warranty.  It might be defective, in which case you agree
 * to be responsible for all resulting costs and damages.
 */

#include <stdlib.h>
#include "QTRSensors.h"
#include <Arduino.h>



// Base class data member initialization (called by derived class init())
void QTRSensors::init(unsigned char *pins, unsigned char numSensors,
  unsigned char emitterPin)
{
    calibratedMinimumOn=0;
    calibratedMaximumOn=0;
    calibratedMinimumOff=0;
    calibratedMaximumOff=0;

    _lastValue=0; // assume initially that the line is left.

    if (numSensors > QTR_MAX_SENSORS)
        _numSensors = QTR_MAX_SENSORS;
    else
        _numSensors = numSensors;

    if (_pins == 0)
    {
        _pins = (unsigned char*)malloc(sizeof(unsigned char)*_numSensors);
        if (_pins == 0)
            return;
    }

    unsigned char i;
    for (i = 0; i < _numSensors; i++)
    {
        _pins[i] = pins[i];
    }

    _emitterPin = emitterPin;
}


// Reads the sensor values into an array. There *MUST* be space
// for as many values as there were sensors specified in the constructor.
// Example usage:
// unsigned int sensor_values[8];
// sensors.read(sensor_values);
// The values returned are a measure of the reflectance in abstract units,
// with higher values corresponding to lower reflectance (e.g. a black
// surface or a void).
void QTRSensors::read(unsigned int *sensor_values, unsigned char readMode)
{
    readPrivate(sensor_values);
}

// Resets the calibration.
void QTRSensors::resetCalibration()
{
    unsigned char i;
    for(i=0;i<_numSensors;i++)
    {
        if(calibratedMinimumOn)
            calibratedMinimumOn[i] = _maxValue;
        if(calibratedMinimumOff)
            calibratedMinimumOff[i] = _maxValue;
        if(calibratedMaximumOn)
            calibratedMaximumOn[i] = 0;
        if(calibratedMaximumOff)
            calibratedMaximumOff[i] = 0;
    }
}

// Reads the sensors 10 times and uses the results for
// calibration.  The sensor values are not returned; instead, the
// maximum and minimum values found over time are stored internally
// and used for the readCalibrated() method.
void QTRSensors::calibrate(unsigned char readMode)
{
        calibrateOnOrOff(&calibratedMinimumOn,
                         &calibratedMaximumOn,
                         QTR_EMITTERS_ON);

}

void QTRSensors::calibrateOnOrOff(unsigned int **calibratedMinimum,
                                        unsigned int **calibratedMaximum,
                                        unsigned char readMode)
{
    int i;
    unsigned int sensor_values[16];
    unsigned int max_sensor_values[16];
    unsigned int min_sensor_values[16];

    // Allocate the arrays if necessary.
    if(*calibratedMaximum == 0)
    {
        *calibratedMaximum = (unsigned int*)malloc(sizeof(unsigned int)*_numSensors);

        // If the malloc failed, don't continue.
        if(*calibratedMaximum == 0)
            return;

        // Initialize the max and min calibrated values to values that
        // will cause the first reading to update them.

        for(i=0;i<_numSensors;i++)
            (*calibratedMaximum)[i] = 0;
    }
    if(*calibratedMinimum == 0)
    {
        *calibratedMinimum = (unsigned int*)malloc(sizeof(unsigned int)*_numSensors);

        // If the malloc failed, don't continue.
        if(*calibratedMinimum == 0)
            return;

        for(i=0;i<_numSensors;i++)
            (*calibratedMinimum)[i] = _maxValue;
    }

    int j;
    for(j=0;j<10;j++)
    {
        read(sensor_values,readMode);
        for(i=0;i<_numSensors;i++)
        {
            // set the max we found THIS time
            if(j == 0 || max_sensor_values[i] < sensor_values[i])
                max_sensor_values[i] = sensor_values[i];

            // set the min we found THIS time
            if(j == 0 || min_sensor_values[i] > sensor_values[i])
                min_sensor_values[i] = sensor_values[i];
        }
    }

    // record the min and max calibration values
    for(i=0;i<_numSensors;i++)
    {
        if(min_sensor_values[i] > (*calibratedMaximum)[i])
            (*calibratedMaximum)[i] = min_sensor_values[i];
        if(max_sensor_values[i] < (*calibratedMinimum)[i])
            (*calibratedMinimum)[i] = max_sensor_values[i];
    }
}


// Returns values calibrated to a value between 0 and 1000, where
// 0 corresponds to the minimum value read by calibrate() and 1000
// corresponds to the maximum value.  Calibration values are
// stored separately for each sensor, so that differences in the
// sensors are accounted for automatically.
void QTRSensors::readCalibrated(unsigned int *sensor_values, unsigned char readMode)
{
    int i;

        if(!calibratedMinimumOn || !calibratedMaximumOn)
            return;

    // read the needed values
    read(sensor_values,readMode);

    for(i=0;i<_numSensors;i++)
    {
        unsigned int calmin,calmax;
        unsigned int denominator;

        // find the correct calibration
        if(readMode == QTR_EMITTERS_ON)
        {
            calmax = calibratedMaximumOn[i];
            calmin = calibratedMinimumOn[i];
        }

        denominator = calmax - calmin;

        signed int x = 0;
        if(denominator != 0)
            x = (((signed long)sensor_values[i]) - calmin)
                * 1000 / denominator;
        if(x < 0)
            x = 0;
        else if(x > 1000)
            x = 1000;
        sensor_values[i] = x;
    }

}


// Operates the same as read calibrated, but also returns an
// estimated position of the robot with respect to a line. The
// estimate is made using a weighted average of the sensor indices
// multiplied by 1000, so that a return value of 0 indicates that
// the line is directly below sensor 0, a return value of 1000
// indicates that the line is directly below sensor 1, 2000
// indicates that it's below sensor 2000, etc.  Intermediate
// values indicate that the line is between two sensors.  The
// formula is:
//
//    0*value0 + 1000*value1 + 2000*value2 + ...
//   --------------------------------------------
//         value0  +  value1  +  value2 + ...
//
// By default, this function assumes a dark line (high values)
// surrounded by white (low values).  If your line is light on
// black, set the optional second argument white_line to true.  In
// this case, each sensor value will be replaced by (1000-value)
// before the averaging.
int QTRSensors::readLine(unsigned int *sensor_values,
    unsigned char readMode, unsigned char white_line)
	{
    unsigned char i, on_line = 0;
    unsigned long avg; // this is for the weighted total, which is long
                       // before division
    unsigned int sum; // this is for the denominator which is <= 64000

    readCalibrated(sensor_values, readMode);

    avg = 0;
    sum = 0;

    for(i=0;i<_numSensors;i++) {
        int value = sensor_values[i];
        if(white_line)
            value = 1000-value;

        // keep track of whether we see the line at all
        if(value > 200) {
            on_line = 1;
        }

        // only average in values that are above a noise threshold
        if(value > 75) {
            avg += (long)(value) * (i * 1000);
            sum += value;
        }
    }
	
	if(on_line && todosEnLinea(sensor_values)){ //CUANDO NO ESTE NI EN LINEA NI EN FONDO BLANCO
		return 7001;
	}

    if(!on_line) //SI NO ESTA EN LINEA
    {
        // If it last read to the left of center, return 0.
        if(_lastValue < 2000){
			_errorValue = 0;
			return 0;
		}

        // If it last read to the right of center, return the max.
        else if(_lastValue > 5000){
			_errorValue = (_numSensors-1)*1000;
			return (_numSensors-1)*1000;
		}
		else{
			return _errorValue;
		}

    }

		
	if(noventaGrados(sensor_values)){
		if(sensor_values[0] < 200){
			_errorValue = 7000;
			return 7000;
		}
		else{
			_errorValue = 0;
			return 0;
		}
	}

	
    _lastValue = avg/sum;

    return _lastValue;
}


// The array 'pins' contains the Arduino pin number for each sensor.

// 'numSensors' specifies the length of the 'pins' array (i.e. the
// number of QTR-RC sensors you are using).  numSensors must be
// no greater than 16.


// Derived Analog class constructors
QTRSensorsAnalog::QTRSensorsAnalog()
{
    calibratedMinimumOn = 0;
    calibratedMaximumOn = 0;
    calibratedMinimumOff = 0;
    calibratedMaximumOff = 0;
    _pins = 0;
}

QTRSensorsAnalog::QTRSensorsAnalog(unsigned char* pins,
  unsigned char numSensors, int S0, int S1, int S2, int S3,
  unsigned char numSamplesPerSensor, unsigned char emitterPin)
{
    calibratedMinimumOn = 0;
    calibratedMaximumOn = 0;
    calibratedMinimumOff = 0;
    calibratedMaximumOff = 0;
    _pins = 0;
	muxS0 = S0;
	muxS1 = S1;
	muxS2 = S2;
	muxS3 = S3;

    init(pins, numSensors, numSamplesPerSensor, emitterPin);
}


// the array 'pins' contains the Arduino analog pin assignment for each
// sensor.  For example, if pins is {0, 1, 7}, sensor 1 is on
// Arduino analog input 0, sensor 2 is on Arduino analog input 1,
// and sensor 3 is on Arduino analog input 7.

// 'numSensors' specifies the length of the 'analogPins' array (i.e. the
// number of QTR-A sensors you are using).  numSensors must be
// no greater than 16.

// 'numSamplesPerSensor' indicates the number of 10-bit analog samples
// to average per channel (i.e. per sensor) for each reading.  The total
// number of analog-to-digital conversions performed will be equal to
// numSensors*numSamplesPerSensor.  Note that it takes about 100 us to
// perform a single analog-to-digital conversion, so:
// if numSamplesPerSensor is 4 and numSensors is 6, it will take
// 4 * 6 * 100 us = ~2.5 ms to perform a full readLine().
// Increasing this parameter increases noise suppression at the cost of
// sample rate.  The recommended value is 4.

// 'emitterPin' is the Arduino pin that controls the IR LEDs on the 8RC
// modules.  If you are using a 1RC (i.e. if there is no emitter pin),
// or if you just want the emitters on all the time and don't want to
// use an I/O pin to control it, use a value of 255 (QTR_NO_EMITTER_PIN).
void QTRSensorsAnalog::init(unsigned char* pins,
    unsigned char numSensors, unsigned char numSamplesPerSensor,
    unsigned char emitterPin)
{
    QTRSensors::init(pins, numSensors, emitterPin);

    _numSamplesPerSensor = numSamplesPerSensor;
    _maxValue = 1023; // this is the maximum returned by the A/D conversion
}


// Reads the sensor values into an array. There *MUST* be space
// for as many values as there were sensors specified in the constructor.
// Example usage:
// unsigned int sensor_values[8];
// sensors.read(sensor_values);
// The values returned are a measure of the reflectance in terms of a
// 10-bit ADC average with higher values corresponding to lower
// reflectance (e.g. a black surface or a void).
void QTRSensorsAnalog::readPrivate(unsigned int *sensor_values)
{
    unsigned char i, j;

    if (_pins == 0)
        return;

    // reset the values
    for(i = 0; i < _numSensors; i++)
        sensor_values[i] = 0;

    for (j = 0; j < _numSamplesPerSensor; j++)
    {
        for (i = 0; i < _numSensors; i++)
        {
			seleccionarPuerta(i);
            sensor_values[i] += analogRead(_pins[i]);   // add the conversion result
        }
    }

    // get the rounded average of the readings for each sensor
    for (i = 0; i < _numSensors; i++)
        sensor_values[i] = (sensor_values[i] + (_numSamplesPerSensor >> 1)) /
            _numSamplesPerSensor;
}

// the destructor frees up allocated memory
QTRSensors::~QTRSensors()
{
    if (_pins)
        free(_pins);
    if(calibratedMaximumOn)
        free(calibratedMaximumOn);
    if(calibratedMaximumOff)
        free(calibratedMaximumOff);
    if(calibratedMinimumOn)
        free(calibratedMinimumOn);
    if(calibratedMinimumOff)
        free(calibratedMinimumOff);
}
