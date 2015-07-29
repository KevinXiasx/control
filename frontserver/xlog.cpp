#include "xlog.h"


void Xlog::addLog(const PacketClass* form, enumlog l)
{
	ofstream fout;
	time_t t= time(NULL);
	struct tm *mytime = localtime(&t);
	char filename[20];
	if(l == ERR)
	{
		sprintf(filename,"logerr%d%d%d",mytime->tm_year+1900,mytime->tm_mon+1,mytime->tm_mday);
		fout.open(filename,ios::out|ios::app);
		if( !fout.is_open() )
		{
			fout.open(filename,ios::out);
			if( !fout.is_open() )
				cout<<"file log is open fail"<<endl;
		}
		fout<<"时间："<<filename<<" "<<mytime->tm_hour<<":"<<mytime->tm_min<<":"<<mytime->tm_sec<<endl;
		fout<<"错误类型："<<form->getErro()<<endl;
		fout<<"发送数据（十六进制）";
		int size = sizeof(st_packet);
		unsigned char *ptr = (unsigned char*)(form->getPackPr());
		for (int i = 0; i < size; ++i)
		{
			if(i%16 == 0)
				fout<<endl;
			fout.width(3);
			fout<<hex<<(int)(*(ptr+i))<<" ";
		}
	}
	else if(l == SEND)
	{
		sprintf(filename,"logsend%d%d%d",mytime->tm_year+1900,mytime->tm_mon+1,mytime->tm_mday);
		fout.open(filename,ios::out|ios::app);
		if( !fout.is_open() )
		{
			fout.open(filename,ios::out);
			if( !fout.is_open() )
				cout<<"file log is open fail"<<endl;
		}
		fout<<"时间："<<filename<<" "<<mytime->tm_hour<<":"<<mytime->tm_min<<":"<<mytime->tm_sec<<endl;
		fout<<"类型："<<form->getType()<<endl;
		fout<<"用户ID: "<<form->getId()<<endl;
		fout<<"交易流水： "<<form->getTrans()<<endl;
		fout<<"发送数据（十六进制）";
		int size = form->getPacketSize();
		unsigned char *ptr = (unsigned char*)(form->getPackPr());
		for (int i = 0; i < size; ++i)
		{
			if(i%16 == 0)
				fout<<endl;
			fout.width(3);
			fout<<hex<<(int)(*(ptr+i))<<" ";
		}
	}
	else if(l == RES)
	{
		sprintf(filename,"logres%d%d%d",mytime->tm_year+1900,mytime->tm_mon+1,mytime->tm_mday);
		fout.open(filename,ios::out|ios::app);
		if( !fout.is_open() )
		{
			fout.open(filename,ios::out);
			if( !fout.is_open() )
				cout<<"file log is open fail"<<endl;
		}
		fout<<"时间："<<filename<<" "<<mytime->tm_hour<<":"<<mytime->tm_min<<":"<<mytime->tm_sec<<endl;
		fout<<"类型："<<form->getType()<<endl;
		fout<<"用户ID: "<<form->getId()<<endl;
		fout<<"交易流水： "<<form->getTrans()<<endl;
		fout<<"发送数据（十六进制）";
		for (int i = 0; i < form->getPacketNumb(); ++i)
		{
			unsigned char *ptr = (unsigned char*)(form->getPackPr(i));
			int size = form->getPacketSize(i);
			for (int j = 0; j < size; ++j)
			{
				if(j%16 == 0)
					fout<<endl;
				fout.width(3);
				fout<<hex<<(int)(*(ptr+j))<<" ";
			}
		}
	}
	fout<<"\n\n";
	fout.close();
}