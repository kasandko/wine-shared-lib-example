#include <iostream>
#include <cstdint>
#include <windows.h>

typedef int32_t (__stdcall *Func)(int32_t, int32_t);

std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    
    HINSTANCE h_ntdll = GetModuleHandle("ntdll.dll");
    if (!h_ntdll)
    {
        std::cout << "Failed load ntdll." << std::endl;
        return 0;
    }
    
    HINSTANCE hinstance = LoadLibrary("sharedlib.dll");
    if (!hinstance)
    {
        std::cout << "Failed load library: " << GetLastErrorAsString() << std::endl;
        return 1;
    }
    
    Func function = (Func) GetProcAddress(hinstance, "diff");
    if (!function)
    {
        std::cout << "Failed get procedure addres: " << GetLastErrorAsString() << std::endl;
        FreeLibrary(hinstance);
        return 2;
    }
    
    int32_t r = function(42, 6);
    std::cout << "Result: " << r << std::endl;
    
    FreeLibrary(hinstance);
    
    return 0;
}
