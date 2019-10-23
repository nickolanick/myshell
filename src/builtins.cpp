#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "myshell.h"
#include "builtins.h"

namespace po = boost::program_options;

int mexit(int argc, char *argv[], char *envp[]) {
    po::options_description basic_options("Options");
    basic_options.add_options()
            ("help,h", "print help message");
    po::options_description hidden_options("Hidden options");
    hidden_options.add_options()
            ("status-code", po::value<int>()->default_value(0), "status code to return");
    po::options_description options;
    options.add(basic_options).add(hidden_options);
    po::positional_options_description positional;
    positional.add("status-code", 1);
    po::variables_map vm;
    auto parsed = po::command_line_parser(argc, argv).options(options).positional(positional).run();
    po::store(parsed, vm);

    if (vm.count("help")) {
        std::cout << mexit_help_message << std::endl;
        return 0;
    } else
        exit(vm["status-code"].as<int>());
}

int mpwd(int argc, char *argv[], char *envp[], char *current_dir) {
    //TODO: use boost::filesystem instead of C-interface
    po::options_description options("Options");
    options.add_options()
            ("help,h", "print help message");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);

    if (vm.count("help"))
        std::cout << mpwd_help_message << std::endl;
    else
        std::cout << current_dir << std::endl;
    return 0;
}

int mcd(int argc, char *argv[], char *envp[], char *current_dir) {
    //TODO: use boost::filesystem instead of C-interface
    //TODO implement mcd
    if (argc == 1) return 0;
    po::options_description options("Options");
    options.add_options()
            ("help,h", "print help message");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);

    if (vm.count("help")) {
        std::cout << mcd_help_message << std::endl;
        return 0;
    }

    if (strcmp(argv[1], "~") == 0)
        //TODO: add env variable for home directory and then implement functionality for changing to home
        std::cout << "changed to home" << std::endl;
    else {
        char *path = realpath(argv[1], nullptr);
        int ret_code = chdir(path);
        if (ret_code < 0) {
            std::cout << mcd_error_message << argv[1] << std::endl;
            return ret_code;
        }
        getcwd(current_dir, MAX_PATH_LEN);
    }
    return 0;
}

int merrno(int argc, char *argv[], char *envp[], const int &errn) {
    po::options_description options("Options");
    options.add_options()
            ("help,h", "print help message");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);

    if (vm.count("help"))
        std::cout << merrno_help_message << std::endl;
    else
        std::cout << errn << std::endl;
    return 0;
}
