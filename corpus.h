#ifndef CORPUS_H_
#define CORPUS_H_

#include <vector>

#include "document.h"

using namespace std;

namespace lda {

class Corpus {
public:
	int getDocuments() const { return documents_.size(); }
	void addDocument(Document&& document) {
		documents_.emplace_back(move(document));
	}
	void addDocument() {
		documents_.emplace_back(Document());
	}

	Document* getMutableDocument(int i) { return &documents_[i]; }

	int getTermNo() const { return term_no_; }
	void setTermNo(int term_no) { term_no_ = term_no; }

	int getWordNo() const { return word_no_; }
	void setWordNo(int word_no) { word_no_ = word_no; }

private:
	// Documents in corpus.
	vector<Document> documents_;

	// Vocabulary size.
	int term_no_;

	// Word total no.
	int word_no_;

};

class CorpusUtils {
public:
	static void ReadCorpus(const string& filename,
												 Corpus* corpus);

	static void SaveGamma(const string& filename,
												vector<vector<double>>& gamma,
												Corpus* corpus);
};

}  // namespace lda

#endif // CORPUS_H_
