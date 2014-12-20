#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;
typedef vector<int> vi; 
typedef int dt; 


class myTrie {

public:

	myTrie() {
		root = new node(); 
	}

	void add(string s) {
		if(counts[s] == 0 && valid(s)) insert(root, s, 0); 
		counts[s]++;
	}
	
	vector<string> completions(string s) {

		node* trav = root;
		vector<string> tbr;
		vector<word> words; 
		int ind = 0;
		while (ind < s.length()) { 
			if (trav->kids[index(s[ind])] == nullptr) {
				return tbr; 
			} else { 
				trav = trav->kids[index(s[ind])];
				ind++; 
			}
		}
		
		string buffer; 
		harvest(trav, tbr, buffer);
		for (auto x: tbr) {
			word w(x, counts[s + x]);
			words.push_back(w); 
		}
		
		sort(words.begin(), words.end(), compare);
		tbr.clear();
		
		for (word x: words) {
			tbr.push_back(x.w); 
		} 
		
		return tbr; 
	}
	
private:

	struct node {
		node* kids[27];
		node(){
			for (int i = 0; i < 28; i++) {
				kids[i] = nullptr; 
			}
		} 
	};
	
	struct word {
		string w;
		int ct;
		word(string s, int count) {
			w = s; ct = count; 
		} 
	};

	static bool compare(word a, word b) {
		return (a.ct < b.ct);
	}

	map<string, int> counts;
	node* root;
	
	bool valid(string s) {
		for (int i = 0; i < s.length(); i++) {
			if (toupper(s[i]) < 'A' || toupper(s[i]) > 'Z') {
				return false; 
			} 
		}
		return true; 
	}
	
	void harvest(node* trav, vector<string>& ends, string buffer) {
		if (trav->kids[26] != nullptr) {
			ends.push_back(buffer);  // one completion
		}
		
		for (int i = 0; i <= index('Z'); ++i) {
			if (trav->kids[i] != nullptr) {
				harvest(trav->kids[i], ends, buffer + character(i)); 
			} 
		} 
	}
	
	void insert(node* trav, const string& s, int ind) {
		if (ind == s.length()) {
			trav->kids[26] = new node();
			return;
		}
		
		if (trav->kids[index(s[ind])] != nullptr) {
			trav = trav->kids[index(s[ind])]; 
		} else {
			trav = trav->kids[index(s[ind])] = new node();
		}
		
		insert(trav, s, ind + 1); 
	}

	int index(char c) { return (int)c - 65; }
	
	char character(int i) { return (i == 26) ? '$' : (char)(i + 65); }
	

};

void input_loop(myTrie& trie) {
	
	string word;
	char c; 
	do {
		cout << ":--> ";
		cin >> word;
		transform(word.begin(), word.end(), word.begin(), ::toupper); 
		for (auto x: trie.completions(word)) cout << word + "-" + ((x == "") ? "$" : x) << endl; 
	} while (word != "exit"); 

}

myTrie read_book(string book) {
	myTrie trie; 
	string word; 
	ifstream input_file(book); 
	while (input_file >> word) {
		transform(word.begin(), word.end(), word.begin(), ::toupper); 
		trie.add(word); 
	}

	return trie; 
}

int main() {
	auto mytrie = read_book("dracula.txt"); 
	input_loop(mytrie);

	return 0; 
} 
