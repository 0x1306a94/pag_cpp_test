#ifndef poster_processor_hpp
#define poster_processor_hpp

#include "processor.hpp"

class PosterProcessor : public Processor {
  public:
    virtual void BeforeCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition) override;
    virtual void AfterCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition, std::shared_ptr<pag::PAGPlayer> player) override;
    
private:
    std::shared_ptr<pag::PAGTextLayer> priceLayer{nullptr};
    std::shared_ptr<pag::PAGTextLayer> symbolLayer{nullptr};
};

#endif /* poster_processor_hpp */