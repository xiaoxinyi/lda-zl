#ifndef LDA_H_
#define LDA_H_

#include "corpus.h"
#include "model.h"


namespace lda {

class LDA {
public:
	LDA();

	Corpus* getMutableCorpus() { return &corpus_; }
	void setCorpus(Corpus&& corpus) { corpus_ = move(corpus); }

	Model* getMutableModel() { return &model_; }
	void setModel(Model&& model) { model_ = move(model); }

	SuffStats* getMutableSuffStats() { return &ss_; }
	void setSuffStats(SuffStats&& ss) { ss_ = move(ss); }

	int getIteration() const { return iteration_; }
  void setIteration(int iteration) { iteration_ = iteration; }
  void incIteration(int val) { iteration_ += val; }


private:
	Corpus corpus_;

	Model model_;

	SuffStats ss_;

  double likelihood_;

	// The current maximum score over several iterations.
  double max_likelihood_;

  // Current iteration.
  int iteration_;


};

class LDAUtils {
public:
	static void ReadLDAInput(const string& filename_corpus,
														const string& filename_settings,
													LDA* lda);

	static LDA* InitLDA(const string& filename_corpus,
											const string& filename_settings,
											long seed);
	static void IterateLDA(LDA* lda);

	static void TrainLDA(const string& filename_corpus,
											 const string& filename_settings,
											 long seed);
};

}  // namespace lda

#endif // LDA_H_
