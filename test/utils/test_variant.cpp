#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"

using namespace MyEngine;

StringList sample = {"Hello", "world", "Here", "is", "a", "sample", "text"};
StringList sample2 = {"apple", "banana", "orange"};

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
        CHECK(var_nullptr.isNull());
        CHECK_FALSE(var_ptr.isNull());
        CHECK_FALSE(var_ptr2.isNull());
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
        CHECK(var_ptr.toPointer() == &sample);
        CHECK(var_ptr2.toPointer() == copy_list);
        CHECK((reinterpret_cast<StringList*>(var_ptr.toPointer()))->at(1) == sample[1]);
        CHECK(reinterpret_cast<StringList*>(var_ptr2.toPointer())->at(2) == copy_list->at(2));
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
    }

    SECTION("Set new variant and check the variant value whether it is changed") {
        var_bool.setValue(false);
        var_uint8.setValue((uint8_t)24);
        var_int16.setValue((int16_t)1024);
        var_int64.setValue(66666666666LL);
        var_float.setValue(123.456f);
        var_double.setValue(123.4567890);
        var_str1.setValue("New str1");
        var_str2.setValue("New str2");
        var_ptr.setValue(&sample2);
        auto copy_sample2 = new StringList(sample2);
        var_ptr2.setValue(copy_sample2, 4096, [](void* data) {
            auto ptr = static_cast<StringList*>(data);
            delete ptr;
        });

        CHECK(var_bool.toBool() == false);
        CHECK(var_uint8.toUInt8() == 24);
        CHECK(var_int16.toInt16() == 1024);
        CHECK(var_int64.toInt64() == 66666666666LL);
        CHECK(var_float.toFloat() == 123.456f);
        CHECK(var_double.toDouble() == 123.4567890);
        CHECK(var_str1.toString() == "New str1");
        CHECK(var_str2.toString() == "New str2");
        CHECK(var_ptr.toPointer() == &sample2);
        CHECK(var_ptr.customTypeID() == 0);
        CHECK(reinterpret_cast<StringList*>(var_ptr.toPointer())->at(1) == sample2[1]);
        CHECK(var_ptr2.customTypeID() == 4096);
        CHECK(var_ptr2.toPointer() == copy_sample2);
        CHECK((reinterpret_cast<StringList*>(var_ptr2.toPointer()))->at(2) == copy_sample2->at(2));
    }

    SECTION("Set new different type of the variant") {
        var_bool.setValue("true");
        var_uint8.setValue(255.f);
        var_int16.setValue(false);
        var_int64.setValue((uint8_t)123);
        var_float.setValue((int8_t)-123);
        var_double.setValue(1234567890ULL);
        var_str1.setValue(&sample);
        auto copy_sample = new StringList(sample2);
        var_str2.setValue(copy_sample, 4096, [](void* data) {
            auto ptr = static_cast<StringList*>(data);
            delete ptr;
        });
        var_ptr.clearValue();
        var_ptr2.clearValue();
        CHECK_THROWS(var_bool.toBool());
        CHECK(var_bool.type() == Variant::String);
        CHECK(var_bool.toString() == "true");
        CHECK(var_uint8.type() == Variant::Float);
        CHECK(var_uint8.toFloat() == 255.f);
        CHECK(var_int16.type() == Variant::Bool);
        CHECK(var_int16.toBool() == false);
        CHECK(var_int64.type() == Variant::UInt8);
        CHECK(var_int64.toUInt8() == (uint8_t)123);
        CHECK(var_float.type() == Variant::Int8);
        CHECK(var_float.toInt8() == (int8_t)-123);
        CHECK(var_double.type() == Variant::UInt64);
        CHECK(var_double.toUInt64() == 1234567890ULL);
        CHECK(var_str1.type() == Variant::Pointer);
        CHECK(var_str1.toPointer() == &sample);
        CHECK(reinterpret_cast<StringList*>(var_str1.toPointer())->at(1) == sample[1]);
        CHECK(var_str2.type() == Variant::Pointer);
        CHECK(var_str2.toPointer() == copy_sample);
        CHECK(var_str2.customTypeID() == 4096);
        CHECK(reinterpret_cast<StringList*>(var_str2.toPointer())->at(2) == copy_sample->at(2));
        CHECK(var_ptr.type() == Variant::Null);
        CHECK(var_ptr.customTypeID() == 0);
        CHECK(var_ptr.isNull() == true);
        CHECK(var_ptr2.type() == Variant::Null);
        CHECK(var_ptr2.customTypeID() == 0);
        CHECK(var_ptr2.isNull() == true);
    }
}
