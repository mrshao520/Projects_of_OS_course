#pragma once

#include <stdio.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <string>
#include <functional>

// #define TOKEN_BUFSIZE 64
// #define TOKEN_DELIM ".\t\r\n\a"

const std::string TOKEN_DELIM = "\\s+";

class MyShell
{
public:
    using commond_type = std::list<std::string>;
    using builtin_type = std::vector<std::string>;
    using funct_type = std::vector<std::function<int(char **)>>;

    enum EXIT_CODE : int
    {
        EXIT_SUCCESS_CODE = 0,
        EXIT_FAILURE_CODE = 1,
    };

    MyShell();
    ~MyShell();

    /**
     * @brief parse command line parameters
     */
    EXIT_CODE get_option(const int argc, char *const argv[]);

    /**
     * @brief get show_version_
     */
    constexpr bool get_show_version()
    {
        return show_version_;
    }

    /**
     * @brief get show_help_
     */
    constexpr bool get_show_help()
    {
        return show_help_;
    }

    /**
     * @brief to show version
     */
    void show_version();

    /**
     * @brief to show usage
     * @param fp : stdin or stderr
     * @param status : success or failure
     */
    void show_usage(FILE *fp, EXIT_CODE status);

    /**
     * @brief exit shell
     * @param status : success or failure
     */
    void exit_shell(EXIT_CODE status)
    {
        fflush(stdout);
        fflush(stderr);

        ::exit(status);
    }

    /**
     * @brief loop getting input and executing it
     */
    void main_loop();

    /**
     * @brief read a line from stdin
     * @return the line from stdin
     */
    std::string read_line();

    /**
     * @brief split a line into tokens
     * @param line : the line
     */
    void split_line(std::string &line, const std::string &split, commond_type &token_list);

    /**
     * @brief execute shell built-in or launch program
     */
    int execute_commond(char **args);

    /**
     * @brief launch a program and wait for it to terminate
     */
    int launch_commond(char **args);

    /**
     * @brief 改变当前工作目录
     * @param 命令行参数
     */
    static int cd(char **args);

    /**
     * @brief 显示帮助文档
     */
    static int help(char **args);

    /**
     * @brief 退出
     */
    static int exit(char **args);

    /**
     * @brief printf info such as MyShell:root:/usr/local
     */
    void info();

private:
    bool show_help_;
    bool show_version_;

    builtin_type builtin_str_vec_;
    funct_type builtin_fun_vec_;
};