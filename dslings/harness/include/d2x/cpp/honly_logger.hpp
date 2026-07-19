#ifndef LOGGER_HPP__HONLY
#define LOGGER_HPP__HONLY

#include <cstdio>

#ifndef HONLY_LOGGER_TAG
#define HONLY_LOGGER_TAG "HONLY"
#endif

#define LOG_ENABLE true
#define _HONLY_LOG(fd, ...) if (LOG_ENABLE) { fprintf (fd, __VA_ARGS__); fprintf (fd, "\033[0m\n"); fflush(stdout); }
#define HONLY_LOGI_P(...) { fprintf (stdout, "\033[32m[%s LOGI]: - ", HONLY_LOGGER_TAG); _HONLY_LOG(stdout, __VA_ARGS__); }
#define HONLY_LOGI(...) { fprintf (stdout, "\033[32m[%s LOGI]: %s: %s:%d - ", HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); _HONLY_LOG(stdout, __VA_ARGS__); }
#define HONLY_LOGD(...) { fprintf (stdout, "\033[37m[%s LOGD]: %s: %s:%d - ", HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); _HONLY_LOG(stdout, __VA_ARGS__); }
#define HONLY_LOGW(...) { fprintf (stdout, "\033[33m[%s LOGW]: %s: %s:%d - ", HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); _HONLY_LOG(stdout, __VA_ARGS__); }
#define HONLY_LOGE(...) { fprintf (stdout, "\033[31m[%s LOGE]: %s: %s:%d - ❌ | ", HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); _HONLY_LOG(stdout, __VA_ARGS__); }

#endif