#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RedProcedureId1 {
  RED_PROCEDURE_ID_redCreateProcedureComputingLanguage = 76,
} RedProcedureId1;

// redCreateContext

typedef enum RedSdkExtension1 {
  RED_SDK_EXTENSION_COMPUTING_LANGUAGE_FEATURE_LEVEL_1 = 14,
} RedSdkExtension1;

typedef enum RedGpuInfoOptionalInfo1 {
  RED_GPU_INFO_OPTIONAL_INFO_COMPUTING_LANGUAGE_FEATURE_LEVEL_1 = 14,
} RedGpuInfoOptionalInfo1;

typedef struct RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1 {
  RedGpuInfoOptionalInfo1 optionalInfo;
  const void *            next;
  RedBool32               supportsComputingLanguageFeatureLevel1;
} RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1;

// redCreateProcedureComputingLanguage

typedef enum RedProcedureComputingLanguageStateExtension {
  RED_PROCEDURE_COMPUTING_LANGUAGE_STATE_EXTENSION_UNDEFINED                = 0,
  RED_PROCEDURE_COMPUTING_LANGUAGE_STATE_EXTENSION_SPECIALIZATION_CONSTANTS = 1,
} RedProcedureComputingLanguageStateExtension;

typedef struct RedProcedureComputingLanguageStateExtensionIterator {
  unsigned     extension;
  const void * next;
} RedProcedureComputingLanguageStateExtensionIterator;

typedef struct RedProcedureComputingLanguageSpecializationConstant {
  unsigned specId;
  unsigned specDataBytesFirst;
  size_t   specDataBytesCount;
} RedProcedureComputingLanguageSpecializationConstant;

typedef struct RedProcedureComputingLanguageStateExtensionSpecializationConstants {
  RedProcedureComputingLanguageStateExtension                 extension;
  const void *                                                next;
  unsigned                                                    specsCount;
  const RedProcedureComputingLanguageSpecializationConstant * specs;
  size_t                                                      specDataBytesCount;
  const void *                                                specData;
} RedProcedureComputingLanguageStateExtensionSpecializationConstants;

REDGPU_DECLSPEC void REDGPU_API redCreateProcedureComputingLanguage (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleProcedureParameters procedureParameters, const char * gpuCodeMainProcedureName, RedHandleGpuCode gpuCode, const void * stateExtension, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

#ifdef __cplusplus
}
#endif
