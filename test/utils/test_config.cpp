#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../include/MyEngine/MyEngine"
using namespace MyEngine;

MACRO_MAKE_CONFIG(GameData,
    MACRO_MAKE_PROPERTY(uint32_t, dataID)
    MACRO_MAKE_PROPERTY(std::string, playerName)
    MACRO_MAKE_PROPERTY(int32_t, HP)
    MACRO_MAKE_PROPERTY(int32_t, MP)
    MACRO_MAKE_PROPERTY(uint8_t, LV)
    MACRO_MAKE_PROPERTY(Config::ConfigObject, bagPack)
);

MACRO_MAKE_CONFIG(GamePlayer,
    MACRO_MAKE_PROPERTY(std::string, playerName)
    MACRO_MAKE_PROPERTY(size_t, last_played_time)
    MACRO_MAKE_PROPERTY(size_t, first_played_time)
    MACRO_MAKE_PROPERTY(float, seed, 0.f)
    MACRO_MAKE_PROPERTY(bool, invalid, false)
);

TEST_CASE("Binary Configuration Parser Simple Test", "[Utils][Conf]") {
    DateTime::DT last_dt(2024, 8, 20, 8, 29, 39);
    DateTime::DT first_dt(2019, 3, 16, 8, 29, 39);
    StringList player_names = {
        "Amy", "Alice", "Bob", "Tom", "Jerry", "Ben", "Mark", "Sam", "Mask", "Swift",
        "Kale", "KK", "Pale", "YuYan", "Wendy", "John", "Tim", "Zed", "X"
    };
    StringList item_names = {
        "HP Posion", "MP Posion", "Apple", "Pear", "Banana", "Orange", "Grapes",
        "Knife", "Sword", "Shield", "Boomber", "Toy"
    };
    SECTION("Write Config Object Test") {
        GamePlayer player;
        player.set_playerName("nico");
        player.set_last_played_time(DateTime::generateTimestamp(last_dt));
        player.set_first_played_time(DateTime::generateTimestamp(first_dt));
        player.set_seed(123.890123);
        player.set_invalid(true);

        MACRO_MAKE_CONFIG_OBJECT(users);
        MACRO_APPEND_PROP_TO_OBJECT(users, player, last_played_time);
        MACRO_APPEND_PROP_TO_OBJECT(users, player, first_played_time);
        MACRO_APPEND_PROP_TO_OBJECT(users, player, playerName);
        MACRO_APPEND_PROP_TO_OBJECT(users, player, seed);
        MACRO_APPEND_PROP_TO_OBJECT(users, player, invalid);

        Config::BinaryConfigParser bin_conf_parser(users);
        CHECK_FALSE(bin_conf_parser.isArray());
        CHECK(bin_conf_parser.isObject());
        bin_conf_parser.setDefaultFileName("./result1.bin");
        CHECK(bin_conf_parser.saveFile());
    }

    SECTION("Read Config Object Test") {
        Config::BinaryConfigParser binary_config_parser("./result1.bin");
        CHECK(binary_config_parser.loadFile());
        CHECK_FALSE(binary_config_parser.isArray());
        CHECK(binary_config_parser.isObject());
        REQUIRE(binary_config_parser.configObject().size() == 5);
        REQUIRE(binary_config_parser.configObject().contains("last_played_time"));
        REQUIRE(binary_config_parser.configObject().contains("first_played_time"));
        REQUIRE(binary_config_parser.configObject().contains("playerName"));
        REQUIRE(binary_config_parser.configObject().contains("seed"));
        REQUIRE(binary_config_parser.configObject().contains("invalid"));
        GamePlayer player;
        player.set_playerName(binary_config_parser.configObject().at("playerName").toString());
        player.set_first_played_time(binary_config_parser.configObject().at("first_played_time").toUInt64());
        player.set_last_played_time(binary_config_parser.configObject().at("last_played_time").toUInt64());
        player.set_seed(binary_config_parser.configObject().at("seed").toFloat());
        player.set_invalid(binary_config_parser.configObject().at("invalid").toBool());
        CHECK(player.get_playerName() == "nico");
        size_t ts1 = DateTime::generateTimestamp(first_dt);
        size_t ts2 = DateTime::generateTimestamp(last_dt);
        CHECK(ts1 == player.get_first_played_time());
        CHECK(ts2 == player.get_last_played_time());
        CHECK(player.get_invalid() == true);
        CHECK(player.get_seed() == 123.890123f);
    }

    SECTION("Write Config Array Test") {
        Config::ConfigArray datas;
        for (int i = 0; i < 0xff; i++) {
            GameData data;
            data.set_playerName(player_names.at(RandomGenerator::randUInt(0, player_names.size() - 1)));
            data.set_dataID(RandomGenerator::randBigUInt(100000, 9999999));
            data.set_HP(RandomGenerator::randInt(0, 10000));
            data.set_MP(RandomGenerator::randInt(0, 10000));
            data.set_LV(RandomGenerator::randUInt(0, 999));
            MACRO_MAKE_CONFIG_OBJECT(obj);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, playerName);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, dataID);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, HP);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, MP);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, LV);

            datas.push_back(obj);
        }
        Config::BinaryConfigParser binary_config_parser(datas);
        CHECK(binary_config_parser.isArray());
        CHECK_FALSE(binary_config_parser.isObject());
        REQUIRE(binary_config_parser.configArray().size() == 0xff);
        REQUIRE_NOTHROW(binary_config_parser.saveFile("./result2.bin"));
    }

    SECTION("Read Config Array Test") {
        Config::BinaryConfigParser binary_config_parser("./result2.bin");
        REQUIRE_NOTHROW(binary_config_parser.loadFile());
        CHECK(binary_config_parser.isArray());
        CHECK_FALSE(binary_config_parser.isObject());
        CHECK(binary_config_parser.configArray().size() == 0xff);
        CHECK(binary_config_parser.configArray()[RandomGenerator::randUInt(0, 0xff - 1)].size() == 5);
    }
}

