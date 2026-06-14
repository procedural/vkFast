#pragma once

#include <X11/Xlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_clipboard.h>

#define VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3

#define GLFW_CLIENT_API 0x00022001
#define GLFW_NO_API 0
#define GLFW_RESIZABLE 0x00020003

#define GLFW_RELEASE 0
#define GLFW_PRESS 1

#define GLFW_FALSE 0
#define GLFW_TRUE 1

// Globals start

int VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3_GLOBAL_hint_GLFW_RESIZABLE = 0;

// Globals end

typedef struct GLFWwindow {
  SDL_Window * sdlWindow;
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
    VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3_GLOBAL_hint_GLFW_RESIZABLE = 0;
    return;
  }
  if (hint == GLFW_RESIZABLE && value == GLFW_TRUE) {
    VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3_GLOBAL_hint_GLFW_RESIZABLE = 1;
    return;
  }
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline GLFWwindow * glfwCreateWindow(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share) {
  SDL_WindowFlags window_flags = 0;
  if (VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3_GLOBAL_hint_GLFW_RESIZABLE == 1) {
    window_flags |= SDL_WINDOW_RESIZABLE;
  }
  SDL_Window * sdlWindow = SDL_CreateWindow(title, width, height, window_flags);
  REDGPU_2_EXPECTFL(sdlWindow != NULL);

  const char * driver_name = SDL_GetCurrentVideoDriver();
  REDGPU_2_EXPECTFL(SDL_strcmp(driver_name, "x11") == 0 || !"The video driver is not X11. On Wayland, you need to run the app like this: SDL_VIDEO_DRIVER=x11 ./a.out");

  // To free
  GLFWwindow * h = (GLFWwindow *)red32MemoryCalloc(sizeof(GLFWwindow));
  REDGPU_2_EXPECTFL(h != NULL);

  // Filling
  GLFWwindow;
  h->sdlWindow = sdlWindow;
  h->windowShouldClose = 0;

  return h;
}

static inline void glfwTerminate(GLFWwindow * window) {
  if (window != NULL) {
    SDL_DestroyWindow(window->sdlWindow);
    red32MemoryFree(window);
    window = NULL;
  }
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

static inline void glfwPollEvents(GLFWwindow * window) {
  SDL_Event event = {0};
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      window->windowShouldClose = 1;
    }
  }
}

static inline void glfwGetWindowSize(GLFWwindow * window, int * width, int * height) {
  bool success = SDL_GetWindowSize(window->sdlWindow, width, height);
  REDGPU_2_EXPECTFL(success == true);
}

static inline void glfwGetCursorPos(GLFWwindow * window, double * xpos, double * ypos) {
  float x = 0;
  float y = 0;
  SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
  xpos[0] = (double)x;
  ypos[0] = (double)y;
}

static inline const char * glfwGetClipboardString(GLFWwindow * window) {
  return SDL_GetClipboardText();
}

static inline void glfwSetClipboardString(GLFWwindow * window, const char * string) {
  SDL_SetClipboardText(string);
}

static inline void glfwSetMouseButtonCallback(GLFWwindow * window, void * TODO_callback) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline void glfwSetScrollCallback(GLFWwindow * window, void * TODO_callback) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline void glfwSetKeyCallback(GLFWwindow * window, void * TODO_callback) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}

static inline void glfwSetCharCallback(GLFWwindow * window, void * TODO_callback) {
  REDGPU_2_EXPECTFL(!"TODO(Constantine)");
}
