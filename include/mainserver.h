#ifndef __MAIN_SERVER_H__
#define __MAIN_SERVER_H__

#include <Arduino.h>
#include <WiFiS3.h>
#include "global.h"

void webserver_task(void *pvParameters);
#endif