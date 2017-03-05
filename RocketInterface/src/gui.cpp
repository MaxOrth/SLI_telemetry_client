#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "serial/serial.h"

#include "gui.h"

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
}

void Gui::Update(phys_data &data)
{
  dataList.push_back(data);
}

void Gui::Draw()
{
  DrawSerialInputSelector();
  //DrawMap();
  DrawGraph();
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
      if (ImGui::MenuItem(i->port.c_str()))
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


void Gui::DrawGraph()
{
  if (ImGui::Begin("Graphs", &graphOpen))
  {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImDrawList *drawList = window->DrawList;
    ImVec2 cursor = window->DC.CursorPos;


    // TODO make it the window content dims?
    float height = 300;
    float width = 600;

    float xmax = 100;
    float ymax = 500;
    
    ImVec2 origin(0, height);
    ImVec2 xAxisEnd(width, height);
    ImVec2 yAxisEnd(0, 0);

    drawList->AddLine(cursor + origin, cursor + yAxisEnd, IM_COL32_WHITE);
    drawList->AddLine(cursor + origin, cursor + xAxisEnd, IM_COL32_WHITE);
    ImVec2 prevPos(0, 0);
    for (auto i = dataList.begin(); i != dataList.end(); ++i)
    {
      ImVec2 next(width * i->time / xmax, ymax - i->altitude);
      drawList->AddLine(cursor + prevPos, cursor + next, IM_COL32(255, 0, 0, 255));
      prevPos = next;
    }



    ImGui::End();
  }
}

