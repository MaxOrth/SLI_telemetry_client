
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "data_types.h"
#include "gui.h"

#include <ctime>
#include <iostream>

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit())
  {
    glfwTerminate();
    return 1;
  }

  ImGui_ImplGlfwGL3_Init(window, true);


  glClearColor(1, 1, 1, 1);

  Gui gui;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Poll for and process events */
    glfwPollEvents();

    ImGui_ImplGlfwGL3_NewFrame();

    size_t avail;
    if (avail = gui.GetSerial().available() >= sizeof(phys_data))
    {
      phys_data new_data;
      gui.GetSerial().read(reinterpret_cast<uint8_t *>(&new_data), sizeof(new_data));
      gui.Update(new_data);
    }

    gui.Draw();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

  }

  glfwTerminate();
  return 0;
}
