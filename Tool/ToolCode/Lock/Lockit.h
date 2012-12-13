#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Lockit_H__
#define __Lockit_H__

#include "Lock.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Lockit.h
//  @brief  ��Ӧ�ö���, ͨ�������������ڵķ�����ȷ�������ͷ�
//  @author ������
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
    /*  @brief  ʹ��Ĭ��ȫ�������� */
    explicit CLockit( void );

private:
    CLockit( const CLockit& other );
    CLockit& operator=( const CLockit& other );

public:
    /*  @brief  ͨ��ö��ֵ, ��������������ȫ�������� */
    explicit CLockit( ELock eLock );

    /*  @brief  ֱ�Ӵ���������������� */
    explicit CLockit( CLock& lock );



    ~CLockit( void );

    //void Lockit( void );
    void UnLockit( void );      // ������ǰ�ͷ���


private:
    CLock& m_Lock;     // ������������
};

#endif  // __Lockit_H__