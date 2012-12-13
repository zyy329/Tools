#include "StdAfx.h"

// ≤‚ ‘”√

//#include "ToolCode/SafeSTL/SafeList.h"
//#include "ToolCode/SafeSTL/SafeMap.h"
//#include "ToolCode/SmartPTR/SmartPTR.h"
//#include "ToolCode/Log/Log.h"
//
//#pragma comment (lib, "winmm.lib")
//
//using namespace std;
//
//
//class TC
//{
//public:
//    TC( void ) { cout << "TC()" << endl; }
//    ~TC( void ) { cout << "~TC()" << endl; }
//
//    char a;
//};
//
//#define _STRING(x) #x
//
//#define STRING(x) _STRING(x)
//
//bool Func_Int( const int& first,  const int& Second, void* pParm1, void* pParm2 )
//{
//    int*   p1   = (int*)pParm1;
//    float* p2   = (float*)pParm2;
//    cout << "Func_Int: \n"
//         << first << ": " << Second 
//         << ", p1 = " << *p1
//         << ", p2 = " << *p2
//         << endl;
//
//    return true;
//}
//
//
//bool Func_Del( const int& value, void* pParm1, void* pParm2, bool& outIsDel )
//{
//    int*   p1   = (int*)pParm1;
//    float* p2   = (float*)pParm2;
//    cout << "Func_Del: \n"
//        << value
//        << ", p1 = " << *p1
//        << ", p2 = " << *p2
//        << endl;
//
//    if ( value % 2 == 0 )
//    {
//        outIsDel = true;
//    }
//    else
//    {
//        outIsDel = false;
//    }
//
//    if ( value >= 6 )
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}
//
//
//void main( void )
//{
//    ////DWORD dwTime = timeGetTime();
//
//    //// int
//    //SafeMap<int, int> TMap;
//    //int nx[10];
//    //for ( int i = 0; i < 10; ++i )
//    //{
//    //    nx[i] = i * 2;
//    //    TMap.Insert( i, nx[i], Use_DebugCode );
//    //}
//
//    //int nOut = 0;
//    //TMap.Find( 3, nOut );
//    //cout << 3 << ": " << nOut << endl;
//
//    //TMap.Delete( 4 );
//    //SafeMap<int, int>::MAP_TYPE mapTemp;
//    //TMap.CopyTemp( mapTemp );
//
//    //int np1 = 35;
//    //float fp2 = 0.35f;
//    //TMap.ForEach( Func_Int, &np1, &fp2 );
//
//    //TMap.Clear();
//
//    ////TMap.Insert( 1, 1 * 2, Use_DebugCode );
//
//
//
//    ////////////////////////////////////////////////////////////////////////////
//    //SafeList<int> TList;
//    //for ( int i = 0; i < 10; ++i )
//    //{
//    //    TList.push_back( i );
//    //}
//
//    //TList.Remove_If( Func_Del, &np1, &fp2 );
//
//
//    //TList.Clear();
//
//    ////cout << _T("Time: ") << timeGetTime() - dwTime << endl;
//
//    //////////////////////////////////////////////////////////////////////////
//    // »’÷æ≤‚ ‘
//    CLog::GetSingleton();
//    CLog::GetSingleton()->LogMsg( CLog::eDefault, _T("Test1") );
//    CLog::GetSingleton()->LogMsg( CLog::eDefault, _T("Test2") );
//    CLog::GetSingleton()->LogMsg( (CLog::EErrorMsg_Type)-1, _T("Test3") );
//    CLog::GetSingleton()->LogMsg( (CLog::EErrorMsg_Type)10, _T("Test3") );
//
//    system( "pause" );
//
//    CLog::destroySingleton();
//}