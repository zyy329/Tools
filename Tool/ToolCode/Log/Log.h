#ifdef _MSC_VER
#pragma once
#endif

#ifndef __Log_H__
#define __Log_H__

#include "Define/Macro.h"
#include "Define/TypeDef.h"
#include "Pool/Pool.h"
#include "SafeSTL/SafeList.h"

//////////////////////////////////////////////////////////////////////////
//  @file   Log.h
//  @brief  ��־��
//  @author ������
//  @date   2012.07.19
//////////////////////////////////////////////////////////////////////////

class CLog
{
    DECLARE_SINGLETON( CLog )

public:
    enum EErrorMsg_Type
    {
        eERROR_MSG_TYPE_START,
        eDefault,

        eErrLog,        // ��־���ڲ�ʹ�ã���־��¼ʧ��ʱ��ͳһ���λ��

        eERROR_MSG_TYPE_NUMBERS
    };

    typedef struct S_LogMsg
    {
        EErrorMsg_Type  eType;
        TString         strMsg;
    };
    typedef SafeList<S_LogMsg*> SMAP_LOG;

    typedef struct _SOpenFile       // ��־����ļ����󣬱��ⲻͣ�Ĵ򿪹ر��ļ�
    {
        _SOpenFile( void ) : pFile(NULL) {}
        FILE*   pFile;          // �򿪵��ļ�ָ��
        CTime   cTime;          // ����Ӧ������, ���ڸ������ڻ�����־��¼�ļ�
    }SOpenFile;
    typedef std::map<EErrorMsg_Type, SOpenFile>     MapLogFile;
    typedef MapLogFile::iterator                    MapLogFileItr;

private:
    // ��̬����
    static const TString    ms_strPoolName; // ��־�ڴ����
    static CPool<S_LogMsg>  ms_LogPool;     // ��־�ڴ��

    // �߳�
    HANDLE      m_hLog_Thread_Handle;       // ��־�߳̾��
    unsigned    m_nLog_Thread_Id;           // ��־�߳�ID
    bool        m_bRunLog;

    // ����
    SMAP_LOG    m_lstLogMsg;                // �������Ϣ ��ʱ�洢����
    MapLogFile  m_mapLogFile;               // ������־������һ�����Ͷ�Ӧһ����־�ļ�

    // ״̬
    bool        m_bLogSwitch;               // ��־����

    // ����
    TString     m_strLogBasePath;           // ��־�������·����������־����ͳһ�������·���� �� �����ļ���


public:
    /** @brief  ��־����ӿ�
    *   @param  eType, �������־����,��������־����������ļ�,��һЩ���Ե���Ϣ
    */
    void LogMsg( EErrorMsg_Type eType, const TCHAR * pszParam,... );


    inline void LogSwitch( bool bOn )           { m_bLogSwitch = bOn; }

    inline bool IsRunLog( void )        const   { return m_bRunLog; }


private:
    /** @brief  ������־�ļ�Ŀ¼�ṹ��ȷ����־�ļ��ܹ��������� */
    void CreateLogDirectory( void );

    void PrintLogMsg( void );
    void LogToFile( const S_LogMsg& logMsg );
    /** @brief  ��־��Ϣ��ʽ��, ͳһ���һЩ��ʽ������Ϣ(������) */
    TString FormMsg( const TString& strMsg );
    const SOpenFile* GetLogFile( EErrorMsg_Type eType );
    void GetLogFilePath( EErrorMsg_Type eType, TString& outFileName );
    bool WriteFile( FILE* pFile, const TString& strMsg );
    /** @brief  ��־��¼ʧ��ʱ��ͳһ���λ�� */
    void ErrLogMsg( EErrorMsg_Type eType, const TString& strMsg, const TString& ErrInfo );

    TString GetTypeName( EErrorMsg_Type eType );

    friend unsigned __stdcall Thread_Log(LPVOID lpVoid);
};

#endif  // __Log_H__