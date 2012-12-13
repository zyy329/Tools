#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Macro_H__
#define __Macro_H__

#include <iostream>
#include "MacroDef.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Macro.h
//  @brief  工具宏代码. 宏定义的函数
//  @author 张逸云
//  @date   2011.12.02
//////////////////////////////////////////////////////////////////////////


// Often there is need to safely release pointer or dynamically
// allocated memory. Macros below provide this kind of functionality
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)     {if(x) {(x)->Release(); (x)=NULL;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)		{if(x) {delete (x);	    (x)=NULL;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x){if(x) {delete[] (x);   (x)=NULL;}}
#endif

#define CheckPTR(getPTR) if(getPTR) getPTR          // 获得指针,检验有效后再使用  例: CheckPTR(class.getPtr())->fuc1();  展开: if( class.getPtr() ) class.getPtr()->fuc1();
#define BoolCheckPTR(getPTR) getPTR && getPTR       // 在判断中使用检测指针; 例: if ( BoolCheckPTR(getPTR())->isFuc2();  展开: if( getPTR() && getPTR()->isFuc2() )s


//////////////////////////////////////////////////////////////////////////
// 单件宏
#define DECLARE_SINGLETON( class_name )                                                                             \
protected:                                                                                                          \
    class_name( void );                                                                                             \
    ~class_name( void );                                                                                            \
public:                                                                                                             \
    static class_name* GetSingleton( void )                                                                         \
    {                                                                                                               \
        return ms_pSingleton?ms_pSingleton:ms_pSingleton=new class_name();                                          \
    }                                                                                                               \
    static const class_name* GetSingleton_C( void )                                                                 \
    {                                                                                                               \
        return GetSingleton();                                                                                      \
    }                                                                                                               \
    static void destroySingleton( void )                                                                            \
    {                                                                                                               \
        SAFE_DELETE(ms_pSingleton);                                                                                 \
    }                                                                                                               \
private:                                                                                                            \
    static class_name* ms_pSingleton;

// 构造和析构需要手动定义
#define IMPLEMENT_SINGLETON( class_name )                                                                           \
    class_name* class_name::ms_pSingleton = NULL;

//////////////////////////////////////////////////////////////////////////
// 本地线程 单件宏
#define DECLARE_SINGLETON_LOCAL( class_name )                                                                       \
protected:                                                                                                          \
    class_name( void );                                                                                             \
    ~class_name( void );                                                                                            \
public:                                                                                                             \
    static class_name* GetSingleton( void )                                                                         \
    {                                                                                                               \
        return ms_pSingleton?ms_pSingleton:ms_pSingleton=new class_name();                                          \
    }                                                                                                               \
    static const class_name* GetSingleton_C( void )                                                                 \
    {                                                                                                               \
        return GetSingleton();                                                                                      \
    }                                                                                                               \
    static void destroySingleton( void )                                                                            \
    {                                                                                                               \
        SAFE_DELETE(ms_pSingleton);                                                                                 \
    }                                                                                                               \
private:                                                                                                            \
    ThreadLocal static class_name* ms_pSingleton;

// 构造和析构需要手动定义
#define IMPLEMENT_SINGLETON_LOCAL( class_name )                                                                     \
    ThreadLocal class_name* class_name::ms_pSingleton = NULL;


// 顺序产生一个数字 或
//// 顺序产生一个数字名称: 类名_数字 (依赖于DECLARE_DYNAMIC宏)
#define DECLARE_NUM( class_name )                                                                                   \
private:                                                                                                            \
    static UINT ms_uiNameNum;                                                                                       \
    UINT CreateNum( void );                                                                                         \
    /*const string CreateNumName( void );*/                                                                             \
    void ReleaseNum( void );


#define IMPLEMENT_NUM( class_name )      /* 在构造函数中使用有局限性,将无法获得实际对象名称,而只是当前构造类的名称*/\
    UINT class_name::ms_uiNameNum = 1;   /* 0,保留用作错误标识*/                                                    \
    UINT class_name::CreateNum( void )                                                                              \
    {                                                                                                               \
        if ( ms_uiNameNum == 0 )                                                                                    \
        {                                                                                                           \
            /*stringstream ss;                                                                                        \
            ss << #class_name << ": CreateNum() -- ms_uiNameNum out of max length";                                 \
            Error::Error_Info( ss.str() );                                                                          \
            return NULL; */                                                                                           \
        }                                                                                                           \
        return ms_uiNameNum++;                                                                                      \
    }                                                                                                               \
    /*const string class_name::CreateNumName( void )                                                                  \
    {                                                                                                               \
        if ( ms_uiNameNum == 0 )                                                                                    \
        {                                                                                                           \
            stringstream ss;                                                                                        \
            ss << #class_name << ": CreateNumName() -- ms_uiNameNum out of max length";                             \
            Error::Error_Info( ss.str() );                                                                          \
            return _T("");                                                                                              \
        }                                                                                                           \
        char NameBuf[32];                                                                                           \
        sprintf_s( NameBuf, "%s_%d", this->GetRuntimeClass()->getClassName(), ms_uiNameNum++ );                     \
        return NameBuf;                                                                                             \
    }*/                                                                                                               \
    void class_name::ReleaseNum( void )                                                                             \
    {                                                                                                               \
        ms_uiNameNum--;                                                                                             \
    }



#endif  // __Macro_H__