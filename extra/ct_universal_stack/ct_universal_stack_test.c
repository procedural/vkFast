#include "ct_universal_stack.h"

#include <stdlib.h> // For malloc
#include <string.h> // For strcmp
#include <assert.h> // For assert
#include <stdio.h>  // For printf

#define FF __FILE__
#define LL __LINE__

typedef struct Chungus {
  uint64_t structSize;
  int age;
  char * name;
} Chungus;

typedef struct SlimChungus {
  Chungus chungus;
  unsigned char stack[512];
} SlimChungus;

typedef struct FatChungus {
  Chungus chungus;
  unsigned char stack[128000];
} FatChungus;

int main() {
  uint64_t AllocationSize = (4 * sizeof(SlimChungus)) + (1 * sizeof(FatChungus));
  unsigned char * Allocation = (unsigned char *)malloc(AllocationSize);

  CtUniStack Chunguses = {0};
  Chunguses.base = Allocation;
  Chunguses.bytesCountMax = AllocationSize;

  CT_UNISTACK_FN_TYPE_ctUniStackReset            stackReset            = ctUniStackReset;
  CT_UNISTACK_FN_TYPE_ctUniStackResetFL          stackResetFL          = ctUniStackResetFL;
  CT_UNISTACK_FN_TYPE_ctUniStackPush             stackPush             = ctUniStackPush;
  CT_UNISTACK_FN_TYPE_ctUniStackPushFL           stackPushFL           = ctUniStackPushFL;
  CT_UNISTACK_FN_TYPE_ctUniStackView             stackView             = ctUniStackView;
  CT_UNISTACK_FN_TYPE_ctUniStackViewFL           stackViewFL           = ctUniStackViewFL;
  CT_UNISTACK_FN_TYPE_ctUniStackViewAndAdvance   stackViewAndAdvance   = ctUniStackViewAndAdvance;
  CT_UNISTACK_FN_TYPE_ctUniStackViewAndAdvanceFL stackViewAndAdvanceFL = ctUniStackViewAndAdvanceFL;

  assert(stackReset            != NULL);
  assert(stackResetFL          != NULL);
  assert(stackPush             != NULL);
  assert(stackPushFL           != NULL);
  assert(stackView             != NULL);
  assert(stackViewFL           != NULL);
  assert(stackViewAndAdvance   != NULL);
  assert(stackViewAndAdvanceFL != NULL);

  SlimChungus * Chungus1 = stackPushFL(&Chunguses, sizeof(SlimChungus), FF, LL);
  SlimChungus * Chungus2 = stackPushFL(&Chunguses, sizeof(SlimChungus), FF, LL);
  FatChungus  * Chungus3 = stackPushFL(&Chunguses, sizeof(FatChungus),  FF, LL);
  SlimChungus * Chungus4 = stackPushFL(&Chunguses, sizeof(SlimChungus), FF, LL);
  SlimChungus * Chungus5 = stackPushFL(&Chunguses, sizeof(SlimChungus), FF, LL);

  Chungus1->chungus.structSize = sizeof(SlimChungus);
  Chungus2->chungus.structSize = sizeof(SlimChungus);
  Chungus3->chungus.structSize = sizeof(FatChungus);
  Chungus4->chungus.structSize = sizeof(SlimChungus);
  Chungus5->chungus.structSize = sizeof(SlimChungus);
  Chungus1->chungus.age = 4;
  Chungus2->chungus.age = 8;
  Chungus3->chungus.age = 15;
  Chungus4->chungus.age = 42;
  Chungus5->chungus.age = 16;
  Chungus1->chungus.name = "Bob";
  Chungus2->chungus.name = "Alice";
  Chungus3->chungus.name = "Jeff";
  Chungus4->chungus.name = "Jack";
  Chungus5->chungus.name = "Dan";

  Chungus * ourChungus = NULL;

  for (uint64_t i = 0, offset = 0; i < 5; i += 1) {
    Chungus * chungus = (Chungus *)(void *)&(Chunguses.base[offset]);

    if (strcmp(chungus->name, "Jack") == 0) {
      ourChungus = chungus;
      break;
    }

    offset += chungus->structSize;
  }

  printf("Our Chungus age: %d\n", ourChungus->age);

  free(Allocation);
}

void ctUniStackCallbackError(CtUniStack * stack, const char * error, const char * function, const char * file, int line) {
  printf("[ctUniStackCallbackError] Error: %s [%s:%d][%s]\n", error, file, line, function);
  exit(1);
}
