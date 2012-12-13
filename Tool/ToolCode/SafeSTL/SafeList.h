#ifdef _MSC_VER
#pragma once
#endif

#ifndef __SafeList_H__
#define __SafeList_H__

#include <list>

#include "Define/Macro.h"
#include "lock/Lockit.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   SafeList.h
//  @brief  �̰߳�ȫ List ����
//  @author ������
//  @date   2012.08.22
//////////////////////////////////////////////////////////////////////////

template<typename TValue>
class SafeList 
{
public:
    typedef std::list<TValue> LIST_TYPE;
    /*  @breif  �����ص�������ʽ����
    *   @return bool, �������Ʊ��,  true, ��������;  false, ��ֹ����
    */
    typedef bool (Func_ForEach) ( const TValue& value, void* pParm1, void* pParm2 );
    
    /*  @breif  ɾ���ص�������ʽ����
    *   @param  outIsDel, �������, ����ͬ־�ص��Ƿ����ɾ��, true,ɾ��;  false,��ɾ��
    *   @return bool, �������Ʊ��,  true, ��������;  false, ��ֹ����
    */
    typedef bool (Func_ForDel) ( const TValue& value, void* pParm1, void* pParm2, bool& outIsDel );

private:
    CLock       m_Lock;
    LIST_TYPE   m_List;

public:
    SafeList( void ) {}
    ~SafeList() { Clear(); }

    void push_front( const TValue& value );
    void pop_front( void );
    void push_back( const TValue& value );
    void pop_back( void );
    void Clear( void );

    void CopyTemp( LIST_TYPE& outTempList );
    /*  @breif ���Ƴ�����,���� SafeList �е�Ԫ������. ���� �������д�ŵ���ָ�����ʱ,��������͸�����ͬһ������ */
    void CopyAndClear( LIST_TYPE& outTempList );


    /*  @breif  ѭ����������,������������ֵɾ��. ͨ���ص�����ȷ���Ƿ����ɾ������
    *   @warning    �ñ����ڼ����ȫ��ס����, �벻Ҫʹ��̫�����ӵ��ж��߼�
    */
    void Remove_If( Func_ForDel* pFunc, void* pParm1 = NULL, void* pParm2 = NULL );

    /*  @breif ѭ����������*/
    void ForEach( Func_ForEach* pFunc, void* pParm1 = NULL, void* pParm2 = NULL );

    void Lock()     { m_Lock.Lock(); }
    void UnLock()   { m_Lock.UnLock(); }
};


//////////////////////////////////////////////////////////////////////////

template<typename TValue>
void SafeList<TValue>::push_front( const TValue& value )
{
    CLockit _Lock(m_Lock);
    return m_List.push_front( value );
}

template<typename TValue>
void SafeList<TValue>::pop_front( void )
{
    CLockit _Lock(m_Lock);
    return m_List.pop_front();
}

template<typename TValue>
void SafeList<TValue>::push_back( const TValue& value )
{
    CLockit _Lock(m_Lock);
    return m_List.push_back( value );
}

template<typename TValue>
void SafeList<TValue>::pop_back( void )
{
    CLockit _Lock(m_Lock);
    return m_List.push_front();
}

template<typename TValue>
void SafeList<TValue>::Clear( void )
{
    CLockit _Lock(m_Lock);
    return m_List.clear();
}

template<typename TValue>
void SafeList<TValue>::CopyTemp( LIST_TYPE& outTempList )
{
    outTempList.clear();

    // lock
    CLockit _Lock(m_Lock);

    // copy
    LIST_TYPE::iterator itr     = m_List.begin();
    LIST_TYPE::iterator itrEnd  = m_List.end();
    for ( ; itr != itrEnd; ++itr )
    {
        outTempList.push_back( *itr );
    }
}

template<typename TValue>
void SafeList<TValue>::CopyAndClear( LIST_TYPE& outTempList )
{
    outTempList.clear();

    // lock
    CLockit _Lock(m_Lock);

    // copy
    LIST_TYPE::iterator itr     = m_List.begin();
    LIST_TYPE::iterator itrEnd  = m_List.end();
    for ( ; itr != itrEnd; ++itr )
    {
        outTempList.push_back( *itr );
    }

    // clear
    m_List.clear();
}

template<typename TValue>
void SafeList<TValue>::Remove_If( Func_ForDel* pFunc, void* pParm1 = NULL, void* pParm2 = NULL )
{
    if ( pFunc == NULL )
    {
        return;
    }

    bool bIsDel = false;
    bool bIsContinue = false;

    // lock
    CLockit _Lock(m_Lock);

    // ɾ��
    LIST_TYPE::iterator itr    = m_List.begin();
    LIST_TYPE::iterator itrEnd = m_List.end();
    for ( ; itr != itrEnd;  )
    {
        bIsContinue = pFunc( *itr, pParm1, pParm2, bIsDel );

        // ɾ��
        if ( bIsDel )
        {
            itr = m_List.erase( itr );
        }
        else
        {
            ++itr;
        }

        // �����ж�
        if ( bIsContinue == false )
        {
            return;
        }
    }
}

template<typename TValue>
void SafeList<TValue>::ForEach( Func_ForEach* pFunc, void* pParm1 = NULL, void* pParm2 = NULL )
{
    if ( pFunc == NULL )
    {
        return;
    }

    // ��������ı���
    LIST_TYPE lstForEach;
    CopyTemp( lstForEach );

    // ��������,���ô�����
    LIST_TYPE::iterator itr    = lstForEach.begin();
    LIST_TYPE::iterator itrEnd = lstForEach.end();
    for ( ; itr != itrEnd; ++itr )
    {
        if ( pFunc( *itr, pParm1, pParm2 ) == false )
        {
            return;
        }
    }
}

#endif  // __SafeList_H__