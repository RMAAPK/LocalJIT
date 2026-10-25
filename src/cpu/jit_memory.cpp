#include "polywarp/cpu_engine.hpp"
#include <sys/mman.h>
#include <cstring>
#include <stdexcept>

namespace PolyWarp {

CPUEngine::CPUEngine() {
    // Allocate RWX page
    m_exec_page = mmap(
        nullptr,
        m_page_size,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_ANONYMOUS | MAP_PRIVATE,
        -1,
        0
    );
    if (m_exec_page == MAP_FAILED) {
        m_exec_page = nullptr;
    }
}

CPUEngine::~CPUEngine() {
    if (m_exec_page) {
        munmap(m_exec_page, m_page_size);
        m_exec_page = nullptr;
    }
}

CPUEngine::EntryFunc CPUEngine::PrepareExecutableBlock(const std::vector<uint8_t>& x86_bytes) {
    if (!m_exec_page || x86_bytes.empty() || x86_bytes.size() > m_page_size) {
        return nullptr;
    }

    std::memcpy(m_exec_page, x86_bytes.data(), x86_bytes.size());

    // Cast buffer directly to callable function pointer
    return reinterpret_cast<EntryFunc>(m_exec_page);
}

} // namespace PolyWarp

