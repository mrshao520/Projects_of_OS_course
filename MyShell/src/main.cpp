#include <iostream>
#include "MyShell.hpp"
#include <spdlog/spdlog.h>

using namespace std;

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

    MyShell my_shell;

    my_shell.get_option(argc, argv);
    if (my_shell.get_show_help())
    {
        my_shell.show_usage(stderr, MyShell::EXIT_SUCCESS_CODE);
        exit(EXIT_SUCCESS);
    }

    if (my_shell.get_show_version())
    {
        my_shell.show_version();
        exit(EXIT_SUCCESS);
    }

    my_shell.main_loop();

    return EXIT_SUCCESS;
}