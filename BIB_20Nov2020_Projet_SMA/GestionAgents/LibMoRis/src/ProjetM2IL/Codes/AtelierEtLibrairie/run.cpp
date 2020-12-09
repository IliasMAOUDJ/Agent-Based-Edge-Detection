#include "Atelier.h"
#include "Holon.h"
#include "HolonMessage.h"
#include "MachineA.h"
#include "MachineB.h"
#include "MachineC.h"
#include "Object2D.h"
#include "Piece.h"
#include "RandomScheduler.h"
#include "Utilities.h"

#include <stdio.h>
#include <string.h>

typedef struct {
  Scheduler * scheduler;
  Atelier * atelier;
  int autoscale;
  int pause;
  unsigned long time;
} AppliData;

int main() {
  AppliData * data;

  /*---- initialize graphic window ----*/
  graphic_init("Atelier","-*-helvetica-*-r-normal--14-*");
  graphic_setWidth(640);
  graphic_setHeight(480);
  graphic_setBackground("black");

  /*---- initialize application specific data ----*/
  data = (AppliData *)malloc(sizeof(AppliData));
  data->atelier = new Atelier();
  data->scheduler = new RandomScheduler();
  data->autoscale = 1;
  data->pause = 0;
  data->time = getTime();

  /*---- run the graphic application ----*/
  graphic_run(data);
  return(0);
}

void graphic_mainLoop(void * userData) {
  AppliData * data = (AppliData *) userData;
  if (data) {
    data->scheduler->cycle(Agent::getAgents(), 1);
  }
  graphic_autoscale();
}

void graphic_keyPressCallback(Object2D * obj2d, const char * key, void * userData) {
  AppliData * data = (AppliData *) userData;
  if (obj2d == NULL) {
    data->atelier->keyPress(key);
    return;
  }

  if((obj2d==data->atelier) || (obj2d==(Object2D *)data->atelier->_input) || (obj2d==(Object2D *)data->atelier->_output)) {
    return;
  }
  if (((Holon *)obj2d)->isA("Machine")) {
    ((Machine *)obj2d)->keyPress(key);
  }
}

void graphic_mouseDragCallback(Object2D * obj2d, double dx, double dy, void * userData) {
  double x,y,theta;
  AppliData * data = (AppliData *) userData;
  if ((obj2d==data->atelier) || (obj2d==(Object2D *)data->atelier->_input) || (obj2d==(Object2D *)data->atelier->_output)) {
    return;
  }
  obj2d->getLocation(x,y,theta);
  x+=dx;
  y+=dy;
  obj2d->setLocation(x,y,theta);
}
