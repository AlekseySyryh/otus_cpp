#include <iostream>
#include <dlib/svm_threaded.h>

typedef dlib::matrix<double,7,1> sample_type;
typedef dlib::polynomial_kernel<sample_type> poly_kernel;

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}