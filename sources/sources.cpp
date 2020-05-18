// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <process.h>
#include <constants.h>

int exit_code;
std::vector <pid_t> processes;
bool pack = false;
bool done = false;
bool install = false;
int seconds = 0;
bool conf = true;

int main(int argc, char **argv) {
    std::string config;
    try {
        boost::program_options
        ::options_description desc("Allowed options");
        desc.add_options()
                ("config", boost::program_options
                ::value<std::string>(), "configuration")
                ("pack", "package")
                ("install", "_install")
                ("timeout", boost::
                program_options::value<int>(), "timing");

        boost::
        program_options::variables_map vm;
        store
        (parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("config")) {
            config = vm["config"].as<std::string>();
            if (config != "Debug")
            {
              conf = false;
            }
            std::cout << "config:" << config << std::endl;
        } else {
            config = "Debug";
        }
        if (vm.count("pack")) {
            std::cout << "package" << std::endl;
            pack = true;
        } else {
            pack = false;
        }
        if (vm.count("install")) {
            install = true;
            std::cout << "install" << std::endl;
        } else {
            install = false;
        }
        if (vm.count("timeout")) {
            seconds = vm["timeout"].as<int>();
        } else {
            seconds = 0;
        }
    }
    catch (...) {
        std::cout << "lazha" << "\n";
    }
    if (seconds) {
        auto task = async::spawn([] {
            sleep(seconds);
            for (const auto &a : processes) {
                kill(a, SIGINT);
            }
        });
    }
    auto task1 = async::spawn([] {
        processes.push_back
        (boost::this_process::get_id());
        if (conf) {
            std::string tmp = std::string(building) +
                              std::string(type1);
            exit_code = boost::process::system
                    (tmp);
        } else {
            std::string tmp = std::string(building) +
                              std::string(type2);
            exit_code = boost::process::system
                    (tmp);
        }
        processes.pop_back();
    });
    auto task2 = task1.then([] {
        if ((install) && (exit_code == 0)) {
            processes.push_back
            (boost::this_process::get_id());
            exit_code = boost::
                    process::system(installing);
            processes.pop_back();
        }
    });
    auto task3 = task2.then([] {
        if ((pack) && (exit_code == 0)) {
            processes.push_back
            (boost::this_process::get_id());
            exit_code = boost::process::system
                    (packing);
            processes.pop_back();
        }
    });
    return 0;
}
