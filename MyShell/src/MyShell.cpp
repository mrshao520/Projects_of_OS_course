#include "MyShell.hpp"
#include <getopt.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <regex>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <pwd.h>
#include <filesystem>
#include <spdlog/spdlog.h>

MyShell::MyShell() : show_help_(false), show_version_(false),
                     builtin_str_vec_({"cd", "help", "exit"}),
                     builtin_fun_vec_({MyShell::cd, MyShell::help, MyShell::exit})
{
}

MyShell::~MyShell()
{
}

MyShell::EXIT_CODE MyShell::get_option(const int argc, char *const argv[])
{
    int opt = -1;
    int option_index = 0;
    const char *option_str = "vVhH";
    static struct option long_options[] =
        {
            {"help", no_argument, nullptr, 'h'},
            {"version", no_argument, nullptr, 'v'},
            {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, option_str, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            spdlog::debug("long option!");
            break;
        case 'v':
        case 'V':
        {
            // show version
            spdlog::debug("show version!");
            show_version_ = true;
            break;
        }
        case 'h':
        case 'H':
        {
            // show help
            spdlog::debug("show help!");
            show_help_ = true;
            break;
        }
        default:
        {
            spdlog::debug("--- default ---");
            show_usage(stderr, EXIT_CODE::EXIT_FAILURE_CODE);
            break;
        }
        }
    }

    return EXIT_CODE::EXIT_SUCCESS_CODE;
}

void MyShell::show_version()
{
    std::cout << "MyShell Version 1.0" << std::endl;
    std::cout << "Copyright (c) 2023" << std::endl;
    std::cout << std::endl;
    std::cout << "Operating system jobs written by Shao Tiancheng and others;" << std::endl;
    std::cout << "see <https://github.com/mrshao520/Projects_of_OS_course>." << std::endl;
    return;
}

void MyShell::show_usage(FILE *fp, EXIT_CODE status)
{
    if (status != EXIT_CODE::EXIT_SUCCESS_CODE)
    {
        fprintf(fp, "Usage: MyShell [GNU long option] [option]\n");
        fprintf(fp, "Try 'MyShell --help' for more information.\n");
    }
    else
    {
        fprintf(fp, "Usage: MyShell [GNU long option] [option]\n");
        printf("\
commond:\n\
        exit             退出\n\
        help             显示帮助文档\n\
        cd ...           改变当前工作目录\n\
\n\
Miscellaneous:\n\
        -V, --version    显示版本号\n\
        -H, --help       显示帮助文档\n");
    }

    return;
}

void MyShell::main_loop()
{
    int status = 0;
    char **args = nullptr;
    // int args_size = -1;
    commond_type args_list;

    do
    {
        args_list.clear();
        info();
        std::string line = read_line();
        split_line(line, TOKEN_DELIM, args_list);
        spdlog::debug("args_list size : {}", args_list.size());
        if (args_list.empty())
        {
            status = 1;
            continue;
        }
        // 函数名中含有字母“v”的函数，使用一个字符串数组指针argv指向参数列表，以NULL结束。
        args = new char *[args_list.size() + 1];
        memset(args, 0, args_list.size() + 1);
        int index = 0;
        for (auto &i : args_list)
        {
            args[index] = new char[i.size() + 1];
            memset(args[index], 0, i.size() + 1);
            strcpy(args[index], i.c_str());
            spdlog::debug("{} : {}", index, args[index]);
            index++;
        }

        status = execute_commond(args);

        for (size_t i = 0; i < args_list.size(); ++i)
        {
            delete[] args[i];
        }
        delete[] args;
        args = nullptr;

    } while (status);
}

std::string MyShell::read_line()
{
    std::string line = "";
    std::getline(std::cin, line);
    spdlog::debug("read line: {}", line);
    return line;
}

void MyShell::split_line(std::string &line, const std::string &split, commond_type &token_list)
{
    std::regex reg(split);
    std::sregex_token_iterator pos(line.begin(), line.end(), reg, -1);
    decltype(pos) end;
    for (; pos != end; ++pos)
    {
        if (pos->str() != "")
        {
            spdlog::debug("--- |{}|", pos->str());
            token_list.push_back(pos->str());
        }
    }

    return;
}

int MyShell::execute_commond(char **args)
{
    if (args[0] == nullptr)
        return 1;
    for (size_t i = 0; i < builtin_str_vec_.size(); ++i)
    {
        if (strcmp(args[0], builtin_str_vec_[i].c_str()) == 0)
        {
            return builtin_fun_vec_[i](args);
        }
    }

    return launch_commond(args);
}

int MyShell::launch_commond(char **args)
{
    pid_t pid = -1;
    pid_t wpid = -1;
    (void)(wpid);
    int status = -1;

    pid = fork();
    if (pid == 0)
    {
        // child
        if (execvp(args[0], args) == -1)
        {
            spdlog::debug("execvp error");
        }
        ::exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // father
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    else
    {
        // error
        spdlog::debug("fork error");
    }

    return 1;
}

int MyShell::cd(char **args)
{
    if (args[1] == nullptr)
    {
        fprintf(stderr, "MyShell : expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            fprintf(stderr, "MyShell : cd error\n");
        }
    }

    return 1;
}

int MyShell::help(char **args)
{
    fprintf(stderr, "Usage: MyShell [GNU long option] [option]\n");
    printf("\
commond:\n\
        exit             退出\n\
        help             显示帮助文档\n\
        cd ...           改变当前工作目录\n\
\n\
Miscellaneous:\n\
        -V, --version    显示版本号\n\
        -H, --help       显示帮助文档\n");
    return 1;
}

int MyShell::exit(char **args)
{
    return 0;
}

void MyShell::info()
{
    struct passwd *pwd = nullptr;

    pwd = getpwuid(getuid());

    fprintf(stderr, "MyShell:%s:%s#", pwd->pw_name, std::filesystem::current_path().c_str());
    return;
}