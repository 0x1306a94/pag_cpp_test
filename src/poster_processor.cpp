#include "poster_processor.hpp"

#include <pag/pag.h>

void PosterProcessor::BeforeCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition) {
    if (composition == nullptr) {
        return;
    }

    auto layers = composition->getLayersByName("price");
    if (layers.size() != 1) {
        return;
    }

    auto targetLayer = layers[0];
    if (targetLayer->layerType() != pag::LayerType::Text) {
        return;
    }
    auto font = pag::PAGFont::RegisterFont("./pag_samples/FiraCode-SemiBold.ttf", 0);

    auto priceLayer = std::static_pointer_cast<pag::PAGTextLayer>(targetLayer);
    priceLayer->setFont(font);
    priceLayer->setText("999999");

    auto symbolLayer = pag::PAGTextLayer::Make(targetLayer->duration(), "¥");
    symbolLayer->setFont(priceLayer->font());
    symbolLayer->setFontSize(priceLayer->fontSize() * .68);
    symbolLayer->setFillColor(priceLayer->fillColor());

    composition->addLayer(symbolLayer);

    this->symbolLayer = symbolLayer;
    this->priceLayer = priceLayer;
}

void PosterProcessor::AfterCreatingPlayer(std::shared_ptr<pag::PAGComposition> composition, std::shared_ptr<pag::PAGPlayer> player) {
    if (composition == nullptr || player == nullptr) {
        return;
    }

    if (this->symbolLayer == nullptr || this->priceLayer == nullptr) {
        return;
    }

    this->symbolLayer->resetMatrix();
    this->priceLayer->resetMatrix();

    // 获取最终的准确的图层位置
    player->flush();

    auto canvasRect = pag::Rect::MakeWH(composition->width(), composition->height());
    auto symbolRect = player->getBounds(symbolLayer);
    auto priceRect = player->getBounds(priceLayer);

    auto priceTx = canvasRect.centerX() - priceRect.centerX() + symbolRect.width() * 0.5;
    auto priceTy = canvasRect.centerY() - priceRect.centerY();
    auto symbolTx = (canvasRect.width() - priceRect.width()) * 0.5 - symbolRect.width() * 0.5;
    auto symbolTy = (canvasRect.centerY() + priceRect.height() * 0.5) - symbolRect.height() * 0.8;

    do {
        auto totalMatrix = this->symbolLayer->getTotalMatrix();
        totalMatrix.setTranslate(symbolTx, symbolTy);
        this->symbolLayer->setMatrix(totalMatrix);
    } while (0);

    do {
        auto totalMatrix = this->priceLayer->getTotalMatrix();
        totalMatrix.setTranslate(priceTx, priceTy);
        this->priceLayer->setMatrix(totalMatrix);
    } while (0);
}

static ProcessorRegister X("Poster", []() -> std::shared_ptr<Processor> {
    return std::make_shared<PosterProcessor>();
});
