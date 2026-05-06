#include "App.h"
#include "canTinyTimber.h"
#include "sciTinyTimber.h"

App app = initApp();
Can can0 = initCan(CAN_PORT0, &app, receiver);
Serial sci0 = initSerial(SCI_PORT0, &app, reader);
Background background = initBackground();