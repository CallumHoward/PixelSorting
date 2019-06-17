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

    int mWidth = 1000;
    int mHeight = 1000;
    gl::TextureRef mTexture;
    Surface32f mImage;
    Frame mBuffer;
};


void PixelSorting::setup() {
    try {
        fs::path path = getOpenFilePath("", ImageIo::getLoadExtensions());
        if (not path.empty()) {
            mImage = loadImage(path);

            for (int row = 0; row < mImage.getHeight(); ++row) {
                auto pixelRow = std::vector<Color>{};
                for (int col = 0; col < mImage.getWidth(); ++col) {
                    pixelRow.push_back(mImage.getPixel(ivec2{row, col}));
                }

                std::sort(std::begin(pixelRow), std::end(pixelRow),
                        [] (const Color& lhs, const Color& rhs) {
                            return lhs.r < rhs.r;
                        });

                for (int col = 0; col < mImage.getWidth(); ++col) {
                    mImage.setPixel(ivec2{row, col}, pixelRow[col]);
                }

                CI_LOG_I("processed row " << row);
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
