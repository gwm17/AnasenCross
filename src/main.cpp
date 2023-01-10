#include "Application.h"
#include "fmt/core.h"
#include "fmt/format.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fmt::print("AnasenCross must have a config file!\n");
        fmt::print("Run like: ./bin/AnasenCross config.json");
        return 1;
    }

    fmt::print("-----------AnasenCross-----------\n");
    fmt::print("Initializing application from config file {}...\n", argv[1]);
    AnasenCross::Application app(argv[1]);
    if(!app.IsInit())
    {
        fmt::print("Failed to initialize application from config file.\n");
        return 1;
    }
    fmt::print("Application successfully configured. Running cross section analysis...\n");
    app.Run();
    fmt::print("Complete.\n");
    return 0;
}