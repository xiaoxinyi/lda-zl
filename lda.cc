#include <cassert>
#include <cstdio>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "option.h"
#include "lda.h"
#include "utils.h"
#include "estimate.h"
#include "inference.h"

namespace lda {

// =======================================================================
// LDA
// =======================================================================

LDA::LDA()
		: iteration_(0) {}


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


LDA* LDAUtils::InitLDA(const string& filename_corpus,
											 const string& filename_settings,
											 long seed) {
	LDA* lda = new LDA;
	ReadLDAInput(filename_corpus, filename_settings, lda);

	Utils::InitRandomNumberGen(seed);

	Corpus* corpus = lda->getMutableCorpus();
	Model* model = lda->getMutableModel();
	SuffStats* ss = lda->getMutableSuffStats();
	Option& option = Option::GetInstance();

	int term_no = corpus->getTermNo();
	int topic_no = option.getTopicNo();
	double alpha = option.getAlpha();
	int estimate_alpha = option.getEstimateAlpha();

	// Init data structure.
	model->initModel(topic_no, term_no, alpha);
	ss->initSuffStats(topic_no, term_no, 0);

	// Init LDA.
	if (option.getRandom() == 1) {
		SuffStatsUtils::InitSuffStatsByRandom(ss, topic_no, term_no);
	}
	if (option.getSeeded() == 1) {
		SuffStatsUtils::InitSuffStatsByCorpus(ss, corpus, topic_no, term_no);
	}

	Estimate::MStep(model, ss);

	if (estimate_alpha == 1) {
		Estimate::EstimateAlpha(model, ss);
	}

	string filename = option.getDirectory() + "/0000";
	ModelUtils::SaveModel(model, filename);

	return lda;
}

void LDAUtils::IterateLDA(LDA* lda) {
	assert(lda != nullptr);

	Corpus* corpus = lda->getMutableCorpus();
	Model* model = lda->getMutableModel();
	SuffStats* ss = lda->getMutableSuffStats();
	Option& option = Option::GetInstance();

	
	
	

	double likelihood = 0.0;
	double likelihood_old = 0.0;
	double converged = 1.0;
	int cur_iteration = 0;

	double em_converged = option.getEmConverged();
	double em_max_iter = option.getEmMaxIter();


	int term_no = corpus->getTermNo();
	int topic_no = option.getTopicNo();
	int doc_no = corpus->getDocuments();
	string directory = option.getDirectory();
	int estimate_alpha = option.getEstimateAlpha();
	int save_lag = option.getSaveLag();

	vector<vector<double>> gamma(doc_no, vector<double>());
	vector<vector<double>> phi;

	string filename_likelihood = directory + "/likelihood.dat";
	ofstream ofs_likelihood(filename_likelihood);

	char filename[50];

	while (((converged < 0) or (converged > em_converged))
					and (cur_iteration < em_max_iter)) {
		lda->incIteration(1);
		cur_iteration = lda->getIteration();
		cout << "Start iteration... " << cur_iteration << endl;


		ss->initSuffStats(term_no, topic_no, 0);
		

		for (int i = 0; i < doc_no; i++) {
			Document* document = corpus->getMutableDocument(i);
			likelihood += Estimate::EStepForDocument(document, gamma[i], phi, model, ss);
		}


		Estimate::MStep(model, ss);

		if (estimate_alpha == 1) {
			Estimate::EstimateAlpha(model, ss);
		}

		if (save_lag % cur_iteration == 0) {
			sprintf(filename, "%s/%04d", directory.c_str(), cur_iteration);
			ModelUtils::SaveModel(model, filename);
			sprintf(filename, "%s/%04d.gamma", directory.c_str(), cur_iteration);
			CorpusUtils::SaveGamma(filename, gamma, corpus);
		}

		if (cur_iteration > 1) {
			converged = (likelihood_old - likelihood) / (likelihood_old);
		}

		if (converged < 0) {
			int var_max_iter = option.getVarMaxIter();
			option.setVarMaxIter(2 * var_max_iter);
		} 

    likelihood_old = likelihood;
    ofs_likelihood << likelihood << " " << converged << endl;

	}

	sprintf(filename, "%s/final", directory.c_str());
	ModelUtils::SaveModel(model, filename);
	sprintf(filename, "%s/final.gamma", directory.c_str());
	CorpusUtils::SaveGamma(filename, gamma, corpus);

	sprintf(filename, "%s/word-assign.dat", directory.c_str());
	ofstream ofs_word_assign(filename);
	for (int i = 0; i < doc_no; i++) {
		Document* document = corpus->getMutableDocument(i);
		Inference::LdaInference(document, model, gamma[i], phi);
		Inference::SaveWordAssignment(ofs_word_assign, document, phi);
	}

	ofs_likelihood.close();
	ofs_word_assign.close();
	
}

void LDAUtils::TrainLDA(const string& filename_corpus,
												const string& filename_settings,
												long seed) {
	LDA* lda = InitLDA(filename_corpus, filename_settings, seed);
	IterateLDA(lda);
	delete lda;
}


void LDAUtils::InferLDA(const string& filename_corpus,
											 const string& filename_settings,
											 long seed) {
	LDA* lda = new LDA;
	ReadLDAInput(filename_corpus, filename_settings, lda);

	Utils::InitRandomNumberGen(seed);

	Corpus* corpus = lda->getMutableCorpus();
	Model* model = lda->getMutableModel();
	Option& option = Option::GetInstance();

	string directory = option.getDirectory();
	int doc_no = corpus->getDocuments();

	string model_root = directory + "/final";

	ModelUtils::LoadModel(model, model_root);

	vector<vector<double>> gamma(doc_no, vector<double>());
	vector<vector<double>> phi;

	char filename[100];
	sprintf(filename, "%s/infer-likelihood.dat", directory.c_str());
	ofstream ofs(filename);
	for (int i = 0; i < doc_no; i++) {
		Document* document = corpus->getMutableDocument(i);
		double likelihood = Inference::LdaInference(document, model, gamma[i], phi);
		ofs.precision(10);
		ofs << likelihood << endl;
	}

	ofs.close();
	sprintf(filename, "%s/infer.gamma", directory.c_str());
	CorpusUtils::SaveGamma(filename, gamma, corpus);

	assert(lda != nullptr);
	delete lda;
}

}  // namespace lda