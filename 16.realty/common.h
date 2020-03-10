#pragma once

#include <array>
#include <dlib/svm_threaded.h>

typedef dlib::matrix<double, 7, 1> sample_type;
typedef dlib::polynomial_kernel<sample_type> kernel_type;
std::array<int, 7> precs = {6, 6, 0, 2, 2, 2, 0};