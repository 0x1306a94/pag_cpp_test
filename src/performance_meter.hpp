

#ifndef performance_meter_hpp
#define performance_meter_hpp

#include <chrono>
#include <iostream>
#include <string>

class PerformanceMeter {
  public:
    explicit PerformanceMeter(const std::string &name, std::ostream &output = std::cerr);
    ~PerformanceMeter();
    void Reset(const std::string &name, bool print = true);
    void Finish(bool print = true);

  private:
    void Print() const;

  private:
    std::string m_name;
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    bool m_finish;
    std::ostream &m_output_stream;
};

#endif /* performance_meter_hpp */
