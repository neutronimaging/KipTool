#ifndef STATISTICSMODULES_GLOBAL_H
#define STATISTICSMODULES_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(STATISTICSMODULES_LIBRARY)
#  define STATISTICSMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STATISTICSMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STATISTICSMODULES_GLOBAL_H
