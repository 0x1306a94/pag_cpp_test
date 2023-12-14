#ifndef processor_hpp
#define processor_hpp

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace pag {
class PAGTextLayer;
class PAGPlayer;
class PAGComposition;
};  // namespace pag

class Processor;
class ProcessorRegister {
  public:
    using ProcessorCreateFunc = std::function<std::shared_ptr<Processor>(void)>;
    explicit ProcessorRegister(const std::string &businessId, ProcessorCreateFunc createFunc);
};

class Processor {
  public:
    virtual ~Processor() = default;
    virtual void BeforeCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition) = 0;
    virtual void AfterCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition, std::shared_ptr<pag::PAGPlayer> player) = 0;
    static std::shared_ptr<Processor> GetProcessor(const std::string &businessId);
};

#endif /* processor_hpp */