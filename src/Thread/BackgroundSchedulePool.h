#pragma once

#include <memory>
#include <functional>
#include <mutex>
#include "BlockingQueue.cpp"
#include <vector>
#include <thread>


class TaskNotification;
class BackgroundSchedulePoolTaskHolder;
class BackgroundSchedulePoolTaskInfo;
class BackgroundSchedulePool {
public:
    friend class BackgroundSchedulePoolTaskInfo;
    using TaskFunc = std::function<void()>;
    using Notification_Ptr = std::shared_ptr<TaskNotification>;
    using NotificationQueue = BlockingQueue<std::shared_ptr<TaskNotification>>;
    using Threads = std::vector<std::thread>;
    using TaskHolder = BackgroundSchedulePoolTaskHolder;
    using TaskInfo = BackgroundSchedulePoolTaskInfo;
    BackgroundSchedulePool(size_t size_);

    TaskHolder createTask(const TaskFunc task_func);
private:
    void threadFunction();

    Threads threads;
    NotificationQueue queue;
    size_t size;
};

class BackgroundSchedulePoolTaskInfo : public std::enable_shared_from_this<BackgroundSchedulePoolTaskInfo> {
public:
    BackgroundSchedulePoolTaskInfo(BackgroundSchedulePool & pool_, const BackgroundSchedulePool::TaskFunc & function_);

    bool schedule();

    bool scheduleAfter(size_t ms, bool overwrite = true);

    void deactivate();

    void activate();

    bool activateAndSchedule();
private:
    friend class TaskNotification;
    void execute();

    void scheduleImpl(std::lock_guard<std::mutex> & schedule_mutex_lock);

    BackgroundSchedulePool & pool;
    BackgroundSchedulePool::TaskFunc function;

    std::mutex exec_mutex;
    std::mutex schedule_mutex;

    bool deactivated = false;
    bool scheduled = false;
    bool delayed = false;
    bool executing = false;
};

using BackgroundSchedulePoolTaskInfoPtr = std::shared_ptr<BackgroundSchedulePoolTaskInfo>;

class BackgroundSchedulePoolTaskHolder
{
public:
    BackgroundSchedulePoolTaskHolder() = default;
    explicit BackgroundSchedulePoolTaskHolder(const BackgroundSchedulePoolTaskInfoPtr & task_info_) : task_info(task_info_){}
    BackgroundSchedulePoolTaskHolder(const BackgroundSchedulePoolTaskHolder & other) = delete;
    BackgroundSchedulePoolTaskHolder(BackgroundSchedulePoolTaskHolder && other) noexcept = default;
    BackgroundSchedulePoolTaskHolder & operator=(const BackgroundSchedulePoolTaskHolder & other) = delete;
    BackgroundSchedulePoolTaskHolder & operator=(BackgroundSchedulePoolTaskHolder && other) noexcept = default;

    ~BackgroundSchedulePoolTaskHolder()
    {
        if (task_info)
            task_info->deactivate();
    }

    operator bool() { return task_info != nullptr;}

    BackgroundSchedulePoolTaskInfo * operator->(){ return task_info.get();}
    const BackgroundSchedulePoolTaskInfo * operator->() const { return task_info.get(); }
private:
    BackgroundSchedulePoolTaskInfoPtr task_info;
};

