#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Lockit_H__
#define __Lockit_H__

#include "Lock.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Lockit.h
//  @brief  锁应用对象, 通过对象生命周期的方法来确保锁的释放
//  @author 张逸云
//  @date   2012.08.02
//////////////////////////////////////////////////////////////////////////

class CLockit
{
public:
    enum ELock
    {
        eLOCK_DEFAULT,

        eLOCK_NUMBERS,
    };

public:
    /*  @brief  使用默认全局锁对象 */
    explicit CLockit( void );

private:
    CLockit( const CLockit& other );
    CLockit& operator=( const CLockit& other );

public:
    /*  @brief  通过枚举值, 锁定事先声明的全局锁对象 */
    explicit CLockit( ELock eLock );

    /*  @brief  直接传入锁对象进行锁定 */
    explicit CLockit( CLock& lock );



    ~CLockit( void );

    //void Lockit( void );
    void UnLockit( void );      // 允许提前释放锁


private:
    CLock& m_Lock;     // 关联的锁对象
};

#endif  // __Lockit_H__