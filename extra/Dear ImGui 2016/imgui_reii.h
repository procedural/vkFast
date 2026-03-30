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
  ReiiHandleTextureMemory gpuFontAtlasMemory;
  ReiiCpuScratchBuffer    gpuFontAtlasScratchBuffer;
  ReiiHandleTexture       gpuFontAtlas;
  RedHandleSampler        gpuSampler;
  uint64_t                gpuBatch;
  ReiiHandleCommandList   gpuCommandList;
  uint64_t                gpuMaxNewBindingsSetsCount;
  gpu_extra_cpu_gpu_array gpuDynamicMeshPosition;
  gpu_extra_cpu_gpu_array gpuDynamicMeshColor;
  Red2Output *            gpuMutableOutputsArray;
  ReiiHandleTexture *     gpuOutputTexture;
  unsigned                gpuOptionalQueueFamilyIndex;
  RedHandleQueue          gpuOptionalQueue;
  double                  time;
  float                   mouseWheel;
  int                     mousePressed[3];
  ReiiBool32              processInputs;
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

  gpu_batch_info_t bindings_info = {0};
  bindings_info.max_new_bindings_sets_count = globalImguiState->gpuMaxNewBindingsSetsCount;
  bindings_info.max_storage_binds_count     = 2 * globalImguiState->gpuMaxNewBindingsSetsCount;
  bindings_info.max_texture_ro_binds_count  = 1 * globalImguiState->gpuMaxNewBindingsSetsCount;
  bindings_info.max_sampler_binds_count     = 1; // NOTE(Constantine): Intentionally not multiplying by num_of_structs_to_allocate: samplers are global, not per-struct.
  if (globalImguiState->gpuOptionalQueue == NULL) {
    globalImguiState->gpuBatch = vfBatchBegin(globalImguiState->gpuContext, globalImguiState->gpuBatch, &bindings_info, NULL, __FILE__, __LINE__);
  } else {
    globalImguiState->gpuBatch = vfBatchBeginEx(globalImguiState->gpuContext, globalImguiState->gpuBatch, &bindings_info, globalImguiState->gpuOptionalQueueFamilyIndex, NULL, __FILE__, __LINE__);
  }
  ReiiHandleCommandList * list = &globalImguiState->gpuCommandList;
  list->batch_id = globalImguiState->gpuBatch;
  reiiCommandListReset(globalImguiState->gpuContext, list);
  reiiCommandSetViewportEx(globalImguiState->gpuContext, list, 0, 0, windowWidth, windowHeight, 0, 1);
  reiiCommandMeshSetState(globalImguiState->gpuContext, list, &globalImguiState->gpuMeshState, NULL);
  reiiCommandBindSamplers(globalImguiState->gpuContext, list, 1, &globalImguiState->gpuSampler);
  for (int commandListIndex = 0; commandListIndex < drawData->commandListsCount; commandListIndex += 1) {
    struct ImDrawList * commandList     = drawData->commandLists[commandListIndex];
    ImguiDrawVertex   * pointerVertices = (ImguiDrawVertex *)((void *)ImDrawList_GetVertexPtr(commandList, 0));
    unsigned          * pointerIndices  = (unsigned *)((void *)ImDrawList_GetIndexPtr(commandList, 0));
    for (int commandIndex = 0; commandIndex < ImDrawList_GetCmdSize(commandList); commandIndex += 1) {
      ImguiDrawCommand * command = (ImguiDrawCommand *)ImDrawList_GetCmdPtr(commandList, commandIndex);
      if (command->userCallback != NULL) {
        command->userCallback((struct ImDrawList *)commandList, (struct ImDrawCmd *)command);
      } else {
        // Project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_min = {command->clipRectangle.x, command->clipRectangle.y};
        ImVec2 clip_max = {command->clipRectangle.z, command->clipRectangle.w};
        // Clamp to viewport as vkCmdSetScissor() won't accept values that are off bounds
        if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
        if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
        if (clip_max.x > windowWidth) { clip_max.x = (float)windowWidth; }
        if (clip_max.y > windowHeight) { clip_max.y = (float)windowHeight; }
        if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) {
          continue;
        }
        reiiCommandSetScissor(globalImguiState->gpuContext, list, clip_min.x, clip_min.y, clip_max.x - clip_min.x, clip_max.y - clip_min.y);
        ReiiHandleTexture * textureToBind = (ReiiHandleTexture *)(intptr_t)command->data;
        RedStructDeclarationMember slots[3] = {0};
        slots[0].slot            = 0;
        slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
        slots[0].count           = 1;
        slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
        slots[1].slot            = 1;
        slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
        slots[1].count           = 1;
        slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
        slots[2].slot            = 2;
        slots[2].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
        slots[2].count           = 1;
        slots[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
        reiiCommandBindNewBindingsSet(globalImguiState->gpuContext, list, _countof(slots), slots);
        gpu_extra_cpu_gpu_array dynamicMeshPositionOffsetted = list->dynamic_mesh_position;
        gpu_extra_cpu_gpu_array dynamicMeshColorOffsetted    = list->dynamic_mesh_color;
        // NOTE(Constantine): Intel GPUs want 64 byte aligned array start addresses.
        while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshPositionVec4Offset * sizeof(ReiiVec4), 64) > 0) {
          list->dynamicMeshPositionVec4Offset += 1;
        }
        while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshColorVec4Offset * sizeof(ReiiVec4), 64) > 0) {
          list->dynamicMeshColorVec4Offset += 1;
        }
        vfeCpuGpuArrayOffset(&dynamicMeshPositionOffsetted, list->dynamicMeshPositionVec4Offset * sizeof(ReiiVec4));
        vfeCpuGpuArrayOffset(&dynamicMeshColorOffsetted, list->dynamicMeshColorVec4Offset * sizeof(ReiiVec4));
        reiiCommandBindStorageRaw(globalImguiState->gpuContext, list, 0, 1, &dynamicMeshPositionOffsetted.gpu);
        reiiCommandBindStorageRaw(globalImguiState->gpuContext, list, 1, 1, &dynamicMeshColorOffsetted.gpu);
        RedStructMemberTexture texture = {0};
        texture.sampler = NULL;
        texture.texture = textureToBind->texture;
        texture.setTo1  = 1;
        reiiCommandBindTextureRO(globalImguiState->gpuContext, list, 2, 1, &texture);
        reiiCommandBindNewBindingsEnd(globalImguiState->gpuContext, list);
        reiiCommandMeshSet(globalImguiState->gpuContext, list);
        for (unsigned i = 0; i < command->elementsCount; i += 1) {
          unsigned index = pointerIndices[i];
          ImVec4 color = imguiUnpackUnorm4x8(pointerVertices[index].color);
          reiiCommandMeshColor(globalImguiState->gpuContext, list, color.x, color.y, color.z, color.w);
          float xformWidth  = 2.f / windowWidth;
          float xformHeight = 2.f /-windowHeight;
          float positionX   = pointerVertices[index].position.x * xformWidth  - 1.f;
          float positionY   = pointerVertices[index].position.y * xformHeight + 1.f;
          reiiCommandMeshPosition(globalImguiState->gpuContext, list, positionX, positionY, pointerVertices[index].uv.x, pointerVertices[index].uv.y);
        }
        reiiCommandMeshEndEx(globalImguiState->gpuContext, list, NULL, globalImguiState->gpuOutputTexture, globalImguiState->gpuOutputTexture->texture);
      }
      pointerIndices += command->elementsCount;
    }
  }
  vfBatchEnd(globalImguiState->gpuContext, globalImguiState->gpuBatch, __FILE__, __LINE__);

  uint64_t wait = 0;
  if (globalImguiState->gpuOptionalQueue == NULL) {
    wait = vfAsyncBatchExecute(globalImguiState->gpuContext, 1, &globalImguiState->gpuBatch, __FILE__, __LINE__);
  } else {
    wait = vfAsyncBatchExecuteEx(globalImguiState->gpuContext, globalImguiState->gpuOptionalQueue, 1, &globalImguiState->gpuBatch, __FILE__, __LINE__);
  }
  vfAsyncWaitToFinish(globalImguiState->gpuContext, wait, __FILE__, __LINE__);
}

