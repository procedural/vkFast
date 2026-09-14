#if defined(_WIN32)
#define REDGPU_OS_WINDOWS
#endif
#if defined(__linux__) && !defined(__ANDROID__)
#define REDGPU_OS_LINUX
#endif
#ifdef __ANDROID__
#define REDGPU_OS_ANDROID
#endif

#ifdef REDGPU_OS_WINDOWS
#define REDGPU_32_DECLSPEC __declspec(dllexport)
#define REDGPU_32_API
#endif
#ifdef REDGPU_OS_LINUX
#define REDGPU_32_DECLSPEC __attribute__((visibility("default")))
#define REDGPU_32_API
#endif

#include "redgpu_2.h"
#include "redgpu_32.h"

#include <stddef.h>   // For size_t
#include <stdint.h>   // For uint64_t
#ifdef REDGPU_OS_WINDOWS
#include <windows.h>
#endif
#ifdef REDGPU_OS_LINUX
#include <stdio.h>    // For fprintf
#include <X11/Xlib.h> // For X11 Display, Window
#include <fcntl.h>    // For O_RDONLY
#include <sys/stat.h> // For fstat
#include <sys/mman.h> // For mmap
#include <unistd.h>   // For close
#endif

#define STB_SPRINTF_IMPLEMENTATION
#include "redgpu_stb_sprintf.h"

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemorySet(void * pointer, int value, size_t bytesCount) {
  uint8_t * setTo = (uint8_t *)pointer;
  for (size_t i = 0; i < bytesCount; i += 1) {
    setTo[i] = value;
  }
  return pointer;
}

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemoryCopy(void * pointerCopyTo, const void * pointerCopyFrom, size_t bytesCount) {
  uint8_t *       copyTo   = (uint8_t *)pointerCopyTo;
  const uint8_t * copyFrom = (const uint8_t *)pointerCopyFrom;
  for (size_t i = 0; i < bytesCount; i += 1) {
    copyTo[i] = copyFrom[i];
  }
  return pointerCopyTo;
}

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemoryCallocAligned(size_t bytesCount, size_t alignment) {
  uint8_t * pointer        = (uint8_t *)calloc(1, (alignment == 1 ? 0 : alignment) + bytesCount + REDGPU_32_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(bytesCount, alignment));
  uint64_t  pointerAddress = (uint64_t)(void *)pointer;
  pointer += REDGPU_32_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(pointerAddress, alignment);
  return (void *)pointer;
}

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemoryCalloc(size_t bytesCount) {
  return red32MemoryCallocAligned(bytesCount, 1);
}

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemoryReallocAligned(void * pointer, size_t newSizeBytesCount, size_t oldSizeBytesCount, size_t alignment) {
  void * newpointer = red32MemoryCallocAligned(newSizeBytesCount, alignment);
  if (pointer == NULL || newpointer == NULL) {
    return newpointer;
  }
  if (oldSizeBytesCount > newSizeBytesCount) {
      oldSizeBytesCount = newSizeBytesCount;
  }
  red32MemoryCopy(newpointer, pointer, oldSizeBytesCount);
  red32MemoryFree(pointer);
  return newpointer;
}

