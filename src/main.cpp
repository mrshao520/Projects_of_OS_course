#include <iostream>
#include <spdlog/spdlog.h>
#include "MyGrep.hpp"
#include <filesystem>

using namespace std;

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

    MyGrep my_grep;
    my_grep.get_option(argc, argv);

    if (my_grep.get_show_version())
    {
        my_grep.show_version();
        return MyGrep::EXIT_SUCCESS_CODE;
    }

    if (my_grep.get_show_help())
    {
        my_grep.show_usage(MyGrep::EXIT_SUCCESS_CODE);
    }

    my_grep.match_from_pipe();

    my_grep.match_from_file();

    my_grep.match_from_directory();

    return 1;
}