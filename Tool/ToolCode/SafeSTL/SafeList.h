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
//  @brief  线程安全 List 容器
//  @author 张逸云
//  @date   2012.08.22
//////////////////////////////////////////////////////////////////////////

template<typename TValue>
class SafeList 
{
public:
    typedef std::list<TValue> LIST_TYPE;
    /*  @breif  遍历回调函数格式定义
    *   @return bool, 遍历控制标记,  true, 继续遍历;  false, 终止遍历
    */
    typedef bool (Func_ForEach) ( const TValue& value, void* pParm1, void* pParm2 );
    
    /*  @breif  删除回调函数格式定义
    *   @param  outIsDel, 输出参数, 用于同志回调是否进行删除, true,删除;  false,不删除
    *   @return bool, 遍历控制标记,  true, 继续遍历;  false, 终止遍历
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
    /*  @breif 复制出备份,并将 SafeList 中的元素清理. 用于 当容器中存放的是指针对向时,保持清理和复制在同一个锁中 */
    void CopyAndClear( LIST_TYPE& outTempList );


    /*  @breif  循环遍历容器,将符合条件的值删掉. 通过回调函数确定是否符合删除条件
    *   @warning    该遍历期间会完全锁住容器, 请不要使用太过复杂的判断逻辑
    */
    void Remove_If( Func_ForDel* pFunc, void* pParm1 = NULL, void* pParm2 = NULL );

    /*  @breif 循环遍历函数*/
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

    // 删除
    LIST_TYPE::iterator itr    = m_List.begin();
    LIST_TYPE::iterator itrEnd = m_List.end();
    for ( ; itr != itrEnd;  )
    {
        bIsContinue = pFunc( *itr, pParm1, pParm2, bIsDel );

        // 删除
        if ( bIsDel )
        {
            itr = m_List.erase( itr );
        }
        else
        {
            ++itr;
        }

        // 结束判断
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

    // 获得容器的备份
    LIST_TYPE lstForEach;
    CopyTemp( lstForEach );

    // 便利备份,调用处理函数
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