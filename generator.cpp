#include <iostream>
#include <sstream>
#include <fstream>
#include <dlib/rand.h>

void generateStars(std::ofstream &fs, size_t stars, dlib::rand &rnd) {
    for (size_t i = 0; i < stars; ++i) {
        fs << rnd.get_double_in_range(-100, 100) << ";" << rnd.get_double_in_range(-100, 100) << std::endl;
    }
}

void generateSample(std::ofstream &fs, dlib::rand &rnd) {
    double x, y;
    const long num = 50;
    double radius = 0.5;
    for (long i = 0; i < num; ++i) {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        x = 2 * radius * rnd.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);
        fs << x << ";" << y << std::endl;
    }

    radius = 10.0;
    for (long i = 0; i < num; ++i) {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        x = 2 * radius * rnd.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);

        fs << x << ";" << y << std::endl;
    }

    // make some samples in a circle around the point (25,25)
    radius = 4.0;
    for (long i = 0; i < num; ++i) {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        x = 2 * radius * rnd.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);

        // translate this point away from the origin
        x += 25;
        y += 25;

        fs << x << ";" << y << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Порядок запуска:\n"
                     "# kkmeans_gen <file> [number_of_stars]\n"
                     "где\n"
                     "• file - имя файла.\n"
                     "• number_of_stars – количество звезд (если не указано - генерируются данные из примера)"
                  << std::endl;
        return EXIT_FAILURE;
    }
    std::ofstream fs(argv[1]);
    dlib::rand rnd(time(0));
    if (argc == 2) {
        generateSample(fs, rnd);
        return EXIT_SUCCESS;
    }
    std::istringstream ss(argv[2]);
    size_t stars;
    ss >> stars;
    if (!ss.fail()) {
        generateStars(fs, stars, rnd);
    } else {
        generateSample(fs, rnd);
    }
    return EXIT_SUCCESS;
}

