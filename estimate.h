#ifndef ESTIMATE_H_
#define ESTIMATE_H_

#include "model.h"


namespace lda {

class Document;

class Estimate {
public:
	static void MStep(Model* model, SuffStats* ss);

	static double EStepForDocument(
								Document* document,
								vector<double>& gamma,
								vector<vector<double>>& phi,
								Model* model,
								SuffStats* ss);

	static void EstimateAlpha(Model* model, SuffStats* ss);
};

double opt_alpha(double ss, int D, int K);

}  // namespace lda

#endif // ESTIMATE_H_
