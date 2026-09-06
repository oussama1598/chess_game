#include "launcher.h"

#if defined(__linux__)
#include <filesystem>
#include <fstream>
#include <string>

namespace {
bool has_pci_vendor(const std::string &expected_vendor) {
    const std::filesystem::path devices{"/sys/bus/pci/devices"};
    std::error_code error;
    if (!std::filesystem::exists(devices, error)) return false;

    for (const auto &entry: std::filesystem::directory_iterator(devices, error)) {
        std::ifstream vendor_file(entry.path() / "vendor");
        std::string vendor;
        if (vendor_file >> vendor && vendor == expected_vendor) return true;
    }
    return false;
}

void request_high_performance_gpu() {
    if (qEnvironmentVariableIsSet("CHESS_DISABLE_GPU_OFFLOAD") ||
        qEnvironmentVariableIsSet("DRI_PRIME") ||
        qEnvironmentVariableIsSet("__NV_PRIME_RENDER_OFFLOAD")) {
        return;
    }

    std::error_code error;
    const bool proprietary_nvidia_driver =
            std::filesystem::exists("/proc/driver/nvidia/version", error);
    if (has_pci_vendor("0x10de") && proprietary_nvidia_driver) {
        qputenv("__NV_PRIME_RENDER_OFFLOAD", "1");
        qputenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia");
        qputenv("__VK_LAYER_NV_optimus", "NVIDIA_only");
    } else if (has_pci_vendor("0x10de") || has_pci_vendor("0x1002")) {
        qputenv("DRI_PRIME", "1");
    }
}
}
#elif defined(_WIN32)
#include <windows.h>

extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 1;
__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char **argv) {
#if defined(__linux__)
    request_high_performance_gpu();
#endif

    Launcher app(argc, argv);

    return app.exec();
}
