/**********************************************************************
*版权所有 （C）2008， 卓越计算机职业培训学校
*文件名称：crc32.cpp
*
*内容摘要：
*    CRC数据校验
*历史记录
*    创建时间：2012-4-1
*日期：2012-4-4    修改人：
*描述：
**********************************************************************/
#include "CRC32.h"


/*********************************************************************
*	函数名称：	CalcCrc32
*	函数描述：	单字节CRC32校验算法 内链函数
*	参数说明：	const unsigned char byte[传入的单字节],
*				unsigned int *dwCrc32[上次单字节校验结果]
*	返 回 值：  
*********************************************************************/
inline void CalcCRC32(const unsigned char byte,unsigned int *dwCrc32)
{
	*dwCrc32 = ((*dwCrc32)>> 8) ^ s_arrdwCrc32Table[(byte)^((*dwCrc32)&0x000000FF)];
}

/*********************************************************************
*	函数名称：	StringCrc32
*	函数描述：	文件数据CRC32校验
*	参数说明：	const unsigned char * buf[传入数据], 
*				unsigned int *dwCrc32[CRC32返回码],
*				int DataLen[文件数据校验长度]
*	返 回 值：  
*********************************************************************/
int StringCRC32(const unsigned char * buf, unsigned int *dwCrc32, int DataLen)
{
	int i=0;

	*dwCrc32 = 0xFFFFFFFF;//初始化CRC校验码 默认值
  
    for(i = 0; i < DataLen; i++) 
	{
		CalcCRC32((unsigned char)*buf, dwCrc32);
		buf++;
	} 

	*dwCrc32 = ~ *dwCrc32;

	return 0;
}

/*********************************************************************
*	函数名称：	CheckCRC32
*	函数描述：	比较旧码和新数据产生的CRC是否一致
*	参数说明：	const unsigned char * buf[传入数据], 
*				unsigned int *dwCrc32[CRC32返回码],
*				int DataLen[文件数据校验长度]
*	返 回 值：  
*********************************************************************/
bool CheckCRC32(int oldCRC, const unsigned char* buf, int DataLen)
{
	unsigned int newCRC = 0;
	StringCRC32(buf, &newCRC, DataLen);			//生成新校验码
	if (newCRC == (unsigned int)oldCRC)
	{
		return true;
	}
	else
	{
		return false;
	}
}