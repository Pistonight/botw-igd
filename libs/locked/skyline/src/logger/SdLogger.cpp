#include "skyline/logger/SdLogger.hpp"

#include "nn/fs.h"

namespace skyline::logger {
nn::fs::FileHandle fileHandle;
s64 offset;

SdLogger::SdLogger(std::string path) {
    nn::fs::DirectoryEntryType type;
    nn::Result result = nn::fs::GetEntryType(&type, path.c_str());

    if (result.GetInnerValueForDebug() == 0x202) {  // Path does not exist
        nn::fs::CreateFile(path.c_str(), 0);
    } else if (result.IsFailure())
        return;

    if (type == nn::fs::DirectoryEntryType_Directory) return;

    NN_ERRORONFAIL(nn::fs::OpenFile(&fileHandle, path.c_str(), nn::fs::OpenMode_ReadWrite | nn::fs::OpenMode_Append));
}

void SdLogger::Initialize() {
    // nothing to do
}

void SdLogger::SendRaw(void* data, size_t size) {
    nn::fs::SetFileSize(fileHandle, offset + size);

    nn::fs::WriteFile(fileHandle, offset, data, size,
                      nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));

    offset += size;
};
};  // namespace skyline::logger