#define REDGPU_PROGRAM_PRESENT_IMAGES_COUNT               2
#define REDGPU_PROGRAM_PRESENT_VSYNC_MODE                 RED_PRESENT_VSYNC_MODE_ON
#define REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES 1

#ifndef REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH
#define REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH 4096
#endif

#ifndef REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT
#define REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT 16
#endif

#ifndef REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT
#define REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT 128
#endif

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "C:/RedGpuSDK/redgpu.h"
#include "C:/RedGpuSDK/redgpu_wsi.h"

#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

#ifdef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#endif

#include <malloc.h>    // For malloc, free
#include <stdio.h>     // For printf, fflush
#include <assert.h>    // For assert
#include <shlwapi.h>   // For PathIsDirectory
#include <math.h>      // For pow
#include "stb_image.h"
#ifdef _WIN32
#pragma comment(lib, "shlwapi.lib")
#endif

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof((array)[0]))
#endif

enum {
  GLOBAL_SPONZA_MESHES_SUBMESH_00,
  GLOBAL_SPONZA_MESHES_SUBMESH_01,
  GLOBAL_SPONZA_MESHES_SUBMESH_01_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_03,
  GLOBAL_SPONZA_MESHES_SUBMESH_05,
  GLOBAL_SPONZA_MESHES_SUBMESH_06,
  GLOBAL_SPONZA_MESHES_SUBMESH_07,
  GLOBAL_SPONZA_MESHES_SUBMESH_08,
  GLOBAL_SPONZA_MESHES_SUBMESH_09,
  GLOBAL_SPONZA_MESHES_SUBMESH_10,
  GLOBAL_SPONZA_MESHES_SUBMESH_100,
  GLOBAL_SPONZA_MESHES_SUBMESH_101,
  GLOBAL_SPONZA_MESHES_SUBMESH_102,
  GLOBAL_SPONZA_MESHES_SUBMESH_103,
  GLOBAL_SPONZA_MESHES_SUBMESH_104,
  GLOBAL_SPONZA_MESHES_SUBMESH_105,
  GLOBAL_SPONZA_MESHES_SUBMESH_106,
  GLOBAL_SPONZA_MESHES_SUBMESH_107,
  GLOBAL_SPONZA_MESHES_SUBMESH_108,
  GLOBAL_SPONZA_MESHES_SUBMESH_109,
  GLOBAL_SPONZA_MESHES_SUBMESH_11,
  GLOBAL_SPONZA_MESHES_SUBMESH_110,
  GLOBAL_SPONZA_MESHES_SUBMESH_111,
  GLOBAL_SPONZA_MESHES_SUBMESH_112,
  GLOBAL_SPONZA_MESHES_SUBMESH_113,
  GLOBAL_SPONZA_MESHES_SUBMESH_114,
  GLOBAL_SPONZA_MESHES_SUBMESH_115,
  GLOBAL_SPONZA_MESHES_SUBMESH_116,
  GLOBAL_SPONZA_MESHES_SUBMESH_117,
  GLOBAL_SPONZA_MESHES_SUBMESH_118,
  GLOBAL_SPONZA_MESHES_SUBMESH_119,
  GLOBAL_SPONZA_MESHES_SUBMESH_12,
  GLOBAL_SPONZA_MESHES_SUBMESH_120,
  GLOBAL_SPONZA_MESHES_SUBMESH_121,
  GLOBAL_SPONZA_MESHES_SUBMESH_122,
  GLOBAL_SPONZA_MESHES_SUBMESH_123,
  GLOBAL_SPONZA_MESHES_SUBMESH_124,
  GLOBAL_SPONZA_MESHES_SUBMESH_125,
  GLOBAL_SPONZA_MESHES_SUBMESH_126,
  GLOBAL_SPONZA_MESHES_SUBMESH_127,
  GLOBAL_SPONZA_MESHES_SUBMESH_128,
  GLOBAL_SPONZA_MESHES_SUBMESH_129,
  GLOBAL_SPONZA_MESHES_SUBMESH_13,
  GLOBAL_SPONZA_MESHES_SUBMESH_130,
  GLOBAL_SPONZA_MESHES_SUBMESH_131,
  GLOBAL_SPONZA_MESHES_SUBMESH_132,
  GLOBAL_SPONZA_MESHES_SUBMESH_133,
  GLOBAL_SPONZA_MESHES_SUBMESH_134,
  GLOBAL_SPONZA_MESHES_SUBMESH_135,
  GLOBAL_SPONZA_MESHES_SUBMESH_136,
  GLOBAL_SPONZA_MESHES_SUBMESH_137,
  GLOBAL_SPONZA_MESHES_SUBMESH_138,
  GLOBAL_SPONZA_MESHES_SUBMESH_139,
  GLOBAL_SPONZA_MESHES_SUBMESH_14,
  GLOBAL_SPONZA_MESHES_SUBMESH_140,
  GLOBAL_SPONZA_MESHES_SUBMESH_141,
  GLOBAL_SPONZA_MESHES_SUBMESH_142,
  GLOBAL_SPONZA_MESHES_SUBMESH_143,
  GLOBAL_SPONZA_MESHES_SUBMESH_144,
  GLOBAL_SPONZA_MESHES_SUBMESH_145,
  GLOBAL_SPONZA_MESHES_SUBMESH_146,
  GLOBAL_SPONZA_MESHES_SUBMESH_147,
  GLOBAL_SPONZA_MESHES_SUBMESH_148,
  GLOBAL_SPONZA_MESHES_SUBMESH_149,
  GLOBAL_SPONZA_MESHES_SUBMESH_15,
  GLOBAL_SPONZA_MESHES_SUBMESH_150,
  GLOBAL_SPONZA_MESHES_SUBMESH_151,
  GLOBAL_SPONZA_MESHES_SUBMESH_152,
  GLOBAL_SPONZA_MESHES_SUBMESH_153,
  GLOBAL_SPONZA_MESHES_SUBMESH_154,
  GLOBAL_SPONZA_MESHES_SUBMESH_155,
  GLOBAL_SPONZA_MESHES_SUBMESH_156,
  GLOBAL_SPONZA_MESHES_SUBMESH_157,
  GLOBAL_SPONZA_MESHES_SUBMESH_158,
  GLOBAL_SPONZA_MESHES_SUBMESH_159,
  GLOBAL_SPONZA_MESHES_SUBMESH_16,
  GLOBAL_SPONZA_MESHES_SUBMESH_160,
  GLOBAL_SPONZA_MESHES_SUBMESH_161,
  GLOBAL_SPONZA_MESHES_SUBMESH_162,
  GLOBAL_SPONZA_MESHES_SUBMESH_163,
  GLOBAL_SPONZA_MESHES_SUBMESH_164,
  GLOBAL_SPONZA_MESHES_SUBMESH_165,
  GLOBAL_SPONZA_MESHES_SUBMESH_166,
  GLOBAL_SPONZA_MESHES_SUBMESH_167,
  GLOBAL_SPONZA_MESHES_SUBMESH_168,
  GLOBAL_SPONZA_MESHES_SUBMESH_169,
  GLOBAL_SPONZA_MESHES_SUBMESH_17,
  GLOBAL_SPONZA_MESHES_SUBMESH_170,
  GLOBAL_SPONZA_MESHES_SUBMESH_171,
  GLOBAL_SPONZA_MESHES_SUBMESH_172,
  GLOBAL_SPONZA_MESHES_SUBMESH_173,
  GLOBAL_SPONZA_MESHES_SUBMESH_174,
  GLOBAL_SPONZA_MESHES_SUBMESH_175,
  GLOBAL_SPONZA_MESHES_SUBMESH_176,
  GLOBAL_SPONZA_MESHES_SUBMESH_177,
  GLOBAL_SPONZA_MESHES_SUBMESH_178,
  GLOBAL_SPONZA_MESHES_SUBMESH_179,
  GLOBAL_SPONZA_MESHES_SUBMESH_18,
  GLOBAL_SPONZA_MESHES_SUBMESH_180,
  GLOBAL_SPONZA_MESHES_SUBMESH_181,
  GLOBAL_SPONZA_MESHES_SUBMESH_182,
  GLOBAL_SPONZA_MESHES_SUBMESH_183,
  GLOBAL_SPONZA_MESHES_SUBMESH_184,
  GLOBAL_SPONZA_MESHES_SUBMESH_185,
  GLOBAL_SPONZA_MESHES_SUBMESH_186,
  GLOBAL_SPONZA_MESHES_SUBMESH_187,
  GLOBAL_SPONZA_MESHES_SUBMESH_188,
  GLOBAL_SPONZA_MESHES_SUBMESH_189,
  GLOBAL_SPONZA_MESHES_SUBMESH_19,
  GLOBAL_SPONZA_MESHES_SUBMESH_190,
  GLOBAL_SPONZA_MESHES_SUBMESH_191,
  GLOBAL_SPONZA_MESHES_SUBMESH_192,
  GLOBAL_SPONZA_MESHES_SUBMESH_193,
  GLOBAL_SPONZA_MESHES_SUBMESH_194,
  GLOBAL_SPONZA_MESHES_SUBMESH_195,
  GLOBAL_SPONZA_MESHES_SUBMESH_196,
  GLOBAL_SPONZA_MESHES_SUBMESH_197,
  GLOBAL_SPONZA_MESHES_SUBMESH_198,
  GLOBAL_SPONZA_MESHES_SUBMESH_199,
  GLOBAL_SPONZA_MESHES_SUBMESH_20,
  GLOBAL_SPONZA_MESHES_SUBMESH_200,
  GLOBAL_SPONZA_MESHES_SUBMESH_201,
  GLOBAL_SPONZA_MESHES_SUBMESH_202,
  GLOBAL_SPONZA_MESHES_SUBMESH_203,
  GLOBAL_SPONZA_MESHES_SUBMESH_204,
  GLOBAL_SPONZA_MESHES_SUBMESH_205,
  GLOBAL_SPONZA_MESHES_SUBMESH_206,
  GLOBAL_SPONZA_MESHES_SUBMESH_207,
  GLOBAL_SPONZA_MESHES_SUBMESH_208,
  GLOBAL_SPONZA_MESHES_SUBMESH_209,
  GLOBAL_SPONZA_MESHES_SUBMESH_21,
  GLOBAL_SPONZA_MESHES_SUBMESH_210,
  GLOBAL_SPONZA_MESHES_SUBMESH_211,
  GLOBAL_SPONZA_MESHES_SUBMESH_212,
  GLOBAL_SPONZA_MESHES_SUBMESH_213,
  GLOBAL_SPONZA_MESHES_SUBMESH_214,
  GLOBAL_SPONZA_MESHES_SUBMESH_215,
  GLOBAL_SPONZA_MESHES_SUBMESH_216,
  GLOBAL_SPONZA_MESHES_SUBMESH_217,
  GLOBAL_SPONZA_MESHES_SUBMESH_218,
  GLOBAL_SPONZA_MESHES_SUBMESH_219,
  GLOBAL_SPONZA_MESHES_SUBMESH_22,
  GLOBAL_SPONZA_MESHES_SUBMESH_220,
  GLOBAL_SPONZA_MESHES_SUBMESH_221,
  GLOBAL_SPONZA_MESHES_SUBMESH_222,
  GLOBAL_SPONZA_MESHES_SUBMESH_223,
  GLOBAL_SPONZA_MESHES_SUBMESH_224,
  GLOBAL_SPONZA_MESHES_SUBMESH_225,
  GLOBAL_SPONZA_MESHES_SUBMESH_226,
  GLOBAL_SPONZA_MESHES_SUBMESH_227,
  GLOBAL_SPONZA_MESHES_SUBMESH_228,
  GLOBAL_SPONZA_MESHES_SUBMESH_229,
  GLOBAL_SPONZA_MESHES_SUBMESH_23,
  GLOBAL_SPONZA_MESHES_SUBMESH_230,
  GLOBAL_SPONZA_MESHES_SUBMESH_231,
  GLOBAL_SPONZA_MESHES_SUBMESH_232,
  GLOBAL_SPONZA_MESHES_SUBMESH_233,
  GLOBAL_SPONZA_MESHES_SUBMESH_234,
  GLOBAL_SPONZA_MESHES_SUBMESH_235,
  GLOBAL_SPONZA_MESHES_SUBMESH_236,
  GLOBAL_SPONZA_MESHES_SUBMESH_237,
  GLOBAL_SPONZA_MESHES_SUBMESH_238,
  GLOBAL_SPONZA_MESHES_SUBMESH_239,
  GLOBAL_SPONZA_MESHES_SUBMESH_24,
  GLOBAL_SPONZA_MESHES_SUBMESH_240,
  GLOBAL_SPONZA_MESHES_SUBMESH_241,
  GLOBAL_SPONZA_MESHES_SUBMESH_242,
  GLOBAL_SPONZA_MESHES_SUBMESH_243,
  GLOBAL_SPONZA_MESHES_SUBMESH_244,
  GLOBAL_SPONZA_MESHES_SUBMESH_245,
  GLOBAL_SPONZA_MESHES_SUBMESH_246,
  GLOBAL_SPONZA_MESHES_SUBMESH_247,
  GLOBAL_SPONZA_MESHES_SUBMESH_248,
  GLOBAL_SPONZA_MESHES_SUBMESH_249,
  GLOBAL_SPONZA_MESHES_SUBMESH_25,
  GLOBAL_SPONZA_MESHES_SUBMESH_250,
  GLOBAL_SPONZA_MESHES_SUBMESH_251,
  GLOBAL_SPONZA_MESHES_SUBMESH_252,
  GLOBAL_SPONZA_MESHES_SUBMESH_253,
  GLOBAL_SPONZA_MESHES_SUBMESH_254,
  GLOBAL_SPONZA_MESHES_SUBMESH_255,
  GLOBAL_SPONZA_MESHES_SUBMESH_256,
  GLOBAL_SPONZA_MESHES_SUBMESH_257,
  GLOBAL_SPONZA_MESHES_SUBMESH_258,
  GLOBAL_SPONZA_MESHES_SUBMESH_259,
  GLOBAL_SPONZA_MESHES_SUBMESH_26,
  GLOBAL_SPONZA_MESHES_SUBMESH_260,
  GLOBAL_SPONZA_MESHES_SUBMESH_261,
  GLOBAL_SPONZA_MESHES_SUBMESH_262,
  GLOBAL_SPONZA_MESHES_SUBMESH_263,
  GLOBAL_SPONZA_MESHES_SUBMESH_264,
  GLOBAL_SPONZA_MESHES_SUBMESH_265,
  GLOBAL_SPONZA_MESHES_SUBMESH_266,
  GLOBAL_SPONZA_MESHES_SUBMESH_267,
  GLOBAL_SPONZA_MESHES_SUBMESH_268,
  GLOBAL_SPONZA_MESHES_SUBMESH_269,
  GLOBAL_SPONZA_MESHES_SUBMESH_27,
  GLOBAL_SPONZA_MESHES_SUBMESH_270,
  GLOBAL_SPONZA_MESHES_SUBMESH_271,
  GLOBAL_SPONZA_MESHES_SUBMESH_272,
  GLOBAL_SPONZA_MESHES_SUBMESH_273,
  GLOBAL_SPONZA_MESHES_SUBMESH_274,
  GLOBAL_SPONZA_MESHES_SUBMESH_275,
  GLOBAL_SPONZA_MESHES_SUBMESH_276,
  GLOBAL_SPONZA_MESHES_SUBMESH_277,
  GLOBAL_SPONZA_MESHES_SUBMESH_278,
  GLOBAL_SPONZA_MESHES_SUBMESH_279,
  GLOBAL_SPONZA_MESHES_SUBMESH_28,
  GLOBAL_SPONZA_MESHES_SUBMESH_280,
  GLOBAL_SPONZA_MESHES_SUBMESH_281,
  GLOBAL_SPONZA_MESHES_SUBMESH_282,
  GLOBAL_SPONZA_MESHES_SUBMESH_283,
  GLOBAL_SPONZA_MESHES_SUBMESH_284,
  GLOBAL_SPONZA_MESHES_SUBMESH_285,
  GLOBAL_SPONZA_MESHES_SUBMESH_286,
  GLOBAL_SPONZA_MESHES_SUBMESH_287,
  GLOBAL_SPONZA_MESHES_SUBMESH_288,
  GLOBAL_SPONZA_MESHES_SUBMESH_289,
  GLOBAL_SPONZA_MESHES_SUBMESH_29,
  GLOBAL_SPONZA_MESHES_SUBMESH_290,
  GLOBAL_SPONZA_MESHES_SUBMESH_291,
  GLOBAL_SPONZA_MESHES_SUBMESH_292,
  GLOBAL_SPONZA_MESHES_SUBMESH_293,
  GLOBAL_SPONZA_MESHES_SUBMESH_294,
  GLOBAL_SPONZA_MESHES_SUBMESH_295,
  GLOBAL_SPONZA_MESHES_SUBMESH_296,
  GLOBAL_SPONZA_MESHES_SUBMESH_297,
  GLOBAL_SPONZA_MESHES_SUBMESH_298,
  GLOBAL_SPONZA_MESHES_SUBMESH_299,
  GLOBAL_SPONZA_MESHES_SUBMESH_30,
  GLOBAL_SPONZA_MESHES_SUBMESH_300,
  GLOBAL_SPONZA_MESHES_SUBMESH_301,
  GLOBAL_SPONZA_MESHES_SUBMESH_302,
  GLOBAL_SPONZA_MESHES_SUBMESH_303,
  GLOBAL_SPONZA_MESHES_SUBMESH_304,
  GLOBAL_SPONZA_MESHES_SUBMESH_305,
  GLOBAL_SPONZA_MESHES_SUBMESH_306,
  GLOBAL_SPONZA_MESHES_SUBMESH_307,
  GLOBAL_SPONZA_MESHES_SUBMESH_308,
  GLOBAL_SPONZA_MESHES_SUBMESH_309,
  GLOBAL_SPONZA_MESHES_SUBMESH_31,
  GLOBAL_SPONZA_MESHES_SUBMESH_310,
  GLOBAL_SPONZA_MESHES_SUBMESH_311,
  GLOBAL_SPONZA_MESHES_SUBMESH_312,
  GLOBAL_SPONZA_MESHES_SUBMESH_313,
  GLOBAL_SPONZA_MESHES_SUBMESH_314,
  GLOBAL_SPONZA_MESHES_SUBMESH_315,
  GLOBAL_SPONZA_MESHES_SUBMESH_316,
  GLOBAL_SPONZA_MESHES_SUBMESH_317,
  GLOBAL_SPONZA_MESHES_SUBMESH_318,
  GLOBAL_SPONZA_MESHES_SUBMESH_319,
  GLOBAL_SPONZA_MESHES_SUBMESH_32,
  GLOBAL_SPONZA_MESHES_SUBMESH_320,
  GLOBAL_SPONZA_MESHES_SUBMESH_321,
  GLOBAL_SPONZA_MESHES_SUBMESH_322,
  GLOBAL_SPONZA_MESHES_SUBMESH_323,
  GLOBAL_SPONZA_MESHES_SUBMESH_324,
  GLOBAL_SPONZA_MESHES_SUBMESH_325,
  GLOBAL_SPONZA_MESHES_SUBMESH_326,
  GLOBAL_SPONZA_MESHES_SUBMESH_327,
  GLOBAL_SPONZA_MESHES_SUBMESH_328,
  GLOBAL_SPONZA_MESHES_SUBMESH_329,
  GLOBAL_SPONZA_MESHES_SUBMESH_33,
  GLOBAL_SPONZA_MESHES_SUBMESH_330,
  GLOBAL_SPONZA_MESHES_SUBMESH_331,
  GLOBAL_SPONZA_MESHES_SUBMESH_332,
  GLOBAL_SPONZA_MESHES_SUBMESH_333,
  GLOBAL_SPONZA_MESHES_SUBMESH_334,
  GLOBAL_SPONZA_MESHES_SUBMESH_335,
  GLOBAL_SPONZA_MESHES_SUBMESH_336,
  GLOBAL_SPONZA_MESHES_SUBMESH_337,
  GLOBAL_SPONZA_MESHES_SUBMESH_338,
  GLOBAL_SPONZA_MESHES_SUBMESH_339,
  GLOBAL_SPONZA_MESHES_SUBMESH_34,
  GLOBAL_SPONZA_MESHES_SUBMESH_340,
  GLOBAL_SPONZA_MESHES_SUBMESH_341,
  GLOBAL_SPONZA_MESHES_SUBMESH_342,
  GLOBAL_SPONZA_MESHES_SUBMESH_343,
  GLOBAL_SPONZA_MESHES_SUBMESH_344,
  GLOBAL_SPONZA_MESHES_SUBMESH_345,
  GLOBAL_SPONZA_MESHES_SUBMESH_346,
  GLOBAL_SPONZA_MESHES_SUBMESH_347,
  GLOBAL_SPONZA_MESHES_SUBMESH_348,
  GLOBAL_SPONZA_MESHES_SUBMESH_349,
  GLOBAL_SPONZA_MESHES_SUBMESH_35,
  GLOBAL_SPONZA_MESHES_SUBMESH_350,
  GLOBAL_SPONZA_MESHES_SUBMESH_351,
  GLOBAL_SPONZA_MESHES_SUBMESH_352,
  GLOBAL_SPONZA_MESHES_SUBMESH_353,
  GLOBAL_SPONZA_MESHES_SUBMESH_354,
  GLOBAL_SPONZA_MESHES_SUBMESH_355,
  GLOBAL_SPONZA_MESHES_SUBMESH_356,
  GLOBAL_SPONZA_MESHES_SUBMESH_357,
  GLOBAL_SPONZA_MESHES_SUBMESH_358,
  GLOBAL_SPONZA_MESHES_SUBMESH_359,
  GLOBAL_SPONZA_MESHES_SUBMESH_36,
  GLOBAL_SPONZA_MESHES_SUBMESH_360,
  GLOBAL_SPONZA_MESHES_SUBMESH_361,
  GLOBAL_SPONZA_MESHES_SUBMESH_362,
  GLOBAL_SPONZA_MESHES_SUBMESH_363,
  GLOBAL_SPONZA_MESHES_SUBMESH_364,
  GLOBAL_SPONZA_MESHES_SUBMESH_365,
  GLOBAL_SPONZA_MESHES_SUBMESH_366,
  GLOBAL_SPONZA_MESHES_SUBMESH_366_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_367,
  GLOBAL_SPONZA_MESHES_SUBMESH_367_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_368,
  GLOBAL_SPONZA_MESHES_SUBMESH_368_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_369,
  GLOBAL_SPONZA_MESHES_SUBMESH_369_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_37,
  GLOBAL_SPONZA_MESHES_SUBMESH_370,
  GLOBAL_SPONZA_MESHES_SUBMESH_370_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_371,
  GLOBAL_SPONZA_MESHES_SUBMESH_371_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_372,
  GLOBAL_SPONZA_MESHES_SUBMESH_372_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_373,
  GLOBAL_SPONZA_MESHES_SUBMESH_374,
  GLOBAL_SPONZA_MESHES_SUBMESH_375,
  GLOBAL_SPONZA_MESHES_SUBMESH_376,
  GLOBAL_SPONZA_MESHES_SUBMESH_377,
  GLOBAL_SPONZA_MESHES_SUBMESH_377_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_378,
  GLOBAL_SPONZA_MESHES_SUBMESH_378_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_379,
  GLOBAL_SPONZA_MESHES_SUBMESH_38,
  GLOBAL_SPONZA_MESHES_SUBMESH_380,
  GLOBAL_SPONZA_MESHES_SUBMESH_381,
  GLOBAL_SPONZA_MESHES_SUBMESH_382,
  GLOBAL_SPONZA_MESHES_SUBMESH_39,
  GLOBAL_SPONZA_MESHES_SUBMESH_40,
  GLOBAL_SPONZA_MESHES_SUBMESH_41,
  GLOBAL_SPONZA_MESHES_SUBMESH_42,
  GLOBAL_SPONZA_MESHES_SUBMESH_43,
  GLOBAL_SPONZA_MESHES_SUBMESH_44,
  GLOBAL_SPONZA_MESHES_SUBMESH_45,
  GLOBAL_SPONZA_MESHES_SUBMESH_46,
  GLOBAL_SPONZA_MESHES_SUBMESH_47,
  GLOBAL_SPONZA_MESHES_SUBMESH_48,
  GLOBAL_SPONZA_MESHES_SUBMESH_49,
  GLOBAL_SPONZA_MESHES_SUBMESH_50,
  GLOBAL_SPONZA_MESHES_SUBMESH_51,
  GLOBAL_SPONZA_MESHES_SUBMESH_52,
  GLOBAL_SPONZA_MESHES_SUBMESH_53,
  GLOBAL_SPONZA_MESHES_SUBMESH_54,
  GLOBAL_SPONZA_MESHES_SUBMESH_55,
  GLOBAL_SPONZA_MESHES_SUBMESH_56,
  GLOBAL_SPONZA_MESHES_SUBMESH_57,
  GLOBAL_SPONZA_MESHES_SUBMESH_58,
  GLOBAL_SPONZA_MESHES_SUBMESH_59,
  GLOBAL_SPONZA_MESHES_SUBMESH_60,
  GLOBAL_SPONZA_MESHES_SUBMESH_61,
  GLOBAL_SPONZA_MESHES_SUBMESH_62,
  GLOBAL_SPONZA_MESHES_SUBMESH_63,
  GLOBAL_SPONZA_MESHES_SUBMESH_64,
  GLOBAL_SPONZA_MESHES_SUBMESH_65,
  GLOBAL_SPONZA_MESHES_SUBMESH_66,
  GLOBAL_SPONZA_MESHES_SUBMESH_67,
  GLOBAL_SPONZA_MESHES_SUBMESH_68,
  GLOBAL_SPONZA_MESHES_SUBMESH_69,
  GLOBAL_SPONZA_MESHES_SUBMESH_70,
  GLOBAL_SPONZA_MESHES_SUBMESH_71,
  GLOBAL_SPONZA_MESHES_SUBMESH_71_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_72,
  GLOBAL_SPONZA_MESHES_SUBMESH_72_001,
  GLOBAL_SPONZA_MESHES_SUBMESH_73,
  GLOBAL_SPONZA_MESHES_SUBMESH_74,
  GLOBAL_SPONZA_MESHES_SUBMESH_75,
  GLOBAL_SPONZA_MESHES_SUBMESH_76,
  GLOBAL_SPONZA_MESHES_SUBMESH_77,
  GLOBAL_SPONZA_MESHES_SUBMESH_78,
  GLOBAL_SPONZA_MESHES_SUBMESH_79,
  GLOBAL_SPONZA_MESHES_SUBMESH_80,
  GLOBAL_SPONZA_MESHES_SUBMESH_81,
  GLOBAL_SPONZA_MESHES_SUBMESH_82,
  GLOBAL_SPONZA_MESHES_SUBMESH_83,
  GLOBAL_SPONZA_MESHES_SUBMESH_84,
  GLOBAL_SPONZA_MESHES_SUBMESH_85,
  GLOBAL_SPONZA_MESHES_SUBMESH_86,
  GLOBAL_SPONZA_MESHES_SUBMESH_87,
  GLOBAL_SPONZA_MESHES_SUBMESH_88,
  GLOBAL_SPONZA_MESHES_SUBMESH_89,
  GLOBAL_SPONZA_MESHES_SUBMESH_90,
  GLOBAL_SPONZA_MESHES_SUBMESH_91,
  GLOBAL_SPONZA_MESHES_SUBMESH_92,
  GLOBAL_SPONZA_MESHES_SUBMESH_93,
  GLOBAL_SPONZA_MESHES_SUBMESH_94,
  GLOBAL_SPONZA_MESHES_SUBMESH_95,
  GLOBAL_SPONZA_MESHES_SUBMESH_96,
  GLOBAL_SPONZA_MESHES_SUBMESH_97,
  GLOBAL_SPONZA_MESHES_SUBMESH_98,
  GLOBAL_SPONZA_MESHES_SUBMESH_99,
  GLOBAL_SPONZA_MESHES_SUBMESH_COUNT,
};

