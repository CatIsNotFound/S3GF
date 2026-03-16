#include "MyEngine"

using namespace MyEngine;

int main(int argc, char* argv[]) {
    Logger::setBaseLogLevel(Logger::Debug);
    auto cur_path = FileSystem::getDirectoryFromFile(argv[0]);
    Engine engine;
    engine.setFPS(60);
    auto res_mgr = ResourceSystem::global();
    res_mgr->addResource("back_button1",
        FileSystem::getAbsolutePath(FMT::format("{}/assets/back_button_1.png", cur_path)), ResourceSystem::Type::Image);
    res_mgr->addResource("back_button2",
        FileSystem::getAbsolutePath(FMT::format("{}/assets/back_button_2.png", cur_path)), ResourceSystem::Type::Image);
    res_mgr->addResource("back_button3",
        FileSystem::getAbsolutePath(FMT::format("{}/assets/back_button_3.png", cur_path)), ResourceSystem::Type::Image);
    res_mgr->addResource("back_button4",
        FileSystem::getAbsolutePath(FMT::format("{}/assets/back_button_4.png", cur_path)), ResourceSystem::Type::Image);
    res_mgr->addResource("bgm", FMT::format("{}/assets/main.mp3", cur_path), ResourceSystem::Type::BGM);
    res_mgr->addResource("peace", FMT::format("{}/assets/peace.wav", cur_path), ResourceSystem::Type::BGM);
    res_mgr->addResource("bell2", FMT::format("{}/assets/Samples/bell2.wav", cur_path), ResourceSystem::Type::SFX);
    res_mgr->addResource("kik", FMT::format("{}/assets/Samples/kik.wav", cur_path), ResourceSystem::Type::SFX);
    res_mgr->addResource("open1", FMT::format("{}/assets/Samples/open1.wav", cur_path), ResourceSystem::Type::SFX);
    res_mgr->addResource("snare1", FMT::format("{}/assets/Samples/snare1.wav", cur_path), ResourceSystem::Type::SFX);
    res_mgr->asyncLoadAllResources();
    return engine.exec();
}
