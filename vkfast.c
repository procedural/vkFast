//#define VKFAST_DEFINE_ENABLE_FEATURE_GPU_DEBUG_PRINTF

#ifdef _WIN32
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "vkfast_ex.h"
#include "vkfast_ids.h"

#ifndef __cplusplus
  #define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#if defined(_WIN32)
  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu_2.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu_wsi.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_context_from_vk.h"
  #else
    #include "C:/RedGpuSDK/misc/np/np.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"
    #include "C:/RedGpuSDK/redgpu_context_from_vk.h"
  #endif
#elif defined(__linux__) && !defined(__ANDROID__)
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_2.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_wsi.h"
  #include "/home/linuxbrew/RedGpuSDK/redgpu_context_from_vk.h"
#else
  #error Unsupported OS for now
#endif

#if defined(VKFAST_DISABLE_WIN32)
  #include <stdio.h>  // For printf
  #include <stdlib.h> // For exit
#else
  #ifdef _WIN32
    #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
      #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_32_termux_windows.h"
    #else
      #include <Windows.h>
    #endif
  #endif
#endif
#if defined(__linux__) && !defined(__ANDROID__)
#include <X11/Xlib.h> // For X11 Display, Window
#endif
#include <string.h> // For strcmp

#if defined(VKFAST_DISABLE_WIN32)
  REDGPU_32_DECLSPEC void REDGPU_32_API red32OutputDebugString(const char * string) {
    // NOTE(Constantine): does nothing.
  }

  REDGPU_32_DECLSPEC void REDGPU_32_API red32ConsolePrint(const char * string) {
    fprintf(stdout, "%s", string);
    fflush(stdout);
  }
#endif

static void vfInternalPrint(const char * string) {
  red32OutputDebugString(string);
  red32ConsolePrint(string);
}

#if defined(__linux__) && !defined(__ANDROID__)
#include <stdio.h>  // For printf
#include <stdlib.h> // For exit

#define MB_OK 0

// Helper function for MessageBoxA to draw multiline text and calculate dimensions
// If draw_text is 0, it only calculates the required height and width
static void MessageBoxA_ProcessText(Display * display, Window win, GC gc, const char * text, int draw_text, int * out_width, int * out_height) {
  const char * start = text;
  const char * end;
  int line_num = 0;
  int max_width = 250;

  int start_y = 35; // Margins from top margin
  int line_spacing = 18; // Vertical distance between text lines

  while ((end = strchr(start, '\n')) != NULL) {
    int len = end - start;
    int width = len * 7 + 40; // Approximate width mapping
    if (width > max_width) max_width = width;

    if (draw_text) {
      XDrawString(display, win, gc, 20, start_y + (line_num * line_spacing), start, len);
    }
    line_num++;
    start = end + 1;
  }

  // Process trailing line (or the only line if no newlines exist)
  int len = strlen(start);
  int width = len * 7 + 40;
  if (width > max_width) max_width = width;

  if (draw_text) {
    XDrawString(display, win, gc, 20, start_y + (line_num * line_spacing), start, len);
  }
  line_num++;

  *out_width = max_width;
  *out_height = start_y + (line_num * line_spacing) + 70; // Text block height + space for button
}

static void MessageBoxA(void * hWnd, const char * lpText, const char * lpCaption, unsigned int uType) {
  Display * display = XOpenDisplay(NULL);
  if (display == NULL) {
    printf("MessageBoxA error: XOpenDisplay(NULL) returned NULL.\n");
    exit(1);
  }

  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);

  // 1. Pre-calculate dynamic layout dimensions based on text contents
  int win_width = 0;
  int win_height = 0;
  MessageBoxA_ProcessText(display, root, NULL, lpText, 0, &win_width, &win_height);

  unsigned long white = WhitePixel(display, screen);
  unsigned long black = BlackPixel(display, screen);

  // 2. Main Dialog Canvas Setup
  Window win = XCreateSimpleWindow(display, root, 100, 100, win_width, win_height, 1, black, white);
  XStoreName(display, win, lpCaption);

  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, win, &wm_delete, 1);

  // 3. OK Action Button Layout positioning
  int btn_w = 60, btn_h = 25;
  int btn_x = (win_width - btn_w) / 2;
  int btn_y = win_height - 45; // Fixed padding from window base
  Window btn = XCreateSimpleWindow(display, win, btn_x, btn_y, btn_w, btn_h, 1, black, white);

  XSelectInput(display, win, ExposureMask | KeyPressMask);
  XSelectInput(display, btn, ExposureMask | ButtonPressMask);

  XMapWindow(display, win);
  XMapWindow(display, btn);

  GC gc = XCreateGC(display, win, 0, NULL);
  XSetForeground(display, gc, black);
  XSetBackground(display, gc, white);

  // 4. Modal Event Loop Interface
  XEvent event;
  int loop = 1;

  while (loop) {
    XNextEvent(display, &event);

    if (event.type == Expose) {
      // Re-render UI segments on damage exposure signals
      if (event.xexpose.window == win) {
        int dummy_w, dummy_h;
        MessageBoxA_ProcessText(display, win, gc, lpText, 1, &dummy_w, &dummy_h);
      } else if (event.xexpose.window == btn) {
        XDrawString(display, btn, gc, 22, 17, "OK", 2);
      }
    }
    else if (event.type == ButtonPress) {
      if (event.xbutton.window == btn) {
        loop = 0;
      }
    }
    else if (event.type == ClientMessage) {
      if ((Atom)event.xclient.data.l == wm_delete) {
        loop = 0;
      }
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, win);
  XCloseDisplay(display);
}
#endif

void red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine) {
  struct StringArray {
    char * items;
    size_t count;
    size_t capacity;
    size_t alignment;
  };

  // To free
  struct StringArray str = {0};

  char optionalLineStr[4096] = {0};
  red32IntToChars(optionalLine, optionalLineStr);

  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "[vkFast][Crash][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalFile);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, ":");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, functionName);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "] ");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, error);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "\n");

  vfInternalPrint(str.items);
  #if !defined(VKFAST_DISABLE_WIN32)
  MessageBoxA(NULL, str.items, "[vkFast][Crash]", MB_OK);
  #endif

  REDGPU_32_DYNAMIC_ARRAY_FREE(str);

  #if !defined(VKFAST_DISABLE_WIN32)
  red32Exit(1);
  #else
  system("pause");
  exit(1);
  #endif
}

