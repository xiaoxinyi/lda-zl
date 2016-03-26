#include <cmath>

#include <gsl/gsl_sf_psi.h>


#include "estimate.h"
#include "inference.h"
#include "document.h"

#define NEWTON_THRESH 1e-5
#define MAX_ALPHA_ITER 1000

namespace lda {

// =======================================================================
// Estimate
// =======================================================================

void Estimate::MStep(Model* model, SuffStats* ss) {

	int topic_no = model->getTopicNo();
	int term_no = model->getTermNo();

	for (int k = 0; k < topic_no; k++) {
		for (int w = 0; w < term_no; w++) {
			if (ss->getClassWord(k, w) > 0) {
				double log_pr_word = log(ss->getClassWord(k, w)) - log(ss->getClassTotal(k));
				model->setLogPrWord(k, w, log_pr_word);
			} else {
				model->setLogPrWord(k, w, -100);
			}
		}
	}
}

double Estimate::EStepForDocument(
								Document* document,
								vector<double>& gamma,
								vector<vector<double>>& phi,
								Model* model,
								SuffStats* ss) {
	double likelihood = 0.0;
	int topic_no = model->getTopicNo();
	int length = document->getLength();


  // posterior inference
  likelihood = Inference::LdaInference(document, model, gamma, phi);

  // update sufficient statistics

  double gamma_sum = 0;
  for (int k = 0; k < topic_no; k++) {
    gamma_sum += gamma[k];
    ss->updateAlphaSS(gsl_sf_psi(gamma[k]));
  }
  ss->updateAlphaSS(-topic_no * gsl_sf_psi(gamma_sum));

  for (int n = 0; n < length; n++) {
    for (int k = 0; k < topic_no; k++) {
      ss->updateClassWord(k, n, document->getCount(n) * phi[n][k]);
      ss->updateClassTotal(k , document->getCount(n) * phi[n][k]);
    }
  }

  ss->updateDocumentNo(1);

  return likelihood;
}

void Estimate::EstimateAlpha(Model* model, SuffStats* ss) {
	double alpha =
        opt_alpha(ss->getAlphaSS(), ss->getDocumentNo(), model->getTopicNo());
  model->setAlpha(alpha);
}

double alhood(double a, double ss, int D, int K)
{ return(D * (lgamma(K * a) - K * lgamma(a)) + (a - 1) * ss); }

double d_alhood(double a, double ss, int D, int K)
{ return(D * (K * gsl_sf_psi(K * a) - K * gsl_sf_psi(a)) + ss); }

double d2_alhood(double a, int D, int K)
{ return(D * (K * K * gsl_sf_psi_1(K * a) - K * gsl_sf_psi_1(a))); }


/*
 * newtons method
 *
 */

double opt_alpha(double ss, int D, int K)
{
    double a, log_a, init_a = 100;
    double f, df, d2f;
    int iter = 0;

    log_a = log(init_a);
    do
    {
        iter++;
        a = exp(log_a);
        if (isnan(a))
        {
            init_a = init_a * 10;
            printf("warning : alpha is nan; new init = %5.5f\n", init_a);
            a = init_a;
            log_a = log(a);
        }
        f = alhood(a, ss, D, K);
        df = d_alhood(a, ss, D, K);
        d2f = d2_alhood(a, D, K);
        log_a = log_a - df/(d2f * a + df);
        printf("alpha maximization : %5.5f   %5.5f\n", f, df);
    }
    while ((fabs(df) > NEWTON_THRESH) && (iter < MAX_ALPHA_ITER));
    return(exp(log_a));
}



}  // namespace lda