const unsigned GlobalSponzaMeshesTexturesArrayIndices[393] = {
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3,
  3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2,
  2, 2, 0, 0, 0, 0, 0, 2, 2, 1, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

const unsigned GlobalSponzaMeshesTexturesLayerIndices[393] = {
  16, 20, 19, 0,  2,  2,  3,  0,  4,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  4,  0,  0,  0,  0,  0,  0,  2,  14, 4,  4,  4,  4,  4,  3,  3,  3,  5,
  5,  5,  5,  5,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  4,  5,  5,  5,
  5,  5,  5,  5,  5,  5,  5,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  4,
  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  3,  5,  5,  5,  5,  5,  5,  5,  5,
  5,  5,  14, 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  0,  5,  5,  5,  5,  5,
  5,  5,  5,  5,  5,  3,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  3,  5,  5,
  5,  5,  5,  5,  5,  5,  5,  5,  0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  0,  5,  5,  5,  5,  5,  5,  5,
  5,  5,  5,  0,  5,  5,  5,  5,  5,  5,  5,  (unsigned)-1, 2,  1,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  16, 16, 16, 16, 16, 0,  16,
  16, 12, 10, 11, 12, 10, 12, 11, 10, 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  0,  6,  7,  8,  7,  8,  7,  6,  8,  7,  6,  0,  0,  0,  0,
  0,  18, 18, 18, 18, 18, 0,  2,  0,  0,  0,  18, 18, 18, 18, 18, 0,  0,  0,
  0,  0,  18, 18, 18, 18, 18, 0,  0,  0,  2,  0,  18, 18, 18, 18, 18, 20, 19,
  20, 19, 20, 19, 20, 19, 3,  20, 19, 20, 19, 20, 19, 17, 17, 17, 17, 0,  1,
  0,  1,  2,  0,  15, 15, 2,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,  3,  0,
  3,  0,  3,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  2,  3,  2,  2,
  9,  2,  9,  2,  9,  9,  9,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
#endif

EXTERN const int      GlobalSponzaMeshesTextureIndices[];
EXTERN const size_t   GlobalSponzaMeshesTextureIndicesSizeof;
EXTERN const unsigned GlobalSponzaMeshesIndices[];
EXTERN const size_t   GlobalSponzaMeshesIndicesSizeof;
EXTERN const float    GlobalSponzaMeshesVertices[];
EXTERN const size_t   GlobalSponzaMeshesVerticesSizeof;
EXTERN const float    GlobalSponzaMeshesUvs[];
EXTERN const size_t   GlobalSponzaMeshesUvsSizeof;
EXTERN const int      GlobalSponzaMeshesSubmeshVertexBegin[];
EXTERN const size_t   GlobalSponzaMeshesSubmeshVertexBeginSizeof;
EXTERN const int      GlobalSponzaMeshesSubmeshVertexEnd[];
EXTERN const size_t   GlobalSponzaMeshesSubmeshVertexEndSizeof;

#undef EXTERN

typedef enum RedHelperMemoryType {
  RED_HELPER_MEMORY_TYPE_VRAM     = 0,
  RED_HELPER_MEMORY_TYPE_UPLOAD   = 1,
  RED_HELPER_MEMORY_TYPE_READBACK = 2,
} RedHelperMemoryType;

REDGPU_DECLSPEC unsigned REDGPU_API redHelperGetMemoryTypeIndex(const RedGpuInfo * gpuInfo, RedHelperMemoryType memoryType, unsigned memoryTypesSupported) {
  const unsigned        memoryTypesCount = gpuInfo->memoryTypesCount;
  const RedMemoryType * memoryTypes      = gpuInfo->memoryTypes;

  unsigned memoryTypeIsSupported[32];
  memoryTypeIsSupported[0]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
  memoryTypeIsSupported[1]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  memoryTypeIsSupported[2]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  memoryTypeIsSupported[3]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[4]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[5]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[6]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[7]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[8]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[9]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[10] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[11] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[12] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[13] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[14] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[15] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[16] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[17] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[18] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[19] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[20] = (memoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[21] = (memoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[22] = (memoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[23] = (memoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[24] = (memoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[25] = (memoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[26] = (memoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[27] = (memoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[28] = (memoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[29] = (memoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[30] = (memoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[31] = (memoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;

  if (memoryType == RED_HELPER_MEMORY_TYPE_VRAM) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isGpuVram == 1 && memoryTypeIsSupported[i] == 1) {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_UPLOAD) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 0 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_READBACK) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  }

  return -1;
}

void redHelperImageSetStateUsable(RedContext context, RedHandleGpu gpu, RedHandleImage image, RedImagePartBitflags allParts, RedHandleQueue queueToSubmitImageStateChange, unsigned queueFamilyIndexToSubmitImageStateChange, RedStatuses * outStatuses, char * optionalFile, int optionalLine, void * optionalUserData) {
  RedCallProceduresAndAddresses callProceduresAndAddresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callProceduresAndAddresses,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedCalls calls = {0};
  np(redCreateCalls,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "queueFamilyIndex", queueFamilyIndexToSubmitImageStateChange,
    "outCalls", &calls,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCallsSet,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "callsReusable", calls.reusable,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedUsageImage imageUsage = {0};
  imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
  imageUsage.oldAccessStages        = 0;
  imageUsage.newAccessStages        = 0;
  imageUsage.oldAccess              = 0;
  imageUsage.newAccess              = 0;
  imageUsage.oldState               = RED_STATE_UNUSABLE;
  imageUsage.newState               = RED_STATE_USABLE;
  imageUsage.queueFamilyIndexSource = -1;
  imageUsage.queueFamilyIndexTarget = -1;
  imageUsage.image                  = image;
  imageUsage.imageAllParts          = allParts;
  imageUsage.imageLevelsFirst       = 0;
  imageUsage.imageLevelsCount       = -1;
  imageUsage.imageLayersFirst       = 0;
  imageUsage.imageLayersCount       = -1;
  np(redCallUsageAliasOrderBarrier,
    "address", callProceduresAndAddresses.redCallUsageAliasOrderBarrier,
    "calls", calls.handle,
    "context", context,
    "arrayUsagesCount", 0,
    "arrayUsages", NULL,
    "imageUsagesCount", 1,
    "imageUsages", &imageUsage,
    "aliasesCount", 0,
    "aliases", NULL,
    "ordersCount", 0,
    "orders", NULL,
    "dependencyByRegion", 0
  );

  np(redCallsEnd,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = NULL;
  timeline.setTo65536                        = NULL;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.handle;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", context,
    "gpu", gpu,
    "queue", queueToSubmitImageStateChange,
    "timelinesCount", 1,
    "timelines", &timeline,
    "signalCpuSignal", cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCpuSignal,
    "context", context,
    "gpu", gpu,
    "cpuSignal", cpuSignal,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCalls,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
}

typedef struct RedHelperLoadImageData {
  void *   pointer;
  uint64_t bytesCount;
  unsigned width;
  unsigned height;
} RedHelperLoadImageData;

typedef struct RedHelperLoadImage {
  RedImage               image;
  RedHandleMemory        imageMemory;
  RedHandleTexture       texture;
  unsigned               imageLevelsCount;
  unsigned               imageLayersCount;
  RedArray               arrays              [REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT][REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT];
  RedHandleMemory        arraysMemory        [REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT][REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT];
  void *                 arraysMemoryPointer [REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT][REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT];
  RedHelperLoadImageData imagesData          [REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT][REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT];
} RedHelperLoadImage;

RedBool32 redHelperSystemPathMerge(const char * a, const char * b, char * outMergedPath) {
  size_t alen = strlen(a);
  size_t blen = strlen(b);

  if (alen + 1 + blen + 1 > REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH) {
    return 0;
  }

  for (unsigned i = 0; i < alen; i += 1) {
    outMergedPath[i] = a[i];
  }

  outMergedPath[alen] = '/';

  for (unsigned i = 0; i < blen; i += 1) {
    outMergedPath[alen + 1 + i] = b[i];
  }

  outMergedPath[alen + 1 + blen] = 0;

  return 1;
}

unsigned redHelperSystemCountDirsOrFiles(const char * path, RedBool32 countFiles, const char * optionalFile, int optionalLine) {
  redMarkSet(__FUNCTION__, optionalFile, optionalLine, NULL);

  char searchPath[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
  redHelperSystemPathMerge(path, "/*", searchPath);

  WIN32_FIND_DATA wfd = {0};
  HANDLE hFind = FindFirstFile(searchPath, &wfd);
  assert(hFind != INVALID_HANDLE_VALUE);

  unsigned count = 0;

  do {
    if (wfd.cFileName[0] == '.' &&
        wfd.cFileName[1] == 0)
    {
      continue;
    } else if (wfd.cFileName[0] == '.' &&
               wfd.cFileName[1] == '.' &&
               wfd.cFileName[2] == 0)
    {
      continue;
    }

    char temp1[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    char temp2[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    redHelperSystemPathMerge(path, "/", temp1);
    redHelperSystemPathMerge(temp1, wfd.cFileName, temp2);

    if (countFiles == 1) {
      if (PathIsDirectory(temp2) == 0) {
        count += 1;
      }
    } else {
      if (PathIsDirectory(temp2) != 0) {
        count += 1;
      }
    }

  } while (FindNextFile(hFind, &wfd) != 0);

  redMarkEnd(optionalFile, optionalLine, NULL);
  return count;
}

RedHelperLoadImageData redHelperLoadImageDataRead(const char * filepath, const char * optionalFile, int optionalLine) {
  redMarkSet(__FUNCTION__, optionalFile, optionalLine, NULL);
  RedHelperLoadImageData out = {0};
  redMarkSet("fopen", optionalFile, optionalLine, NULL);
  FILE * fopenFd = fopen(filepath, "rb");
  redMarkEnd(optionalFile, optionalLine, NULL);
  if (fopenFd == NULL) {
    return out;
  }
  int imageBpp = 0;
  redMarkSet("stbi_load_from_file", optionalFile, optionalLine, NULL);
  out.pointer = (void *)stbi_load_from_file(fopenFd, (int *)&out.width, (int *)&out.height, &imageBpp, STBI_rgb_alpha);
  redMarkEnd(optionalFile, optionalLine, NULL);
  redMarkSet("fclose", optionalFile, optionalLine, NULL);
  fclose(fopenFd);
  redMarkEnd(optionalFile, optionalLine, NULL);
  out.bytesCount = out.width * out.height * sizeof(unsigned);
  redMarkEnd(optionalFile, optionalLine, NULL);
  return out;
}

RedHelperLoadImage redHelperLoadImage(RedContext context, RedHandleGpu gpu, const RedGpuInfo * gpuInfo, RedHandleCalls callsToRecordImageUploadingInto, RedHandleQueue queueToSubmitImageStateChange, unsigned queueFamilyIndexToSubmitImageStateChange, char * handleName, const char * path, unsigned levelsDirDigitsCount, const char * levelsDirDigitsFormat, unsigned layersDirDigitsCount, const char * layersDirDigitsFormat, unsigned width, unsigned height, RedStatuses * outStatuses, char * optionalFile, int optionalLine, void * optionalUserData) {
  redMarkSet(__FUNCTION__, optionalFile, optionalLine, NULL);

  RedHelperLoadImage out = {0};

  out.imageLevelsCount = redHelperSystemCountDirsOrFiles(path, 0, optionalFile, optionalLine);
  assert(out.imageLevelsCount <= REDGPU_HELPER_LOAD_IMAGE_MAX_LEVELS_COUNT);

  {
    char level0DirName[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    level0DirName[levelsDirDigitsCount] = 0;
#ifdef _WIN32
    _snprintf_s(level0DirName, sizeof(level0DirName), sizeof(level0DirName), levelsDirDigitsFormat, 0);
#else
    snprintf(level0DirName, sizeof(level0DirName, levelsDirDigitsFormat, 0);
#endif
    char level0DirPath[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    {
      RedBool32 result = redHelperSystemPathMerge(path, level0DirName, level0DirPath);
      assert(result == 1);
    }
    out.imageLayersCount = redHelperSystemCountDirsOrFiles(level0DirPath, 1, optionalFile, optionalLine);
    assert(out.imageLayersCount <= REDGPU_HELPER_LOAD_IMAGE_MAX_LAYERS_COUNT);
  }

  np(redCreateImage,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "dimensions", RED_IMAGE_DIMENSIONS_2D,
    "format", RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED,
    "width", width,
    "height", height,
    "depth", 1,
    "levelsCount", out.imageLevelsCount,
    "layersCount", out.imageLayersCount,
    "multisampleCount", RED_MULTISAMPLE_COUNT_BITFLAG_1,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndexToSubmitImageStateChange,
    "dedicate", 0,
    "outImage", &out.image,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "bytesCount", out.image.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, out.image.memoryTypesSupported),
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &out.imageMemory,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedMemoryImage memoryImage = {0};
  memoryImage.setTo1000157001  = 1000157001;
  memoryImage.setTo0           = 0;
  memoryImage.image            = out.image.handle;
  memoryImage.memory           = out.imageMemory;
  memoryImage.memoryBytesFirst = 0;
  np(redMemorySet,
    "context", context,
    "gpu", gpu,
    "memoryArraysCount", 0,
    "memoryArrays", NULL,
    "memoryImagesCount", 1,
    "memoryImages", &memoryImage,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCreateTexture,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "image", out.image.handle,
    "parts", RED_IMAGE_PART_BITFLAG_COLOR,
    "dimensions", RED_TEXTURE_DIMENSIONS_2D_LAYERED,
    "format", RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED,
    "levelsFirst", 0,
    "levelsCount", out.imageLevelsCount,
    "layersFirst", 0,
    "layersCount", out.imageLayersCount,
    "restrictToAccess", 0,
    "outTexture", &out.texture,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  redHelperImageSetStateUsable(
    context,
    gpu,
    out.image.handle,
    RED_IMAGE_PART_BITFLAG_COLOR,
    queueToSubmitImageStateChange,
    queueFamilyIndexToSubmitImageStateChange,
    outStatuses,
    optionalFile,
    optionalLine,
    optionalUserData
  );

  for (unsigned i = 0; i < out.imageLevelsCount; i += 1) {
    char levelDirName[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    levelDirName[levelsDirDigitsCount] = 0;
#ifdef _WIN32
    _snprintf_s(levelDirName, sizeof(levelDirName), sizeof(levelDirName), levelsDirDigitsFormat, i);
#else
    snprintf(levelDirName, sizeof(levelDirName, levelsDirDigitsFormat, i);
#endif
    char levelDirPath[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
    {
      RedBool32 result = redHelperSystemPathMerge(path, levelDirName, levelDirPath);
      assert(result == 1);
    }
    for (unsigned j = 0; j < out.imageLayersCount; j += 1) {
      char layerDirName[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
      layerDirName[layersDirDigitsCount] = 0;
#ifdef _WIN32
      _snprintf_s(layerDirName, sizeof(layerDirName), sizeof(layerDirName), layersDirDigitsFormat, j);
#else
      snprintf(layerDirName, sizeof(layerDirName, layersDirDigitsFormat, j);
#endif
      char layerPath[REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH];
      {
        RedBool32 result = redHelperSystemPathMerge(levelDirPath, layerDirName, layerPath);
        assert(result == 1);
      }
      size_t layerPathLength = strlen(layerPath);
      assert(layerPathLength + 4 + 1 <= REDGPU_HELPER_SYSTEM_MAX_PATH_LENGTH);
      layerPath[layerPathLength + 0] = '.';
      layerPath[layerPathLength + 1] = 'p';
      layerPath[layerPathLength + 2] = 'n';
      layerPath[layerPathLength + 3] = 'g';
      layerPath[layerPathLength + 4] = 0;
      out.imagesData[i][j] = redHelperLoadImageDataRead(layerPath, optionalFile, optionalLine);
      size_t arrayBytesCount = out.imagesData[i][j].bytesCount;
      np(redCreateArray,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "type", RED_ARRAY_TYPE_ARRAY_RO,
        "bytesCount", arrayBytesCount,
        "structuredBufferElementBytesCount", 0,
        "restrictToAccess", 0,
        "initialQueueFamilyIndex", queueFamilyIndexToSubmitImageStateChange,
        "dedicate", 0,
        "outArray", &out.arrays[i][j],
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
      np(redMemoryAllocateMappable,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "dedicate", 0,
        "array", out.arrays[i][j].handle,
        "arrayMemoryBytesCount", out.arrays[i][j].memoryBytesCount,
        "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_UPLOAD, out.arrays[i][j].memoryTypesSupported),
        "memoryBitflags", 0,
        "outMemory", &out.arraysMemory[i][j],
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
      RedMemoryArray memoryArray = {0};
      memoryArray.setTo1000157000  = 1000157000;
      memoryArray.setTo0           = 0;
      memoryArray.array            = out.arrays[i][j].handle;
      memoryArray.memory           = out.arraysMemory[i][j];
      memoryArray.memoryBytesFirst = 0;
      np(redMemorySet,
        "context", context,
        "gpu", gpu,
        "memoryArraysCount", 1,
        "memoryArrays", &memoryArray,
        "memoryImagesCount", 0,
        "memoryImages", NULL,
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
      void * pointer = NULL;
      np(redMemoryMap,
        "context", context,
        "gpu", gpu,
        "mappableMemory", out.arraysMemory[i][j],
        "mappableMemoryBytesFirst", 0,
        "mappableMemoryBytesCount", arrayBytesCount,
        "outVolatilePointer", &pointer,
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
      out.arraysMemoryPointer[i][j] = pointer;
      memcpy(out.arraysMemoryPointer[i][j], out.imagesData[i][j].pointer, out.imagesData[i][j].bytesCount);
      {
        RedMappableMemoryRange memoryRange = {0};
        memoryRange.setTo6                        = 6;
        memoryRange.setTo0                        = 0;
        memoryRange.mappableMemory                = out.arraysMemory[i][j];
        memoryRange.mappableMemoryRangeBytesFirst = 0;
        memoryRange.mappableMemoryRangeBytesCount = out.imagesData[i][j].bytesCount;
        np(redMemoryNonCoherentFlush,
          "context", context,
          "gpu", gpu,
          "mappableMemoryRangesCount", 1,
          "mappableMemoryRanges", &memoryRange,
          "outStatuses", outStatuses,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", optionalUserData
        );
      }
    }
  }

  RedCallProceduresAndAddresses callProceduresAndAddresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callProceduresAndAddresses,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  for (unsigned i = 0; i < out.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < out.imageLayersCount; j += 1) {
      int      divide = (int)pow(2, (double)i);
      unsigned width  = out.imagesData[0][0].width  / divide;
      unsigned height = out.imagesData[0][0].height / divide;
      redCallMarkSet(callProceduresAndAddresses.redCallMarkSet, callsToRecordImageUploadingInto, "relCreateImage::calls::imageUploading");

      RedCopyArrayImageRange copy = {0};
      copy.arrayBytesFirst               = 0;
      copy.arrayTexelsCountToNextRow     = width;
      copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
      copy.imageParts.allParts           = RED_IMAGE_PART_BITFLAG_COLOR;
      copy.imageParts.level              = i;
      copy.imageParts.layersFirst        = j;
      copy.imageParts.layersCount        = 1;
      copy.imageOffset.texelX            = 0;
      copy.imageOffset.texelY            = 0;
      copy.imageOffset.texelZ            = 0;
      copy.imageExtent.texelsCountWidth  = width;
      copy.imageExtent.texelsCountHeight = height;
      copy.imageExtent.texelsCountDepth  = 1;
      npfp(redCallCopyArrayToImage, callProceduresAndAddresses.redCallCopyArrayToImage,
        "calls", callsToRecordImageUploadingInto,
        "arrayR", out.arrays[i][j].handle,
        "imageW", out.image.handle,
        "setTo1", 1,
        "rangesCount", 1,
        "ranges", &copy
      );

      redCallMarkEnd(callProceduresAndAddresses.redCallMarkEnd, callsToRecordImageUploadingInto);
    }
  }

  for (unsigned i = 0; i < out.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < out.imageLayersCount; j += 1) {
      if (out.imagesData[i][j].pointer != NULL) {
        stbi_image_free(out.imagesData[i][j].pointer);
        out.imagesData[i][j].pointer = NULL;
      }
    }
  }

  redMarkEnd(optionalFile, optionalLine, NULL);

  return out;
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    int mode = glfwGetInputMode(window, GLFW_CURSOR);
    glfwSetInputMode(window, GLFW_CURSOR, mode == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  }
}

int main() {
  unsigned windowWidth  = 1920;
  unsigned windowHeight = 1080;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(640, 360, "REDGPU Example: Sponza", NULL, NULL);
  {
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode->width <= windowWidth || mode->height <= windowHeight) {
      int x = 0;
      int y = 0;
      int w = 0;
      int h = 0;
      glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
      int l = 0;
      int t = 0;
      int r = 0;
      int b = 0;
      glfwGetWindowFrameSize(window, &l, &t, &r, &b);
      windowWidth  = w - l - r;
      windowHeight = h - t - b;
#ifdef REDGPU_WSI_WIN32
      glfwSetWindowPos(window, l, t);
#endif
    }
    glfwSetWindowSize(window, windowWidth, windowHeight);
  }
  glfwSetMouseButtonCallback(window, mouseButtonCallback);

#ifdef _WIN32
  unsigned extensions[] = {
    RED_SDK_EXTENSION_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT,
    RED_SDK_EXTENSION_DRIVER_PROPERTIES,
    RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL,
    RED_SDK_EXTENSION_DEDICATE_MEMORY,
    RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES,
    RED_SDK_EXTENSION_BATCH_MEMORY_SET,
    RED_SDK_EXTENSION_RASTERIZATION_MODE,
    RED_SDK_EXTENSION_FORMAL_MEMORY_MODEL,
    RED_SDK_EXTENSION_WSI_WIN32
  };
#endif
  RedContext context = NULL;
  np(redCreateContext,
    "malloc", malloc,
    "free", free,
    "optionalMallocTagged", NULL,
    "optionalFreeTagged", NULL,
    "debugCallback", NULL,
    "sdkVersion", RED_SDK_VERSION_1_0_135,
    "sdkExtensionsCount", _countof(extensions),
    "sdkExtensions", extensions,
    "optionalProgramName", NULL,
    "optionalProgramVersion", 0,
    "optionalEngineName", NULL,
    "optionalEngineVersion", 0,
    "optionalSettings", NULL,
    "outContext", &context,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  if (context->gpus[0].supportsGpuCodeDynamicallyIndexableArraysOfTextureROs != 1) {
    printf("Error: this example uses supportsGpuCodeDynamicallyIndexableArraysOfTextureROs feature, but context->gpus[0].supportsGpuCodeDynamicallyIndexableArraysOfTextureROs != 1.\n");
    fflush(stdout);
  }

  RedHandleGpu       gpu              =  context->gpus[0].gpu;
  const RedGpuInfo * gpuInfo          = &context->gpus[0];
  RedHandleQueue     queue            =  context->gpus[0].queues[0];
  unsigned           queueFamilyIndex =  context->gpus[0].queuesFamilyIndex[0];

  RedCallProceduresAndAddresses callsPA = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callsPA,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleSampler samplerAnisotropic = NULL;
  np(redCreateSampler,
    "context", context,
    "gpu", gpu,
    "handleName", "samplerAnisotropic",
    "filteringMag", RED_SAMPLER_FILTERING_LINEAR,
    "filteringMin", RED_SAMPLER_FILTERING_LINEAR,
    "filteringMip", RED_SAMPLER_FILTERING_MIP_LINEAR,
    "behaviorOutsideTextureCoordinateU", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateV", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateW", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "mipLodBias", 0.f,
    "enableAnisotropy", 1,
    "maxAnisotropy", 8,
    "enableCompare", 0,
    "compareOp", RED_COMPARE_OP_NEVER,
    "minLod", 0.f,
    "maxLod", 1000.f,
    "outSampler", &samplerAnisotropic,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleSampler samplerLinear = NULL;
  np(redCreateSampler,
    "context", context,
    "gpu", gpu,
    "handleName", "samplerLinear",
    "filteringMag", RED_SAMPLER_FILTERING_LINEAR,
    "filteringMin", RED_SAMPLER_FILTERING_LINEAR,
    "filteringMip", RED_SAMPLER_FILTERING_MIP_LINEAR,
    "behaviorOutsideTextureCoordinateU", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateV", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateW", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "mipLodBias", 0.f,
    "enableAnisotropy", 0,
    "maxAnisotropy", 0,
    "enableCompare", 0,
    "compareOp", RED_COMPARE_OP_NEVER,
    "minLod", 0.f,
    "maxLod", 1000.f,
    "outSampler", &samplerLinear,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedCalls callsUploads = {0};
  np(redCreateCallsReusable,
    "context", context,
    "gpu", gpu,
    "handleName", "callsUploads",
    "queueFamilyIndex", queueFamilyIndex,
    "outCalls", &callsUploads,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  size_t   meshIndicesCpuBytesCount = GlobalSponzaMeshesIndicesSizeof;
  RedArray meshIndicesCpu           = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshIndicesCpu",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", meshIndicesCpuBytesCount,
    "structuredBufferElementBytesCount", 0,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshIndicesCpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedArray meshIndicesGpu = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshIndicesGpu",
    "type", RED_ARRAY_TYPE_INDEX_RO,
    "bytesCount", GlobalSponzaMeshesIndicesSizeof,
    "structuredBufferElementBytesCount", 0,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshIndicesGpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  size_t   meshVerticesCpuBytesCount = GlobalSponzaMeshesVerticesSizeof;
  RedArray meshVerticesCpu           = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshVerticesCpu",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", meshVerticesCpuBytesCount,
    "structuredBufferElementBytesCount", 0,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshVerticesCpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedArray meshVerticesGpu = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshVerticesGpu",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", GlobalSponzaMeshesVerticesSizeof,
    "structuredBufferElementBytesCount", 4 * sizeof(float),
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshVerticesGpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  size_t   meshUvsCpuBytesCount = GlobalSponzaMeshesUvsSizeof;
  RedArray meshUvsCpu           = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshUvsCpu",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", meshUvsCpuBytesCount,
    "structuredBufferElementBytesCount", 0,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshUvsCpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedArray meshUvsGpu = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "meshUvsGpu",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", GlobalSponzaMeshesUvsSizeof,
    "structuredBufferElementBytesCount", 2 * sizeof(float),
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &meshUvsGpu,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshIndicesCpuMemory = {0};
  np(redMemoryAllocateMappable,
    "context", context,
    "gpu", gpu,
    "handleName", "meshIndicesCpuMemory",
    "dedicate", 0,
    "array", meshIndicesCpu.handle,
    "arrayMemoryBytesCount", meshIndicesCpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_UPLOAD, meshIndicesCpu.memoryTypesSupported),
    "memoryBitflags", 0,
    "outMemory", &meshIndicesCpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshIndicesGpuMemory = {0};
  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "meshIndicesGpuMemory",
    "bytesCount", meshIndicesGpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, meshIndicesGpu.memoryTypesSupported),
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &meshIndicesGpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshVerticesCpuMemory = {0};
  np(redMemoryAllocateMappable,
    "context", context,
    "gpu", gpu,
    "handleName", "meshVerticesCpuMemory",
    "dedicate", 0,
    "array", meshVerticesCpu.handle,
    "arrayMemoryBytesCount", meshVerticesCpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_UPLOAD, meshVerticesCpu.memoryTypesSupported),
    "memoryBitflags", 0,
    "outMemory", &meshVerticesCpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshVerticesGpuMemory = {0};
  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "meshVerticesGpuMemory",
    "bytesCount", meshVerticesGpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, meshVerticesGpu.memoryTypesSupported),
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &meshVerticesGpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshUvsCpuMemory = {0};
  np(redMemoryAllocateMappable,
    "context", context,
    "gpu", gpu,
    "handleName", "meshUvsCpuMemory",
    "dedicate", 0,
    "array", meshUvsCpu.handle,
    "arrayMemoryBytesCount", meshUvsCpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_UPLOAD, meshUvsCpu.memoryTypesSupported),
    "memoryBitflags", 0,
    "outMemory", &meshUvsCpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory meshUvsGpuMemory = {0};
  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "meshUvsGpuMemory",
    "bytesCount", meshUvsGpu.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, meshUvsGpu.memoryTypesSupported),
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &meshUvsGpuMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  {
    RedMemoryArray memorysArrays[6] = {0};
    memorysArrays[0].setTo1000157000  = 1000157000;
    memorysArrays[0].setTo0           = 0;
    memorysArrays[0].array            = meshIndicesCpu.handle;
    memorysArrays[0].memory           = meshIndicesCpuMemory;
    memorysArrays[0].memoryBytesFirst = 0;
    memorysArrays[1].setTo1000157000  = 1000157000;
    memorysArrays[1].setTo0           = 0;
    memorysArrays[1].array            = meshIndicesGpu.handle;
    memorysArrays[1].memory           = meshIndicesGpuMemory;
    memorysArrays[1].memoryBytesFirst = 0;
    memorysArrays[2].setTo1000157000  = 1000157000;
    memorysArrays[2].setTo0           = 0;
    memorysArrays[2].array            = meshVerticesCpu.handle;
    memorysArrays[2].memory           = meshVerticesCpuMemory;
    memorysArrays[2].memoryBytesFirst = 0;
    memorysArrays[3].setTo1000157000  = 1000157000;
    memorysArrays[3].setTo0           = 0;
    memorysArrays[3].array            = meshVerticesGpu.handle;
    memorysArrays[3].memory           = meshVerticesGpuMemory;
    memorysArrays[3].memoryBytesFirst = 0;
    memorysArrays[4].setTo1000157000  = 1000157000;
    memorysArrays[4].setTo0           = 0;
    memorysArrays[4].array            = meshUvsCpu.handle;
    memorysArrays[4].memory           = meshUvsCpuMemory;
    memorysArrays[4].memoryBytesFirst = 0;
    memorysArrays[5].setTo1000157000  = 1000157000;
    memorysArrays[5].setTo0           = 0;
    memorysArrays[5].array            = meshUvsGpu.handle;
    memorysArrays[5].memory           = meshUvsGpuMemory;
    memorysArrays[5].memoryBytesFirst = 0;
    np(redMemorySet,
      "context", context,
      "gpu", gpu,
      "memoryArraysCount", _countof(memorysArrays),
      "memoryArrays", memorysArrays,
      "memoryImagesCount", 0,
      "memoryImages", NULL,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  void * meshIndicesCpuMemoryPointer = NULL;
  np(redMemoryMap,
    "context", context,
    "gpu", gpu,
    "mappableMemory", meshIndicesCpuMemory,
    "mappableMemoryBytesFirst", 0,
    "mappableMemoryBytesCount", meshIndicesCpuBytesCount,
    "outVolatilePointer", &meshIndicesCpuMemoryPointer,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  void * volatile meshIndicesCpuMemoryVolatilePointer = meshIndicesCpuMemoryPointer;

  void * meshVerticesCpuMemoryPointer = NULL;
  np(redMemoryMap,
    "context", context,
    "gpu", gpu,
    "mappableMemory", meshVerticesCpuMemory,
    "mappableMemoryBytesFirst", 0,
    "mappableMemoryBytesCount", meshVerticesCpuBytesCount,
    "outVolatilePointer", &meshVerticesCpuMemoryPointer,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  void * volatile meshVerticesCpuMemoryVolatilePointer = meshVerticesCpuMemoryPointer;

  void * meshUvsCpuMemoryPointer = NULL;
  np(redMemoryMap,
    "context", context,
    "gpu", gpu,
    "mappableMemory", meshUvsCpuMemory,
    "mappableMemoryBytesFirst", 0,
    "mappableMemoryBytesCount", meshUvsCpuBytesCount,
    "outVolatilePointer", &meshUvsCpuMemoryPointer,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  void * volatile meshUvsCpuMemoryVolatilePointer = meshUvsCpuMemoryPointer;

  memcpy(meshIndicesCpuMemoryVolatilePointer, GlobalSponzaMeshesIndices, GlobalSponzaMeshesIndicesSizeof);
  memcpy(meshVerticesCpuMemoryVolatilePointer, GlobalSponzaMeshesVertices, GlobalSponzaMeshesVerticesSizeof);
  memcpy(meshUvsCpuMemoryVolatilePointer, GlobalSponzaMeshesUvs, GlobalSponzaMeshesUvsSizeof);

  RedMappableMemoryRange memorysRangesToFlush[3] = {0};
  memorysRangesToFlush[0].setTo6                        = 6;
  memorysRangesToFlush[0].setTo0                        = 0;
  memorysRangesToFlush[0].mappableMemory                = meshIndicesCpuMemory;
  memorysRangesToFlush[0].mappableMemoryRangeBytesFirst = 0;
  memorysRangesToFlush[0].mappableMemoryRangeBytesCount = GlobalSponzaMeshesIndicesSizeof;
  memorysRangesToFlush[1].setTo6                        = 6;
  memorysRangesToFlush[1].setTo0                        = 0;
  memorysRangesToFlush[1].mappableMemory                = meshVerticesCpuMemory;
  memorysRangesToFlush[1].mappableMemoryRangeBytesFirst = 0;
  memorysRangesToFlush[1].mappableMemoryRangeBytesCount = GlobalSponzaMeshesVerticesSizeof;
  memorysRangesToFlush[2].setTo6                        = 6;
  memorysRangesToFlush[2].setTo0                        = 0;
  memorysRangesToFlush[2].mappableMemory                = meshUvsCpuMemory;
  memorysRangesToFlush[2].mappableMemoryRangeBytesFirst = 0;
  memorysRangesToFlush[2].mappableMemoryRangeBytesCount = GlobalSponzaMeshesUvsSizeof;
  np(redMemoryNonCoherentFlush,
    "context", context,
    "gpu", gpu,
    "mappableMemoryRangesCount", _countof(memorysRangesToFlush),
    "mappableMemoryRanges", memorysRangesToFlush,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  np(redCallsSet,
    "context", context,
    "gpu", gpu,
    "calls", callsUploads.handle,
    "callsMemory", callsUploads.memory,
    "callsReusable", callsUploads.reusable,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  {
    RedCopyArrayRange copyArrayRange = {0};
    copyArrayRange.arrayRBytesFirst = 0;
    copyArrayRange.arrayWBytesFirst = 0;
    copyArrayRange.bytesCount       = GlobalSponzaMeshesIndicesSizeof;
    npfp(redCallCopyArrayToArray, callsPA.redCallCopyArrayToArray,
      "calls", callsUploads.handle,
      "arrayR", meshIndicesCpu.handle,
      "arrayW", meshIndicesGpu.handle,
      "rangesCount", 1,
      "ranges", &copyArrayRange
    );
  }
  {
    RedCopyArrayRange copyArrayRange = {0};
    copyArrayRange.arrayRBytesFirst = 0;
    copyArrayRange.arrayWBytesFirst = 0;
    copyArrayRange.bytesCount       = GlobalSponzaMeshesVerticesSizeof;
    npfp(redCallCopyArrayToArray, callsPA.redCallCopyArrayToArray,
      "calls", callsUploads.handle,
      "arrayR", meshVerticesCpu.handle,
      "arrayW", meshVerticesGpu.handle,
      "rangesCount", 1,
      "ranges", &copyArrayRange
    );
  }
  {
    RedCopyArrayRange copyArrayRange = {0};
    copyArrayRange.arrayRBytesFirst = 0;
    copyArrayRange.arrayWBytesFirst = 0;
    copyArrayRange.bytesCount       = GlobalSponzaMeshesUvsSizeof;
    npfp(redCallCopyArrayToArray, callsPA.redCallCopyArrayToArray,
      "calls", callsUploads.handle,
      "arrayR", meshUvsCpu.handle,
      "arrayW", meshUvsGpu.handle,
      "rangesCount", 1,
      "ranges", &copyArrayRange
    );
  }

  RedHelperLoadImage images256x256 = redHelperLoadImage(/*RedContext context*/context, /*RedHandleGpu gpu*/gpu, /*const RedGpuInfo * gpuInfo*/gpuInfo, /*RedHandleCalls callsToRecordImageUploadingInto*/callsUploads.handle, /*RedHandleQueue queueToSubmitImageStateChange*/queue, /*unsigned queueFamilyIndexToSubmitImageStateChange*/queueFamilyIndex, /*char * handleName*/"images256x256", /*const char * path*/"sponza_textures/256x256", /*unsigned levelsDirDigitsCount*/2, /*const char * levelsDirDigitsFormat*/"%02d", /*unsigned layersDirDigitsCount*/3, /*const char * layersDirDigitsFormat*/"%03d", /*unsigned width*/256, /*unsigned height*/256, /*RedStatuses * outStatuses*/NULL, /*char * optionalFile*/__FILE__, /*int optionalLine*/__LINE__, /*void * optionalUserData*/NULL);
  RedHelperLoadImage images128x128 = redHelperLoadImage(/*RedContext context*/context, /*RedHandleGpu gpu*/gpu, /*const RedGpuInfo * gpuInfo*/gpuInfo, /*RedHandleCalls callsToRecordImageUploadingInto*/callsUploads.handle, /*RedHandleQueue queueToSubmitImageStateChange*/queue, /*unsigned queueFamilyIndexToSubmitImageStateChange*/queueFamilyIndex, /*char * handleName*/"images128x128", /*const char * path*/"sponza_textures/128x128", /*unsigned levelsDirDigitsCount*/2, /*const char * levelsDirDigitsFormat*/"%02d", /*unsigned layersDirDigitsCount*/3, /*const char * layersDirDigitsFormat*/"%03d", /*unsigned width*/128, /*unsigned height*/128, /*RedStatuses * outStatuses*/NULL, /*char * optionalFile*/__FILE__, /*int optionalLine*/__LINE__, /*void * optionalUserData*/NULL);
  RedHelperLoadImage images64x256  = redHelperLoadImage(/*RedContext context*/context, /*RedHandleGpu gpu*/gpu, /*const RedGpuInfo * gpuInfo*/gpuInfo, /*RedHandleCalls callsToRecordImageUploadingInto*/callsUploads.handle, /*RedHandleQueue queueToSubmitImageStateChange*/queue, /*unsigned queueFamilyIndexToSubmitImageStateChange*/queueFamilyIndex, /*char * handleName*/"images64x256",  /*const char * path*/"sponza_textures/64x256",  /*unsigned levelsDirDigitsCount*/2, /*const char * levelsDirDigitsFormat*/"%02d", /*unsigned layersDirDigitsCount*/3, /*const char * layersDirDigitsFormat*/"%03d", /*unsigned width*/ 64, /*unsigned height*/256, /*RedStatuses * outStatuses*/NULL, /*char * optionalFile*/__FILE__, /*int optionalLine*/__LINE__, /*void * optionalUserData*/NULL);
  RedHelperLoadImage images64x64   = redHelperLoadImage(/*RedContext context*/context, /*RedHandleGpu gpu*/gpu, /*const RedGpuInfo * gpuInfo*/gpuInfo, /*RedHandleCalls callsToRecordImageUploadingInto*/callsUploads.handle, /*RedHandleQueue queueToSubmitImageStateChange*/queue, /*unsigned queueFamilyIndexToSubmitImageStateChange*/queueFamilyIndex, /*char * handleName*/"images64x64",   /*const char * path*/"sponza_textures/64x64",   /*unsigned levelsDirDigitsCount*/2, /*const char * levelsDirDigitsFormat*/"%02d", /*unsigned layersDirDigitsCount*/3, /*const char * layersDirDigitsFormat*/"%03d", /*unsigned width*/ 64, /*unsigned height*/ 64, /*RedStatuses * outStatuses*/NULL, /*char * optionalFile*/__FILE__, /*int optionalLine*/__LINE__, /*void * optionalUserData*/NULL);

  np(redCallsEnd,
    "context", context,
    "gpu", gpu,
    "calls", callsUploads.handle,
    "callsMemory", callsUploads.memory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  {
    RedHandleCpuSignal cpuSignal = NULL;
    np(redCreateCpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", NULL,
      "createSignaled", 0,
      "outCpuSignal", &cpuSignal,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    RedGpuTimeline timeline = {0};
    timeline.setTo4                            = 4;
    timeline.setTo0                            = 0;
    timeline.waitForAndUnsignalGpuSignalsCount = 0;
    timeline.waitForAndUnsignalGpuSignals      = NULL;
    timeline.setTo65536                        = NULL;
    timeline.callsCount                        = 1;
    timeline.calls                             = &callsUploads.handle;
    timeline.signalGpuSignalsCount             = 0;
    timeline.signalGpuSignals                  = NULL;
    np(redQueueSubmit,
      "context", context,
      "gpu", gpu,
      "queue", queue,
      "timelinesCount", 1,
      "timelines", &timeline,
      "signalCpuSignal", cpuSignal,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    np(redCpuSignalWait,
      "context", context,
      "gpu", gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &cpuSignal,
      "waitAll", 1,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    np(redDestroyCpuSignal,
      "context", context,
      "gpu", gpu,
      "cpuSignal", cpuSignal,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedHandleSurface  surface                                               = NULL;
  RedHandlePresent  present                                               = NULL;
  RedAccessBitflags presentImagesAccess                                   = RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
  RedHandleImage    presentImages   [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleTexture  presentTextures [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};

#ifdef _WIN32
  np(redCreateSurfaceWin32,
    "context", context,
    "gpu", gpu,
    "handleName", "surface",
    "win32Hinstance", GetModuleHandle(NULL),
    "win32Hwnd", glfwGetWin32Window(window),
    "outSurface", &surface,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
#endif
  RedQueueFamilyIndexGetSupportsPresentOnSurface queueFamilyIndexSupportsPresentOnSurface = {0};
  queueFamilyIndexSupportsPresentOnSurface.surface                                     = surface;
  queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface = 0;
  np(redQueueFamilyIndexGetSupportsPresent,
    "context", context,
    "gpu", gpu,
    "queueFamilyIndex", queueFamilyIndex,
    "supportsPresentOnWin32", NULL,
    "supportsPresentOnXlib", NULL,
    "supportsPresentOnXcb", NULL,
    "supportsPresentOnSurface", &queueFamilyIndexSupportsPresentOnSurface,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  if (queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 0) {
    printf("Error: queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 0\n");
    fflush(stdout);
  }
  
  RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {0};
  np(redSurfaceGetCurrentPropertiesAndPresentLimits,
    "context", context,
    "gpu", gpu,
    "surface", surface,
    "outSurfaceCurrentPropertiesAndPresentLimits", &surfaceCurrentPropertiesAndPresentLimits,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth  != -1) { windowWidth  = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth;  }
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight != -1) { windowHeight = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight; }
  
  np(redCreatePresent,
    "context", context,
    "gpu", gpu,
    "queue", queue,
    "handleName", "present",
    "surface", surface,
    "imagesCount", REDGPU_PROGRAM_PRESENT_IMAGES_COUNT,
    "imagesWidth", windowWidth,
    "imagesHeight", windowHeight,
    "imagesLayersCount", 1,
    "imagesRestrictToAccess", presentImagesAccess,
    "transform", RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY,
    "compositeAlpha", RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE,
    "vsyncMode", REDGPU_PROGRAM_PRESENT_VSYNC_MODE,
    "clipped", 0,
    "discardAfterPresent", 0,
    "oldPresent", NULL,
    "outPresent", &present,
    "outImages", presentImages,
    "outTextures", presentTextures,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedImage         depthMsaa        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedImage         colorMsaa        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleMemory  depthMsaaMemory  [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleMemory  colorMsaaMemory  [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleTexture depthMsaaTexture [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleTexture colorMsaaTexture [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    np(redCreateImage,
      "context", context,
      "gpu", gpu,
      "handleName", "depthMsaa",
      "dimensions", RED_IMAGE_DIMENSIONS_2D,
      "format", RED_FORMAT_DEPTH_32_FLOAT,
      "width", windowWidth,
      "height", windowHeight,
      "depth", 1,
      "levelsCount", 1,
      "layersCount", 1,
      "multisampleCount", RED_MULTISAMPLE_COUNT_BITFLAG_2,
      "restrictToAccess", RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R | RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW,
      "initialQueueFamilyIndex", queueFamilyIndex,
      "dedicate", 1, // [ STATE_CREATION ERROR #640: CREATERESOURCEANDHEAP_INVALIDHEAPMISCFLAGS] error is fixed in REDGPU SDK from May 03, 2023.
      "outImage", &depthMsaa[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateImage,
      "context", context,
      "gpu", gpu,
      "handleName", "colorMsaa",
      "dimensions", RED_IMAGE_DIMENSIONS_2D,
      "format", RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1,
      "width", windowWidth,
      "height", windowHeight,
      "depth", 1,
      "levelsCount", 1,
      "layersCount", 1,
      "multisampleCount", RED_MULTISAMPLE_COUNT_BITFLAG_2,
      "restrictToAccess", RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R | RED_ACCESS_BITFLAG_OUTPUT_COLOR_W,
      "initialQueueFamilyIndex", queueFamilyIndex,
      "dedicate", 1, // [ STATE_CREATION ERROR #640: CREATERESOURCEANDHEAP_INVALIDHEAPMISCFLAGS] error is fixed in REDGPU SDK from May 03, 2023.
      "outImage", &colorMsaa[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redMemoryAllocate,
      "context", context,
      "gpu", gpu,
      "handleName", "depthMsaaMemory",
      "bytesCount", depthMsaa[i].memoryBytesCount,
      "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, depthMsaa[i].memoryTypesSupported),
      "dedicateToArray", NULL,
      "dedicateToImage", depthMsaa[i].handle,
      "memoryBitflags", 0,
      "outMemory", &depthMsaaMemory[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redMemoryAllocate,
      "context", context,
      "gpu", gpu,
      "handleName", "colorMsaaMemory",
      "bytesCount", colorMsaa[i].memoryBytesCount,
      "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, colorMsaa[i].memoryTypesSupported),
      "dedicateToArray", NULL,
      "dedicateToImage", colorMsaa[i].handle,
      "memoryBitflags", 0,
      "outMemory", &colorMsaaMemory[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    RedMemoryImage memorysImages[2] = {0};
    memorysImages[0].setTo1000157001  = 1000157001;
    memorysImages[0].setTo0           = 0;
    memorysImages[0].image            = depthMsaa[i].handle;
    memorysImages[0].memory           = depthMsaaMemory[i];
    memorysImages[0].memoryBytesFirst = 0;
    memorysImages[1].setTo1000157001  = 1000157001;
    memorysImages[1].setTo0           = 0;
    memorysImages[1].image            = colorMsaa[i].handle;
    memorysImages[1].memory           = colorMsaaMemory[i];
    memorysImages[1].memoryBytesFirst = 0;
    np(redMemorySet,
      "context", context,
      "gpu", gpu,
      "memoryArraysCount", 0,
      "memoryArrays", NULL,
      "memoryImagesCount", _countof(memorysImages),
      "memoryImages", memorysImages,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateTexture,
      "context", context,
      "gpu", gpu,
      "handleName", "depthMsaaTexture",
      "image", depthMsaa[i].handle,
      "parts", RED_IMAGE_PART_BITFLAG_DEPTH,
      "dimensions", RED_TEXTURE_DIMENSIONS_2D,
      "format", RED_FORMAT_DEPTH_32_FLOAT,
      "levelsFirst", 0,
      "levelsCount", 1,
      "layersFirst", 0,
      "layersCount", 1,
      "restrictToAccess", 0,
      "outTexture", &depthMsaaTexture[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateTexture,
      "context", context,
      "gpu", gpu,
      "handleName", "colorMsaaTexture",
      "image", colorMsaa[i].handle,
      "parts", RED_IMAGE_PART_BITFLAG_COLOR,
      "dimensions", RED_TEXTURE_DIMENSIONS_2D,
      "format", RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1,
      "levelsFirst", 0,
      "levelsCount", 1,
      "layersFirst", 0,
      "layersCount", 1,
      "restrictToAccess", 0,
      "outTexture", &colorMsaaTexture[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    redHelperImageSetStateUsable(
      context,
      gpu,
      depthMsaa[i].handle,
      RED_IMAGE_PART_BITFLAG_DEPTH,
      queue,
      queueFamilyIndex,
      NULL,
      __FILE__,
      __LINE__,
      NULL
    );
    redHelperImageSetStateUsable(
      context,
      gpu,
      colorMsaa[i].handle,
      RED_IMAGE_PART_BITFLAG_COLOR,
      queue,
      queueFamilyIndex,
      NULL,
      __FILE__,
      __LINE__,
      NULL
    );
  }

  RedHandleGpuSignal gpusigPresent [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleCpuSignal cpusig        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleGpuSignal gpusig        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    np(redCreateGpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "gpusigPresent",
      "outGpuSignal", &gpusigPresent[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateCpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "cpusig",
      "createSignaled", 1,
      "outCpuSignal", &cpusig[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateGpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "gpusig",
      "outGpuSignal", &gpusig[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedCalls calls[REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    np(redCreateCalls,
      "context", context,
      "gpu", gpu,
      "handleName", "calls",
      "queueFamilyIndex", queueFamilyIndex,
      "outCalls", &calls[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedOutputDeclarationMembers outputDeclarationMembers = {0};
  outputDeclarationMembers.depthStencilEnable                        = 1;
  outputDeclarationMembers.depthStencilFormat                        = RED_FORMAT_DEPTH_32_FLOAT;
  outputDeclarationMembers.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilSharesMemoryWithAnotherMember = 0;
  outputDeclarationMembers.colorsCount                               = 1;
  outputDeclarationMembers.colorsFormat[0]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[1]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[2]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[3]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[4]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[5]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[6]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[7]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsMultisampleCount[0]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[1]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[2]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[3]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[4]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[5]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[6]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsMultisampleCount[7]                 = RED_MULTISAMPLE_COUNT_BITFLAG_2;
  outputDeclarationMembers.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[1]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[2]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[3]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[4]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[5]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[6]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsSetProcedureOutputOp[7]             = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
  outputDeclarationMembers.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[1]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[2]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[3]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[4]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[5]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[6]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[7]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[0]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[1]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[2]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[3]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[4]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[5]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[6]    = 0;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[7]    = 0;
  RedOutputDeclarationMembersResolveSources resolveSources = {0};
  resolveSources.resolveModeDepth    = RED_RESOLVE_MODE_NONE;
  resolveSources.resolveModeStencil  = RED_RESOLVE_MODE_NONE;
  resolveSources.resolveDepthStencil = 0;
  resolveSources.resolveColors       = 1;
  RedHandleOutputDeclaration outputDeclaration = NULL;
  np(redCreateOutputDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", "outputDeclaration",
    "outputDeclarationMembers", &outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", &resolveSources,
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "outOutputDeclaration", &outputDeclaration,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedOutput rendertargets[REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (int i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    RedOutputMembers outputMembers = {0};
    outputMembers.depthStencil = depthMsaaTexture[i];
    outputMembers.colorsCount  = 1;
    outputMembers.colors[0]    = colorMsaaTexture[i];
    outputMembers.colors[1]    = NULL;
    outputMembers.colors[2]    = NULL;
    outputMembers.colors[3]    = NULL;
    outputMembers.colors[4]    = NULL;
    outputMembers.colors[5]    = NULL;
    outputMembers.colors[6]    = NULL;
    outputMembers.colors[7]    = NULL;
    RedOutputMembersResolveTargets outputMembersResolveTargets = {0};
    outputMembersResolveTargets.depthStencil = NULL;
    outputMembersResolveTargets.colors[0]    = presentTextures[i];
    outputMembersResolveTargets.colors[1]    = NULL;
    outputMembersResolveTargets.colors[2]    = NULL;
    outputMembersResolveTargets.colors[3]    = NULL;
    outputMembersResolveTargets.colors[4]    = NULL;
    outputMembersResolveTargets.colors[5]    = NULL;
    outputMembersResolveTargets.colors[6]    = NULL;
    outputMembersResolveTargets.colors[7]    = NULL;
    np(redCreateOutput,
      "context", context,
      "gpu", gpu,
      "handleName", NULL,
      "outputDeclaration", outputDeclaration,
      "outputMembers", &outputMembers,
      "outputMembersResolveTargets", &outputMembersResolveTargets,
      "width", windowWidth,
      "height", windowHeight,
      "outOutput", &rendertargets[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedStructDeclarationMember structDeclarationSamplersMembers[1] = {0};
  structDeclarationSamplersMembers[0].slot            = 2;
  structDeclarationSamplersMembers[0].type            = RED_STRUCT_MEMBER_TYPE_SAMPLER;
  structDeclarationSamplersMembers[0].count           = 1;
  structDeclarationSamplersMembers[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationSamplersMembers[0].inlineSampler   = NULL;
  RedHandleStructDeclaration structDeclarationSamplers = NULL;
  np(redCreateStructDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", "structDeclarationSamplers",
    "structDeclarationMembersCount", _countof(structDeclarationSamplersMembers),
    "structDeclarationMembers", structDeclarationSamplersMembers,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "procedureParametersHandlesDeclaration", 0,
    "outStructDeclaration", &structDeclarationSamplers,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedStructDeclarationMember structDeclarationMembers[3] = {0};
  structDeclarationMembers[0].slot            = 0;
  structDeclarationMembers[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structDeclarationMembers[0].count           = 1;
  structDeclarationMembers[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationMembers[0].inlineSampler   = NULL;
  structDeclarationMembers[1].slot            = 1;
  structDeclarationMembers[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structDeclarationMembers[1].count           = 1;
  structDeclarationMembers[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationMembers[1].inlineSampler   = NULL;
  structDeclarationMembers[2].slot            = 3;
  structDeclarationMembers[2].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
  structDeclarationMembers[2].count           = 4;
  structDeclarationMembers[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationMembers[2].inlineSampler   = NULL;
  RedStructDeclarationMemberArrayRO arrayROs[2] = {0};
  arrayROs[0].slot = 0;
  arrayROs[1].slot = 1;
  RedHandleStructDeclaration structDeclaration = NULL;
  np(redCreateStructDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", "structDeclaration",
    "structDeclarationMembersCount", _countof(structDeclarationMembers),
    "structDeclarationMembers", structDeclarationMembers,
    "structDeclarationMembersArrayROCount", _countof(arrayROs),
    "structDeclarationMembersArrayRO", arrayROs,
    "procedureParametersHandlesDeclaration", 0,
    "outStructDeclaration", &structDeclaration,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedProcedureParametersDeclaration procedureParametersDeclaration = {0};
  procedureParametersDeclaration.variablesSlot            = 4;
  procedureParametersDeclaration.variablesVisibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  procedureParametersDeclaration.variablesBytesCount      = 4 * sizeof(float) + 4 * sizeof(float) + 2 * sizeof(unsigned);
  procedureParametersDeclaration.structsDeclarationsCount = 2;
  procedureParametersDeclaration.structsDeclarations[0]   = structDeclaration;
  procedureParametersDeclaration.structsDeclarations[1]   = structDeclarationSamplers;
  procedureParametersDeclaration.structsDeclarations[2]   = NULL;
  procedureParametersDeclaration.structsDeclarations[3]   = NULL;
  procedureParametersDeclaration.structsDeclarations[4]   = NULL;
  procedureParametersDeclaration.structsDeclarations[5]   = NULL;
  procedureParametersDeclaration.structsDeclarations[6]   = NULL;
  procedureParametersDeclaration.handlesDeclaration       = NULL;
  RedHandleProcedureParameters procedureParameters = NULL;
  np(redCreateProcedureParameters,
    "context", context,
    "gpu", gpu,
    "handleName", "procedureParameters",
    "procedureParametersDeclaration", &procedureParametersDeclaration,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleStructsMemory structsMemorySamplers = NULL;
  np(redStructsMemoryAllocateSamplers,
    "context", context,
    "gpu", gpu,
    "handleName", "structsMemorySamplers",
    "maxStructsCount", 2,
    "maxStructsMembersOfTypeSamplerCount", 1 * 2,
    "outStructsMemory", &structsMemorySamplers,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleStructsMemory structsMemory = NULL;
  np(redStructsMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "structsMemory",
    "maxStructsCount", 2,
    "maxStructsMembersOfTypeArrayROConstantCount", 0,
    "maxStructsMembersOfTypeArrayROOrArrayRWCount", 2 * 2,
    "maxStructsMembersOfTypeTextureROCount", 4 * 2,
    "maxStructsMembersOfTypeTextureRWCount", 0,
    "outStructsMemory", &structsMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleStruct structsSamplers[2] = {0};
  {
    RedHandleStructDeclaration structsDeclarations[2] = {0};
    structsDeclarations[0] = structDeclarationSamplers;
    structsDeclarations[1] = structDeclarationSamplers;
    const char * structsHandleNames[2] = {0};
    structsHandleNames[0] = "structsSamplers[0]";
    structsHandleNames[1] = "structsSamplers[1]";
    np(redStructsMemorySuballocateStructs,
      "context", context,
      "gpu", gpu,
      "handleNames", structsHandleNames,
      "structsMemory", structsMemorySamplers,
      "structsDeclarationsCount", _countof(structsDeclarations),
      "structsDeclarations", structsDeclarations,
      "outStructs", structsSamplers,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedHandleStruct structs[2] = {0};
  {
    RedHandleStructDeclaration structsDeclarations[2] = {0};
    structsDeclarations[0] = structDeclaration;
    structsDeclarations[1] = structDeclaration;
    const char * structsHandleNames[2] = {0};
    structsHandleNames[0] = "structs[0]";
    structsHandleNames[1] = "structs[1]";
    np(redStructsMemorySuballocateStructs,
      "context", context,
      "gpu", gpu,
      "handleNames", structsHandleNames,
      "structsMemory", structsMemory,
      "structsDeclarationsCount", _countof(structsDeclarations),
      "structsDeclarations", structsDeclarations,
      "outStructs", structs,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  {
    RedStructMemberArray array0 = {0};
    array0.array                = meshVerticesGpu.handle;
    array0.arrayRangeBytesFirst = 0;
    array0.arrayRangeBytesCount =-1;
    RedStructMemberArray array1 = {0};
    array1.array                = meshUvsGpu.handle;
    array1.arrayRangeBytesFirst = 0;
    array1.arrayRangeBytesCount =-1;
    RedStructMemberTexture sampler = {0};
    sampler.sampler = samplerAnisotropic;
    sampler.texture = NULL;
    sampler.setTo1  = 1;
    RedStructMemberTexture textures[4] = {0};
    textures[0].sampler = NULL;
    textures[0].texture = images256x256.texture;
    textures[0].setTo1  = 1;
    textures[1].sampler = NULL;
    textures[1].texture = images128x128.texture;
    textures[1].setTo1  = 1;
    textures[2].sampler = NULL;
    textures[2].texture = images64x256.texture;
    textures[2].setTo1  = 1;
    textures[3].sampler = NULL;
    textures[3].texture = images64x64.texture;
    textures[3].setTo1  = 1;
    RedStructMember structMembers[4] = {0};
    structMembers[0].setTo35   = 35;
    structMembers[0].setTo0    = 0;
    structMembers[0].structure = structs[0];
    structMembers[0].slot      = 0;
    structMembers[0].first     = 0;
    structMembers[0].count     = 1;
    structMembers[0].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[0].textures  = NULL;
    structMembers[0].arrays    = &array0;
    structMembers[0].setTo00   = 0;
    structMembers[1].setTo35   = 35;
    structMembers[1].setTo0    = 0;
    structMembers[1].structure = structs[0];
    structMembers[1].slot      = 1;
    structMembers[1].first     = 0;
    structMembers[1].count     = 1;
    structMembers[1].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[1].textures  = NULL;
    structMembers[1].arrays    = &array1;
    structMembers[1].setTo00   = 0;
    structMembers[2].setTo35   = 35;
    structMembers[2].setTo0    = 0;
    structMembers[2].structure = structsSamplers[0];
    structMembers[2].slot      = 2;
    structMembers[2].first     = 0;
    structMembers[2].count     = 1;
    structMembers[2].type      = RED_STRUCT_MEMBER_TYPE_SAMPLER;
    structMembers[2].textures  = &sampler;
    structMembers[2].arrays    = NULL;
    structMembers[2].setTo00   = 0;
    structMembers[3].setTo35   = 35;
    structMembers[3].setTo0    = 0;
    structMembers[3].structure = structs[0];
    structMembers[3].slot      = 3;
    structMembers[3].first     = 0;
    structMembers[3].count     = _countof(textures);
    structMembers[3].type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
    structMembers[3].textures  = textures;
    structMembers[3].arrays    = NULL;
    structMembers[3].setTo00   = 0;
    np(redStructsSet,
      "context", context,
      "gpu", gpu,
      "structsMembersCount", _countof(structMembers),
      "structsMembers", structMembers,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }
  {
    RedStructMemberArray array0 = {0};
    array0.array                = meshVerticesGpu.handle;
    array0.arrayRangeBytesFirst = 0;
    array0.arrayRangeBytesCount =-1;
    RedStructMemberArray array1 = {0};
    array1.array                = meshUvsGpu.handle;
    array1.arrayRangeBytesFirst = 0;
    array1.arrayRangeBytesCount =-1;
    RedStructMemberTexture sampler = {0};
    sampler.sampler = samplerLinear;
    sampler.texture = NULL;
    sampler.setTo1  = 1;
    RedStructMemberTexture textures[4] = {0};
    textures[0].sampler = NULL;
    textures[0].texture = images256x256.texture;
    textures[0].setTo1  = 1;
    textures[1].sampler = NULL;
    textures[1].texture = images128x128.texture;
    textures[1].setTo1  = 1;
    textures[2].sampler = NULL;
    textures[2].texture = images64x256.texture;
    textures[2].setTo1  = 1;
    textures[3].sampler = NULL;
    textures[3].texture = images64x64.texture;
    textures[3].setTo1  = 1;
    RedStructMember structMembers[4] = {0};
    structMembers[0].setTo35   = 35;
    structMembers[0].setTo0    = 0;
    structMembers[0].structure = structs[1];
    structMembers[0].slot      = 0;
    structMembers[0].first     = 0;
    structMembers[0].count     = 1;
    structMembers[0].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[0].textures  = NULL;
    structMembers[0].arrays    = &array0;
    structMembers[0].setTo00   = 0;
    structMembers[1].setTo35   = 35;
    structMembers[1].setTo0    = 0;
    structMembers[1].structure = structs[1];
    structMembers[1].slot      = 1;
    structMembers[1].first     = 0;
    structMembers[1].count     = 1;
    structMembers[1].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[1].textures  = NULL;
    structMembers[1].arrays    = &array1;
    structMembers[1].setTo00   = 0;
    structMembers[2].setTo35   = 35;
    structMembers[2].setTo0    = 0;
    structMembers[2].structure = structsSamplers[1];
    structMembers[2].slot      = 2;
    structMembers[2].first     = 0;
    structMembers[2].count     = 1;
    structMembers[2].type      = RED_STRUCT_MEMBER_TYPE_SAMPLER;
    structMembers[2].textures  = &sampler;
    structMembers[2].arrays    = NULL;
    structMembers[2].setTo00   = 0;
    structMembers[3].setTo35   = 35;
    structMembers[3].setTo0    = 0;
    structMembers[3].structure = structs[1];
    structMembers[3].slot      = 3;
    structMembers[3].first     = 0;
    structMembers[3].count     = _countof(textures);
    structMembers[3].type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
    structMembers[3].textures  = textures;
    structMembers[3].arrays    = NULL;
    structMembers[3].setTo00   = 0;
    np(redStructsSet,
      "context", context,
      "gpu", gpu,
      "structsMembersCount", _countof(structMembers),
      "structsMembers", structMembers,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedHandleGpuCode gpuCodeVS = NULL;
  RedHandleGpuCode gpuCodeFS = NULL;
  {
    const char * filepath         = "geometry_vs.ir";
    long         irCodeBytesCount = 0;
    void *       irCode           = NULL;
    FILE *       irCodeFd         = fopen(filepath, "rb");

    if (irCodeFd != NULL) {
      fseek(irCodeFd, 0, SEEK_END);
      irCodeBytesCount = ftell(irCodeFd);
      fseek(irCodeFd, 0, SEEK_SET);
      irCode = malloc(irCodeBytesCount);
      fread(irCode, 1, irCodeBytesCount, irCodeFd);
      fclose(irCodeFd);
    } else {
      printf("[REDGPU]: gpuCodeVS::filepath(%s): fopen returned NULL\n", filepath);
      fflush(stdout);
    }

    np(redCreateGpuCode,
      "context", context,
      "gpu", gpu,
      "handleName", "gpuCodeVS",
      "irBytesCount", irCodeBytesCount,
      "ir", irCode,
      "outGpuCode", &gpuCodeVS,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (irCode != NULL) {
      free(irCode);
    }
  }
  {
    const char * filepath         = "geometry_fs.ir";
    long         irCodeBytesCount = 0;
    void *       irCode           = NULL;
    FILE *       irCodeFd         = fopen(filepath, "rb");

    if (irCodeFd != NULL) {
      fseek(irCodeFd, 0, SEEK_END);
      irCodeBytesCount = ftell(irCodeFd);
      fseek(irCodeFd, 0, SEEK_SET);
      irCode = malloc(irCodeBytesCount);
      fread(irCode, 1, irCodeBytesCount, irCodeFd);
      fclose(irCodeFd);
    } else {
      printf("[REDGPU]: gpuCodeFS::filepath(%s): fopen returned NULL\n", filepath);
      fflush(stdout);
    }

    np(redCreateGpuCode,
      "context", context,
      "gpu", gpu,
      "handleName", "gpuCodeFS",
      "irBytesCount", irCodeBytesCount,
      "ir", irCode,
      "outGpuCode", &gpuCodeFS,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (irCode != NULL) {
      free(irCode);
    }
  }

  RedHandleProcedure procedureDraw = NULL;
  {
    RedProcedureState state = {0};
    state.inputAssemblyTopology                          = RED_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    state.inputAssemblyPrimitiveRestartEnable            = 0;
    state.viewportDynamic                                = 0;
    state.viewportStaticX                                = 0;
    state.viewportStaticY                                = 0;
    state.viewportStaticWidth                            = windowWidth;
    state.viewportStaticHeight                           = windowHeight;
    state.viewportStaticDepthMin                         = 0;
    state.viewportStaticDepthMax                         = 1;
    state.scissorDynamic                                 = 0;
    state.scissorStaticX                                 = 0;
    state.scissorStaticY                                 = 0;
    state.scissorStaticWidth                             = windowWidth;
    state.scissorStaticHeight                            = windowHeight;
    state.rasterizationDepthClampEnable                  = 0;
    state.rasterizationDiscardAllPrimitivesEnable        = 0;
    state.rasterizationCullMode                          = RED_CULL_MODE_BACK;
    state.rasterizationFrontFace                         = RED_FRONT_FACE_CLOCKWISE;
    state.rasterizationDepthBiasEnable                   = 0;
    state.rasterizationDepthBiasDynamic                  = 0;
    state.rasterizationDepthBiasStaticConstantFactor     = 0;
    state.rasterizationDepthBiasStaticClamp              = 0;
    state.rasterizationDepthBiasStaticSlopeFactor        = 0;
    state.multisampleCount                               = RED_MULTISAMPLE_COUNT_BITFLAG_2;
    state.multisampleSampleMask                          = NULL;
    state.multisampleSampleShadingEnable                 = 0;
    state.multisampleSampleShadingMin                    = 0;
    state.multisampleAlphaToCoverageEnable               = 0;
    state.multisampleAlphaToOneEnable                    = 0;
    state.depthTestEnable                                = 1;
    state.depthTestDepthWriteEnable                      = 1;
    state.depthTestDepthCompareOp                        = RED_COMPARE_OP_GREATER_OR_EQUAL;
    state.depthTestBoundsTestEnable                      = 0;
    state.depthTestBoundsTestDynamic                     = 0;
    state.depthTestBoundsTestStaticMin                   = 0;
    state.depthTestBoundsTestStaticMax                   = 1;
    state.stencilTestEnable                              = 0;
    state.stencilTestFrontAndBackDynamicCompareMask      = 0;
    state.stencilTestFrontAndBackDynamicWriteMask        = 0;
    state.stencilTestFrontAndBackDynamicReference        = 0;
    state.stencilTestFrontStencilTestFailOp              = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontStencilTestPassDepthTestPassOp = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontStencilTestPassDepthTestFailOp = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontCompareOp                      = RED_COMPARE_OP_NEVER;
    state.stencilTestBackStencilTestFailOp               = RED_STENCIL_OP_KEEP;
    state.stencilTestBackStencilTestPassDepthTestPassOp  = RED_STENCIL_OP_KEEP;
    state.stencilTestBackStencilTestPassDepthTestFailOp  = RED_STENCIL_OP_KEEP;
    state.stencilTestBackCompareOp                       = RED_COMPARE_OP_NEVER;
    state.stencilTestFrontAndBackDynamicCompareMask      = 0;
    state.stencilTestFrontAndBackDynamicWriteMask        = 0;
    state.stencilTestFrontAndBackDynamicReference        = 0;
    state.stencilTestFrontAndBackStaticCompareMask       = 0;
    state.stencilTestFrontAndBackStaticWriteMask         = 0;
    state.stencilTestFrontAndBackStaticReference         = 0;
    state.blendLogicOpEnable                             = 0;
    state.blendLogicOp                                   = RED_LOGIC_OP_CLEAR;
    state.blendConstantsDynamic                          = 0;
    state.blendConstantsStatic[0]                        = 0;
    state.blendConstantsStatic[1]                        = 0;
    state.blendConstantsStatic[2]                        = 0;
    state.blendConstantsStatic[3]                        = 0;
    state.outputColorsCount                              = 1;
    state.outputColorsWriteMask[0]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[1]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[2]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[3]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[4]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[5]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[6]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[7]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsBlendEnable[0]                     = 0;
    state.outputColorsBlendEnable[1]                     = 0;
    state.outputColorsBlendEnable[2]                     = 0;
    state.outputColorsBlendEnable[3]                     = 0;
    state.outputColorsBlendEnable[4]                     = 0;
    state.outputColorsBlendEnable[5]                     = 0;
    state.outputColorsBlendEnable[6]                     = 0;
    state.outputColorsBlendEnable[7]                     = 0;
    state.outputColorsBlendColorFactorSource[0]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[1]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[2]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[3]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[4]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[5]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[6]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorSource[7]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[0]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[1]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[2]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[3]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[4]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[5]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[6]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[7]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorOp[0]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[1]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[2]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[3]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[4]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[5]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[6]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendColorOp[7]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaFactorSource[0]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[1]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[2]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[3]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[4]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[5]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[6]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorSource[7]          = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[0]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[1]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[2]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[3]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[4]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[5]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[6]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[7]          = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaOp[0]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[1]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[2]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[3]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[4]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[5]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[6]                    = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaOp[7]                    = RED_BLEND_OP_ADD;
    np(redCreateProcedure,
      "context", context,
      "gpu", gpu,
      "handleName", "procedureDraw",
      "procedureCache", NULL,
      "outputDeclaration", outputDeclaration,
      "procedureParameters", procedureParameters,
      "gpuCodeVertexMainProcedureName", "main",
      "gpuCodeVertex", gpuCodeVS,
      "gpuCodeFragmentMainProcedureName", "main",
      "gpuCodeFragment", gpuCodeFS,
      "state", &state,
      "stateExtension", NULL,
      "deriveBase", 0,
      "deriveFrom", NULL,
      "outProcedure", &procedureDraw,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedHandleProcedure procedureDrawBlend = NULL;
  {
    RedProcedureState state = {0};
    state.inputAssemblyTopology                          = RED_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    state.inputAssemblyPrimitiveRestartEnable            = 0;
    state.viewportDynamic                                = 0;
    state.viewportStaticX                                = 0;
    state.viewportStaticY                                = 0;
    state.viewportStaticWidth                            = windowWidth;
    state.viewportStaticHeight                           = windowHeight;
    state.viewportStaticDepthMin                         = 0;
    state.viewportStaticDepthMax                         = 1;
    state.scissorDynamic                                 = 0;
    state.scissorStaticX                                 = 0;
    state.scissorStaticY                                 = 0;
    state.scissorStaticWidth                             = windowWidth;
    state.scissorStaticHeight                            = windowHeight;
    state.rasterizationDepthClampEnable                  = 0;
    state.rasterizationDiscardAllPrimitivesEnable        = 0;
    state.rasterizationCullMode                          = RED_CULL_MODE_BACK;
    state.rasterizationFrontFace                         = RED_FRONT_FACE_CLOCKWISE;
    state.rasterizationDepthBiasEnable                   = 0;
    state.rasterizationDepthBiasDynamic                  = 0;
    state.rasterizationDepthBiasStaticConstantFactor     = 0;
    state.rasterizationDepthBiasStaticClamp              = 0;
    state.rasterizationDepthBiasStaticSlopeFactor        = 0;
    state.multisampleCount                               = RED_MULTISAMPLE_COUNT_BITFLAG_2;
    state.multisampleSampleMask                          = NULL;
    state.multisampleSampleShadingEnable                 = 0;
    state.multisampleSampleShadingMin                    = 0;
    state.multisampleAlphaToCoverageEnable               = 1;
    state.multisampleAlphaToOneEnable                    = 0;
    state.depthTestEnable                                = 1;
    state.depthTestDepthWriteEnable                      = 1;
    state.depthTestDepthCompareOp                        = RED_COMPARE_OP_GREATER_OR_EQUAL;
    state.depthTestBoundsTestEnable                      = 0;
    state.depthTestBoundsTestDynamic                     = 0;
    state.depthTestBoundsTestStaticMin                   = 0;
    state.depthTestBoundsTestStaticMax                   = 1;
    state.stencilTestEnable                              = 0;
    state.stencilTestFrontAndBackDynamicCompareMask      = 0;
    state.stencilTestFrontAndBackDynamicWriteMask        = 0;
    state.stencilTestFrontAndBackDynamicReference        = 0;
    state.stencilTestFrontStencilTestFailOp              = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontStencilTestPassDepthTestPassOp = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontStencilTestPassDepthTestFailOp = RED_STENCIL_OP_KEEP;
    state.stencilTestFrontCompareOp                      = RED_COMPARE_OP_NEVER;
    state.stencilTestBackStencilTestFailOp               = RED_STENCIL_OP_KEEP;
    state.stencilTestBackStencilTestPassDepthTestPassOp  = RED_STENCIL_OP_KEEP;
    state.stencilTestBackStencilTestPassDepthTestFailOp  = RED_STENCIL_OP_KEEP;
    state.stencilTestBackCompareOp                       = RED_COMPARE_OP_NEVER;
    state.stencilTestFrontAndBackDynamicCompareMask      = 0;
    state.stencilTestFrontAndBackDynamicWriteMask        = 0;
    state.stencilTestFrontAndBackDynamicReference        = 0;
    state.stencilTestFrontAndBackStaticCompareMask       = 0;
    state.stencilTestFrontAndBackStaticWriteMask         = 0;
    state.stencilTestFrontAndBackStaticReference         = 0;
    state.blendLogicOpEnable                             = 0;
    state.blendLogicOp                                   = RED_LOGIC_OP_CLEAR;
    state.blendConstantsDynamic                          = 0;
    state.blendConstantsStatic[0]                        = 0;
    state.blendConstantsStatic[1]                        = 0;
    state.blendConstantsStatic[2]                        = 0;
    state.blendConstantsStatic[3]                        = 0;
    state.outputColorsCount                              = 1;
    state.outputColorsWriteMask[0]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[1]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[2]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[3]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[4]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[5]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[6]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    state.outputColorsWriteMask[7]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
    for (unsigned i = 0; i < 8; i += 1) {
      state.outputColorsBlendEnable[i]                   = 1;
      state.outputColorsBlendColorFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
      state.outputColorsBlendColorFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
      state.outputColorsBlendColorOp[i]                  = RED_BLEND_OP_ADD;
      state.outputColorsBlendAlphaFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
      state.outputColorsBlendAlphaFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
      state.outputColorsBlendAlphaOp[i]                  = RED_BLEND_OP_ADD;
    }
    np(redCreateProcedure,
      "context", context,
      "gpu", gpu,
      "handleName", "procedureDrawBlend",
      "procedureCache", NULL,
      "outputDeclaration", outputDeclaration,
      "procedureParameters", procedureParameters,
      "gpuCodeVertexMainProcedureName", "main",
      "gpuCodeVertex", gpuCodeVS,
      "gpuCodeFragmentMainProcedureName", "main",
      "gpuCodeFragment", gpuCodeFS,
      "state", &state,
      "stateExtension", NULL,
      "deriveBase", 0,
      "deriveFrom", NULL,
      "outProcedure", &procedureDrawBlend,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float pos_x = 0;
  float pos_y = 0.05f;
  float pos_z = 0;
  float rot_x = 1.5707f;
  float rot_y = 0;

  unsigned F = 0;
  unsigned f = 0;

  glfwPollEvents();

  double start_mouse_x = 0;
  double start_mouse_y = 0;
  glfwGetCursorPos(window, &start_mouse_x, &start_mouse_y);
  double mouse_x_prev = start_mouse_x;
  double mouse_y_prev = start_mouse_y;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    double mouse_x      = 0;
    double mouse_y      = 0;
    float  mouse_disp_x = 0;
    float  mouse_disp_y = 0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
      mouse_disp_x = (float)(mouse_x_prev - mouse_x) * 0.0035f;
      mouse_disp_y = (float)(mouse_y - mouse_y_prev) * 0.0035f;
    } else {
      mouse_x_prev = mouse_x;
      mouse_y_prev = mouse_y;
    }

    rot_x += mouse_disp_x;
    rot_y += mouse_disp_y;

    float move_vec_x = 0;
    float move_vec_y = 0;
    float move_vec_z = 0;

    move_vec_x += glfwGetKey(window, GLFW_KEY_D);
    move_vec_x -= glfwGetKey(window, GLFW_KEY_A);

    move_vec_y += glfwGetKey(window, GLFW_KEY_E);
    move_vec_y -= glfwGetKey(window, GLFW_KEY_Q);

    move_vec_z += glfwGetKey(window, GLFW_KEY_W);
    move_vec_z -= glfwGetKey(window, GLFW_KEY_S);

    float move_vec_len = sqrtf(move_vec_x*move_vec_x + move_vec_y*move_vec_y + move_vec_z*move_vec_z);
    if (move_vec_len != 0) {
      move_vec_x /= move_vec_len;
      move_vec_y /= move_vec_len;
      move_vec_z /= move_vec_len;
    }

    {
      float rot_cos = cosf(rot_y);
      float rot_sin = sinf(rot_y);
      float pos_y = move_vec_y;
      float pos_z = move_vec_z;
      move_vec_y = pos_y * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_y * rot_sin;
    }

    {
      float rot_cos = cosf(rot_x);
      float rot_sin = sinf(rot_x);
      float pos_x = move_vec_x;
      float pos_z = move_vec_z;
      move_vec_x = pos_x * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_x * rot_sin;
    }

    pos_x += move_vec_x * 0.0015f;
    pos_y += move_vec_y * 0.0015f;
    pos_z += move_vec_z * 0.0015f;

    float cameraPosition[4] = {pos_x, pos_y, pos_z, 0};
    float cameraRotation[4] = {cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y)};

    RedHandleGpuSignal gpusigPresentInconsistent = NULL;
    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 1) {
      np(redCreateGpuSignal,
        "context", context,
        "gpu", gpu,
        "handleName", "gpusigPresent",
        "outGpuSignal", &gpusigPresentInconsistent,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
      
      F = f;
      np(redPresentGetImageIndex,
        "context", context,
        "gpu", gpu,
        "present", present,
        "signalCpuSignal", NULL,
        "signalGpuSignal", gpusigPresentInconsistent,
        "outImageIndex", &f,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
    }

    np(redCpuSignalWait,
      "context", context,
      "gpu", gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &cpusig[f],
      "waitAll", 1,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 1) {
      redDestroyGpuSignal(context, gpu, gpusigPresent[f], __FILE__, __LINE__, NULL);
      gpusigPresent[f] = gpusigPresentInconsistent;
    }

    np(redCpuSignalUnsignal,
      "context", context,
      "gpu", gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &cpusig[f],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    np(redCallsSet,
      "context", context,
      "gpu", gpu,
      "calls", calls[f].handle,
      "callsMemory", calls[f].memory,
      "callsReusable", calls[f].reusable,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    {
      {
        redCallMarkSet(callsPA.redCallMarkSet, calls[f].handle, "redCallUsageAliasOrderBarrier RED_STATE_UNUSABLE -> RED_STATE_USABLE");
        RedUsageImage imageUsage = {0};
        imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
        imageUsage.oldAccessStages        = 0;
        imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_RESOLVE;
        imageUsage.oldAccess              = 0;
        imageUsage.newAccess              = RED_ACCESS_BITFLAG_RESOLVE_TARGET_W;
        imageUsage.oldState               = RED_STATE_UNUSABLE;
        imageUsage.newState               = RED_STATE_USABLE;
        imageUsage.queueFamilyIndexSource = -1;
        imageUsage.queueFamilyIndexTarget = -1;
        imageUsage.image                  = presentImages[f];
        imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
        imageUsage.imageLevelsFirst       = 0;
        imageUsage.imageLevelsCount       = -1;
        imageUsage.imageLayersFirst       = 0;
        imageUsage.imageLayersCount       = -1;
        np(redCallUsageAliasOrderBarrier,
          "address", callsPA.redCallUsageAliasOrderBarrier,
          "calls", calls[f].handle,
          "context", context,
          "arrayUsagesCount", 0,
          "arrayUsages", NULL,
          "imageUsagesCount", 1,
          "imageUsages", &imageUsage,
          "aliasesCount", 0,
          "aliases", NULL,
          "ordersCount", 0,
          "orders", NULL,
          "dependencyByRegion", 0
        );
        redCallMarkEnd(callsPA.redCallMarkEnd, calls[f].handle);
      }

      {
        redCallMarkSet(callsPA.redCallMarkSet, calls[f].handle, "redCallProcedureIndexed batch");
        RedColorsClearValuesFloat colorsClearValuesFloat = {0};
        colorsClearValuesFloat.r[0] = 0.98f;
        colorsClearValuesFloat.g[0] = 0.98f;
        colorsClearValuesFloat.b[0] = 0.98f;
        colorsClearValuesFloat.a[0] = 1.f;
        float    depthClearValue   = 0.f;
        unsigned stencilClearValue = 0;
        np(redCallSetProcedureOutput,
          "address", callsPA.redCallSetProcedureOutput,
          "calls", calls[f].handle,
          "outputDeclaration", outputDeclaration,
          "output", rendertargets[f].handle,
          "inlineOutput", NULL,
          "outputWidth", rendertargets[f].width,
          "outputHeight", rendertargets[f].height,
          "outputDepthStencilEnable", rendertargets[f].depthStencilEnable,
          "outputColorsCount", rendertargets[f].colorsCount,
          "depthClearValue", depthClearValue,
          "stencilClearValue", stencilClearValue,
          "colorsClearValuesFloat", &colorsClearValuesFloat,
          "colorsClearValuesSint", NULL,
          "colorsClearValuesUint", NULL
        );
        np(redCallSetStructsMemory,
          "address", callsPA.redCallSetStructsMemory,
          "calls", calls[f].handle,
          "structsMemory", structsMemory,
          "structsMemorySamplers", structsMemorySamplers
        );
        {
          npfp(redCallSetProcedure, callsPA.redCallSetProcedure,
            "calls", calls[f].handle,
            "procedureType", RED_PROCEDURE_TYPE_DRAW,
            "procedure", procedureDraw
          );
          np(redCallSetProcedureParameters,
            "address", callsPA.redCallSetProcedureParameters,
            "calls", calls[f].handle,
            "procedureType", RED_PROCEDURE_TYPE_DRAW,
            "procedureParameters", procedureParameters
          );
          RedHandleStruct structsToSet[2] = {0};
          structsToSet[0] = structs[0];
          structsToSet[1] = structsSamplers[0];
          npfp(redCallSetProcedureParametersStructs, callsPA.redCallSetProcedureParametersStructs,
            "calls", calls[f].handle,
            "procedureType", RED_PROCEDURE_TYPE_DRAW,
            "procedureParameters", procedureParameters,
            "procedureParametersDeclarationStructsDeclarationsFirst", 0,
            "structsCount", _countof(structsToSet),
            "structs", structsToSet,
            "setTo0", 0,
            "setTo00", 0
          );
          npfp(redCallSetProcedureIndices, callsPA.redCallSetProcedureIndices,
            "calls", calls[f].handle,
            "array", meshIndicesGpu.handle,
            "setTo0", 0,
            "setTo1", 1
          );
          npfp(redCallSetProcedureParametersVariables, callsPA.redCallSetProcedureParametersVariables,
            "calls", calls[f].handle,
            "procedureParameters", procedureParameters,
            "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
            "variablesBytesFirst", 0,
            "dataBytesCount", 4 * sizeof(float),
            "data", cameraPosition
          );
          npfp(redCallSetProcedureParametersVariables, callsPA.redCallSetProcedureParametersVariables,
            "calls", calls[f].handle,
            "procedureParameters", procedureParameters,
            "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
            "variablesBytesFirst", 4 * sizeof(float),
            "dataBytesCount", 4 * sizeof(float),
            "data", cameraRotation
          );
          for (unsigned i = 0; i < GLOBAL_SPONZA_MESHES_SUBMESH_COUNT; i += 1) {
            if (// Foliage:
                i == GLOBAL_SPONZA_MESHES_SUBMESH_370_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_369_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_368_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_367_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_366_001 ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_01_001  || i == GLOBAL_SPONZA_MESHES_SUBMESH_372_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_371_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_276     || i == GLOBAL_SPONZA_MESHES_SUBMESH_275     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_281     || i == GLOBAL_SPONZA_MESHES_SUBMESH_00      || i == GLOBAL_SPONZA_MESHES_SUBMESH_280     || i == GLOBAL_SPONZA_MESHES_SUBMESH_279     || i == GLOBAL_SPONZA_MESHES_SUBMESH_278     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_277     ||
                // Chains:
                i == GLOBAL_SPONZA_MESHES_SUBMESH_341     || i == GLOBAL_SPONZA_MESHES_SUBMESH_339     || i == GLOBAL_SPONZA_MESHES_SUBMESH_340     || i == GLOBAL_SPONZA_MESHES_SUBMESH_342     || i == GLOBAL_SPONZA_MESHES_SUBMESH_333     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_331     || i == GLOBAL_SPONZA_MESHES_SUBMESH_332     || i == GLOBAL_SPONZA_MESHES_SUBMESH_330     || i == GLOBAL_SPONZA_MESHES_SUBMESH_360     || i == GLOBAL_SPONZA_MESHES_SUBMESH_358     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_357     || i == GLOBAL_SPONZA_MESHES_SUBMESH_359     || i == GLOBAL_SPONZA_MESHES_SUBMESH_348     || i == GLOBAL_SPONZA_MESHES_SUBMESH_350     || i == GLOBAL_SPONZA_MESHES_SUBMESH_351     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_349)
            {
              continue;
            } else {
            }
            unsigned textureSelectIndices[2] = {0};
            textureSelectIndices[0] = GlobalSponzaMeshesTexturesArrayIndices[i];
            textureSelectIndices[1] = GlobalSponzaMeshesTexturesLayerIndices[i];
            if (textureSelectIndices[1] == (unsigned)-1) {
              textureSelectIndices[1] = 13; // NOTE: An arbitrary choice
            }
            npfp(redCallSetProcedureParametersVariables, callsPA.redCallSetProcedureParametersVariables,
              "calls", calls[f].handle,
              "procedureParameters", procedureParameters,
              "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
              "variablesBytesFirst", 4 * sizeof(float) + 4 * sizeof(float),
              "dataBytesCount", 2 * sizeof(unsigned),
              "data", textureSelectIndices
            );
            redCallMarkSet(callsPA.redCallMarkSet, calls[f].handle, "redCallProcedureIndexed");
            npfp(redCallProcedureIndexed, callsPA.redCallProcedureIndexed,
              "calls", calls[f].handle,
              "indexCount", GlobalSponzaMeshesSubmeshVertexEnd[i] - GlobalSponzaMeshesSubmeshVertexBegin[i],
              "instanceCount", 1,
              "indexFirst", GlobalSponzaMeshesSubmeshVertexBegin[i],
              "vertexBase", 0,
              "instanceFirst", 0
            );
            redCallMarkEnd(callsPA.redCallMarkEnd, calls[f].handle);
          }
        }
        {
          npfp(redCallSetProcedure, callsPA.redCallSetProcedure,
            "calls", calls[f].handle,
            "procedureType", RED_PROCEDURE_TYPE_DRAW,
            "procedure", procedureDrawBlend
          );
          RedHandleStruct structsToSet[2] = {0};
          structsToSet[0] = structs[1];
          structsToSet[1] = structsSamplers[1];
          npfp(redCallSetProcedureParametersStructs, callsPA.redCallSetProcedureParametersStructs,
            "calls", calls[f].handle,
            "procedureType", RED_PROCEDURE_TYPE_DRAW,
            "procedureParameters", procedureParameters,
            "procedureParametersDeclarationStructsDeclarationsFirst", 0,
            "structsCount", _countof(structsToSet),
            "structs", structsToSet,
            "setTo0", 0,
            "setTo00", 0
          );
          for (unsigned i = 0; i < GLOBAL_SPONZA_MESHES_SUBMESH_COUNT; i += 1) {
            if (// Foliage:
                i == GLOBAL_SPONZA_MESHES_SUBMESH_370_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_369_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_368_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_367_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_366_001 ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_01_001  || i == GLOBAL_SPONZA_MESHES_SUBMESH_372_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_371_001 || i == GLOBAL_SPONZA_MESHES_SUBMESH_276     || i == GLOBAL_SPONZA_MESHES_SUBMESH_275     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_281     || i == GLOBAL_SPONZA_MESHES_SUBMESH_00      || i == GLOBAL_SPONZA_MESHES_SUBMESH_280     || i == GLOBAL_SPONZA_MESHES_SUBMESH_279     || i == GLOBAL_SPONZA_MESHES_SUBMESH_278     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_277     ||
                // Chains:
                i == GLOBAL_SPONZA_MESHES_SUBMESH_341     || i == GLOBAL_SPONZA_MESHES_SUBMESH_339     || i == GLOBAL_SPONZA_MESHES_SUBMESH_340     || i == GLOBAL_SPONZA_MESHES_SUBMESH_342     || i == GLOBAL_SPONZA_MESHES_SUBMESH_333     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_331     || i == GLOBAL_SPONZA_MESHES_SUBMESH_332     || i == GLOBAL_SPONZA_MESHES_SUBMESH_330     || i == GLOBAL_SPONZA_MESHES_SUBMESH_360     || i == GLOBAL_SPONZA_MESHES_SUBMESH_358     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_357     || i == GLOBAL_SPONZA_MESHES_SUBMESH_359     || i == GLOBAL_SPONZA_MESHES_SUBMESH_348     || i == GLOBAL_SPONZA_MESHES_SUBMESH_350     || i == GLOBAL_SPONZA_MESHES_SUBMESH_351     ||
                i == GLOBAL_SPONZA_MESHES_SUBMESH_349)
            {
            } else {
              continue;
            }
            unsigned textureSelectIndices[2] = {0};
            textureSelectIndices[0] = GlobalSponzaMeshesTexturesArrayIndices[i];
            textureSelectIndices[1] = GlobalSponzaMeshesTexturesLayerIndices[i];
            if (textureSelectIndices[1] == (unsigned)-1) {
              textureSelectIndices[1] = 13; // NOTE: An arbitrary choice
            }
            npfp(redCallSetProcedureParametersVariables, callsPA.redCallSetProcedureParametersVariables,
              "calls", calls[f].handle,
              "procedureParameters", procedureParameters,
              "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
              "variablesBytesFirst", 4 * sizeof(float) + 4 * sizeof(float),
              "dataBytesCount", 2 * sizeof(unsigned),
              "data", textureSelectIndices
            );
            redCallMarkSet(callsPA.redCallMarkSet, calls[f].handle, "redCallProcedureIndexed");
            npfp(redCallProcedureIndexed, callsPA.redCallProcedureIndexed,
              "calls", calls[f].handle,
              "indexCount", GlobalSponzaMeshesSubmeshVertexEnd[i] - GlobalSponzaMeshesSubmeshVertexBegin[i],
              "instanceCount", 1,
              "indexFirst", GlobalSponzaMeshesSubmeshVertexBegin[i],
              "vertexBase", 0,
              "instanceFirst", 0
            );
            redCallMarkEnd(callsPA.redCallMarkEnd, calls[f].handle);
          }
        }
        np(redCallEndProcedureOutput, "address", callsPA.redCallEndProcedureOutput, "calls", calls[f].handle);
        redCallMarkEnd(callsPA.redCallMarkEnd, calls[f].handle);
      }

      {
        redCallMarkSet(callsPA.redCallMarkSet, calls[f].handle, "redCallUsageAliasOrderBarrier RED_STATE_USABLE -> RED_STATE_PRESENT");
        RedUsageImage imageUsage = {0};
        imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
        imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_RESOLVE;
        imageUsage.newAccessStages        = 0;
        imageUsage.oldAccess              = RED_ACCESS_BITFLAG_RESOLVE_TARGET_W;
        imageUsage.newAccess              = 0;
        imageUsage.oldState               = RED_STATE_USABLE;
        imageUsage.newState               = RED_STATE_PRESENT;
        imageUsage.queueFamilyIndexSource = -1;
        imageUsage.queueFamilyIndexTarget = -1;
        imageUsage.image                  = presentImages[f];
        imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
        imageUsage.imageLevelsFirst       = 0;
        imageUsage.imageLevelsCount       = -1;
        imageUsage.imageLayersFirst       = 0;
        imageUsage.imageLayersCount       = -1;
        np(redCallUsageAliasOrderBarrier,
          "address", callsPA.redCallUsageAliasOrderBarrier,
          "calls", calls[f].handle,
          "context", context,
          "arrayUsagesCount", 0,
          "arrayUsages", NULL,
          "imageUsagesCount", 1,
          "imageUsages", &imageUsage,
          "aliasesCount", 0,
          "aliases", NULL,
          "ordersCount", 0,
          "orders", NULL,
          "dependencyByRegion", 0
        );
        redCallMarkEnd(callsPA.redCallMarkEnd, calls[f].handle);
      }
    }
    np(redCallsEnd,
      "context", context,
      "gpu", gpu,
      "calls", calls[f].handle,
      "callsMemory", calls[f].memory,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 0) {
      F = f;
      np(redPresentGetImageIndex,
        "context", context,
        "gpu", gpu,
        "present", present,
        "signalCpuSignal", NULL,
        "signalGpuSignal", gpusigPresent[F],
        "outImageIndex", &f,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
      if (F != f) {
        printf("%s\n", "[REDGPU][Assert]: Present image indices are inconsistent. Set REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES to 1 and recompile the program.\n");
        return 1;
      }
    }

    {
      RedHandleCalls callsHandles[] = {
        calls[f].handle,
      };
      RedGpuTimeline timelines[1] = {0};
      unsigned arrayOf65536[1] = {65536};
      timelines[0].setTo4                            = 4;
      timelines[0].setTo0                            = 0;
      timelines[0].waitForAndUnsignalGpuSignalsCount = 1;
      timelines[0].waitForAndUnsignalGpuSignals      = &gpusigPresent[f];
      timelines[0].setTo65536                        = arrayOf65536;
      timelines[0].callsCount                        = _countof(callsHandles);
      timelines[0].calls                             = callsHandles;
      timelines[0].signalGpuSignalsCount             = 1;
      timelines[0].signalGpuSignals                  = &gpusig[f];
      np(redQueueSubmit,
        "context", context,
        "gpu", gpu,
        "queue", queue,
        "timelinesCount", _countof(timelines),
        "timelines", timelines,
        "signalCpuSignal", cpusig[f],
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
    }

    np(redQueuePresent,
      "context", context,
      "gpu", gpu,
      "queue", queue,
      "waitForAndUnsignalGpuSignalsCount", 1,
      "waitForAndUnsignalGpuSignals", &gpusig[f],
      "presentsCount", 1,
      "presents", &present,
      "presentsImageIndex", &f,
      "outPresentsStatus", NULL,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;

    f += 1;
    f %= REDGPU_PROGRAM_PRESENT_IMAGES_COUNT;
  }

  redQueuePresent(context, gpu, queue, 0, NULL, 0, NULL, NULL, NULL, NULL, __FILE__, __LINE__, NULL);

  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", REDGPU_PROGRAM_PRESENT_IMAGES_COUNT,
    "cpuSignals", cpusig,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  redDestroyCalls(context, gpu, callsUploads.handle, callsUploads.memory, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyCalls(context, gpu, calls[i].handle, calls[i].memory, __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyGpuSignal(context, gpu, gpusig[i], __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyGpuSignal(context, gpu, gpusigPresent[i], __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyCpuSignal(context, gpu, cpusig[i], __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < images256x256.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < images256x256.imageLayersCount; j += 1) {
      redDestroyArray(context, gpu, images256x256.arrays[i][j].handle, __FILE__, __LINE__, NULL);
      redMemoryFree(context, gpu, images256x256.arraysMemory[i][j], __FILE__, __LINE__, NULL);
    }
  }
  for (unsigned i = 0; i < images128x128.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < images128x128.imageLayersCount; j += 1) {
      redDestroyArray(context, gpu, images128x128.arrays[i][j].handle, __FILE__, __LINE__, NULL);
      redMemoryFree(context, gpu, images128x128.arraysMemory[i][j], __FILE__, __LINE__, NULL);
    }
  }
  for (unsigned i = 0; i < images64x256.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < images64x256.imageLayersCount; j += 1) {
      redDestroyArray(context, gpu, images64x256.arrays[i][j].handle, __FILE__, __LINE__, NULL);
      redMemoryFree(context, gpu, images64x256.arraysMemory[i][j], __FILE__, __LINE__, NULL);
    }
  }
  for (unsigned i = 0; i < images64x64.imageLevelsCount; i += 1) {
    for (unsigned j = 0; j < images64x64.imageLayersCount; j += 1) {
      redDestroyArray(context, gpu, images64x64.arrays[i][j].handle, __FILE__, __LINE__, NULL);
      redMemoryFree(context, gpu, images64x64.arraysMemory[i][j], __FILE__, __LINE__, NULL);
    }
  }
  redDestroyTexture(context, gpu, images256x256.texture, __FILE__, __LINE__, NULL);
  redDestroyTexture(context, gpu, images128x128.texture, __FILE__, __LINE__, NULL);
  redDestroyTexture(context, gpu, images64x256.texture, __FILE__, __LINE__, NULL);
  redDestroyTexture(context, gpu, images64x64.texture, __FILE__, __LINE__, NULL);
  redDestroyImage(context, gpu, images256x256.image.handle, __FILE__, __LINE__, NULL);
  redDestroyImage(context, gpu, images128x128.image.handle, __FILE__, __LINE__, NULL);
  redDestroyImage(context, gpu, images64x256.image.handle, __FILE__, __LINE__, NULL);
  redDestroyImage(context, gpu, images64x64.image.handle, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, images256x256.imageMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, images128x128.imageMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, images64x256.imageMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, images64x64.imageMemory, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshIndicesCpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshIndicesGpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshVerticesCpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshVerticesGpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshUvsCpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(context, gpu, meshUvsGpu.handle, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshIndicesCpuMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshIndicesGpuMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshVerticesCpuMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshVerticesGpuMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshUvsCpuMemory, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, meshUvsGpuMemory, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyTexture(context, gpu, depthMsaaTexture[i], __FILE__, __LINE__, NULL);
    redDestroyTexture(context, gpu, colorMsaaTexture[i], __FILE__, __LINE__, NULL);
    redDestroyImage(context, gpu, depthMsaa[i].handle, __FILE__, __LINE__, NULL);
    redDestroyImage(context, gpu, colorMsaa[i].handle, __FILE__, __LINE__, NULL);
    redMemoryFree(context, gpu, depthMsaaMemory[i], __FILE__, __LINE__, NULL);
    redMemoryFree(context, gpu, colorMsaaMemory[i], __FILE__, __LINE__, NULL);
  }
  redDestroySampler(context, gpu, samplerAnisotropic, __FILE__, __LINE__, NULL);
  redDestroySampler(context, gpu, samplerLinear, __FILE__, __LINE__, NULL);
  redDestroyGpuCode(context, gpu, gpuCodeVS, __FILE__, __LINE__, NULL);
  redDestroyGpuCode(context, gpu, gpuCodeFS, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyOutput(context, gpu, rendertargets[i].handle, __FILE__, __LINE__, NULL);
  }
  redDestroyOutputDeclaration(context, gpu, outputDeclaration, __FILE__, __LINE__, NULL);
  redDestroyStructDeclaration(context, gpu, structDeclaration, __FILE__, __LINE__, NULL);
  redDestroyStructDeclaration(context, gpu, structDeclarationSamplers, __FILE__, __LINE__, NULL);
  redStructsMemoryFree(context, gpu, structsMemory, __FILE__, __LINE__, NULL);
  redStructsMemoryFree(context, gpu, structsMemorySamplers, __FILE__, __LINE__, NULL);
  redDestroyProcedureParameters(context, gpu, procedureParameters, __FILE__, __LINE__, NULL);
  redDestroyProcedure(context, gpu, procedureDraw, __FILE__, __LINE__, NULL);
  redDestroyProcedure(context, gpu, procedureDrawBlend, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyTexture(context, gpu, presentTextures[i], __FILE__, __LINE__, NULL);
  }
  redDestroyPresent(context, gpu, present, __FILE__, __LINE__, NULL);
  redDestroySurface(context, gpu, surface, __FILE__, __LINE__, NULL);

  redDestroyContext(context, __FILE__, __LINE__, NULL);
}
