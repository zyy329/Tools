#include "StdAfx.h"
#include "PoolMng.h"

#include "Pool.h"

IMPLEMENT_SINGLETON(CPool_Manager)

CPool_Manager::CPool_Manager( void )
{

}

CPool_Manager::~CPool_Manager( void )
{
    ClearPool();
}

void CPool_Manager::RegistPool( CPool_Base* pPool )
{
    if ( pPool != NULL )
    {
        m_mapPool.Insert( pPool->GetPoolName(), pPool, Use_DebugCode );
    }
}

void CPool_Manager::ClearPool( void )
{
    m_mapPool.Clear();
}