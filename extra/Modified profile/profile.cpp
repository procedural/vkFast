#if 0
g++ -std=c++11 -static-libstdc++ -static-libgcc -Os -fno-exceptions -fno-rtti -fPIC -shared -o libprofile.so profile.cpp profile.s
strip --strip-all libprofile.so
exit
#endif
// cl /LD profile.cpp

#include <stdint.h> // For uint64_t
#include <stdlib.h> // For atexit
#include <time.h>   // For time_t
#include <vector>   // For std::vector
#include <mutex>    // For std::mutex
#include <thread>   // For std::thread
#include <new>      // For std::new, std::nothrow
#include <fstream>  // For std::ifstream, std::ofstream
#ifdef _WIN32
#include <windows.h>
#endif
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define PROFILE_MAX_STRING_LENGTH            67
#define PROFILE_MAX_SAMPLES_COUNT            512
#define PROFILE_FLUSH_THREAD_SLEEP_TIME_NS   1000
#define PROFILE_WAIT_FOR_FLUSH_SLEEP_TIME_NS 500

#if defined(__linux__)
#define PROFILE_LONG long
#endif
#if defined(_WIN32)
#define PROFILE_LONG long long
#endif

typedef struct ProfileInternalSample {
#if defined(__linux__)
  time_t        seconds;
  long          nanoseconds;
#endif
#if defined(_WIN32)
  LARGE_INTEGER counter;
#endif
  uint64_t      threadId;
  char          label[PROFILE_MAX_STRING_LENGTH];
  char          BorE;
} ProfileInternalSample;

volatile int          __PROFILE_GLOBAL_6bf241eae_isInitialized = 0;
volatile int          __PROFILE_GLOBAL_6bf241eae_threadExit    = 0;
std::thread *         __PROFILE_GLOBAL_6bf241eae_thread        = 0;
std::mutex            __PROFILE_GLOBAL_6bf241eae_mutex;
ProfileInternalSample __PROFILE_GLOBAL_6bf241eae_sampleInitial = {};
ProfileInternalSample __PROFILE_GLOBAL_6bf241eae_samples[PROFILE_MAX_SAMPLES_COUNT] = {};
volatile int          __PROFILE_GLOBAL_6bf241eae_samplesCount  = 0;

#if defined(__linux__)
extern "C" void * __PROFILE_PROCEDURE_6bf241eae_syscall(long);
#endif

static inline double profileInternalSecondsAndNanosecondsToMicrosecondsWithDecimalNanoseconds(PROFILE_LONG seconds, PROFILE_LONG nanoseconds) {
  PROFILE_LONG seconds_as_milliseconds = seconds                 * 1000;
  PROFILE_LONG seconds_as_microseconds = seconds_as_milliseconds * 1000;
  PROFILE_LONG seconds_as_nanoseconds  = seconds_as_microseconds * 1000;

  PROFILE_LONG summed_seconds_as_nanoseconds_and_nanoseconds = seconds_as_nanoseconds + nanoseconds;
  double microseconds_with_decimal_nanoseconds = (double)summed_seconds_as_nanoseconds_and_nanoseconds / 1000.0;

  return microseconds_with_decimal_nanoseconds;
}

static void profileInternalAtExit(void) {
  __PROFILE_GLOBAL_6bf241eae_threadExit = 1;
  __PROFILE_GLOBAL_6bf241eae_thread->join();
#if defined(__linux__)
  std::ofstream fsAppend("/tmp/github_procedural_profile.json", std::ofstream::app);
#else
  std::ofstream fsAppend("github_procedural_profile.json", std::ofstream::app);
#endif
  fsAppend << "],\"displayTimeUnit\":\"ns\"}\n";
  fsAppend.close();
  delete __PROFILE_GLOBAL_6bf241eae_thread;
}

