#include <Disks/DiskLocal.h>


bool DiskLocal::exists(const String &path) const {
    return fs::exists(fs::path(disk_path) / path);
}

size_t DiskLocal::getFileSize(const String &path) const {
    return fs::file_size(fs::path(disk_path) / path);
}
