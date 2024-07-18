#include "WindowsFileDialog.h"
#include <locale>
#include <codecvt>
#include "Core/Log.h"

namespace taskhub {

    namespace Utils {

        std::string WStrToStr(const std::wstring& wstr) {

            if (wstr.empty()) return std::string();

            int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);

            std::string str(sizeNeeded, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), &str[0], sizeNeeded, nullptr, nullptr);

            return str;
        }
    }

    std::string taskhub::WindowsFileDialog::OpenFile() {


        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr)) {

            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr)) {
                // Show the Open dialog box.
                hr = pFileOpen->Show(nullptr);

                if (SUCCEEDED(hr)) {

                    // Get the file name from the dialog box.
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr)) {

                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Return the file path to the user.
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


    std::string WindowsFileDialog::OpenFolder() {

        HRESULT  hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr)) {

            IFileOpenDialog* pFileDialog;

            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));
            
            if (SUCCEEDED(hr)) {
                DWORD dwOptions;
                hr = pFileDialog->GetOptions(&dwOptions);

                if (SUCCEEDED(hr)) {
                    // Set FOS_PICKFOLDERS to indicate we want to pick a folder.
                    hr = pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

                    if (SUCCEEDED(hr)) {

                        hr = pFileDialog->Show(nullptr);

                        if (SUCCEEDED(hr)) {
                            IShellItem* pItem;
                            hr = pFileDialog->GetResult(&pItem);

                            if (SUCCEEDED(hr)) {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Return the folder path to the user.
                                if (SUCCEEDED(hr)) {
                                    return Utils::WStrToStr(pszFilePath);

                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                    }
                }
            }
            pFileDialog->Release();
            CoUninitialize();
        }

        return std::string();
    }
}