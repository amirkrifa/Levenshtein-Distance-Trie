#include "LevenshteinDistanceTrie.hh"
#include <vector>

 
LevenshteinDistanceTrie::~LevenshteinDistanceTrie ()
{
  next_t::iterator l_iter;

  while (!m_nextLetters.empty())
  {
    l_iter = m_nextLetters.begin();
    delete l_iter->second;
    m_nextLetters.erase(l_iter);
  }
}

void 
LevenshteinDistanceTrie::insert(const string& p_word)
{
  string l_word(string("$") + p_word);

  int sz = l_word.size();

  LevenshteinDistanceTrie* l_currentNode = this;
  for (int c_char = 0; c_char < sz; ++c_char) 
  {
    if (l_currentNode->m_nextLetters.find(l_word[c_char]) == l_currentNode->m_nextLetters.end()) 
    {
      l_currentNode->m_nextLetters[l_word[c_char]] = new LevenshteinDistanceTrie();
      if(l_currentNode->m_nextLetters[l_word[c_char]] == NULL)
      {
    	exit(3);
      }
    }

    l_currentNode = l_currentNode->m_nextLetters[l_word[c_char]];
  }

  l_currentNode->m_currentWord = l_word;
}


void 
LevenshteinDistanceTrie::searchForMatchingPredicatsRecur(LevenshteinDistanceTrie* p_tree,
    	                                                 const unsigned int&            p_maxDistance,
    	                                                 const char&              p_ch,
    	                                                 const vector<unsigned uint>&    p_lastRow,
    	                                                 const string&            p_word,
    	                                                 unsigned uint&                  p_minCost,
    	                                                 vector<string>&          p_resultVector)
{
  unsigned int sz = p_lastRow.size();

  vector<unsigned int> current_row(sz);
  current_row[0] = p_lastRow[0] + 1;

  // Calculate the min cost of insertion, deletion, match or substitution
  int insert_or_del = 0, replace = 0;

  for (unsigned int c = 1; c < sz; ++c)
  {
    insert_or_del = min(current_row[c - 1] + 1, p_lastRow[c] + 1);
    replace = (p_word[c - 1] == p_ch) ? p_lastRow[c - 1] : (p_lastRow[c - 1] + 1);
    current_row[c] = min(insert_or_del, replace);
  }

  // When we find a cost that is less than the p_minCost, is because
  // it is the minimum until the current row, so we update
    
  if ((current_row[sz-1] < p_minCost) && (p_tree->m_currentWord != ""))
  {
    p_minCost = current_row[sz-1];
    if(p_minCost <=  p_maxDistance)
    {
      p_resultVector.push_back(p_tree->m_currentWord.substr(1));
    }
  }

  // If there is an element which is smaller than the current minimum cost,
  //     we can have another cost smaller than the current minimum cost
  if (*min_element(current_row.begin(), current_row.end()) < p_minCost)
  {
    for (next_t::iterator c_iter = p_tree->m_nextLetters.begin(); c_iter != p_tree->m_nextLetters.end(); ++c_iter)
    {
      searchForMatchingPredicatsRecur(c_iter->second, p_maxDistance, c_iter->first, current_row, p_word, p_minCost, p_resultVector);
    }
  }
}


int
LevenshteinDistanceTrie::searchForMatchingWords(const string& p_word,
                                                const unsigned int& p_maxDistance,
    	                                        vector<string>& p_result)
{
  string l_word = string("$") + p_word;
  int sz = l_word.size();
  vector<unsigned int> current_row(sz + 1);

  // Naive DP initialization
  for (int c = 0; c < sz; ++c)
  {
    current_row[c] = c;
  }

  current_row[sz] = sz;

  unsigned int l_minCost = 0x3f3f3f3f;
  // For each letter in the root map which matches with a
  //     letter in word, we must call the search
  for (int c_char = 0 ; c_char < sz; ++c_char)
  {
    if (m_nextLetters.find(l_word[c_char]) != m_nextLetters.end())
    {
      searchForMatchingPredicatsRecur(m_nextLetters[l_word[c_char]], p_maxDistance, l_word[c_char],
        	                          current_row, l_word, l_minCost, p_result);
    }
  }
    
  return l_minCost;
}
