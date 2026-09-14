#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(_WIN64)
typedef uint64_t                      RedHandleSurface;
typedef uint64_t                      RedHandlePresent;
#else
typedef struct RedTypeHandleSurface * RedHandleSurface;
typedef struct RedTypeHandlePresent * RedHandlePresent;
#endif

// redCreatePresent

typedef unsigned RedSurfaceTransformBitflags;
typedef enum RedSurfaceTransformBitflag {
  RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY                     = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001),
  RED_SURFACE_TRANSFORM_BITFLAG_ROTATE_90                    = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_SURFACE_TRANSFORM_BITFLAG_ROTATE_180                   = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
  RED_SURFACE_TRANSFORM_BITFLAG_ROTATE_270                   = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_SURFACE_TRANSFORM_BITFLAG_HORIZONTAL_MIRROR            = REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000),
  RED_SURFACE_TRANSFORM_BITFLAG_HORIZONTAL_MIRROR_ROTATE_90  = REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000),
  RED_SURFACE_TRANSFORM_BITFLAG_HORIZONTAL_MIRROR_ROTATE_180 = REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000),
  RED_SURFACE_TRANSFORM_BITFLAG_HORIZONTAL_MIRROR_ROTATE_270 = REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000),
  RED_SURFACE_TRANSFORM_BITFLAG_INHERIT                      = REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000),
} RedSurfaceTransformBitflag;

typedef unsigned RedSurfaceCompositeAlphaBitflags;
typedef enum RedSurfaceCompositeAlphaBitflag {
  RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001),
  RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_PRE_MULTIPLIED  = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_POST_MULTIPLIED = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
  RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_INHERIT         = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
} RedSurfaceCompositeAlphaBitflag;

typedef enum RedPresentVsyncMode {
  RED_PRESENT_VSYNC_MODE_OFF        = 0,
  RED_PRESENT_VSYNC_MODE_ON         = 2,
  RED_PRESENT_VSYNC_MODE_ON_RELAXED = 3,
} RedPresentVsyncMode;

// redQueueFamilyIndexGetSupportsPresent

typedef struct RedQueueFamilyIndexGetSupportsPresentOnWin32 {
  RedBool32        outQueueFamilyIndexSupportsPresentOnWin32;
} RedQueueFamilyIndexGetSupportsPresentOnWin32;

typedef struct RedQueueFamilyIndexGetSupportsPresentOnXlib {
  const void *     display;
  unsigned long    visualId;
  RedBool32        outQueueFamilyIndexSupportsPresentOnXlib;
} RedQueueFamilyIndexGetSupportsPresentOnXlib;

typedef struct RedQueueFamilyIndexGetSupportsPresentOnXcb {
  const void *     connection;
  unsigned         visualId;
  RedBool32        outQueueFamilyIndexSupportsPresentOnXcb;
} RedQueueFamilyIndexGetSupportsPresentOnXcb;

typedef struct RedQueueFamilyIndexGetSupportsPresentOnSurface {
  RedHandleSurface surface;
  RedBool32        outQueueFamilyIndexSupportsPresentOnSurface;
} RedQueueFamilyIndexGetSupportsPresentOnSurface;

// redSurfaceGetPresentFeatures

typedef struct RedSurfacePresentFeatures {
  RedBool32 supportsPresentVsyncModeOff;
  RedBool32 supportsPresentVsyncModeOn;        // Guaranteed to be supported
  RedBool32 supportsPresentVsyncModeOnRelaxed;
} RedSurfacePresentFeatures;

// redSurfaceGetCurrentPropertiesAndPresentLimits

typedef struct RedSurfaceCurrentPropertiesAndPresentLimits {
  unsigned                         currentSurfaceWidth;     // The current width  of the surface, or the special value 0xFFFFFFFF indicating that the surface width  will be determined by the width  of a present targeting the surface
  unsigned                         currentSurfaceHeight;    // The current height of the surface, or the special value 0xFFFFFFFF indicating that the surface height will be determined by the height of a present targeting the surface
  RedSurfaceTransformBitflag       currentSurfaceTransform;
  unsigned                         minPresentImagesCount;
  unsigned                         maxPresentImagesCount;
  unsigned                         minPresentImagesWidth;
  unsigned                         maxPresentImagesWidth;
  unsigned                         minPresentImagesHeight;
  unsigned                         maxPresentImagesHeight;
  unsigned                         maxPresentImagesLayersCount;
  RedBool32                        supportsPresentImagesAccessCopyR;
  RedBool32                        supportsPresentImagesAccessCopyW;
  RedBool32                        supportsPresentImagesAccessTextureRO;
  RedBool32                        supportsPresentImagesAccessTextureRW;
  RedBool32                        supportsPresentImagesAccessOutputDepthStencil;
  RedBool32                        supportsPresentImagesAccessOutputColor;        // Guaranteed to be supported
  RedSurfaceTransformBitflags      supportedPresentTransforms;
  RedSurfaceCompositeAlphaBitflags supportedPresentCompositeAlphas;
} RedSurfaceCurrentPropertiesAndPresentLimits;

