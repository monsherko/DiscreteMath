#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iterator>
#include <cstdio>


void errorLex();
using namespace std;

static unordered_map<string, int> vars;


class StringtoLexems {
private:
	deque<string> tokens;
	const string sep;
	const int len;
	int i = 0; int l;

	void toName() {
		for (l = i; i < len; i++) {
			if (!isalnum(sep.at(i))) {
				UnitStr(l, i);
				break;
			}
		}
	}

	void toNumber() {
		for (l = i; i < len; i++) {
			if (isalpha(sep.at(i))) {
				errorLex();
			}
			if (!isdigit(sep.at(i))) {
				UnitStr(l, i);
				break;
			}
		}
	}

	bool value(char t) {
		return(t == '*' || t == '=' || t == '-'
			|| t == '+' || t == '(' || t == '/'
			|| t == ',' || t == ')') ? true : false;
	}

	void UnitStr(int next, int end) {
		string tok(1, sep.at(next));
		for (next++; next < end; next++) {
			tok += sep.at(next);
		}
		tokens.push_back(tok);

	}
	bool lexem() {

		while (i < len) {
			if (isalpha(sep.at(i))) {
				toName();
			}
			else if (isdigit(sep.at(i))) {
				toNumber();
			}
			else if (value(sep.at(i))) {
				UnitStr(i, 1);
				i++;
			}
			else if (isspace(sep.at(i))) {
				i++;
			}
			else {
				return false;
			}

		}

		return true;
	}
public:

	StringtoLexems(string sep_, int len_) : sep(sep_), len(len_) {}

	deque<string> whoIsLexems() {
		if (lexem()) {
			return tokens;
		}
		else {
			errorLex();
		}
	}

	string getform() {
		return sep;
	}

	void printVe() {
		for (auto it : tokens)
			cout << it << endl;
	}



};

class LexTPars {
public:
	deque<string> lexems;
	vector<string> depends;
	int n;
	LexTPars(deque<string> ghost, int n_) : lexems(ghost), n(n_) {}

	void parse() {

		int x = parseVars();


		if (x < 1) {
			errorLex();
		}

		while (lexems.size() != 0) {
			parseExpr();
			string lx = getToken();
			if (lx.length() != 0 &&
				(lx.at(0) != ',' || lexems.size() == 0)) {
				errorLex();
			}
			x--;

		}
		if (x != 0) {
			errorLex();
		}
	}

	void printLexems() {
		for (auto str : lexems) {
			cout << str << "  ";
		}
		cout << endl;
	}

private:

	string getToken() {
		if (lexems.size() != 0) {
			string q = lexems.front();
			lexems.pop_front();
			return q;
		}
		else return "";
	}

	int parseVars() {
		string lx = getToken();
		int ans = 0;


		do {
			if (isalpha(lx.at(0))) {
				auto search = vars.find(lx);
				if (search != vars.end()) {
					errorLex();
					//èíèöèàëèçèðîâàíà
				}
				vars[lx] = n;
				ans++;
				lx = getToken();
				if (lexems.size() == 0)
					errorLex();
				else if (lx.at(0) == ',' && isalpha(lexems.front().at(0))) {
					lx = getToken();
				}
				else if (lx.at(0) == '=') {
					break;
				}
			}
			else {
				errorLex();
			}
		} while (lx.at(0) != '=');

		return ans;
	}
	void parseExpr() {
		parseTerm();
		for (; lexems.size() != 0 && (lexems.front().at(0) == '+' || lexems.front().at(0) == '-');) {
			getToken();
			parseTerm();
		}
	}

	void parseTerm() {
		parsePower();
		for (; lexems.size() != 0 && (lexems.front().at(0) == '*' || lexems.front().at(0) == '/'); ) {
			getToken();
			parsePower();
		}
	}

	void parsePower() {

		if (lexems.size() == 0) {
			errorLex();
    }
		string  lx = getToken();

		if (isdigit(lx.at(0))) {
			return;
		} else if (isalpha(lx.at(0))) {
			depends.push_back(lx);
    }	else if (lx.at(0) == '-') {
			parsePower();
		} else if (lx.at(0) == '(') {
			parseExpr();

			if (lexems.size() == 0 || getToken().at(0) != ')') {
        errorLex();
      }
		}
		else if (!isdigit(lx.at(0))) {
			errorLex();
		}
	}

};

class Vertex {
public:
	int mark = 1;
  int i;
	vector<Vertex* > edges; vector<string> depends;	string formula;

	Vertex(vector<string> depends_, int i_, string formula_) : depends(depends_), i(i_), formula(formula_) {}

};

std::ostream &operator<<(std::ostream &os, Vertex const m) {
	return os << m.formula;
}


static 	vector<Vertex > ans;
vector<Vertex> g;

void dfs(Vertex &v) {
	v.mark = 0;
	for (Vertex* u : v.edges) {
		if (u->mark == 0) {
			cout << "cycle" << endl;
			exit(0);
		}
		else if (u->mark == 1) {
			dfs(*u);
		}
	}
	v.mark = -1;
	ans.push_back(v);
}

void errorLex() {
	cout << "syntax error" << endl;
	exit(0);
}


int main() {

	for (int i = 0; (cin.peek() != EOF); ++i) {
		string tmp;
		getline(cin, tmp);
		tmp += "  ";
		StringtoLexems Lert(tmp, tmp.length());
		LexTPars Ver(Lert.whoIsLexems(), i);
		Ver.parse();
		g.push_back(Vertex(Ver.depends, i, tmp));
	}


	for (Vertex& x : g) {
		for (string y : x.depends) {
			if (vars.find(y) == vars.end())
				errorLex();
			x.edges.push_back(&g.at(vars[y]));
		}
	}


	for (Vertex &v : g) {
		if (v.mark == 1) {
			dfs(v);
		}
	}


	std::copy(ans.begin(), ans.end(),
		std::ostream_iterator<Vertex>(cout, "\n"));

	return 0;
}
