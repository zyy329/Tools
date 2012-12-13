#ifdef _MSC_VER
#pragma once
#endif

#ifndef __LockFree_Queue_H__
#define __LockFree_Queue_H__

#include "Define/Macro.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   LockFree_Queue.h
//  @brief  ���߳���������
//  @author ������
//  @date   2011-11-28
//////////////////////////////////////////////////////////////////////////

template<class T>
class CLockFreeQueue
{
public:
    struct S_List     // ����Ԫ�ؽṹ�壨�������ʽ��
    {
        friend class CLockFreeQueue;
    private:
        S_List( void )      { Init(); }
        void Init( void )   { _pNext = NULL; }
    public:
        inline const T& GetValue( void )    const { return _value; }

    private:
        T           _value;

        S_List*     _pNext;
    };

public:
    CLockFreeQueue( void );
    ~CLockFreeQueue( void );

    void Clear( void );

    /** @brief  ���βѹ��һ��Ԫ��(����) */
    void PushBack( const T& Val );

    /** @brief  �Ӷ�ͷȡ��Ԫ�� 
    *   @return bool, �Ƿ�ȡ����Ԫ��
    */
    bool PopFront( T& retVal );

private:
    S_List* m_pQueueFirstF;     // ����Ԫ�ص�ǰһλ(ռλԪ�أ���������Ч������ָʾ����Ԫ�ص�����)
    S_List* m_pQueueLast;       // ��βԪ��

    S_List* m_pIdleFirst;       // �洢����Ԫ�ص��б���������(�Զ��еķ�ʽ������ʹ��)
    S_List* m_pIdleLast;        // �洢����Ԫ�ص��б�����һ��Ԫ��
};


//////////////////////////////////////////////////////////////////////////

template<class T>
CLockFreeQueue<T>::CLockFreeQueue( void )
: m_pQueueFirstF( new S_List() )    // ռλԪ��
, m_pQueueLast( NULL )
, m_pIdleFirst( new S_List() )      // ռλԪ��
, m_pIdleLast( NULL )
{
    m_pQueueLast = m_pQueueFirstF;
    m_pIdleLast = m_pIdleFirst;
}

template<class T>
CLockFreeQueue<T>::~CLockFreeQueue( void )
{
    // ȫ���������ö�����ȥ
    Clear();

    // ɾ�����ö���, �����ͷ�����Ԫ�صĿռ�
    S_List* pDelTemp  = NULL;
    while ( m_pIdleFirst )
    {
        pDelTemp = m_pIdleFirst;
        m_pIdleFirst = m_pIdleFirst->_pNext;

        SAFE_DELETE( pDelTemp );
    }

    //// ɾ������
    //while ( m_pQueueFirstF )
    //{
    //    pDelTemp = m_pQueueFirstF;
    //    m_pQueueFirstF = m_pQueueFirstF->_pNext;

    //    SAFE_DELETE( pDelTemp );
    //}
}

template<class T>
void CLockFreeQueue<T>::Clear( void )
{
    T temp;
    while ( PopFront(temp) == true )
    {
        // NULL
    }
}


template<class T>
void CLockFreeQueue<T>::PushBack( const T& Val )
{
    S_List* pPushElm = NULL;
    if ( m_pIdleFirst != m_pIdleLast )
    {
        // �����ö�����ȡ��Ԫ��
        pPushElm        = m_pIdleFirst;
        m_pIdleFirst    = m_pIdleFirst->_pNext;
        pPushElm->Init();
    }
    else
    {
        // ��������һ��Ԫ��
        pPushElm = new S_List();
    }
    pPushElm->_value = Val;

    m_pQueueLast->_pNext    = pPushElm;
    m_pQueueLast            = m_pQueueLast->_pNext;     // �ٽ罻��㣬����ָ�븳ֵ������ԭ�ӵģ����Բ��õ��Ķ��߳�����
}

template<class T>
bool CLockFreeQueue<T>::PopFront( T& retVal )
{
    if ( m_pQueueFirstF != m_pQueueLast )
    {
        retVal = m_pQueueFirstF->_pNext->_value;

        // �����õ�Ԫ���������ö�����
        S_List* pDelTemp = NULL;
        pDelTemp = m_pQueueFirstF;
        m_pQueueFirstF = m_pQueueFirstF->_pNext;

        pDelTemp->Init();
        m_pIdleLast->_pNext = pDelTemp;
        m_pIdleLast         = m_pIdleLast->_pNext;

        return true;
    }
    else
    {
        return false;
    }
}

#endif  // __LockFree_Queue_H__