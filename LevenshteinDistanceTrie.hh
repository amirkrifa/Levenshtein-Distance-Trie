#ifndef LEVENSHTEIN_DISTANCE_TRIE_H_
#define LEVENSHTEIN_DISTANCE_TRIE_H_

#include <map>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

using namespace std;


class LevenshteinDistanceTrie;

typedef map<char, LevenshteinDistanceTrie* > next_t;

// Trie's node used for the calculation of the levenshtein distance
class LevenshteinDistanceTrie: private boost::noncopyable
{
public:
  
  LevenshteinDistanceTrie (){};

  ~LevenshteinDistanceTrie ();

  void insert (const string& p_word);

  //! Returns the set of matching predicats with a Levenshtein distance smaller than p_maxDistance to the considered p_word
  //! This is a recursive algorithm based on the Levenshtein Trie.
  //! @param p_word: (in) the word for which we will look for matching predicats
  //! @param p_maxDistance: (in) the maximum allowed Levenshtein distance
  //! @param p_result: (out) the resulting predicats vector
  //! @return the minimum cost
  int searchForMatchingWords(const string& p_word,
                             const unsigned int& p_maxDistance,
                             vector<string>& p_result);

private:
     
  //!Recursive implementation for the function above
  void searchForMatchingPredicatsRecur(LevenshteinDistanceTrie* p_tree,
                                       const unsigned int32&            p_maxDistance,
                                       const char&              p_ch,
                                       const vector<unsigned int>&    p_lastRow,
                                       const string&            p_word,
                                       unsigned int&                  p_minCost,
                                       vector<string>&          p_resultVector);

  // The set with all the letters which this node is prefix
  next_t m_nextLetters;

  // If word is equal to "" is because there is no word in the
  //     dictionary which ends here.
  string m_currentWord;
};


#endif // LEVENSHTEIN_DISTANCE_TRIE_H_

