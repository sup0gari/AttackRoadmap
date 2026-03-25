#include <windows.h>
#include <iostream>
#include <fstream>

int main() {
    CreateDirectoryA("C:\\tmp", NULL);

    int count = 0;
    while (true) {
        count += 10;
        std::ofstream ofs("C:\\tmp\\count.txt", std::ios::app);
        if (ofs.is_open()) {
            ofs << count << " sec" << std::endl;
            ofs.close();
        }
        Sleep(10000);
    }
    return 0;
}
