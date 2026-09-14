Jun 27, 2026:

  * Header include paths are now relative.

Jun 21, 2026:

  * Fix redgpu.cpp for modern compilers that refuse to compile old goto code.

Jun 01, 2026:

  * Bazzite/SteamOS is the only supported Linux distro now. RedGpuSDK now must be placed not in /opt/ folder, but in /home/linuxbrew/ folder.

May 15, 2026:

  * Fixed bugs where VK_ATTACHMENT_*_OP_DONT_CARE flags could cause garbage writes to textures.

Sep 18, 2025:

  * redgpu.h: redCreateArray::initialAccess parameter is removed and replaced with redCreateArray::restrictToAccess parameter.
  * redgpu.h: redCreateImage::initialAccess parameter is removed.
  * Update the examples.

Sep 06, 2025:

  * Fixed bugs where programs could crash on resources that couldn't be created due to RED_STATUS_ERROR_OUT_OF_GPU_MEMORY.

Aug 25, 2025:

  * REDGPU debug library: unsigned long is not enough to print max bytes counts, changing all of them to print as uint64_t.

Aug 24, 2025:

  * Make all code compile as C by default.
  * Named parameters generator now accepts filename as an argument.
  * Add np() and npfp() named parameters metamacros.

Aug 08, 2025:

  * Added more examples to RedGpuSDK/misc/examples/ folder.
  * Added generation of npNfp macros to RedGpuSDK/misc/npgenerator/npgenerator_np_h_generator.cpp to call function pointers with named parameters.

Aug 07, 2025:

  * REDGPU X backend is removed.
  * REDGPU SDK folders and files structures are changed.
  * REDGPU implementation files, named parameters and named parameters generator are imported from REDGPU 2 git repo, commit 7f417c01ca0036abcfe35fdce00593583f2d6170.
  * REDGPU implementation: on Windows, C:\Windows\System32\vulkan-1.dll is loaded at runtime now, no need to link vulkan-1.lib anymore.
  * Small changes and cleanups to implementation only, no API changes were made.
  * Added RedGpuSDK/misc/examples/ folder.

May 20, 2023:

  * `RED_CONTEXT_OPTIONAL_SETTINGS_CREATE_CONTEXT_PERFORMANCE` enum value is changed to `4`.

May 18, 2023:

  * **NEW**: redgpu_tracing libraries to generate `about:tracing` profiling data.
  * **NEW**: `redStructsMemoryAllocateWithInlineSamplers` and `redStructsMemoryAllocateSamplersWithInlineSamplers` procedures are added. Some GPU vendors' drivers need to allocate extra structs memory space to hold inline samplers.
  * **NEW**: `RedContextOptionalSettingsCreateContextPerformance` structure.

Apr 10, 2023:

  * redgpu.h: RedArrayType::RED_ARRAY_TYPE_ARRAY_R0 name is corrected to RedArrayType::RED_ARRAY_TYPE_ARRAY_RO.

Dec 03, 2022:

  * Make calling redQueuePresent with redQueuePresent::waitForAndUnsignalGpuSignalsCount set to 0 and redQueuePresent::presentsCount set to 0 before calling redDestroyPresent mandatory.

Dec 01, 2022:

  * redgpu.h: redCallSetProcedureOutput::inlineOutput parameter is corrected to be const.
  * redgpu_wsi.h: new features are added, see the comments at the bottom of the header file.

Aug 13, 2022:

  * Correct RedHandleArrayTimestamp type in redgpu_array_timestamp.h for 32-bit systems.

Oct 09, 2021:

  * REDGPU log debug libraries are added, they redirect the console debug print output to /tmp/redgpu.txt file on Linux and redgpu.txt file located in executable's folder on Windows.
  * REDGPU debug library: context pointers are now printed as well.

Oct 07, 2021:

  * libredgpu.so is now compiled with Clang 13.
  * libredgpu.so.su static stack usage analysis file is added, it specifies the maximum amount of stack used per procedure, in bytes. (Not relevant to the public API: internally, PPHI stands for Procedure Parameters Handles Implementation.)
  * Fixed a bug when procedures could return empty RedStatuses::statusErrorDescription strings for some errors.
  * The stack size of internal statuses setting procedures is minimized.

Oct 03, 2021:

  * **NEW**: `redgpu_array_timestamp.h` extension.
  * REDGPU debug library: RedStatuses::statusFile and RedStatuses::statusErrorFile strings are now printed in quotes.

Sep 19, 2021:

  * **NEW**: `redgpu_context_from_vk.h::RedContextOptionalSettingsContextFromVkGetInstanceProcAddr` struct.

Sep 07, 2021:

  * **NEW**: `redgpu_computing_language.h` extension.
  * **NEW**: `redgpu_context_from_vk.h` context optional settings.
  * Minor improvement to internal GPU statuses setting of redCreateContext.
  * Status code info strings are now set to RedStatuses::statusErrorDescription.

Aug 25, 2021:

  * Fix redCreatePresent::outStatuses setting that could incorrectly set ::statusProcedureId and ::statusErrorProcedureId to RED_PROCEDURE_ID_redQueuePresent.
  * Internal statuses setting optimizations.

Aug 11, 2021:

  * Release REDGPU under Apache License, Version 2.0.
