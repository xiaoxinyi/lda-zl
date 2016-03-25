#include <cassert>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "option.h"
#include "lda.h"

namespace lda {

// =======================================================================
// LDA
// =======================================================================

LDA::LDA()
		: likelihood_(0.0),
			max_likelihood_(0.0),
			iteration_(0) {}


// =======================================================================
// LDAUtils
// =======================================================================

void LDAUtils::ReadLDAInput(const string& filename_corpus,
											 const string& filename_settings,
											 LDA* lda) {
	Corpus* corpus = lda->getMutableCorpus();
	CorpusUtils::ReadCorpus(filename_corpus, corpus);
	OptionUtils::ReadOptions(filename_settings);
}


void LDAUtils::InitLDA(const string& filename_corpus,
											 const string& filename_settings,
											 long seed) {
	LDA* lda = new LDA;
	ReadLDAInput(filename_corpus, filename_settings, lda);

	Corpus* corpus = lda->getMutableCorpus();
	Model* model = lda->getMutableModel();
	SuffStats* ss = lda->getMutableSuffStats();
	Option& option = Option::GetInstance();

	int term_no = corpus->getTermNo();
	int topic_no = option.getTopicNo();

	


	if (option.getRandom() == 1) {
		
	}
}



}  // namespace lda