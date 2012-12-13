#ifdef _MSC_VER
#pragma once
#endif

#ifndef __SafeMap_H__
#define __SafeMap_H__

#include <map>

#include "Define/Macro.h"
#include "lock/Lockit.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   SafeMap.h 
//  @brief  �̰߳�ȫ map ����
//  @author ������
//  @date   2012.08.05
//////////////////////////////////////////////////////////////////////////

template<typename TFirst, typename TSecond>
class SafeMap 
{
public:
    typedef std::map<TFirst, TSecond> MAP_TYPE;
    //typedef MAP_SAFETYPE::iterator      MAP_SAFETYPE_ITR;
    /*  @breif  �����ص�������ʽ����
    *   @return bool, �������Ʊ��,  true, ��������;  false, ��ֹ����
    */
    typedef bool (Func_ForEach) ( const TFirst& first, const TSecond& second, void* pParm1, void* pParm2 );

private:
    CLock       m_Lock;
    MAP_TYPE    m_Map;

public:
    SafeMap( void ) {}
    ~SafeMap() { Clear(); }

    bool Insert( const TFirst& key, TSecond& value, Define_DebugCode );
    bool Find( const TFirst& key, TSecond& outValue );
    bool Delete( const TFirst& key );
    void Clear( void );

    void CopyTemp( MAP_TYPE& outTempMap );
    /*  @breif ���Ƴ�����,���� SafeList �е�Ԫ������. ���� �������д�ŵ���ָ�����ʱ,��������͸�����ͬһ������ */
    void CopyAndClear( MAP_TYPE& outTempMap );

    /*  @breif ѭ����������*/
    void ForEach( Func_ForEach* pFunc, void* pParm1 = NULL, void* pParm2 = NULL );

    void Lock()     { m_Lock.Lock(); }
    void UnLock()   { m_Lock.UnLock(); }
};


//////////////////////////////////////////////////////////////////////////

template<typename TFirst, typename TSecond>
bool SafeMap<TFirst, TSecond>::Insert( const TFirst& key, TSecond& value, Define_DebugCode )
{
    if ( m_Map.find( key ) != m_Map.end() )
    {
        // Log...
        cout << "File: " << szFile << endl;
        cout << "Fuc: " << szFuncName << endl;
        cout << "Line: " << nLineNum << endl;
        return false;
    }

    CLockit _Lock(m_Lock);
    return m_Map.insert( make_pair(key, value) ).second;
}

template<typename TFirst, typename TSecond>
bool SafeMap<TFirst, TSecond>::Find( const TFirst& key, TSecond& outValue )
{
    CLockit _Lock(m_Lock);

    MAP_TYPE::iterator itr = m_Map.find( key );
    if ( itr != m_Map.end() )
    {
        outValue = itr->second;

        return true;
    }
    else
    {
        return false;
    }    
}

template<typename TFirst, typename TSecond>
bool SafeMap<TFirst, TSecond>::Delete( const TFirst& key )
{
    CLockit _Lock(m_Lock);

    MAP_TYPE::iterator itr = m_Map.find( key );
    if ( itr != m_Map.end() )
    {
        m_Map.erase( itr );

        return true;
    }
    else
    {
        return false;
    }  
}

template<typename TFirst, typename TSecond>
void SafeMap<TFirst, TSecond>::Clear( void )
{
    CLockit _Lock(m_Lock);

    m_Map.clear();
}

template<typename TFirst, typename TSecond>
void SafeMap<TFirst, TSecond>::CopyTemp( MAP_TYPE& outTempMap )
{
    outTempMap.clear();

    CLockit _Lock(m_Lock);

    outTempMap.insert( m_Map.begin(), m_Map.end() );
}

template<typename TFirst, typename TSecond>
void SafeMap<TFirst, TSecond>::CopyAndClear( MAP_TYPE& outTempMap )
{
    outTempMap.clear();

    CLockit _Lock(m_Lock);

    outTempMap.insert( m_Map.begin(), m_Map.end() );
    m_Map.clear();
}

template<typename TFirst, typename TSecond>
void SafeMap<TFirst, TSecond>::ForEach( Func_ForEach* pFunc, void* pParm1 = NULL, void* pParm2 = NULL )
{
    if ( pFunc == NULL )
    {
        return;
    }

    // ��������ı���
    MAP_TYPE mapForEach;
    CopyTemp( mapForEach );

    // ��������,���ô�����
    MAP_TYPE::iterator itr    = mapForEach.begin();
    MAP_TYPE::iterator itrEnd = mapForEach.end();
    for ( ; itr != itrEnd; ++itr )
    {
        if ( pFunc( itr->first, itr->second, pParm1, pParm2 ) == false )
        {
            return;
        }
    }
}

#endif  // __SafeMap_H__