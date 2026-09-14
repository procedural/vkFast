#include <vector>
#include <string>
#include <fstream>
#include <sstream>

int main() {
  const int maxParametersCount = 50;

  printf("#pragma once\n");
  printf("\n");

  // np() metamacro that gets lowered to np0(), np1(), np2(), etc.
  {
    printf("#define __NP_COUNTOF_CAT( a, b ) a b\n");
    printf("#define __NP_COUNTOF_A( \\\n  ");
    for (int i = 0; i < (1 + 1 + (maxParametersCount * 2)); i += 1) {
      printf("a%d, ", i);
    }
    printf("\\\n  n, ... ) n\n");
    printf("#define __NP_COUNTOF(...) __NP_COUNTOF_CAT( __NP_COUNTOF_A, ( 0, ##__VA_ARGS__, \\\n  ");
    for (int i = 0; i < maxParametersCount; i += 1) {
      printf("%d, 0, ", maxParametersCount - i);
    }
    printf("0, 0 ) )\n");
    printf("#define __NP_VA_EXPAND(name, ...)        __NP_VA_EXPAND_JOIN(name, __NP_COUNTOF(__VA_ARGS__))(__VA_ARGS__)\n");
    printf("#define __NP_VA_EXPAND_JOIN(name, count) __NP_VA_EXPAND_J0IN(name, count)\n");
    printf("#define __NP_VA_EXPAND_J0IN(name, count) __NP_VA_EXPAND_J01N(name, count)\n");
    printf("#define __NP_VA_EXPAND_J01N(name, count) name##count\n");
    printf("#define np(...) __NP_VA_EXPAND(np, __VA_ARGS__)\n");
  }
  printf("\n");

  // npfp() metamacro that gets lowered to npfp0(), npfp1(), npfp2(), etc.
  {
    printf("#define __NPFP_COUNTOF_CAT( a, b ) a b\n");
    printf("#define __NPFP_COUNTOF_A( \\\n  ");
    for (int i = 0; i < (1 + 2 + (maxParametersCount * 2)); i += 1) {
      printf("a%d, ", i);
    }
    printf("\\\n  n, ... ) n\n");
    printf("#define __NPFP_COUNTOF(...) __NPFP_COUNTOF_CAT( __NPFP_COUNTOF_A, ( 0, ##__VA_ARGS__, \\\n  ");
    for (int i = 0; i < maxParametersCount; i += 1) {
      printf("%d, 0, ", maxParametersCount - i);
    }
    printf("0, 0, 0 ) )\n");
    printf("#define __NPFP_VA_EXPAND(name, ...)        __NPFP_VA_EXPAND_JOIN(name, __NPFP_COUNTOF(__VA_ARGS__))(__VA_ARGS__)\n");
    printf("#define __NPFP_VA_EXPAND_JOIN(name, count) __NPFP_VA_EXPAND_J0IN(name, count)\n");
    printf("#define __NPFP_VA_EXPAND_J0IN(name, count) __NPFP_VA_EXPAND_J01N(name, count)\n");
    printf("#define __NPFP_VA_EXPAND_J01N(name, count) name##count\n");
    printf("#define npfp(...) __NPFP_VA_EXPAND(npfp, __VA_ARGS__)\n");
  }
  printf("\n");

  printf("#ifdef REDGPU_DISABLE_NAMED_PARAMETERS\n");
  printf("\n");
  {
    printf("#define np0(ProcedureName) ProcedureName()\n");
    printf("\n");

    for (int i = 0; i < maxParametersCount; i += 1) {

      printf("#define np%d(ProcedureName, \\\n", i + 1);

      for (int j = 0; j < (i + 1); j += 1) {
        printf("  p%d_PassedName, p%d", j + 1, j + 1);
        if (j != i) {
          printf(",");
        }
        printf(" \\\n");
      }

      printf(") \\\n");
      printf("  ProcedureName(");
      for (int j = 0; j < (i + 1); j += 1) {
        if (j != 0) printf(", ");
        printf("p%d", j + 1);
      }
      printf(")\n");

      printf("\n");
    }

    // NOTE(Constantine): Aug 07, 2025: function pointer np versions.
    {
      printf("#define npfp0(ProcedureName, ProcedureCall) ProcedureCall()\n");
      printf("\n");

      for (int i = 0; i < maxParametersCount; i += 1) {

        printf("#define npfp%d(ProcedureName, ProcedureCall, \\\n", i + 1);

        for (int j = 0; j < (i + 1); j += 1) {
          printf("  p%d_PassedName, p%d", j + 1, j + 1);
          if (j != i) {
            printf(",");
          }
          printf(" \\\n");
        }

        printf(") \\\n");
        printf("  ProcedureCall(");
        for (int j = 0; j < (i + 1); j += 1) {
          if (j != 0) printf(", ");
          printf("p%d", j + 1);
        }
        printf(")\n");

        printf("\n");
      }
    }
  }
  printf("#else // #ifdef REDGPU_DISABLE_NAMED_PARAMETERS\n");
  printf("\n");
  {
    printf("#include <string_view> // NOTE(Constantine): std::string_view requires C++17.\n");
    printf("\n");
    printf("#define NP_STRING(a) #a\n");
    printf("#define NP_STRINGIFY(a) NP_STRING(a)\n");
    printf("\n");
    printf("#define np0(ProcedureName) ProcedureName()\n");
    printf("\n");

    for (int i = 0; i < maxParametersCount; i += 1) {

      printf("#define np%d(ProcedureName, \\\n", i + 1);

      for (int j = 0; j < (i + 1); j += 1) {
        printf("  p%d_PassedName, p%d", j + 1, j + 1);
        if (j != i) {
          printf(",");
        }
        printf(" \\\n");
      }

      printf(") \\\n");
      printf("  ProcedureName(");
      for (int j = 0; j < (i + 1); j += 1) {
        if (j != 0) printf(", ");
        printf("p%d", j + 1);
      }
      printf("); \\\n");

      for (int j = 0; j < (i + 1); j += 1) {
        printf("  static_assert(std::string_view(p%d_PassedName) == NP_STRINGIFY(_np%d_##ProcedureName), \"Expected parameter name: \" #ProcedureName \"(\" NP_STRINGIFY(_np%d_##ProcedureName) \": ...), got: \" #ProcedureName \"(\" p%d_PassedName \": ...)\")", j + 1, j + 1, j + 1, j + 1);
        if (j != i) {
          printf("; \\");
        }
        printf("\n");
      }

      printf("\n");
    }

    // NOTE(Constantine): Aug 07, 2025: function pointer np versions.
    {
      printf("#define npfp0(ProcedureName, ProcedureCall) ProcedureCall()\n");
      printf("\n");

      for (int i = 0; i < maxParametersCount; i += 1) {

        printf("#define npfp%d(ProcedureName, ProcedureCall, \\\n", i + 1);

        for (int j = 0; j < (i + 1); j += 1) {
          printf("  p%d_PassedName, p%d", j + 1, j + 1);
          if (j != i) {
            printf(",");
          }
          printf(" \\\n");
        }

        printf(") \\\n");
        printf("  ProcedureCall(");
        for (int j = 0; j < (i + 1); j += 1) {
          if (j != 0) printf(", ");
          printf("p%d", j + 1);
        }
        printf("); \\\n");

        for (int j = 0; j < (i + 1); j += 1) {
          printf("  static_assert(std::string_view(p%d_PassedName) == NP_STRINGIFY(_np%d_##ProcedureName), \"Expected parameter name: \" #ProcedureName \"(\" NP_STRINGIFY(_np%d_##ProcedureName) \": ...), got: \" #ProcedureName \"(\" p%d_PassedName \": ...)\")", j + 1, j + 1, j + 1, j + 1);
          if (j != i) {
            printf("; \\");
          }
          printf("\n");
        }

        printf("\n");
      }
    }
  }
  printf("#endif // #ifdef REDGPU_DISABLE_NAMED_PARAMETERS\n");
  printf("\n");
}