char * imguiGLFW3ClipboardTextGet() {
  return (char *)glfwGetClipboardString(globalImguiState->window);
}

void imguiGLFW3ClipboardTextSet(char * text) {
  glfwSetClipboardString(globalImguiState->window, text);
}

void imguiGLFW3MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
  if (globalImguiState->processInputs == 0) {
    return;
  }
  if (action == GLFW_PRESS && button >= 0 && button < 3) {
    globalImguiState->mousePressed[button] = 1;
  }
}

void imguiGLFW3ScrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
  ImguiIO * io = (ImguiIO *)igGetIO();
  if (globalImguiState->processInputs == 0) {
    globalImguiState->mouseWheel = 0;
    return;
  }
  globalImguiState->mouseWheel = (float)yoffset;
}

void imguiGLFW3KeyCallback(GLFWwindow * window, int key, int keycode, int action, int mods) {
  ImguiIO * io = (ImguiIO *)igGetIO();
  if (globalImguiState->processInputs == 0) {
    io->keysDown[key] = 0;
    io->keyCtrl  = 0;
    io->keyShift = 0;
    io->keyAlt   = 0;
    io->keySuper = 0;
    return;
  }
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
  if (globalImguiState->processInputs == 0) {
    return;
  }
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

  reiiCreateTextureFromTextureMemory(globalImguiState->gpuContext, &globalImguiState->gpuFontAtlasMemory, REII_TEXTURE_BINDING_2D, &globalImguiState->gpuFontAtlas);
  reiiTextureSetStateMipmap(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, &globalImguiState->gpuFontAtlas, 0);
  reiiTextureSetStateMipmapLevelsCount(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, &globalImguiState->gpuFontAtlas, 1);
  memcpy(globalImguiState->gpuFontAtlasScratchBuffer.cpu_scratch_buffer_ptr, data, width * height * bpp);
  reiiTextureDefineAndCopyFromCpu(globalImguiState->gpuContext, REII_TEXTURE_BINDING_2D, &globalImguiState->gpuFontAtlas, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, width, height, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, &globalImguiState->gpuFontAtlasScratchBuffer);

  ImFontAtlas_SetTexID(io->fonts, (void *)(intptr_t)&globalImguiState->gpuFontAtlas);
}

