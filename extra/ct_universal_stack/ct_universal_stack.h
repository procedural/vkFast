// ct_universal_stack.h

#pragma once

#include <stdint.h> // For uint64_t

#if !defined(CT_UNISTACK_EXTERN_C)
  #if defined(__cplusplus)
    #define CT_UNISTACK_EXTERN_C extern "C"
  #else
    #define CT_UNISTACK_EXTERN_C
  #endif
#endif

#if !defined(CT_UNISTACK_API_NON_STATIC) && !defined(ALL_API_NON_STATIC)
  #define CT_UNISTACK_API_PRE  static
  #define CT_UNISTACK_API_POST
  #define CT_UNISTACK_IMPLEMENTATION
#endif

#if !defined(CT_UNISTACK_API_FN_PTR)
  #define CT_UNISTACK_API_FN_PTR
#endif

typedef struct CtUniStack {
  unsigned char * base;
  uint64_t        bytesCountMax;
  uint64_t        bytesCountTaken;
  const char *    optionalStackName;
} CtUniStack;

CT_UNISTACK_EXTERN_C void   ctUniStackCallbackError (CtUniStack * stack, const char * error, const char * function, const char * file, int line);

CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackReset   (CtUniStack * stack);
CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackResetFL (CtUniStack * stack, const char * file, int line);

CT_UNISTACK_API_PRE  void * CT_UNISTACK_API_POST ctUniStackPush    (CtUniStack * stack, uint64_t pushBytesCount);
CT_UNISTACK_API_PRE  void * CT_UNISTACK_API_POST ctUniStackPushFL  (CtUniStack * stack, uint64_t pushBytesCount, const char * file, int line);

CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackView    (CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView);
CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackViewFL  (CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line);

CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackViewAndAdvance   (CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView);
CT_UNISTACK_API_PRE  void   CT_UNISTACK_API_POST ctUniStackViewAndAdvanceFL (CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line);

typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackReset)            (CtUniStack * stack);
typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackResetFL)          (CtUniStack * stack, const char * file, int line);
typedef void * (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackPush)             (CtUniStack * stack, uint64_t pushBytesCount);
typedef void * (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackPushFL)           (CtUniStack * stack, uint64_t pushBytesCount, const char * file, int line);
typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackView)             (CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView);
typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackViewFL)           (CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line);
typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackViewAndAdvance)   (CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView);
typedef void   (CT_UNISTACK_API_FN_PTR * CT_UNISTACK_FN_TYPE_ctUniStackViewAndAdvanceFL) (CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line);

#ifdef CT_UNISTACK_IMPLEMENTATION

#include <stddef.h> // For NULL

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackResetFL(CtUniStack * stack, const char * file, int line) {
  if (stack == NULL) {
    ctUniStackCallbackError(stack, "stack == NULL", __FUNCTION__, file, line);
  }

  stack->bytesCountTaken = 0;
}

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackReset(CtUniStack * stack) {
  ctUniStackResetFL(stack, NULL, 0);
}

CT_UNISTACK_API_PRE void * CT_UNISTACK_API_POST ctUniStackPushFL(CtUniStack * stack, uint64_t pushBytesCount, const char * file, int line) {
  if (stack == NULL) {
    ctUniStackCallbackError(stack, "stack == NULL", __FUNCTION__, file, line);
  }

  if (pushBytesCount == 0) {
    return NULL; // NOTE(Constantine): Returning NULL so that different consequent pushes can't cause overlap between each other.
  }

  void * data = &stack->base[stack->bytesCountTaken];
  stack->bytesCountTaken += pushBytesCount;

  if (stack->bytesCountTaken > stack->bytesCountMax) {
    ctUniStackCallbackError(stack, "stack->bytesCountTaken > stack->bytesCountMax", __FUNCTION__, file, line);
  }

  return data;
}

CT_UNISTACK_API_PRE void * CT_UNISTACK_API_POST ctUniStackPush(CtUniStack * stack, uint64_t pushBytesCount) {
  return ctUniStackPushFL(stack, pushBytesCount, NULL, 0);
}

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackViewFL(CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line) {
  if (stack == NULL) {
    ctUniStackCallbackError(stack, "stack == NULL", __FUNCTION__, file, line);
  }

  if (viewBytesCount == 0) {
    ctUniStackCallbackError(stack, "viewBytesCount == 0", __FUNCTION__, file, line);
  }

  if ((stackOffsetBytesCount + viewBytesCount) > stack->bytesCountMax) {
    ctUniStackCallbackError(stack, "(stackOffsetBytesCount + viewBytesCount) > stack->bytesCountMax", __FUNCTION__, file, line);
  }

  unsigned char * newBase = &stack->base[stackOffsetBytesCount];

  outView->base            = newBase;
  outView->bytesCountMax   = viewBytesCount;
  outView->bytesCountTaken = 0;
}

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackView(CtUniStack * stack, uint64_t stackOffsetBytesCount, uint64_t viewBytesCount, CtUniStack * outView) {
  ctUniStackViewFL(stack, stackOffsetBytesCount, viewBytesCount, outView, NULL, 0);
}

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackViewAndAdvanceFL(CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView, const char * file, int line) {
  if (stack == NULL) {
    ctUniStackCallbackError(stack, "stack == NULL", __FUNCTION__, file, line);
  }

  ctUniStackViewFL(stack, stack->bytesCountTaken, viewBytesCount, outView, file, line);

  stack->bytesCountTaken += viewBytesCount;
}

CT_UNISTACK_API_PRE void CT_UNISTACK_API_POST ctUniStackViewAndAdvance(CtUniStack * stack, uint64_t viewBytesCount, CtUniStack * outView) {
  ctUniStackViewAndAdvanceFL(stack, viewBytesCount, outView, NULL, 0);
}

#endif // CT_UNISTACK_IMPLEMENTATION
