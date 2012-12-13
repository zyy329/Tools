#ifdef _MSC_VER
#pragma once
#endif

#ifndef _STDAFX_H_
#define _STDAFX_H_

// 不加载 cstdio / cstring / cwchar 这几个头文件,  <atltime.h> 会间接加载他们
#define _CSTDIO_
#define _CSTRING_
#define _CWCHAR_

// System Header
#include <Windows.h>
#include <atltime.h>
#include <process.h>        // 线程

#include <iostream>
#include <tchar.h>
#include <strsafe.h>        // 安全字符串函数
#include <string>

#include <map>
#include <list>

using namespace std;


// User Header

#endif  // _STDAFX_H_