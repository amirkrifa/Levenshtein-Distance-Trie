#include "TranslateUTF8Accents.hh"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>


string
TranslateUTF8Accents::utf8Encode(const string & w_src)
{
  string dst;

  dst.reserve(w_src.size() * 2);
  for (const char *p = w_src.c_str(); *p; p++)
  {
    if ((unsigned) (*p) < 0x80)
    {
      dst += *p;
    }
    else if ((unsigned) (*p) < 0xC0)
    {
      char encoded = 0xC2;
      dst += encoded;
      dst += *p;
    }
    else
    {
      char encoded = 0xC3;
      dst += encoded;
      encoded = *p - 0x40;
      dst += encoded;
    }
  }
  dst.resize(dst.size());
  return dst;
}


string
TranslateUTF8Accents::utf8Decode(const string & w_src)
{
  string dst;

  dst.reserve(w_src.size());
  for (const char *p = w_src.c_str(); *p; p++)
  {
    if (*p == (char) (0xC2))
    {
      p++;
      dst += *p;
    }
    else if (*p == (char) (0xC3))
    {
      p++;
      char decoded = *p + 0x40;
      dst += decoded;
    }
    else
    {
      dst += *p;
    }
  }
  dst.resize(dst.size());
  return dst;
}

TranslateUTF8Accents::TranslateUTF8Accents()
{
  m_charactersMap.insert(make_pair(utf8Decode("À")[0], 'A'));
  m_charactersMap.insert(make_pair(utf8Decode("Á")[0], 'A'));
  m_charactersMap.insert(make_pair(utf8Decode("Â")[0], 'A'));
  m_charactersMap.insert(make_pair(utf8Decode("Ã")[0], 'A'));
  m_charactersMap.insert(make_pair(utf8Decode("Ä")[0], 'A'));
  m_charactersMap.insert(make_pair(utf8Decode("Å")[0], 'A'));

  m_charactersMap.insert(make_pair(utf8Decode("à")[0], 'a'));
  m_charactersMap.insert(make_pair(utf8Decode("á")[0], 'a'));
  m_charactersMap.insert(make_pair(utf8Decode("â")[0], 'a'));
  m_charactersMap.insert(make_pair(utf8Decode("ã")[0], 'a'));
  m_charactersMap.insert(make_pair(utf8Decode("ä")[0], 'a'));
  m_charactersMap.insert(make_pair(utf8Decode("å")[0], 'a'));

  m_charactersMap.insert(make_pair(utf8Decode("Ò")[0], 'O'));
  m_charactersMap.insert(make_pair(utf8Decode("Ó")[0], 'O'));
  m_charactersMap.insert(make_pair(utf8Decode("Ô")[0], 'O'));
  m_charactersMap.insert(make_pair(utf8Decode("Õ")[0], 'O'));
  m_charactersMap.insert(make_pair(utf8Decode("Ö")[0], 'O'));
  m_charactersMap.insert(make_pair(utf8Decode("Ø")[0], 'O'));

  m_charactersMap.insert(make_pair(utf8Decode("ò")[0], 'o'));
  m_charactersMap.insert(make_pair(utf8Decode("ó")[0], 'o'));
  m_charactersMap.insert(make_pair(utf8Decode("ô")[0], 'o'));
  m_charactersMap.insert(make_pair(utf8Decode("õ")[0], 'o'));
  m_charactersMap.insert(make_pair(utf8Decode("ö")[0], 'o'));
  m_charactersMap.insert(make_pair(utf8Decode("ø")[0], 'o'));

  m_charactersMap.insert(make_pair(utf8Decode("È")[0], 'E'));
  m_charactersMap.insert(make_pair(utf8Decode("É")[0], 'E'));
  m_charactersMap.insert(make_pair(utf8Decode("Ê")[0], 'E'));
  m_charactersMap.insert(make_pair(utf8Decode("Ë")[0], 'E'));

  m_charactersMap.insert(make_pair(utf8Decode("è")[0], 'e'));
  m_charactersMap.insert(make_pair(utf8Decode("é")[0], 'e'));
  m_charactersMap.insert(make_pair(utf8Decode("ê")[0], 'e'));
  m_charactersMap.insert(make_pair(utf8Decode("ë")[0], 'e'));

  m_charactersMap.insert(make_pair(utf8Decode("Ç")[0], 'C'));
  m_charactersMap.insert(make_pair(utf8Decode("ç")[0], 'c'));

  m_charactersMap.insert(make_pair(utf8Decode("Ì")[0], 'I'));
  m_charactersMap.insert(make_pair(utf8Decode("Í")[0], 'I'));
  m_charactersMap.insert(make_pair(utf8Decode("Î")[0], 'I'));
  m_charactersMap.insert(make_pair(utf8Decode("Ï")[0], 'I'));

  m_charactersMap.insert(make_pair(utf8Decode("ì")[0], 'i'));
  m_charactersMap.insert(make_pair(utf8Decode("í")[0], 'i'));
  m_charactersMap.insert(make_pair(utf8Decode("î")[0], 'i'));
  m_charactersMap.insert(make_pair(utf8Decode("ï")[0], 'i'));

  m_charactersMap.insert(make_pair(utf8Decode("Ù")[0], 'U'));
  m_charactersMap.insert(make_pair(utf8Decode("Ú")[0], 'U'));
  m_charactersMap.insert(make_pair(utf8Decode("Û")[0], 'U'));
  m_charactersMap.insert(make_pair(utf8Decode("Ü")[0], 'U'));

  m_charactersMap.insert(make_pair(utf8Decode("ù")[0], 'u'));
  m_charactersMap.insert(make_pair(utf8Decode("ú")[0], 'u'));
  m_charactersMap.insert(make_pair(utf8Decode("û")[0], 'u'));
  m_charactersMap.insert(make_pair(utf8Decode("ü")[0], 'u'));

  m_charactersMap.insert(make_pair(utf8Decode("ÿ")[0], 'y'));

  m_charactersMap.insert(make_pair(utf8Decode("Ñ")[0], 'N'));

  m_charactersMap.insert(make_pair(utf8Decode("ñ")[0], 'n'));
}

void
TranslateUTF8Accents::translate (char& p_input)
{
  map<char, char>::const_iterator l_iter = m_charactersMap.find(p_input);
  if(l_iter == m_charactersMap.end())
  {
    p_input = l_iter->second;
  }
}

void
TranslateUTF8Accents::removeUTF8Accents (string& p_data)
{
  p_data = utf8Decode(p_data);

  for (uint32 c_char = 0; c_char < p_data.length(); ++c_char)
  {
    translate(p_data[c_char]);
  }
}
