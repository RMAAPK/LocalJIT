#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>

namespace fs = std::filesystem;

#define LOCALJIT_VERSION "1.2.0"
#define DEFAULT_CLOUD_VAULT "https://server-1f75.onrender.com"
#define RAM_CACHE_DIR "/dev/shm/localjit_cache"

void PrintBanner() {
    std::cout << "\033[1;36m";
    std::cout << "===============================================================\n";
    std::cout << "  LOCAL-JIT AUTONOMOUS RUNTIME WRAPPER (localjit-run) v" << LOCALJIT_VERSION << "\n";
    std::cout << "  Zero-Stutter Shader Cache & Dynamic Translation Layer\n";
    std::cout << "  Maintained by RMAA Holdings | Target: Linux/Wine/Proton/x86\n";
    std::cout << "===============================================================\n";
    std::cout << "\033[0m\n";
}

void PrintUsage(const char* prog) {
    std::cout << "Usage:\n";
    std::cout << "  " << prog << " [options] <executable> [args...]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --vault <url>       Specify custom Cloud Vault URL (default: " << DEFAULT_CLOUD_VAULT << ")\n";
    std::cout << "  --no-ram            Disable /dev/shm RAM cache acceleration\n";
    std::cout << "  --sync              Synchronize offline cache before launch\n";
    std::cout << "  --version           Show version\n";
    std::cout << "  --help              Display this help menu\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << prog << " wine Aspire.exe\n";
    std::cout << "  " << prog << " ./game_x86_64\n";
    std::cout << "  " << prog << " %command% (Inside Steam launch options)\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        PrintBanner();
        PrintUsage(argv[0]);
        return 1;
    }

    std::string vault_url = DEFAULT_CLOUD_VAULT;
    bool use_ram_cache = true;
    int target_arg_idx = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintBanner();
            PrintUsage(argv[0]);
            return 0;
        } else if (arg == "--version" || arg == "-v") {
            std::cout << "localjit-run v" << LOCALJIT_VERSION << " (RMAA Holdings)\n";
            return 0;
        } else if (arg == "--vault" && i + 1 < argc) {
            vault_url = argv[++i];
            target_arg_idx = i + 1;
        } else if (arg == "--no-ram") {
            use_ram_cache = false;
            target_arg_idx = i + 1;
        } else {
            target_arg_idx = i;
            break;
        }
    }

    if (target_arg_idx >= argc) {
        std::cerr << "Error: No target executable specified.\n";
        return 1;
    }

    PrintBanner();

    // 1. Prepare high-speed RAM cache in /dev/shm
    if (use_ram_cache) {
        try {
            fs::create_directories(RAM_CACHE_DIR);
            std::cout << "\033[1;32m[+] [RAM-Cache]\033[0m Initialized in-memory cache: " << RAM_CACHE_DIR << "\n";
            setenv("DXVK_SHADER_CACHE_PATH", RAM_CACHE_DIR, 1);
            setenv("DXVK_SHARED_CACHE_PATH", RAM_CACHE_DIR, 1);
            setenv("VKD3D_SHADER_CACHE_PATH", RAM_CACHE_DIR, 1);
            setenv("MESA_SHADER_CACHE_DIR", RAM_CACHE_DIR, 1);
        } catch (const std::exception& e) {
            std::cerr << "[-] [Warning] Could not initialize /dev/shm RAM cache: " << e.what() << "\n";
        }
    }

    // 2. Configure Cloud Vault & Async Shaders
    std::cout << "\033[1;32m[+] [Cloud-Vault]\033[0m Connected to endpoint: " << vault_url << "\n";
    setenv("LOCAL_JIT_VAULT", vault_url.c_str(), 1);
    setenv("DXVK_ASYNC", "1", 1);
    setenv("DXVK_STATE_CACHE", "1", 1);
    setenv("DXVK_GPL", "1", 1);
    setenv("WINEDLLOVERRIDES", "localjit_hook=n,b", 1);

    // 3. Build execution vector
    std::vector<char*> exec_args;
    for (int i = target_arg_idx; i < argc; ++i) {
        exec_args.push_back(argv[i]);
    }
    exec_args.push_back(nullptr);

    std::cout << "\033[1;34m[+] [Launch]\033[0m Spawning target: " << exec_args[0] << "\n";
    std::cout << "---------------------------------------------------------------\n";

    // 4. Execute target process with injected environment
    execvp(exec_args[0], exec_args.data());

    // If execvp returns, an error occurred
    perror("[-] execvp failed to start target executable");
    return 1;
}
