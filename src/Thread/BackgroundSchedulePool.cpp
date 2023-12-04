//
// Created by didi on 2021/7/14.
//
#include "BackgroundSchedulePool.h"


class TaskNotification {
public:
    using Ptr = std::shared_ptr<TaskNotification>;

    explicit TaskNotification(BackgroundSchedulePoolTaskInfoPtr task_) : task(task_) {};

    ~TaskNotification() {
        std::cout << "~TaskNotification" << std::endl;
    }

    void execute() {
        task->execute();
    }

private:
    BackgroundSchedulePoolTaskInfoPtr task;
};

BackgroundSchedulePoolTaskInfo::BackgroundSchedulePoolTaskInfo(BackgroundSchedulePool &pool_,
                                                               const BackgroundSchedulePool::TaskFunc &function_)
        : pool(pool_),
          function(function_) {

}

bool BackgroundSchedulePoolTaskInfo::schedule() {
    std::lock_guard <std::mutex> lock(schedule_mutex);

    if (deactivated || scheduled) {
        return false;
    }

    scheduleImpl(lock);
    return true;
}

void BackgroundSchedulePoolTaskInfo::scheduleImpl(std::lock_guard <std::mutex> &schedule_mutex_lock) {
    scheduled = true;

    if (!executing)
        pool.queue.put(std::make_shared<TaskNotification>(shared_from_this()));
}

void BackgroundSchedulePoolTaskInfo::activate() {
    std::lock_guard <std::mutex> locker(schedule_mutex);
    deactivated = false;
}

bool BackgroundSchedulePoolTaskInfo::activateAndSchedule() {
    std::lock_guard <std::mutex> locker(schedule_mutex);
    deactivated = false;

    if (scheduled) {
        return false;
    }
    scheduleImpl(locker);

    return true;
}

void BackgroundSchedulePoolTaskInfo::deactivate() {
    std::lock_guard <std::mutex> lock_exec(exec_mutex);
    std::lock_guard <std::mutex> lock_schedule(schedule_mutex);

    if (deactivated) {
        return;
    }

    deactivated = true;
    scheduled = false;

// delay
}

void BackgroundSchedulePoolTaskInfo::execute() {
    std::lock_guard <std::mutex> lock_exec(exec_mutex);

    {
        std::lock_guard <std::mutex> lock_schedule(schedule_mutex);

        if (deactivated)
            return;

        scheduled = false;
        executing = true;
    }

    function();

    {
        std::lock_guard <std::mutex> lock_schedule(schedule_mutex);

        executing = false;

        if (scheduled)
            pool.queue.put(std::make_shared<TaskNotification>(shared_from_this()));
    }
}

BackgroundSchedulePool::BackgroundSchedulePool(size_t size_) :
        size(size_) {
    threads.resize(size);
    for (auto & thread : threads) {
        thread = std::thread([this] {
            threadFunction();
        });

    }
}

void BackgroundSchedulePool::threadFunction() {
    while (true) {
        if (Notification_Ptr notification = queue.take()) {
            notification->execute();
        }
    }
}

BackgroundSchedulePool::TaskHolder BackgroundSchedulePool::createTask(const TaskFunc task_func) {
    return TaskHolder(std::make_shared<TaskInfo>(*this, task_func));
}