static void __PROFILE_PROCEDURE_6bf241eae_threadTask(void) {
#if defined(_WIN32)
  LARGE_INTEGER performanceFrequency = {};
  QueryPerformanceFrequency(&performanceFrequency);
#endif
  volatile int          samplesCount = 0;
  ProfileInternalSample samples[PROFILE_MAX_SAMPLES_COUNT];
  while (1) {
    if (__PROFILE_GLOBAL_6bf241eae_threadExit == 1) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(PROFILE_FLUSH_THREAD_SLEEP_TIME_NS));
    {
      std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
      samplesCount = __PROFILE_GLOBAL_6bf241eae_samplesCount;
      if (samplesCount == 0) {
        continue;
      }
      if (samplesCount > PROFILE_MAX_SAMPLES_COUNT) {
        samplesCount = PROFILE_MAX_SAMPLES_COUNT;
      }
      for (int i = 0, count = samplesCount; i < count; i += 1) {
        samples[i] = __PROFILE_GLOBAL_6bf241eae_samples[i];
      }
      __PROFILE_GLOBAL_6bf241eae_samplesCount = 0;
    }
    int count = samplesCount;
    // NOTE(Constantine): Now the copied samples can be flushed to disk.
    {
      ProfileInternalSample initialSample = __PROFILE_GLOBAL_6bf241eae_sampleInitial;
#if defined(__linux__)
      std::ofstream fsAppend("/tmp/github_procedural_profile.json", std::ofstream::app);
#else
      std::ofstream fsAppend("github_procedural_profile.json", std::ofstream::app);
#endif
      for (int i = 0; i < count; i += 1) {
        ProfileInternalSample s = samples[i];
#if defined(__linux__)
        PROFILE_LONG seconds        = s.seconds;
        PROFILE_LONG nanoseconds    = s.nanoseconds;
        PROFILE_LONG initialSeconds = initialSample.seconds;
#endif
#if defined(_WIN32)
        PROFILE_LONG seconds        = (s.counter.QuadPart / performanceFrequency.QuadPart);
        PROFILE_LONG nanoseconds    = (s.counter.QuadPart % performanceFrequency.QuadPart) * 1000000000 / performanceFrequency.QuadPart;
        PROFILE_LONG initialSeconds = (initialSample.counter.QuadPart / performanceFrequency.QuadPart);
#endif
        double time = profileInternalSecondsAndNanosecondsToMicrosecondsWithDecimalNanoseconds(seconds - initialSeconds, nanoseconds);
        int size = stbsp_snprintf(0, 0, ",{\"ph\":\"%c\",\"ts\":%.3lf,\"pid\":0,\"tid\":%d,\"name\":\"%s\"}\n", s.BorE, time, s.threadId, s.label);
#if defined(_WIN32)
        char * line = (char *)_alloca(size + 1);
#else
        char line[size + 1];
#endif
        stbsp_snprintf(line, size + 1,  ",{\"ph\":\"%c\",\"ts\":%.3lf,\"pid\":0,\"tid\":%d,\"name\":\"%s\"}\n", s.BorE, time, s.threadId, s.label);
        line[size] = 0;
        fsAppend << line;
      }
      fsAppend.close();
    }
  }
}

