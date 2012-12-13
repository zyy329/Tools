#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Pool_H__
#define __Pool_H__

#include "PoolMng.h"
#include "Queue/LockFree_Queue.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   Pool.h
//  @brief  �ڴ��ģ��
//  @author ������
//  @date   2012.03.20
//////////////////////////////////////////////////////////////////////////

class CPool_Base
{
public:
    CPool_Base( const TString& strPoolName )
        : m_strPoolName( strPoolName )
    {
        CPool_Manager::GetSingleton()->RegistPool( this );
    }
    virtual ~CPool_Base( void ){}

    inline const TString& GetPoolName( void )   { return m_strPoolName; }
private:
    TString m_strPoolName;      // �ڴ�ص�����,���ڹ������
};

template<typename T>
class CPool : CPool_Base
{
    typedef SafeMap<T*,int> SMAP_USED;
private:
    CLockFreeQueue<T*>  m_Pool;     // ���ó�
    SMAP_USED           m_mapUesd;  // ���ڱ��ⲿʹ�õ�Ԫ��; second, ������

public:
    CPool( const TString& strPoolName );
    ~CPool( void );

    T* Pool_Pop( void );
    void Pool_Push( T* pObj );
};

template<typename T>
CPool<T>::CPool( const TString& strPoolName )
: CPool_Base( strPoolName )
{
    
};

template<typename T>
CPool<T>::~CPool( void )
{
    // m_mapUesd ���� �ͷ�
    SMAP_USED::MAP_TYPE mapTemp;
    m_mapUesd.CopyAndClear( mapTemp );

    SMAP_USED::MAP_TYPE::iterator mapItr       = mapTemp.begin();
    SMAP_USED::MAP_TYPE::iterator mapItrEnd    = mapTemp.end();
    for ( ; mapItr != mapItrEnd; ++mapItr )
    {
        T* pObj = mapItr->first;
        SAFE_DELETE(pObj);
    }

    // m_Pool ���� �ͷ�
    T* pElm = NULL;
    while ( m_Pool.PopFront(pElm) == true )
    {
        SAFE_DELETE(pElm);
    }
};


template<typename T>
T* CPool<T>::Pool_Pop( void )
{
    // ���Ԫ��
    T* pElm = NULL;
    if ( m_Pool.PopFront(pElm) == true )
    {
        // NULL
    }
    else
    {
        pElm = new T();
    }

    // ��¼�����ó���
    int nSec = 0;       // ��ʱ,��ǰ second ����
    if ( m_mapUesd.Insert( pElm, nSec, Use_DebugCode ) == false )
    {
        SAFE_DELETE(pElm);
        return NULL;
    }

    return pElm;
}

template<typename T>
void CPool<T>::Pool_Push( T* pObj )
{
    if ( m_mapUesd.Delete( pObj ) == true )
    {
        m_Pool.PushBack( pObj );
    }
    else
    {
        // bad
        SAFE_DELETE( pObj );
    }
}

#endif  // __Pool_H__