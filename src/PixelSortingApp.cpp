#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "PixelSorting.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class PixelSortingApp : public App {
public:
    void setup() override;
    void mouseDown(MouseEvent event) override;
    void update() override;
    void draw() override;

private:
    ch::PixelSorting mPixelSortingCanvas;
};

void PixelSortingApp::setup() {
    mPixelSortingCanvas.setup();
}

void PixelSortingApp::mouseDown(MouseEvent event) {}

void PixelSortingApp::update() {
    mPixelSortingCanvas.update();
}

void PixelSortingApp::draw() {
    gl::clear(Color{0, 0, 0});
    mPixelSortingCanvas.draw();
}

CINDER_APP(PixelSortingApp, RendererGl)
