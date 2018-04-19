#pragma once

#include "pthread.h"
#include "semaphore.h"

class ObsThreadWorker
{
public:
	typedef std::function<void*()> TaskT;

private:
	ObsThreadWorker(const ObsThreadWorker&);
	ObsThreadWorker& operator= (const ObsThreadWorker&);

	std::list<std::function<int()> > freeResources;
	pthread_t workerThread;
	sem_t queueSemaphore;

	volatile bool isThreadRunning;
	pthread_cond_t isThreadRunningCondVar;
	pthread_mutex_t isThreadRunningMutex;
	void* stopSignal; //this address returned from taskT means the thread should stop

	static void* WorkerProc(void* userData);
	void WorkerProcImpl();

	std::queue<TaskT> taskQueue;
	pthread_mutex_t taskQueueMutex;
public:
	ObsThreadWorker();
	~ObsThreadWorker();

	bool Start();
	bool Stop();

	void AddTask(TaskT&& task);
};