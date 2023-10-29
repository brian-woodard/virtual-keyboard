

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Stopwatch.h"
#include "Stats.h"

#define VSYNC_ENABLE 1

void processInput(GLFWwindow *window, int k, int s, int action, int mods);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void reshape(GLFWwindow *window, int width, int height);
void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

double x_pos, y_pos;
std::ostringstream cursorPos;

int width, height;
bool resize = false;
bool cursor_enabled = true;
bool test_enabled = true;

int mouse_x_offset, mouse_y_offset;
int cap_mouse_x_pos, cap_mouse_y_pos;
bool window_move = false;

int win_x_pos, win_y_pos;

CStats* stats = nullptr;

int main(int argc, char *argv[])
{
   int count = 0;
   GLFWmonitor **monitors = nullptr;
   GLFWwindow *window = nullptr;
   vec3 green(0.0f, 1.0f, 0.0f);
   double frame_time = 0.0;
   CStopwatch fps_timer;
   CStopwatch total_time;
   CStopwatch sleep_time;

   // glfw: initialize and configure
   // ------------------------------
   if (!glfwInit())
   {
      cout << "Failed to initialize GLFW" << endl;
      exit(EXIT_FAILURE);
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
   glfwWindowHint(GLFW_DECORATED, 0);
   glfwWindowHint(GLFW_FLOATING, 1);

   // glfw window creation
   // --------------------
   // try to create a window using a monitor
   monitors = glfwGetMonitors(&count);

   for (int i = 0; i < count; i++)
   {
      const GLFWvidmode *monMode = glfwGetVideoMode(monitors[i]);
      if (monMode->width == SCR_WIDTH &&
          monMode->height == SCR_HEIGHT)
      {
         // glfw window creation
         // --------------------
         window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IRES Symbology", monitors[i], NULL);
         if (window == NULL)
         {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
         }

         // we found our monitor
         break;
      }
   }

   // if we didn't find a monitor matching the width/height, just try creating one
   if (!window)
   {
      window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IRES Symbology", NULL, NULL);
      if (window == NULL)
      {
         std::cout << "Failed to create GLFW window" << std::endl;
         glfwTerminate();
         return -1;
      }
   }

   glfwMakeContextCurrent(window);

   glewInit();

   glfwSetWindowPos(window, 0, 0);
   glfwGetFramebufferSize(window, &width, &height);
   glfwSetKeyCallback(window, processInput);
   glfwSetMouseButtonCallback(window, MouseCallback);
   glfwSetFramebufferSizeCallback(window, reshape);
   glfwSetCursorPosCallback(window, cursorPositionCallback);
   glfwSwapInterval(VSYNC_ENABLE);

   GLCALL(glEnable(GL_BLEND));
   GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

   // setup projection
   glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
   glm::mat4 menu_projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

   stats = new CStats(width, height);

   // render loop
   // -----------
   while (!glfwWindowShouldClose(window))
   {
      if (resize)
      {
         projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
         menu_projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

         if (stats)
         {
            delete stats;
            stats = new CStats(width, height);
         }

         resize = false;
      }

      if (stats)
      {
         stats->Timer(CStats::TIMER_WAIT_FOR_NEXT_FRAME) = sleep_time.GetTime();
         total_time.Start();
      }

      GLCALL(glClearColor(0.2f, 0.2f, 0.2f, 0.95f));
      GLCALL(glClear(GL_COLOR_BUFFER_BIT));

      if (stats)
      {
         // scoped timer for symbology drawing
         CStopwatch stopwatch(&stats->Timer(CStats::TIMER_SYMB_DRAW));
      }

      stats->Timer(CStats::TIMER_TOTAL) = total_time.GetTime();

      int mouse_over = -1;
      if (stats)
      {
         if (stats->StatsEnabled())
            mouse_over = stats->MouseOverTimer((float)x_pos, (float)y_pos);

         if (stats->IsGrabbed())
         {
            stats->UpdateLine((float)x_pos, (float)y_pos);
         }

         stats->Draw(frame_time);
      }

      frame_time = fps_timer.GetTime();
      fps_timer.Start();

      std::string temp = cursorPos.str();

      // check mouse-over on timing lines
      if (mouse_over != -1)
      {
         char time_str[100] = {};
         double time_ms = stats->GetMouseOverTime();

         if (mouse_over == CStats::TIMER_TOTAL)
            sprintf(time_str, "Total Draw Time %.1f ms", time_ms);
         else if (mouse_over == CStats::TIMER_PROCESS_PDU)
            sprintf(time_str, "Process PDU Time %.1f ms", time_ms);
         else if (mouse_over == CStats::TIMER_SYMB_DRAW)
            sprintf(time_str, "Symbology Draw Time %.1f ms", time_ms);
         else if (mouse_over == CStats::TIMER_MENU_DRAW)
            sprintf(time_str, "Menu Draw Time %.1f ms", time_ms);

         temp = time_str;
      }

      if (cursor_enabled)
      {
         CText cursorPosition(menu_projection, 28);
         cursorPosition.SetInvertY(true);

         // if cursor is close to right edge, move test
         float str_width = cursorPosition.GetWidth(temp.c_str());
         float str_height = (float)cursorPosition.GetHeight();
         float move_left = 0.0;
         float move_down = 0.0;

         if ((float)x_pos > (float)width - str_width)
            move_left = str_width + (float)x_pos - (float)width;
         if ((float)y_pos < str_height)
            move_down = (float)y_pos - str_height;

         cursorPosition.Print(temp, (float)x_pos - move_left, (float)y_pos - move_down, 1.0f, green);
      }

      sleep_time.Start();

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();

      if (window_move)
      {
         glfwGetWindowPos(window, &win_x_pos, &win_y_pos);

         win_x_pos += mouse_x_offset;
         win_y_pos += mouse_y_offset;

         glfwSetWindowPos(window, win_x_pos, win_y_pos);

         //cap_mouse_x_pos += mouse_x_offset;
         //cap_mouse_y_pos += mouse_y_offset;
         mouse_x_offset = 0;
         mouse_y_offset = 0;
      }
   }

   // glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
   glfwTerminate();

   if (stats)
      delete stats;

   // Exit program
   return EXIT_SUCCESS;
}

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
   x_pos = xpos;
   y_pos = ypos;

   if (window_move)
   {
      mouse_x_offset = xpos - cap_mouse_x_pos;
      mouse_y_offset = ypos - cap_mouse_y_pos;
   }

   cursorPos.str("");
   cursorPos.clear();
   cursorPos.precision(2);
   cursorPos << "(" << std::fixed << x_pos << "," << y_pos << ")";
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, int k, int s, int action, int mods)
{
   if (action != GLFW_PRESS)
      return;

   switch (k)
   {
      case GLFW_KEY_Z:
         break;
      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, GLFW_TRUE);
         break;
      case GLFW_KEY_UP:
         break;
      case GLFW_KEY_DOWN:
         break;
      case GLFW_KEY_LEFT:
         break;
      case GLFW_KEY_RIGHT:
         break;
      case GLFW_KEY_C:
         cursor_enabled = !cursor_enabled;
         break;
      case GLFW_KEY_T:
         test_enabled = !test_enabled;
         break;
      case GLFW_KEY_F:
         if (stats)
            stats->ToggleFps();
         break;
      case GLFW_KEY_S:
         if (stats)
            stats->ToggleStats();
         break;
      case GLFW_KEY_P:
         if (stats)
            stats->TogglePause();
         break;
      case GLFW_KEY_1:
         if (stats)
            stats->ToggleTimer(CStats::TIMER_TOTAL);
         break;
      case GLFW_KEY_2:
         if (stats)
            stats->ToggleTimer(CStats::TIMER_PROCESS_PDU);
         break;
      case GLFW_KEY_3:
         if (stats)
            stats->ToggleTimer(CStats::TIMER_SYMB_DRAW);
         break;
      case GLFW_KEY_4:
         if (stats)
            stats->ToggleTimer(CStats::TIMER_MENU_DRAW);
         break;
      default:
         return;
   }
}

void MouseCallback(GLFWwindow *window, int button, int action, int mods)
{
   if (!stats)
      return;

   switch (button)
   {
      case GLFW_MOUSE_BUTTON_LEFT:
         if (action == GLFW_PRESS && !window_move)
         {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowPos(window, &win_x_pos, &win_y_pos);

            if (!window_move)
            {
               window_move = true;
               cap_mouse_x_pos = xpos;
               cap_mouse_y_pos = ypos;
            }
         }
         else if (action == GLFW_RELEASE)
         {
            window_move = false;
         }
         break;
      default:
         return;
   }
}

/* new window size */
void reshape(GLFWwindow *window, int win_width, int win_height)
{
   // set 0,0 to be at the bottom, but 0,0 is at the top left?
   GLCALL(glViewport(0, 0, win_width, win_height));
   width = win_width;
   height = win_height;
   resize = true;
}
