#include <cmath>
#include <cassert>

#include <gsl/gsl_sf_psi.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "document.h"
#include "option.h"
#include "model.h"
#include "inference.h"
#include "utils.h"

namespace lda {

// =======================================================================
// Inference
// =======================================================================

double Inference::LdaInference(Document* document,
														 Model* model,
														 vector<double>& var_gamma,
														 vector<vector<double>>& phi) {
	double likelihood = 0.0, likelihood_old = 0.0;
	double converged = 1.0;
	double VAR_CONVERGED = Option::GetInstance().getVarConverged();
	double VAR_MAX_ITER = Option::GetInstance().getVarMaxIter();

	int topic_no = model->getTopicNo();
	int length = document->getLength();
	double init_var_gamma = model->getAlpha() + 
													1.0 * document->getTotal() / topic_no;
	var_gamma = vector<double>(topic_no, init_var_gamma);
	phi = vector<vector<double>>(length, vector<double>(topic_no, 1.0 / topic_no));
	vector<double> digamma_gam(topic_no, gsl_sf_psi(init_var_gamma));

	vector<double> oldphi(topic_no, 0.0);
	double phisum = 0.0;

	int var_iter = 0;

	while ((converged > VAR_CONVERGED) and
				 ((var_iter < VAR_MAX_ITER) or (VAR_MAX_ITER == -1))) {
		var_iter++;
		for (int n = 0; n < length; n++) {
			phisum = 0;
      for (int k = 0; k < topic_no; k++) {
        oldphi[k] = phi[n][k];
        int word_id = document->getWord(n);
        phi[n][k] = digamma_gam[k] + model->getLogPrWord(k, word_id);

        if (k > 0)
          phisum = Utils::LogSum(phisum, phi[n][k]);
        else
          phisum = phi[n][k]; 
      }

      for (int k = 0; k < topic_no; k++) {
        phi[n][k] = exp(phi[n][k] - phisum);
        var_gamma[k] = var_gamma[k] + document->getCount(n) * (phi[n][k] - oldphi[k]);
        digamma_gam[k] = gsl_sf_psi(var_gamma[k]);
      }
		}

		likelihood = ComputeLikelihood(document, model, var_gamma, phi);
    assert(!isnan(likelihood));
    converged = (likelihood_old - likelihood) / likelihood_old;
    likelihood_old = likelihood;

	}

	return likelihood;
}

double Inference::ComputeLikelihood(Document* document,
														 			Model* model,
														 			vector<double>& var_gamma,
														 			vector<vector<double>>& phi) {
  int topic_no = model->getTopicNo();
  double likelihood = 0, digsum = 0, var_gamma_sum = 0, dig[topic_no];
  double alpha = model->getAlpha();
  int length = document->getLength();

  for (int k = 0; k < topic_no; k++) {
    dig[k] = gsl_sf_psi(var_gamma[k]);
    var_gamma_sum += var_gamma[k];
  }
  digsum = gsl_sf_psi(var_gamma_sum);

  likelihood = lgamma(alpha * topic_no) - topic_no * lgamma(alpha) -
               (lgamma(var_gamma_sum));

  for (int k = 0; k < topic_no; k++) {
    likelihood += (alpha - 1) * (dig[k] - digsum) + lgamma(var_gamma[k]) -
                  (var_gamma[k] - 1) * (dig[k] - digsum);

    for (int n = 0; n < length; n++) {
      if (phi[n][k] > 0) {
        likelihood += document->getCount(n) *
                      (phi[n][k] * ((dig[k] - digsum) - log(phi[n][k]) +
                                    model->getLogPrWord(k, n)));
      }
    }
  }
  return likelihood;
}

void Inference::SaveWordAssignment(ofstream& ofs,
																 Document* document,
																 vector<vector<double>>& phi) {
	for (int i = 0; i < document->getLength(); i++) {
		int argmax_topic = distance(begin(phi[i]), max_element(begin(phi[i]), end(phi[i])));
		int word_id = document->getWord(i);
		ofs << word_id << ":" << argmax_topic << " "; 
	}
	ofs << endl;
}

void Inference::SaveGamma(ofstream& ofs,
													vector<double>& gamma) {
	ofs.precision(12);
	for (auto& d : gamma) {
		ofs << d << " ";
	}
	ofs << endl;
}

}  // namespace lda