#include "FileDialog.h"

#ifdef HUB_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsFileDialog.h"
#endif //HUB_PLATFORM_WINDOWS 


namespace taskhub {

    std::unique_ptr<FileDialog> FileDialog::Create() {

        #ifdef HUB_PLATFORM_WINDOWS

            return std::make_unique<WindowsFileDialog>();
        #endif //HUB_PLATFORM_WINDOWS 

        return std::unique_ptr<FileDialog>();
    }
}