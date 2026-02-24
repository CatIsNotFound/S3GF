
#include "MyEngine"

using namespace MyEngine;

MACRO_MAKE_CONFIG(Rect,
    MACRO_MAKE_PROPERTY(float, x)
    MACRO_MAKE_PROPERTY(float, y)
    MACRO_MAKE_PROPERTY(float, width)
    MACRO_MAKE_PROPERTY(float, height)
    MACRO_MAKE_PROPERTY(std::string, name)
    MACRO_MAKE_PROPERTY(uint64_t, id)
    MACRO_MAKE_PROPERTY(SDL_Color, backgroundColor, 255, 149, 36, 255)
    MACRO_MAKE_PROPERTY(SDL_Color, foregroundColor, StdColor::Black)
);


int main() {
    Config::BinaryConfigParser parser(Config::ConfigObject{});
    std::cout << "[parser 1]\n";
    std::cout << "Object: " << (parser.isObject() ? "true" : "false") << std::endl;
    std::cout << "Array: " << (parser.isArray() ? "true" : "false") << std::endl;

    parser.configObject().emplace("Test", 123456);
    parser.configObject().emplace("ID", "1a2b3c4d");
    parser.configObject().emplace("bool", true);
    parser.configObject().emplace("PI", 3.1415926);
    parser.configObject().emplace("uint8", static_cast<uint8_t>(127));
    if (parser.saveFile("./test.bin")) {
        Logger::log("Log file saved!", Logger::LogLevel::Info);
    } else {
        Logger::log("Log file is not saved!", Logger::Info);
    }

    Config::BinaryConfigParser parser2(Config::ConfigArray{});
    std::cout << "[parser 2]\n";
    std::cout << "Object: " << (parser2.isObject() ? "true" : "false") << std::endl;
    std::cout << "Array: " << (parser2.isArray() ? "true" : "false") << std::endl;

    Config::ConfigObject object1;
    object1.emplace("name", "Amy");
    object1.emplace("id", 1234);
    object1.emplace("Rand", 123.4567);

    Config::ConfigObject object2;
    object2.emplace("name", "Bob");
    object2.emplace("id", 1235);
    object2.emplace("Rand", 188.7418);

    parser2.configArray().push_back(object1);
    parser2.configArray().push_back(object2);

    if (parser2.saveFile("./test_arr.bin")) {
        Logger::log("Log file saved!", Logger::LogLevel::Info);
    } else {
        Logger::log("Log file is not saved!", Logger::Info);
    }

    Config::BinaryConfigParser parser3("./test.bin");
    if (parser3.loadFile()) {
        Logger::log("Log file loaded!", Logger::Info);
    } else {
        Logger::log("Log file is not loaded!", Logger::Info);
        return 0;
    }
    std::cout << "[parser 3]\n";
    std::cout << "Object: " << (parser3.isObject() ? "true" : "false") << std::endl;
    std::cout << "Array: " << (parser3.isArray() ? "true" : "false") << std::endl;

    for (auto& key : parser3.configObject()) {
        Logger::log(Logger::Info, "{}:{}", key.first, key.second.valueAsString());
    }
    Logger::log(Logger::Info, "Test: {}", parser3.configObject().at("Test").toUInt64());
    std::cout << std::endl;

    Config::BinaryConfigParser parser4("./test_arr.bin");
    if (parser4.loadFile()) {
        Logger::log("Log file loaded!", Logger::Info);
    } else {
        Logger::log("Log file is not loaded!", Logger::Info);
        return 0;
    }
    std::cout << "[parser 4]\n";
    std::cout << "Object: " << (parser4.isObject() ? "true" : "false") << std::endl;
    std::cout << "Array: " << (parser4.isArray() ? "true" : "false") << std::endl;
    size_t idx = 0;
    for (auto& map : parser4.configArray()) {
        std::cout << "[Index " << idx++ << "]\n";
        for (auto& key : map) {
            std::cout << key.first << ":" << key.second.valueAsString() << "\n";
        }
    }
    return 0;
}
