#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Lock_H__
#define __Lock_H__

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Lock.h
//  @brief  �� CRITICAL_SECTION �����һ���򵥷�װ,ʹ������������Щ
//  @author ������
//  @date   2012.08.02
//////////////////////////////////////////////////////////////////////////

class CLock
{
public:
    CLock( void );
    ~CLock( void );

    void Lock( void );
    void UnLock( void );

private:
    CRITICAL_SECTION m_cs;
};

#endif  // __Lock_H__