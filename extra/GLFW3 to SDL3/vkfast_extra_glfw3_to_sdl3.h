#pragma once

#include <X11/Xlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_clipboard.h>

#ifndef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
#define VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
#endif

#define GLFW_CLIENT_API 0x00022001
#define GLFW_NO_API 0
#define GLFW_RESIZABLE 0x00020003

#define GLFW_RELEASE 0
#define GLFW_PRESS 1

#define GLFW_FALSE 0
#define GLFW_TRUE 1

#define GLFW_KEY_LEFT_SHIFT 340
#define GLFW_KEY_LEFT_CONTROL 341
#define GLFW_KEY_LEFT_ALT 342
#define GLFW_KEY_LEFT_SUPER 343
#define GLFW_KEY_RIGHT_SHIFT 344
#define GLFW_KEY_RIGHT_CONTROL 345
#define GLFW_KEY_RIGHT_ALT 346
#define GLFW_KEY_RIGHT_SUPER 347

#define GLFW_MOUSE_BUTTON_1 0
#define GLFW_MOUSE_BUTTON_2 1
#define GLFW_MOUSE_BUTTON_3 2
#define GLFW_MOUSE_BUTTON_4 3
#define GLFW_MOUSE_BUTTON_5 4
#define GLFW_MOUSE_BUTTON_6 5
#define GLFW_MOUSE_BUTTON_7 6
#define GLFW_MOUSE_BUTTON_8 7
#define GLFW_MOUSE_BUTTON_LAST GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3

#define GLFW_KEY_TAB 258
#define GLFW_KEY_LEFT 263
#define GLFW_KEY_RIGHT 262
#define GLFW_KEY_UP 265
#define GLFW_KEY_DOWN 264
#define GLFW_KEY_PAGE_UP 266
#define GLFW_KEY_PAGE_DOWN 267
#define GLFW_KEY_HOME 268
#define GLFW_KEY_END 269
#define GLFW_KEY_DELETE 261
#define GLFW_KEY_BACKSPACE 259
#define GLFW_KEY_ENTER 257
#define GLFW_KEY_ESCAPE 256
#define GLFW_KEY_A 65
#define GLFW_KEY_C 67
#define GLFW_KEY_D 68
#define GLFW_KEY_E 69
#define GLFW_KEY_Q 81
#define GLFW_KEY_S 83
#define GLFW_KEY_V 86
#define GLFW_KEY_W 87
#define GLFW_KEY_X 88
#define GLFW_KEY_Y 89
#define GLFW_KEY_Z 90
#define GLFW_KEY_MINUS 45 /* - */
#define GLFW_KEY_EQUAL 61 /* = */

#define GLFW_FOCUSED 0x00020001

#define GLFW_CURSOR 0x00033001
#define GLFW_CURSOR_NORMAL 0x00034001
#define GLFW_CURSOR_DISABLED 0x00034003

// Globals start

int VKFAST_EXTRA_GLOBAL_GLFW3_TO_SDL3_hint_GLFW_RESIZABLE = 0;

// Globals end

typedef struct GLFWwindow {
  SDL_Window * sdlWindow;
  SDL_WindowID sdlWindowId;
  int          windowShouldClose;
} GLFWwindow;

typedef struct GLFWmonitor {
  int _;
} GLFWmonitor;

static inline int glfwInit() {
  bool success = SDL_Init(SDL_INIT_VIDEO);
  REDGPU_2_EXPECTFL(success == true);
  return success;
}

