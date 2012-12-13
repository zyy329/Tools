#ifdef _MSC_VER
#pragma once
#endif

#ifndef __TraverseQueue_H__
#define __TraverseQueue_H__

#include "Define/Macro.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   TraverseQueue.h     (遍历队列)
//  @brief  可遍历的多线程无锁队列( 要求 同一队列所有的压入操作在同一个线程中，所有压出操作在同一个线程中)
//          从队首压出元素，从队尾压入元素。 保证压入，压出 和 复数以上的循环读取 之间线程安全
//          局限性： 需保证被取出的对象要用来获得下一对象或主动进行释放，否则可能导致元素阻塞，大量的无用的元素得不到自动释放（因为释放是按队列依次释放的）
//                   建议在一个封闭的小环境内进行使用
//  @author 张逸云
//  @date   2011.09.27
//////////////////////////////////////////////////////////////////////////

template<class T>
class CTraverseQueue
{
public:
    struct S_TQ_Element     // 链
    {
        friend class CTraverseQueue;
    private:
        S_TQ_Element( void )    { Init(); }
        void Init( void )       { _pNext = NULL; _nReadCount = 0; _bErase = false; }
    public:
        inline const T& GetValue( void )    const { return _value; }

    private:
        T               _value;

        S_TQ_Element*   _pNext;
        unsigned int    _nReadCount;        // 使用者计数，没有使用者了才会被删除
        bool            _bErase;            // 删除标记，用于队首指针移动。且被标记为删除的元素不会再被读出
    };

public:
    CTraverseQueue( void );
    ~CTraverseQueue( void );

    void Clear( void );

    /** @brief  向队尾压入一个元素 */
    void PushBack( const T& Val );

    /** @brief  清理待删除队列中所有无效元素（在m_pReadFirstF之前，且使用者计数为0）, 不会直接删除而是放入空闲列表中，待重用 */
    void PopErase_Front( void );

    /** @brief  获得队首的第一个可读元素，以进行遍历. 并自动增加该元素的使用者计数, 如果没有可读元素将返回 NULL 
    *   @warning    请确保被取出的元素指针被用来或取下一元素或被释放了！！！！！！！否则可能造成阻塞，大量无用元素得不到释放
    */
    S_TQ_Element* GetFirstRead( void )
    {
        if ( m_pReadFirstF != m_pReadLast )     // 当第一个可读元素的前一位与队位指针重合时，说明不存在可读元素
        {
            m_pReadFirstF->_pNext->_nReadCount++;

            return m_pReadFirstF->_pNext;
        }

        return NULL;
    }
    
    /** 
    *   @brief  获得下一个可读读元素。 
    *   @param  ppElm,  当前元素指针。如果下一元素可读，将会被赋值为下一元素，否则将会被赋值为 NULL
    *                   并会自动减少当前元素的使用计数。增加下一元素的计数(如果可读)
    *   @param  bErase, 标记当前元素为删除，不可再被读取(对于类的使用者而言，相当于被删除)，且自动调整m_pReadFirstF指针。
    *   @return bool,   如果拥有下一可读元素, 将返回 true, 否则返回 false(没有可读得下一位或传入值无效).
    *   @warning    请确保被取出的元素指针被用来或取下一元素或被释放了！！！！！！！否则可能造成阻塞，大量无用元素得不到释放
    */
    bool GetNextRead( __in __out S_TQ_Element** ppElm, bool bErase = true );
    
    /** @brief  主动释放当前元素。 并将其本身置为 NULL 进行返回，以避免对无效值的使用。 会自动减少该元素的使用计数，自动调整m_pReadFirstF指针。*/
    void ReleaseElement( __in __out S_TQ_Element** ppElm );

private:
    S_TQ_Element* m_pReadFirstF;    // 第一个可读元素的前一位
    S_TQ_Element* m_pReadLast;      // 最后一个可读元素

    S_TQ_Element* m_pFirstDel;      // 待删除队列的队首。 待删除队列中的元素不可被或取，当_nReadCount为0时将进行实际删除(元素无效，被压入重用池)
                                    //                    待删除队列的队尾与可读队列相连

    S_TQ_Element* m_pIdleFirst;     // 存储空闲元素的列表，用于重用(以队列的方式来进行使用)
    S_TQ_Element* m_pIdleLast;      // 存储空闲元素的列表的最后一个元素
};



//////////////////////////////////////////////////////////////////////////

template<class T>
CTraverseQueue<T>::CTraverseQueue( void )
: m_pReadFirstF( new S_TQ_Element() )
, m_pReadLast( NULL )
, m_pFirstDel( new S_TQ_Element() )
, m_pIdleFirst( new S_TQ_Element() )
, m_pIdleLast( NULL )
{
    m_pReadFirstF->_bErase  = true;

    // 设置队尾
    m_pReadLast             = m_pReadFirstF;        // 可读队列
    m_pFirstDel->_pNext     = m_pReadFirstF;        // 待删除队列
    m_pIdleLast             = m_pIdleFirst;         // 重用队列
}

