#ifdef _MSC_VER
#pragma once
#endif

#ifndef __TraverseQueue_H__
#define __TraverseQueue_H__

#include "Define/Macro.h"

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   TraverseQueue.h     (��������)
//  @brief  �ɱ����Ķ��߳���������( Ҫ�� ͬһ�������е�ѹ�������ͬһ���߳��У�����ѹ��������ͬһ���߳���)
//          �Ӷ���ѹ��Ԫ�أ��Ӷ�βѹ��Ԫ�ء� ��֤ѹ�룬ѹ�� �� �������ϵ�ѭ����ȡ ֮���̰߳�ȫ
//          �����ԣ� �豣֤��ȡ���Ķ���Ҫ���������һ��������������ͷţ�������ܵ���Ԫ�����������������õ�Ԫ�صò����Զ��ͷţ���Ϊ�ͷ��ǰ����������ͷŵģ�
//                   ������һ����յ�С�����ڽ���ʹ��
//  @author ������
//  @date   2011.09.27
//////////////////////////////////////////////////////////////////////////

template<class T>
class CTraverseQueue
{
public:
    struct S_TQ_Element     // ��
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
        unsigned int    _nReadCount;        // ʹ���߼�����û��ʹ�����˲Żᱻɾ��
        bool            _bErase;            // ɾ����ǣ����ڶ���ָ���ƶ����ұ����Ϊɾ����Ԫ�ز����ٱ�����
    };

public:
    CTraverseQueue( void );
    ~CTraverseQueue( void );

    void Clear( void );

    /** @brief  ���βѹ��һ��Ԫ�� */
    void PushBack( const T& Val );

    /** @brief  �����ɾ��������������ЧԪ�أ���m_pReadFirstF֮ǰ����ʹ���߼���Ϊ0��, ����ֱ��ɾ�����Ƿ�������б��У������� */
    void PopErase_Front( void );

    /** @brief  ��ö��׵ĵ�һ���ɶ�Ԫ�أ��Խ��б���. ���Զ����Ӹ�Ԫ�ص�ʹ���߼���, ���û�пɶ�Ԫ�ؽ����� NULL 
    *   @warning    ��ȷ����ȡ����Ԫ��ָ�뱻������ȡ��һԪ�ػ��ͷ��ˣ�������������������������������������Ԫ�صò����ͷ�
    */
    S_TQ_Element* GetFirstRead( void )
    {
        if ( m_pReadFirstF != m_pReadLast )     // ����һ���ɶ�Ԫ�ص�ǰһλ���λָ���غ�ʱ��˵�������ڿɶ�Ԫ��
        {
            m_pReadFirstF->_pNext->_nReadCount++;

            return m_pReadFirstF->_pNext;
        }

        return NULL;
    }
    
    /** 
    *   @brief  �����һ���ɶ���Ԫ�ء� 
    *   @param  ppElm,  ��ǰԪ��ָ�롣�����һԪ�ؿɶ������ᱻ��ֵΪ��һԪ�أ����򽫻ᱻ��ֵΪ NULL
    *                   �����Զ����ٵ�ǰԪ�ص�ʹ�ü�����������һԪ�صļ���(����ɶ�)
    *   @param  bErase, ��ǵ�ǰԪ��Ϊɾ���������ٱ���ȡ(�������ʹ���߶��ԣ��൱�ڱ�ɾ��)�����Զ�����m_pReadFirstFָ�롣
    *   @return bool,   ���ӵ����һ�ɶ�Ԫ��, ������ true, ���򷵻� false(û�пɶ�����һλ����ֵ��Ч).
    *   @warning    ��ȷ����ȡ����Ԫ��ָ�뱻������ȡ��һԪ�ػ��ͷ��ˣ�������������������������������������Ԫ�صò����ͷ�
    */
    bool GetNextRead( __in __out S_TQ_Element** ppElm, bool bErase = true );
    
    /** @brief  �����ͷŵ�ǰԪ�ء� �����䱾����Ϊ NULL ���з��أ��Ա������Чֵ��ʹ�á� ���Զ����ٸ�Ԫ�ص�ʹ�ü������Զ�����m_pReadFirstFָ�롣*/
    void ReleaseElement( __in __out S_TQ_Element** ppElm );

