#include "C:/RedGpuSDK/redgpu_2.h"
#include "C:/RedGpuSDK/redgpu_32.h"

#include <stdlib.h> // For exit
#include <stdio.h>  // For printf

void red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine) {
  printf("[REDGPU 2][Crash][%s:%d][%s] %s\n", optionalFile, optionalLine, functionName, error);
  exit(1);
}

int main() {
  {
    struct StringArray {
      char * items;
      size_t count;
      size_t capacity;
      size_t alignment;
    };

    struct StringArray str = {0};

    REDGPU_32_DYNAMIC_ARRAY_RESIZE(str, 6);

    REDGPU_2_EXPECTFL( str.items     != 0 );
    REDGPU_2_EXPECTFL( str.count     == 0 );
    REDGPU_2_EXPECTFL( str.capacity  == 6 );
    REDGPU_2_EXPECTFL( str.alignment == 1 );

    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "Hi");

    REDGPU_2_EXPECTFL( str.items     != 0 );
    REDGPU_2_EXPECTFL( str.count     == 3 );
    REDGPU_2_EXPECTFL( str.capacity  == 6 );
    REDGPU_2_EXPECTFL( str.alignment == 1 );

    REDGPU_2_EXPECTFL( str.items[0]  == 'H' );
    REDGPU_2_EXPECTFL( str.items[1]  == 'i' );
    REDGPU_2_EXPECTFL( str.items[2]  == 0   );

    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "!!!");

    REDGPU_2_EXPECTFL( str.items     != 0 );
    REDGPU_2_EXPECTFL( str.count     == 6 );
    REDGPU_2_EXPECTFL( str.capacity  == 6 );
    REDGPU_2_EXPECTFL( str.alignment == 1 );

    REDGPU_2_EXPECTFL( str.items[0]  == 'H' );
    REDGPU_2_EXPECTFL( str.items[1]  == 'i' );
    REDGPU_2_EXPECTFL( str.items[2]  == '!' );
    REDGPU_2_EXPECTFL( str.items[3]  == '!' );
    REDGPU_2_EXPECTFL( str.items[4]  == '!' );
    REDGPU_2_EXPECTFL( str.items[5]  == 0   );

    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "?");

    REDGPU_2_EXPECTFL( str.items     != 0  );
    REDGPU_2_EXPECTFL( str.count     == 7  );
    REDGPU_2_EXPECTFL( str.capacity  == 12 );
    REDGPU_2_EXPECTFL( str.alignment == 1  );

    REDGPU_2_EXPECTFL( str.items[0]  == 'H' );
    REDGPU_2_EXPECTFL( str.items[1]  == 'i' );
    REDGPU_2_EXPECTFL( str.items[2]  == '!' );
    REDGPU_2_EXPECTFL( str.items[3]  == '!' );
    REDGPU_2_EXPECTFL( str.items[4]  == '!' );
    REDGPU_2_EXPECTFL( str.items[5]  == '?' );
    REDGPU_2_EXPECTFL( str.items[6]  == 0   );

    REDGPU_32_DYNAMIC_ARRAY_FREE(str);

    REDGPU_2_EXPECTFL( str.items     == 0 );
    REDGPU_2_EXPECTFL( str.count     == 0 );
    REDGPU_2_EXPECTFL( str.capacity  == 0 );
    REDGPU_2_EXPECTFL( str.alignment == 1 );
  }

  printf("All tests are passed successfully.\n");
}
