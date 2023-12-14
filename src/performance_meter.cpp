
#include "performance_meter.hpp"

#include <iostream>

PerformanceMeter::PerformanceMeter(const std::string &name, std::ostream &output)
    : m_name(name)
    , m_start(std::chrono::steady_clock::now())
    , m_finish(false)
    , m_output_stream(output) {
}

PerformanceMeter::~PerformanceMeter() {
    Print();
}

void PerformanceMeter::Reset(const std::string &name, bool print) {
    if (print) {
        Print();
    }
    m_name = name;
    m_start = std::chrono::steady_clock::now();
}

void PerformanceMeter::Finish(bool print) {
    if (print) {
        Print();
    }
    m_finish = true;
}

void PerformanceMeter::Print() const {
    if (m_finish) {
        return;
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start);
    m_output_stream << m_name << ": " << elapsed.count() << "ms" << std::endl;
}
