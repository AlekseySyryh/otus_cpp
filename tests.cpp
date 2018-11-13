#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(D1)

    BOOST_AUTO_TEST_CASE(DefaultMatrixIsEmpty) {
        Matrix<int, -1, 1> matrix;
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IfElementNotSetItIsDefault) {
        Matrix<int, -1, 1> matrix;
        BOOST_CHECK(matrix.get(0) == -1);
    }

    BOOST_AUTO_TEST_CASE(SettingNonDefaultValueChangedSize) {
        Matrix<int, -1, 1> matrix;
        matrix.set(1, 0);
        BOOST_CHECK(matrix.get(1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultValueNotChangedSize) {
        Matrix<int, -2, 1> matrix;
        matrix.set(1, -2);
        BOOST_CHECK(matrix.get(1) == -2);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultDeleteExistRecord) {
        Matrix<int, -1, 1> matrix;
        matrix.set(1, 0);
        BOOST_CHECK(matrix.get(1) == 0);
        BOOST_CHECK(matrix.size() == 1);
        matrix.set(1, -1);
        BOOST_CHECK(matrix.get(1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedGetter) {
        Matrix<int, -1, 1> matrix;
        matrix.set(1, 0);
        BOOST_CHECK(matrix[1] == 0);
        BOOST_CHECK(matrix.size() == 1);
        auto a = matrix[1];
        BOOST_CHECK(a == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetter) {
        Matrix<int, -1, 1> matrix;
        matrix[1] = 0;
        BOOST_CHECK(matrix.get(1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetterHardCase) {
        Matrix<int, -1, 1> matrix;
        (matrix[1] = 0) = -1;
        BOOST_CHECK(matrix.get(1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IteratorWorking) {
        Matrix<int, -1, 1> matrix;
        matrix[0] = 0;
        matrix[1] = 1;
        matrix[2] = 2;
        matrix[3] = -1;
        std::vector<std::tuple<size_t, int>> results;
        for (const auto &a : matrix) {
            results.push_back(a);
        }
        BOOST_CHECK(results.size() == 3);
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, int>(0, 0)) != results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, int>(1, 1)) != results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, int>(2, 2)) != results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, int>(3, -1)) == results.end());
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(D2)

    BOOST_AUTO_TEST_CASE(DefaultMatrixIsEmpty) {
        Matrix<int, -1> matrix;
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IfElementNotSetItIsDefault) {
        Matrix<int, -1> matrix;
        BOOST_CHECK(matrix.get(0, 0) == -1);
    }

    BOOST_AUTO_TEST_CASE(SettingNonDefaultValueChangedSize) {
        Matrix<int, -1> matrix;
        matrix.set(1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultValueNotChangedSize) {
        Matrix<int, -2> matrix;
        matrix.set(1, 1, -2);
        BOOST_CHECK(matrix.get(1, 1) == -2);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultDeleteExistRecord) {
        Matrix<int, -1> matrix;
        matrix.set(1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
        matrix.set(1, 1, -1);
        BOOST_CHECK(matrix.get(1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedGetter) {
        Matrix<int, -1> matrix;
        matrix.set(1, 1, 0);
        BOOST_CHECK(matrix[1][1] == 0);
        BOOST_CHECK(matrix.size() == 1);
        auto a = matrix[1][1];
        BOOST_CHECK(a == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetter) {
        Matrix<int, -1> matrix;
        matrix[1][1] = 0;
        BOOST_CHECK(matrix.get(1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetterHardCase) {
        Matrix<int, -1> matrix;
        (matrix[1][1] = 0) = -1;
        BOOST_CHECK(matrix.get(1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IteratorWorking) {
        Matrix<int, -1> matrix;
        matrix[0][0] = 0;
        matrix[1][1] = 1;
        matrix[2][2] = 2;
        matrix[3][3] = -1;
        std::vector<std::tuple<size_t, size_t, int>> results;
        for (const auto &a : matrix) {
            results.push_back(a);
        }
        BOOST_CHECK(results.size() == 3);
        BOOST_CHECK(
                std::find(results.begin(), results.end(), std::tuple<size_t, size_t, int>(0, 0, 0)) != results.end());
        BOOST_CHECK(
                std::find(results.begin(), results.end(), std::tuple<size_t, size_t, int>(1, 1, 1)) != results.end());
        BOOST_CHECK(
                std::find(results.begin(), results.end(), std::tuple<size_t, size_t, int>(2, 2, 2)) != results.end());
        BOOST_CHECK(
                std::find(results.begin(), results.end(), std::tuple<size_t, size_t, int>(3, 3, -1)) == results.end());
    }


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(D3)

    BOOST_AUTO_TEST_CASE(DefaultMatrixIsEmpty) {
        Matrix<int, -1, 3> matrix;
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IfElementNotSetItIsDefault) {
        Matrix<int, -1, 3> matrix;
        BOOST_CHECK(matrix.get(0, 0, 0) == -1);
    }

    BOOST_AUTO_TEST_CASE(SettingNonDefaultValueChangedSize) {
        Matrix<int, -1, 3> matrix;
        matrix.set(1, 1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }


    BOOST_AUTO_TEST_CASE(SettingDefaultValueNotChangedSize) {
        Matrix<int, -2, 3> matrix;
        matrix.set(1, 1, 1, -2);
        BOOST_CHECK(matrix.get(1, 1, 1) == -2);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultDeleteExistRecord) {
        Matrix<int, -1, 3> matrix;
        matrix.set(1, 1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
        matrix.set(1, 1, 1, -1);
        BOOST_CHECK(matrix.get(1, 1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedGetter) {
        Matrix<int, -1, 3> matrix;
        matrix.set(1, 1, 1, 0);
        BOOST_CHECK(matrix[1][1][1] == 0);
        BOOST_CHECK(matrix.size() == 1);
        auto a = matrix[1][1][1];
        BOOST_CHECK(a == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetter) {
        Matrix<int, -1, 3> matrix;
        matrix[1][1][1] = 0;
        BOOST_CHECK(matrix.get(1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetterHardCase) {
        Matrix<int, -1, 3> matrix;
        (matrix[1][1][1] = 0) = -1;
        BOOST_CHECK(matrix.get(1, 1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IteratorWorking) {
        Matrix<int, -1, 3> matrix;
        matrix[0][0][0] = 0;
        matrix[1][1][1] = 1;
        matrix[2][2][2] = 2;
        matrix[3][3][3] = -1;
        std::vector<std::tuple<size_t, size_t, size_t, int>> results;
        for (const auto &a : matrix) {
            results.push_back(a);
        }
        BOOST_CHECK(results.size() == 3);
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, size_t, size_t, int>(0, 0, 0, 0)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, size_t, size_t, int>(1, 1, 1, 1)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, size_t, size_t, int>(2, 2, 2, 2)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(), std::tuple<size_t, size_t, size_t, int>(3, 3, 3, -1)) ==
                    results.end());
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(D4)

    BOOST_AUTO_TEST_CASE(DefaultMatrixIsEmpty) {
        Matrix<int, -1, 4> matrix;
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IfElementNotSetItIsDefault) {
        Matrix<int, -1, 4> matrix;
        BOOST_CHECK(matrix.get(0, 0, 0, 0) == -1);
    }

    BOOST_AUTO_TEST_CASE(SettingNonDefaultValueChangedSize) {
        Matrix<int, -1, 4> matrix;
        matrix.set(1, 1, 1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }


    BOOST_AUTO_TEST_CASE(SettingDefaultValueNotChangedSize) {
        Matrix<int, -2, 4> matrix;
        matrix.set(1, 1, 1, 1, -2);
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == -2);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(SettingDefaultDeleteExistRecord) {
        Matrix<int, -1, 4> matrix;
        matrix.set(1, 1, 1, 1, 0);
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
        matrix.set(1, 1, 1, 1, -1);
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedGetter) {
        Matrix<int, -1, 4> matrix;
        matrix.set(1, 1, 1, 1, 0);
        BOOST_CHECK(matrix[1][1][1][1] == 0);
        BOOST_CHECK(matrix.size() == 1);
        auto a = matrix[1][1][1][1];
        BOOST_CHECK(a == 0);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetter) {
        Matrix<int, -1, 4> matrix;
        matrix[1][1][1][1] = 0;
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == 0);
        BOOST_CHECK(matrix.size() == 1);
    }

    BOOST_AUTO_TEST_CASE(IndexedSetterHardCase) {
        Matrix<int, -1, 4> matrix;
        (matrix[1][1][1][1] = 0) = -1;
        BOOST_CHECK(matrix.get(1, 1, 1, 1) == -1);
        BOOST_CHECK(matrix.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(IteratorWorking) {
        Matrix<int, -1, 4> matrix;
        matrix[0][0][0][0] = 0;
        matrix[1][1][1][1] = 1;
        matrix[2][2][2][2] = 2;
        matrix[3][3][3][3] = -1;
        std::vector<std::tuple<size_t, size_t, size_t, size_t, int>> results;
        for (const auto &a : matrix) {
            results.push_back(a);
        }
        BOOST_CHECK(results.size() == 3);
        BOOST_CHECK(std::find(results.begin(), results.end(),
                              std::tuple<size_t, size_t, size_t, size_t, int>(0, 0, 0, 0, 0)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(),
                              std::tuple<size_t, size_t, size_t, size_t, int>(1, 1, 1, 1, 1)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(),
                              std::tuple<size_t, size_t, size_t, size_t, int>(2, 2, 2, 2, 2)) !=
                    results.end());
        BOOST_CHECK(std::find(results.begin(), results.end(),
                              std::tuple<size_t, size_t, size_t, size_t, int>(3, 3, 3, 3, -1)) ==
                    results.end());
    }

BOOST_AUTO_TEST_SUITE_END()