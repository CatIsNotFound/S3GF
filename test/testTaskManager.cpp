#include "MultiThread/All"
#include "Utils/All"

using namespace S3GF;

void task1() {
    Logger::log(std::format("Started Task 1!"), S3GF::Logger::INFO);
    uint64_t sum = 0;
    for (int i = 0; i < 100; ++i) {
        sum += i;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::log(std::format("Finished Task 1! Sum is {}", sum), S3GF::Logger::INFO);
}

void task2() {
    Logger::log(std::format("Started Task 2!"), S3GF::Logger::INFO);
    int64_t sum = 1000000;
    for (int i = 1; i < 1000; ++i) {
        sum -= i;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::log(std::format("Finished Task 2! Sum is {}", sum), S3GF::Logger::INFO);
}

void task3() {
    Logger::log(std::format("Started Task 3!"), S3GF::Logger::INFO);
    const uint32_t COUNT = 500;
    double pi = 4;
    double sum = 0;
    double add = 1;
    for (uint32_t i = 1; i <= COUNT; ++i) {
        sum += add * (1 / i);
        add = (add == 1 ? -1 : 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    pi *= sum;
    // throw std::runtime_error("Error for task 3");
    Logger::log(std::format("Finished Task 3! Pi is {} ({} calc(s))", pi, COUNT), S3GF::Logger::INFO);
}

void task4() {
    Logger::log("Task 4 is started!", S3GF::Logger::INFO);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto _ret = DateTime::currentTimestamp();
    Logger::log(std::format("Task 4 is finished! Timestamp is {}", _ret), S3GF::Logger::INFO);
}

void task5() {
    Logger::log("Task 5 is started!", S3GF::Logger::INFO);
    SLEEP(5);
    Logger::log("Task 5 is finished!", S3GF::Logger::INFO);
}

void task6(int& k) {
    Logger::log("Task 6 is started!", S3GF::Logger::INFO);
    for (int i = 1; i < 6; ++i) {
        k += i;
    }
    Logger::log("Task 6 is finished!", S3GF::Logger::INFO);
}

int testTasksManager() {
    Logger::setBaseLogLevel(Logger::DEBUG);
    TasksManager tasks_mgr(2, 1);
    tasks_mgr.addTask(task1, S3GF::TasksManager::Low);
    tasks_mgr.addTask(task2, S3GF::TasksManager::High);
    tasks_mgr.addTask(task3, S3GF::TasksManager::High);
    tasks_mgr.addTask(task4, S3GF::TasksManager::Medium);
    tasks_mgr.addTask(task5, S3GF::TasksManager::Low);
    int k = 10;
    tasks_mgr.addTask([&k] { task6(k); }, S3GF::TasksManager::Medium);
    tasks_mgr.startAllTasks();
    tasks_mgr.wait();
    return 0;
}