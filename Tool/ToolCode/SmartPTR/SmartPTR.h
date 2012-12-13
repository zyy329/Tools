//////////////////////////////////////////////////////////////////////////
//
//  文件名称：SmartPTR.h
//  文件说明：智能指针;
//            由 InitPTR 进行创建,存储实例
//               ReleasePTR 进行主动释放.删除实例,使得所有共用副本不可用.
//               Unlink 进行断开连接,使得仅当前副本不再可用,其它副本仍可用
//
//
//  作者：zyy
//  日期：2010.01.24
//
//////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif

#ifndef __SmartPTRH__
#define __SmartPTRH__


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if (p){delete (p); (p) = 0;} }
#endif


template<typename T>
class CSmartPTR
{
private:
    T* m_p;             // 共用的实例
    size_t* m_pUse;     // 共用计数器

public:
    CSmartPTR( void ) : m_p( NULL ), m_pUse( NULL ) {}
    CSmartPTR( const CSmartPTR& other ) : m_p( other.m_p ), m_pUse( other.m_pUse ) { if ( m_pUse ) ++*m_pUse; }
    ~CSmartPTR( void ) { Unlink(); }

    CSmartPTR& operator=( const CSmartPTR& other );

public:
    void InitPTR( T* pObj );        // 创建初始化共用实例.  请传入一个new出的指针，智能指针将负责对其进行释放
    void ReleasePTR( void );        // 主动释放指针,其他智能指针副本也会变得不可用
    void Unlink( void );            // 断开与原有实例的连接,清空数据 <--- 由于该方法,暂未实现只读指针的设想;待解决

    inline bool isUseAble( void )           const { if (m_p) return true; else  return false; }     // 当返回值为false时,应调用Unlink()进行断开连接

    // 使用前应先用isUseAble进行判断
    inline T& operator*( void )             const { if (m_p) return *m_p; else throw std::logic_error("unbound Object"); }
    inline T* operator->( void )            const { if (m_p) return m_p; else throw std::logic_error("unbound Object"); }
    inline T* getObj( void )                const { if (m_p) return m_p; else throw std::logic_error("unbound Object"); }
    inline size_t getUserNum( void )        const { if (m_pUse) return *m_pUse; else throw std::logic_error("unbound Object"); }
    //const CSmartPTR getCopy( void )         const;      // 获得该智能指针的拷贝副本,可对实例进行读写操作,但不可创建和删除

    ////////////////////////////////////////////////////////////////////////////
    ////  隐患,可通过这个函数获得 m_pUse 进而直接修改其副本
    ////  但为编写 SmartP_Cast, 暂行办法. 使用时应仅SmartP_Cast()可调用这两个函数.
    //inline size_t* getUseP( void )          const { return m_pUse; }
    ////////////////////////////////////////////////////////////////////////////

    //// 智能指针类型转换.转换后新智能指针仅m_p指针类型与原指针不同.实际指向的内容相同,共用计数器.可用于多态指针类型的转换
    //template<typename T1> void SmartP_Cast( CSmartPTR<T1> castP );

private:
    // 计数器递减控制,当计数器递减完毕,而数据任可用时,清除数据
    inline void decr_user( void ) { if ( m_pUse && (--*m_pUse == 0) ){ SAFE_DELETE(m_p); SAFE_DELETE(m_pUse); } }
};


// 模板定义
template<typename T>
CSmartPTR<T>& CSmartPTR<T>::operator =( const CSmartPTR<T>& other )
{
    Unlink();           // 断开与原有对象的连接,因为该句柄将会被赋予新的副本,原来的副本将不再指向

    m_p         = other.m_p;
    m_pUse      = other.m_pUse;
    if ( m_pUse )
    {
        ++*m_pUse;
    }

    return *this;
}

template<typename T>
void CSmartPTR<T>::InitPTR( T* pObj )
{
    Unlink();           // 断开与原有对象的连接

    // 重新初始化数据
    m_p         = pObj;
    m_pUse      = new size_t( 1 );
}

template<typename T>
void CSmartPTR<T>::ReleasePTR( void )
{
    if ( m_p )
    {
        SAFE_DELETE(m_p);
        Unlink();
    }
}

template<typename T>
void CSmartPTR<T>::Unlink( void )
{
    decr_user();        // 断开与原有对象的连接(数据任在)

    // 清空数据
    m_p         = NULL;
    m_pUse      = NULL;
}

//template<typename T>
//const CSmartPTR<T> CSmartPTR<T>::getCopy( void ) const
//{

//}

//// 智能指针类型转换.转换后新智能指针仅m_p指针类型与原指针不同.实际指向的内容相同,共用计数器.可用于多态指针类型的转换
//template<typename T> template<typename T1>
//void CSmartPTR<T>::SmartP_Cast( CSmartPTR<T1> castP )
//{
//    Unlink();           // 断开与原有对象的连接,因为该句柄将会被赋予新的副本,原来的副本将不再指向
//
//    m_p         = static_cast<T*>( castP.getObj() );
//    m_pUse      = castP.getUseP();
//    if ( m_pUse )
//    {
//        ++*m_pUse;
//    }
//}

#endif  // __SmartPTRH__