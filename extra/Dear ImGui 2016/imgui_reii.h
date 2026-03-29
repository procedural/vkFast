#pragma once

#include "cimgui.h"

typedef enum ImguiKey {
  IMGUI_KEY_TAB,
  IMGUI_KEY_LEFT,
  IMGUI_KEY_RIGHT,
  IMGUI_KEY_UP,
  IMGUI_KEY_DOWN,
  IMGUI_KEY_PAGE_UP,
  IMGUI_KEY_PAGE_DOWN,
  IMGUI_KEY_HOME,
  IMGUI_KEY_END,
  IMGUI_KEY_DELETE,
  IMGUI_KEY_BACKSPACE,
  IMGUI_KEY_ENTER,
  IMGUI_KEY_ESCAPE,
  IMGUI_KEY_A,
  IMGUI_KEY_C,
  IMGUI_KEY_V,
  IMGUI_KEY_X,
  IMGUI_KEY_Y,
  IMGUI_KEY_Z,
  IMGUI_KEY_COUNT,
} ImguiKey;

typedef enum ImguiColor {
  IMGUI_COLOR_TEXT,
  IMGUI_COLOR_TEXT_DISABLED,
  IMGUI_COLOR_WINDOW_BACKGROUND,
  IMGUI_COLOR_CHILD_WINDOW_BACKGROUND,
  IMGUI_COLOR_POPUP_BACKGROUND,
  IMGUI_COLOR_BORDER,
  IMGUI_COLOR_BORDER_SHADOW,
  IMGUI_COLOR_FRAME_BACKGROUND,
  IMGUI_COLOR_FRAME_BACKGROUND_HOVERED,
  IMGUI_COLOR_FRAME_BACKGROUND_ACTIVE,
  IMGUI_COLOR_TITLE_BACKGROUND,
  IMGUI_COLOR_TITLE_BACKGROUND_COLLAPSED,
  IMGUI_COLOR_TITLE_BACKGROUND_ACTIVE,
  IMGUI_COLOR_MENU_BAR_BACKGROUND,
  IMGUI_COLOR_SCROLLBAR_BACKGROUND,
  IMGUI_COLOR_SCROLLBAR_GRAB,
  IMGUI_COLOR_SCROLLBAR_GRAB_HOVERED,
  IMGUI_COLOR_SCROLLBAR_GRAB_ACTIVE,
  IMGUI_COLOR_COMBO_BACKGROUND,
  IMGUI_COLOR_CHECK_MARK,
  IMGUI_COLOR_SLIDER_GRAB,
  IMGUI_COLOR_SLIDER_GRAB_ACTIVE,
  IMGUI_COLOR_BUTTON,
  IMGUI_COLOR_BUTTON_HOVERED,
  IMGUI_COLOR_BUTTON_ACTIVE,
  IMGUI_COLOR_HEADER,
  IMGUI_COLOR_HEADER_HOVERED,
  IMGUI_COLOR_HEADER_ACTIVE,
  IMGUI_COLOR_COLUMN,
  IMGUI_COLOR_COLUMN_HOVERED,
  IMGUI_COLOR_COLUMN_ACTIVE,
  IMGUI_COLOR_RESIZE_GRIP,
  IMGUI_COLOR_RESIZE_GRIP_HOVERED,
  IMGUI_COLOR_RESIZE_GRIP_ACTIVE,
  IMGUI_COLOR_CLOSE_BUTTON,
  IMGUI_COLOR_CLOSE_BUTTON_HOVERED,
  IMGUI_COLOR_CLOSE_BUTTON_ACTIVE,
  IMGUI_COLOR_PLOT_LINES,
  IMGUI_COLOR_PLOT_LINES_HOVERED,
  IMGUI_COLOR_PLOT_HISTOGRAM,
  IMGUI_COLOR_PLOT_HISTOGRAM_HOVERED,
  IMGUI_COLOR_TEXT_SELECTED_BACKGROUND,
  IMGUI_COLOR_MODAL_WINDOW_DARKENING,
  IMGUI_COLOR_COUNT,
} ImguiColor;

typedef struct ImVec2 {
  float x;
  float y;
} ImVec2;

typedef struct ImVec3 {
  float x;
  float y;
  float z;
} ImVec3;

typedef struct ImVec4 {
  float x;
  float y;
  float z;
  float w;
} ImVec4;

