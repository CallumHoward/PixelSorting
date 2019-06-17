// PixelSorting.hpp
// Callum Howard, 2019

#ifndef PIXELSORTING_HPP
#define PIXELSORTING_HPP

#include <iso646.h>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

namespace ch {

using namespace ci;
using namespace ci::app;

class PixelSorting {

public:
    using Frame = std::vector<uint8_t>;

    void setup();
    void update();
    void draw();

private:
    void readData();

    gl::TextureRef mTexture;
    Surface32f mImage;
    Frame mBuffer;
};


void PixelSorting::setup() {
    try {
        fs::path path = getOpenFilePath("", ImageIo::getLoadExtensions());
        if (not path.empty()) {
            mImage = loadImage(path);

            auto pixelIterRead = mImage.getIter();
            auto pixelIterWrite = mImage.getIter();
            while (pixelIterRead.line() and pixelIterWrite.line()) {
                auto pixelRow = std::vector<Color>{};
                pixelRow.reserve(mImage.getWidth());

                while (pixelIterRead.pixel()) {
                    pixelRow.emplace_back(
                            pixelIterRead.r(),
                            pixelIterRead.g(),
                            pixelIterRead.b());
                }

                std::sort(std::begin(pixelRow), std::end(pixelRow),
                        [] (const Color& lhs, const Color& rhs) {
                            return lhs.r < rhs.r;
                        });

                for (int row = 0; row < mImage.getWidth() and pixelIterWrite.pixel(); ++row) {
                    pixelIterWrite.r() = pixelRow[row].r;
                    pixelIterWrite.g() = pixelRow[row].g;
                    pixelIterWrite.b() = pixelRow[row].b;
                }
            }

            mTexture = gl::Texture::create(mImage);
        }

    } catch (Exception &exc) {
        CI_LOG_EXCEPTION("failed to load image.", exc);
    }
}

void PixelSorting::update() {

}

void PixelSorting::draw() {
    if (mTexture) {
        Rectf destRect = Rectf(mTexture->getBounds())
                .getCenteredFit(getWindowBounds(), true)
                .scaledCentered(0.85f);

        gl::draw(mTexture, destRect);
    }
}


} // namespace ch

#endif
