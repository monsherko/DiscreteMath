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


#define ZERO	(0)
#define ONE 	(1)
#define TWO 	(2)
#define COUNT 	(13)

using namespace std;


static unordered_map<string, int> vars;
static unordered_map<string, int> unit;
static unordered_map<string, string> keyFormula;

int times = ZERO; int count = ZERO;

class StringtoLexems {
private:
	deque<string> tokens;
	const string sep;
	const int len; int i = ZERO; int l;

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
		return	t == '*' || t == '=' || t == '-'
			|| t == '+' || t == '(' || t == '/'
			|| t == ',' || t == ')' || t == '?'
			|| t == ';' || t == ':' || t == '<'
			|| t == '>';
	}

	void UnitStr(int next, int end) {
		string tok(ONE, sep.at(next++));
		while (next < end) {
			tok += sep.at(next++);
		}
		tokens.push_back(tok);
	}

	void UnitTok(string str) {
		tokens.push_back(str);
	}
	bool lexem() {

		while (i < len) {
			if (isalpha(sep.at(i))) {
				toName();
			}
			else if (isdigit(sep.at(i))) {
				toNumber();
			}
			else if (i + ONE < len && ((sep.at(i) == '<' && sep.at(i + ONE) == '>') ||
				(sep.at(i) == ':'  && sep.at(i + ONE) == '=') ||
				((sep.at(i) == '<' || sep.at(i) == '>') && sep.at(i + ONE) == '='))) {
				UnitStr(i, i + TWO);
				i += TWO;
			}
			else if (value(sep.at(i))) {
				UnitStr(i++, ONE);
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
			deque<string> tpe;
		}
	}

	void errorLex() {
		cout << "syntax error" << endl;
		exit(ZERO);
	}

};


class LexTPars {
public:
	deque<string> lexems;
	vector<string> depends;
	unordered_map<string, unsigned char> valueOfForm;
	unordered_map<string, unsigned char> dependsOfForm;
	string formula;

	int n; bool fl = false;
	LexTPars(deque<string> ghost, string formula_, bool fl_) : lexems(ghost), formula(formula_), fl(fl_) { parsFunc(); }




	string getToken() {
		if (!compare(lexems.size())) {
			string q = lexems.front();
			lexems.pop_front();
			return q;
		}
		else return "";
	}

	int parsFormalList() {
		string lx;
		int i = ZERO; bool flag = false;
		if (!compare(strcmp(lexems.front().c_str(), ")"))) {

			parsExpr();
			i++;
			if (strcmp(lexems.front().c_str(), ")")) {
				for (lx = getToken(); !compare(strcmp(lx.c_str(), ")")); lx = getToken()) {
					if (compare(strcmp(lx.c_str(), ","))) {
						flag = true; i++;
						parsExpr();
					}
					else if (!compare(strcmp(lx.c_str(), ","))) {
						flag = false;
					}
					else {
						cout << lx << endl;
					}
				}

				if (!flag || compare(lexems.size())) {
					errorLex();
				}
			}
			else {
				getToken();
			}
		}
		else {
			getToken();
		}
		fl = true;
		return i;
	}


	void parseAr() {
		parseTerm();
		for (; !compare(lexems.size()) && (compare(strcmp(lexems.front().c_str(), "+")) ||
			compare(strcmp(lexems.front().c_str(), "-"))); getToken(), parseTerm()) {
		}
	}

	void parseTerm() {
		parsFact();
		for (; !compare(lexems.size()) && (compare(strcmp(lexems.front().c_str(), "*")) ||
			compare(strcmp(lexems.front().c_str(), "/"))); getToken(), parsFact()) {
		}
	}

	void parsComp() {
		parseAr();
		if (!compare(lexems.size()) && (compare(strcmp(lexems.front().c_str(), "<=")) || compare(strcmp(lexems.front().c_str(), "<")) ||
			compare(strcmp(lexems.front().c_str(), "=")) || compare(strcmp(lexems.front().c_str(), ">=")) ||
			compare(strcmp(lexems.front().c_str(), ">")) || compare(strcmp(lexems.front().c_str(), "<>")))) {
			getToken(); parseAr();
		}
	}

