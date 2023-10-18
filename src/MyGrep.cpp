#include "MyGrep.hpp"
#include <getopt.h>
#include <string.h>
#include <string>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <regex>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

MyGrep::MyGrep() : ignore_case_(false), display_number_(false),
                   display_match_number_(false),
                   reverse_lookup_(false),
                   file_(false), directory_(false), recurrence_(false),
                   compare_string_(""), without_filename_(false),
                   with_filename_(false), show_help_(0),
                   show_version_(false), compare_regex_("")
{
}

MyGrep::~MyGrep()
{
}

auto MyGrep::get_option(const int argc, char *const argv[]) -> int
{
        int opt = -1;
        int option_index = 0;
        const char *option_str = "incvf:d:rhHVs:";
        static struct option long_options[] =
            {
                {"help", no_argument, &show_help_, 1},
                {"version", no_argument, nullptr, 'V'},
                {0, 0, 0, 0}};

        while ((opt = getopt_long(argc, argv, option_str, long_options, &option_index)) != -1)
        {
                switch (opt)
                {
                case 0:
                        spdlog::debug("long option!");
                        break;
                case 's':
                        spdlog::debug("compare string : {}", optarg);
                        compare_string_ = optarg;
                        break;
                case 'V':
                        // show version
                        spdlog::debug("show version!");
                        show_version_ = true;
                        break;
                case 'i':
                        // ignore case
                        spdlog::debug("ignore case!");
                        ignore_case_ = true;
                        break;
                case 'n':
                        // display number
                        spdlog::debug("display number!");
                        display_number_ = true;
                        break;
                case 'c':
                        // display match number
                        spdlog::debug("display match number!");
                        display_match_number_ = true;
                        break;
                case 'v':
                        // reverse lookup
                        spdlog::debug("reverse lookup");
                        reverse_lookup_ = true;
                        break;
                case 'r':
                        spdlog::debug("recurrence!");
                        recurrence_ = true;
                        break;
                case 'h':
                        spdlog::debug("without filename!");
                        without_filename_ = true;
                        with_filename_ = false;
                        break;
                case 'H':
                        spdlog::debug("with filename!");
                        with_filename_ = true;
                        without_filename_ = false;
                        break;
                case 'f':
                {
                        spdlog::debug("file!");

                        file_ = true;

                        // spdlog::debug("index : {}", optind);
                        // spdlog::debug("file name : {}", optarg);

                        int tmp = optind - 1;
                        while (tmp < argc && strncmp(argv[tmp], "-", 1) != 0)
                        {
                                if (std::filesystem::is_regular_file(argv[tmp]))
                                {
                                        spdlog::debug("file name : {}", argv[tmp]);
                                        file_list_.emplace_back(argv[tmp]);
                                }
                                else
                                {
                                        spdlog::info("{} is not a regular file!", argv[tmp]);
                                }
                                ++tmp;
                        }

                        if (file_list_.empty())
                        {
                                spdlog::info("empty file list!");
                                file_ = false;
                        }

                        break;
                }
                case 'd':
                {
                        spdlog::debug("directory!");

                        directory_ = true;
                        int tmp = optind - 1;
                        while (tmp < argc && strncmp(argv[tmp], "-", 1) != 0)
                        {
                                if (std::filesystem::is_directory(argv[tmp]))
                                {
                                        spdlog::debug("directory name : {}", argv[tmp]);
                                        spdlog::debug("the abslute path is {}", std::filesystem::absolute(argv[tmp]).string());
                                        directory_list_.emplace_back(std::filesystem::absolute(argv[tmp]).string());
                                }
                                else
                                {
                                        spdlog::info("{} is not a directory!", argv[tmp]);
                                }
                                ++tmp;
                        }

                        if (directory_list_.empty())
                        {
                                spdlog::debug("empty directory list!");
                                directory_ = false;
                        }

                        break;
                }
                default:
                        spdlog::debug("--- default ---");
                        show_usage(EXIT_CODE::EXIT_FAILURE_CODE);
                        break;
                }
        }

        if (ignore_case_)
        {
                compare_regex_ = std::regex(compare_string_, std::regex::grep | std::regex::icase);
        }
        else
        {
                compare_regex_ = std::regex(compare_string_, std::regex::grep);
        }

        return EXIT_CODE::EXIT_FAILURE_CODE;
}

auto MyGrep::get_show_version() -> bool
{
        return show_version_;
}

auto MyGrep::get_show_help() -> bool
{
        return show_help_;
}

void MyGrep::show_version()
{
        std::cout << "MyGrep Version 1.0" << std::endl;
        std::cout << "Copyright (c) 2023" << std::endl;
        std::cout << std::endl;
        std::cout << "Operating system jobs written by Shao Tiancheng and others;" << std::endl;
        std::cout << "see <https://github.com/mrshao520/MyGrep>." << std::endl;
}

