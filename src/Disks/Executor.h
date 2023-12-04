#pragma once

#include <future>

class Executor {
public:
    virtual ~Executor() = default;

    virtual std::future<void> execute(std::function<void()> task) = 0;
};


class SyncExecutor : public Executor {
public:
    SyncExecutor() = default;

    std::future<void> execute(std::function<void()> task) {
        auto promise = std::make_shared<std::promise<void>>();
        try {
            task();
            promise->set_value();
        }
        catch (...) {
            try {
                promise->set_exception(std::current_exception());
            }
            catch (...) {}
        }
        return promise->get_future();
    }
};
