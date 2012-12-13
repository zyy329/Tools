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
//  @brief  ���ߺ����. �궨��ĺ���
//  @author ������
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

#define CheckPTR(getPTR) if(getPTR) getPTR          // ���ָ��,������Ч����ʹ��  ��: CheckPTR(class.getPtr())->fuc1();  չ��: if( class.getPtr() ) class.getPtr()->fuc1();
#define BoolCheckPTR(getPTR) getPTR && getPTR       // ���ж���ʹ�ü��ָ��; ��: if ( BoolCheckPTR(getPTR())->isFuc2();  չ��: if( getPTR() && getPTR()->isFuc2() )s


//////////////////////////////////////////////////////////////////////////
// ������
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

// �����������Ҫ�ֶ�����
#define IMPLEMENT_SINGLETON( class_name )                                                                           \
    class_name* class_name::ms_pSingleton = NULL;

//////////////////////////////////////////////////////////////////////////
// �����߳� ������
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

// �����������Ҫ�ֶ�����
#define IMPLEMENT_SINGLETON_LOCAL( class_name )                                                                     \
    ThreadLocal class_name* class_name::ms_pSingleton = NULL;


// ˳�����һ������ ��
//// ˳�����һ����������: ����_���� (������DECLARE_DYNAMIC��)
#define DECLARE_NUM( class_name )                                                                                   \
private:                                                                                                            \
    static UINT ms_uiNameNum;                                                                                       \
    UINT CreateNum( void );                                                                                         \
    /*const string CreateNumName( void );*/                                                                             \
    void ReleaseNum( void );


#define IMPLEMENT_NUM( class_name )      /* �ڹ��캯����ʹ���о�����,���޷����ʵ�ʶ�������,��ֻ�ǵ�ǰ�����������*/\
    UINT class_name::ms_uiNameNum = 1;   /* 0,�������������ʶ*/                                                    \
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