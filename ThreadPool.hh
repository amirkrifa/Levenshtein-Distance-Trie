#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <boost/noncopyable.hpp>


using namespace std;

const unsigned int DEFAULT_THREAD_POOL_DESTROY_POLL_SECONDS = 2;

//!WorkerThread class to be subclassed
class WorkerThread: private boost::noncopyable
{
public:
  WorkerThread ()
  {}
  virtual ~WorkerThread ()
  {}
  virtual unsigned int executeThis (void) = 0;
};


//!ThreadPool class manages all the ThreadPool related activities. 
//! This includes keeping track of idle threads and ynchronizations between all threads.
class ThreadPool: private boost::noncopyable
{
public:
  //! Constructor 
  //! @param p_maxThreads: (in) the maximum supported number of threads
  ThreadPool (const unsigned int& p_maxThreads, const string& p_id);

  //! Destructor
  virtual ~ThreadPool();
  
  //! Waits for the threads until they finish all the pending work then, destroys the pool
  //! @param p_maxPollSecs: the number of seconds to wait before re checking whether all the pending work ended or not
  void destroyPool (const unsigned int& p_maxPollSecs);

  //! Assigns a new work to the thread pool
  //! @param p_worker: (inà pointer to the new worker thread class
  //! @return 0 on success, error code otherwise
  int assignWork (WorkerThread* p_worker);

  bool fetchWork (WorkerThread** p_worker);

  //! Initializes the thread pool
  void initializeThreads (void);

  //! The run method which will be executed by all the threads when initialized
  static void *threadExecute (void* p_arg);
    
  static void threadTerminationHandler (int p_signum);

  pthread_mutex_t m_mutexSync;
  pthread_mutex_t m_mutexWorkCompletion;
  
  

private:

  unsigned int m_maxThreads; // max number of threads supported within the pool
  sem_t m_availableWork; // Semaphore used to control the number of available works
  sem_t m_availableThreads; // Semaphore used to control the number of available threads
  vector<WorkerThread*> m_workerQueue; // the works queue
  unsigned int m_topIndex; // Work queue topIndex
  unsigned int m_bottomIndex; // Work queue bottom index
  unsigned int m_incompleteWork; // the number of still incomplete threads
  unsigned int m_queueSize; // the works queue size
  string m_poolId; // Pool id
  vector<pthread_t> m_threads;

};

#endif  // THREAD_POOL_H_


