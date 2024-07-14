#include "FileDialog.h"
#include "Core/Assert.h"

#ifdef HUB_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsFileDialog.h"
#endif //HUB_PLATFORM_WINDOWS 


namespace taskhub {

    std::unique_ptr<FileDialog> FileDialog::Create() {

    #ifdef HUB_PLATFORM_WINDOWS
        return std::make_unique<WindowsFileDialog>();
    #else
        HUB_CORE_ASSERT(false, "Platform unknown.");
        return nullptr;
    #endif
    }
}