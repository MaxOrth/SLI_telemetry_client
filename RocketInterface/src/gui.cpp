#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "serial/serial.h"

#include "gui.h"

#include <ctime>
#include <tuple>
#include <fstream>
#include <Windows.h>
#include <string>

namespace
{
  const long BAUDS[] = { 
    BAUD_9600, 
    BAUD_14400, 
    BAUD_19200,
    BAUD_28800,
    BAUD_38400,
    BAUD_56000,
    BAUD_57600,
    BAUD_115200
  };
}

Gui::Gui()
  : dataList(), currentPort(), port("", BAUD_115200), currentBaud(BAUD_115200), graphOpen(true), mapOpen(true)
{
  beginTime = std::time(nullptr);
}

void Gui::Update(phys_data &data)
{
  dataList.push_back({ data, ElapsedU() });
  std::ofstream outFile("out.txt", std::iostream::app);
  outFile << data.gps.x << "," << data.gps.y << std::endl;
  outFile.close();
}

void Gui::Draw()
{
  DrawSerialInputSelector();
  //DrawMap();

  // draws a graph, but its a lot of work so... yaaaaaa
  //DrawGraph();

  // help synchronize the serial connection
  ImGui::Begin("Serial setup");
  if (ImGui::Button("Connect"))
  {
    port.write("ABCDabcd");
  }
  ImGui::End();

  if (ImGui::Begin("Telemetry"))
  {
    if (!dataList.empty())
    {
      auto &lasttuple = dataList.back();
      phys_data &last = std::get<0>(lasttuple);
      if (ImGui::Button("Open in google maps"))
      {
        StartGoogleMaps(last.gps);
      }
      ImGui::Text("Recieved packet %l at %f:%f:%f", std::get<1>(lasttuple), last.timestamp.x, last.timestamp.y, last.timestamp.z);
      ImGui::Text("Fix quality: %i", static_cast<int>(last.flags));
      ImGui::Text("GPS: %.6f, %.6f", last.gps.x, last.gps.y);
      ImGui::Text("Altitude: %.1f", last.altitude);
      ImGui::Text("Orientation");
      ImGui::Text("x: %.3f", last.orientation.x);
      ImGui::Text("y: %.3f", last.orientation.y);
      ImGui::Text("z: %.3f", last.orientation.z);
      ImGui::Text("Acceleration");
      ImGui::Text("x: %.3f", last.acceleration.x);
      ImGui::Text("y: %.3f", last.acceleration.y);
      ImGui::Text("z: %.3f", last.acceleration.z);
      ImGui::Text("Angular Velocity");
      ImGui::Text("x: %.3f", last.angv.x);
      ImGui::Text("y: %.3f", last.angv.y);
      ImGui::Text("z: %.3f", last.angv.z);
    }
  }
  ImGui::End();
}

serial::Serial& Gui::GetSerial()
{
  return port;
}


void Gui::StartGoogleMaps(float2 latlon)
{
  // https://maps.google.com/maps?z=12&t=m&q=loc:38.9419+-78.3020 
  std::string cmd("\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\" ");
  cmd += "https://maps.google.com/maps/place/@";
  cmd += std::to_string(latlon.x) + ',' + std::to_string(latlon.y) + ",14z";
  system(cmd.c_str());
}

void Gui::DrawSerialInputSelector()
{
  std::vector<serial::PortInfo> devices = serial::list_ports();

  ImGui::BeginMainMenuBar();
  bool portChange = false;
  if (ImGui::BeginMenu("Port"))
  {
    for (auto i = devices.begin(); i != devices.end(); ++i)
    {
      if (ImGui::MenuItem(i->port.c_str(), nullptr, i->port == port.getPort()))
      {
        // selected port
        currentPort = i->port;
        portChange = true;
      }
    }
    ImGui::EndMenu();
  }
  
  if (portChange)
  {
    port.setPort(currentPort);
    if (!port.isOpen())
    {
      port.open();
      port.write("ABCDabcd");
    }
  }


  if (ImGui::BeginMenu("Baud"))
  {
    for (long const *i = BAUDS; i != BAUDS + sizeof(BAUDS)/sizeof(BAUDS[0]); ++i)
    {
      std::string baud = std::to_string(*i);
      if (ImGui::MenuItem(baud.c_str(), nullptr, *i == currentBaud))
      {
        port.setBaudrate(*i);
        currentBaud = *i;
      }
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

unsigned Gui::ElapsedU()
{
  return beginTime - std::time(NULL);
}

float Gui::Elapsed()
{
  return static_cast<float>(beginTime - std::time(NULL));
}

void Gui::DrawGraph()
{
  if (ImGui::Begin("Graphs", &graphOpen))
  {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImDrawList *drawList = window->DrawList;
    ImVec2 cursor = window->DC.CursorPos;





    ImGui::End();
  }
}

