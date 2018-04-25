#include "textstats.hpp"
#include <algorithm>

using namespace std;

void get_tokens(  const string &s, const unordered_set<char> &delimiters,  vector<string> &tokens) {
    int i = 0, len, onset;
        for (len = s.length(), onset = 0; i <= len; i++) {
                if (delimiters.find(s[i]) != delimiters.end() || s[i] == '\0' ) {
                        if (onset == i) {
                                onset++;
                        }

                        else {
                                string tmp = s.substr(onset, i - onset);
                                for(int len_tmp = tmp.length(); len_tmp >= 0; tmp[len_tmp] = (char) tolower(tmp[len_tmp--])) {}
                                tokens.push_back(tmp);
                                onset = -~i;
                        }
                }
        }
}


void get_type_freq(const vector<string> &tokens,  map<string, int> &freqdi) {
    for(auto &p : tokens)
        freqdi[p]++;
}

void get_types( const vector<string> &tokens, vector<string> &wtypes) {
   for (auto &p : tokens) {
        if (find(wtypes.begin(), wtypes.end(), p) == wtypes.end())
            wtypes.push_back(p);
   }
   sort(wtypes.begin(), wtypes.end());
}


void get_x_length_words( const vector<string> &wtypes, int x, vector<string> &words ) {
    for(auto &p : wtypes)
       if ( p.length() >= x ) words.push_back(p);
}

void get_x_freq_words( const map<string, int> &freqdi, int x, vector<string> &words) {
   for ( auto &p : freqdi ) {
       if (p.second >= x )
           words.push_back(p.first);
   }
}

void get_words_by_length_dict( const vector<string> &wtypes,  map<int, vector<string> > &lengthdi) {
    for(auto &p : wtypes) {
        int len = p.length();
        if(lengthdi.find(len) == lengthdi.end()) {
            vector<string> src;
            lengthdi.insert(pair<int, vector<string>>(len,  src));
            lengthdi[len].push_back(p);
        }

        else
            lengthdi[len].push_back(p);
    }
}
