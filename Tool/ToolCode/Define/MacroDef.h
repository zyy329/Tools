#ifdef _MSC_VER
#pragma once
#endif

#ifndef __MacroDef_H__
#define __MacroDef_H__

#include <iostream>

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   MacroDef.h
//  @brief  工具宏代码. 宏定义的常量
//  @author 张逸云
//  @date   2012.08.11
//////////////////////////////////////////////////////////////////////////

//#if __MAYA__
//#define ThreadLocal
//#elif __WIN32__
#define ThreadLocal __declspec(thread)
//#elif __XBOX360__
//#define ThreadLocal __declspec(thread)
//#elif __WII__
//#define ThreadLocal
//#elif __PS3__
//#define ThreadLocal __thread
//#elif __OSX__
//// NOTE: the __thread keyword is not supported on OSX,
//// we keep the definition as it is so that GCC throws an error
//// if it encounters ThreadLocal!
//#define ThreadLocal __thread
//#else
//#error "Unsupported platform!"
//#endif

#ifdef _UNICODE     //   如果使用的是unicode 
#undef      cout    //   取消从前cout的定义. 
#define     cout   wcout 
#endif 

// 调试信息
#define Define_DebugCode    const TCHAR* szFile, const TCHAR* szFuncName, const int nLineNum
#define Use_DebugCode       _T(__FILE__), _T(__FUNCTION__), __LINE__
#define Send_DebugCode      szFile, szFuncName, nLineNum


#endif  // __MacroDef_H__