extern "C"
#if defined(_WIN32)
__declspec(dllexport)
#endif
void profileBegin(const char * label) {
  ProfileInternalSample sample;
#if defined(__linux__)
  struct timespec timespec;
  clock_gettime(CLOCK_REALTIME, &timespec);
  sample.seconds     = timespec.tv_sec;
  sample.nanoseconds = timespec.tv_nsec;
  sample.threadId    = (uint64_t)__PROFILE_PROCEDURE_6bf241eae_syscall(186);
#endif
#if defined(_WIN32)
  QueryPerformanceCounter(&sample.counter);
  sample.threadId    = (uint64_t)GetCurrentThreadId();
#endif
  if (label == 0) {
    sample.label[0] = 0;
  } else {
    for (int i = 0; i < (PROFILE_MAX_STRING_LENGTH-1); i += 1) {
      char c = label[i];
      sample.label[i] = c;
      if (c == 0) {
        break;
      }
    }
    sample.label[PROFILE_MAX_STRING_LENGTH-1] = 0;
  }
  sample.BorE = 'B';

  {
    std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
    if (__PROFILE_GLOBAL_6bf241eae_isInitialized == 0) {
      __PROFILE_GLOBAL_6bf241eae_isInitialized = 1;
      __PROFILE_GLOBAL_6bf241eae_thread        = new(std::nothrow) std::thread(__PROFILE_PROCEDURE_6bf241eae_threadTask);
      __PROFILE_GLOBAL_6bf241eae_sampleInitial = sample;
#if defined(__linux__)
      std::ofstream fs("/tmp/github_procedural_profile.json", std::ofstream::out);
#else
      std::ofstream fs("github_procedural_profile.json", std::ofstream::out);
#endif
      fs << "{\"traceEvents\":[{}\n";
      fs.close();
      atexit(profileInternalAtExit);
    }
  }

  if (__PROFILE_GLOBAL_6bf241eae_thread == 0) {
    return;
  }

  volatile int attemptFailed = 0;
  {
    std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
    if (__PROFILE_GLOBAL_6bf241eae_samplesCount >= PROFILE_MAX_SAMPLES_COUNT) {
      attemptFailed = 1;
    } else {
      attemptFailed = 0;
      __PROFILE_GLOBAL_6bf241eae_samples[__PROFILE_GLOBAL_6bf241eae_samplesCount] = sample;
      __PROFILE_GLOBAL_6bf241eae_samplesCount += 1;
    }
  }
  if (attemptFailed == 1) {
    while (1) {
      std::this_thread::sleep_for(std::chrono::nanoseconds(PROFILE_WAIT_FOR_FLUSH_SLEEP_TIME_NS));
      {
        std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
        if (__PROFILE_GLOBAL_6bf241eae_samplesCount >= PROFILE_MAX_SAMPLES_COUNT) {
          attemptFailed = 1;
        } else {
          attemptFailed = 0;
          __PROFILE_GLOBAL_6bf241eae_samples[__PROFILE_GLOBAL_6bf241eae_samplesCount] = sample;
          __PROFILE_GLOBAL_6bf241eae_samplesCount += 1;
        }
      }
      if (attemptFailed == 0) {
        break;
      }
    }
  }
}

extern "C"
#if defined(_WIN32)
__declspec(dllexport)
#endif
void profileEnd(const char * label) {
  ProfileInternalSample sample;
#if defined(__linux__)
  struct timespec timespec;
  clock_gettime(CLOCK_REALTIME, &timespec);
  sample.seconds     = timespec.tv_sec;
  sample.nanoseconds = timespec.tv_nsec;
  sample.threadId    = (uint64_t)__PROFILE_PROCEDURE_6bf241eae_syscall(186);
#endif
#if defined(_WIN32)
  QueryPerformanceCounter(&sample.counter);
  sample.threadId    = (uint64_t)GetCurrentThreadId();
#endif
  if (label == 0) {
    sample.label[0] = 0;
  } else {
    for (int i = 0; i < (PROFILE_MAX_STRING_LENGTH-1); i += 1) {
      char c = label[i];
      sample.label[i] = c;
      if (c == 0) {
        break;
      }
    }
    sample.label[PROFILE_MAX_STRING_LENGTH-1] = 0;
  }
  sample.BorE = 'E';

  if (__PROFILE_GLOBAL_6bf241eae_thread == 0) {
    return;
  }

  volatile int attemptFailed = 0;
  {
    std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
    if (__PROFILE_GLOBAL_6bf241eae_samplesCount >= PROFILE_MAX_SAMPLES_COUNT) {
      attemptFailed = 1;
    } else {
      attemptFailed = 0;
      __PROFILE_GLOBAL_6bf241eae_samples[__PROFILE_GLOBAL_6bf241eae_samplesCount] = sample;
      __PROFILE_GLOBAL_6bf241eae_samplesCount += 1;
    }
  }
  if (attemptFailed == 1) {
    while (1) {
      std::this_thread::sleep_for(std::chrono::nanoseconds(PROFILE_WAIT_FOR_FLUSH_SLEEP_TIME_NS));
      {
        std::lock_guard<std::mutex> __6bf241eae_mutexScope(__PROFILE_GLOBAL_6bf241eae_mutex);
        if (__PROFILE_GLOBAL_6bf241eae_samplesCount >= PROFILE_MAX_SAMPLES_COUNT) {
          attemptFailed = 1;
        } else {
          attemptFailed = 0;
          __PROFILE_GLOBAL_6bf241eae_samples[__PROFILE_GLOBAL_6bf241eae_samplesCount] = sample;
          __PROFILE_GLOBAL_6bf241eae_samplesCount += 1;
        }
      }
      if (attemptFailed == 0) {
        break;
      }
    }
  }
}
