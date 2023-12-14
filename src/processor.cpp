#include "processor.hpp"

static std::unordered_map<std::string, ProcessorRegister::ProcessorCreateFunc> &GetProcessorMap() {
    static std::unordered_map<std::string, ProcessorRegister::ProcessorCreateFunc> processorMap;
    return processorMap;
}

std::shared_ptr<Processor> Processor::GetProcessor(const std::string &businessId) {
    if (businessId.empty()) {
        return nullptr;
    }
    auto iter = GetProcessorMap().find(businessId);
    if (iter == GetProcessorMap().end() || iter->second == nullptr) {
        return nullptr;
    }
    return iter->second();
}

ProcessorRegister::ProcessorRegister(const std::string &businessId, ProcessorCreateFunc createFunc) {
    if (businessId.empty() || createFunc == nullptr) {
        return;
    }

    GetProcessorMap().emplace(businessId, std::move(createFunc));
}
