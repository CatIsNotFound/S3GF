#include "FilledWin.h"
#include "MyEngine"
using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(Logger::Debug);
    Engine engine;
    engine.setFPS(144);
    auto win = new FilledWin("./tiny_block.png", &engine, engine.applicationName(), 1024, 800);
    win->setWindowIcon("./block.png");
    win->setMinimumSize(300, 150);
    win->setResizable(true);
    return engine.exec();
}