#pragma once

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <iostream>
#include "hash_factory.h"
#include <boost/filesystem.hpp>
#include <boost/unordered_set.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>

static const boost::container::vector<std::string> charsToEscape = {"[", "]", "/", "^", "$", ".", "|", "+", "(", ")",
                                                                    "{", "}"};
class options {
public:
    options(int argc, const char *argv[]) {
        try {
            fillDescription();
            store(parse_command_line(argc, argv, desc), vm);

                    processHelp() &&
                    processScan() &&
                    processExclude() &&
                    processDeep() &&
                    processMinSize() &&
                    processMask() &&
                    processBlockSize() &&
                    processAlgorithm() &&
                    processDebug();

        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cout << desc << std::endl;
            exit_fail = true;
            return;
        }
    }

    bool exit_ok = false;
    bool exit_fail = false;
    boost::container::vector<boost::filesystem::path> dirsToScan;
    boost::unordered_set<boost::filesystem::path> dirsToExclude;
    size_t deep = 0;
    size_t minsize = 0;
    boost::container::vector<boost::regex> masks;
    size_t blocksize = 0;
    std::string algo;
    bool debug = false;
private:
    boost::program_options::options_description desc{"Опции"};
    boost::program_options::variables_map vm;

    void fillDescription() {
        std::string algorithms = boost::algorithm::join(HashFactory.getAlgoritms(), ", ");
        desc.add_options()
                ("help,h", "Этот экран")
                ("scan,s",
                 boost::program_options::value<std::vector<std::__cxx11::string>>(), //boost::container::vector - тут решительно не хочет. Пришлось взять STL... :(
                 "Директории для сканирования (может быть несколько)")
                ("exclude,e", boost::program_options::value<std::vector<std::__cxx11::string>>(),
                 "Директории для исключения из сканирования (может быть несколько)")
                ("deep,d", boost::program_options::value<size_t>()->default_value(0),
                 "Глубина сканирования (один на все директории, 0 - только указанная директория без вложенных)")
                ("minsize", boost::program_options::value<std::__cxx11::string>()->default_value("1B"),
                 "Минимальный размер файла, по умолчанию проверяются все файлы больше 1 байта")
                ("mask", boost::program_options::value<std::vector<std::__cxx11::string>>(),
                 "Маски имен файлов разрешенных для сравнения (не зависят от регистра)")
                ("blocksize", boost::program_options::value<std::__cxx11::string>()->default_value("1K"),
                 "Размер блока, которым производится чтения файлов")
                ("algo", boost::program_options::value<std::__cxx11::string>()->default_value("CRC"),
                 (boost::format("Алгоритм хэширования (возможные значения: %1%)") % algorithms).str().data())
                ("debug", "Отладочная информация");
    }

    boost::filesystem::path checkDir(std::string dir) {
        boost::filesystem::path p(dir);
        boost::filesystem::file_status fs = status(p);
        if (!is_directory(fs)) {
            exit_fail = true;
            std::cerr << "Параметр " << dir << " не описывает существующую директорию" << std::endl;
            throw std::logic_error("");
        }
        return canonical(p);
    }

    bool processHelp() {
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            exit_ok = true;
            return false;
        }
        return true;
    }

    bool processScan() {
        if (!vm.count("scan")) {
            std::cerr << "Следует задать не менее одной директории для сканирования" << std::endl;
            exit_fail = true;
            return false;
        }
        BOOST_FOREACH(auto dir, vm["scan"].as<std::vector<std::string>>()) {
                        try {
                            dirsToScan.push_back(checkDir(dir));
                        } catch (const std::logic_error &) {
                            return false;
                        }
                    }
        return true;
    }

    bool processExclude() {
        if (!vm.count("exclude")) {
            return true;
        }
        BOOST_FOREACH(auto dir, vm["exclude"].as<std::vector<std::string>>()) {
                        try {
                            dirsToExclude.insert(checkDir(dir));
                        } catch (const std::logic_error &) {
                            return false;
                        }
                    }
        return true;
    }

    bool processDeep() {
        if (vm.count("deep")) {
            deep = vm["deep"].as<size_t>();
        }
        return true;
    }

    size_t parseSize(const std::string &size) {
        size_t result;
        std::istringstream ss(size);
        ss >> result;
        if (!ss.eof()) {
            char suffix;
            ss >> suffix;
            switch (toupper(suffix)) {
                case 'B':
                    break;
                case 'K':
                    result *= 1024;
                    break;
                case 'M':
                    result *= 1024 * 1024;
                    break;
                case 'G':
                    result *= 1024 * 1024 * 1024;
                    break;
                default:
                    std::cerr << "Не могу разобрать размер " << size;
                    exit_fail = true;
                    throw std::logic_error("");
            }
        }
        return result;
    }

    bool processMinSize() {
        if (vm.count("minsize")) {
            try {
                minsize = parseSize(vm["minsize"].as<std::string>());
            } catch (const std::logic_error &) {
                return false;
            }
        }
        return true;
    }

    bool processMask() {
        if (vm.count("mask")) {
            BOOST_FOREACH(auto mask, vm["mask"].as<std::vector<std::string>>()) {
                            BOOST_FOREACH(auto chr, charsToEscape) {
                                            boost::replace_all(mask, chr, "\\" + chr);
                                        }
                            boost::replace_all(mask, "*", ".*");
                            boost::replace_all(mask, "?", ".{1}");
                            masks.emplace_back(mask, boost::regex::icase);
                        }
        } else {
            masks.emplace_back(".*");
        }
        return true;
    }

    bool processBlockSize() {
        if (vm.count("blocksize")) {
            try {
                blocksize = parseSize(vm["blocksize"].as<std::string>());
            } catch (const std::logic_error &) {
                return false;
            }
        }
        return true;
    }

    bool processAlgorithm() {
        if (vm.count("algo")) {
            auto alg = vm["algo"].as<std::string>();
            boost::to_upper(alg);
            if (HashFactory.getAlgoritms().count(alg)) {
                algo = alg;
            } else {
                std::cerr << "Неизвестный алгоритм " << alg << std::endl;
                exit_fail = true;
                return false;
            }
        }
        return true;
    }

    bool processDebug() {
        debug = vm.count("debug") > 0;
        return true;
    }

};
