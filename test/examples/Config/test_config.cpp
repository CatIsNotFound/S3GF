
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

using BagPack = std::unordered_map<std::string, Variant>;
using AllBags = std::vector<BagPack>;

MACRO_MAKE_CONFIG(SaveData,
    MACRO_MAKE_PROPERTY(std::string, playerName)
    MACRO_MAKE_PROPERTY(uint64_t, id)
    MACRO_MAKE_PROPERTY(uint64_t, firstPlayedTime)
    MACRO_MAKE_PROPERTY(uint64_t, lastPlayedTime)
    MACRO_MAKE_PROPERTY(BagPack, bagPack)
    MACRO_MAKE_PROPERTY(AllBags, allBags)
);


int main() {
    Config::BinaryConfigParser parser(Config::ConfigObject{});
    std::cout << "[parser 1]\n";
    std::cout << "Object: " << (parser.isObject() ? "true" : "false") << std::endl;
    std::cout << "Array: " << (parser.isArray() ? "true" : "false") << std::endl;
    SaveData saveData;
    saveData.set_id(100);
    saveData.set_playerName("Red");
    saveData.set_firstPlayedTime(DateTime::currentTimestamp());
    saveData.set_lastPlayedTime(DateTime::currentTimestamp());
    BagPack bagPack;
    bagPack["a0"] = Variant(2);
    bagPack["a1"] = Variant(3);
    bagPack["a2"] = Variant(4);
    saveData.set_bagPack(std::move(bagPack));
    BagPack bag1;
    bag1["grape"] = Variant(50);
    bag1["banana"] = Variant(200);
    BagPack bag2;
    bag2["banana"] = Variant(300);
    bag2["grape"] = Variant(400);
    AllBags allBags = {
        bag1, bag2
    };
    saveData.set_allBags(std::move(allBags));
    MACRO_MAKE_CONFIG_OBJECT(saveDataObject);
    MACRO_APPEND_PROP_TO_OBJECT(saveDataObject, saveData, id);
    MACRO_APPEND_PROP_TO_OBJECT(saveDataObject, saveData, playerName);
    MACRO_APPEND_PROP_TO_OBJECT(saveDataObject, saveData, firstPlayedTime);
    MACRO_APPEND_PROP_TO_OBJECT(saveDataObject, saveData, lastPlayedTime);
    auto all_bags = std::make_unique<AllBags>(saveData.get_allBags());
    auto bag_pack = std::make_unique<BagPack>(saveData.get_bagPack());
    Variant var_bagpack(bag_pack.get(), Config::Var_ConfigObject);
    saveDataObject.emplace("bagPack", var_bagpack);
    Variant var_all_bags(all_bags.get(), Config::Var_ConfigArray);
    saveDataObject.emplace("allBags", var_all_bags);
    parser.setConfigObject(std::move(saveDataObject));
    Logger::log(Logger::Info, "Saved file: {}", parser.saveFile("./test_obj_complex.bin"));
    Logger::log(Logger::Info, "{}\nArr: {}", parser.configObject().size(), static_cast<BagPack*>(var_bagpack.toPointer())->size());

    Config::BinaryConfigParser load_parser("./test_obj_complex.bin");
    if (load_parser.loadFile()) {
        Logger::log("Loaded config file");
    } else {
        Logger::log("Failed to load config file");
        return 1;
    }
    Logger::log(Logger::Info, "is_object = {}, is_array = {}", load_parser.isObject(), load_parser.isArray());
    for (auto& [key, value] : load_parser.configObject()) {
        Logger::log(Logger::Info, "{} -> {}", key, value.valueAsString([](void* d, uint32_t type_id) {
            std::string out;
            if (type_id == Config::Var_ConfigObject) {
                auto obj = static_cast<Config::ConfigObject*>(d);
                out = std::to_string(obj->size());
            } else if (type_id == Config::Var_ConfigArray) {
                auto arr = static_cast<Config::ConfigArray*>(d);
                out = std::to_string(arr->size());
            }
            return out;
        }));
    }
    return 0;
}
