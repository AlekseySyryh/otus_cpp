#define BOOST_TEST_MODULE test

#include "split.h"
#include "ipaddress.h"
#include "process.h"
#include <vector>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_split)

    void Test(const std::string &str, char d, std::vector<std::string> exceptedResult) {
        //Вдруг мы захотим использовать другой контейнер?  Не переписывать же тест...
        //(Бред, но очень хотелось decltype)
        auto result = split(str, d);
        BOOST_CHECK(result == decltype(result)(exceptedResult));
    }

    BOOST_AUTO_TEST_CASE(case1) {
        Test("", '.', {""});
    }

    BOOST_AUTO_TEST_CASE(case2) {
        Test("11", '.', {"11"});
    }

    BOOST_AUTO_TEST_CASE(case3) {
        Test("..", '.', {"", "", ""});
    }

    BOOST_AUTO_TEST_CASE(case4) {
        Test("11.", '.', {"11", ""});
    }

    BOOST_AUTO_TEST_CASE(case5) {
        Test(".11", '.', {"", "11"});
    }

    BOOST_AUTO_TEST_CASE(case6) {
        Test("11.22", '.', {"11", "22"});
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_ipAddress)

    BOOST_AUTO_TEST_CASE(WrongLength) {
        BOOST_CHECK_THROW(ipAddress("1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1.1.1.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1.1.1.1.1"), std::logic_error);
    }

    BOOST_AUTO_TEST_CASE(WrongNumber) {
        BOOST_CHECK_THROW(ipAddress("1.999.22.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("-1.1.1.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1.999.2"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("999.1.1.1"), std::logic_error);
        BOOST_CHECK_THROW(ipAddress("1.1.1.999"), std::logic_error);
    }

    BOOST_AUTO_TEST_CASE(NonNumber) {
        BOOST_CHECK_THROW(ipAddress("a.b.c.d"), std::logic_error);
    }

    BOOST_AUTO_TEST_CASE(TupleChecker) {
        ipAddress address("1.2.3.4");
        BOOST_CHECK(address.tuple == ipAddressTuple({1, 2, 3, 4}));
    }

    BOOST_AUTO_TEST_CASE(IsMatchFirst) {
        ipAddress address("1.2.3.4");
        BOOST_CHECK(address.isMatch(1));
        BOOST_CHECK(!address.isMatch(2));
    }

    BOOST_AUTO_TEST_CASE(IsMatchTwo) {
        ipAddress address("1.2.3.4");
        BOOST_CHECK(address.isMatch(1, 2));
        BOOST_CHECK(!address.isMatch(2, 2));
        BOOST_CHECK(!address.isMatch(2, 1));
        BOOST_CHECK(!address.isMatch(1, 1));
    }

    BOOST_AUTO_TEST_CASE(IsMatchAny) {
        ipAddress address("1.2.3.4");
        BOOST_CHECK(address.isMatchAny(1));
        BOOST_CHECK(address.isMatchAny(2));
        BOOST_CHECK(address.isMatchAny(3));
        BOOST_CHECK(address.isMatchAny(4));
        BOOST_CHECK(!address.isMatchAny(5));

    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_Processer)

    BOOST_AUTO_TEST_CASE(Sort) {
        std::vector<ipAddress> ips{
                ipAddress("1.1.1.1"),
                ipAddress("1.1.10.1"),
                ipAddress("1.1.2.1"),
                ipAddress("1.10.1.1"),
                ipAddress("1.10.10.1"),
                ipAddress("1.10.2.1"),
                ipAddress("1.2.1.1"),
                ipAddress("1.2.10.1"),
                ipAddress("1.2.2.1"),
        };
        std::vector<ipAddress> exepted{
                ipAddress("1.10.10.1"),
                ipAddress("1.10.2.1"),
                ipAddress("1.10.1.1"),
                ipAddress("1.2.10.1"),
                ipAddress("1.2.2.1"),
                ipAddress("1.2.1.1"),
                ipAddress("1.1.10.1"),
                ipAddress("1.1.2.1"),
                ipAddress("1.1.1.1"),
        };
        sortVector(ips);
        BOOST_CHECK(ips == exepted);
    }

    BOOST_AUTO_TEST_CASE(ShowAll) {
        std::vector<ipAddress> ips{
                ipAddress("1.1.10.1"),
                ipAddress("1.1.2.1"),
                ipAddress("1.1.1.1"),
        };
        std::vector<std::string> result;
        showAll(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{"1.1.10.1", "1.1.2.1", "1.1.1.1"};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(Filter1Match) {
        std::vector<ipAddress> ips{
                ipAddress("2.1.10.1"),
                ipAddress("1.1.2.1"),
                ipAddress("1.1.1.1"),
        };
        std::vector<std::string> result;
        filter1(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{"1.1.2.1", "1.1.1.1"};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(Filter1NoMatch) {
        std::vector<ipAddress> ips{
                ipAddress("2.1.10.1"),
                ipAddress("2.1.2.1"),
                ipAddress("2.1.1.1"),
        };
        std::vector<std::string> result;
        filter1(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(Filter2Match) {
        std::vector<ipAddress> ips{
                ipAddress("47.1.1.1"),
                ipAddress("46.80.1.1"),
                ipAddress("46.70.255.1"),
                ipAddress("46.70.1.1"),
                ipAddress("46.69.1.1"),
                ipAddress("1.1.1.1"),
        };
        std::vector<std::string> result;
        filter2(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{"46.70.255.1", "46.70.1.1"};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(Filter2NoMatch) {
        std::vector<ipAddress> ips{
                ipAddress("47.1.1.1"),
                ipAddress("46.80.1.1"),
                ipAddress("46.69.1.1"),
                ipAddress("1.1.1.1"),
        };
        std::vector<std::string> result;
        filter2(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(FilterAnyMatch) {
        std::vector<ipAddress> ips{
                ipAddress("255.255.255.255"),
                ipAddress("255.255.255.46"),
                ipAddress("255.255.46.255"),
                ipAddress("255.46.255.255"),
                ipAddress("255.43.255.255"),
                ipAddress("46.255.255.255"),
                ipAddress("46.255.255.46"),
                ipAddress("45.45.45.45"),

        };
        std::vector<std::string> result;
        filterAny(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{"255.255.255.46",
                                         "255.255.46.255",
                                         "255.46.255.255",
                                         "46.255.255.255",
                                         "46.255.255.46"};
        BOOST_CHECK(result == exepted);
    }

    BOOST_AUTO_TEST_CASE(FilterAnyNoMatch) {
        std::vector<ipAddress> ips{
                ipAddress("255.255.255.255"),
                ipAddress("255.255.255.43"),
                ipAddress("255.255.48.255"),
                ipAddress("255.49.255.255"),
                ipAddress("255.43.255.255"),
                ipAddress("49.255.255.255"),
                ipAddress("48.255.255.43"),
                ipAddress("45.45.45.45"),

        };
        std::vector<std::string> result;
        filterAny(ips, [&result](std::string row) {
            result.push_back(row);
        });
        std::vector<std::string> exepted{};
        BOOST_CHECK(result == exepted);
    }

BOOST_AUTO_TEST_SUITE_END()