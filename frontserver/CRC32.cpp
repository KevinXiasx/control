/**********************************************************************
*��Ȩ���� ��C��2008�� ׿Խ�����ְҵ��ѵѧУ
*�ļ����ƣ�crc32.cpp
*
*����ժҪ��
*    CRC����У��
*��ʷ��¼
*    ����ʱ�䣺2012-4-1
*���ڣ�2012-4-4    �޸��ˣ�
*������
**********************************************************************/
#include "CRC32.h"


/*********************************************************************
*	�������ƣ�	CalcCrc32
*	����������	���ֽ�CRC32У���㷨 ��������
*	����˵����	const unsigned char byte[����ĵ��ֽ�],
*				unsigned int *dwCrc32[�ϴε��ֽ�У����]
*	�� �� ֵ��  
*********************************************************************/
inline void CalcCRC32(const unsigned char byte,unsigned int *dwCrc32)
{
	*dwCrc32 = ((*dwCrc32)>> 8) ^ s_arrdwCrc32Table[(byte)^((*dwCrc32)&0x000000FF)];
}

/*********************************************************************
*	�������ƣ�	StringCrc32
*	����������	�ļ�����CRC32У��
*	����˵����	const unsigned char * buf[��������], 
*				unsigned int *dwCrc32[CRC32������],
*				int DataLen[�ļ�����У�鳤��]
*	�� �� ֵ��  
*********************************************************************/
int StringCRC32(const unsigned char * buf, unsigned int *dwCrc32, int DataLen)
{
	int i=0;

	*dwCrc32 = 0xFFFFFFFF;//��ʼ��CRCУ���� Ĭ��ֵ
  
    for(i = 0; i < DataLen; i++) 
	{
		CalcCRC32((unsigned char)*buf, dwCrc32);
		buf++;
	} 

	*dwCrc32 = ~ *dwCrc32;

	return 0;
}

/*********************************************************************
*	�������ƣ�	CheckCRC32
*	����������	�ȽϾ���������ݲ�����CRC�Ƿ�һ��
*	����˵����	const unsigned char * buf[��������], 
*				unsigned int *dwCrc32[CRC32������],
*				int DataLen[�ļ�����У�鳤��]
*	�� �� ֵ��  
*********************************************************************/
bool CheckCRC32(int oldCRC, const unsigned char* buf, int DataLen)
{
	unsigned int newCRC = 0;
	StringCRC32(buf, &newCRC, DataLen);			//������У����
	if (newCRC == (unsigned int)oldCRC)
	{
		return true;
	}
	else
	{
		return false;
	}
}