template<class T>
CTraverseQueue<T>::~CTraverseQueue( void )
{
    // 依次释放所有元素的空间
    S_TQ_Element* pDelTemp  = NULL;

    // 删除重用队列
    while ( m_pIdleFirst )
    {
        pDelTemp = m_pIdleFirst;
        m_pIdleFirst = m_pIdleFirst->_pNext;

        SAFE_DELETE( pDelTemp );
    }

    // 删除待删除和可读队列
    while ( m_pFirstDel )
    {
        pDelTemp = m_pFirstDel;
        m_pFirstDel = m_pFirstDel->_pNext;

        SAFE_DELETE( pDelTemp );
    }
}

template<class T>
void CTraverseQueue<T>::Clear( void )
{
    // 清理到待删除队列中
    S_TQ_Element* pRead = GetFirstRead();
    while ( GetNextRead(&pRead, true) )        // 遍历并标记为删除
    {
        // NULL
    }

    // 清理到重用队列中 (如果都无人在使用的话)
    PopErase_Front();
}

template<class T>
void CTraverseQueue<T>::PushBack( const T& Val )
{
    S_TQ_Element* pPushElm = NULL;
    if ( m_pIdleFirst != m_pIdleLast )
    {
        pPushElm = m_pIdleFirst;
        m_pIdleFirst = m_pIdleFirst->_pNext;
        pPushElm->Init();
    }
    else
    {
        pPushElm = new S_TQ_Element();
    }
    pPushElm->_value = Val;

    m_pReadLast->_pNext = pPushElm;
    m_pReadLast = pPushElm;                     // 原子操作，不用担心多线程问题
}

template<class T>
void CTraverseQueue<T>::PopErase_Front( void )
{
    // 清理待删除队列中 无人再使用的元素

    // 从队头开始清理，同步移动队头指针
    S_TQ_Element* pDelTemp = NULL;
    while ( m_pFirstDel->_pNext != m_pReadFirstF 
        && m_pFirstDel->_nReadCount == 0 )
    {
        pDelTemp = m_pFirstDel;
        m_pFirstDel = m_pFirstDel->_pNext;

        // 清理到空闲容器中
        pDelTemp->Init();
        m_pIdleLast->_pNext = pDelTemp;
        m_pIdleLast = m_pIdleLast->_pNext;
    }

    // 如果存在仍被使用的元素，需要跳过继续删除（此后将不再同步移动m_pFirstDel指针）
    if ( m_pFirstDel->_pNext != m_pReadFirstF )
    {
        S_TQ_Element* pLastTemp = m_pFirstDel;
        pDelTemp  = pLastTemp->_pNext;

        while ( pDelTemp->_pNext != m_pReadFirstF )
        {
            if ( pDelTemp->_nReadCount == 0 )   // 无人再进行使用,清理
            {
                pLastTemp->_pNext = pDelTemp->_pNext;

                // 清理到空闲容器中
                pDelTemp->Init();
                m_pIdleLast->_pNext = pDelTemp;
                m_pIdleLast = m_pIdleLast->_pNext;

                pDelTemp = pLastTemp->_pNext;                
            }
            else                                // 跳过,累加移位
            {
                pLastTemp = pDelTemp;
                pDelTemp = pDelTemp->_pNext;
            }
        }
    }
}

template<class T>
bool CTraverseQueue<T>::GetNextRead( __in __out S_TQ_Element** ppElm, bool bErase = true )
{
    S_TQ_Element* pCurElm = *ppElm;
    *ppElm = NULL;
    if ( pCurElm )
    {
        // 移动到下一可读位
        S_TQ_Element* pMovElm = pCurElm;
        while ( pMovElm && pMovElm != m_pReadLast )
        {
            pMovElm = pMovElm->_pNext;

            if ( pMovElm->_bErase == false )        // 找到了一个有效的可读位
            {
                pMovElm->_nReadCount++;
                *ppElm = pMovElm;

                break;
            }
        }

        // 释放当前位
        if ( bErase )
        {
            ReleaseElement( &pCurElm );       // 这里面也同样进行递减操作
        }
        else
        {
            pCurElm->_nReadCount--;
        }
    }

    return *ppElm == NULL ? false : true;
}

template<class T>
void CTraverseQueue<T>::ReleaseElement( __in __out S_TQ_Element** ppElm )
{
    S_TQ_Element* pTemp = *ppElm;
    if ( pTemp )
    {
        *ppElm = NULL;

        pTemp->_bErase = true;
        pTemp->_nReadCount--;

        // 调整m_pReadFirstF指针
        while ( m_pReadFirstF != m_pReadLast            // 不是结束元素，下一位可用
            && m_pReadFirstF->_bErase == true )
        {
            m_pReadFirstF = m_pReadFirstF->_pNext;
        }
    }
}

#endif  // __TraverseQueue_H__