typedef struct ImguiDrawData {
  bool                 isValid;
  struct ImDrawList ** commandLists;
  int                  commandListsCount;
  int                  totalVertexCount;
  int                  totalIndexCount;
} ImguiDrawData;

typedef struct ImguiIO {
  ImVec2               displaySize;
  float                deltaTime;
  float                iniSavingRate;
  char *               iniFilename;
  char *               logFilename;
  float                mouseDoubleClickTime;
  float                mouseDoubleClickMaxDistance;
  float                mouseDragThreshold;
  int                  keyMap[19];
  float                keyRepeatDelay;
  float                keyRepeatRate;
  void *               userData;
  struct ImFontAtlas * fonts;
  float                fontGlobalScale;
  bool                 fontAllowUserScaling;
  ImVec2               displayFramebufferScale;
  ImVec2               displayVisibleMin;
  ImVec2               displayVisibleMax;
  bool                 osxBehaviors;
  void               (*renderDrawLists)(ImguiDrawData * drawData);
  char *             (*getClipboardText)();
  void               (*setClipboardText)(char * text);
  void *             (*memoryAllocate)(size_t bytesCount);
  void               (*memoryFree)(void * pointer);
  void               (*setInputScreenPosition)(int x, int y);
  void *               windowHandle;
  ImVec2               mousePosition;
  bool                 mouseDown[5];
  float                mouseWheel;
  bool                 mouseDrawCursor;
  bool                 keyCtrl;
  bool                 keyShift;
  bool                 keyAlt;
  bool                 keySuper;
  bool                 keysDown[512];
  unsigned short       inputCharacters[17];
  bool                 wantCaptureMouse;
  bool                 wantCaptureKeyboard;
  bool                 wantTextInput;
  float                framerate;
  int                  metricsAllocations;
  int                  metricsRenderVertices;
  int                  metricsRenderIndices;
  int                  metricsActiveWindows;
  ImVec2               mousePositionPrevious;
  ImVec2               mouseDelta;
  bool                 mouseClicked[5];
  ImVec2               mouseClickedPosition[5];
  float                mouseClickedTime[5];
  bool                 mouseDoubleClicked[5];
  bool                 mouseReleased[5];
  bool                 mouseDownOwned[5];
  float                mouseDownDuration[5];
  float                mouseDownDurationPrevious[5];
  float                mouseDragMaxDistanceSquared[5];
  float                keysDownDuration[512];
  float                keysDownDurationPrevious[512];
} ImguiIO;

typedef struct ImguiDrawCommand {
  int                  elementsCount;
  ImVec4               clipRectangle;
  void *               data;
  ImDrawCallback       userCallback;
  void *               userCallbackData;
} ImguiDrawCommand;

typedef struct ImguiDrawVertex {
  ImVec2               position;
  ImVec2               uv;
  unsigned             color;
} ImguiDrawVertex;

typedef struct ImguiStyle {
  float                alpha;
  ImVec2               windowPadding;
  ImVec2               windowMinSize;
  float                windowRounding;
  ImVec2               windowTitleAlignment;
  float                childWindowRounding;
  ImVec2               framePadding;
  float                frameRounding;
  ImVec2               itemSpacing;
  ImVec2               itemInnerSpacing;
  ImVec2               touchExtraPadding;
  float                indentSpacing;
  float                columnsMinSpacing;
  float                scrollbarSize;
  float                scrollbarRounding;
  float                grabMinSize;
  float                grabRounding;
  ImVec2               buttonTextAlignment;
  ImVec2               displayWindowPadding;
  ImVec2               displaySafeAreaPadding;
  bool                 antialiasedLines;
  bool                 antialiasedShapes;
  float                curveTessellationTolerance;
  ImVec4               colors[43];
} ImguiStyle;

typedef struct ImguiState {
  GLFWwindow *            window;
  gpu_handle_context_t    gpuContext;
  ReiiMeshState           gpuMeshState;
  ReiiHandleCommandList   gpuCommandList;
  ReiiHandleTexture       gpuFontAtlas;
  double                  time;
  float                   mouseWheel;
  int                     mousePressed[3];
} ImguiState;

ImguiState   globalImguiStateData = {0};
ImguiState * globalImguiState     = &globalImguiStateData;

