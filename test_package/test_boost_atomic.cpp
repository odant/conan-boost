// Boost test for Conan package
// Dmitriy Vetutnev, Odant, 2018


#include <boost/atomic.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>


int main(int, char**) {

    std::cout << std::boolalpha;


    boost::atomic<int8_t> int8{0};
    std::thread thread_int8_1{ [&int8](){ int8++; } };
    std::thread thread_int8_2{ [&int8](){ int8++; } };
    thread_int8_1.join();
    thread_int8_2.join();
    if (int8 != 2) {
        std::cout << "boost::atomic<int8_t> faild, result: " << std::to_string(int8) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "int8: " << std::to_string(int8) << ", is_lock_free: " << int8.is_lock_free() << std::endl;


    boost::atomic<int16_t> int16{0};
    std::thread thread_int16_1{ [&int16](){ int16++; } };
    std::thread thread_int16_2{ [&int16](){ int16++; } };
    thread_int16_1.join();
    thread_int16_2.join();
    if (int16 != 2) {
        std::cout << "boost::atomic<int16_t> faild, result: " << std::to_string(int16) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "int16: " << std::to_string(int16) << ", is_lock_free: " << int16.is_lock_free() << std::endl;


    boost::atomic<int32_t> int32{0};
    std::thread thread_int32_1{ [&int32](){ int32++; } };
    std::thread thread_int32_2{ [&int32](){ int32++; } };
    thread_int32_1.join();
    thread_int32_2.join();
    if (int32 != 2) {
        std::cout << "boost::atomic<int32_t> faild, result: " << std::to_string(int32) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "int32: " << std::to_string(int32) << ", is_lock_free: " << int32.is_lock_free() << std::endl;


    boost::atomic<int64_t> int64{0};
    std::thread thread_int64_1{ [&int64](){ int64++; } };
    std::thread thread_int64_2{ [&int64](){ int64++; } };
    thread_int64_1.join();
    thread_int64_2.join();
    if (int64 != 2) {
        std::cout << "boost::atomic<int64_t> faild, result: " << std::to_string(int64) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "int64: " << std::to_string(int64) << ", is_lock_free: " << int64.is_lock_free() << std::endl;


    return EXIT_SUCCESS;
}