TEST_CASE("Binary Configuration Parser Complex Test", "[Utils][Conf]") {
    GameData game_data;
    game_data.set_LV(20);
    game_data.set_HP(3000);
    game_data.set_MP(2000);
    game_data.set_playerName("Hero");
    game_data.set_dataID(RandomGenerator::randBigUInt(100000, 9999999));
    Config::ConfigObject bagpack;
    bagpack.emplace("Sword", 1);
    bagpack.emplace("Shield", 1);
    bagpack.emplace("Mace", 1);
    bagpack.emplace("Laser", 1);
    bagpack.emplace("HP Poison", 5);
    bagpack.emplace("MP Poison", 5);
    game_data.set_bagPack(bagpack);

    SECTION("Write binary object to file") {
        MACRO_MAKE_CONFIG_OBJECT(new_obj);
        MACRO_APPEND_PROP_TO_OBJECT(new_obj, game_data, LV);
        MACRO_APPEND_PROP_TO_OBJECT(new_obj, game_data, MP);
        MACRO_APPEND_PROP_TO_OBJECT(new_obj, game_data, HP);
        MACRO_APPEND_PROP_TO_OBJECT(new_obj, game_data, playerName);
        MACRO_APPEND_PROP_TO_OBJECT(new_obj, game_data, dataID);
        MACRO_APPEND_CONF_OBJ_PROP_TO_OBJECT(bag_pack_ptr, new_obj, game_data, bagPack);
        Config::BinaryConfigParser bin_parser{new_obj};
        CHECK(bin_parser.isObject() == true);
        CHECK(bin_parser.isArray() == false);
        CHECK_NOTHROW(bin_parser.saveFile("./result3.bin") == true);
    }

    SECTION("Read binary object from file") {
        Config::BinaryConfigParser read_bin_parser("./result3.bin");
        REQUIRE_NOTHROW(read_bin_parser.loadFile() == true);
        CHECK(read_bin_parser.isObject() == true);
        CHECK(read_bin_parser.isArray() == false);
        auto& object = read_bin_parser.configObject();
        CHECK(object.size() == 6);
        CHECK(object.contains("HP") == true);
        CHECK(object.contains("MP") == true);
        CHECK(object.contains("LV") == true);
        CHECK(object.contains("dataID") == true);
        CHECK(object.contains("playerName") == true);
        REQUIRE(object.contains("bagPack") == true);
        CHECK_NOFAIL(reinterpret_cast<Config::ConfigObject*>(object.at("bagPack")
                                                             .toPointer())->size() == game_data.bagPack().size());
        auto my_bag_pack = reinterpret_cast<Config::ConfigObject*>(object.at("bagPack").toPointer());
        CHECK(my_bag_pack->at("HP Poison").toInt32() == game_data.bagPack().at("HP Poison").toInt32());
    }

    SECTION("Write binary array to file") {
        Config::ConfigArray game_datas;
        std::vector<std::unique_ptr<Config::ConfigObject>> bg_packs;
        for (int i = 0; i < 300; i++) {
            GameData data;
            data.set_playerName("Player_" + std::to_string(i));
            data.set_dataID(RandomGenerator::randBigUInt(100000, 9999999));
            data.set_HP(RandomGenerator::randInt(1000, 5000));
            data.set_MP(RandomGenerator::randInt(500, 3000));
            data.set_LV(RandomGenerator::randUInt(1, 50));
            Config::ConfigObject the_bagpack;
            the_bagpack.emplace("Sword", RandomGenerator::randInt(1, 3));
            the_bagpack.emplace("Shield", RandomGenerator::randInt(1, 3));
            the_bagpack.emplace("HP Poison", RandomGenerator::randInt(1, 10));
            data.set_bagPack(the_bagpack);
            MACRO_MAKE_CONFIG_OBJECT(obj);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, playerName);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, dataID);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, HP);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, MP);
            MACRO_APPEND_PROP_TO_OBJECT(obj, data, LV);
            MACRO_APPEND_CONF_OBJ_PROP_TO_OBJECT(new_bagpack, obj, data, bagPack);
            bg_packs.push_back(std::move(new_bagpack));
            game_datas.push_back(obj);
        }
        Config::BinaryConfigParser bin_parser{game_datas};
        CHECK(bin_parser.isArray() == true);
        CHECK(bin_parser.isObject() == false);
        REQUIRE(bin_parser.configArray().size() == 300);
        CHECK_NOTHROW(bin_parser.saveFile("./result4.bin") == true);
    }

    SECTION("Read binary array to file") {
        Config::BinaryConfigParser test_bin_parser("./result4.bin");
        REQUIRE_NOTHROW(test_bin_parser.loadFile() == true);
        CHECK(test_bin_parser.isArray() == true);
        CHECK(test_bin_parser.isObject() == false);
        auto& array = test_bin_parser.configArray();
        CHECK(array.size() == 300);
        CHECK(array[0].size() == 6);
        CHECK(array[0].contains("playerName") == true);
        CHECK(array[0].contains("dataID") == true);
        CHECK(array[0].contains("HP") == true);
        CHECK(array[0].contains("MP") == true);
        CHECK(array[0].contains("LV") == true);
        CHECK(array[0].contains("bagPack") == true);
        auto first_player_name = array[0].at("playerName").toString();
        CHECK(first_player_name == "Player_0");
        auto first_bag_pack = reinterpret_cast<Config::ConfigObject*>(array[0].at("bagPack").toPointer());
        CHECK(first_bag_pack->size() == 3);
        CHECK(first_bag_pack->contains("Sword") == true);
        CHECK(first_bag_pack->contains("Shield") == true);
        CHECK(first_bag_pack->contains("HP Poison") == true);
    }
}