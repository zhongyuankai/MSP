#pragma once

#include <memory>
#include <Core/Types.h>
//#include <boost/noncopyable.hpp>
#include <Disks/Executor.h>
#include <filesystem>

namespace fs = std::filesystem;

//class IDiskDirectoryIterator;
//using DiskDirectoryIteratorPtr = std::unique_ptr<IDiskDirectoryIterator>;


class IReservation;

using ReservationPtr = std::shared_ptr<IReservation>;
using Reservations = std::vector<ReservationPtr>;

class WriteBufferFromFileBase;

class ReadBufferFromFileBase;

enum class WriteMode {
    Rewrite,
    Append
};

class Space : std::enable_shared_from_this<Space> {
public:
    virtual const String &getName() const = 0;

    virtual ReservationPtr reserve(UInt64 size) = 0;

    virtual ~Space() = default;
};


#define DBMS_DEFAULT_BUFFER_SIZE 1048576ULL

class IDisk : public Space {
public:
    explicit IDisk(std::unique_ptr<Executor> executor_ = std::make_unique<SyncExecutor>()) : executor(
            std::move(executor_)) {}

    virtual const String &getPath() const = 0;
//
//    virtual UInt64 getTotalSpace() const = 0;
//
//    virtual UInt64 getAvailableSpace() const = 0;
//
//    virtual UInt64 getUnreservedSpace() const = 0;
//
//    virtual UInt64 getKeepingFreeSpace() const { return 0; }

    virtual bool exists(const String &path) const = 0;

//    virtual bool isFile(const String & path) const = 0;

//    virtual bool isDirectory(const String & path) const = 0;

    virtual size_t getFileSize(const String &path) const = 0;

//    virtual void createDirectory(const String & path) const = 0;

//    virtual void createDirectories(const String & path) const = 0;

//    virtual void moveDirectory(const String & from_path, const String & to_path) = 0;

//    virtual DiskDirectoryIteratorPtr iteratorDirector(const String & path) = 0;

    virtual std::unique_ptr<ReadBufferFromFileBase> readFile(
            const String &path,
            size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE,
            size_t estimated_size = 0,
            size_t aio_threshold = 0,
            size_t mmap_threshold = 0) = 0;

    virtual std::unique_ptr<WriteBufferFromFileBase> writeFile(
            const String &path,
            size_t buffer_size = DBMS_DEFAULT_BUFFER_SIZE,
            WriteMode mode = WriteMode::Rewrite,
            size_t estimated_size = 0,
            size_t aio_threshold = 0) = 0;

private:
    std::unique_ptr<Executor> executor;
};

using DiskPtr = std::shared_ptr<IDisk>;
using Disks = std::vector<DiskPtr>;

class IReservation {
    virtual UInt64 getSize() const = 0;

    virtual DiskPtr getDisk(size_t i) const = 0;

    virtual Disks getDisks() const = 0;

    virtual void update(UInt64 new_size) = 0;

    virtual ~IReservation() = default;
};

//class IDiskDirectoryIterator
//{
//    virtual void next() = 0;
//
//    virtual bool isValid() const = 0;
//
//    virtual String path() const = 0;
//
//    virtual String name() const = 0;
//
//    virtual ~IDiskDirectoryIterator() = default;
//};
