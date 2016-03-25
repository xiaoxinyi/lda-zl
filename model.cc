#include <cassert>
#include <cmath>

#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "model.h"
#include "utils.h"
#include "corpus.h"

const int BUF_SIZE = 10000;
const int INIT_NO = 1;

namespace lda {

// =======================================================================
// Model
// =======================================================================

Model::Model(
				double alpha, 
				int topic_no, 
				int term_no)
		: alpha_(alpha),
		  log_pr_word_(topic_no, vector<double>(term_no, 0.0)),
		  topic_no_(topic_no),
		  term_no_(term_no) {

}

Model::Model(int topic_no, int term_no) 
		: alpha_(1.0),
		  log_pr_word_(topic_no, vector<double>(term_no, 0.0)),
		  topic_no_(topic_no),
		  term_no_(term_no) {
}

Model::Model() 
		: alpha_(1.0),
		  topic_no_(0),
		  term_no_(0) {
}


void Model::initModel(int topic_no, int term_no, double alpha) {
	alpha_ = alpha;
	topic_no_ = topic_no;
	term_no_ = term_no;
	log_pr_word_ = vector<vector<double>>(topic_no, 
																				vector<double>(term_no, 0.0));
}

// =======================================================================
// ModelUtils
// =======================================================================

void ModelUtils::SaveModel(
				Model* model, 
				const string& model_root) {
	string filename = model_root + ".beta";
	ofstream ofs(filename);
	ofs.precision(12);
	ofs << setw(15);
	int topic_no = model->getTopicNo();
	int term_no = model->getTermNo();
	for (int i = 0; i < topic_no; i++) {
		for (int j = 0; j < term_no; j++) {
			ofs << model->getLogPrWord(i, j) << " ";
		}
		ofs << endl;
	}
	ofs.close();

	filename = model_root + ".other";
	ofs = ofstream(filename);
	assert(ofs.is_open());

	ofs.precision(12);
	ofs << setw(15);
	ofs << "topic_no " << topic_no << endl;
	ofs << "term_no " << term_no << endl;
	ofs << "alpha " << model->getAlpha() << endl;

	ofs.close(); 
}

void ModelUtils::LoadModel(
				Model* model, 
				const string& model_root) {
	double alpha = 1.0;
	int topic_no = 0;
	int term_no = 0;

	string filename = model_root + ".other";
	cout << "loading " << filename << endl;
	ifstream ifs(filename);
	char buf[BUF_SIZE];
	while (ifs.getline(buf, BUF_SIZE)) {
		istringstream iss(buf);
		string str;
		getline(iss, str, ' ');
		string value;
		getline(iss, value, ' ');

		if (str.compare("topic_no") == 0) {
			topic_no = atoi(value.c_str());
		} else if (str.compare("term_no") == 0) {
			term_no = atoi(value.c_str());
		} else if (str.compare("alpha") == 0) {
			alpha = atof(value.c_str());
		}
	}
	ifs.close();

	model->initModel(topic_no, term_no, alpha);

	filename = model_root + ".beta";
	ifs = ifstream(filename);
	cout << "loading " << filename << endl;
	for (int i = 0; i < topic_no; i++) {
		ifs.getline(buf, BUF_SIZE);
		istringstream iss(buf);
		for (int j = 0; j < term_no; j++) {
			string value;
			iss >> value;
			double log_pr_word = atof(value.c_str());
			model->setLogPrWord(i, j, log_pr_word);
		}
	}

	ifs.close();

}
// =======================================================================
// SuffStats
// =======================================================================

SuffStats::SuffStats(int doc_no, int topic_no, int term_no) 
		: class_word_(topic_no,vector<double>(term_no, 0.0)),
		  class_total_(topic_no, 0.0),
		  doc_no_(doc_no),
		  alpha_ss_(0.0) {

}


SuffStats::SuffStats(int topic_no, int term_no) 
		: class_word_(topic_no,vector<double>(term_no, 0.0)),
		  class_total_(topic_no, 0.0),
		  doc_no_(0),
		  alpha_ss_(0.0) {

}

SuffStats::SuffStats() 
		:	doc_no_(0),
		  alpha_ss_(0.0) {

}

void SuffStats::initSuffStats(int topic_no, 
															int term_no, 
															int doc_no) {
	class_word_ = vector<vector<double>>(topic_no, 
																			 vector<double>(term_no, 0.0));
	class_total_ = vector<double>(topic_no, 0.0);
	doc_no_ = doc_no;
	alpha_ss_ = 0.0;
}

// =======================================================================
// SuffStatsUtils
// =======================================================================

void SuffStatsUtils::InitSuffStatsByRandom(
											SuffStats* ss, 
											int topic_no, 
											int term_no) {
	for (int k = 0; k < topic_no; k++) {
		for (int n = 0; n < term_no; n++) {
			ss->updateClassWord(k, n, Utils::RandNo() + 1.0 / term_no);
			ss->updateClassTotal(k, ss->getClassWord(k, n));
		}
	}
}


void SuffStatsUtils::InitSuffStatsByCorpus(
											SuffStats* ss,
											Corpus* corpus,
											int topic_no,
											int term_no) {
	int already_selected = 0;
	vector<vector<int> > seen(topic_no, vector<int>(INIT_NO, -1));
	int d = -1;
	int doc_no = corpus->getDocuments();

	for (int k = 0; k < topic_no; k++) {
		for (int j = 0; j < INIT_NO; j++) {
			
			do {
				d = floor(Utils::RandNo() * doc_no);

				already_selected = 0;
				for (int i = 0; i < k; i++) {
					if (seen[i][j] == d) {
						already_selected = 1;
						cout << "skipping duplicate seed document" << endl;
					}
				}
			} while(already_selected);

			seen[k][j] = d;
			Document* document = corpus->getMutableDocument(d);
			for (int n = 0; n < document->getLength(); n++) {
				int word_id = document->getWord(n);
				int count = document->getCount(n);
				ss->updateClassWord(k, word_id, count);
			}
		}

		for (int n = 0; n < term_no; n++) {
			ss->updateClassWord(k, n, 1.0);
			ss->updateClassTotal(k, ss->getClassWord(k, n));
		}
	}
}
}  // namespace lda