#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"
using namespace MyEngine;

MACRO_MAKE_CONFIG(GameData,
    MACRO_MAKE_PROPERTY(uint32_t, dataID)
    MACRO_MAKE_PROPERTY(std::string, playerName)
    MACRO_MAKE_PROPERTY(int32_t, HP)
    MACRO_MAKE_PROPERTY(int32_t, MP)
    MACRO_MAKE_PROPERTY(uint8_t, LV)
);

MACRO_MAKE_CONFIG(GamePlayer,
    MACRO_MAKE_PROPERTY(std::string, playerName)
    MACRO_MAKE_PROPERTY(size_t, last_played_time)
    MACRO_MAKE_PROPERTY(size_t, first_played_time)
    MACRO_MAKE_PROPERTY(float, seed, 0.f)
    MACRO_MAKE_PROPERTY(bool, invalid, false)
);

TEST_CASE("Configuration Test", "[Utils][Conf]") {
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
        for (int i = 0; i < 0xfffff; i++) {
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
        REQUIRE(binary_config_parser.configArray().size() == 0xfffff);
        REQUIRE_NOTHROW(binary_config_parser.saveFile("./result2.bin"));
    }

    SECTION("Read Config Array Test") {
        Config::BinaryConfigParser binary_config_parser("./result2.bin");
        REQUIRE_NOTHROW(binary_config_parser.loadFile());
        CHECK(binary_config_parser.isArray());
        CHECK_FALSE(binary_config_parser.isObject());
        CHECK(binary_config_parser.configArray().size() == 0xfffff);
        CHECK(binary_config_parser.configArray()[RandomGenerator::randUInt(0, 0xfffff - 1)].size() == 5);
    }
}