#pragma once

#include <string>
#include <list>
#include <queue>
#include <regex>

class MyGrep
{
public:
    enum EXIT_CODE : int
    {

        EXIT_SUCCESS_CODE = 0,
        EXIT_FAILURE_CODE = 1
    };

    MyGrep();
    ~MyGrep();

    /**
     * parse command line parameters
     */
    auto get_option(const int argc, char *const argv[]) -> int;

    /**
     * get show_version_
     */
    auto get_show_version() -> bool;

    /**
     * get show_help_
     */
    auto get_show_help() -> bool;

    /**
     * to show version
     */
    void show_version();

    /**
     * show usage
     *
     */
    void show_usage(EXIT_CODE status);

    /**
     * get string from pipe and match with compare_string_
     * such as : ls -a | MyGrep -s compare_string_
     */
    auto match_from_pipe() -> int;

    /**
     * read line from file and match with compare_string_
     * such as : MyGrep -s compare_string -f file.txt
     */
    auto match_from_file() -> int;

    auto match_from_file(const std::list<std::string> &file_list) -> int;

    /**
     * read line from file in the specified directory and match with compare_string_
     * such as : MyGrep -s compare_string -d directory
     */
    auto match_from_directory() -> int;

    /**
     * concat string by line number and filename
     */
    auto to_string(const char *ch, const std::string &filename, unsigned long lineno, bool multifile) -> std::string;

private:
    bool ignore_case_;           // -i 忽略大小写进行匹配
    bool display_number_;        // -n 显示匹配行的行号
    bool display_match_number_;  // -c 只打印匹配的行数,
    bool reverse_lookup_;        // -v 反向查找
    bool file_;                  // -f 查找的文件
    bool directory_;             // -d 查找的目录
    bool recurrence_;            // -r 递归查找子目录中的文件
    std::string compare_string_; // -s

    /* 最后一个 h 或 H 才会生效，h 和 H 互斥 */
    bool without_filename_; // -h 在显示符合样式的那一行之前，不标示该行所属的文件名称。
    bool with_filename_;    // -H 在显示符合样式的那一行之前，表示该行所属的文件名称。多文件默认调用-H

    int show_help_;     // --help
    bool show_version_; // --version -V

    std::list<std::string> file_list_;      // 保存文件名
    std::list<std::string> directory_list_; // 保存目录名

    std::regex compare_regex_;
};