ImVec4 imguiUnpackUnorm4x8(unsigned p) {
  ImVec4 color;
  color.x = ((p & 0x000000FF)      ) / 255.f;
  color.y = ((p & 0x0000FF00) >> 8 ) / 255.f;
  color.z = ((p & 0x00FF0000) >> 16) / 255.f;
  color.w = ((p & 0xFF000000) >> 24) / 255.f;
  return color;
}

void imguiRenderDrawList(ImguiDrawData * drawData) {
  ImguiIO * io = (ImguiIO *)igGetIO();

  float windowWidth  = io->displaySize.x * io->displayFramebufferScale.x;
  float windowHeight = io->displaySize.y * io->displayFramebufferScale.y;
  if (windowWidth == 0 || windowHeight == 0) {
    return;
  }
  ImDrawData_ScaleClipRects((struct ImDrawData *)drawData, io->displayFramebufferScale);

  reiiCommandListSet(globalImguiState->gpuContext, globalImguiState->gpuCommandList);
  reiiCommandSetViewport(globalImguiState->gpuContext, 0, 0, windowWidth, windowHeight);
  for (int commandListIndex = 0; commandListIndex < drawData->commandListsCount; commandListIndex += 1) {
    struct ImDrawList * commandList     = drawData->commandLists[commandListIndex];
    ImguiDrawVertex   * pointerVertices = (ImguiDrawVertex *)((void *)ImDrawList_GetVertexPtr(commandList, 0));
    unsigned          * pointerIndices  = (unsigned *)((void *)ImDrawList_GetIndexPtr(commandList, 0));
    for (int commandIndex = 0; commandIndex < ImDrawList_GetCmdSize(commandList); commandIndex += 1) {
      ImguiDrawCommand * command = (ImguiDrawCommand *)ImDrawList_GetCmdPtr(commandList, commandIndex);
      if (command->userCallback != NULL) {
        command->userCallback((struct ImDrawList *)commandList, (struct ImDrawCmd *)command);
      } else {
        reiiCommandSetScissor(globalImguiState->gpuContext, (int)command->clipRectangle.x, (int)(windowHeight - command->clipRectangle.w), (int)(command->clipRectangle.z - command->clipRectangle.x), (int)(command->clipRectangle.w - command->clipRectangle.y));
        globalImguiState->gpuMeshTextureBindings.texture[0] = (ReiiHandleTexture)(intptr_t)command->data;
        reiiCommandMeshSetState(globalImguiState->gpuContext, &globalImguiState->gpuMeshState, &globalImguiState->gpuMeshTextureBindings);
        reiiCommandMeshSet(globalImguiState->gpuContext);
        for (unsigned i = 0; i < command->elementsCount; i += 1) {
          unsigned index = pointerIndices[i];
          ImVec4 color = imguiUnpackUnorm4x8(pointerVertices[index].color);
          reiiCommandMeshColor(globalImguiState->gpuContext, color.x, color.y, color.z, color.w);
          float xformWidth  = 2.f / windowWidth;
          float xformHeight = 2.f /-windowHeight;
          float positionX   = pointerVertices[index].position.x * xformWidth  - 1.f;
          float positionY   = pointerVertices[index].position.y * xformHeight + 1.f;
          reiiCommandMeshPosition(globalImguiState->gpuContext, positionX, positionY, pointerVertices[index].uv.x, pointerVertices[index].uv.y);
        }
        reiiCommandMeshEnd(globalImguiState->gpuContext);
      }
      pointerIndices += command->elementsCount;
    }
  }
  reiiCommandListEnd(globalImguiState->gpuContext);

  reiiSubmitCommandLists(globalImguiState->gpuContext, 1, &globalImguiState->gpuCommandList);
}

char * imguiGLFW3ClipboardTextGet() {
  return (char *)glfwGetClipboardString(globalImguiState->window);
}

void imguiGLFW3ClipboardTextSet(char * text) {
  glfwSetClipboardString(globalImguiState->window, text);
}

void imguiGLFW3MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button >= 0 && button < 3) {
    globalImguiState->mousePressed[button] = 1;
  }
}

void imguiGLFW3ScrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
  ImguiIO * io = (ImguiIO *)igGetIO();
  globalImguiState->mouseWheel = (float)yoffset;
}

