#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void profileBegin       (const char * mark);
void profileEnd         (const char * mark);
void profileInsertBegin (const char * mark, long long linux_seconds, long long linux_nanoseconds, long long windows_counter);
void profileInsertEnd   (const char * mark, long long linux_seconds, long long linux_nanoseconds, long long windows_counter);

#ifdef __cplusplus
}
#endif