void MyGrep::show_usage(EXIT_CODE status)
{
        if (status != EXIT_CODE::EXIT_SUCCESS_CODE)
        {
                std::cout << "Usage: MyGrep [OPTION]... -s [PATTERNS] -f [FILE] -d [DIRECTORY]..." << std::endl;
                std::cout << "Try 'MyGrep --help' for more information." << std::endl;
        }
        else
        {
                std::cout << "Usage: MyGrep [OPTION]... -s [PATTERNS] -f [FILE] -d [DIRECTORY]..." << std::endl;
                std::cout << "Search for PATTERNS in each FILE." << std::endl;
                printf("\
Example: MyGrep -i -s 'hello world' menu.h main.c\n\
PATTERNS can contain multiple patterns separated by newlines.\n\
\n\
Pattern selection and interpretation:\n\
        -i 忽略大小写进行匹配\n\
        -n 显示匹配行的行号\n\
        -c 只打印匹配的行数\n\
        -v 反向查找\n\
        -d 查找的目录\n\
        -r 递归查找子目录中的文件\n\
        -s 匹配的字符串\n\
        -h 在显示符合样式的那一行之前，不标示该行所属的文件名称\n\
        -H 在显示符合样式的那一行之前，表示该行所属的文件名称\n\
\n\
Miscellaneous:\n\
        -V, --version    显示版本号\n\
            --help       显示帮助文档\n");
        }

        exit(status);
}

auto MyGrep::match_from_pipe() -> int
{
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int buffer_size = sizeof(buffer) - 1;
        unsigned long lineno = 0;       // 行号
        unsigned long match_number = 0; // 匹配的行数
        std::string file_name = "(standard input)";

        if (file_ || directory_)
        {
                spdlog::debug("ignore the standard input!");
                return EXIT_CODE::EXIT_FAILURE_CODE;
        }

        while ((fgets(buffer, buffer_size, stdin)) != nullptr)
        {
                lineno++;
                /*(standard input)*/
                if (!reverse_lookup_)
                {
                        if (std::regex_search(buffer, compare_regex_))
                        {
                                if (display_match_number_)
                                {
                                        match_number++;
                                }
                                else
                                {
                                        std::cout << to_string(buffer, file_name, lineno, false);
                                }
                        }
                }
                else
                {
                        if (!std::regex_search(buffer, compare_regex_))
                        {
                                if (display_match_number_)
                                {
                                        match_number++;
                                }
                                else
                                {
                                        std::cout << to_string(buffer, file_name, lineno, false);
                                }
                        }
                }
        }

        if (display_match_number_)
        {
                std::cout << to_string(std::to_string(match_number).c_str(),
                                       file_name, lineno, false)
                          << std::endl;
        }

        return EXIT_CODE::EXIT_SUCCESS_CODE;
}

auto MyGrep::match_from_file() -> int
{
        if (!file_)
                return EXIT_CODE::EXIT_FAILURE_CODE;

        return match_from_file(file_list_);
}

auto MyGrep::match_from_file(const std::list<std::string> &file_list) -> int
{
        std::ifstream file_stream;
        std::string line;
        unsigned long lineno = 0;       // 行号
        unsigned long match_number = 0; // 匹配的行数
        bool multifile = (file_list.size() > 1 ? true : false);

        for (const auto &file : file_list)
        {
                file_stream.open(file, std::ios_base::in);
                if (!file_stream.good())
                {
                        spdlog::debug("can't open the file {}", file);
                        continue;
                }

                lineno = 0;
                match_number = 0;

                while (std::getline(file_stream, line))
                {
                        lineno++;

                        if (!reverse_lookup_)
                        {
                                if (std::regex_search(line, compare_regex_))
                                {
                                        if (display_match_number_)
                                        {
                                                match_number++;
                                        }
                                        else
                                        {
                                                std::cout << to_string(line.c_str(), file, lineno, multifile) << std::endl;
                                                ;
                                        }
                                }
                        }
                        else
                        {
                                if (!std::regex_search(line, compare_regex_))
                                {
                                        if (display_match_number_)
                                        {
                                                match_number++;
                                        }
                                        else
                                        {
                                                std::cout << to_string(line.c_str(), file, lineno, multifile) << std::endl;
                                        }
                                }
                        }
                }

                if (display_match_number_)
                {
                        std::cout << to_string(std::to_string(match_number).c_str(),
                                               file, lineno, multifile)
                                  << std::endl;
                }

                file_stream.close();
        }

        return EXIT_CODE::EXIT_SUCCESS_CODE;
}

auto MyGrep::match_from_directory() -> int
{
        DIR *directory = nullptr;
        struct dirent *next_file = nullptr;
        std::list<std::string> file_list;

        if (!directory_)
                return EXIT_CODE::EXIT_FAILURE_CODE;

        for (const auto &dir_name : directory_list_)
        {
                directory = opendir(dir_name.c_str());
                if (directory == nullptr)
                {
                        spdlog::debug("can't open the directory {}", dir_name);
                        return EXIT_CODE::EXIT_FAILURE_CODE;
                }

                while ((next_file = readdir(directory)) != nullptr)
                {
                        if (next_file->d_name[0] == '.')
                        {
                                /* 忽略 . .. 以及 以.开头的隐藏文件 */
                                continue;
                        }

                        spdlog::debug("{} : file name is {} ", dir_name, next_file->d_name);

                        if (std::filesystem::is_regular_file(next_file->d_name))
                        {
                                file_list.emplace_back(next_file->d_name);
                        }
                        else if (std::filesystem::is_directory(next_file->d_name) && recurrence_)
                        {
                                directory_list_.emplace_back(next_file->d_name);
                        }
                }

                closedir(directory);
        }

        return match_from_file(file_list);;
}

auto MyGrep::to_string(const char *ch, const std::string &filename, unsigned long lineno, bool multifile) -> std::string
{
        std::string ret = "";
        if (with_filename_ || (multifile && !without_filename_))
        {
                /*-H 显示文件名 or 多文件默认显示文件名*/
                ret = ret + ":" + filename;
        }

        if (display_number_)
        {
                ret = ret + ":" + std::to_string(lineno);
        }

        ret = ret + ":" + ch;

        return ret;
}
