#pragma once

#include "data_types.h"

#include <vector>
#include <string>
#include "serial/serial.h"

#define BAUD_9600 9600
#define BAUD_14400 14400
#define BAUD_19200 19200
#define BAUD_28800 28800
#define BAUD_38400 38400
#define BAUD_56000 56000
#define BAUD_57600 57600
#define BAUD_115200 115200

class Gui
{
private:

  std::vector<phys_data> dataList;

  void DrawMap();
  void DrawSerialInputSelector();
  void DrawGraph();

  std::string currentPort;
  serial::Serial port;
  long currentBaud;

  bool graphOpen;
  bool mapOpen;

  time_t beginTime;
  
public:
  Gui();
  void Draw();

  void Update(phys_data &data);

  serial::Serial& GetSerial();

  float Elapsed();

};
