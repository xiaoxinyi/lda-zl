#include <iostream>

#include "lda.h"

using namespace lda;
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 3) {
    // The random number generator seed.
    // For testing an example seed is: t = 1147530551;
    long rng_seed;
    (void) time(&rng_seed);

    std::string filename_corpus = argv[1];
    std::string filename_settings = argv[2];

    LDAUtils::TrainLDA(filename_corpus, filename_settings, rng_seed);
    
  } else {
    cout << "Arguments: "
        "(1) corpus filename "
        "(2) settings filename" << endl;
  }
  return 0;
}