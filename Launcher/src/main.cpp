#include "CocoriumLauncher.h"

int main(int argc, char* argv[]) {
    beyota_engine = new BeyotaCore();
    CocoriumLauncher launcher;
    launcher.Run();
    delete beyota_engine;
    return 0;
}
