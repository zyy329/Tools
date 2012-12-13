#include "StdAfx.h"
#include "Lock.h"

CLock::CLock( void )
{
    InitializeCriticalSection( &m_cs );
}

CLock::~CLock( void )
{
    DeleteCriticalSection( &m_cs );
}

void CLock::Lock( void )
{
    EnterCriticalSection( &m_cs );
}

void CLock::UnLock( void )
{
    LeaveCriticalSection( &m_cs );
}