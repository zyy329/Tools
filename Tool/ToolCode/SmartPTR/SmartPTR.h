//////////////////////////////////////////////////////////////////////////
//
//  �ļ����ƣ�SmartPTR.h
//  �ļ�˵��������ָ��;
//            �� InitPTR ���д���,�洢ʵ��
//               ReleasePTR ���������ͷ�.ɾ��ʵ��,ʹ�����й��ø���������.
//               Unlink ���жϿ�����,ʹ�ý���ǰ�������ٿ���,���������Կ���
//
//
//  ���ߣ�zyy
//  ���ڣ�2010.01.24
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
    T* m_p;             // ���õ�ʵ��
    size_t* m_pUse;     // ���ü�����

public:
    CSmartPTR( void ) : m_p( NULL ), m_pUse( NULL ) {}
    CSmartPTR( const CSmartPTR& other ) : m_p( other.m_p ), m_pUse( other.m_pUse ) { if ( m_pUse ) ++*m_pUse; }
    ~CSmartPTR( void ) { Unlink(); }

    CSmartPTR& operator=( const CSmartPTR& other );

public:
    void InitPTR( T* pObj );        // ������ʼ������ʵ��.  �봫��һ��new����ָ�룬����ָ�뽫�����������ͷ�
    void ReleasePTR( void );        // �����ͷ�ָ��,��������ָ�븱��Ҳ���ò�����
    void Unlink( void );            // �Ͽ���ԭ��ʵ��������,������� <--- ���ڸ÷���,��δʵ��ֻ��ָ�������;�����

    inline bool isUseAble( void )           const { if (m_p) return true; else  return false; }     // ������ֵΪfalseʱ,Ӧ����Unlink()���жϿ�����

    // ʹ��ǰӦ����isUseAble�����ж�
    inline T& operator*( void )             const { if (m_p) return *m_p; else throw std::logic_error("unbound Object"); }
    inline T* operator->( void )            const { if (m_p) return m_p; else throw std::logic_error("unbound Object"); }
    inline T* getObj( void )                const { if (m_p) return m_p; else throw std::logic_error("unbound Object"); }
    inline size_t getUserNum( void )        const { if (m_pUse) return *m_pUse; else throw std::logic_error("unbound Object"); }
    //const CSmartPTR getCopy( void )         const;      // ��ø�����ָ��Ŀ�������,�ɶ�ʵ�����ж�д����,�����ɴ�����ɾ��

    ////////////////////////////////////////////////////////////////////////////
    ////  ����,��ͨ������������ m_pUse ����ֱ���޸��丱��
    ////  ��Ϊ��д SmartP_Cast, ���а취. ʹ��ʱӦ��SmartP_Cast()�ɵ�������������.
    //inline size_t* getUseP( void )          const { return m_pUse; }
    ////////////////////////////////////////////////////////////////////////////

    //// ����ָ������ת��.ת����������ָ���m_pָ��������ԭָ�벻ͬ.ʵ��ָ���������ͬ,���ü�����.�����ڶ�ָ̬�����͵�ת��
    //template<typename T1> void SmartP_Cast( CSmartPTR<T1> castP );

private:
    // �������ݼ�����,���������ݼ����,�������ο���ʱ,�������
    inline void decr_user( void ) { if ( m_pUse && (--*m_pUse == 0) ){ SAFE_DELETE(m_p); SAFE_DELETE(m_pUse); } }
};


// ģ�嶨��
template<typename T>
CSmartPTR<T>& CSmartPTR<T>::operator =( const CSmartPTR<T>& other )
{
    Unlink();           // �Ͽ���ԭ�ж��������,��Ϊ�þ�����ᱻ�����µĸ���,ԭ���ĸ���������ָ��

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
    Unlink();           // �Ͽ���ԭ�ж��������

    // ���³�ʼ������
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
    decr_user();        // �Ͽ���ԭ�ж��������(��������)

    // �������
    m_p         = NULL;
    m_pUse      = NULL;
}

//template<typename T>
//const CSmartPTR<T> CSmartPTR<T>::getCopy( void ) const
//{

//}

//// ����ָ������ת��.ת����������ָ���m_pָ��������ԭָ�벻ͬ.ʵ��ָ���������ͬ,���ü�����.�����ڶ�ָ̬�����͵�ת��
//template<typename T> template<typename T1>
//void CSmartPTR<T>::SmartP_Cast( CSmartPTR<T1> castP )
//{
//    Unlink();           // �Ͽ���ԭ�ж��������,��Ϊ�þ�����ᱻ�����µĸ���,ԭ���ĸ���������ָ��
//
//    m_p         = static_cast<T*>( castP.getObj() );
//    m_pUse      = castP.getUseP();
//    if ( m_pUse )
//    {
//        ++*m_pUse;
//    }
//}

#endif  // __SmartPTRH__