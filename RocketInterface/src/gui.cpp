#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "serial/serial.h"

#include "gui.h"

#include <ctime>


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
  dataList.push_back(data);
}

void Gui::Draw()
{
  DrawSerialInputSelector();
  //DrawMap();

  // draws a graph, but its a lot of work so... yaaaaaa
  DrawGraph();

  // lists every velocity
  ImGui::Begin("gps Stack");
  for (int i = 0; i < dataList.size(); ++i)
  {
    ImGui::Text("%.1f,  %.1f", dataList[i].gps.x, dataList[i].gps.y);
  }
  ImGui::End();
}

serial::Serial& Gui::GetSerial()
{
  return port;
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

