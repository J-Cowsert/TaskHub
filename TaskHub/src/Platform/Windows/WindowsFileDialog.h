#pragma once

#include "FileDialog.h"
#include <windows.h>
#include <shobjidl.h> 

namespace taskhub {

    class WindowsFileDialog : public FileDialog {
    public:
        WindowsFileDialog() = default;
        virtual ~WindowsFileDialog() = default;

        std::string OpenFile() override;
    };
}