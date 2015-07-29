#ifndef SHAREMEM_H_
#define SHAREMEM_H_ 
#include "packet_x.h"

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

typedef struct shmhead
{
	int readpoint;
	int writepoint;
	int packetsize;
	int packetsum;
}st_shmhead;

template <class item>
class ShareMem
{
public:
	ShareMem();
	void shareinit(int shmkey,int semkey);
	~ShareMem();

	void shmheadinit();
	void shmwrite(const item* datefrom,int i = 1) ;
	void shmread(item* date,int i = 1) ;

private:
	void sem_p(int arr,int i = 1);
	void sem_v(int arr,int i = 1 );
	void *shmPr;

	st_shmhead *myHead;
	item* mydata;

	int shmid;
	int semid;

	int shmblocknumb;
	int shmblocksize;
	pthread_mutex_t mutex;//
};


template <class item>
ShareMem<item>::ShareMem()
{
	pthread_mutex_init(&mutex,NULL);
}


template <class item>
void ShareMem<item>::shareinit(int shmkey,int blocknumb)
{
	shmblocknumb = blocknumb;
	shmblocksize = sizeof(item);
	shmid = shmget(shmkey,shmblocknumb*shmblocksize+sizeof(st_shmhead),IPC_CREAT|IPC_EXCL|0644);
	if(shmid > 0)
	{
		shmPr = shmat(shmid,0,0);
		myHead = (st_shmhead*)shmPr;
		mydata = (item*)((st_shmhead*)shmPr+1);
		shmheadinit();
	}
	else
	{
		if(errno == EEXIST)
		{
			shmid = shmget(shmkey,0,0);
			shmPr = shmat(shmid,0,0);
			myHead = (st_shmhead*)shmPr;
			mydata = (item*)((st_shmhead*)shmPr+1);
		}
		else
		{
			perror("sharemem get fail");
			exit(0);
		}
	}


	semid = semget(shmkey,2,IPC_CREAT|IPC_EXCL|0644);
	if(semid > 0)
	{
		union semun semumcel;
		semumcel.val = 0;
		semctl(semid,0,SETVAL,semumcel);
		semumcel.val = shmblocknumb;
		semctl(semid,1,SETVAL,semumcel);
	}
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
			semid = semget(shmkey,0,0);
		}
		else
		{
			perror("sem get fail");
			exit(0);
		}
	}

}

template <class item>
ShareMem<item>::~ShareMem()
{
	pthread_mutex_destroy(&mutex);
	shmdt(shmPr);
}

template <class item>
void ShareMem<item>::shmheadinit()
{
	st_shmhead head;
	head.readpoint = 0;
	head.writepoint = 0;
	head.packetsize = shmblocksize;
	head.packetsum = shmblocknumb;
	memcpy(myHead,&head,sizeof(st_shmhead));
}


template <class item>
void ShareMem<item>::shmwrite(const item* datefrom,int i) 
{
	pthread_mutex_lock(&mutex);
	sem_p(1,i);
	for (int j = 0; j < i; ++j)
	{
		memcpy(mydata+myHead->writepoint,datefrom+j,shmblocksize);
		if(myHead->writepoint == shmblocknumb-1)
			(myHead->writepoint) = 0;
		else
			(myHead->writepoint)++;
	}
	sem_v(0,i);
	pthread_mutex_unlock(&mutex);
}

template <class item>
void ShareMem<item>::shmread(item* date,int i) 
{
	pthread_mutex_lock(&mutex);
	sem_p(0,i);
	for (int j = 0; j < i; ++j)
	{
		memcpy(date+j,mydata+myHead->readpoint,shmblocksize);
		if(myHead->readpoint == shmblocknumb-1)
			(myHead->readpoint) = 0;
		else
			(myHead->readpoint)++;
	}
	sem_v(1,i);
	pthread_mutex_unlock(&mutex);
}
template <class item>
void ShareMem<item>::sem_p(int arr,int i)
{
	struct sembuf sem = {arr,-i,0};
	semop(semid,&sem,1);
}

template <class item>
void ShareMem<item>::sem_v(int arr,int i)
{
	struct sembuf sem = {arr,i,0};
	semop(semid,&sem,1);
}

#endif	