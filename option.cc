#include <fstream>
#include <sstream>
#include <iostream>

#include "option.h"


const int BUF_SIZE = 1000;
const int DEFAULT_VAR_MAX_ITER = 10;
const double DEFAULT_VAR_CONVERGED = 1e-5;
const int DEFAULT_EM_MAX_ITER = 10000;
const double DEFAULT_EM_CONVERGED = 1e-8;
const int DEFAULT_ESTIMATE_ALPHA = 0;


namespace lda {

// =======================================================================
// Option
// =======================================================================

Option& Option::GetInstance() {
	static Option instance;
	return instance;
}


// =======================================================================
// OptionUtils
// =======================================================================

void OptionUtils::ReadOptions(const string& filename) {
	ifstream infile(filename.c_str());
  char buf[BUF_SIZE];

  int var_max_iter = DEFAULT_VAR_MAX_ITER;
  double var_converged = DEFAULT_VAR_CONVERGED;
  int em_max_iter = DEFAULT_EM_MAX_ITER;
  double em_converged = DEFAULT_EM_CONVERGED;
  int estimate_alpha = DEFAULT_ESTIMATE_ALPHA;
  
  int topic_no = 50;
  int est = 0;
  int inf = 0;
  int random = 1;
  int seeded = 0;
  double alpha = 1.0;
  int save_lag = 100;

  string model_root = "model";
  string directory = "result";



  while (infile.getline(buf, BUF_SIZE)) {
    istringstream s_line(buf);
    // Consider each line at a time.
    std::string str;
    getline(s_line, str, ' ');
    std::string value;
    getline(s_line, value, ' ');
    if (str.compare("var_max_iter") == 0) {
      var_max_iter = atoi(value.c_str());
    } else if (str.compare("var_converged") == 0) {
      var_converged = atof(value.c_str());
    } else if (str.compare("em_max_iter") == 0) {
      em_max_iter = atoi(value.c_str());
    } else if (str.compare("em_converged") == 0) {
      em_converged = atof(value.c_str());
    } else if (str.compare("estimate_alpha") == 0) {
      estimate_alpha = atoi(value.c_str());
    } else if (str.compare("topic_no") == 0) {
      topic_no = atoi(value.c_str());
    } else if (str.compare("model_root") == 0) {
      model_root = value;
    } else if (str.compare("directory") == 0) {
      directory = value;
    } else if (str.compare("est") == 0) {
      est = atoi(value.c_str());
    } else if (str.compare("inf") == 0) {
      inf = atoi(value.c_str());
    } else if (str.compare("random") == 0) {
      random = atoi(value.c_str());
    } else if (str.compare("seeded") == 0) {
      seeded = atoi(value.c_str());
    } else if (str.compare("alpha") == 0) {
      alpha = atof(value.c_str());
    } else if (str.compare("save_lag") == 0) {
      save_lag = atoi(value.c_str());
    }
  }

  infile.close();

  Option& option = Option::GetInstance();
 	option.setVarMaxIter(var_max_iter);
 	option.setVarConverged(var_converged);
 	option.setEmMaxIter(em_max_iter);
 	option.setEmConverged(em_converged);
 	option.setEstimateAlpha(estimate_alpha);
 	option.setTopicNo(topic_no);
 	option.setModelRoot(model_root);
 	option.setDirectory(directory);
 	option.setEstimate(est);
 	option.setInference(inf);
 	option.setSeeded(seeded);
 	option.setRandom(random);
 	option.setAlpha(alpha);
 	option.setSaveLag(save_lag);


 	cout << "var_max_iter : " << var_max_iter << endl;
 	cout << "var_converged : " << var_converged << endl;
 	cout << "em_max_iter : " << em_max_iter << endl;
 	cout << "em_converged : " << em_converged << endl;
 	cout << "estimate_alpha : " << estimate_alpha << endl;
}

}  // namespace lda