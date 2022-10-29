#ifndef CPU_LIMIT_H_
#define CPU_LIMIT_H_
#include <atomic>
#include <thread>
#include <vector>

class CpuLimiter {

public:
  void SetCpuLimit(unsigned char limit);
  unsigned char GetCpuLimit();
  unsigned char GetCpuUsage();

private:
  CpuLimiter();
  void UsageControlThread();
  unsigned char ComputeCpuUsage(char core_id);

  std::thread ctr_thread_;
  std::vector<std::thread> payload_thread_;
  std::atomic_char core_num_;
  std::vector<unsigned char> usage_list_;
};

#endif // CPU_LIMIT_H_