static inline void imguiCreateDeviceObjects() {
  #include "imgui_shader.vs.h"
  #include "imgui_shader.fs.h"
  gpu_program_info_t vp = {0};
  vp.program_binary_bytes_count = sizeof(g_main_vs);
  vp.program_binary             = g_main_vs;
  gpu_program_info_t fp = {0};
  fp.program_binary_bytes_count = sizeof(g_main_fs);
  fp.program_binary             = g_main_fs;
  char * vp_string = NULL;
  char * fp_string = NULL;

  RedStructDeclarationMember slots[3] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[2].slot            = 2;
  slots[2].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
  slots[2].count           = 1;
  slots[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_compile_info = {0};
  mesh_state_compile_info.state_multisample_count     = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  mesh_state_compile_info.output_depth_stencil_enable = 0;
  mesh_state_compile_info.output_depth_stencil_format = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_compile_info.output_color_format         = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_compile_info.variables_slot              = 3;
  mesh_state_compile_info.variables_bytes_count       = 0;
  mesh_state_compile_info.struct_members_count        = _countof(slots);
  mesh_state_compile_info.struct_members              = slots;
  mesh_state_compile_info.samplers_count              = 1;
  globalImguiState->gpuMeshState.compileInfo                                    = mesh_state_compile_info;
  globalImguiState->gpuMeshState.programVertex                                  = vp;
  globalImguiState->gpuMeshState.programFragment                                = fp;
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
  reiiMeshStateCompile(globalImguiState->gpuContext, &globalImguiState->gpuMeshState);

  globalImguiState->gpuSampler = reiiCreateSampler(globalImguiState->gpuContext, NULL, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);

  imguiCreateFontTexture();
}

static inline void imguiNewFrame() {
  ImguiIO * io = (ImguiIO *)igGetIO();

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

  if (globalImguiState->processInputs == 1 && glfwGetWindowAttrib(globalImguiState->window, GLFW_FOCUSED)) {
    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(globalImguiState->window, &mouse_x, &mouse_y);
    io->mousePosition = (ImVec2){(float)mouse_x, (float)mouse_y};
  } else {
    io->mousePosition = (ImVec2){-1.f, -1.f};
  }

  for (int i = 0; i < 3; i += 1) {
    if (globalImguiState->processInputs == 1) {
      io->mouseDown[i] = globalImguiState->mousePressed[i] == 1 || glfwGetMouseButton(globalImguiState->window, i) != 0; // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    } else {
      io->mouseDown[i] = 0;
    }
    globalImguiState->mousePressed[i] = 0;
  }

  io->mouseWheel = globalImguiState->mouseWheel;
  globalImguiState->mouseWheel = 0;

  igNewFrame();
}

static inline void imguiInvalidateFontTexture() {
  ImguiIO * io = (ImguiIO *)igGetIO();
  reiiDestroyEx(globalImguiState->gpuContext, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, &globalImguiState->gpuFontAtlas);
  reiiResetTextureMemory(globalImguiState->gpuContext, &globalImguiState->gpuFontAtlasMemory);
  ImFontAtlas_SetTexID(io->fonts, 0);
}

static inline void imguiInvalidateDeviceObjects() {
  ImguiIO * io = (ImguiIO *)igGetIO();
  imguiInvalidateFontTexture();
  reiiDestroyEx(globalImguiState->gpuContext, GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST, &globalImguiState->gpuCommandList);
  reiiDestroyEx(globalImguiState->gpuContext, GPU_EXTRA_REII_DESTROY_TYPE_SAMPLER, globalImguiState->gpuSampler);
  reiiDestroyEx(globalImguiState->gpuContext, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &globalImguiState->gpuMeshState);
  uint64_t ids[] = {
    globalImguiState->gpuBatch,
  };
  vfIdDestroy(_countof(ids), ids, __FILE__, __LINE__);
}

static inline void imguiDeinit() {
  imguiInvalidateDeviceObjects();
  igShutdown();
  ImguiState defaults = {0};
  globalImguiState[0] = defaults;
}

static inline void imguiInit(
  GLFWwindow *            window,
  gpu_handle_context_t    context,
  ReiiHandleTextureMemory fontAtlasMemory,
  ReiiCpuScratchBuffer    fontAtlasScratchBuffer,
  uint64_t                maxNewBindingsSetsCount,
  gpu_extra_cpu_gpu_array dynamicMeshPosition,
  gpu_extra_cpu_gpu_array dynamicMeshColor,
  uint64_t                mutableOutputsArrayMaxCapacity,
  Red2Output *            mutableOutputsArray,
  ReiiHandleTexture *     outputTexture,
  unsigned                optionalQueueFamilyIndex,
  RedHandleQueue          optionalQueue
)
{
  ImguiIO * io = (ImguiIO *)igGetIO();

  globalImguiState->window                      = window;
  globalImguiState->gpuContext                  = context;
  globalImguiState->gpuFontAtlasMemory          = fontAtlasMemory;
  globalImguiState->gpuFontAtlasScratchBuffer   = fontAtlasScratchBuffer;
  globalImguiState->gpuMaxNewBindingsSetsCount  = maxNewBindingsSetsCount;
  globalImguiState->gpuDynamicMeshPosition      = dynamicMeshPosition;
  globalImguiState->gpuDynamicMeshColor         = dynamicMeshColor;
  globalImguiState->gpuMutableOutputsArray      = mutableOutputsArray;
  globalImguiState->gpuOutputTexture            = outputTexture;
  globalImguiState->gpuOptionalQueueFamilyIndex = optionalQueueFamilyIndex;
  globalImguiState->gpuOptionalQueue            = optionalQueue;

  globalImguiState->gpuCommandList.mutable_outputs_array.items    = globalImguiState->gpuMutableOutputsArray;
  globalImguiState->gpuCommandList.mutable_outputs_array.capacity = mutableOutputsArrayMaxCapacity;
  globalImguiState->gpuCommandList.dynamic_mesh_position          = globalImguiState->gpuDynamicMeshPosition;
  globalImguiState->gpuCommandList.dynamic_mesh_color             = globalImguiState->gpuDynamicMeshColor;

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

  imguiCreateDeviceObjects();
}

void imguiSetProcessInputsState(ReiiBool32 enable) {
  globalImguiState->processInputs = enable;
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
