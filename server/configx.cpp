#include "configx.h"

Configx::Configx()
{
	//标准配置项目
	stdconfig.push_back("stocport");
	stdconfig.push_back("ctosport");
	pthread_rwlock_init(&configLock,NULL);

	string filename = "socket.config";
	int res = open(filename.c_str(),O_RDWR|O_CREAT|O_EXCL,0644);
	if(res > 0) //不存在配置文件，则进行输入创建
	{
		for(int i = 0;i<stdconfig.size();i++)
		{
			cout<<"please input "<<stdconfig[i]<<": ";
			string temp;
			cin>>temp;
			myConfig.insert(pair<string,string>(stdconfig[i],temp));
		}
		std::map<string, string>::iterator pr;
		for(pr = myConfig.begin();pr != myConfig.end(); pr++)
		{
            string temp =pr->first+"="+pr->second+"\n";
            write(res,temp.c_str(),temp.size());
		}
        close(res);
	}
	else
	{
        if( errno == EEXIST)//已存在文件，进行读取
		{
            ifstream myfile;
            myfile.open(filename.c_str());
            std::list<string> v;
            std::list<string>::iterator pr;
            string tempstr;
            while(getline(myfile,tempstr))
                v.push_back(tempstr);
			for (pr = v.begin(); pr != v.end(); pr++)
			{	
                if((*pr)[0] == '#'||(*pr).empty())
					continue;
				string name = (*pr).substr(0,(*pr).find_first_of("="));
				(*pr).erase(0,(*pr).find_first_of("=")+1);
				string value = (*pr).substr(0);
				myConfig.insert(pair<string,string>(name,value));
			}
            myfile.close();
            //此处开始校验读取的配置数据是否完整
            if( myConfig.size() != stdconfig.size() )
            {
            	ofstream fout(filename.c_str(),ios::out);
            	if(!fout.is_open())//错误打开
            	{
            		cout<<filename<<" is write fail"<<endl;
            		exit(1);
            	}
            	std::vector<string> temp;
            	temp =stdconfig;
            	std::map<string, string>::iterator pr;
				for(pr = myConfig.begin();pr != myConfig.end(); pr++)
		            temp.erase( find(temp.begin(),temp.end(),pr->first) );
		        for(int i = 0;i<temp.size();i++)
				{
					cout<<"please input "<<temp[i]<<": ";
					string tempstr;
					cin>>tempstr;
					myConfig.insert(pair<string,string>(temp[i],tempstr));
				}
				std::map<string, string>::iterator prt;
				for(prt = myConfig.begin();prt != myConfig.end(); prt++)
				{
		            fout<<prt->first<<"="<<prt->second<<endl;
				}
		        fout.close();
		    }
		}
		else
		{
			perror("socket.config cread fail");
			exit(0);
		}
	}
}
Configx::~Configx()
{
	pthread_rwlock_destroy(&configLock);
}


string Configx::getconfig(string name)
{
	pthread_rwlock_rdlock(&configLock);
	string con = myConfig.find(name)->second;
	pthread_rwlock_unlock(&configLock);
	return con;
}

int Configx::getconfint(string name)
{
	pthread_rwlock_rdlock(&configLock);
	string con = myConfig.find(name)->second;
	pthread_rwlock_unlock(&configLock);
	return atoi(con.c_str());
}


bool Configx::admen(string name,string value)
{
	bool ret;
	std::map<string, string>::iterator pr;
	pthread_rwlock_wrlock(&configLock);
	pr = myConfig.find(name);
	if( pr == myConfig.end() )
		ret = false;
	else
	{
		pr->second = value;
		ret = true;
	}
	pthread_rwlock_unlock(&configLock);
	return ret;
}