static inline void glfwWindowHint(int hint, int value) {
  if (hint == GLFW_CLIENT_API && value == GLFW_NO_API) {
    // Nothing.
    return;
  }
  if (hint == GLFW_RESIZABLE && value == GLFW_FALSE) {
    VKFAST_EXTRA_GLOBAL_GLFW3_TO_SDL3_hint_GLFW_RESIZABLE = 0;
    return;
  }
  if (hint == GLFW_RESIZABLE && value == GLFW_TRUE) {
    VKFAST_EXTRA_GLOBAL_GLFW3_TO_SDL3_hint_GLFW_RESIZABLE = 1;
    return;
  }
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline GLFWwindow * glfwCreateWindow(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share) {
  SDL_WindowFlags window_flags = 0;
  if (VKFAST_EXTRA_GLOBAL_GLFW3_TO_SDL3_hint_GLFW_RESIZABLE == 1) {
    window_flags |= SDL_WINDOW_RESIZABLE;
  }
  SDL_Window * sdlWindow = SDL_CreateWindow(title, width, height, window_flags);
  REDGPU_2_EXPECTFL(sdlWindow != NULL);

  SDL_WindowID sdlWindowId = SDL_GetWindowID(sdlWindow);
  REDGPU_2_EXPECTFL(sdlWindowId != 0);

  const char * driver_name = SDL_GetCurrentVideoDriver();
  REDGPU_2_EXPECTFL(SDL_strcmp(driver_name, "x11") == 0 || !"The video driver is not X11. On Wayland, you need to run the app like this: SDL_VIDEO_DRIVER=x11 ./a.out");

  // To free
  GLFWwindow * h = (GLFWwindow *)red32MemoryCalloc(sizeof(GLFWwindow));
  REDGPU_2_EXPECTFL(h != NULL);

  // Filling
  GLFWwindow;
  h->sdlWindow = sdlWindow;
  h->sdlWindowId = sdlWindowId;
  h->windowShouldClose = 0;

  return h;
}

static inline void glfwTerminateWindow(GLFWwindow * window) {
  if (window != NULL) {
    SDL_DestroyWindow(window->sdlWindow);
    red32MemoryFree(window);
    window = NULL;
  }
}

static inline void glfwTerminate() {
  // Nothing.
}

static inline Display * glfwGetX11Display(GLFWwindow * window) {
  SDL_PropertiesID props = SDL_GetWindowProperties(window->sdlWindow);
  REDGPU_2_EXPECTFL(props != 0);
  return (Display *)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
}

static inline Window glfwGetX11Window(GLFWwindow * window) {
  SDL_PropertiesID props = SDL_GetWindowProperties(window->sdlWindow);
  REDGPU_2_EXPECTFL(props != 0);
  return (Window)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
}

static inline int glfwWindowShouldClose(GLFWwindow * window) {
  return window->windowShouldClose;
}

static inline void glfwPollEvents(int windows_count, GLFWwindow ** windows) {
  SDL_Event event = {0};
  while (SDL_PollEvent(&event)) {
    SDL_WindowID eventSdlWindowId = event.window.windowID;

    GLFWwindow * window = NULL;
    for (int i = 0; i < windows_count; i += 1) {
      if (windows[i]->sdlWindowId == eventSdlWindowId) {
        window = windows[i];
        break;
      }
    }

    if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      if (window == NULL) {
        for (int i = 0; i < windows_count; i += 1) {
          windows[i]->windowShouldClose = 1;
        }
      } else {
        window->windowShouldClose = 1;
      }
      continue;
    }
  }
}

static inline void glfwGetWindowSize(GLFWwindow * window, int * width, int * height) {
  bool success = SDL_GetWindowSize(window->sdlWindow, width, height);
  REDGPU_2_EXPECTFL(success == true);
}

static inline void glfwGetFramebufferSize(GLFWwindow * window, int * width, int * height) {
  bool success = SDL_GetWindowSize(window->sdlWindow, width, height);
  REDGPU_2_EXPECTFL(success == true);
}

static inline void glfwGetCursorPos(GLFWwindow * window, double * xpos, double * ypos) {
  float x = 0;
  float y = 0;
  SDL_MouseButtonFlags buttons = SDL_GetGlobalMouseState(&x, &y);

  int window_x = 0;
  int window_y = 0;
  SDL_GetWindowPosition(window->sdlWindow, &window_x, &window_y);

  float local_x = x - window_x;
  float local_y = y - window_y;

  xpos[0] = (double)local_x;
  ypos[0] = (double)local_y;
}

static inline const char * glfwGetClipboardString(GLFWwindow * window) {
  return SDL_GetClipboardText();
}

static inline void glfwSetClipboardString(GLFWwindow * window, const char * string) {
  SDL_SetClipboardText(string);
}

static inline void glfwSetMouseButtonCallback(GLFWwindow * window, void * TODO_callback) {
  return; // TODO(Constantine)
}

static inline void glfwSetScrollCallback(GLFWwindow * window, void * TODO_callback) {
  return; // TODO(Constantine)
}

static inline void glfwSetKeyCallback(GLFWwindow * window, void * TODO_callback) {
  return; // TODO(Constantine)
}

static inline void glfwSetCharCallback(GLFWwindow * window, void * TODO_callback) {
  return; // TODO(Constantine)
}

static inline int glfwGetWindowAttrib(GLFWwindow * window, int attrib) {
  SDL_WindowFlags flags = SDL_GetWindowFlags(window->sdlWindow);

  if (attrib == GLFW_FOCUSED) {
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0 ? 1 : 0;
  }

  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
  return 0;
}

static inline int glfwGetMouseButton(GLFWwindow * window, int button) {
  REDGPU_2_EXPECTFL(
    button == GLFW_MOUSE_BUTTON_LEFT   ||
    button == GLFW_MOUSE_BUTTON_RIGHT  ||
    button == GLFW_MOUSE_BUTTON_MIDDLE ||
    !"TODO(Constantine)"
  );

  float x = 0;
  float y = 0;
  SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    return (buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0 ? 1 : 0;
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    return (buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0 ? 1 : 0;
  }
  if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    return (buttons & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0 ? 1 : 0;
  }

  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
  return 0;
}

static inline void glfwSetInputMode(GLFWwindow * window, int mode, int value) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline int glfwGetKey(GLFWwindow * window, int key) {
  return 0; // TODO(Constantine)
}

static inline void glfwSetWindowTitle(GLFWwindow * window, const char * title) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}
