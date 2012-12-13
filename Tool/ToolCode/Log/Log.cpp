#include "stdafx.h"
#include "Log.h"

IMPLEMENT_SINGLETON( CLog )
const TString CLog::ms_strPoolName = _T("Log");
CPool<CLog::S_LogMsg> CLog::ms_LogPool(CLog::ms_strPoolName);

CLog::CLog( void )
: m_hLog_Thread_Handle( NULL )
, m_nLog_Thread_Id( 0 )
, m_bRunLog( false )
, m_bLogSwitch( true )
, m_strLogBasePath( _T("Log\\") /*_T("E:\\Code\\My\\Tool\\Log\\")*/ )
{
    CreateLogDirectory();

    // ������־�߳�
    m_hLog_Thread_Handle = (HANDLE)_beginthreadex( NULL, 0, (unsigned(__stdcall*)(void*)) Thread_Log, 0, 0, &m_nLog_Thread_Id );
    m_bRunLog = true;
}

CLog::~CLog( void )
{
    // �ر���־�߳�
    m_bRunLog = false;
    WaitForSingleObject( m_hLog_Thread_Handle, INFINITE );
    CloseHandle( m_hLog_Thread_Handle );

    // ���һ�ε�����־���,��ȷ���������������
    PrintLogMsg();

    // �ر���־�ļ�
    MapLogFileItr itr       = m_mapLogFile.begin();
    MapLogFileItr itrEnd    = m_mapLogFile.end();
    for ( ; itr != itrEnd; ++itr )
    {
        if( itr->second.pFile != NULL )
        {
            fclose(itr->second.pFile);
            itr->second.pFile = NULL;
        }
    }
    m_mapLogFile.clear();
}


void CLog::LogMsg( EErrorMsg_Type eType, const TCHAR * pszParam,... )
{
    if ( m_bRunLog == false )       // ��־�ѹر�
    {
        return;
    }

    static const int BUF_SIZE = 4096;
    TCHAR szCharBuf[BUF_SIZE];
    memset( &szCharBuf, 0x00, sizeof(szCharBuf) );

    // va string
    va_list va; 
    va_start(va, pszParam); 

    StringCbVPrintf(szCharBuf, BUF_SIZE, pszParam, va);

    va_end(va);
    

    // add
    S_LogMsg* pLog = ms_LogPool.Pool_Pop();
    pLog->eType = eType;
    pLog->strMsg = szCharBuf;

    m_lstLogMsg.push_back( pLog );
}

//////////////////////////////////////////////////////////////////////////
void CLog::CreateLogDirectory( void )
{
    // ������Ŀ¼
    CreateDirectory( m_strLogBasePath.c_str(), NULL );

    // ����������־���ͣ�������־�ṹĿ¼
    for ( int i = eERROR_MSG_TYPE_START; i != eERROR_MSG_TYPE_NUMBERS; ++i )
    {
        TString strDirectory = m_strLogBasePath;
        strDirectory += GetTypeName( (EErrorMsg_Type)i );
        CreateDirectory( strDirectory.c_str(), NULL );
    }
}

void CLog::PrintLogMsg( void )
{
    // ȡ�������������־
    SMAP_LOG::LIST_TYPE tempLst;
    m_lstLogMsg.CopyAndClear( tempLst );

    // ���� �������ɾ��
    SMAP_LOG::LIST_TYPE::iterator itr = tempLst.begin();
    SMAP_LOG::LIST_TYPE::iterator itrEnd = tempLst.end();
    for ( ; itr != itrEnd; ++itr )
    {
        // ��־���
        LogToFile( **itr );

        // ɾ���ѱ��������־
        ms_LogPool.Pool_Push( *itr );
    }
}

void CLog::LogToFile( const S_LogMsg& logMsg )
{
    // ��־��Ϣ��ʽ��, ͳһ���һЩ��ʽ������Ϣ(������)
    const TString& strLogMsg = FormMsg( logMsg.strMsg );

    // �����ж�
    if ( logMsg.eType <= eERROR_MSG_TYPE_START || logMsg.eType >= eErrLog )
    {
        // bad, ��־���Ͳ��Ϸ�
        ErrLogMsg( logMsg.eType, strLogMsg, _T("Err Log Type") );
        return;
    }

    // ��־�ļ����
    const SOpenFile* pOpenFlil = GetLogFile( logMsg.eType );
    if ( pOpenFlil == NULL )
    {
        // bad, ��־�ļ����ʧ��
        ErrLogMsg( logMsg.eType, strLogMsg, _T("Cannot open logFile") );
        return;
    }

    // ���
    if ( WriteFile( pOpenFlil->pFile, strLogMsg ) == false )
    {
        // bad, ��־д��ʧ��
        ErrLogMsg( logMsg.eType, strLogMsg, _T("Write logFile failed!") );
        return;
    }
}

