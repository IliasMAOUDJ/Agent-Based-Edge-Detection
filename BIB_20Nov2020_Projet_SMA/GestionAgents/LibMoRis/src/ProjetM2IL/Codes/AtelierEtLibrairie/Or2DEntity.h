#ifndef _Or2DEntity_H
#define _Or2DEntity_H

#include "Object2D.h"

class Or2DEntity : public Object2D {
 private:
  double _linearVelocityX, _linearVelocityY, _angularVelocity;
  double _linearAccelerationX, _linearAccelerationY, _angularAcceleration;
  int _oldTime;

 protected:
  // Calculating
  void _Kinematic(int);
  
 public:
  // Constructing
  Or2DEntity(double, double, double);
  Or2DEntity();

  // Destructing
  ~Or2DEntity();
  
  // Accessing (Position)
  void getXY(double &, double &);
  void setXY(double, double);
  
  // Accessing (Velocity) 
  double getLinearVelocityX();
  double getLinearVelocityY();
  double getAngularVelocity();
  void getLinearVelocity(double &, double &);
  void getVelocity(double &, double &,double &);
  
  void setLinearVelocityX(double);
  void setLinearVelocityY(double);
  void setAngularVelocity(double);
  void setLinearVelocity(double, double);
  void setVelocity(double, double, double);
  
  // Accessing (Acceleration)
  double getLinearAccelerationX();
  double getLinearAccelerationY();
  double getAngularAcceleration();
  void getLinearAcceleration(double &, double &);
  void getAcceleration(double &, double &, double &);
  
  void setLinearAccelerationX(double);
  void setLinearAccelerationY(double);
  void setAngularAcceleration(double);
  void setLinearAcceleration(double, double);
  void setAcceleration(double, double, double);
};

#endif