REDGPU_32_DECLSPEC void * REDGPU_32_API red32MemoryRealloc(void * pointer, size_t newSizeBytesCount, size_t oldSizeBytesCount) {
  return red32MemoryReallocAligned(pointer, newSizeBytesCount, oldSizeBytesCount, 1);
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32MemoryFree(void * pointer) {
  free(pointer);
}

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC void * REDGPU_32_API red32GetModuleHandle(const char * moduleName) {
  return GetModuleHandleA(moduleName);
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC void * REDGPU_32_API red32WindowCreate(const char * title) {
  WNDCLASSEXA wndClassEx = {0};
  wndClassEx.cbSize        = sizeof(wndClassEx);
  wndClassEx.lpfnWndProc   = DefWindowProcA;
  wndClassEx.lpszClassName = title;
  RegisterClassExA(&wndClassEx);
  HWND window = CreateWindowExA(0, title, title, WS_POPUP | WS_MAXIMIZE | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0);
  return (void *)window;
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC void * REDGPU_32_API red32WindowCreate(const char * title) {
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  // To free
  struct X11WindowData * h = malloc(sizeof(struct X11WindowData));

  // 1. Establish connection to the X Server
  h->display = XOpenDisplay(NULL);
  REDGPU_2_EXPECTFL(h->display != NULL);

  int screen = DefaultScreen(h->display);

  // 2. Create the window
  h->window = XCreateSimpleWindow(
    h->display,
    RootWindow(h->display, screen),    // Parent window
    0, 0,                              // Initial X, Y position
    DisplayWidth(h->display, screen),  // Width
    DisplayHeight(h->display, screen), // Height
    1,                                 // Border width
    BlackPixel(h->display, screen),    // Border color
    WhitePixel(h->display, screen)     // Background color
  );
  REDGPU_2_EXPECTFL(h->window != 0);

  // 2.1. Borderless fullscreen
  if (0) // NOTE(Constantine): Nah, this 'borderless fullscreen' method can't be closed with Alt + F4 on Bazzite/SteamOS, have to Ctrl + Alt + Delete to log out of the user session, lol.
  {
    // 2.1 Set X11 window to borderless fullscreen
    XSetWindowAttributes attributes = {0};
    attributes.override_redirect = True;
    XChangeWindowAttributes(h->display, h->window, CWOverrideRedirect, &attributes);
    XFlush(h->display);
  }
  if (0) // NOTE(Constantine): Okay, this 'borderless fullscreen' method can't hide the taskbar on Bazzite/SteamOS, but you can hide it yourself via Right Mouse Button click on the taskbar -> 'Show Panel Configuration' -> 'Visibility' -> change 'Always visible' to 'Dodge windows'.
  {
    struct MotifWmHints {
      unsigned long flags;
      unsigned long functions;
      unsigned long decorations;
      long input_mode;
      unsigned long status;
    };

    Atom motif_hints = XInternAtom(h->display, "_MOTIF_WM_HINTS", False);
    struct MotifWmHints hints = {0};
    hints.flags = 2; // MWM_HINTS_DECORATIONS
    hints.decorations = 0; // No borders/title bar
    XChangeProperty(h->display, h->window, motif_hints, motif_hints, 32, PropModeReplace, (unsigned char *)&hints, 5);
  }

  // 2.2. Set the window title
  XStoreName(h->display, h->window, title);

  // 3. Register for input events (KeyPress and Window Exposure)
  XSelectInput(h->display, h->window, ExposureMask | KeyPressMask);

  // 4. Map the window to the screen (make it visible)
  XMapWindow(h->display, h->window);
  XRaiseWindow(h->display, h->window);
  if (1) // NOTE(Constantine): Okay, this 'borderless fullscreen' method works as it should on Bazzite/SteamOS! But only when its code is placed exactly here.
  {
    Atom wm_state = XInternAtom(h->display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(h->display, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev = {0};
    xev.type = ClientMessage;
    xev.xclient.window = h->window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1; // 1 = _NET_WM_STATE_ADD
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0; // No second property
    xev.xclient.data.l[3] = 0; // Source indication
    xev.xclient.data.l[4] = 0;
    XSendEvent(h->display, DefaultRootWindow(h->display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  }
  XFlush(h->display);

  // 5. Setup window closure handling protocol
  h->wmDeleteMessage = XInternAtom(h->display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(h->display, h->window, &h->wmDeleteMessage, 1);

  return h;
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC int REDGPU_32_API red32WindowDestroy(void * windowHandle) {
  if (windowHandle != NULL) {
    int destroyWindowSuccess = DestroyWindow((HWND)windowHandle);
    if (destroyWindowSuccess == 0) { // "If the function fails, the return value is zero." https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroywindow
      return -1;
    }
  }
  return 0;
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC int REDGPU_32_API red32WindowDestroy(void * windowHandle) {
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData * h = windowHandle;

  XDestroyWindow(h->display, h->window);
  XCloseDisplay(h->display);

  free(h);

  return 0;
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC int REDGPU_32_API red32WindowLoop(void * windowHandle) {
  int loop = 1;
  MSG msg = {0};
  while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT || msg.message == WM_CLOSE || msg.message == WM_DESTROY || msg.message == WM_SYSCOMMAND) {
      loop = 0;
    }
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
  return loop;
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC int REDGPU_32_API red32WindowLoop(void * windowHandle) {
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData * h = windowHandle;

  int loop = 1;
  XEvent event = {0};
  while (XPending(h->display)) {
    XNextEvent(h->display, &event);

    // Handle window redrawing
    if (event.type == Expose) {
      // Draw graphics or text here if desired
    }

    // Handle keyboard press
    if (event.type == KeyPress) {
      // Handle any keypresses here if desired
    }

    // Handle the window 'X' close button
    if (event.type == ClientMessage) {
      if (event.xclient.data.l[0] == h->wmDeleteMessage) {
        loop = 0;
      }
    }
  }
  return loop;
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC void REDGPU_32_API red32ConsolePrint(const char * string) {
  DWORD _;
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, strlen(string), &_, 0);
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC void REDGPU_32_API red32ConsolePrint(const char * string) {
  fprintf(stdout, "%s", string);
  fflush(stdout);
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC void REDGPU_32_API red32ConsolePrintError(const char * string) {
  DWORD _;
  WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), string, strlen(string), &_, 0);
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC void REDGPU_32_API red32ConsolePrintError(const char * string) {
  fprintf(stderr, "%s", string);
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC int REDGPU_32_API red32FileMap(const unsigned short * filepath, void ** outFileDescriptorHandle, void ** outFileMappingHandle, void ** outFileDataPointer) {
  HANDLE fd = CreateFileW((LPCWSTR)filepath, FILE_READ_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (fd == INVALID_HANDLE_VALUE) {
    return -1;
  }
  if (outFileDescriptorHandle != NULL) {
    outFileDescriptorHandle[0] = fd;
  }
  HANDLE fm = CreateFileMappingA(fd, 0, PAGE_READONLY, 0, 0, 0);
  if (fm == INVALID_HANDLE_VALUE) {
    return -2;
  }
  if (outFileMappingHandle != NULL) {
    outFileMappingHandle[0] = fm;
  }
  void * fdata = MapViewOfFile(fm, FILE_MAP_READ, 0, 0, 0);
  if (outFileDataPointer != NULL) {
    outFileDataPointer[0] = fdata;
  }
  return 0;
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC int REDGPU_32_API red32FileMap(const unsigned short * _filepath, void ** outFileDescriptorHandle, void ** outFileMappingHandle, void ** outFileDataPointer) {
  const char * filepath = (const char *)_filepath;

  int fd = open(filepath, O_RDONLY);
  if (fd == -1) {
    return -1;
  }
  if (outFileDescriptorHandle != NULL) {
    outFileDescriptorHandle[0] = (void *)(int64_t)fd;
  }

  struct stat sb = {0};
  if (fstat(fd, &sb) == -1) {
    return -2;
  }
  if (sb.st_size == 0) {
    return -2;
  }
  if (outFileMappingHandle != NULL) {
    outFileMappingHandle[0] = (void *)sb.st_size;
  }

  // PROT_READ: memory can be read
  // MAP_PRIVATE: modifications are private and not written to disk
  void * fdata = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (fdata == MAP_FAILED) {
    if (outFileDataPointer != NULL) {
      outFileDataPointer[0] = MAP_FAILED;
    }
    return -3;
  }
  if (outFileDataPointer != NULL) {
    outFileDataPointer[0] = fdata;
  }
  return 0;
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC int REDGPU_32_API red32FileUnmap(void * fileHandle, void * fileMappingDescriptorHandle, void * fileMapping) {
  if (fileMapping != NULL) {
    int unmapViewOfFileSuccess = UnmapViewOfFile(fileMapping);
    if (unmapViewOfFileSuccess == 0) { // "If the function fails, the return value is zero." https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
      return -1;
    }
  }
  if (fileMappingDescriptorHandle != INVALID_HANDLE_VALUE) {
    BOOL closeHandleSuccess = CloseHandle(fileMappingDescriptorHandle);
    if (closeHandleSuccess == 0) {     // "If the function fails, the return value is zero." https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
      return -2;
    }
  }
  if (fileHandle != INVALID_HANDLE_VALUE) {
    BOOL closeHandleSuccess = CloseHandle(fileHandle);
    if (closeHandleSuccess == 0) {     // "If the function fails, the return value is zero." https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
      return -3;
    }
  }
  return 0;
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC int REDGPU_32_API red32FileUnmap(void * fileHandle, void * fileMappingDescriptorHandle, void * fileMapping) {
  if (fileMapping != MAP_FAILED) {
    // NOTE(Constantine): a valid fileMappingDescriptorHandle value must be passed from red32FileMap() for munmap().
    off_t sb_st_size = (off_t)fileMappingDescriptorHandle;
    if (sb_st_size == 0) {
      return -2;
    }
    if (munmap(fileMapping, sb_st_size) == -1) { // "Upon successful completion, munmap() shall return 0; otherwise, it shall return -1 and set errno to indicate the error." https://man7.org/linux/man-pages/man3/munmap.3p.html
      return -1;
    }
  }
  if ((int)(int64_t)fileHandle >= 0) {
    if (close((int)(int64_t)fileHandle) == -1) { // "close() returns zero on success.  On error, -1 is returned, and errno is set to indicate the error." https://man7.org/linux/man-pages/man2/close.2.html
      return -3;
    }
  }
  return 0;
}
#endif

#ifdef REDGPU_OS_WINDOWS
REDGPU_32_DECLSPEC void REDGPU_32_API red32OutputDebugString(const char * string) {
  OutputDebugStringA(string);
}
#endif

#ifdef REDGPU_OS_LINUX
REDGPU_32_DECLSPEC void REDGPU_32_API red32OutputDebugString(const char * string) {
  // NOTE(Constantine): does nothing, this function is for Windows only.
}
#endif

REDGPU_32_DECLSPEC void REDGPU_32_API red32Exit(int exitCode) {
  exit(exitCode);
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32IntToChars(int value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%d", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%d", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32UnsignedToChars(unsigned value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%u", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%u", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32Int64ToChars(int64_t value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%lld", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%lld", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32Uint64ToChars(uint64_t value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%llu", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%llu", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32FloatToChars(float value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%.9g", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%.9g", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC int REDGPU_32_API red32DoubleToChars(double value, char * outChars) {
  int size = stbsp_snprintf(0, 0, "%.17g", value);
  if (outChars != 0) {
    stbsp_snprintf(outChars, size + 1, "%.17g", value);
  }
  return size + 1;
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32BinaryUint8ToCharsNoNullTerm(uint8_t value, char * outCharsNoNullTerm) {
  uint8_t b = value;
  outCharsNoNullTerm[0]  = (b & 0b10000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[1]  = (b & 0b01000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[2]  = (b & 0b00100000) == 0 ? '0' : '1';
  outCharsNoNullTerm[3]  = (b & 0b00010000) == 0 ? '0' : '1';
  outCharsNoNullTerm[4]  = (b & 0b00001000) == 0 ? '0' : '1';
  outCharsNoNullTerm[5]  = (b & 0b00000100) == 0 ? '0' : '1';
  outCharsNoNullTerm[6]  = (b & 0b00000010) == 0 ? '0' : '1';
  outCharsNoNullTerm[7]  = (b & 0b00000001) == 0 ? '0' : '1';
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32BinaryUnsignedToCharsNoNullTerm(unsigned value, char * outCharsNoNullTerm) {
  unsigned b = value;
  outCharsNoNullTerm[0]  = (b & 0b10000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[1]  = (b & 0b01000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[2]  = (b & 0b00100000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[3]  = (b & 0b00010000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[4]  = (b & 0b00001000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[5]  = (b & 0b00000100000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[6]  = (b & 0b00000010000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[7]  = (b & 0b00000001000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[8]  = (b & 0b00000000100000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[9]  = (b & 0b00000000010000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[10] = (b & 0b00000000001000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[11] = (b & 0b00000000000100000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[12] = (b & 0b00000000000010000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[13] = (b & 0b00000000000001000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[14] = (b & 0b00000000000000100000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[15] = (b & 0b00000000000000010000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[16] = (b & 0b00000000000000001000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[17] = (b & 0b00000000000000000100000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[18] = (b & 0b00000000000000000010000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[19] = (b & 0b00000000000000000001000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[20] = (b & 0b00000000000000000000100000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[21] = (b & 0b00000000000000000000010000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[22] = (b & 0b00000000000000000000001000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[23] = (b & 0b00000000000000000000000100000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[24] = (b & 0b00000000000000000000000010000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[25] = (b & 0b00000000000000000000000001000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[26] = (b & 0b00000000000000000000000000100000) == 0 ? '0' : '1';
  outCharsNoNullTerm[27] = (b & 0b00000000000000000000000000010000) == 0 ? '0' : '1';
  outCharsNoNullTerm[28] = (b & 0b00000000000000000000000000001000) == 0 ? '0' : '1';
  outCharsNoNullTerm[29] = (b & 0b00000000000000000000000000000100) == 0 ? '0' : '1';
  outCharsNoNullTerm[30] = (b & 0b00000000000000000000000000000010) == 0 ? '0' : '1';
  outCharsNoNullTerm[31] = (b & 0b00000000000000000000000000000001) == 0 ? '0' : '1';
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32BinaryUint64ToCharsNoNullTerm(uint64_t value, char * outCharsNoNullTerm) {
  uint64_t b = value;
  outCharsNoNullTerm[0]  = (b & 0b1000000000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[1]  = (b & 0b0100000000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[2]  = (b & 0b0010000000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[3]  = (b & 0b0001000000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[4]  = (b & 0b0000100000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[5]  = (b & 0b0000010000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[6]  = (b & 0b0000001000000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[7]  = (b & 0b0000000100000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[8]  = (b & 0b0000000010000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[9]  = (b & 0b0000000001000000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[10] = (b & 0b0000000000100000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[11] = (b & 0b0000000000010000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[12] = (b & 0b0000000000001000000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[13] = (b & 0b0000000000000100000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[14] = (b & 0b0000000000000010000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[15] = (b & 0b0000000000000001000000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[16] = (b & 0b0000000000000000100000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[17] = (b & 0b0000000000000000010000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[18] = (b & 0b0000000000000000001000000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[19] = (b & 0b0000000000000000000100000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[20] = (b & 0b0000000000000000000010000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[21] = (b & 0b0000000000000000000001000000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[22] = (b & 0b0000000000000000000000100000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[23] = (b & 0b0000000000000000000000010000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[24] = (b & 0b0000000000000000000000001000000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[25] = (b & 0b0000000000000000000000000100000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[26] = (b & 0b0000000000000000000000000010000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[27] = (b & 0b0000000000000000000000000001000000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[28] = (b & 0b0000000000000000000000000000100000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[29] = (b & 0b0000000000000000000000000000010000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[30] = (b & 0b0000000000000000000000000000001000000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[31] = (b & 0b0000000000000000000000000000000100000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[32] = (b & 0b0000000000000000000000000000000010000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[33] = (b & 0b0000000000000000000000000000000001000000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[34] = (b & 0b0000000000000000000000000000000000100000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[35] = (b & 0b0000000000000000000000000000000000010000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[36] = (b & 0b0000000000000000000000000000000000001000000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[37] = (b & 0b0000000000000000000000000000000000000100000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[38] = (b & 0b0000000000000000000000000000000000000010000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[39] = (b & 0b0000000000000000000000000000000000000001000000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[40] = (b & 0b0000000000000000000000000000000000000000100000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[41] = (b & 0b0000000000000000000000000000000000000000010000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[42] = (b & 0b0000000000000000000000000000000000000000001000000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[43] = (b & 0b0000000000000000000000000000000000000000000100000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[44] = (b & 0b0000000000000000000000000000000000000000000010000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[45] = (b & 0b0000000000000000000000000000000000000000000001000000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[46] = (b & 0b0000000000000000000000000000000000000000000000100000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[47] = (b & 0b0000000000000000000000000000000000000000000000010000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[48] = (b & 0b0000000000000000000000000000000000000000000000001000000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[49] = (b & 0b0000000000000000000000000000000000000000000000000100000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[50] = (b & 0b0000000000000000000000000000000000000000000000000010000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[51] = (b & 0b0000000000000000000000000000000000000000000000000001000000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[52] = (b & 0b0000000000000000000000000000000000000000000000000000100000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[53] = (b & 0b0000000000000000000000000000000000000000000000000000010000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[54] = (b & 0b0000000000000000000000000000000000000000000000000000001000000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[55] = (b & 0b0000000000000000000000000000000000000000000000000000000100000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[56] = (b & 0b0000000000000000000000000000000000000000000000000000000010000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[57] = (b & 0b0000000000000000000000000000000000000000000000000000000001000000) == 0 ? '0' : '1';
  outCharsNoNullTerm[58] = (b & 0b0000000000000000000000000000000000000000000000000000000000100000) == 0 ? '0' : '1';
  outCharsNoNullTerm[59] = (b & 0b0000000000000000000000000000000000000000000000000000000000010000) == 0 ? '0' : '1';
  outCharsNoNullTerm[60] = (b & 0b0000000000000000000000000000000000000000000000000000000000001000) == 0 ? '0' : '1';
  outCharsNoNullTerm[61] = (b & 0b0000000000000000000000000000000000000000000000000000000000000100) == 0 ? '0' : '1';
  outCharsNoNullTerm[62] = (b & 0b0000000000000000000000000000000000000000000000000000000000000010) == 0 ? '0' : '1';
  outCharsNoNullTerm[63] = (b & 0b0000000000000000000000000000000000000000000000000000000000000001) == 0 ? '0' : '1';
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32HexUint8ToCharsNoNullTerm(uint8_t value, char * outCharsNoNullTerm) {
  const char * hexTable[256] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
  };
  const char * hex0 = hexTable[value];
  outCharsNoNullTerm[0] = hex0[0];
  outCharsNoNullTerm[1] = hex0[1];
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32HexUnsignedToCharsNoNullTerm(unsigned value, char * outCharsNoNullTerm) {
  const char * hexTable[256] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
  };
  unsigned char * vs    = (unsigned char *)(void *)&value;
  unsigned char   v0    = vs[0];
  unsigned char   v1    = vs[1];
  unsigned char   v2    = vs[2];
  unsigned char   v3    = vs[3];
  const char *    hex0  = hexTable[v0];
  const char *    hex1  = hexTable[v1];
  const char *    hex2  = hexTable[v2];
  const char *    hex3  = hexTable[v3];
  outCharsNoNullTerm[0] = hex3[0];
  outCharsNoNullTerm[1] = hex3[1];
  outCharsNoNullTerm[2] = hex2[0];
  outCharsNoNullTerm[3] = hex2[1];
  outCharsNoNullTerm[4] = hex1[0];
  outCharsNoNullTerm[5] = hex1[1];
  outCharsNoNullTerm[6] = hex0[0];
  outCharsNoNullTerm[7] = hex0[1];
}

REDGPU_32_DECLSPEC void REDGPU_32_API red32HexUint64ToCharsNoNullTerm(uint64_t value, char * outCharsNoNullTerm) {
  const char * hexTable[256] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
  };
  unsigned char * vs   = (unsigned char *)(void *)&value;
  unsigned char   v0   = vs[0];
  unsigned char   v1   = vs[1];
  unsigned char   v2   = vs[2];
  unsigned char   v3   = vs[3];
  unsigned char   v4   = vs[4];
  unsigned char   v5   = vs[5];
  unsigned char   v6   = vs[6];
  unsigned char   v7   = vs[7];
  const char *    hex0 = hexTable[v0];
  const char *    hex1 = hexTable[v1];
  const char *    hex2 = hexTable[v2];
  const char *    hex3 = hexTable[v3];
  const char *    hex4 = hexTable[v4];
  const char *    hex5 = hexTable[v5];
  const char *    hex6 = hexTable[v6];
  const char *    hex7 = hexTable[v7];
  outCharsNoNullTerm[0]  = hex7[0];
  outCharsNoNullTerm[1]  = hex7[1];
  outCharsNoNullTerm[2]  = hex6[0];
  outCharsNoNullTerm[3]  = hex6[1];
  outCharsNoNullTerm[4]  = hex5[0];
  outCharsNoNullTerm[5]  = hex5[1];
  outCharsNoNullTerm[6]  = hex4[0];
  outCharsNoNullTerm[7]  = hex4[1];
  outCharsNoNullTerm[8]  = hex3[0];
  outCharsNoNullTerm[9]  = hex3[1];
  outCharsNoNullTerm[10] = hex2[0];
  outCharsNoNullTerm[11] = hex2[1];
  outCharsNoNullTerm[12] = hex1[0];
  outCharsNoNullTerm[13] = hex1[1];
  outCharsNoNullTerm[14] = hex0[0];
  outCharsNoNullTerm[15] = hex0[1];
}
