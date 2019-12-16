// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using std::vector;
using std::fstream;

size_t l1, l2, l3;
vector<unsigned> meaningExp;
unsigned char *buffer;

class Experiment {
public:
    Experiment(int L1, int L2, int L3) {
        l1 = L1 * 1024; //kb ->byte
        l2 = L2 * 1024; // kb -> byte
        l3 = L3 * 1024 * 1024; // mb -> byte
        int x = 1;
        int n = 0;

        while (pow(2, x) < 0.5 * l1) {
            ++x;
        }
        while (pow(2, x + n) < (1.5) * l3) {
            meaningExp.push_back(pow(2, x + n));
            ++n;
        }
        meaningExp.push_back(1.5 * l3);
        for (size_t i = 0; i < meaningExp.size(); ++i) {
            std::cout << i << " experiment: size = " << meaningExp[i] <<  std::endl;
        }
    }

    void createBuffer(unsigned size_exp) {
        buffer = new unsigned char[size_exp];
    }

    void warmUp_cache(size_t size_exp) {
        for (size_t j = 0; j < 10; ++j) {
            for (size_t n = 0; n < size_exp; ++n) {
                buffer[n] = static_cast<char>(rand_r(&j) % 256 - 128);
            }
        }
    }

    clock_t directPass(unsigned numberExp) {
        clock_t begin = clock();
        for (int d = 0; d < 1000; ++d) {
            for (unsigned i = 0; i < meaningExp[numberExp]; ++i) {
                buffer[i] = static_cast<char>(rand_r(&d) % 256 - 128);
            }
        }
        clock_t end = clock();
        clock_t time = end - begin;
        return time;
    }

    clock_t returnPass(unsigned numberExp) {
        clock_t begin = clock();
        for (int d = 0; d < 1000; ++d) {
            for (unsigned i = meaningExp[numberExp] - 1; i > 0; --i) {
                buffer[i] = static_cast<char>(rand_r(&d) % 256 - 128);
            }
        }
        clock_t end = clock();
        clock_t time = end - begin;
        return time;
    }

    clock_t randomPass(unsigned numberExp) {
        vector<int> current_num;
        clock_t time1;

        for (unsigned i = 0; i < meaningExp[numberExp]; ++i) {
            current_num.push_back(i);
        }

        srand(time(NULL));
        random_shuffle(current_num.begin(), current_num.end());
        clock_t begin = clock();
        for (unsigned i = 0; i < 1000; ++i) {
            for (unsigned n = 0; n < meaningExp[numberExp]; ++n) {
                buffer[current_num[n]] =
                        static_cast<char>(rand_r(&i) % 255 - 128);
            }
        }
        clock_t end = clock();
        time1 = end - begin;
        return time1;
    }

    void start() {
        for (int pass = 0; pass < 3; ++pass) {
            const int dir = 0;
            const int ret = 1;
            const int run = 2;
            printTravel_order(pass);
            for (int count = 0; count < meaningExp.size(); ++count) {
                clock_t time;
                createBuffer(meaningExp[count]);
                warmUp_cache(meaningExp[count]);
                if (pass == dir) {
                    time = directPass(count);
                    printRes(time, count, meaningExp[count]);
                }
                if (pass == ret) {
                    time = returnPass(count);
                    printRes(time, count, meaningExp[count]);
                }
                if (pass == run) {
                    time = randomPass(count);
                    printRes(time, count, meaningExp[count]);
                }
            }
        }
    }

    void printTravel_order(int PASS) {
        std::cout << "investigation:" <<  std::endl;
        if (PASS == 0) {
            std::cout << "\ttravel_variant: " << "direction" <<  std::endl;
        }
        if (PASS == 1) {
            std::cout << "\ttravel_variant: " << "return" <<  std::endl;
        }
        if (PASS == 2) {
            std::cout << "\ttravel_variant: " << "random" <<  std::endl;
        }
        std::cout << "\texperiments: " <<  std::endl;
    }

    void printRes(clock_t TIME, int COUNT, unsigned MEANING) {
        std::cout << "\t- experiment:" <<  std::endl;
        std::cout << "\t\tnumber:" << COUNT + 1 <<  std::endl;
        std::cout << "\t\tinput data: " <<  std::endl;
        std::cout << "\t\t buffer_size:";
        std::cout << MEANING << " byte ";
        std::cout <<  std::endl << "\t\tresults:" <<  std::endl;
        std::cout << "\t\t duration: ";
        std::cout << TIME << "ms";
        std::cout <<  std::endl;
    }
};
#endif // INCLUDE_HEADER_HPP_