private:
    S_TQ_Element* m_pReadFirstF;    // ��һ���ɶ�Ԫ�ص�ǰһλ
    S_TQ_Element* m_pReadLast;      // ���һ���ɶ�Ԫ��

    S_TQ_Element* m_pFirstDel;      // ��ɾ�����еĶ��ס� ��ɾ�������е�Ԫ�ز��ɱ���ȡ����_nReadCountΪ0ʱ������ʵ��ɾ��(Ԫ����Ч����ѹ�����ó�)
                                    //                    ��ɾ�����еĶ�β��ɶ���������

    S_TQ_Element* m_pIdleFirst;     // �洢����Ԫ�ص��б���������(�Զ��еķ�ʽ������ʹ��)
    S_TQ_Element* m_pIdleLast;      // �洢����Ԫ�ص��б�����һ��Ԫ��
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

    // ���ö�β
    m_pReadLast             = m_pReadFirstF;        // �ɶ�����
    m_pFirstDel->_pNext     = m_pReadFirstF;        // ��ɾ������
    m_pIdleLast             = m_pIdleFirst;         // ���ö���
}

template<class T>
CTraverseQueue<T>::~CTraverseQueue( void )
{
    // �����ͷ�����Ԫ�صĿռ�
    S_TQ_Element* pDelTemp  = NULL;

    // ɾ�����ö���
    while ( m_pIdleFirst )
    {
        pDelTemp = m_pIdleFirst;
        m_pIdleFirst = m_pIdleFirst->_pNext;

        SAFE_DELETE( pDelTemp );
    }

    // ɾ����ɾ���Ϳɶ�����
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
    // ������ɾ��������
    S_TQ_Element* pRead = GetFirstRead();
    while ( GetNextRead(&pRead, true) )        // ���������Ϊɾ��
    {
        // NULL
    }

    // �������ö����� (�����������ʹ�õĻ�)
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
    m_pReadLast = pPushElm;                     // ԭ�Ӳ��������õ��Ķ��߳�����
}

template<class T>
void CTraverseQueue<T>::PopErase_Front( void )
{
    // �����ɾ�������� ������ʹ�õ�Ԫ��

    // �Ӷ�ͷ��ʼ����ͬ���ƶ���ͷָ��
    S_TQ_Element* pDelTemp = NULL;
    while ( m_pFirstDel->_pNext != m_pReadFirstF 
        && m_pFirstDel->_nReadCount == 0 )
    {
        pDelTemp = m_pFirstDel;
        m_pFirstDel = m_pFirstDel->_pNext;

        // ��������������
        pDelTemp->Init();
        m_pIdleLast->_pNext = pDelTemp;
        m_pIdleLast = m_pIdleLast->_pNext;
    }

    // ��������Ա�ʹ�õ�Ԫ�أ���Ҫ��������ɾ�����˺󽫲���ͬ���ƶ�m_pFirstDelָ�룩
    if ( m_pFirstDel->_pNext != m_pReadFirstF )
    {
        S_TQ_Element* pLastTemp = m_pFirstDel;
        pDelTemp  = pLastTemp->_pNext;

        while ( pDelTemp->_pNext != m_pReadFirstF )
        {
            if ( pDelTemp->_nReadCount == 0 )   // �����ٽ���ʹ��,����
            {
                pLastTemp->_pNext = pDelTemp->_pNext;

                // ��������������
                pDelTemp->Init();
                m_pIdleLast->_pNext = pDelTemp;
                m_pIdleLast = m_pIdleLast->_pNext;

                pDelTemp = pLastTemp->_pNext;                
            }
            else                                // ����,�ۼ���λ
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
        // �ƶ�����һ�ɶ�λ
        S_TQ_Element* pMovElm = pCurElm;
        while ( pMovElm && pMovElm != m_pReadLast )
        {
            pMovElm = pMovElm->_pNext;

            if ( pMovElm->_bErase == false )        // �ҵ���һ����Ч�Ŀɶ�λ
            {
                pMovElm->_nReadCount++;
                *ppElm = pMovElm;

                break;
            }
        }

        // �ͷŵ�ǰλ
        if ( bErase )
        {
            ReleaseElement( &pCurElm );       // ������Ҳͬ�����еݼ�����
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

        // ����m_pReadFirstFָ��
        while ( m_pReadFirstF != m_pReadLast            // ���ǽ���Ԫ�أ���һλ����
            && m_pReadFirstF->_bErase == true )
        {
            m_pReadFirstF = m_pReadFirstF->_pNext;
        }
    }
}

#endif  // __TraverseQueue_H__