void imguiGLFW3KeyCallback(GLFWwindow * window, int key, int keycode, int action, int mods) {
  ImguiIO * io = (ImguiIO *)igGetIO();
  if (action == GLFW_PRESS) {
    io->keysDown[key] = 1;
  }
  if (action == GLFW_RELEASE) {
    io->keysDown[key] = 0;
  }
  io->keyCtrl  = io->keysDown[GLFW_KEY_LEFT_CONTROL] || io->keysDown[GLFW_KEY_RIGHT_CONTROL];
  io->keyShift = io->keysDown[GLFW_KEY_LEFT_SHIFT]   || io->keysDown[GLFW_KEY_RIGHT_SHIFT];
  io->keyAlt   = io->keysDown[GLFW_KEY_LEFT_ALT]     || io->keysDown[GLFW_KEY_RIGHT_ALT];
  io->keySuper = io->keysDown[GLFW_KEY_LEFT_SUPER]   || io->keysDown[GLFW_KEY_RIGHT_SUPER];
}

void imguiGLFW3CharCallback(GLFWwindow * window, unsigned int c) {
  if (c > 0 && c < 0x10000) {
    ImGuiIO_AddInputCharacter((unsigned short)c);
  }
}

static inline void imguiCreateFontTexture() {
  ImguiIO * io = (ImguiIO *)igGetIO();

  unsigned char * data = NULL;
  int width  = 0;
  int height = 0;
  int bpp    = 0;
  ImFontAtlas_GetTexDataAsRGBA32(io->fonts, &data, &width, &height, &bpp);

  reiiCreateTexture(globalImguiState->gpuContext, &globalImguiState->gpuFontAtlas);
  reiiTextureSetStateMipmap(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, globalImguiState->gpuFontAtlas, 0);
  reiiTextureSetStateSampler(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, globalImguiState->gpuFontAtlas, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);
  reiiTextureDefineAndCopyFromCpu(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, globalImguiState->gpuFontAtlas, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, width, height, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, data);

  ImFontAtlas_SetTexID(io->fonts, (void *)(intptr_t)globalImguiState->gpuFontAtlas);
}

