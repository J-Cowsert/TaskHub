#include "WindowsFileDialog.h"
#include <locale>
#include <codecvt>
#include "Core/Log.h"

namespace taskhub {

    namespace Utils {

        std::string WStrToStr(const std::wstring& wstr) {

            if (wstr.empty()) return std::string();

            int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);

            std::string str(sizeNeeded, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), &str[0], sizeNeeded, NULL, NULL);

            return str;
        }
    }

    std::string taskhub::WindowsFileDialog::OpenFile() {


        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr)) {

            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr)) {
                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr)) {

                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr)) {

                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Return the file name to the user.
                        if (SUCCEEDED(hr)) {
                            return Utils::WStrToStr(pszFilePath);
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
        return std::string();
    }
}