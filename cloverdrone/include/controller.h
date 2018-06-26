#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <RTIMULib.h>

//#define qPLUSCONFIG 1

/*
	 TOP VIEW
	cwm1 \ x / m4ccw
	       \|/
	     y--z
	       / \
	ccwm2 /   \ m3cw
*/

//Kd should be -ve
struct qPIDvariables {
	qPIDvariables() : Kp(0),Ki(0),Kd(0),integratedSum(0),previousError(0),previousTime(0),boundIterm(0.35) {}
	float Kp;
	float Ki;
	float Kd;
	float KpBuffer;
	float KiBuffer;
	float KdBuffer;
	float integratedSum;
	float previousError;
	uint64_t previousTime;
	float computedPIDvalue;
	float boundIterm;
	float KpAngular;
};

struct qMotorThrust {
	qMotorThrust() : m1Value(0),m2Value(0),m3Value(0),m4Value(0),mMinBound(1.0),mMaxBound(2.0),mMinPID(-0.5),mMaxPID(0.5) {}
	float m1Value;
	float m2Value;
	float m3Value;
	float m4Value;
	float mMinBound;
	float mMaxBound;
	float mMinPID;
	float mMaxPID;
};

struct qTarget {
	qTarget() : throttle(0),theta(0),phi(0),gamma(0) {}
	float throttle;
	float theta;
	float phi;
	float gamma;
};

class qControl{
	qPIDvariables* qPIDval[3]; //Roll(x),Pitch(y),Yaw(z)
	qMotorThrust* qM;
	RTIMU_DATA* qIMUValues;
	qTarget* qTargetValues;
	float iTT;
	float pdTT;
	public:
		qControl(qPIDvariables*, qPIDvariables*, qPIDvariables*, qMotorThrust*, RTIMU_DATA*, qTarget*, float, float);
		void compute();
	private:
		void qPIDlimit();
		void qPIDcompute(float, float, float, uint64_t, qPIDvariables*);
		float qConstrain(float, float, float);
		void qCalibrateMotor();
};

#endif //_CONTROLLER_H_
