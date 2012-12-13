#ifdef _MSC_VER
#pragma once
#endif

#ifndef __LockFree_Queue_H__
#define __LockFree_Queue_H__

#include "Define/Macro.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   LockFree_Queue.h
//  @brief  多线程无锁队列
//  @author 张逸云
//  @date   2011-11-28
//////////////////////////////////////////////////////////////////////////

template<class T>
class CLockFreeQueue
{
public:
    struct S_List     // 队列元素结构体（链表的形式）
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

    /** @brief  向队尾压入一个元素(复制) */
    void PushBack( const T& Val );

    /** @brief  从队头取出元素 
    *   @return bool, 是否取到了元素
    */
    bool PopFront( T& retVal );

private:
    S_List* m_pQueueFirstF;     // 队首元素的前一位(占位元素，内容已无效，用于指示队首元素的所在)
    S_List* m_pQueueLast;       // 队尾元素

    S_List* m_pIdleFirst;       // 存储空闲元素的列表，用于重用(以队列的方式来进行使用)
    S_List* m_pIdleLast;        // 存储空闲元素的列表的最后一个元素
};


//////////////////////////////////////////////////////////////////////////

template<class T>
CLockFreeQueue<T>::CLockFreeQueue( void )
: m_pQueueFirstF( new S_List() )    // 占位元素
, m_pQueueLast( NULL )
, m_pIdleFirst( new S_List() )      // 占位元素
, m_pIdleLast( NULL )
{
    m_pQueueLast = m_pQueueFirstF;
    m_pIdleLast = m_pIdleFirst;
}

template<class T>
CLockFreeQueue<T>::~CLockFreeQueue( void )
{
    // 全部清理到重用队列中去
    Clear();

    // 删除重用队列, 依次释放所有元素的空间
    S_List* pDelTemp  = NULL;
    while ( m_pIdleFirst )
    {
        pDelTemp = m_pIdleFirst;
        m_pIdleFirst = m_pIdleFirst->_pNext;

        SAFE_DELETE( pDelTemp );
    }

    //// 删除队列
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
        // 从重用队列中取出元素
        pPushElm        = m_pIdleFirst;
        m_pIdleFirst    = m_pIdleFirst->_pNext;
        pPushElm->Init();
    }
    else
    {
        // 重新生成一个元素
        pPushElm = new S_List();
    }
    pPushElm->_value = Val;

    m_pQueueLast->_pNext    = pPushElm;
    m_pQueueLast            = m_pQueueLast->_pNext;     // 临界交叉点，由于指针赋值操作是原子的，所以不用担心多线程问题
}

template<class T>
bool CLockFreeQueue<T>::PopFront( T& retVal )
{
    if ( m_pQueueFirstF != m_pQueueLast )
    {
        retVal = m_pQueueFirstF->_pNext->_value;

        // 将不用的元素清理到重用队列中
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