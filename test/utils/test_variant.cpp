#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"

using namespace MyEngine;

StringList sample = {"Hello", "world", "Here", "is", "a", "sample", "text"};

TEST_CASE("Variant Simple Test", "[Utils][Var]") {
    Variant var_bool(true);
    Variant var_uint8 = Variant((uint8_t)127);
    Variant var_int16(static_cast<int16_t>(-2048));
    Variant var_int64(1234567890LL);
    Variant var_float = Variant(3.1415926f);
    Variant var_double = Variant(3.14159265358979323846);
    Variant var_str1("Hello world!");
    Variant var_str2 = Variant(std::string("Flag2"));
    Variant var_nullptr;
    Variant var_ptr = Variant(&sample);
    auto copy_list = new StringList(sample);
    Variant var_ptr2(copy_list, 2048, [](void* data) {
        auto ptr = static_cast<StringList*>(data);
        delete ptr;
    });
    SECTION("Check the variable type is correct") {
        CHECK(var_bool.type() == Variant::Bool);
        CHECK(var_uint8.type() == Variant::UInt8);
        CHECK(var_int64.type() == Variant::Int64);
        CHECK(var_float.type() == Variant::Float);
        CHECK(var_double.type() == Variant::Double);
        CHECK(var_str1.type() == Variant::String);
        CHECK(var_str2.type() == Variant::String);
        CHECK(var_nullptr.type() == Variant::Null);
        CHECK(var_ptr.type() == Variant::Pointer);
        CHECK(var_ptr.customTypeID() == 0);
        CHECK(var_ptr2.type() == Variant::Pointer);
        CHECK(var_ptr2.customTypeID() == 2048);
    }

    SECTION("Check the variable value") {
        CHECK(var_bool.toBool() == true);
        CHECK(var_uint8.toUInt8() == (uint8_t)127);
        CHECK(var_int16.toInt16() == (int16_t)-2048);
        CHECK(var_int64.toInt64() == 1234567890LL);
        CHECK(var_float.toFloat() == 3.1415926f);
        CHECK(var_double.toDouble() == 3.14159265358979323846);
        CHECK(var_str1.toString() == "Hello world!");
        CHECK(var_str2.toString() == "Flag2");
        CHECK(var_nullptr.isNull());
        CHECK_FALSE(var_ptr.isNull());
        CHECK_FALSE(var_ptr2.isNull());
        CHECK(var_ptr.toPointer() == &sample);
        CHECK(var_ptr2.toPointer() == copy_list);
    }

    SECTION("Check the variable conversation") {
        CHECK(var_bool.toInt32() == 1);
        CHECK(var_uint8.toInt64() == 127LL);
        CHECK(var_uint8.toFloat() == 127.f);
        CHECK(var_int16.toInt32() == -2048);
        CHECK(var_int16.toInt64() == -2048);
        CHECK(var_int16.toDouble() == -2048.0);
        CHECK(var_int64.toUInt64() == 1234567890LL);
        CHECK(var_int64.toFloat() == 1234567890.f);
        CHECK(var_int64.toDouble() == 1234567890.000000);
        CHECK(var_float.toInt8() == (int8_t)3);
        CHECK(var_double.toUInt8() == 3ULL);
        CHECK_THROWS(var_float.toString());
        CHECK_THROWS(var_double.toString());
    }
}