static void vfFillMemoryTypeIsSupportedArray(unsigned resourceMemoryTypesSupported, unsigned char * outMemoryTypeIsSupportedArrayOf32) {
  outMemoryTypeIsSupportedArrayOf32[0]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[1]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[2]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[3]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[4]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[5]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[6]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[7]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[8]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[9]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[10] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[11] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[12] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[13] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[14] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[15] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[16] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[17] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[18] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[19] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[20] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[21] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[22] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[23] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[24] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[25] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[26] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[27] = (resourceMemoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[28] = (resourceMemoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[29] = (resourceMemoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[30] = (resourceMemoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[31] = (resourceMemoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
}

static unsigned vfPickSpecificMemoryTypeGpuVram(const RedGpuInfo * gpuInfo, const RedArray * array) {
  unsigned memoryTypesSupported = array->memoryTypesSupported;

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  vfFillMemoryTypeIsSupportedArray(memoryTypesSupported, arrayMemoryTypeIsSupported);

  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    const RedMemoryType * type = &gpuInfo->memoryTypes[i];
    if (arrayMemoryTypeIsSupported[i] == 1 &&
        type->isGpuVram     == 1 &&
        type->isCpuMappable == 0 &&
        type->isCpuCoherent == 0 &&
        type->isCpuCached   == 0 &&
        gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == 1 &&
        gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
    {
      return i;
    }
  }
  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    const RedMemoryType * type = &gpuInfo->memoryTypes[i];
    if (arrayMemoryTypeIsSupported[i] == 1 &&
        type->isGpuVram == 1 &&
        gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == 1 &&
        gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
    {
      return i;
    }
  }

  return -1;
}

static unsigned vfPickSpecificMemoryTypeCpuUpload(const RedGpuInfo * gpuInfo, const RedArray * array) {
  unsigned memoryTypesSupported = array->memoryTypesSupported;

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  vfFillMemoryTypeIsSupportedArray(memoryTypesSupported, arrayMemoryTypeIsSupported);

  for (int isGpuVram = 0; isGpuVram < 2; isGpuVram += 1) { // NOTE(Constantine): First, we look for non-VRAM heaps, then can fall back to VRAM heaps.
    for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
      const RedMemoryType * type = &gpuInfo->memoryTypes[i];
      if (arrayMemoryTypeIsSupported[i] == 1 &&
          type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 0 &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == isGpuVram &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
      const RedMemoryType * type = &gpuInfo->memoryTypes[i];
      if (arrayMemoryTypeIsSupported[i] == 1 &&
          type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == isGpuVram &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
      {
        return i;
      }
    }
  }

  return -1;
}

static unsigned vfPickSpecificMemoryTypeCpuReadback(const RedGpuInfo * gpuInfo, const RedArray * array) {
  unsigned memoryTypesSupported = array->memoryTypesSupported;

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  vfFillMemoryTypeIsSupportedArray(memoryTypesSupported, arrayMemoryTypeIsSupported);

  for (int isGpuVram = 0; isGpuVram < 2; isGpuVram += 1) { // NOTE(Constantine): First, we look for non-VRAM heaps, then can fall back to VRAM heaps.
    for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
      const RedMemoryType * type = &gpuInfo->memoryTypes[i];
      if (arrayMemoryTypeIsSupported[i] == 1 &&
          type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 1 &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == isGpuVram &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
      const RedMemoryType * type = &gpuInfo->memoryTypes[i];
      if (arrayMemoryTypeIsSupported[i] == 1 &&
          type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == isGpuVram &&
          gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0)
      {
        return i;
      }
    }
  }

  return -1;
}

static RedBool32 vfRedGpuDebugCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  if (0 == strcmp(data->messageIdName, "VUID-VkDebugUtilsMessengerCallbackDataEXT-flags-zerobitmask")) {
    return 0;
  }
  if (0 == strcmp(data->messageIdName, "RED_PROCEDURE_ID_redQueuePresent") &&
      0 == strcmp(data->message,       "RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE"))
  {
    return 0;
  }
  if (0 == strcmp(data->messageIdName, "RED_PROCEDURE_ID_redPresentGetImageIndex") &&
      0 == strcmp(data->message,       "RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE"))
  {
    return 0;
  }
  if (0 == strcmp(data->messageIdName, "RED_PROCEDURE_ID_redCreateContext") &&
      0 == strcmp(data->message,       "RED_STATUS_ERROR_INCOMPATIBLE_DRIVER"))
  {
    return 0; // NOTE(Constantine): Android can only set 4 GPU structs max.
  }
  vfInternalPrint("[vkFast][Debug callback] ");
  vfInternalPrint(data->message);
  #if !defined(VKFAST_DISABLE_WIN32)
  #ifndef VKFAST_DEFINE_ENABLE_FEATURE_GPU_DEBUG_PRINTF
  MessageBoxA(NULL, data->message, "[vkFast][Debug callback]", MB_OK);
  red32Exit(1);
  #endif
  #else
  vfInternalPrint("\n");
  system("pause");
  exit(1);
  #endif
  return 0;
}

static void vfInternalExpectMinimumGuaranteesAdreno735(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateCount                      >= REDGPU_2_EXPECTED_maxMemoryAllocateCount_4096); // maxMemoryAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryAllocateBytesAlignment             <= REDGPU_2_EXPECTED_minMemoryAllocateBytesAlignment_4096); // minMemoryMapAlignment; RX 550 and Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateBytesCount                 >= REDGPU_2_EXPECTED_maxMemoryAllocateBytesCount_1073741824); // maxMemoryAllocationSize; Redmi Note 8 on Android 11; Not used: RX 550 and RX 9070 XT on Windows is 2147483648
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryNonCoherentBlockBytesCount         <= REDGPU_2_EXPECTED_minMemoryNonCoherentBlockBytesCount_256); // nonCoherentAtomSize
  REDGPU_2_EXPECTWG(gpuInfo->maxCreateSamplerCount                       >= REDGPU_2_EXPECTED_maxCreateSamplerCount_4000); // maxSamplerAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryPageSeparationArrayImageBytesCount <= REDGPU_2_EXPECTED_minMemoryPageSeparationArrayImageBytesCount_65536); // bufferImageGranularity; D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT (65536)
  REDGPU_2_EXPECTWG(gpuInfo->minArrayROCStructMemberRangeBytesAlignment  <= REDGPU_2_EXPECTED_minArrayROCStructMemberRangeBytesAlignment_256); // minUniformBufferOffsetAlignment; D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT (256)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayROCStructMemberRangeBytesCount      >= REDGPU_2_EXPECTED_maxArrayROCStructMemberRangeBytesCount_65536); // maxUniformBufferRange; D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096) * 16
  REDGPU_2_EXPECTWG(gpuInfo->minArrayRORWStructMemberRangeBytesAlignment <= REDGPU_2_EXPECTED_minArrayRORWStructMemberRangeBytesAlignment_64); // minStorageBufferOffsetAlignment; Arc A770 and Arc B580 on Windows, Redmi Note 8 on Android 11; Not used: D3D12_RAW_UAV_SRV_BYTE_ALIGNMENT (16)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayRORWStructMemberRangeBytesCount     >= 134217728);
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayIndexUint32Value                    == 0xFFFFFFFF); // maxDrawIndexedIndexValue
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions1D                        >= REDGPU_2_EXPECTED_maxImageDimensions1D_16384); // maxImageDimension1D; D3D12_REQ_TEXTURE1D_U_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions2D                        >= REDGPU_2_EXPECTED_maxImageDimensions2D_16384); // maxImageDimension2D; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions3D                        >= REDGPU_2_EXPECTED_maxImageDimensions3D_2048); // maxImageDimension3D; D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensionsCube                      >= REDGPU_2_EXPECTED_maxImageDimensionsCube_16384); // maxImageDimensionCube; D3D12_REQ_TEXTURECUBE_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageLayersCount                         >= REDGPU_2_EXPECTED_maxImageLayersCount_2048); // maxImageArrayLayers; D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerMipLodBias                        >= REDGPU_2_EXPECTED_maxSamplerMipLodBias_15); // maxSamplerLodBias; RTX 2060; Not used: D3D12_MIP_LOD_BIAS_MAX (15.99f)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerAnisotropy                        >= REDGPU_2_EXPECTED_maxSamplerAnisotropy_16); // maxSamplerAnisotropy; D3D12_MAX_MAXANISOTROPY (16)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMagMin         >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMagMin_8); // subTexelPrecisionBits; D3D12_SUBTEXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMip            >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMip_8); // mipmapPrecisionBits; D3D12_MIP_LOD_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputWidth                              >= REDGPU_2_EXPECTED_maxOutputWidth_16384); // maxFramebufferWidth; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputHeight                             >= REDGPU_2_EXPECTED_maxOutputHeight_16384); // maxFramebufferHeight; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputColorsCount                        >= REDGPU_2_EXPECTED_maxOutputColorsCount_8); // maxColorAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputDepth    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputStencil  >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputColor    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForEmptyOutput    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSlotsCount                         >= REDGPU_2_EXPECTED_maxStructSlotsCount_1024); // maxPerSetDescriptors; Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayROCsCount             >= REDGPU_2_EXPECTED_maxStructPerStageArrayROCsCount_12); // maxPerStageDescriptorUniformBuffers; Not used: Tegra X1 on Linux is 15
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayRORWsCount            >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageSamplersCount              >= 64); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureROsCount            >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureRWsCount            >= 16); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageAllResourcesCount          >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayROCsCount                     >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayRORWsCount                    >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSamplersCount                      >= 576); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureROsCount                    >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureRWsCount                    >= 144); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxProcedureParametersVariablesBytesCount   >= REDGPU_2_EXPECTED_maxProcedureParametersVariablesBytesCount_128); // maxPushConstantsSize; Arc A770 on Linux, RX 550 on Windows, Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[0]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsX_16384); // maxViewportDimensions[0]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[1]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsY_16384); // maxViewportDimensions[1]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->minViewportBoundsRange                      <= REDGPU_2_EXPECTED_minViewportBoundsRange_minus_32768); // viewportBoundsRange[0]; D3D12_VIEWPORT_BOUNDS_MIN (-32768)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportBoundsRange                      >= REDGPU_2_EXPECTED_maxViewportBoundsRange_32767); // viewportBoundsRange[1]; D3D12_VIEWPORT_BOUNDS_MAX (32767)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportCoordinates            >= REDGPU_2_EXPECTED_precisionBitsViewportCoordinates_8); // subPixelPrecisionBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportBounds                 >= REDGPU_2_EXPECTED_precisionBitsViewportBounds_8); // viewportSubPixelBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipDistances                    >= REDGPU_2_EXPECTED_maxPerStageClipDistances_8); // maxClipDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageCullDistances                    >= REDGPU_2_EXPECTED_maxPerStageCullDistances_8); // maxCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipAndCullDistances             >= REDGPU_2_EXPECTED_maxPerStageClipAndCullDistances_8); // maxCombinedClipAndCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxVertexOutputLocationsCount               >= REDGPU_2_EXPECTED_maxVertexOutputLocationsCount_128); // maxVertexOutputComponents; D3D12_VS_INPUT_REGISTER_COUNT (32) * D3D12_VS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentInputLocationsCount              >= REDGPU_2_EXPECTED_maxFragmentInputLocationsCount_112); // maxFragmentInputComponents; Redmi Note 8 on Android 11; Not used: Arc A770 and Arc B580 on Linux is 116; Not used: D3D12_PS_INPUT_REGISTER_COUNT (32) * D3D12_PS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCount                >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCount_8); // maxFragmentOutputAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCountBlendDualSource >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCountBlendDualSource_1); // maxFragmentDualSrcAttachments
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputResourcesCount             >= REDGPU_2_EXPECTED_maxFragmentOutputResourcesCount_16); // maxFragmentCombinedOutputResources; Tegra X1 on Linux, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8) + D3D12_PS_CS_UAV_REGISTER_COUNT (8); Not used: Redmi Note 8 on Android 11 is 72
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeSharedMemoryBytesCount            >= REDGPU_2_EXPECTED_maxComputeSharedMemoryBytesCount_16384); // maxComputeSharedMemorySize; Redmi Note 8 on Android 11, D3D12_CS_THREAD_LOCAL_TEMP_REGISTER_POOL (16384); Not used: Arc A770 and Arc B580 and RX 9070 XT on Windows is 32768
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[0]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountX_65535); // maxComputeWorkGroupCount[0]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[1]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountY_65535); // maxComputeWorkGroupCount[1]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[2]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountZ_65535); // maxComputeWorkGroupCount[2]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupInvocationsCount         >= REDGPU_2_EXPECTED_maxComputeWorkgroupInvocationsCount_1024); // maxComputeWorkGroupInvocations; D3D12_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[0]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsX_1024); // maxComputeWorkGroupSize[0]; D3D12_CS_THREAD_GROUP_MAX_X (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[1]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsY_1024); // maxComputeWorkGroupSize[1]; D3D12_CS_THREAD_GROUP_MAX_Y (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[2]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsZ_64); // maxComputeWorkGroupSize[2]; D3D12_CS_THREAD_GROUP_MAX_Z (64)
  REDGPU_2_EXPECTWG(gpuInfo->minImageSampleImageFetchOffset              <= REDGPU_2_EXPECTED_minImageSampleImageFetchOffset_minus_8); // minTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_NEGATIVE (-8)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageSampleImageFetchOffset              >= REDGPU_2_EXPECTED_maxImageSampleImageFetchOffset_7); // maxTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_POSITIVE (7)
  REDGPU_2_EXPECTWG(gpuInfo->minImageGatherOffset                        <= REDGPU_2_EXPECTED_minImageGatherOffset_minus_32); // minTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxImageGatherOffset                        >= REDGPU_2_EXPECTED_maxImageGatherOffset_31); // maxTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->minInterpolateAtOffset                      <= -0.5f); // minInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxInterpolateAtOffset                      >= 0.4375f); // maxInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsInterpolateAtOffset            >= 4); // subPixelInterpolationOffsetBits
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment           <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment_128); // optimalBufferCopyOffsetAlignment
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment_128); // optimalBufferCopyRowPitchAlignment
  REDGPU_2_EXPECTWG(gpuInfo->supportsWsi                                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMemoryGetBudget                                           >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsFullArrayIndexUint32Value                                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsSamplerAnisotropy                                         == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsTextureDimensionsCubeLayered                              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthClamp                     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasDynamic               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasClamp                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleSampleShading                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleAlphaToOne                       >= 0); // Not supported on RX 550 and RX 9070 XT on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTest                         >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTestDynamic                  >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask   == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateBlendLogicOp                                >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendVaryingPerColor            == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendDualSource                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleEmptyOutputVariableMultisampleCount            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleStandardSampleLocations                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageVertex                      == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageFragment                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeImageGatherExtended                                == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWExtendedFormats                           == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWMultisample                               >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWReadWithoutFormat                         >= 0); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWWriteWithoutFormat                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfSamplers               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureROs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeClipDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeCullDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeInt64                                              >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeFloat64                                            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeMinLod                                             >= 0); // Not supported on Redmi Note 8 on Android 11
}

static void vfInternalExpectMinimumImageFormatsLimitsAndFeaturesAdreno735(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS) {
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS) {
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS) {
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS) {
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status >= RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputColorBlend >= 0);
}

