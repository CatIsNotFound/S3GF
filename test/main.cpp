#include "../src/Core.h"
#include "../src/Algorithm/Draw.h"
#include "../src/MultiThread/Components.h"
#include "../src/Utils/DateTime.h"

int main(int argc, char* argv[]) {
    S3GF::Engine engine;
    engine.newWindow(new S3GF::Window("Hello world!"));
    engine.window()->setResizable(true);
    engine.setFPS(30);
    S3GF::Timer timer(3000, []{
        S3GF::Logger::log(S3GF::DateTime::formatDateTime("Now: %E"), S3GF::Logger::INFO);
        S3GF::Logger::log(S3GF::DateTime::formatDateTime("N: %C"), S3GF::Logger::INFO);
    });

    S3GF::Graphics::Line line1(300, 100, 200, 600, 10, S3GF::StdColor::LightGreen);
    S3GF::Graphics::Line line2(100, 200, 400, 180, 32, S3GF::StdColor::DarkOrange);
    S3GF::Graphics::Triangle tri({100, 100}, {200, 200}, {0, 0}, {100, 100, 100, 255});
    S3GF::Graphics::Point pt;
    S3GF::Graphics::Line line3(line1);
    line3.setStartPosition(500, 400);
    engine.window()->installPaintEvent([&line1, &line2, &line3, &tri](S3GF::Renderer* r) {
        r->fillBackground(S3GF::StdColor::DarkGray);
        r->drawLine(line1);
        r->drawLine(line2);
        r->drawLine(line3);
        r->drawTriangle(tri);
    });

    timer.start(0);
    return engine.exec();
}
