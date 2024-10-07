#pragma once

#ifndef _EXPORT
#ifdef _WIN32
#define _EXPORT __declspec(dllexport)
#else 
#define _EXPORT 
#endif
#endif