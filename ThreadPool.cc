#include "ThreadPool.hh"
#include <stdlib.h>
#include <signal.h>


ThreadPool::ThreadPool(const unsigned int& p_maxThreads, const string& p_id):m_topIndex(0),
                                                                       m_bottomIndex(0),
                                                                       m_incompleteWork(0),
                                                                       m_poolId(p_id)                            
{
  pthread_mutex_init(&m_mutexSync, NULL);
  pthread_mutex_init(&m_mutexWorkCompletion, NULL);

  if (p_maxThreads < 1)  
  {
    m_maxThreads = 1;
  } else
  {
    m_maxThreads = p_maxThreads;
  }

  m_queueSize = m_maxThreads;
  
  m_workerQueue.resize(m_maxThreads, NULL);

  sem_init(&m_availableWork, 0, 0);
  sem_init(&m_availableThreads, 0, m_queueSize);
  
}

void 
ThreadPool::initializeThreads(void)
{
  for(unsigned int c_thread = 0; c_thread < m_maxThreads; ++c_thread)
  {
    pthread_attr_t l_attr;
    pthread_t l_id;
    pthread_attr_init(&l_attr);
    pthread_attr_setdetachstate(&l_attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&l_id, NULL, &ThreadPool::threadExecute, (void *) this );
    m_threads.push_back(l_id);
  }
}

ThreadPool::~ThreadPool()
{
  for(unsigned int c_work = 0; c_work < m_workerQueue.size(); ++c_work)
  {
    delete m_workerQueue[c_work];
  }
  
  sem_destroy(&m_availableWork);
  sem_destroy(&m_availableThreads);
  pthread_mutex_destroy(&m_mutexSync);
  pthread_mutex_destroy(&m_mutexWorkCompletion);
}



void 
ThreadPool::destroyPool(const unsigned int& p_maxPollSecs)
{
  while(m_incompleteWork > 0 )
  {
     sleep(p_maxPollSecs);
  }

  for(unsigned int c = 0; c < m_threads.size(); c++)
  {
    pthread_kill(m_threads[c], SIGUSR1);
  }

  void* l_ret = NULL;

  for(unsigned int c = 0; c < m_threads.size(); c++)
  {
    pthread_join(m_threads[c], &l_ret);
  }
}


int 
ThreadPool::assignWork (WorkerThread *p_workerThread)
{
  pthread_mutex_lock(&m_mutexWorkCompletion);
  ++m_incompleteWork;
  pthread_mutex_unlock(&m_mutexWorkCompletion);
  
  sem_wait(&m_availableThreads);
    
  pthread_mutex_lock(&m_mutexSync);
  m_workerQueue[m_topIndex] = p_workerThread;

  if(m_queueSize != 1)
  {
    m_topIndex =  (m_topIndex + 1) % m_queueSize;
  }
  
  sem_post(&m_availableWork);
  
  pthread_mutex_unlock(&m_mutexSync);
  
  return 0;
}

bool 
ThreadPool::fetchWork(WorkerThread** p_worker)
{ 
  sem_wait(&m_availableWork);
  
  pthread_mutex_lock(&m_mutexSync);
  
  *p_worker = m_workerQueue[m_bottomIndex];
  
  m_workerQueue[m_bottomIndex] = NULL;
  
  if(m_queueSize != 1)
  {
    m_bottomIndex = (m_bottomIndex + 1) % m_queueSize;
  }
  
  sem_post(&m_availableThreads);
  
  pthread_mutex_unlock(&m_mutexSync);
  
  return true;
}

void
ThreadPool::threadTerminationHandler (int /*p_signum*/)
{
  // Signal SIGUSR1 received, terminating the thread
  pthread_exit(NULL);  
}

void* 
ThreadPool::threadExecute(void* p_arg)
{
  // Associating signal handler to the SIGUSR1 signal
  struct sigaction l_action;
  memset(&l_action, 0, sizeof(l_action));
  l_action.sa_handler = threadTerminationHandler;
  sigemptyset(&l_action.sa_mask );
  l_action.sa_flags = 0;
  
  sigaction(SIGUSR1, &l_action, NULL );

  // Extracting a new work object if any and running it
  WorkerThread* worker = NULL;
  ThreadPool* l_pool = (ThreadPool*)p_arg;
  
  while(l_pool->fetchWork(&worker))
  {
    if(worker != NULL)
    {
      worker->executeThis();
      delete worker;
      pthread_mutex_lock(&(l_pool->m_mutexWorkCompletion));
      l_pool->m_incompleteWork--;
      pthread_mutex_unlock(&(l_pool->m_mutexWorkCompletion));
    }
  }

  return 0;
}
