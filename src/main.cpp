
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <chrono>

#include <pag/pag.h>

#include "performance_meter.hpp"
#include "processor.hpp"

void BmpWrite(unsigned char *image, int imageWidth, int imageHeight,
              const char *filename) {
    unsigned char header[54] = {0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
                                40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                                32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int64_t file_size =
        static_cast<int64_t>(imageWidth) * static_cast<int64_t>(imageHeight) * 4 +
        54;
    header[2] = static_cast<unsigned char>(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    int64_t width = imageWidth;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    int64_t height = -imageHeight;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    std::stringstream ss;
    ss << filename << ".bmp";

    auto fname_bmp = ss.str();
    FILE *fp;
    if (!(fp = fopen(fname_bmp.c_str(), "wb"))) {
        return;
    }

    fwrite(header, sizeof(unsigned char), 54, fp);
    fwrite(image, sizeof(unsigned char),
           (size_t)(int64_t)imageWidth * imageHeight * 4, fp);
    fclose(fp);
}

std::int64_t TimeToFrame(int64_t time, float frameRate) {
    return static_cast<int64_t>(floor(time * frameRate / 1000000ll));
}

int main(int argc, char *const argv[]) {

    PerformanceMeter t0("PAGFile::Load");
    std::shared_ptr<pag::PAGFile> m_file = pag::PAGFile::Load("./pag_samples/test3.pag");
    t0.Finish();
    if (!m_file) {
        std::cerr << "无法加载pag文件" << std::endl;
        return EXIT_FAILURE;
    }

    auto processor = Processor::GetProcessor("Poster");
    if (processor) {
        PerformanceMeter t1("Processor BeforeCreatingPlayer");
        processor->BeforeCreatingPlayer(m_file);
    }

    auto totalFrames = TimeToFrame(m_file->duration(), m_file->frameRate());

    PerformanceMeter t2("Processor BeforeCreatingPlayer");
    std::shared_ptr<pag::PAGSurface> m_surface = pag::PAGSurface::MakeOffscreen(m_file->width(), m_file->height());
    std::shared_ptr<pag::PAGPlayer> m_player = std::make_shared<pag::PAGPlayer>();
    m_player->setSurface(m_surface);
    m_player->setComposition(m_file);
    t2.Finish();

    if (processor) {
        PerformanceMeter t3("Processor AfterCreatingPlayer");
        processor->AfterCreatingPlayer(m_player->getComposition(), m_player);
    }

    PerformanceMeter t4("Renderer");
    int frame_index = 1;
    std::size_t bytesLength = m_surface->width() * m_surface->height() * 4;
    std::size_t dstRowBytes = m_surface->width() * 4;
    m_player->setProgress(frame_index * 1.0 / totalFrames);
    if (!m_player->flush()) {
        std::cerr << "flush 失败" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<uint8_t> pixels(bytesLength);
    if (!m_surface->readPixels(pag::ColorType::BGRA_8888,
                               pag::AlphaType::Premultiplied, pixels.data(),
                               dstRowBytes)) {
        std::cerr << "readPixels 失败" << std::endl;
        return EXIT_FAILURE;
    }
    t4.Finish();

    BmpWrite(pixels.data(), m_surface->width(), m_surface->height(), "test");

    return EXIT_SUCCESS;
}
