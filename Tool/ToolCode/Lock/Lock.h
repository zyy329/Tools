#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Lock_H__
#define __Lock_H__

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Lock.h
//  @brief  对 CRITICAL_SECTION 对象的一个简单封装,使得用起来方便些
//  @author 张逸云
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