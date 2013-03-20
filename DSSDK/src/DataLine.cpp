
#include "DataLine.h"
//#include "DataTable.h"
//#include "LogSystem.h"

//CDataLine::CDataLine(CDataTable* pTable):
//	m_pTable(pTable)
//{
//	m_vCells.reserve(m_pTable->GetColNum());
//}
//
//CDataLine::~CDataLine()
//{
//}
//
//VOID CDataLine::AddInt(INT intValue)
//{
//	m_vCells.push_back(CDataCell(intValue));
//}
//
//VOID CDataLine::AddFloat(FLOAT floatValue)
//{
//	m_vCells.push_back(CDataCell(floatValue));
//}
//
//VOID CDataLine::AddString(const CHAR* szStrValue)
//{
//	m_vCells.push_back(CDataCell(szStrValue));
//}
//
//INT	CDataLine::GetInt(INT nColId) const
//{
//	if (m_pTable->GetColType(nColId) != TABLE_DATA_TYPE_INT && m_pTable->GetColType(nColId) != TABLE_DATA_TYPE_SUB_TABLE)
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetInt faild! Wrong type. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return 0;
//	}
//	if( nColId < 0 || nColId >= (INT)m_vCells.size())
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetInt faild! Wrong nColId. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return 0;
//	}
//	return m_vCells.at(nColId).m_uValue.m_nValue;
//}
//
//FLOAT	CDataLine::GetFloat(INT nColId) const
//{
//	if (m_pTable->GetColType(nColId) != TABLE_DATA_TYPE_FLOAT)
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetFloat faild! Wrong type. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return 0.0f;
//	}
//	if( nColId < 0 || nColId >= (INT)m_vCells.size())
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetFloat faild! Wrong nColId. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return 0.0f;
//	}
//	return m_vCells.at(nColId).m_uValue.m_fValue;
//}
//
//const CHAR*	CDataLine::GetString(INT nColId) const
//{
//	if (m_pTable->GetColType(nColId) != TABLE_DATA_TYPE_STRING)
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetString faild! Wrong type. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return NULL;
//	}
//	if( nColId < 0 || nColId >= (INT)m_vCells.size())
//	{
//		DiskLog_ERROR(LOG_ASSERT, "DataLine GetString faild! Wrong nColId. line id: %d. col id: %d .file name %s", GetId(), nColId, m_pTable->GetFileName());
//		return NULL;
//	}
//	return m_vCells.at(nColId).m_uValue.m_pStrValue;
//}