static gpu_handle_context_t vfInternalContextInit(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const gpu_context_ex3_parameters_t * optional_ex3_parameters, const char * optionalFile, int optionalLine) {
  if (enable_debug_mode) {
    vfInternalPrint("[vkFast][Debug] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
  }

  void * optional_pointer_to_custom_vf_handle_context = 0;

  if (optional_parameters != NULL) {
    optional_pointer_to_custom_vf_handle_context = optional_parameters->optional_pointer_to_custom_vf_handle_context;
  }

  vf_handle_context_t * vkfast = (vf_handle_context_t *)optional_pointer_to_custom_vf_handle_context;
  if (vkfast == NULL) {
    // To free
    vkfast = (vf_handle_context_t *)red32MemoryCalloc(sizeof(vf_handle_context_t));
    REDGPU_2_EXPECT(vkfast != NULL);
    vkfast->gpuIndex = gpu_index;
  }

  uint64_t internalMemoryAllocationSizeGpuVramArrays           = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_ONLY_512MB;
  uint64_t internalMemoryAllocationSizeCpuVisible              = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_UPLOAD_512MB;
  uint64_t internalMemoryAllocationSizeCpuReadback             = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB;
  uint64_t internalMemoryAllocationSizeCpuVisiblePresentPixels = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_PIXELS_CPU_UPLOAD_288MB;
  if (optional_parameters != NULL) {
    if (optional_parameters->internal_memory_allocation_sizes != NULL) {
      internalMemoryAllocationSizeGpuVramArrays           = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_gpu_only;
      internalMemoryAllocationSizeCpuVisible              = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_cpu_upload;
      internalMemoryAllocationSizeCpuReadback             = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_cpu_readback;
      internalMemoryAllocationSizeCpuVisiblePresentPixels = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_present_pixels_type_cpu_upload;
    }
  }

  RedContext context = vkfast->context;
  if (context == NULL) {
    RedContextOptionalSettingsContextFromVk            contextFromVk            = {0};
    RedContextOptionalSettings0                        optionalSettings0        = {0};
    RedContextOptionalSettingsCreateContextPerformance createContextPerformance = {0};
    void * optionalSettings = NULL;
    if (optional_ex2_parameters != NULL) {
      contextFromVk.settings             = RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK;
      contextFromVk.next                 = NULL;
      contextFromVk.instance             = optional_ex2_parameters->external_VkInstance;
      contextFromVk.physicalDevicesCount = 1;
      contextFromVk.physicalDevices      = (uint64_t *)&optional_ex2_parameters->external_VkPhysicalDevice;
      contextFromVk.devices              = (uint64_t *)&optional_ex2_parameters->external_VkDevice;

      optionalSettings0.settings                      = RED_CONTEXT_OPTIONAL_SETTINGS_0;
      optionalSettings0.next                          = optional_ex2_parameters->external_VkInstance == 0 ? NULL : &contextFromVk;
      optionalSettings0.skipCheckingContextLayers     = 0;
      optionalSettings0.skipCheckingContextExtensions = 0;
      optionalSettings0.gpusExposeOnlyOneQueue        = optional_ex2_parameters->exposeOnlyOneQueue;

      createContextPerformance.settings         = RED_CONTEXT_OPTIONAL_SETTINGS_CREATE_CONTEXT_PERFORMANCE;
      createContextPerformance.next             = &optionalSettings0;
      createContextPerformance.exposeOnlyOneGpu = optional_ex2_parameters->exposeOnlyOneGpu;

      optionalSettings = &createContextPerformance;
    }
    #if defined(_WIN32)
    unsigned extensions[] = {
      RED_SDK_EXTENSION_WSI_WIN32,
      #ifdef VKFAST_DEFINE_ENABLE_FEATURE_GPU_DEBUG_PRINTF
      RED_SDK_EXTENSION_RAY_TRACING,
      #endif
    };
    #elif defined(__linux__) && !defined(__ANDROID__)
    unsigned extensions[] = {
      RED_SDK_EXTENSION_WSI_XLIB,
      #ifdef VKFAST_DEFINE_ENABLE_FEATURE_GPU_DEBUG_PRINTF
      RED_SDK_EXTENSION_RAY_TRACING,
      #endif
    };
    #endif
    np(redCreateContext,
      "malloc", red32MemoryCalloc,
      "free", red32MemoryFree,
      "optionalMallocTagged", NULL,
      "optionalFreeTagged", NULL,
      "debugCallback", enable_debug_mode == 1 ? vfRedGpuDebugCallback : NULL,
      "sdkVersion", RED_SDK_VERSION_1_0_135,
      "sdkExtensionsCount", sizeof(extensions) / sizeof(extensions[0]),
      "sdkExtensions", extensions,
      "optionalProgramName", NULL,
      "optionalProgramVersion", 0,
      "optionalEngineName", NULL,
      "optionalEngineVersion", 0,
      "optionalSettings", optionalSettings,
      "outContext", &context,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  REDGPU_2_EXPECT(context != NULL);
  REDGPU_2_EXPECT(context->gpusCount > 0);
  REDGPU_2_EXPECT(vkfast->gpuIndex < context->gpusCount);

  const RedGpuInfo * gpuInfo = &context->gpus[vkfast->gpuIndex]; // NOTE(Constantine): Picking the first available GPU by default.

  if (enable_debug_mode == 1) {
    vfInternalPrint("[vkFast][Debug] Your GPU name: ");
    vfInternalPrint(gpuInfo->gpuName);
    vfInternalPrint("\n");
    vfInternalPrint("[vkFast][Debug] For extra debug information, recompile redgpu.c as redgpu.cpp with REDGPU_COMPILE_SWITCH 3" "\n");
  }

  REDGPU_2_EXPECT(gpuInfo->queuesCount > 0);

  if (enable_debug_mode == 1) {
    if (gpuInfo->supportsMemoryGetBudget == 1) {
      RedMemoryBudget memoryBudget = {0};
      memoryBudget.setTo1000237000 = 1000237000;
      memoryBudget.setTo0          = 0;
      np(redMemoryGetBudget,
        "context", context,
        "gpu", gpuInfo->gpu,
        "outMemoryBudget", &memoryBudget,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      char numberString[4096] = {0};

      vfInternalPrint("[vkFast][Debug] gpuInfo->memoryHeaps current process heap budget:" "\n");
      for (unsigned i = 0; i < gpuInfo->memoryHeapsCount; i += 1) {
        vfInternalPrint("[vkFast][Debug]   [");
        red32Uint64ToChars(i, numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("]: ");
        red32Uint64ToChars(memoryBudget.memoryHeapsBudget[i], numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("\n");
      }

      vfInternalPrint("[vkFast][Debug] gpuInfo->memoryHeaps current process estimated heap usage:" "\n");
      for (unsigned i = 0; i < gpuInfo->memoryHeapsCount; i += 1) {
        vfInternalPrint("[vkFast][Debug]   [");
        red32Uint64ToChars(i, numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("]: ");
        red32Uint64ToChars(memoryBudget.memoryHeapsUsage[i], numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("\n");
      }
    }
  }

  if (gpuInfo->gpuVendorId == 0x5143) {
    vfInternalExpectMinimumGuaranteesAdreno735(gpuInfo, optionalFile, optionalLine);
  } else {
    np(red2ExpectMinimumGuaranteesIntelUHDGraphics730,
      "gpuInfo", gpuInfo,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine
    );
  }

  if (gpuInfo->gpuVendorId == 0x5143) {
    vfInternalExpectMinimumImageFormatsLimitsAndFeaturesAdreno735(gpuInfo, optionalFile, optionalLine);
  } else {
    np(red2ExpectMinimumImageFormatsLimitsAndFeatures,
      "gpuInfo", gpuInfo,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine
    );
  }

  RedHandleGpu   gpu       = gpuInfo->gpu;
  RedHandleQueue mainQueue = gpuInfo->queues[vkfast->mainQueueIndex];

  RedArray memoryGpuVramForArrays_array   = {0};
  RedArray memoryCpuUpload_array          = {0};
  RedArray memoryCpuReadback_array        = {0};
  unsigned specificMemoryTypesGpuVram     = -1;
  unsigned specificMemoryTypesCpuUpload   = -1;
  unsigned specificMemoryTypesCpuReadback = -1;
  RedHandleMemory memoryGpuVramForArrays_memory = NULL;
  RedHandleMemory memoryCpuUpload_memory        = NULL;
  RedHandleMemory memoryCpuReadback_memory      = NULL;
  void * memoryCpuUpload_mapped_void_ptr        = NULL;
  void * memoryCpuReadback_mapped_void_ptr      = NULL;
  {
    if (internalMemoryAllocationSizeGpuVramArrays > 0) {
      uint64_t bytesCount = internalMemoryAllocationSizeGpuVramArrays;
      const char * handleName = "vkFast_vfInternalContextInit_memoryGpuVramForArrays_array";
      uint64_t maxAllowedOverallocationBytesCount = 0;
      RedArray array = {0};

      np(redCreateArray,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "type", RED_ARRAY_TYPE_ARRAY_RW,
        "bytesCount", bytesCount,
        "structuredBufferElementBytesCount", 0,
        "restrictToAccess", RED_ARRAY_TYPE_ARRAY_RW,
        "initialQueueFamilyIndex", gpuInfo->queuesCount > 1 ? -1 : (unsigned)gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
        "dedicate", 0,
        "outArray", &array,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(array.handle != NULL);
      size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
      REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
      REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);

      memoryGpuVramForArrays_array = array;
    }
    if (internalMemoryAllocationSizeCpuVisible > 0) {
      uint64_t bytesCount = internalMemoryAllocationSizeCpuVisible;
      const char * handleName = "vkFast_vfInternalContextInit_memoryCpuUpload_array";
      uint64_t maxAllowedOverallocationBytesCount = 0;
      RedArray array = {0};

      np(redCreateArray,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "type", RED_ARRAY_TYPE_ARRAY_RO,
        "bytesCount", bytesCount,
        "structuredBufferElementBytesCount", 0,
        "restrictToAccess", RED_ACCESS_BITFLAG_COPY_R,
        "initialQueueFamilyIndex", gpuInfo->queuesCount > 1 ? -1 : (unsigned)gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
        "dedicate", 0,
        "outArray", &array,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(array.handle != NULL);
      size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
      REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
      REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);

      memoryCpuUpload_array = array;
    }
    if (internalMemoryAllocationSizeCpuReadback > 0) {
      uint64_t bytesCount = internalMemoryAllocationSizeCpuReadback;
      const char * handleName = "vkFast_vfInternalContextInit_memoryCpuReadback_array";
      uint64_t maxAllowedOverallocationBytesCount = 0;
      RedArray array = {0};

      np(redCreateArray,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "type", RED_ARRAY_TYPE_ARRAY_RW,
        "bytesCount", bytesCount,
        "structuredBufferElementBytesCount", 0,
        "restrictToAccess", RED_ACCESS_BITFLAG_COPY_W,
        "initialQueueFamilyIndex", gpuInfo->queuesCount > 1 ? -1 : (unsigned)gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
        "dedicate", 0,
        "outArray", &array,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(array.handle != NULL);
      size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
      REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
      REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);

      memoryCpuReadback_array = array;
    }

    int userSpecificMemoryTypeGpuVram     = 0;
    int userSpecificMemoryTypeCpuUpload   = 0;
    int userSpecificMemoryTypeCpuReadback = 0;
    if (optional_ex3_parameters != NULL) {
      if (optional_ex3_parameters->optionalSpecificMemoryTypeGpuVram     != NULL) { userSpecificMemoryTypeGpuVram     = 1; }
      if (optional_ex3_parameters->optionalSpecificMemoryTypeCpuUpload   != NULL) { userSpecificMemoryTypeCpuUpload   = 1; }
      if (optional_ex3_parameters->optionalSpecificMemoryTypeCpuReadback != NULL) { userSpecificMemoryTypeCpuReadback = 1; }
    }
    if (userSpecificMemoryTypeGpuVram == 1) {
      specificMemoryTypesGpuVram = optional_ex3_parameters->optionalSpecificMemoryTypeGpuVram[0];
    } else {
      specificMemoryTypesGpuVram = vfPickSpecificMemoryTypeGpuVram(gpuInfo, &memoryGpuVramForArrays_array);
    }
    if (userSpecificMemoryTypeCpuUpload == 1) {
      specificMemoryTypesCpuUpload = optional_ex3_parameters->optionalSpecificMemoryTypeCpuUpload[0];
    } else {
      specificMemoryTypesCpuUpload = vfPickSpecificMemoryTypeCpuUpload(gpuInfo, &memoryCpuUpload_array);
    }
    if (userSpecificMemoryTypeCpuReadback == 1) {
      specificMemoryTypesCpuReadback = optional_ex3_parameters->optionalSpecificMemoryTypeCpuReadback[0];
    } else {
      specificMemoryTypesCpuReadback = vfPickSpecificMemoryTypeCpuReadback(gpuInfo, &memoryCpuReadback_array);
    }
    if (internalMemoryAllocationSizeGpuVramArrays > 0) { REDGPU_2_EXPECTWG(specificMemoryTypesGpuVram     != -1); }
    if (internalMemoryAllocationSizeCpuVisible    > 0) { REDGPU_2_EXPECTWG(specificMemoryTypesCpuUpload   != -1); }
    if (internalMemoryAllocationSizeCpuReadback   > 0) { REDGPU_2_EXPECTWG(specificMemoryTypesCpuReadback != -1); }

    if (internalMemoryAllocationSizeGpuVramArrays > 0) {
      np(redMemoryAllocate,
        "context", context,
        "gpu", gpu,
        "handleName", "vkFast_vfInternalContextInit_memoryGpuVramForArrays_memory",
        "bytesCount", internalMemoryAllocationSizeGpuVramArrays,
        "memoryTypeIndex", specificMemoryTypesGpuVram,
        "dedicateToArray", NULL,
        "dedicateToImage", NULL,
        "memoryBitflags", 0,
        "outMemory", &memoryGpuVramForArrays_memory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(memoryGpuVramForArrays_memory != NULL);

      RedMemoryArray memoryArray = {0};
      memoryArray.setTo1000157000  = 1000157000;
      memoryArray.setTo0           = 0;
      memoryArray.array            = memoryGpuVramForArrays_array.handle;
      memoryArray.memory           = memoryGpuVramForArrays_memory;
      memoryArray.memoryBytesFirst = 0;
      RedStatuses opstatuses = {0};
      np(redMemorySet,
        "context", context,
        "gpu", gpu,
        "memoryArraysCount", 1,
        "memoryArrays", &memoryArray,
        "memoryImagesCount", 0,
        "memoryImages", NULL,
        "outStatuses", &opstatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(opstatuses.statusError == RED_STATUS_SUCCESS);
    }
    if (internalMemoryAllocationSizeCpuVisible > 0) {
      np(redMemoryAllocate,
        "context", context,
        "gpu", gpu,
        "handleName", "vkFast_vfInternalContextInit_memoryCpuUpload_memory",
        "bytesCount", internalMemoryAllocationSizeCpuVisible,
        "memoryTypeIndex", specificMemoryTypesCpuUpload,
        "dedicateToArray", NULL,
        "dedicateToImage", NULL,
        "memoryBitflags", 0,
        "outMemory", &memoryCpuUpload_memory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(memoryCpuUpload_memory != NULL);

      RedMemoryArray memoryArray = {0};
      memoryArray.setTo1000157000  = 1000157000;
      memoryArray.setTo0           = 0;
      memoryArray.array            = memoryCpuUpload_array.handle;
      memoryArray.memory           = memoryCpuUpload_memory;
      memoryArray.memoryBytesFirst = 0;
      RedStatuses opstatuses = {0};
      np(redMemorySet,
        "context", context,
        "gpu", gpu,
        "memoryArraysCount", 1,
        "memoryArrays", &memoryArray,
        "memoryImagesCount", 0,
        "memoryImages", NULL,
        "outStatuses", &opstatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(opstatuses.statusError == RED_STATUS_SUCCESS);

      np(redMemoryMap,
        "context", context,
        "gpu", gpu,
        "mappableMemory", memoryCpuUpload_memory,
        "mappableMemoryBytesFirst", 0,
        "mappableMemoryBytesCount", memoryCpuUpload_array.memoryBytesCount,
        "outVolatilePointer", &memoryCpuUpload_mapped_void_ptr,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(memoryCpuUpload_mapped_void_ptr != NULL);
      REDGPU_2_EXPECTWG(!"Start address is not aligned" || (0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)memoryCpuUpload_mapped_void_ptr, gpuInfo->minMemoryAllocateBytesAlignment)));
    }
    if (internalMemoryAllocationSizeCpuReadback > 0) {
      np(redMemoryAllocate,
        "context", context,
        "gpu", gpu,
        "handleName", "vkFast_vfInternalContextInit_memoryCpuReadback_memory",
        "bytesCount", internalMemoryAllocationSizeCpuReadback,
        "memoryTypeIndex", specificMemoryTypesCpuReadback,
        "dedicateToArray", NULL,
        "dedicateToImage", NULL,
        "memoryBitflags", 0,
        "outMemory", &memoryCpuReadback_memory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(memoryCpuReadback_memory != NULL);

      RedMemoryArray memoryArray = {0};
      memoryArray.setTo1000157000  = 1000157000;
      memoryArray.setTo0           = 0;
      memoryArray.array            = memoryCpuReadback_array.handle;
      memoryArray.memory           = memoryCpuReadback_memory;
      memoryArray.memoryBytesFirst = 0;
      RedStatuses opstatuses = {0};
      np(redMemorySet,
        "context", context,
        "gpu", gpu,
        "memoryArraysCount", 1,
        "memoryArrays", &memoryArray,
        "memoryImagesCount", 0,
        "memoryImages", NULL,
        "outStatuses", &opstatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(opstatuses.statusError == RED_STATUS_SUCCESS);

      np(redMemoryMap,
        "context", context,
        "gpu", gpu,
        "mappableMemory", memoryCpuReadback_memory,
        "mappableMemoryBytesFirst", 0,
        "mappableMemoryBytesCount", memoryCpuReadback_array.memoryBytesCount,
        "outVolatilePointer", &memoryCpuReadback_mapped_void_ptr,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(memoryCpuReadback_mapped_void_ptr != NULL);
      REDGPU_2_EXPECTWG(!"Start address is not aligned" || (0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)memoryCpuReadback_mapped_void_ptr, gpuInfo->minMemoryAllocateBytesAlignment)));
    }
  }

  RedCalls presentCopyCalls = {0};
  np(redCreateCalls,
    "context", context,
    "gpu", gpu,
    "handleName", "vkFast_vfInternalContextInit_presentCopyCalls",
    "queueFamilyIndex", gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
    "outCalls", &presentCopyCalls,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(presentCopyCalls.handle != NULL);

  REDGPU_2_EXPECTWG(gpuInfo->minArrayRORWStructMemberRangeBytesAlignment <= 64);

  // Filling
  vf_handle_context_t;
  vkfast->doNotDestroyRawContext;
  vkfast->doNotFreeHandle;
  vkfast->isDebugMode = enable_debug_mode;
  vkfast->context = context;
  vkfast->gpuInfo = gpuInfo;
  vkfast->gpuIndex;
  vkfast->gpu = gpu; // NOTE(Constantine): If vkfast->gpuIndex is changed on the user side, vkfast->gpu must be changed by the user too.
  vkfast->mainQueueIndex;
  vkfast->mainQueueFamilyIndex;
  vkfast->mainQueue = mainQueue; // NOTE(Constantine): If vkfast->mainQueueIndex is changed on the user side, vkfast->mainQueue must be changed by the user too.
  vkfast->specificMemoryTypesGpuVram = specificMemoryTypesGpuVram;
  vkfast->specificMemoryTypesCpuUpload = specificMemoryTypesCpuUpload;
  vkfast->specificMemoryTypesCpuReadback = specificMemoryTypesCpuReadback;
  vkfast->memoryAllocationSizeGpuVram = internalMemoryAllocationSizeGpuVramArrays;
  vkfast->memoryAllocationSizeCpuUpload = internalMemoryAllocationSizeCpuVisible;
  vkfast->memoryAllocationSizeCpuReadback = internalMemoryAllocationSizeCpuReadback;
  vkfast->memoryAllocationSizeCpuUploadPresentPixels = internalMemoryAllocationSizeCpuVisiblePresentPixels;
  vkfast->memoryGpuVramForArrays_array = memoryGpuVramForArrays_array;
  vkfast->memoryGpuVramForArrays_memory = memoryGpuVramForArrays_memory;
  vkfast->memoryGpuVramForArrays_memory_suballocations_offset = 0;
  vkfast->memoryCpuUpload_array = memoryCpuUpload_array;
  vkfast->memoryCpuUpload_memory = memoryCpuUpload_memory;
  vkfast->memoryCpuUpload_mapped_void_ptr_original = memoryCpuUpload_mapped_void_ptr;
  vkfast->memoryCpuUpload_mapped_void_ptr_offset = memoryCpuUpload_mapped_void_ptr;
  vkfast->memoryCpuUpload_memory_suballocations_offset = 0;
  vkfast->memoryCpuReadback_array = memoryCpuReadback_array;
  vkfast->memoryCpuReadback_memory = memoryCpuReadback_memory;
  vkfast->memoryCpuReadback_mapped_void_ptr_original = memoryCpuReadback_mapped_void_ptr;
  vkfast->memoryCpuReadback_mapped_void_ptr_offset = memoryCpuReadback_mapped_void_ptr;
  vkfast->memoryCpuReadback_memory_suballocations_offset = 0;
  vkfast->windowHandle = NULL;
  vkfast->windowHandleDoDestroy = 0;
  vkfast->screenWidth = 0;
  vkfast->screenHeight = 0;
  vkfast->presentQueueIndex;
  vkfast->surface = NULL;
  vkfast->present = NULL;
  vkfast->presentImages[0] = NULL;
  vkfast->presentImages[1] = NULL;
  vkfast->presentImages[2] = NULL;
  vkfast->presentCpuSignal = NULL;
  vkfast->presentGpuSignalAcquire = NULL;
  vkfast->presentGpuSignalSubmit = NULL;
  vkfast->presentCopyCalls = presentCopyCalls;
  vkfast->presentPixelsCpuUpload_memory_allocation_size = internalMemoryAllocationSizeCpuVisiblePresentPixels;
  vkfast->presentPixelsCpuUpload_memory_and_array = REDGPU_32_STRUCT(Red2Array, 0);
  vkfast->presentPixelsCpuUpload_void_ptr_original = NULL;
  vkfast->presentVsyncMode = RED_PRESENT_VSYNC_MODE_ON;
  vkfast->presentImagesCount = 3;

  return (gpu_handle_context_t)(void *)vkfast;
}

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optionalFile, int optionalLine) {
  return vfInternalContextInit(enable_debug_mode, 0, optional_parameters, NULL, NULL, optionalFile, optionalLine);
}

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const char * optionalFile, int optionalLine) {
  return vfInternalContextInit(enable_debug_mode, gpu_index, optional_parameters, NULL, NULL, optionalFile, optionalLine);
}

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx2(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const char * optionalFile, int optionalLine) {
  return vfInternalContextInit(enable_debug_mode, 0, optional_parameters, optional_ex2_parameters, NULL, optionalFile, optionalLine);
}

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx3(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const gpu_context_ex3_parameters_t * optional_ex3_parameters, const char * optionalFile, int optionalLine) {
  return vfInternalContextInit(enable_debug_mode, 0, optional_parameters, optional_ex2_parameters, optional_ex3_parameters, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfIdDestroy(uint64_t ids_count, const uint64_t * ids, const char * optionalFile, int optionalLine) {
  for (uint64_t i = 0; i < ids_count; i += 1) {
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    if (handle == NULL) {
      continue;
    }
    if (handle->handle_id == VF_HANDLE_ID_INVALID) {
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_GPU_CODE) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_GPU_CODE,
        "handle", handle->gpuCode.gpuCode,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_PROCEDURE) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PROCEDURE_PARAMETERS,
        "handle", handle->procedure.procedureParameters.procedureParameters,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      for (int i = 0; i < 7; i += 1) {
        np(red2DestroyHandle,
          "context", handle->vkfast->context,
          "gpu", handle->vkfast->gpu,
          "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
          "handle", handle->procedure.procedureParameters.structsDeclarations[i],
          "optionalHandle2", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
      }
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
        "handle", handle->procedure.procedureParameters.handlesDeclaration,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PROCEDURE,
        "handle", handle->procedure.procedure,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_BATCH) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCTS_MEMORY,
        "handle", handle->batch.structsMemory,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCTS_MEMORY,
        "handle", handle->batch.structsMemorySamplers,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
        "handle", handle->batch.currentStructSamplers.handleDeclaration,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_CALLS,
        "handle", handle->batch.calls.handle,
        "optionalHandle2", handle->batch.calls.memory,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }
  }

  for (uint64_t i = 0; i < ids_count; i += 1) {
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    red32MemoryFree(handle); // NOTE(Constantine): Internally, all handles must be allocated, except for async_id.
  }
}

GPU_API_PRE void GPU_API_POST vfContextDeinit(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  vfAllQueuesWaitIdle(context, optionalFile, optionalLine);

  // NOTE(Constantine): WSI.
  {
    if (vkfast->presentPixelsCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory != NULL) {
      np(redMemoryUnmap,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "mappableMemory", vkfast->presentPixelsCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_ARRAY,
      "handle", vkfast->presentPixelsCpuUpload_memory_and_array.array.handle,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_MEMORY,
      "handle", vkfast->presentPixelsCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CALLS,
      "handle", vkfast->presentCopyCalls.handle,
      "optionalHandle2", vkfast->presentCopyCalls.memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
      "handle", vkfast->presentCpuSignal,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
      "handle", vkfast->presentGpuSignalAcquire,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
      "handle", vkfast->presentGpuSignalSubmit,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_PRESENT,
      "handle", vkfast->present,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_SURFACE,
      "handle", vkfast->surface,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryGpuVramForArrays_array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryGpuVramForArrays_memory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->memoryCpuUpload_memory != NULL) {
    np(redMemoryUnmap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", vkfast->memoryCpuUpload_memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryCpuUpload_array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryCpuUpload_memory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->memoryCpuReadback_memory != NULL) {
    np(redMemoryUnmap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", vkfast->memoryCpuReadback_memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryCpuReadback_array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryCpuReadback_memory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->doNotDestroyRawContext == 0) {
    np(redDestroyContext,
      "context", vkfast->context,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  if (vkfast->windowHandle != NULL && vkfast->windowHandleDoDestroy == 1) {
    #if !defined(VKFAST_DISABLE_WIN32)
    red32WindowDestroy(vkfast->windowHandle);
    #endif
  }

  if (vkfast->doNotFreeHandle == 0) {
    red32MemoryFree(vkfast);
  }
}

GPU_API_PRE RedContext GPU_API_POST vfContextGetRaw(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  REDGPU_2_EXPECT(vkfast != NULL);

  return vkfast->context;
}

GPU_API_PRE void GPU_API_POST vfContextResetAndInvalidateAllStorages(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  vkfast->memoryGpuVramForArrays_memory_suballocations_offset = 0;
  vkfast->memoryCpuUpload_mapped_void_ptr_offset = vkfast->memoryCpuUpload_mapped_void_ptr_original;
  vkfast->memoryCpuUpload_memory_suballocations_offset = 0;
  vkfast->memoryCpuReadback_mapped_void_ptr_offset = vkfast->memoryCpuReadback_mapped_void_ptr_original;
  vkfast->memoryCpuReadback_memory_suballocations_offset = 0;
}

#if defined(_WIN32) && !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE void GPU_API_POST vfGetMainMonitorAreaRectangle(int * out4ints, const char * optionalFile, int optionalLine) {
  // https://learn.microsoft.com/ru-ru/windows/win32/api/winuser/nf-winuser-monitorfrompoint
  POINT point = {0};
  HMONITOR hmonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);

  // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmonitorinfoa
  MONITORINFO monitorInfo = {0};
  monitorInfo.cbSize = sizeof(MONITORINFO);
  REDGPU_2_EXPECT(GetMonitorInfoA(hmonitor, &monitorInfo));

  out4ints[0] = monitorInfo.rcMonitor.left;
  out4ints[1] = monitorInfo.rcMonitor.top;
  out4ints[2] = monitorInfo.rcMonitor.right;
  out4ints[3] = monitorInfo.rcMonitor.bottom;
}
#endif

#if defined(__linux__) && !defined(__ANDROID__)
GPU_API_PRE void GPU_API_POST vfGetMainMonitorAreaRectangle(int * out4ints, const char * optionalFile, int optionalLine) {
  Display * display = XOpenDisplay(NULL);
  REDGPU_2_EXPECT(display != NULL);

  int screen = DefaultScreen(display);

  out4ints[0] = 0;
  out4ints[1] = 0;
  out4ints[2] = (int)DisplayWidth(display, screen);
  out4ints[3] = (int)DisplayHeight(display, screen);

  XCloseDisplay(display);
  display = NULL;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
static int vfInternalRebuildPresent(gpu_handle_context_t context, RedPresentVsyncMode presentVsyncMode, int presentImagesCount, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECT(presentImagesCount == 2 || presentImagesCount == 3);

  RedHandlePresent present = NULL;
  RedHandleImage presentImages[3] = {0};

  // NOTE(Constantine): Destroying previous possible present resources.
  {
    // NOTE(Constantine):
    // Intentional, do not remove, waiting for the present cpu signal here
    // because we have to be sure we can re-record calls with which this signal is paired.
    if (vkfast->presentCpuSignal != NULL) {
      np(redCpuSignalWait,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "cpuSignalsCount", 1,
        "cpuSignals", &vkfast->presentCpuSignal,
        "waitAll", 1,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }

    // NOTE(Constantine): Present queue wait idle.
    np(redQueuePresent,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
      "waitForAndUnsignalGpuSignalsCount", 0,
      "waitForAndUnsignalGpuSignals", NULL,
      "presentsCount", 0,
      "presents", NULL,
      "presentsImageIndex", NULL,
      "outPresentsStatus", NULL,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    // NOTE(Constantine): Destroy aborted cpu and gpu signals.
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
      "handle", vkfast->presentCpuSignal,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    vkfast->presentCpuSignal = NULL;
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
      "handle", vkfast->presentGpuSignalAcquire,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    vkfast->presentGpuSignalAcquire = NULL;
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
      "handle", vkfast->presentGpuSignalSubmit,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    vkfast->presentGpuSignalSubmit = NULL;
  }

  if (vkfast->surface == NULL) {
    #if defined(_WIN32)
    np(redCreateSurfaceWin32,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalRebuildPresent_surface_win32",
      "win32Hinstance", GetModuleHandle(NULL),
      "win32Hwnd", vkfast->windowHandle,
      "outSurface", &vkfast->surface,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    #endif
    #if defined(__linux__) && !defined(__ANDROID__)
    {
      // NOTE(Constantine): this struct's layout is defined in redgpu_32.c file of REDGPU 2 SDK.
      struct X11WindowData {
        Display * display;
        Window    window;
        Atom      wmDeleteMessage;
      };
      struct X11WindowData * h = vkfast->windowHandle;

      np(redCreateSurfaceXlibOrXcb,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleName", "vkFast_vfInternalRebuildPresent_surface_xlib",
        "xlibDisplay", h->display,
        "xlibWindow", h->window,
        "xcbConnection", NULL,
        "xcbWindow", 0,
        "outSurface", &vkfast->surface,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }
    #endif
    REDGPU_2_EXPECTWG(vkfast->surface != NULL);
  }

  RedQueueFamilyIndexGetSupportsPresentOnSurface queueFamilyIndexSupportsPresentOnSurface = {0};
  queueFamilyIndexSupportsPresentOnSurface.surface                                     = vkfast->surface;
  queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface = 0;
  np(redQueueFamilyIndexGetSupportsPresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
    "supportsPresentOnWin32", NULL,
    "supportsPresentOnXlib", NULL,
    "supportsPresentOnXcb", NULL,
    "supportsPresentOnSurface", &queueFamilyIndexSupportsPresentOnSurface,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 1);

  RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {0};
  np(redSurfaceGetCurrentPropertiesAndPresentLimits,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "surface", vkfast->surface,
    "outSurfaceCurrentPropertiesAndPresentLimits", &surfaceCurrentPropertiesAndPresentLimits,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  // printf("[vkFast][DEBUG PRINTF] surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth/Height: %d, %d\n", surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth, surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight);
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth == 0 || surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight == 0) {
    // NOTE(Constantine): The window is minimized or of unwatchable size (resized to 0 in width or 0 in height). Tested on Windows 10.
    int isRebuilded = 0;
    return isRebuilded;
  }
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth != -1) {
    vkfast->screenWidth = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth;
  }
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight != -1) {
    vkfast->screenHeight = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight;
  }

  // NOTE(Constantine):
  // WSI destroy before the rebuild (except for present calls that are re-recordable anyway,
  // and present pixels CPU upload resources, since they're pre-allocated for the worst case
  // window resolution of 8kx8k, currently).
  {
    if (vkfast->present != NULL) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PRESENT,
        "handle", vkfast->present,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      vkfast->present = NULL;
    }
  }

  np(redCreatePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "handleName", "vkFast_vfInternalRebuildPresent_present",
    "surface", vkfast->surface,
    "imagesCount", presentImagesCount,
    "imagesWidth", vkfast->screenWidth,
    "imagesHeight", vkfast->screenHeight,
    "imagesLayersCount", 1,
    "imagesRestrictToAccess", RED_ACCESS_BITFLAG_COPY_W,
    "transform", RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY,
    "compositeAlpha", RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE,
    "vsyncMode", presentVsyncMode,
    "clipped", 0,
    "discardAfterPresent", 1, // NOTE(Constantine): Optimization.
    "oldPresent", NULL,
    "outPresent", &present,
    "outImages", presentImages,
    "outTextures", NULL,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(present != NULL);

  if (vkfast->presentPixelsCpuUpload_memory_and_array.array.handle == NULL && vkfast->presentPixelsCpuUpload_memory_allocation_size > 0) {
    np(red2CreateArray,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalRebuildPresent_presentPixelsCpuUpload_memory_and_array",
      "type", RED_ARRAY_TYPE_ARRAY_RO,
      "bytesCount", vkfast->presentPixelsCpuUpload_memory_allocation_size,
      "structuredBufferElementBytesCount", 0,
      "restrictToAccess", RED_ACCESS_BITFLAG_COPY_R,
      "initialQueueFamilyIndex", vkfast->gpuInfo->queuesCount > 1 ? -1 : (unsigned)vkfast->gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
      "maxAllowedOverallocationBytesCount", 0, // NOTE(Constantine): Intel UHD Graphics 730 on Windows 10 aligns CPU visible allocations to 64 bytes.
      "dedicate", 0,
      "mappable", 1,
      "dedicateOrMappableMemoryTypeIndex", vkfast->specificMemoryTypesCpuUpload,
      "dedicateOrMappableMemoryBitflags", 0,
      "suballocateFromMemoryOnFirstMatchPointersCount", 0,
      "suballocateFromMemoryOnFirstMatchPointers", NULL,
      "outArray", &vkfast->presentPixelsCpuUpload_memory_and_array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(redMemoryMap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", vkfast->presentPixelsCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", vkfast->presentPixelsCpuUpload_memory_and_array.array.memoryBytesCount,
      "outVolatilePointer", &vkfast->presentPixelsCpuUpload_void_ptr_original,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(vkfast->presentPixelsCpuUpload_void_ptr_original != NULL);
    REDGPU_2_EXPECTWG(0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)vkfast->presentPixelsCpuUpload_void_ptr_original, vkfast->gpuInfo->minMemoryAllocateBytesAlignment)); // NOTE(Constantine): Start address is guaranteed to be aligned.
  }

  vkfast->surface;
  vkfast->present = present;
  vkfast->presentImages[0] = presentImages[0];
  vkfast->presentImages[1] = presentImages[1];
  vkfast->presentImages[2] = presentImages[2];
  vkfast->presentCpuSignal;
  vkfast->presentGpuSignalAcquire;
  vkfast->presentGpuSignalSubmit;
  vkfast->presentCopyCalls;
  vkfast->presentVsyncMode;
  vkfast->presentImagesCount;

  int isRebuilded = 1;
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfWindowFullscreenEx(gpu_handle_context_t context, void * optional_external_window_handle, const char * window_title, int screen_width, int screen_height, unsigned draw_queue_index, RedPresentVsyncMode present_vsync_mode, int present_images_count, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(draw_queue_index < vkfast->gpuInfo->queuesCount);

  void * window_handle = optional_external_window_handle;
  int window_handle_do_destroy = 0;
  if (window_handle == NULL) {
    window_handle = red32WindowCreate(window_title);
    window_handle_do_destroy = 1;
  }
  REDGPU_2_EXPECTWG(window_handle != NULL);

  vkfast->windowHandle = window_handle;
  vkfast->windowHandleDoDestroy = window_handle_do_destroy;
  vkfast->screenWidth = screen_width;
  vkfast->screenHeight = screen_height;
  vkfast->presentQueueIndex = draw_queue_index;
  vkfast->presentVsyncMode = present_vsync_mode;
  vkfast->presentImagesCount = present_images_count;

  return vfInternalRebuildPresent(context, present_vsync_mode, present_images_count, optionalFile, optionalLine);
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfWindowFullscreen(gpu_handle_context_t context, void * optional_external_window_handle, const char * window_title, int screen_width, int screen_height, unsigned draw_queue_index, RedPresentVsyncMode present_vsync_mode, const char * optionalFile, int optionalLine) {
  return vfWindowFullscreenEx(context, optional_external_window_handle, window_title, screen_width, screen_height, draw_queue_index, present_vsync_mode, 3, optionalFile, optionalLine);
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfWindowLoop(gpu_handle_context_t context) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  return red32WindowLoop(vkfast->windowHandle);
}
#endif

#if defined(_WIN32) && !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfWindowIsMinimized(gpu_handle_context_t context) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  return (int)IsIconic((HWND)vkfast->windowHandle);
}
#endif

#if defined(__linux__) && !defined(__ANDROID__)
GPU_API_PRE int GPU_API_POST vfWindowIsMinimized(gpu_handle_context_t context) {
  return 0; // NOTE(Constantine): There's no simple X11 code to check for this, so just assume the window is always visible.
}
#endif

GPU_API_PRE void GPU_API_POST vfWindowGetSize(gpu_handle_context_t context, int * out_window_width, int * out_window_height) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (out_window_width  != NULL) { out_window_width[0]  = vkfast->screenWidth;  }
  if (out_window_height != NULL) { out_window_height[0] = vkfast->screenHeight; }
}

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE void GPU_API_POST vfExit(int exit_code) {
  red32Exit(exit_code);
}
#endif

GPU_API_PRE void GPU_API_POST vfStorageCreate(gpu_handle_context_t context, const gpu_storage_info_t * storage_info, gpu_storage_t * out_storage, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(storage_info->storage_type != GPU_STORAGE_TYPE_NONE);

  uint64_t             alignment         = 0;
  RedStructMemberArray arrayRangeInfo    = {0};
  void *               mappedVoidPointer = NULL;
  {
    // NOTE(Constantine): Storage range mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_GPU_ONLY) {
      
      alignment = vkfast->gpuInfo->minArrayRORWStructMemberRangeBytesAlignment;

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryGpuVramForArrays_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryGpuVramForArrays_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryGpuVramForArrays_array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryGpuVramForArrays_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryGpuVramForArrays_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryGpuVramForArrays_memory_suballocations_offset <= vkfast->memoryGpuVramForArrays_array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      alignment = vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryCpuUpload_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryCpuUpload_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryCpuUpload_array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryCpuUpload_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryCpuUpload_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryCpuUpload_memory_suballocations_offset <= vkfast->memoryCpuUpload_array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      alignment = vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryCpuReadback_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryCpuReadback_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryCpuReadback_array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryCpuReadback_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryCpuReadback_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryCpuReadback_memory_suballocations_offset <= vkfast->memoryCpuReadback_array.memoryBytesCount);

    } else {
#if defined(__linux__) || defined(__MINGW32__)
      REDGPU_2_EXPECT(!"[vkFast Internal] Unreachable enum value.");
#else
      REDGPU_2_EXPECT(!"[vkFast Internal][" __FUNCTION__ "] Unreachable enum value.");
#endif
    }

    // NOTE(Constantine): Pointer mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      mappedVoidPointer = vkfast->memoryCpuUpload_mapped_void_ptr_offset; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)vkfast->memoryCpuUpload_mapped_void_ptr_offset;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      vkfast->memoryCpuUpload_mapped_void_ptr_offset = (void *)ptr;

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      mappedVoidPointer = vkfast->memoryCpuReadback_mapped_void_ptr_offset; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)vkfast->memoryCpuReadback_mapped_void_ptr_offset;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      vkfast->memoryCpuReadback_mapped_void_ptr_offset = (void *)ptr;

    }
  }

  REDGPU_2_EXPECTWG(arrayRangeInfo.arrayRangeBytesCount <= vkfast->context->gpus[vkfast->gpuIndex].maxArrayRORWStructMemberRangeBytesCount);

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_storage_t;
  handle->vkfast                 = vkfast;
  handle->handle_id              = VF_HANDLE_ID_STORAGE;
  handle->storage.info           = storage_info[0];
  handle->storage.arrayRangeInfo = arrayRangeInfo;

  // Filling
  gpu_storage_t;
  out_storage->id              = (uint64_t)(void *)handle;
  out_storage->info            = storage_info[0];
  out_storage->alignment       = alignment;
  out_storage->mapped_void_ptr = mappedVoidPointer;
}

GPU_API_PRE void GPU_API_POST vfStorageGetRaw(gpu_handle_context_t context, uint64_t storage_id, RedStructMemberArray * out_storage_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * storage = (vf_handle_t *)(void *)storage_id;
  vf_handle_context_t * vkfast = storage->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);

  out_storage_raw[0] = storage->storage.arrayRangeInfo;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryCompute(gpu_handle_context_t context, const gpu_program_info_t * program_info, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  // To destroy
  RedHandleGpuCode gpuCode = NULL;
  np(redCreateGpuCode,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_info->optional_debug_name,
    "irBytesCount", program_info->program_binary_bytes_count,
    "ir", (const void *)program_info->program_binary,
    "outGpuCode", &gpuCode,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(gpuCode != NULL);

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  vf_handle_t;
  vf_handle_gpu_code_t;
  handle->vkfast              = vkfast;
  handle->handle_id           = VF_HANDLE_ID_GPU_CODE;
  handle->gpuCode.info        = program_info[0];
  handle->gpuCode.gpuCodeType = VF_GPU_CODE_TYPE_COMPUTE;
  handle->gpuCode.gpuCode     = gpuCode;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramPipelineCreateCompute(gpu_handle_context_t context, const gpu_program_pipeline_compute_info_t * program_pipeline_compute_info, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  vf_handle_t * gpuCodeCompute = (vf_handle_t *)(void *)program_pipeline_compute_info->compute_program;
  REDGPU_2_EXPECTWG(gpuCodeCompute->handle_id == VF_HANDLE_ID_GPU_CODE);
  REDGPU_2_EXPECTWG(gpuCodeCompute->gpuCode.gpuCodeType == VF_GPU_CODE_TYPE_COMPUTE);

  if (program_pipeline_compute_info->variables_bytes_count > 0) {
    for (unsigned i = 0; i < program_pipeline_compute_info->struct_members_count; i += 1) {
      REDGPU_2_EXPECTWG(program_pipeline_compute_info->variables_slot != program_pipeline_compute_info->struct_members[i].slot);
    }
  }

  Red2ProcedureParametersDeclaration parameters = {0};
  parameters.variablesSlot            = program_pipeline_compute_info->variables_slot;
  parameters.variablesVisibleToStages = program_pipeline_compute_info->variables_bytes_count == 0 ? 0 : RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  parameters.variablesBytesCount      = program_pipeline_compute_info->variables_bytes_count;
  parameters.structsDeclarationsCount = program_pipeline_compute_info->struct_members_count == 0 ? 0 : 1;
  parameters.structsDeclarations[0].structDeclarationMembersCount        = program_pipeline_compute_info->struct_members_count;
  parameters.structsDeclarations[0].structDeclarationMembers             = program_pipeline_compute_info->struct_members;
  parameters.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
  parameters.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;

  // To destroy
  Red2ProcedureParametersAndDeclarations procedureParameters = {0};
  np(red2CreateProcedureParameters,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_pipeline_compute_info->optional_debug_name,
    "procedureParametersDeclaration", &parameters,
    "outProcedureParametersAndDeclarations", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(procedureParameters.procedureParameters != NULL);

  // To destroy
  RedHandleProcedure procedure = NULL;
  np(redCreateProcedureCompute,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_pipeline_compute_info->optional_debug_name,
    "procedureCache", NULL,
    "procedureParameters", procedureParameters.procedureParameters,
    "gpuCodeMainProcedureName", "main",
    "gpuCode", gpuCodeCompute->gpuCode.gpuCode,
    "outProcedure", &procedure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(procedure != NULL);

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_procedure_t;
  handle->vkfast                        = vkfast;
  handle->handle_id                     = VF_HANDLE_ID_PROCEDURE;
  handle->procedure.infoCompute         = program_pipeline_compute_info[0];
  handle->procedure.procedureType       = VF_PROCEDURE_TYPE_COMPUTE;
  handle->procedure.procedureParameters = procedureParameters;
  handle->procedure.procedure           = procedure;

  return (uint64_t)(void *)handle;
}

static uint64_t vfInternalBatchBegin(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, unsigned queue_family_index, const char * optional_debug_name, const char * optionalFile, int optionalLine) {
  vf_handle_t * handle = (vf_handle_t *)(void *)existing_batch_id;
  
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  if (handle == NULL) {
    // To destroy
    RedCalls calls = {0};
    np(redCreateCallsReusable,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", optional_debug_name,
      "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[queue_family_index],
      "outCalls", &calls,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(calls.handle != NULL);
  
    RedCallProceduresAndAddresses addresses = {0};
    np(redGetCallProceduresAndAddresses,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outCallProceduresAndAddresses", &addresses,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    RedHandleStructsMemory structsMemory = 0;
    if (batch_info != NULL) {
      if (batch_info->max_new_bindings_sets_count > 0) {
        // To destroy
        np(redStructsMemoryAllocate,
          "context", vkfast->context,
          "gpu", vkfast->gpu,
          "handleName", optional_debug_name,
          "maxStructsCount", batch_info->max_new_bindings_sets_count,
          "maxStructsMembersOfTypeArrayROConstantCount", 0,
          "maxStructsMembersOfTypeArrayROOrArrayRWCount", batch_info->max_storage_binds_count,
          "maxStructsMembersOfTypeTextureROCount", batch_info->max_texture_ro_binds_count,
          "maxStructsMembersOfTypeTextureRWCount", batch_info->max_texture_rw_binds_count,
          "outStructsMemory", &structsMemory,
          "outStatuses", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
        REDGPU_2_EXPECTWG(structsMemory != NULL);
      }
    }

    RedHandleStructsMemory structsMemorySamplers = NULL;
    if (batch_info != NULL) {
      if (batch_info->max_sampler_binds_count > 0) {
        REDGPU_2_EXPECTWG(batch_info->max_sampler_binds_count <= 4000);
        // To destroy
        np(redStructsMemoryAllocateSamplers,
          "context", vkfast->context,
          "gpu", vkfast->gpu,
          "handleName", optional_debug_name,
          "maxStructsCount", 1, // NOTE(Constantine): Only one samplers struct per command list for now.
          "maxStructsMembersOfTypeSamplerCount", batch_info->max_sampler_binds_count,
          "outStructsMemory", &structsMemorySamplers,
          "outStatuses", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
        REDGPU_2_EXPECTWG(structsMemorySamplers != NULL);
      }
    }

    // To free
    handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
    REDGPU_2_EXPECTWG(handle != NULL);

    // Filling
    vf_handle_t;
    vf_handle_batch_t;
    handle->vkfast                                  = vkfast;
    handle->handle_id                               = VF_HANDLE_ID_BATCH;
    handle->batch.calls                             = calls;
    handle->batch.addresses                         = addresses;
    handle->batch.structsMemory                     = structsMemory;
    handle->batch.structsMemorySamplers             = structsMemorySamplers;
    handle->batch.currentStruct                     = REDGPU_32_STRUCT(Red2Struct, 0);
    handle->batch.currentStructSamplers             = REDGPU_32_STRUCT(Red2Struct, 0); // NOTE(Constantine): Set below.
    handle->batch.currentProcedureParametersCompute = NULL;
  }

  np(redCallsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", handle->batch.calls.handle,
    "callsMemory", handle->batch.calls.memory,
    "callsReusable", handle->batch.calls.reusable,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (handle->batch.structsMemory != NULL) {
    np(redStructsMemoryReset,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "structsMemory", handle->batch.structsMemory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  if (handle->batch.structsMemorySamplers != NULL) {
    np(redStructsMemoryReset,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "structsMemory", handle->batch.structsMemorySamplers,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", handle->vkfast->context,
      "gpu", handle->vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
      "handle", handle->batch.currentStructSamplers.handleDeclaration,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    RedStructDeclarationMember samplers[4000] = {0}; // NOTE(Constantine): Kinda big on stack size, but whatever.
    for (unsigned i = 0; i < batch_info->max_sampler_binds_count; i += 1) {
      samplers[i].slot            = i;
      samplers[i].type            = RED_STRUCT_MEMBER_TYPE_SAMPLER;
      samplers[i].count           = 1;
      samplers[i].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT; // NOTE(Constantine): I doubt anyone needs to sample textures in vertex shaders?
    }
    Red2Struct currentStructSamplers = {0};
    np(red2StructsMemorySuballocateStruct,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", optional_debug_name,
      "structsMemory", handle->batch.structsMemorySamplers,
      "structDeclarationMembersCount", batch_info->max_sampler_binds_count,
      "structDeclarationMembers", samplers,
      "structDeclarationMembersArrayROCount", 0,
      "structDeclarationMembersArrayRO", NULL,
      "outStruct", &currentStructSamplers,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(currentStructSamplers.handleDeclaration != NULL);
    REDGPU_2_EXPECTWG(currentStructSamplers.handle != NULL || !"red2StructsMemorySuballocateStruct() call returned NULL. Ran out of vfBatchBegin()::batch_bindings_info::max_new_bindings_sets_count and vfBatchBegin()::batch_bindings_info::max_sampler_binds_count memory to allocate?");
    handle->batch.currentStructSamplers = currentStructSamplers;
  }

  if (handle->batch.structsMemory != NULL || handle->batch.structsMemorySamplers != NULL) {
    // NOTE(Constantine): Oh God, I hope this call doesn't copy descriptors from structsMemory like redCallSetProcedureParametersStructs() :D
    np(redCallSetStructsMemory,
      "address", handle->batch.addresses.redCallSetStructsMemory,
      "calls", handle->batch.calls.handle,
      "structsMemory", handle->batch.structsMemory,
      "structsMemorySamplers", handle->batch.structsMemorySamplers
    );
  }

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfBatchBegin(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, const char * optional_debug_name, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  return vfInternalBatchBegin(context, existing_batch_id, batch_info, vkfast->mainQueueFamilyIndex, optional_debug_name, optionalFile, optionalLine);
}

GPU_API_PRE uint64_t GPU_API_POST vfBatchBeginEx(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, unsigned queue_family_index, const char * optional_debug_name, const char * optionalFile, int optionalLine) {
  return vfInternalBatchBegin(context, existing_batch_id, batch_info, queue_family_index, optional_debug_name, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_cpu_storage = (vf_handle_t *)(void *)from_cpu_storage_id;
  REDGPU_2_EXPECTWG(from_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_gpu_storage = (vf_handle_t *)(void *)to_gpu_storage_id;
  REDGPU_2_EXPECTWG(to_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_cpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.arrayWBytesFirst  = to_gpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.bytesCount        = from_cpu_storage->storage.info.bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_cpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_gpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_gpu_storage = (vf_handle_t *)(void *)from_gpu_storage_id;
  REDGPU_2_EXPECTWG(from_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_cpu_storage = (vf_handle_t *)(void *)to_cpu_storage_id;
  REDGPU_2_EXPECTWG(to_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_gpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.arrayWBytesFirst  = to_cpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.bytesCount        = from_gpu_storage->storage.info.bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_gpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_cpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyRaw(gpu_handle_context_t context, uint64_t batch_id, RedHandleArray from_storage_raw, RedHandleArray to_storage_raw, const RedCopyArrayRange * range, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_storage_raw,
    "arrayW", to_storage_raw,
    "rangesCount", 1,
    "ranges", range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindProgramPipelineCompute(gpu_handle_context_t context, uint64_t batch_id, uint64_t program_pipeline_compute_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  vf_handle_t * program_pipeline_compute = (vf_handle_t *)(void *)program_pipeline_compute_id;
  REDGPU_2_EXPECTWG(program_pipeline_compute->handle_id == VF_HANDLE_ID_PROCEDURE);
  REDGPU_2_EXPECTWG(program_pipeline_compute->procedure.procedureType == VF_PROCEDURE_TYPE_COMPUTE);

  npfp(redCallSetProcedure, batch->batch.addresses.redCallSetProcedure,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedure", program_pipeline_compute->procedure.procedure
  );

  batch->batch.currentProcedureParametersCompute = program_pipeline_compute->procedure.procedureParameters.procedureParameters;
}

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsSet(gpu_handle_context_t context, uint64_t batch_id, int slots_count, const RedStructDeclarationMember * slots, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.structsMemory != NULL || !"vfBatchBegin()::batch_bindings_info was set to NULL?");
  if (batch->batch.currentProcedureParametersCompute == NULL) {
    REDGPU_2_EXPECTWG(!"Was vfBatchBindProgramPipelineCompute() ever called previously?");
  }

  Red2Struct structure = {0};
  np(red2StructsMemorySuballocateStruct,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "structsMemory", batch->batch.structsMemory,
    "structDeclarationMembersCount", slots_count,
    "structDeclarationMembers", slots,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "outStruct", &structure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(structure.handleDeclaration != NULL);
  REDGPU_2_EXPECTWG(structure.handle != NULL || !"red2StructsMemorySuballocateStruct() call returned NULL. Ran out of vfBatchBegin()::batch_bindings_info::max_new_bindings_sets_count and all the other vfBatchBegin()::batch_bindings_info::max_* memory to allocate?");
  batch->batch.currentStruct = structure;

  np(redCallSetProcedureParameters,
    "address", batch->batch.addresses.redCallSetProcedureParameters,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", batch->batch.currentProcedureParametersCompute
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindStorageRaw(gpu_handle_context_t context, uint64_t batch_id, int slot, int storage_raw_count, const RedStructMemberArray * storage_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was vfBatchBindNewBindingsSet() ever called previously?");

  for (int i = 0; i < storage_raw_count; i += 1) {
    REDGPU_2_EXPECTWG(storage_raw[i].arrayRangeBytesCount <= vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount);
  }

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = storage_raw_count;
  member.type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  member.textures  = NULL;
  member.arrays    = storage_raw;
  member.setTo00   = 0;
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindStorageSingle(gpu_handle_context_t context, uint64_t batch_id, int slot, uint64_t storage_id, const char * optionalFile, int optionalLine) {
  RedStructMemberArray storageRaw = {0};
  vfStorageGetRaw(context, storage_id, &storageRaw, optionalFile, optionalLine);
  vfBatchBindStorageRaw(context, batch_id, slot, 1, &storageRaw, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfBatchBindTextureRWEx(gpu_handle_context_t context, uint64_t batch_id, int slot, int textures_rw_count, const RedStructMemberTexture * textures_rw, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was vfBatchBindNewBindingsSet() ever called previously?");

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = textures_rw_count;
  member.type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
  member.textures  = textures_rw;
  member.arrays    = NULL;
  member.setTo00   = 0;
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", batch->batch.currentProcedureParametersCompute,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1, // NOTE(Constantine): Only one struct for now.
    "structs", &batch->batch.currentStruct.handle,
    "setTo0", 0,
    "setTo00", 0
  );

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
    "handle", batch->batch.currentStruct.handleDeclaration,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  batch->batch.currentStruct.handleDeclaration = NULL;
  batch->batch.currentStruct.handle = NULL;
}

GPU_API_PRE void GPU_API_POST vfBatchBindVariablesCopy(gpu_handle_context_t context, uint64_t batch_id, unsigned variables_bytes_offset, unsigned data_bytes_count, const void * data, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentProcedureParametersCompute != NULL || !"Was vfBatchBindProgramPipelineCompute() ever called previously?");

  npfp(redCallSetProcedureParametersVariables, batch->batch.addresses.redCallSetProcedureParametersVariables,
    "calls", batch->batch.calls.handle,
    "procedureParameters", batch->batch.currentProcedureParametersCompute,
    "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE,
    "variablesBytesFirst", variables_bytes_offset,
    "dataBytesCount", data_bytes_count,
    "data", data
  );
}

GPU_API_PRE void GPU_API_POST vfBatchCompute(gpu_handle_context_t context, uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallProcedureCompute, batch->batch.addresses.redCallProcedureCompute,
    "calls", batch->batch.calls.handle,
    "workgroupsCountX", workgroups_count_x,
    "workgroupsCountY", workgroups_count_y,
    "workgroupsCountZ", workgroups_count_z
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBarrierMemory(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalOrderBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBarrierCpuReadback(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalReadbackBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(redCallsEnd,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", batch->batch.calls.handle,
    "callsMemory", batch->batch.calls.memory,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  batch->batch.currentStruct.handle = NULL;
  batch->batch.currentStruct.handleDeclaration = NULL;
  batch->batch.currentProcedureParametersCompute = NULL;
}

GPU_API_PRE void GPU_API_POST vfGpuThreadCreate(gpu_handle_context_t context, unsigned gpu_threads_count, gpu_thread_t * out_gpu_threads, const char ** optional_gpu_threads_debug_name, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  for (unsigned i = 0; i < gpu_threads_count; i += 1) {
    np(redCreateGpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", optional_gpu_threads_debug_name == NULL ? NULL : optional_gpu_threads_debug_name[i],
      "outGpuSignal", &out_gpu_threads[i],
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(out_gpu_threads[i] != NULL);
  }

  {
    RedHandleCpuSignal tempCpuSignal = NULL;
    np(redCreateCpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfGpuThreadCreate_tempCpuSignal",
      "createSignaled", 0,
      "outCpuSignal", &tempCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(tempCpuSignal != NULL);

    RedCalls tempCalls = {0};
    np(redCreateCalls,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfGpuThreadCreate_tempCalls",
      "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
      "outCalls", &tempCalls,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(tempCalls.handle != NULL);

    {
      RedCallProceduresAndAddresses addresses = {0};
      np(redGetCallProceduresAndAddresses,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "outCallProceduresAndAddresses", &addresses,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      np(redCallsSet,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "calls", tempCalls.handle,
        "callsMemory", tempCalls.memory,
        "callsReusable", tempCalls.reusable,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      np(red2CallGlobalOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", tempCalls.handle
      );

      np(redCallsEnd,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "calls", tempCalls.handle,
        "callsMemory", tempCalls.memory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }

    RedGpuTimeline timelines[1] = {0};
    timelines[0].setTo4                            = 4;
    timelines[0].setTo0                            = 0;
    timelines[0].waitForAndUnsignalGpuSignalsCount = 0;
    timelines[0].waitForAndUnsignalGpuSignals      = NULL;
    timelines[0].setTo65536                        = NULL;
    timelines[0].callsCount                        = 1;
    timelines[0].calls                             = &tempCalls.handle;
    timelines[0].signalGpuSignalsCount             = gpu_threads_count;
    timelines[0].signalGpuSignals                  = out_gpu_threads;
    np(redQueueSubmit,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->mainQueue,
      "timelinesCount", 1,
      "timelines", timelines,
      "signalCpuSignal", tempCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(redCpuSignalWait,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &tempCpuSignal,
      "waitAll", 1,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
      "handle", tempCpuSignal,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CALLS,
      "handle", tempCalls.handle,
      "optionalHandle2", tempCalls.memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
}

GPU_API_PRE void GPU_API_POST vfGpuThreadDestroy(gpu_handle_context_t context, gpu_thread_t gpu_thread) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
    "handle", gpu_thread,
    "optionalHandle2", NULL,
    "optionalFile", NULL,
    "optionalLine", 0,
    "optionalUserData", NULL
  );
}

GPU_API_PRE RedHandleCalls GPU_API_POST vfBatchGetRawHandle(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  return batch->batch.calls.handle;
}

static uint64_t vfInternalAsyncBatchExecuteRaw(gpu_handle_context_t context, RedHandleQueue queue, uint64_t batch_calls_count, const RedHandleCalls * batch_calls, unsigned gpu_threads_count, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  // To destroy
  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(cpuSignal != NULL);

  RedGpuTimeline timelines[1] = {0};
  timelines[0].setTo4                            = 4;
  timelines[0].setTo0                            = 0;
  timelines[0].waitForAndUnsignalGpuSignalsCount = gpu_threads_count;
  timelines[0].waitForAndUnsignalGpuSignals      = gpu_threads;
  timelines[0].setTo65536                        = gpu_threads_array_of_65536_int_values;
  timelines[0].callsCount                        = batch_calls_count;
  timelines[0].calls                             = batch_calls;
  timelines[0].signalGpuSignalsCount             = gpu_threads_count;
  timelines[0].signalGpuSignals                  = gpu_threads;
  np(redQueueSubmit,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", queue,
    "timelinesCount", 1,
    "timelines", timelines,
    "signalCpuSignal", cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  return (uint64_t)(void *)cpuSignal;
}

GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecuteRaw(gpu_handle_context_t context, uint64_t batch_raw_count, const RedHandleCalls * batch_raw, unsigned gpu_threads_count, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  return vfInternalAsyncBatchExecuteRaw(context, vkfast->mainQueue, batch_raw_count, batch_raw, gpu_threads_count, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
}

GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecuteRawEx(gpu_handle_context_t context, RedHandleQueue queue, uint64_t batch_raw_count, const RedHandleCalls * batch_raw, unsigned gpu_threads_count, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  return vfInternalAsyncBatchExecuteRaw(context, queue, batch_raw_count, batch_raw, gpu_threads_count, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfAsyncWaitToFinish(gpu_handle_context_t context, uint64_t async_id, const char * optionalFile, int optionalLine) {
  if (async_id == 0) {
    return;
  }

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleCpuSignal cpuSignal = (RedHandleCpuSignal)(void *)async_id;

  np(redCpuSignalWait,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
    "handle", cpuSignal,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

#if !defined(VKFAST_DISABLE_WIN32)
static int vfInternalAsyncDrawPixels(gpu_handle_context_t context, const RedStructMemberArray * pixels_storage_raw, const void * copy_pixels, int * out_optional_internal_present_image_index, RedBool32 optional_copy_image, RedHandleImage optional_image_to_copy, unsigned gpu_threads_count_plus_one_empty, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  unsigned presentImageIndex = 0;
  RedStatuses presentGetImageIndexStatuses = {0};

  if (vkfast->presentCpuSignal == NULL) {
    np(redCreateCpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalAsyncDrawPixels_presentCpuSignal",
      "createSignaled", 1,
      "outCpuSignal", &vkfast->presentCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(vkfast->presentCpuSignal != NULL);
  }
  if (vkfast->presentGpuSignalAcquire == NULL) {
    np(redCreateGpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalAsyncDrawPixels_presentGpuSignalAcquire",
      "outGpuSignal", &vkfast->presentGpuSignalAcquire,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(vkfast->presentGpuSignalAcquire != NULL);
  }
  if (vkfast->presentGpuSignalSubmit == NULL) {
    np(redCreateGpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalAsyncDrawPixels_presentGpuSignalSubmit",
      "outGpuSignal", &vkfast->presentGpuSignalSubmit,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(vkfast->presentGpuSignalSubmit != NULL);
  }

  np(redPresentGetImageIndex,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "present", vkfast->present,
    "signalCpuSignal", NULL,
    "signalGpuSignal", vkfast->presentGpuSignalAcquire,
    "outImageIndex", &presentImageIndex,
    "outStatuses", &presentGetImageIndexStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (out_optional_internal_present_image_index != NULL) {
    out_optional_internal_present_image_index[0] = presentImageIndex;
  }

  REDGPU_2_EXPECTWG(presentGetImageIndexStatuses.status == RED_STATUS_SUCCESS || presentGetImageIndexStatuses.status == RED_STATUS_PRESENT_IS_SUBOPTIMAL);
  REDGPU_2_EXPECTWG(presentGetImageIndexStatuses.statusError == RED_STATUS_SUCCESS || presentGetImageIndexStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE);
  if (presentGetImageIndexStatuses.status == RED_STATUS_PRESENT_IS_SUBOPTIMAL || presentGetImageIndexStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE) {
    int isRebuilded = vfInternalRebuildPresent(context, vkfast->presentVsyncMode, vkfast->presentImagesCount, optionalFile, optionalLine);
    return isRebuilded;
  }

  if (copy_pixels != NULL) {
    // NOTE(Constantine): The reason we copy pixels here is because vkfast->screenWidth/Height were updated in a potential vfInternalRebuildPresent call above.
    red32MemoryCopy(vkfast->presentPixelsCpuUpload_void_ptr_original, copy_pixels, sizeof(unsigned char) * 4 * vkfast->screenHeight * vkfast->screenWidth);
  }

  np(redCpuSignalWait,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &vkfast->presentCpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(redCpuSignalUnsignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &vkfast->presentCpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedCalls * calls = &vkfast->presentCopyCalls;

  {
    RedCallProceduresAndAddresses addresses = {0};
    np(redGetCallProceduresAndAddresses,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outCallProceduresAndAddresses", &addresses,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(redCallsSet,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "calls", calls->handle,
      "callsMemory", calls->memory,
      "callsReusable", calls->reusable,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    {
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = 0;
      imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.oldAccess              = 0;
      imageUsage.newAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.oldState               = RED_STATE_UNUSABLE;
      imageUsage.newState               = RED_STATE_USABLE;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = vkfast->presentImages[presentImageIndex];
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      Red2UsageImageTempCallStruct imageUsageTempStruct = {0};
      np(red2CallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", calls->handle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "arrayTempCallStructs", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "imageTempCallStructs", &imageUsageTempStruct,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }

    if (optional_copy_image == 1) {
      // NOTE(Constantine):
      // 'RedBool32 optional_copy_image' parameter is intentional, because
      // if the user will pass a NULL image accidentally, this will still
      // take the image copy path, rather than the pixels buffer copy path.
      RedCopyImageRange copy = {0};
      copy.imageRParts.allParts     = RED_IMAGE_PART_BITFLAG_COLOR;
      copy.imageRParts.level        = 0;
      copy.imageRParts.layersFirst  = 0;
      copy.imageRParts.layersCount  = 1;
      copy.imageROffset.texelX      = 0;
      copy.imageROffset.texelY      = 0;
      copy.imageROffset.texelZ      = 0;
      copy.imageWParts.allParts     = RED_IMAGE_PART_BITFLAG_COLOR;
      copy.imageWParts.level        = 0;
      copy.imageWParts.layersFirst  = 0;
      copy.imageWParts.layersCount  = 1;
      copy.imageWOffset.texelX      = 0;
      copy.imageWOffset.texelY      = 0;
      copy.imageWOffset.texelZ      = 0;
      copy.extent.texelsCountWidth  = vkfast->screenWidth;
      copy.extent.texelsCountHeight = vkfast->screenHeight;
      copy.extent.texelsCountDepth  = 1;
      npfp(redCallCopyImageToImage, addresses.redCallCopyImageToImage,
        "calls", calls->handle,
        "imageR", optional_image_to_copy,
        "setTo1", 1,
        "imageW", vkfast->presentImages[presentImageIndex],
        "setTo01", 1,
        "rangesCount", 1,
        "ranges", &copy
      );
    } else {
      RedCopyArrayImageRange copy = {0};
      copy.arrayBytesFirst               = pixels_storage_raw->arrayRangeBytesFirst;
      copy.arrayTexelsCountToNextRow     = vkfast->screenWidth;
      copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
      copy.imageParts.allParts           = RED_IMAGE_PART_BITFLAG_COLOR;
      copy.imageParts.level              = 0;
      copy.imageParts.layersFirst        = 0;
      copy.imageParts.layersCount        = 1;
      copy.imageOffset.texelX            = 0;
      copy.imageOffset.texelY            = 0;
      copy.imageOffset.texelZ            = 0;
      copy.imageExtent.texelsCountWidth  = vkfast->screenWidth;
      copy.imageExtent.texelsCountHeight = vkfast->screenHeight;
      copy.imageExtent.texelsCountDepth  = 1;
      npfp(redCallCopyArrayToImage, addresses.redCallCopyArrayToImage,
        "calls", calls->handle,
        "arrayR", pixels_storage_raw->array,
        "imageW", vkfast->presentImages[presentImageIndex],
        "setTo1", 1,
        "rangesCount", 1,
        "ranges", &copy
      );
    }

    {
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.newAccessStages        = 0;
      imageUsage.oldAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.newAccess              = 0;
      imageUsage.oldState               = RED_STATE_USABLE;
      imageUsage.newState               = RED_STATE_PRESENT;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = vkfast->presentImages[presentImageIndex];
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      Red2UsageImageTempCallStruct imageUsageTempStruct = {0};
      np(red2CallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", calls->handle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "arrayTempCallStructs", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "imageTempCallStructs", &imageUsageTempStruct,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }

    np(redCallsEnd,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "calls", calls->handle,
      "callsMemory", calls->memory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  // NOTE(Constantine): Defend vkfast->presentGpuSignalSubmit from being signaled while it's still in use by the previous redQueuePresent() call.
  np(redQueuePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "waitForAndUnsignalGpuSignalsCount", 0,
    "waitForAndUnsignalGpuSignals", NULL,
    "presentsCount", 0,
    "presents", NULL,
    "presentsImageIndex", NULL,
    "outPresentsStatus", NULL,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  REDGPU_2_EXPECTWG(gpu_threads_count_plus_one_empty >= 1);

  {
    gpu_threads[gpu_threads_count_plus_one_empty-1] = vkfast->presentGpuSignalAcquire;

    RedGpuTimeline timelines[1] = {0};
    timelines[0].setTo4                            = 4;
    timelines[0].setTo0                            = 0;
    timelines[0].waitForAndUnsignalGpuSignalsCount = gpu_threads_count_plus_one_empty;
    timelines[0].waitForAndUnsignalGpuSignals      = gpu_threads;
    timelines[0].setTo65536                        = gpu_threads_array_of_65536_int_values;
    timelines[0].callsCount                        = 1;
    timelines[0].calls                             = &calls->handle;
    timelines[0].signalGpuSignalsCount             = 1;
    timelines[0].signalGpuSignals                  = &vkfast->presentGpuSignalSubmit;
    np(redQueueSubmit,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
      "timelinesCount", 1,
      "timelines", timelines,
      "signalCpuSignal", vkfast->presentCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  // NOTE(Constantine):
  // This code is here to guarantee that all the input threads are left in the output state.
  {
    RedHandleCpuSignal tempCpuSignal = NULL;
    np(redCreateCpuSignal,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalAsyncDrawPixels_tempCpuSignal",
      "createSignaled", 0,
      "outCpuSignal", &tempCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(tempCpuSignal != NULL);

    RedCalls tempCalls = {0};
    np(redCreateCalls,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_vfInternalAsyncDrawPixels_tempCalls",
      "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
      "outCalls", &tempCalls,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(tempCalls.handle != NULL);

    {
      RedCallProceduresAndAddresses addresses = {0};
      np(redGetCallProceduresAndAddresses,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "outCallProceduresAndAddresses", &addresses,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      np(redCallsSet,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "calls", tempCalls.handle,
        "callsMemory", tempCalls.memory,
        "callsReusable", tempCalls.reusable,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      np(red2CallGlobalOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", tempCalls.handle
      );

      np(redCallsEnd,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "calls", tempCalls.handle,
        "callsMemory", tempCalls.memory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }

    gpu_threads[gpu_threads_count_plus_one_empty-1] = vkfast->presentGpuSignalSubmit;

    RedGpuTimeline timelines[1] = {0};
    timelines[0].setTo4                            = 4;
    timelines[0].setTo0                            = 0;
    timelines[0].waitForAndUnsignalGpuSignalsCount = 1;
    timelines[0].waitForAndUnsignalGpuSignals      = &vkfast->presentGpuSignalSubmit;
    timelines[0].setTo65536                        = gpu_threads_array_of_65536_int_values;
    timelines[0].callsCount                        = 1;
    timelines[0].calls                             = &tempCalls.handle;
    timelines[0].signalGpuSignalsCount             = gpu_threads_count_plus_one_empty;
    timelines[0].signalGpuSignals                  = gpu_threads;
    np(redQueueSubmit,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
      "timelinesCount", 1,
      "timelines", timelines,
      "signalCpuSignal", tempCpuSignal,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(redCpuSignalWait,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &tempCpuSignal,
      "waitAll", 1,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
      "handle", tempCpuSignal,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CALLS,
      "handle", tempCalls.handle,
      "optionalHandle2", tempCalls.memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  RedStatus queuePresentStatus = RED_STATUS_SUCCESS;
  RedStatuses queuePresentStatuses = {0};
  np(redQueuePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "waitForAndUnsignalGpuSignalsCount", 1,
    "waitForAndUnsignalGpuSignals", &vkfast->presentGpuSignalSubmit,
    "presentsCount", 1,
    "presents", &vkfast->present,
    "presentsImageIndex", &presentImageIndex,
    "outPresentsStatus", &queuePresentStatus,
    "outStatuses", &queuePresentStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(queuePresentStatus == RED_STATUS_SUCCESS || queuePresentStatus == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE || queuePresentStatus == RED_STATUS_PRESENT_IS_SUBOPTIMAL);
  REDGPU_2_EXPECTWG(queuePresentStatuses.status == RED_STATUS_SUCCESS || queuePresentStatuses.status == RED_STATUS_PRESENT_IS_SUBOPTIMAL);
  REDGPU_2_EXPECTWG(queuePresentStatuses.statusError == RED_STATUS_SUCCESS || queuePresentStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE);
  if (queuePresentStatus == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE || queuePresentStatus == RED_STATUS_PRESENT_IS_SUBOPTIMAL || queuePresentStatuses.status == RED_STATUS_PRESENT_IS_SUBOPTIMAL || queuePresentStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE) {
    int isRebuilded = vfInternalRebuildPresent(context, vkfast->presentVsyncMode, vkfast->presentImagesCount, optionalFile, optionalLine);
    return isRebuilded;
  }

  int isRebuilded = 0;
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfDrawPixels(gpu_handle_context_t context, const void * pixels, int * out_optional_internal_present_image_index, unsigned gpu_threads_count_plus_one_empty, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  RedStructMemberArray presentPixels_storage_raw = {0};
  presentPixels_storage_raw.array = vkfast->presentPixelsCpuUpload_memory_and_array.array.handle;
  presentPixels_storage_raw.arrayRangeBytesFirst = 0;
  presentPixels_storage_raw.arrayRangeBytesCount = vkfast->presentPixelsCpuUpload_memory_allocation_size;

  int isRebuilded = vfInternalAsyncDrawPixels(context, &presentPixels_storage_raw, pixels, out_optional_internal_present_image_index, 0, NULL, gpu_threads_count_plus_one_empty, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfAsyncDrawPixels(gpu_handle_context_t context, uint64_t pixels_storage_id, int * out_optional_internal_present_image_index, unsigned gpu_threads_count_plus_one_empty, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  vf_handle_t * storage = (vf_handle_t *)(void *)pixels_storage_id;
  vf_handle_context_t * vkfast = storage->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);
  int isRebuilded = vfInternalAsyncDrawPixels(context, &storage->storage.arrayRangeInfo, NULL, out_optional_internal_present_image_index, 0, NULL, gpu_threads_count_plus_one_empty, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfAsyncDrawPixelsRaw(gpu_handle_context_t context, const RedStructMemberArray * pixels_storage_raw, int * out_optional_internal_present_image_index, unsigned gpu_threads_count_plus_one_empty, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  int isRebuilded = vfInternalAsyncDrawPixels(context, pixels_storage_raw, NULL, out_optional_internal_present_image_index, 0, NULL, gpu_threads_count_plus_one_empty, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE int GPU_API_POST vfAsyncDrawImageRaw(gpu_handle_context_t context, RedHandleImage image_raw, int * out_optional_is_image_copy_finished_cpu_signal_index, unsigned gpu_threads_count_plus_one_empty, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optionalFile, int optionalLine) {
  int isRebuilded = vfInternalAsyncDrawPixels(context, NULL, NULL, out_optional_is_image_copy_finished_cpu_signal_index, 1, image_raw, gpu_threads_count_plus_one_empty, gpu_threads, gpu_threads_array_of_65536_int_values, optionalFile, optionalLine);
  return isRebuilded;
}
#endif

#if !defined(VKFAST_DISABLE_WIN32)
GPU_API_PRE RedHandleCpuSignal GPU_API_POST vfAsyncDrawGetCpuSignal(gpu_handle_context_t context) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  // NOTE(Constantine):
  // Do not destroy and do not unsignal this CPU signal on the user side.
  return vkfast->presentCpuSignal;
}
#endif

GPU_API_PRE void GPU_API_POST vfAllQueuesWaitIdle(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  // NOTE(Constantine): All queues wait idle
  for (unsigned i = 0; i < vkfast->gpuInfo->queuesCount; i += 1) {
    np(redQueuePresent,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->gpuInfo->queues[i],
      "waitForAndUnsignalGpuSignalsCount", 0,
      "waitForAndUnsignalGpuSignals", NULL,
      "presentsCount", 0,
      "presents", NULL,
      "presentsImageIndex", NULL,
      "outPresentsStatus", NULL,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
}
