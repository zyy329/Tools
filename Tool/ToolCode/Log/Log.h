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
//  @brief  日志类
//  @author 张逸云
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

        eErrLog,        // 日志类内部使用，日志记录失败时的统一输出位置

        eERROR_MSG_TYPE_NUMBERS
    };

    typedef struct S_LogMsg
    {
        EErrorMsg_Type  eType;
        TString         strMsg;
    };
    typedef SafeList<S_LogMsg*> SMAP_LOG;

    typedef struct _SOpenFile       // 日志输出文件对象，避免不停的打开关闭文件
    {
        _SOpenFile( void ) : pFile(NULL) {}
        FILE*   pFile;          // 打开的文件指针
        CTime   cTime;          // 所对应的日期, 用于根据日期划分日志记录文件
    }SOpenFile;
    typedef std::map<EErrorMsg_Type, SOpenFile>     MapLogFile;
    typedef MapLogFile::iterator                    MapLogFileItr;

private:
    // 静态变量
    static const TString    ms_strPoolName; // 日志内存池名
    static CPool<S_LogMsg>  ms_LogPool;     // 日志内存池

    // 线程
    HANDLE      m_hLog_Thread_Handle;       // 日志线程句柄
    unsigned    m_nLog_Thread_Id;           // 日志线程ID
    bool        m_bRunLog;

    // 容器
    SMAP_LOG    m_lstLogMsg;                // 待输出消息 临时存储容器
    MapLogFile  m_mapLogFile;               // 常规日志容器，一种类型对应一个日志文件

    // 状态
    bool        m_bLogSwitch;               // 日志开关

    // 其他
    TString     m_strLogBasePath;           // 日志基础输出路径，所有日志都将统一输出到该路径下 或 其子文件下


public:
    /** @brief  日志输入接口
    *   @param  eType, 输入的日志类型,将决定日志被输出到的文件,及一些特性的信息
    */
    void LogMsg( EErrorMsg_Type eType, const TCHAR * pszParam,... );


    inline void LogSwitch( bool bOn )           { m_bLogSwitch = bOn; }

    inline bool IsRunLog( void )        const   { return m_bRunLog; }


private:
    /** @brief  创建日志文件目录结构，确保日志文件能够正常创建 */
    void CreateLogDirectory( void );

    void PrintLogMsg( void );
    void LogToFile( const S_LogMsg& logMsg );
    /** @brief  日志信息格式化, 统一添加一些格式化的信息(如日期) */
    TString FormMsg( const TString& strMsg );
    const SOpenFile* GetLogFile( EErrorMsg_Type eType );
    void GetLogFilePath( EErrorMsg_Type eType, TString& outFileName );
    bool WriteFile( FILE* pFile, const TString& strMsg );
    /** @brief  日志记录失败时的统一输出位置 */
    void ErrLogMsg( EErrorMsg_Type eType, const TString& strMsg, const TString& ErrInfo );

    TString GetTypeName( EErrorMsg_Type eType );

    friend unsigned __stdcall Thread_Log(LPVOID lpVoid);
};

#endif  // __Log_H__