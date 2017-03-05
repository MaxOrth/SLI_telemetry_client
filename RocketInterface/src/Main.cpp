
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "gui.h"


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
