#ifndef OPTION_H_
#define OPTION_H_

#include <string>

using namespace std;

namespace lda {

class Option {
public:
	static Option& GetInstance();
public:
	void setVarMaxIter(int var_max_iter) { var_max_iter_ = var_max_iter; }
	int getVarMaxIter() const { return var_max_iter_; }

	void setVarConverged(int var_converged) { var_converged_ = var_converged; }
	double getVarConverged() const { return var_converged_; }

	void setEmMaxIter(int em_max_iter) { em_max_iter_ = em_max_iter; }
	int getEmMaxIter() const { return em_max_iter_; }

	void setEmConverged(double em_converged) { em_converged_ = em_converged; }
	double getEmConverged() const { return em_converged_; }

	void setEstimateAlpha(int estimate_alpha) { estimate_alpha_ = estimate_alpha; }
	int getEstimateAlpha() const { return estimate_alpha_; }

	void setTopicNo(int topic_no) { topic_no_ = topic_no; }
	int getTopicNo() const { return topic_no_; }

	void setModelRoot(string& model_root) { model_root_ = model_root; }
	string getModelRoot() const { return model_root_; }

	void setDirectory(string& dir) { directory_ = dir; }
	string getDirectory() const { return directory_; }

	void setEstimate(int est) { estimate_ = est; }
	int getEstimate() const { return estimate_; }

	void setInference(int inference) { inference_ = inference; }
	int getInference() const { return inference_; }

	void setRandom(int random) { random_ = random; }
	int getRandom() const { return random_; }

	void setSeeded(int seeded) { seeded_ = seeded; }
	int getSeeded() const { return seeded_; }

	void setAlpha(double alpha) { alpha_ = alpha; }
	double getAlpha() const { return alpha_; }


private:
	// Variational inference max iters.
	int var_max_iter_;

	// Variatioinal inference converge.
	double var_converged_;

	int em_max_iter_;

	double em_converged_;

	int estimate_alpha_;

	int topic_no_;

	int estimate_;

	int inference_;

	int random_;

	int seeded_;

	double alpha_;

	string model_root_;

	string directory_;

	Option() { }
};


class OptionUtils {
public:
	static void ReadOptions(const string& filename);
};


}  // namespace lda





#endif // OPTION_H_


