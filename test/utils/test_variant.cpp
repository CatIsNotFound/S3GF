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

TEST_CASE("Variant Custom Pointer Test", "[Utils][Var]") {
    SECTION("Create custom pointer variant with deleter") {
        auto* data = new int(42);
        bool deleted = false;
        Variant var(data, 1001, [&deleted](void* ptr) {
            delete static_cast<int*>(ptr);
            deleted = true;
        });
        
        CHECK(var.type() == Variant::Pointer);
        CHECK(var.customTypeID() == 1001);
        CHECK(var.toPointer() == data);
        CHECK(*static_cast<int*>(var.toPointer()) == 42);
        CHECK(var.usedCount() == 1);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test reference counting with copy constructor") {
        auto* data = new std::string("Hello");
        bool deleted = false;
        Variant var1(data, 2002, [&deleted](void* ptr) {
            delete static_cast<std::string*>(ptr);
            deleted = true;
        });
        
        CHECK(var1.usedCount() == 1);
        
        Variant var2(var1);
        CHECK(var1.usedCount() == 2);
        CHECK(var2.usedCount() == 2);
        CHECK(var1.toPointer() == var2.toPointer());
        CHECK(var1.customTypeID() == var2.customTypeID());
        CHECK(*static_cast<std::string*>(var2.toPointer()) == "Hello");
        CHECK_FALSE(deleted);
        
        Variant var3(var2);
        CHECK(var1.usedCount() == 3);
        CHECK(var2.usedCount() == 3);
        CHECK(var3.usedCount() == 3);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test reference counting with copy assignment") {
        auto* data = new float(3.14f);
        bool deleted = false;
        Variant var1(data, 3003, [&deleted](void* ptr) {
            delete static_cast<float*>(ptr);
            deleted = true;
        });
        
        Variant var2;
        CHECK(var1.usedCount() == 1);
        
        var2 = var1;
        CHECK(var1.usedCount() == 2);
        CHECK(var2.usedCount() == 2);
        CHECK(var1.toPointer() == var2.toPointer());
        CHECK(*static_cast<float*>(var2.toPointer()) == 3.14f);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test move semantics") {
        auto* data = new double(2.718);
        bool deleted = false;
        Variant var1(data, 4004, [&deleted](void* ptr) {
            delete static_cast<double*>(ptr);
            deleted = true;
        });
        
        CHECK(var1.usedCount() == 1);
        
        Variant var2(std::move(var1));
        CHECK(var1.isNull());
        CHECK(var1.usedCount() == 0);
        CHECK(var2.usedCount() == 1);
        CHECK(var2.toPointer() == data);
        CHECK(*static_cast<double*>(var2.toPointer()) == 2.718);
        CHECK_FALSE(deleted);
        
        Variant var3;
        var3 = std::move(var2);
        CHECK(var2.isNull());
        CHECK(var2.usedCount() == 0);
        CHECK(var3.usedCount() == 1);
        CHECK(var3.toPointer() == data);
        CHECK(*static_cast<double*>(var3.toPointer()) == 2.718);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test reset functionality") {
        auto* data1 = new int(100);
        auto* data2 = new int(200);
        bool deleted1 = false;
        bool deleted2 = false;
        
        Variant var(data1, 5005, [&deleted1](void* ptr) {
            delete static_cast<int*>(ptr);
            deleted1 = true;
        });
        
        CHECK(var.usedCount() == 1);
        CHECK(*static_cast<int*>(var.toPointer()) == 100);
        CHECK_FALSE(deleted1);
        
        var.setValue(data2, 6006, [&deleted2](void* ptr) {
            delete static_cast<int*>(ptr);
            deleted2 = true;
        });
        
        CHECK(deleted1);
        CHECK_FALSE(deleted2);
        CHECK(var.usedCount() == 1);
        CHECK(var.customTypeID() == 6006);
        CHECK(*static_cast<int*>(var.toPointer()) == 200);
    }
    
    SECTION("Test clearValue with custom pointer") {
        auto* data = new std::vector<int>{1, 2, 3, 4, 5};
        bool deleted = false;
        
        {
            Variant var(data, 7007, [&deleted](void* ptr) {
                delete static_cast<std::vector<int>*>(ptr);
                deleted = true;
            });
            
            CHECK(var.usedCount() == 1);
            CHECK(static_cast<std::vector<int>*>(var.toPointer())->size() == 5);
            CHECK_FALSE(deleted);
            
            var.clearValue();
            CHECK(var.isNull());
            CHECK(var.usedCount() == 0);
            CHECK(deleted);
        }
    }
    
    SECTION("Test customTypeID getter and setter") {
        auto* data = new char('A');
        Variant var(data, [](void* ptr) {
            delete static_cast<char*>(ptr);
        });
        
        CHECK(var.customTypeID() == 0);
        
        var.setCustomTypeID(8008);
        CHECK(var.customTypeID() == 8008);
        
        var.setCustomTypeID(9009);
        CHECK(var.customTypeID() == 9009);
    }
    
    SECTION("Test multiple copies and destruction order") {
        auto* data = new int(999);
        bool deleted = false;
        
        {
            Variant var1(data, 10101, [&deleted](void* ptr) {
                delete static_cast<int*>(ptr);
                deleted = true;
            });
            
            {
                Variant var2(var1);
                Variant var3(var2);
                Variant var4(var3);
                
                CHECK(var1.usedCount() == 4);
                CHECK(var2.usedCount() == 4);
                CHECK(var3.usedCount() == 4);
                CHECK(var4.usedCount() == 4);
                CHECK_FALSE(deleted);
            }
            
            CHECK(var1.usedCount() == 1);
            CHECK_FALSE(deleted);
        }
        
        CHECK(deleted);
    }
    
    SECTION("Test self-assignment") {
        auto* data = new int(777);
        bool deleted = false;
        
        Variant var(data, 11111, [&deleted](void* ptr) {
            delete static_cast<int*>(ptr);
            deleted = true;
        });
        
        auto original_ptr = var.toPointer();
        auto original_count = var.usedCount();
        
        var = var;
        
        CHECK(var.toPointer() == original_ptr);
        CHECK(var.usedCount() == original_count);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test pointer without custom type ID") {
        auto* data = new int(123);
        bool deleted = false;
        
        Variant var(data, [&deleted](void* ptr) {
            delete static_cast<int*>(ptr);
            deleted = true;
        });
        
        CHECK(var.type() == Variant::Pointer);
        CHECK(var.customTypeID() == 0);
        CHECK(var.toPointer() == data);
        CHECK(*static_cast<int*>(var.toPointer()) == 123);
        CHECK(var.usedCount() == 1);
        CHECK_FALSE(deleted);
    }
    
    SECTION("Test pointer without deleter") {
        int data = 456;
        
        Variant var(&data);
        
        CHECK(var.type() == Variant::Pointer);
        CHECK(var.customTypeID() == 0);
        CHECK(var.toPointer() == &data);
        CHECK(*static_cast<int*>(var.toPointer()) == 456);
        CHECK(var.usedCount() == 1);
    }
}
