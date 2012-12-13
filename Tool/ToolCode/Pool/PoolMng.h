#ifdef _MSC_VER
#pragma once
#endif

#ifndef __PoolMng_H__
#define __PoolMng_H__

#include "Define/Macro.h"
#include "Define/TypeDef.h"
#include "SafeSTL/SafeMap.h"

//#include "Pool.h"

// forward declaration
class CPool_Base;

//////////////////////////////////////////////////////////////////////////
//  @file   PoolMng.h
//  @brief  �ڴ�ع�����
//  @author ������
//  @date   2012.03.20
//////////////////////////////////////////////////////////////////////////

class CPool_Manager
{
    DECLARE_SINGLETON(CPool_Manager)

    friend class CPool_Base;

private:
    SafeMap<TString, CPool_Base*> m_mapPool;    // �ض�������,  first, PoolName


protected:
    // ע���ڴ�ض���
    void RegistPool( CPool_Base* pPool );

private:
    // �����ڴ�ض���
    void ClearPool( void );
};

#endif  // __PoolMng_H__