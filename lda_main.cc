#include <iostream>

#include "lda.h"

using namespace lda;
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 4) {
    // The random number generator seed.
    // For testing an example seed is: t = 1147530551;
    long rng_seed;
    (void) time(&rng_seed);

    string filename_corpus = argv[1];
    string filename_settings = argv[2];
    string is_train = argv[3];
    if (is_train.compare("-est") == 0) {
      LDAUtils::TrainLDA(filename_corpus, filename_settings, rng_seed);
    } else if (is_train.compare("-inf") == 0) {
      LDAUtils::InferLDA(filename_corpus, filename_settings, rng_seed);
    }

    
  } else {
    cout << "Arguments: "
        "(1) corpus filename "
        "(2) settings filename "
        "(3) -est/-inf" << endl;
  }
  return 0;
}