static inline void imguiCreateDeviceObjects() {
  globalImguiState->gpuMeshTextureBindings.binding[0] = REII_TEXTURE_BINDING_2D;

  char * vp_string =
    "!!ARBvp1.0"
    "PARAM c[1] = {{0,1}};"
    "MOV result.texcoord[0], vertex.color;"
    "MOV result.texcoord[1].xy, vertex.position.zwzw;"
    "MOV result.position.xy, vertex.position;"
    "MOV result.position.zw, c[0].xyxy;"
    "END";

  char * fp_string =
    "!!ARBfp1.0"
    "TEMP R0;"
    "TEX R0, fragment.texcoord[1], texture[0], 2D;"
    "MUL result.color, fragment.texcoord[0], R0;"
    "END";

  globalImguiState->gpuMeshState.codeVertex                                     = vp_string;
  globalImguiState->gpuMeshState.codeFragment                                   = fp_string;
  globalImguiState->gpuMeshState.rasterizationDepthClampEnable                  = 0;
  globalImguiState->gpuMeshState.rasterizationCullMode                          = REII_CULL_MODE_BACK;
  globalImguiState->gpuMeshState.rasterizationFrontFace                         = REII_FRONT_FACE_CLOCKWISE;
  globalImguiState->gpuMeshState.rasterizationDepthBiasEnable                   = 0;
  globalImguiState->gpuMeshState.rasterizationDepthBiasConstantFactor           = 0;
  globalImguiState->gpuMeshState.rasterizationDepthBiasSlopeFactor              = 0;
  globalImguiState->gpuMeshState.multisampleEnable                              = 0;
  globalImguiState->gpuMeshState.multisampleAlphaToCoverageEnable               = 0;
  globalImguiState->gpuMeshState.multisampleAlphaToOneEnable                    = 0;
  globalImguiState->gpuMeshState.depthTestEnable                                = 0;
  globalImguiState->gpuMeshState.depthTestDepthWriteEnable                      = 0;
  globalImguiState->gpuMeshState.depthTestDepthCompareOp                        = REII_COMPARE_OP_GREATER_OR_EQUAL;
  globalImguiState->gpuMeshState.stencilTestEnable                              = 0;
  globalImguiState->gpuMeshState.stencilTestFrontStencilTestFailOp              = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestFrontStencilTestPassDepthTestPassOp = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestFrontStencilTestPassDepthTestFailOp = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestFrontCompareOp                      = REII_COMPARE_OP_NEVER;
  globalImguiState->gpuMeshState.stencilTestBackStencilTestFailOp               = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestBackStencilTestPassDepthTestPassOp  = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestBackStencilTestPassDepthTestFailOp  = REII_STENCIL_OP_KEEP;
  globalImguiState->gpuMeshState.stencilTestBackCompareOp                       = REII_COMPARE_OP_NEVER;
  globalImguiState->gpuMeshState.stencilTestFrontAndBackCompareMask             = 0;
  globalImguiState->gpuMeshState.stencilTestFrontAndBackWriteMask               = 0;
  globalImguiState->gpuMeshState.stencilTestFrontAndBackReference               = 0;
  globalImguiState->gpuMeshState.blendLogicOpEnable                             = 0;
  globalImguiState->gpuMeshState.blendLogicOp                                   = REII_LOGIC_OP_CLEAR;
  globalImguiState->gpuMeshState.blendConstants[0]                              = 0;
  globalImguiState->gpuMeshState.blendConstants[1]                              = 0;
  globalImguiState->gpuMeshState.blendConstants[2]                              = 0;
  globalImguiState->gpuMeshState.blendConstants[3]                              = 0;
  globalImguiState->gpuMeshState.outputColorWriteEnableR                        = 1;
  globalImguiState->gpuMeshState.outputColorWriteEnableG                        = 1;
  globalImguiState->gpuMeshState.outputColorWriteEnableB                        = 1;
  globalImguiState->gpuMeshState.outputColorWriteEnableA                        = 1;
  globalImguiState->gpuMeshState.outputColorBlendEnable                         = 1;
  globalImguiState->gpuMeshState.outputColorBlendColorFactorSource              = REII_BLEND_FACTOR_SOURCE_ALPHA;
  globalImguiState->gpuMeshState.outputColorBlendColorFactorTarget              = REII_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
  globalImguiState->gpuMeshState.outputColorBlendColorOp                        = REII_BLEND_OP_ADD;
  globalImguiState->gpuMeshState.outputColorBlendAlphaFactorSource              = REII_BLEND_FACTOR_SOURCE_ALPHA;
  globalImguiState->gpuMeshState.outputColorBlendAlphaFactorTarget              = REII_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
  globalImguiState->gpuMeshState.outputColorBlendAlphaOp                        = REII_BLEND_OP_ADD;

  reiiCreateCommandList(globalImguiState->gpuContext, &globalImguiState->gpuCommandList);

  imguiCreateFontTexture();
}

static void inline imguiNewFrame() {
  ImguiIO * io = (ImguiIO *)igGetIO();

  if (globalImguiState->gpuFontAtlas == 0) {
    imguiCreateDeviceObjects();
  }

  int w = 0;
  int h = 0;
  int displayW = 0;
  int displayH = 0;
  glfwGetWindowSize(globalImguiState->window, &w, &h);
  glfwGetFramebufferSize(globalImguiState->window, &displayW, &displayH);
  io->displaySize = (ImVec2){(float)w, (float)h};
  io->displayFramebufferScale = (ImVec2){w > 0 ? ((float)displayW / w) : 0, h > 0 ? ((float)displayH / h) : 0};

  double currentTime = glfwGetTime();
  io->deltaTime = globalImguiState->time > 0.0 ? (float)(currentTime - globalImguiState->time) : (float)(1.0f / 60.0f);
  globalImguiState->time = currentTime;

  if (glfwGetWindowAttrib(globalImguiState->window, GLFW_FOCUSED)) {
    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(globalImguiState->window, &mouse_x, &mouse_y);
    io->mousePosition = (ImVec2){(float)mouse_x, (float)mouse_y};
  } else {
    io->mousePosition = (ImVec2){-1.f, -1.f};
  }

  for (int i = 0; i < 3; i += 1) {
    io->mouseDown[i] = globalImguiState->mousePressed[i] == 1 || glfwGetMouseButton(globalImguiState->window, i) != 0; // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    globalImguiState->mousePressed[i] = 0;
  }

  io->mouseWheel = globalImguiState->mouseWheel;
  globalImguiState->mouseWheel = 0;

  igNewFrame();
}

