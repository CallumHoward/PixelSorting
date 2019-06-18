// PixelSorting.hpp
// Callum Howard, 2019

#ifndef PIXELSORTING_HPP
#define PIXELSORTING_HPP

#include <iso646.h>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h"
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
    void sortPixels(SurfaceT<unsigned char>& image);

    CaptureRef mCapture;

    gl::TextureRef mTexture;
    Surface32f image;
    Frame mBuffer;
};


void PixelSorting::setup() {
    try {
        mCapture = Capture::create(640, 480);
        mCapture->start();
    } catch (ci::Exception &exc) {
        CI_LOG_EXCEPTION("Failed to init capture ", exc);
    }
}

void PixelSorting::update() {
    if (mCapture && mCapture->checkNewFrame()) {
        if (!mTexture) {
            // Capture images come back as top-down, and it's more efficient to
            // keep them that way
            auto captureSurface = *mCapture->getSurface();
            sortPixels(captureSurface);
            mTexture = gl::Texture::create(captureSurface,
                    gl::Texture::Format().loadTopDown());
        } else {
            auto captureSurface = *mCapture->getSurface();
            sortPixels(captureSurface);
            mTexture->update(captureSurface);
        }
    }
}

void PixelSorting::draw() {
    if (mTexture) {
        Rectf destRect = Rectf(mTexture->getBounds())
                .getCenteredFit(getWindowBounds(), true)
                .scaledCentered(0.85f);

        gl::draw(mTexture, destRect);
    }
}

void PixelSorting::sortPixels(SurfaceT<unsigned char>& image) {
    auto pixelIterRead = image.getIter();
    auto pixelIterWrite = image.getIter();
    while (pixelIterRead.line() and pixelIterWrite.line()) {
        auto pixelRow = std::vector<Color>{};
        pixelRow.reserve(image.getWidth());

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

        for (int row = 0; row < image.getWidth() and pixelIterWrite.pixel(); ++row) {
            pixelIterWrite.r() = pixelRow[row].r;
            pixelIterWrite.g() = pixelRow[row].g;
            pixelIterWrite.b() = pixelRow[row].b;
        }
    }
}

} // namespace ch

#endif
