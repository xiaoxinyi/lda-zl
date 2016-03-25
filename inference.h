#ifndef INFERENCE_H_
#define INFERENCE_H_

#include <vector>

using namespace std;

namespace lda {

class Document;
class Model;
class SuffStats;
class Option;

class Inference {
public:
	static double LdaInference(Document* document,
														 Model* model,
														 vector<double>& var_gamma,
														 vector<vector<double>>& phi);

	static double ComputeLikelihood(Document* document,
														 			Model* model,
														 			vector<double>& var_gamma,
														 			vector<vector<double>>& phi);

	static void SaveWordAssignment(ofstream& ofs,
																 Document* document,
																 vector<vector<double>>& phi);

	static void SaveGamma(ofstream& ofs,
												vector<double>& gamma);
};

}  // namespace lda


#endif // INFERENCE_H_