TString CLog::FormMsg( const TString& strMsg )
{
    return strMsg + _T("\n");
}

const CLog::SOpenFile* CLog::GetLogFile( EErrorMsg_Type eType )
{
    static SOpenFile* pOpenFile;
    pOpenFile = NULL;

    // ��־�ļ�������
    if ( eType > eERROR_MSG_TYPE_START && eType < eERROR_MSG_TYPE_NUMBERS )
    {
        MapLogFileItr itr = m_mapLogFile.find( eType );

        if ( itr != m_mapLogFile.end() )
        {
            // ����Ѵ򿪵���־�ļ�����
            pOpenFile = &(itr->second);
        }
        else
        {
            // ��������־�ļ�����
            pair<MapLogFileItr, bool> ret = m_mapLogFile.insert( make_pair(eType, SOpenFile()) );
            if ( ret.second == true )
            {
                pOpenFile = &(ret.first->second);
            }
        }
    }

    // ��־�ļ�ָ��ά��
    CTime tempTime = CTime::GetCurrentTime();
    if( pOpenFile != NULL )
    {
        // ��־�ļ�ά��
        if ( pOpenFile->pFile == NULL || (pOpenFile->cTime.GetDay() != tempTime.GetDay()) )
        {
            if(pOpenFile->pFile != NULL) fclose(pOpenFile->pFile);

            TString strFliePath;
            GetLogFilePath( eType, strFliePath );
            pOpenFile->pFile = _tfsopen(strFliePath.c_str(), _T("a+t"), _SH_DENYNO);

            if (pOpenFile->pFile != NULL)
            {
                pOpenFile->cTime = tempTime;
            }
        }
    }

    return pOpenFile;
}

void CLog::GetLogFilePath( EErrorMsg_Type eType, TString& outFileName )
{
    static const int BUF_SIZE = 256;
    TCHAR szCharBuf[BUF_SIZE];

    CTime tempTime = CTime::GetCurrentTime();
    switch ( eType )
    {
    case eDefault:
    case eErrLog:
        {
            _stprintf_s( szCharBuf, BUF_SIZE, _T("%s%s\\-%02d%02d.log")
                , m_strLogBasePath.c_str(), GetTypeName(eType).c_str(), tempTime.GetMonth(), tempTime.GetDay() );
        }break;
    default:
        {
       
        }break;
    }

    //outFileName.str
    outFileName = szCharBuf;
}


bool CLog::WriteFile( FILE* pFile, const TString& strMsg )
{
    bool bLog = false;
    if ( pFile )
    {
        fflush(pFile);

        int ret = _ftprintf( pFile, strMsg.c_str() );

        if(ret >= 0)
        {
            bLog = true;    // ����Ѽ�¼��־
        }
    }

    return bLog;
}


void CLog::ErrLogMsg( EErrorMsg_Type eType, const TString& strMsg, const TString& ErrInfo )
{

    // ��־�ļ����
    const SOpenFile* pOpenFlil = GetLogFile( eErrLog );
    if ( pOpenFlil == NULL )
    {
        // bad, ��־�ļ����ʧ��
        return;
    }

    // ���
    static const int BUF_SIZE = 256;
    TCHAR szCharBuf[BUF_SIZE];

    _stprintf_s( szCharBuf, BUF_SIZE, _T("%s  Type: %d; Msg: %s")
                , ErrInfo.c_str(), eType, strMsg.c_str() );

    if ( WriteFile( pOpenFlil->pFile, szCharBuf ) == false )
    {
        // bad, ��־д��ʧ��
        return;
    }
}

TString CLog::GetTypeName( EErrorMsg_Type eType )
{
    switch ( eType )
    {
    case eDefault:
        {
            return _T("Default");
        }break;
    case eErrLog:
        {
            return _T("ErrLog");
        }break;
    default:
        {
            return _T("NoType");
        }break;
    }
}



//////////////////////////////////////////////////////////////////////////
unsigned __stdcall Thread_Log( LPVOID lpVoid )
{
    // ʹ���в�, ����ᾯ��
    void* pParm = lpVoid;
    if ( pParm )
    {
    }

    CLog* pLog = CLog::GetSingleton();

    while ( pLog->IsRunLog() )
    {
        pLog->PrintLogMsg();

        Sleep( 1 );
    }

    _endthreadex( NULL );	// �߳̽���

    return NULL;
}