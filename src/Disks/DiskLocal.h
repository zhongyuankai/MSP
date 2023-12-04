#pragma once

#include <Disks/IDisk.h>

namespace ErrorCodes {
    extern const int LOGICAL_ERROR;
}

class DiskLocal : public IDisk {
public:
    DiskLocal(const String &name_, const String &disk_path_, const UInt64 keep_free_space_bytes_)
            : name(name_), disk_path(disk_path_), keep_free_space_bytes(keep_free_space_bytes_) {
//        if (disk_path.back() == '/')
//            throw Exception("Disk path must ends with '/', but '" + disk_path + "' doesn't,", ErrorCodes::LOGICAL_ERROR);
    }

    const String &getPath() const override { return disk_path; }

    bool exists(const String &path) const override;

    size_t getFileSize(const String &path) const override;

private:
    const String name;
    const String disk_path;
    const UInt64 keep_free_space_bytes;

};