	void parsFact() {
		string lx = getToken();
		string h = lx;
		if (isdigit(lx.at(ZERO))) {

		}
		else if (isalpha(lx.at(ZERO))) {
			if (fl && !compare(strcmp(lx.c_str(), "("))
				&& !compare(strcmp(lexems.front().c_str(), "("))) {
				if (valueOfForm.find(lx) == valueOfForm.end()) {
					errorLex(); //posle ravno
				}
			}
			else if (!fl &&  !compare(strcmp(lx.c_str(), "("))) {
				if (valueOfForm.find(lx) == valueOfForm.end()) {
					valueOfForm[lx] = COUNT; // do ravno
				}
			}

			if (compare(strcmp(lexems.front().c_str(), "("))) {

				getToken();
				int y = parsFormalList();
				if (vars.find(h) == vars.end()) {
					vars[h] = y;
				}
				else if (vars[h] != y) {
					errorLex();
				}

				if (!fl && unit.find(h) == unit.end()) {
					unit[h] = y;
				}
				else if (!fl && unit[h] != y) {
					errorLex();
				}

				if (dependsOfForm.find(h) == dependsOfForm.end()) {
					depends.push_back(h); dependsOfForm[h] = COUNT;
				}
			}
		}
		else if (compare(strcmp(lx.c_str(), "("))) {
			parsExpr();
			if (compare(strcmp(lexems.front().c_str(), ")"))) {
				getToken();
			}
			else {
				errorLex();
			}
		}
		else if (compare(strcmp(lx.c_str(), "-"))) {
			parsExpr();
		}
		else {
			errorLex();
		}
	}

	void parsFunc() {
		int n__ = ZERO;
		string name = getToken();
		if ((!fl && !isalpha(name.at(ZERO))) ||
			(!compare(strcmp(getToken().c_str(), "(")))) {
			errorLex();
		}

		keyFormula[name] = formula;

		n__ = parsFormalList();

		if (((unit.find(name) != unit.end()) || (vars.find(name) != vars.end() && vars[name] != n__)) ||
			(compare(lexems.size()) || !compare(strcmp(getToken().c_str(), ":=")))) {
			errorLex();
		}

		vars[name] = n__; unit[name] = n__; fl = true;

		parsExpr();
		if (!compare(strcmp(getToken().c_str(), ";"))) {
			errorLex();
		}
	}

	void parsExpr() {
		parsComp();
		if (compare(strcmp(lexems.front().c_str(), "?"))) {
			getToken(); parsComp();
			if (!compare(strcmp(lexems.front().c_str(), ":"))) {
				errorLex();
			}
			getToken(); parsExpr();
		}
	}
	void errorLex() {
		cout << "syntax error" << endl;
		exit(ZERO);
	}


	bool compare(int a) {
		return a == ZERO;
	}

};

class Vertex {
public:
	int i;	int T1 = ZERO; int comp = COUNT; int low;
	vector<Vertex* > edges; vector<string> depends; string formula;

	Vertex(vector<string> depends_, int i_, string formula_) : depends(depends_), i(i_), formula(formula_) {
	}

	void printToDepends() {
		copy(depends.begin(), depends.end(),
			ostream_iterator<string>(cout, "     "));
	}
};

deque<Vertex*> helpStack;

void VisitVertex_Tarjan(Vertex& v) {
	v.T1 = v.low = ::times++;
	helpStack.push_front(&v);
	for (Vertex* tmp : v.edges) {
		if (tmp->T1 == ZERO) {
			VisitVertex_Tarjan(*tmp);
		}
		if (tmp->comp == COUNT && tmp->low < v.low) {
			v.low = tmp->low;
		}
	}
	Vertex* cmp;
	if (v.T1 == v.low && helpStack.size() != ZERO) {
		do {
			(cmp = helpStack.front())->comp = ::count;
			helpStack.pop_front();
		} while (cmp != &v);
		::count++;
	}
}

vector<Vertex> graf;

int main(void) {

	for (int i = 0; cin.peek() != EOF; i++) {
		string str;
		getline(cin, str);
		str += "  ";
		StringtoLexems Lex(str, str.length());
		LexTPars My(Lex.whoIsLexems(), str, false);
		graf.push_back(Vertex(My.depends, i, str));
	}

	for (Vertex& simple : graf) {
		for (string y : simple.depends) {
			if (vars.find(y) == vars.end()) {
				exit(ZERO);
			}

			for (Vertex& cs : graf) {
				if (cs.formula == keyFormula[y] && &cs != &simple) {
					simple.edges.push_back(&cs);	break;
				}
			}
		}
	}


	for (Vertex& simple : graf)
		if (simple.T1 == ZERO)
			VisitVertex_Tarjan(simple);

	cout << ::count << endl;

	return ZERO;
}
