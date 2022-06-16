#include "skyline/utils/cpputils.hpp"

#include "nn/nn.h"
#include "skyline/utils/utils.h"

namespace skyline {

std::string utils::g_RomMountStr = "rom:/";

u64 utils::g_MainTextAddr;
u64 utils::g_MainRodataAddr;
u64 utils::g_MainDataAddr;
u64 utils::g_MainBssAddr;
u64 utils::g_MainHeapAddr;

void utils::init() {
    // find .text
    utils::g_MainTextAddr =
        memGetMapAddr((u64)nninitStartup);  // nninitStartup can be reasonably assumed to be exported by main
    // find .rodata
    utils::g_MainRodataAddr = memNextMap(utils::g_MainTextAddr);
    // find .data
    utils::g_MainDataAddr = memNextMap(utils::g_MainRodataAddr);
    // find .bss
    utils::g_MainBssAddr = memNextMap(utils::g_MainDataAddr);
    // find heap
    utils::g_MainHeapAddr = memNextMapOfType(utils::g_MainBssAddr, MemType_Heap);

}

bool endsWith(std::string const& str1, std::string const& str2) {
    return str2.size() <= str1.size() && str1.find(str2, str1.size() - str2.size()) != str1.npos;
}

void* utils::getRegionAddress(skyline::utils::region region) {
    switch (region) {
        case region::Text:
            return (void*)g_MainTextAddr;
        case region::Rodata:
            return (void*)g_MainRodataAddr;
        case region::Data:
            return (void*)g_MainDataAddr;
        case region::Bss:
            return (void*)g_MainBssAddr;
        case region::Heap:
            return (void*)g_MainHeapAddr;
        default:
            return NULL;
    }
}
};  // namespace skyline
