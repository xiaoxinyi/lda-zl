#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

using namespace std;

namespace lda {

class Model {
public:
Model(double alpha, int topic_no, int term_no);
Model(int topic_no, int term_no);
Model();

void initModel(int topic_no, int term_no, double alpha = 1.0);
double getAlpha() const { return alpha_; }
void setAlpha(double alpha) { alpha_ = alpha; }

int getTopicNo() const { return topic_no_; }
void setTopicNo(int topic_no) { topic_no_ = topic_no; }

int getTermNo() const { return term_no_; }
void setTermNo(int term_no) { term_no_ = term_no; }

double getLogPrWord(int topic, int word) const { 
	return log_pr_word_[topic][word];
}

void setLogPrWord(int topic, int word, double val) {
	log_pr_word_[topic][word] = val;
}

private:
	// Hyper parameter.
	double alpha_;

	// Log probabilities of each word in each topic.
	vector<vector<double>> log_pr_word_;

	// Number of topics.
	int topic_no_;

	// Number of vocabulary.
	int term_no_;

};

class ModelUtils {
public:
	static void SaveModel(Model* model, const string& model_root);

	static void LoadModel(Model* model, const string& string_root);
};

class SuffStats {
public:
	SuffStats(int doc_no, int topic_no, int term_no);
	SuffStats(int topic_no, int term);
	SuffStats();

	void initSuffStats(int topic_no, int term_no, int doc_no = 0);

	int getDocumentNo() const { return doc_no_; }
	void setDocumentNo(int doc_no) { doc_no_ = doc_no; }
	void updateDocumentNo(int update) { doc_no_ += update; }

	double getClassWord(int topic, int doc) const {
		return class_word_[topic][doc];
	}
	void setClassWord(int topic, int doc, double val) { 
		class_word_[topic][doc] = val;
	}
	void updateClassWord(int topic, int doc, double update) {
		class_word_[topic][doc] += update;
	}

	double getClassTotal(int topic) const { return class_total_[topic];}
	void setClassTotal(int topic, double val) { class_total_[topic] = val; }
	void updateClassTotal(int topic, double update) {
		class_total_[topic] += update;
	}

	double getAlphaSS() const { return alpha_ss_; }
	void setAlphaSS(double alpha_ss) { alpha_ss_ = alpha_ss; }
	void updateAlphaSS(double update) { alpha_ss_ += update; }

private:
	// Word statistics in each document.
	vector<vector<double>> class_word_;

	// Total word statistics in each document.
	vector<double> class_total_;

	// Number of documents.
	int doc_no_;

	// Alpha sufficient statistics.
	double alpha_ss_;
};

// Forward declare.
class Corpus;

class SuffStatsUtils {
public:
	static void InitSuffStatsByRandom(
									SuffStats* ss, 
									int topic_no, 
									int term_no);

	static void InitSuffStatsByCorpus(
									SuffStats* ss,
									Corpus* corpus,
									int topic_no,
									int term_no);
};
}  // namespace lda



#endif // MODEL_H_
