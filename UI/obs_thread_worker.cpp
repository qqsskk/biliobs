#include "stdafx.h"
#include "obs_thread_worker.h"

extern LONG AppCrashHandleCallback(EXCEPTION_POINTERS *pE);

class PThreadMutexLocker
{
	PThreadMutexLocker(const PThreadMutexLocker&);
	PThreadMutexLocker& operator=(const PThreadMutexLocker&);

	pthread_mutex_t* mutex;
public:
	PThreadMutexLocker(pthread_mutex_t* m) : mutex(m)
	{
		pthread_mutex_lock(mutex);
	}

	void UnLock()
	{
		if (mutex != 0)
			pthread_mutex_unlock(mutex);
		mutex = 0;
	}

	~PThreadMutexLocker()
	{
		UnLock();
	}
};

ObsThreadWorker::ObsThreadWorker()
	: workerThread(pthread_t())
	, queueSemaphore(sem_t())
	, isThreadRunningCondVar(pthread_cond_t())
	, isThreadRunningMutex(pthread_mutex_t())
	, isThreadRunning(false)
{

}

void* ObsThreadWorker::WorkerProc(void* userData)
{
	static_cast<ObsThreadWorker*>(userData)->WorkerProcImpl();
	return 0;
}

void ObsThreadWorker::WorkerProcImpl()
{
	bool stopLoop = false;
	while (stopLoop == false)
	{
		sem_wait(&queueSemaphore);

		PThreadMutexLocker queueLocker(&taskQueueMutex);
		TaskT task = std::move(taskQueue.front());
		taskQueue.pop();
		queueLocker.UnLock();

		void* r = task();
		if (r == &stopSignal)
			stopLoop = true;
	}

	PThreadMutexLocker locker(&isThreadRunningMutex);
	isThreadRunning = false;
	pthread_cond_signal(&isThreadRunningCondVar);
	locker.UnLock();
}

ObsThreadWorker::~ObsThreadWorker()
{
	Stop();
}

bool ObsThreadWorker::Start()
{
	if (isThreadRunning)
		return true;

	//ÉèÖÃ±ÀÀ£
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)AppCrashHandleCallback);

	bool bRet = false;
	do 
	{
		if (sem_init(&queueSemaphore, 0, 0) != 0)
			break;
		freeResources.push_back(std::bind(&sem_destroy, &queueSemaphore));

		if (pthread_mutex_init(&isThreadRunningMutex, 0) != 0)
			break;
		freeResources.push_back(std::bind(&pthread_mutex_destroy, &isThreadRunningMutex));

		if (pthread_cond_init(&isThreadRunningCondVar, 0) != 0)
			break;
		freeResources.push_back(std::bind(&pthread_cond_destroy, &isThreadRunningCondVar));

		if (pthread_mutex_init(&taskQueueMutex, 0) != 0)
			break;
		freeResources.push_back(std::bind(&pthread_mutex_destroy, &taskQueueMutex));

		if (pthread_create(&workerThread, 0, &ObsThreadWorker::WorkerProc, this) != 0)
			break;
		
		PThreadMutexLocker locker(&isThreadRunningMutex);
		bRet = true;

	} while (0);
	
	if (!bRet)
	{
		for (auto& x : freeResources)
			x();
		freeResources.clear();
	}
	return bRet;
}

static void* stopSignalWrapper(void* s)
{
	return s;
}

bool ObsThreadWorker::Stop()
{
	if (isThreadRunning == false)
		return true;

	if (freeResources.size() == 0)
		return true;

	PThreadMutexLocker locker(&isThreadRunningMutex);
	if (isThreadRunning == false)
		return true;

	locker.UnLock();

	AddTask(std::bind(&stopSignalWrapper, &this->stopSignal));

	pthread_mutex_lock(&isThreadRunningMutex);
	while (isThreadRunning == true)
		pthread_cond_wait(&isThreadRunningCondVar, &isThreadRunningMutex);

	void* joinValue;
	pthread_join(workerThread, &joinValue);

	for (auto& x : freeResources)
		x();
	freeResources.clear();

	return true;
}

void ObsThreadWorker::AddTask(TaskT&& task)
{
	PThreadMutexLocker queueLocker(&taskQueueMutex);
	taskQueue.push(std::move(task));
	sem_post(&queueSemaphore);
}
