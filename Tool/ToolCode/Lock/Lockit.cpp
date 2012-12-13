#include "StdAfx.h"
#include "Lockit.h"

namespace
{
    CLock   g_Locks[CLockit::eLOCK_NUMBERS];
}


CLockit::CLockit( void )
: m_Lock( g_Locks[eLOCK_DEFAULT] )
{
    m_Lock.Lock();
}

CLockit::CLockit( const CLockit& other )
: m_Lock( other.m_Lock )
{

}

CLockit& CLockit::operator=( const CLockit& other )
{
    m_Lock = other.m_Lock;
    return *this;
}

CLockit::CLockit( ELock eLock )
: m_Lock( g_Locks[eLock] )
{
    m_Lock.Lock();
}

CLockit::CLockit( CLock& lock )
: m_Lock( lock )
{
    m_Lock.Lock();    
}

CLockit::~CLockit( void )
{
    UnLockit();
}

void CLockit::UnLockit( void )
{
    m_Lock.UnLock();
}