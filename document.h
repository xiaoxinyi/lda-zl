#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <vector>

using namespace  std;

namespace lda {

class Document {
public:
	int getLength() const { return length_; }
	void setLength(int length) { length_ = length; }

	int getTotal() const { return total_; }
	void setTotal(int total) { total_ = total; }

	int getWord(int i) const { return words_[i]; }
	void addWord(int word_id, int count) { 
		words_.push_back(word_id);
		counts_.push_back(count);
 }

 	void setWords(vector<int>&& words) { words_ = move(words); }
 	void setCounts(vector<int>&& counts) { counts_ = move(counts); }

	int getCount(int i) const { return counts_[i]; }
private:
	// Word ids.
	vector<int> words_;

	// Word counts.
	vector<int> counts_;

	// Distinct word number.
	int length_;

	// Total words.
	int total_;

};


}  // namespace lda


#endif // DOCUMENT_H_
