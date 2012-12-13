#ifdef _MSC_VER
#pragma once
#endif

#ifndef __TypeDef_H__
#define __TypeDef_H__

#include <vector>

// forward declaration

//////////////////////////////////////////////////////////////////////////
//  @file   TypeDef.h
//  @brief  �������ʹ���. ���������һЩ�͸�����Ŀ�޹صģ��ɱ�ͨ�õĴ���
//  @author ������
//  @date   2011.12.02
//////////////////////////////////////////////////////////////////////////

//namespace ToolCode_Type
//{

// string
#ifdef  UNICODE
typedef std::wstring TString;
#else
typedef std::string TString;
#endif // UNICODE

typedef std::vector<TString>            VEC_STR;
typedef VEC_STR::iterator               VEC_STR_ITR;


//}

#endif  // __TypeDef_H__