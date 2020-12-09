#include "Or2DEntity.h"
#include "Utilities.h"

#include <sys/time.h>

// Constructing
Or2DEntity::Or2DEntity():Object2D() {
  _linearVelocityX = 0;
  _linearVelocityY = 0;
  _angularVelocity = 0;
	
  _linearAccelerationX = 0;
  _linearAccelerationY = 0;
  _angularAcceleration = 0;
  
  _oldTime = getTime();
}
Or2DEntity::Or2DEntity(double x, double y, double theta):Object2D() {
  this->setLocation(x, y, theta);
  
  _linearVelocityX = 0;
  _linearVelocityY = 0;
  _angularVelocity = 0;
  
  _linearAccelerationX = 0;
  _linearAccelerationY = 0;
  _angularAcceleration = 0;
  
  _oldTime = getTime();
}

// Destructing
Or2DEntity::~Or2DEntity() {}

// Accessing (Position)
void Or2DEntity::getXY(double& x,double& y) {
  x = this->getX();
  y = this->getY();
}
void Or2DEntity::setXY(double x,double y) {
  this->setX(x);
  this->setY(y);
}

// Accessing (Velocity)
double Or2DEntity::getLinearVelocityX() {
  return _linearVelocityX;
}
double Or2DEntity::getLinearVelocityY() {
  return _linearVelocityY;
}
double Or2DEntity::getAngularVelocity() {
  return _angularVelocity;
}
void Or2DEntity::getLinearVelocity(double& xVelocity,double& yVelocity) {
  xVelocity = _linearVelocityX;
  yVelocity = _linearVelocityY;
}
void Or2DEntity::getVelocity(double& xVelocity,double& yVelocity,double& thetaVelocity) {
  xVelocity = _linearVelocityX;
  yVelocity = _linearVelocityY;
  thetaVelocity = _angularVelocity;
}
void Or2DEntity::setLinearVelocityX(double xVelocity) {
  _linearVelocityX = xVelocity;
}
void Or2DEntity::setLinearVelocityY(double yVelocity) {
  _linearVelocityY = yVelocity;
}
void Or2DEntity::setAngularVelocity(double thetaVelocity) {
  _angularVelocity = thetaVelocity;
}
void Or2DEntity::setLinearVelocity(double xVelocity,double yVelocity) {
  _linearVelocityX = xVelocity;
  _linearVelocityY = yVelocity;
}
void Or2DEntity::setVelocity(double xVelocity,double yVelocity,double thetaVelocity) {
  _linearVelocityX = xVelocity;
  _linearVelocityY = yVelocity;
  _angularVelocity = thetaVelocity;
}

// Accessing (Acceleration)
double Or2DEntity::getLinearAccelerationX() {
  return _linearAccelerationX;
}
double Or2DEntity::getLinearAccelerationY() {
  return _linearAccelerationY;
}
double Or2DEntity::getAngularAcceleration() {
  return _angularAcceleration;
}
void Or2DEntity::getLinearAcceleration(double & xAcceleration, double & yAcceleration) {
  xAcceleration = _linearAccelerationX;
  yAcceleration = _linearAccelerationY; 
}
void Or2DEntity::getAcceleration(double & xAcceleration, double & yAcceleration, double & thetaAcceleration) {
  xAcceleration = _linearAccelerationX;
  yAcceleration = _linearAccelerationY; 
  thetaAcceleration = _angularAcceleration;
}
void Or2DEntity::setLinearAccelerationX(double xAcceleration) {
  _linearAccelerationX = xAcceleration;
}
void Or2DEntity::setLinearAccelerationY(double yAcceleration) {
  _linearAccelerationY = yAcceleration;
}
void Or2DEntity::setAngularAcceleration(double thetaAcceleration) {
  _angularAcceleration = thetaAcceleration;
}
void Or2DEntity::setLinearAcceleration(double xAcceleration, double yAcceleration) {
  _linearAccelerationX = xAcceleration;
  _linearAccelerationY = yAcceleration;
}
void Or2DEntity::setAcceleration(double xAcceleration, double yAcceleration, double thetaAcceleration) {
  _linearAccelerationX = xAcceleration;
  _linearAccelerationY = yAcceleration;
  _angularAcceleration = thetaAcceleration;
}

// Calculating
void Or2DEntity::_Kinematic(int currentTime) {
  double deltaTime = ((double)(currentTime - _oldTime))/1000;
  _oldTime = currentTime;

  double deltaTime2 = deltaTime*deltaTime;

  double xold, yold, thetaold;
  double xnew, ynew, thetanew;

  this->getLocation(xold,yold,thetaold);
  
  xnew = 0.5 * _linearAccelerationX*deltaTime2 + _linearVelocityX*deltaTime + xold;
  ynew = 0.5 * _linearAccelerationY*deltaTime2 + _linearVelocityY*deltaTime + yold;
  thetanew = 0.5 * _angularAcceleration*deltaTime2 + _angularVelocity*deltaTime + thetaold;

  _linearVelocityX += _linearAccelerationX*deltaTime;
  _linearVelocityY += _linearAccelerationY*deltaTime;
  _angularVelocity += _angularAcceleration*deltaTime;

  if ((xnew!=xold) || (ynew!=yold) || (thetanew!=thetaold)) {
    this->setLocation(xnew,ynew,thetanew);
  }
}