static inline void imguiInvalidateDeviceObjects() {
  ImguiIO * io = (ImguiIO *)igGetIO();
  if (globalImguiState->gpuMeshState.programVertex != 0) {
    reiiDestroyProgram(globalImguiState->gpuContext, globalImguiState->gpuMeshState.programVertex);
    globalImguiState->gpuMeshState.programVertex = 0;
  }
  if (globalImguiState->gpuMeshState.programFragment != 0) {
    reiiDestroyProgram(globalImguiState->gpuContext, globalImguiState->gpuMeshState.programFragment);
    globalImguiState->gpuMeshState.programFragment = 0;
  }
  if (globalImguiState->gpuCommandList != 0) {
    reiiDestroyCommandList(globalImguiState->gpuContext, globalImguiState->gpuCommandList);
    globalImguiState->gpuCommandList = 0;
  }
  if (globalImguiState->gpuFontAtlas != 0) {
    reiiDestroyTexture(globalImguiState->gpuContext, globalImguiState->gpuFontAtlas);
    globalImguiState->gpuFontAtlas = 0;
    ImFontAtlas_SetTexID(io->fonts, 0);
  }
}

static inline void imguiDeinit() {
  imguiInvalidateDeviceObjects();
  igShutdown();
}

static inline void imguiInit(GLFWwindow * window, ReiiContext * context) {
  ImguiIO * io = (ImguiIO *)igGetIO();

  globalImguiState->window     = window;
  globalImguiState->gpuContext = context;

  io->keyMap[IMGUI_KEY_TAB]       = GLFW_KEY_TAB;
  io->keyMap[IMGUI_KEY_LEFT]      = GLFW_KEY_LEFT;
  io->keyMap[IMGUI_KEY_RIGHT]     = GLFW_KEY_RIGHT;
  io->keyMap[IMGUI_KEY_UP]        = GLFW_KEY_UP;
  io->keyMap[IMGUI_KEY_DOWN]      = GLFW_KEY_DOWN;
  io->keyMap[IMGUI_KEY_PAGE_UP]   = GLFW_KEY_PAGE_UP;
  io->keyMap[IMGUI_KEY_PAGE_DOWN] = GLFW_KEY_PAGE_DOWN;
  io->keyMap[IMGUI_KEY_HOME]      = GLFW_KEY_HOME;
  io->keyMap[IMGUI_KEY_END]       = GLFW_KEY_END;
  io->keyMap[IMGUI_KEY_DELETE]    = GLFW_KEY_DELETE;
  io->keyMap[IMGUI_KEY_BACKSPACE] = GLFW_KEY_BACKSPACE;
  io->keyMap[IMGUI_KEY_ENTER]     = GLFW_KEY_ENTER;
  io->keyMap[IMGUI_KEY_ESCAPE]    = GLFW_KEY_ESCAPE;
  io->keyMap[IMGUI_KEY_A]         = GLFW_KEY_A;
  io->keyMap[IMGUI_KEY_C]         = GLFW_KEY_C;
  io->keyMap[IMGUI_KEY_V]         = GLFW_KEY_V;
  io->keyMap[IMGUI_KEY_X]         = GLFW_KEY_X;
  io->keyMap[IMGUI_KEY_Y]         = GLFW_KEY_Y;
  io->keyMap[IMGUI_KEY_Z]         = GLFW_KEY_Z;

  io->renderDrawLists  = imguiRenderDrawList;
  io->getClipboardText = imguiGLFW3ClipboardTextGet;
  io->setClipboardText = imguiGLFW3ClipboardTextSet;

  glfwSetMouseButtonCallback(window, imguiGLFW3MouseButtonCallback);
  glfwSetScrollCallback(window,      imguiGLFW3ScrollCallback);
  glfwSetKeyCallback(window,         imguiGLFW3KeyCallback);
  glfwSetCharCallback(window,        imguiGLFW3CharCallback);
}

