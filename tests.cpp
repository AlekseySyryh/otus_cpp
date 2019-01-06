#include "radix_tree.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(tests_add)

    BOOST_AUTO_TEST_CASE(Step0) {
        RadixTree rt;
        BOOST_CHECK(!rt.getRoot());
    }

    BOOST_AUTO_TEST_CASE(Step1) {
        RadixTree rt;
        rt.add("aleksey");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"aleksey");
        BOOST_CHECK(rt.getRoot()->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(Step2) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"");
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getLabel() == L"aleksey");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getLabel() == L"sasha");
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(Step3) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        rt.add("aleks");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"");
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getLabel() == L"aleks");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('e')->getLabel() == L"ey");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('e')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getLabel() == L"sasha");
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(Step4) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        rt.add("aleks");
        rt.add("alek");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"");
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getLabel() == L"alek");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('s')->getLabel() == L"s");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('s')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('s')->getChilds().at('e')->getLabel() == L"ey");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('s')->getChilds().at('e')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getLabel() == L"sasha");
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(Step5) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        rt.add("aleks");
        rt.add("alek");
        rt.add("alesha");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"");
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getLabel() == L"ale");
        BOOST_CHECK(!rt.getRoot()->getChilds().at('a')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getLabel() == L"k");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getLabel() == L"s");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getChilds().at(
                'e')->getLabel() == L"ey");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getChilds().at(
                'e')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getLabel() == L"sasha");
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(Step6) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        rt.add("aleks");
        rt.add("alek");
        rt.add("alesha");
        rt.add("maksim");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"");
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getLabel() == L"ale");
        BOOST_CHECK(!rt.getRoot()->getChilds().at('a')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getLabel() == L"k");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getLabel() == L"s");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getChilds().at(
                'e')->getLabel() == L"ey");
        BOOST_CHECK(rt.getRoot()->getChilds().at('a')->getChilds().at('k')->getChilds().at('s')->getChilds().at(
                'e')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getLabel() == L"sasha");
        BOOST_CHECK(rt.getRoot()->getChilds().at('s')->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at('m')->getLabel() == L"maksim");
        BOOST_CHECK(rt.getRoot()->getChilds().at('m')->getIsEnd());
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tests_utf8)

    BOOST_AUTO_TEST_CASE(UTF8CanBeAdded) {
        RadixTree rt;
        rt.add("ыы");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"ыы");
        BOOST_CHECK(rt.getRoot()->getLabel().size() == 2);
        BOOST_CHECK(rt.getRoot()->getIsEnd());
    }

    BOOST_AUTO_TEST_CASE(UTF8SplitOk) {
        RadixTree rt;
        rt.add("ыы");
        rt.add("ый");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabel() == L"ы");
        BOOST_CHECK(rt.getRoot()->getLabel().size() == 1);
        BOOST_CHECK(!rt.getRoot()->getIsEnd());
        BOOST_CHECK(rt.getRoot()->getChilds().at(L'ы')->getLabel() == L"ы");
        BOOST_CHECK(rt.getRoot()->getChilds().at(L'й')->getLabel() == L"й");
    }

    BOOST_AUTO_TEST_CASE(LabelAsUTF8) {
        RadixTree rt;
        rt.add("ыы");
        BOOST_CHECK(rt.getRoot());
        BOOST_CHECK(rt.getRoot()->getLabelAsString() == "ыы");
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tests_enumerate)

    BOOST_AUTO_TEST_CASE(Enumerate) {
        RadixTree rt;
        rt.add("aleksey");
        rt.add("sasha");
        rt.add("aleks");
        rt.add("alek");
        rt.add("alesha");
        rt.add("maksim");
        std::vector<std::pair<std::string, std::string>> result = rt.enumerate();
        std::sort(result.begin(), result.end());
        BOOST_CHECK(result.size() == 6);
        BOOST_CHECK(result[0].first == "alek");
        BOOST_CHECK(result[0].second == "alek");
        BOOST_CHECK(result[1].first == "aleks");
        BOOST_CHECK(result[1].second == "aleks");
        BOOST_CHECK(result[2].first == "aleksey");
        BOOST_CHECK(result[2].second == "alekse");
        BOOST_CHECK(result[3].first == "alesha");
        BOOST_CHECK(result[3].second == "ales");
        BOOST_CHECK(result[4].first == "maksim");
        BOOST_CHECK(result[4].second == "m");
        BOOST_CHECK(result[5].first == "sasha");
        BOOST_CHECK(result[5].second == "s");
    }

    BOOST_AUTO_TEST_CASE(EnumerateUTF) {
        RadixTree rt;
        rt.add("ы");
        rt.add("ыыы");
        rt.add("ыйй");
        std::vector<std::pair<std::string, std::string>> result = rt.enumerate();
        std::sort(result.begin(), result.end());
        BOOST_CHECK(result.size() == 3);
        BOOST_CHECK(result[0].first == "ы");
        BOOST_CHECK(result[0].second == "ы");
        BOOST_CHECK(result[1].first == "ыйй");
        BOOST_CHECK(result[1].second == "ый");
        BOOST_CHECK(result[2].first == "ыыы");
        BOOST_CHECK(result[2].second == "ыы");
    }

BOOST_AUTO_TEST_SUITE_END()