#include <fstream>
#include <sstream>
#include <iostream>

#include "document.h"
#include "corpus.h"
#include "inference.h"

const int BUF_SIZE = 10000;

namespace lda {


// =======================================================================
// CorpusUtils
// =======================================================================

void CorpusUtils::ReadCorpus(
    const string& filename,
    Corpus* corpus) {

  ifstream infile(filename.c_str());
  char buf[BUF_SIZE];

  int doc_no = 0;
  int term_no = 0;
  int total_word_count = 0;
  int words;

  while (infile.getline(buf, BUF_SIZE)) {
    istringstream s_line(buf);
    // Consider each line at a time.
    int word_count_pos = 0;
    Document document;
    while (s_line.getline(buf, BUF_SIZE, ' ')) {
      if (word_count_pos == 0) {
        words = atoi(buf);
      } else {
        int word_id, word_count;
        istringstream s_word_count(buf);
        std::string str;
        getline(s_word_count, str, ':');
        word_id = atoi(str.c_str());
        getline(s_word_count, str, ':');
        word_count = atoi(str.c_str());
        total_word_count += word_count;
        document.addWord(word_id, word_count);
        if (word_id >= term_no) {
          term_no = word_id + 1;
        }
      }
      word_count_pos++;
    }
    document.setLength(word_count_pos - 1);
    document.setTotal(words);
    corpus->addDocument(move(document));
    doc_no += 1;
  }

  infile.close();

  corpus->setTermNo(term_no);
  corpus->setWordNo(total_word_count);
  cout << "Number of documents in corpus: " << doc_no << endl;
  cout << "Number of distinct words in corpus: " << term_no << endl;
  cout << "Number of words in corpus: " << total_word_count << endl;
}

void CorpusUtils::SaveGamma(const string& filename,
												vector<vector<double>>& gamma,
												Corpus* corpus) {
	ofstream ofs(filename);
	int documents = corpus->getDocuments();
	for (int i = 0; i < documents; i++) {
		Inference::SaveGamma(ofs, gamma[i]);
	}
	ofs.close();
}
}  // namespace lda