static inline void imguiEasyTheming(ImVec3 colorText, ImVec3 colorHead, ImVec3 colorArea, ImVec3 colorBody, ImVec3 colorPops) {
  ImguiStyle * style = (ImguiStyle *)igGetStyle();

  style->colors[IMGUI_COLOR_TEXT]                       = (ImVec4){colorText.x, colorText.y, colorText.z, 1.00f};
  style->colors[IMGUI_COLOR_TEXT_DISABLED]              = (ImVec4){colorText.x, colorText.y, colorText.z, 0.58f};
  style->colors[IMGUI_COLOR_WINDOW_BACKGROUND]          = (ImVec4){colorBody.x, colorBody.y, colorBody.z, 1.00f};
  style->colors[IMGUI_COLOR_CHILD_WINDOW_BACKGROUND]    = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 0.58f};
  style->colors[IMGUI_COLOR_BORDER]                     = (ImVec4){colorBody.x, colorBody.y, colorBody.z, 0.00f};
  style->colors[IMGUI_COLOR_BORDER_SHADOW]              = (ImVec4){colorBody.x, colorBody.y, colorBody.z, 0.00f};
  style->colors[IMGUI_COLOR_FRAME_BACKGROUND]           = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 1.00f};
  style->colors[IMGUI_COLOR_FRAME_BACKGROUND_HOVERED]   = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.78f};
  style->colors[IMGUI_COLOR_FRAME_BACKGROUND_ACTIVE]    = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_TITLE_BACKGROUND]           = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 1.00f};
  style->colors[IMGUI_COLOR_TITLE_BACKGROUND_COLLAPSED] = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 0.75f};
  style->colors[IMGUI_COLOR_TITLE_BACKGROUND_ACTIVE]    = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_MENU_BAR_BACKGROUND]        = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 1.00f};
  style->colors[IMGUI_COLOR_SCROLLBAR_BACKGROUND]       = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 1.00f};
  style->colors[IMGUI_COLOR_SCROLLBAR_GRAB]             = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.50f};
  style->colors[IMGUI_COLOR_SCROLLBAR_GRAB_HOVERED]     = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.78f};
  style->colors[IMGUI_COLOR_SCROLLBAR_GRAB_ACTIVE]      = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_COMBO_BACKGROUND]           = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 1.00f};
  style->colors[IMGUI_COLOR_CHECK_MARK]                 = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.80f};
  style->colors[IMGUI_COLOR_SLIDER_GRAB]                = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.50f};
  style->colors[IMGUI_COLOR_SLIDER_GRAB_ACTIVE]         = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_BUTTON]                     = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.50f};
  style->colors[IMGUI_COLOR_BUTTON_HOVERED]             = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.86f};
  style->colors[IMGUI_COLOR_BUTTON_ACTIVE]              = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_HEADER]                     = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.76f};
  style->colors[IMGUI_COLOR_HEADER_HOVERED]             = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.86f};
  style->colors[IMGUI_COLOR_HEADER_ACTIVE]              = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_COLUMN]                     = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.32f};
  style->colors[IMGUI_COLOR_COLUMN_HOVERED]             = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.78f};
  style->colors[IMGUI_COLOR_COLUMN_ACTIVE]              = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_RESIZE_GRIP]                = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.15f};
  style->colors[IMGUI_COLOR_RESIZE_GRIP_HOVERED]        = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.78f};
  style->colors[IMGUI_COLOR_RESIZE_GRIP_ACTIVE]         = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_CLOSE_BUTTON]               = (ImVec4){colorText.x, colorText.y, colorText.z, 0.16f};
  style->colors[IMGUI_COLOR_CLOSE_BUTTON_HOVERED]       = (ImVec4){colorText.x, colorText.y, colorText.z, 0.39f};
  style->colors[IMGUI_COLOR_CLOSE_BUTTON_ACTIVE]        = (ImVec4){colorText.x, colorText.y, colorText.z, 1.00f};
  style->colors[IMGUI_COLOR_PLOT_LINES]                 = (ImVec4){colorText.x, colorText.y, colorText.z, 0.63f};
  style->colors[IMGUI_COLOR_PLOT_LINES_HOVERED]         = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_PLOT_HISTOGRAM]             = (ImVec4){colorText.x, colorText.y, colorText.z, 0.63f};
  style->colors[IMGUI_COLOR_PLOT_HISTOGRAM_HOVERED]     = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 1.00f};
  style->colors[IMGUI_COLOR_TEXT_SELECTED_BACKGROUND]   = (ImVec4){colorHead.x, colorHead.y, colorHead.z, 0.43f};
  style->colors[IMGUI_COLOR_POPUP_BACKGROUND]           = (ImVec4){colorPops.x, colorPops.y, colorPops.z, 0.92f};
  style->colors[IMGUI_COLOR_MODAL_WINDOW_DARKENING]     = (ImVec4){colorArea.x, colorArea.y, colorArea.z, 0.73f};
}