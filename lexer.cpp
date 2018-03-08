
/*
********************************************************

Compiler Constrution for C-- Language

********************************************************

Lexical Analyzer

********************************************************

Takes the source code in a file with extension .cm
Generates the tokens

********************************************************
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
#include <conio.h>
using namespace std;

const vector<string> reservedWords = { "int" , "char", "if", "else", "while", "return", "out", "in" };

vector<string> getTokens(vector<char> code) {
	
	vector<string> tokens, errors;
	string token = "";
	int parenCount = 0, braceCount = 0, curlyCount = 0;

	for (int i = 0; i < code.size(); ++i) {
		
		token = "";
		if (isalpha(code[i])) {	// identifier starting

			while (isalpha(code[i]) || isdigit(code[i])) {
				token += code[i++];
			}

			// checking if identifier is a reservedWord or not
			bool check = false;
			for (auto word : reservedWords) {
				if (word == token) {
					check = true;
				}
			}

			if (check) {	// if a reserved word, making uppercase
				for (int i = 0; i < token.size(); ++i) {
					token[i] = toupper(token[i]);
				}
				tokens.push_back(token + ", NULL");
			}
			else {
				tokens.push_back("ID, " + token);
			}

			i--;	// in the docs, at the end of DFA, theres a * which depicts we move the pointer one step back
		}

		else if (isdigit(code[i])) {	// checking for numeric constant
			
			while (isdigit(code[i])) {
				token += code[i++];
			}
			tokens.push_back("NUM, " + token);

			i--;
		}

		else if (code[i] == '+') {
			tokens.push_back("ADD, NULL");
		}

		else if (code[i] == '-') {
			tokens.push_back("SUB, NULL");
		}

		else if (code[i] == '*') {
			
			// keeping end-of-comment */ and multiply * separate
			if (code[i + 1] == '/') {
				tokens.push_back("END_OF_COMMENT, NULL");
			}

			else {
				tokens.push_back("MUL, NULL");
			}
		}

		else if (code[i] == '/') {
			
			// keeping divide / separate from comments // or /* or */
			if (code[i + 1] == '/') {
				
				while (code[i] != '\n') {
					token += code[i++];
				}

				i--;
				tokens.push_back("COMMENT, " + token);
			}

			else if (code[i + 1] == '*') {	// checking for multi-line comment /* */
				
				i += 2;
				while (code[i] != '*' && code[i + 1] != '/') {
					token += code[i++];
				}

				i--;
				tokens.push_back("MULTI_LINE_COMMENT, " + token);
			}

			else {
				tokens.push_back("DIV, NULL");
			}
		}

		else if (code[i] == '\'' && code[i + 2] == '\'') {	// checking for literal constant 'char'
			token += code[i++];
			token += code[i++];
			token += code[i++];
			tokens.push_back("LITERAL, " + token);
		}

		else if (code[i] == '<') {
			
			if (code[i + 1] == '=') {
				tokens.push_back("RO, LE");
			}

			else if (code[i + 1] == '>') {
				tokens.push_back("RO, NE");
			}

			else {
				tokens.push_back("RO, LT");
				i--;
			}

			i++;
		}

		else if (code[i] == '>') {
			
			if (code[i + 1] == '=') {
				tokens.push_back("RO, GE");
				i++;
			}

			else {
				tokens.push_back("RO, GT");
			}
		}

		else if (code[i] == '=') {
			
			if (code[i + 1] == '=') {
				tokens.push_back("RO, EQ");
				i++;
			}

			else {
				tokens.push_back("ASSIGN, NULL");
			}
		}

		else if (code[i] == '(') {
			tokens.push_back("LPAREN, NULL");
			parenCount++;
		}

		else if (code[i] == ')') {
			tokens.push_back("RPAREN, NULL");
			parenCount--;
		}

		else if (code[i] == '[') {
			tokens.push_back("LBRAC, NULL");
			braceCount++;
		}

		else if (code[i] == ']') {
			tokens.push_back("RBRAC, NULL");
			braceCount--;
		}

		else if (code[i] == '{') {
			tokens.push_back("LCURLY, NULL");
			curlyCount++;
		}

		else if (code[i] == '}') {
			tokens.push_back("RCURLY, NULL");
			curlyCount--;
		}

		else if (code[i] == ';') {
			tokens.push_back("SEMICOLON, NULL");
		}

		else if (code[i] == ',') {
			tokens.push_back("COMMA, NULL");
		}

		else if (code[i] != ' ' && code[i] != '\t' && code[i] != '\n') {
			errors.push_back("Error: Invalid character found: " + code[i]);
		}
	}

	if (parenCount) {
		errors.push_back("Error: Parentheseis () are not balanced");
	}
	if (braceCount) {
		errors.push_back("Error: Square Brackets [] are not balanced");
	}
	if (curlyCount) {
		errors.push_back("Error: Curly Brackets {} are not balanced");
	}
	if (tokens.size() == 0) {
		errors.push_back("Error: No source code found.");
	}
	if (errors.size()) {
		return errors;
	}

	return tokens;
}

int main(int argc, char *argv[])
{
	if (argc == 2) {
		string filename = argv[1];
		
		if (filename.find(".cm") != std::string::npos) {
			
			ifstream fin(argv[1]);
			
			if (fin.is_open()) {
				ofstream fout("words.txt");
				ofstream symbol("symboltable.txt");

				vector<char> code;
				vector<string> tokens;

				string line = "";
				
				while (getline(fin, line)) {	// reading lines from source code
					
					for (auto character : line) {	// breaking lines to characters
						code.push_back(character);
					}

					code.push_back('\n');
				}

				/*for (int i = 0; i < code.size(); i++) {	// uncomment to see output of source code provided
					cout << code[i];
				}
				cout << endl << endl;*/

				fin.close();

				tokens = getTokens(code);
				
				/*cout << "TOKENS" << endl; 	// uncomment to see output of tokens generated
				for (auto token : tokens) {
					cout << token << endl;
				}
				cout << endl;*/

				if (fout.is_open()) {	// writing all the tokens generated
					for (auto token : tokens) {
						fout << token << endl;
					}
					fout.close();
				}
				else {
					cout << "Error: Could not open file for writing" << endl;
				}

				if (symbol.is_open()) {	// writing only names of identifiers
					
					for (int token = 0; token < tokens.size(); token++) {
						
						bool duplicate = false;
						if (tokens[token][0] == 'I' && tokens[token][1] == 'D') {
							
							for (int i = 0; i < token; i++) {
								
								if (tokens[i] == tokens[token]) {	// checking for duplicate IDs, no need to print them
									duplicate = true;
								}
							}

							if (!duplicate) {
								
								for (int i = 4; i < tokens[token].length(); i++) {
									symbol << tokens[token][i];
								}
								symbol << endl;
							}
						}
					}
					symbol.close();
				}
				else {
					cout << "Error: Could not open file for writing" << endl;
				}
			}
			else {
				cout << "Error: Could not open file for reading" << endl;
			}
		}
		else {
			cout << "Error: Could not detect file with .cm extension" << endl;
		}
	}
	else {
		cout << "Error: Invalid Argument. Expecting one file with .cm extension." << endl;
	}

	cout << "Press any key to continue" << endl;
	_getch();

	return 0;
}
