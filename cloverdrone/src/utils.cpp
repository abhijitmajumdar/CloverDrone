#include "utils.h"
#include <sched.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <stdexcept>

#if defined(USE_THREAD_SLEEP)
  #include <thread>
  #include <chrono>
#elif defined(USE_USLEEP)
  #include <unistd.h>
#endif

/*
 * Used to move thread to selected processor core
 * https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/
 */
void Utils::move_to_core(int core_id)
{
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores){
      std::cerr << "Check core ID to move to\n";
    }
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);
   pthread_t current_thread = pthread_self();
   int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
   if (rc != 0) {
      std::cerr << "Error setting affinity of thread to core: " << rc << "\n";
    }
}

/*
* Define custom function to handle signals like CTRL+C
*/
void Utils::ctrl_c_exit(void (*f)(int))
{
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = f;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
}

/*
Wrapper around sleep functions such that it can be changed later
****milli seconds
*/
void Utils::sleep(unsigned long t)
{
#if defined(USE_THREAD_SLEEP)
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
#elif defined(USE_USLEEP)
  usleep(t*1000);
#endif
}

/*
Min-Max scaling
*/
float Utils::scale(float* v, float* v_min, float* v_max, float* c_min, float* c_max)
{
  return (*v) * (*c_max-*c_min)/(*v_max-*v_min);
}

/*
Return the package path configured as an environment variable
*/
std::string Utils::get_configfile_path()
{
  if(const char* env_p = std::getenv("CLOVERDRONEDIR")) return std::string(env_p);
  else Utils::throw_error("Please export CLOVERDRONEDIR environment variable");
}

/*
Error reporting
*/
void Utils::throw_error(std::string error)
{
  throw std::runtime_error(error);
}