// Create

REDGPU_DECLSPEC void REDGPU_API redCreateSurfaceWin32     (RedContext context, RedHandleGpu gpu, const char * handleName, const void * win32Hinstance, const void * win32Hwnd, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redCreateSurfaceXlibOrXcb (RedContext context, RedHandleGpu gpu, const char * handleName, const void * xlibDisplay, unsigned long xlibWindow, const void * xcbConnection, unsigned xcbWindow, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redCreatePresent          (RedContext context, RedHandleGpu gpu, RedHandleQueue queue, const char * handleName, RedHandleSurface surface, unsigned imagesCount, unsigned imagesWidth, unsigned imagesHeight, unsigned imagesLayersCount, RedAccessBitflags imagesRestrictToAccess, RedSurfaceTransformBitflag transform, RedSurfaceCompositeAlphaBitflag compositeAlpha, RedPresentVsyncMode vsyncMode, RedBool32 clipped, RedBool32 discardAfterPresent, RedHandlePresent oldPresent, RedHandlePresent * outPresent, RedHandleImage * outImages, RedHandleTexture * outTextures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Destroy

REDGPU_DECLSPEC void REDGPU_API redDestroySurface         (RedContext context, RedHandleGpu gpu, RedHandleSurface surface, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redDestroyPresent         (RedContext context, RedHandleGpu gpu, RedHandlePresent present, const char * optionalFile, int optionalLine, void * optionalUserData);

// Queue family index

REDGPU_DECLSPEC void REDGPU_API redQueueFamilyIndexGetSupportsPresent          (RedContext context, RedHandleGpu gpu, unsigned queueFamilyIndex, RedQueueFamilyIndexGetSupportsPresentOnWin32 * supportsPresentOnWin32, RedQueueFamilyIndexGetSupportsPresentOnXlib * supportsPresentOnXlib, RedQueueFamilyIndexGetSupportsPresentOnXcb * supportsPresentOnXcb, RedQueueFamilyIndexGetSupportsPresentOnSurface * supportsPresentOnSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Surface

REDGPU_DECLSPEC void REDGPU_API redSurfaceGetPresentFeatures                   (RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfacePresentFeatures * outSurfacePresentFeatures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redSurfaceGetCurrentPropertiesAndPresentLimits (RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfaceCurrentPropertiesAndPresentLimits * outSurfaceCurrentPropertiesAndPresentLimits, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Present

REDGPU_DECLSPEC void REDGPU_API redPresentGetImageIndex   (RedContext context, RedHandleGpu gpu, RedHandlePresent present, RedHandleCpuSignal signalCpuSignal, RedHandleGpuSignal signalGpuSignal, unsigned * outImageIndex, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Queue

REDGPU_DECLSPEC void REDGPU_API redQueuePresent           (RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned waitForAndUnsignalGpuSignalsCount, const RedHandleGpuSignal * waitForAndUnsignalGpuSignals, unsigned presentsCount, const RedHandlePresent * presents, const unsigned * presentsImageIndex, RedStatus * outPresentsStatus, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

#ifdef __cplusplus
}
#endif

// Additional notes:
// * redPresentGetImageIndex may or may not block.
// * redQueuePresent         may or may not block.
// * If redPresentGetImageIndex won't block implicitly, you can block explicitly on a CPU signal.
// * Since Dec 01, 2022, you can set redCreatePresent::imagesCount to max value (0xFFFFFFFF), pass to redCreatePresent::outImages an array of count RedSurfaceCurrentPropertiesAndPresentLimits::maxPresentImagesCount and pass to redCreatePresent::outTextures either a null pointer or an array of count RedSurfaceCurrentPropertiesAndPresentLimits::maxPresentImagesCount for the graphics driver to return a system-preferable number of images and textures between RedSurfaceCurrentPropertiesAndPresentLimits::minPresentImagesCount and RedSurfaceCurrentPropertiesAndPresentLimits::maxPresentImagesCount.
// * Since Dec 01, 2022, redQueuePresent called with redQueuePresent::waitForAndUnsignalGpuSignalsCount set to 0 and redQueuePresent::presentsCount set to 0 does nothing but waits for the redQueuePresent::queue to become idle. This partially addresses the issue #1678.
