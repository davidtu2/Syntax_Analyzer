/*
main.cpp

CPSC 323, Spring 2016, Prof. Song Choi, Assignment 2

NAME:			David Tu, Section 2 (david.tu2@csu.fullerton.edu)
DESCRIPTION:	Rat16 Compiler - Syntax Analyzer implementation (Assignment 2)
REVISIONS:		Version 1
MODIFIED BY:	2016/03/29
*/

#include "LexicalAnalyzer.h"

LexicalAnalyzer NewLA;
bool printSwitch = true;

//prototypes
void rat16S();
void optionalFunctionDefinitions();
void functionDefinitions();
void functionDefinitionsPrime();
void function();
void optionalParameterList();
void parameterList();
void parameterListPrime();
void parameter();
void qualifier();
void body();
void optionalDeclarationList();
void declarationList();
void declarationListPrime();
void declaration();
void IDs();
void IDsPrime();
void statementList();
void statementListPrime();
void statement();
void compound();
void assign();
void ifRule();
void ifRulePrime();
void returnRule();
void returnRulePrime();
void write();
void read();
void whileRule();
void condition();
void relop();
void expression();
void expressionPrime();
void term();
void termPrime();
void factor();
void primary();
void primaryPrime();
void empty();

int main()
{
	if (NewLA.readFileOpen()) {
		NewLA.lexer();
		rat16S();
	}
	else {
		cout << "ERROR: Cannot open the file" << endl;
		exit(1);
	}

	NewLA.closeReadFile();
	NewLA.closeOutputFile();

	return 0;
}

void rat16S() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Rat16S> -> <Opt Function Definitions> $$ <Opt Declaration List> $$ <Statement List> $$" << endl;
	}

	optionalFunctionDefinitions();

	if (NewLA.getLexeme() == "$$") {
		NewLA.lexer();
		optionalDeclarationList();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 87: $$ expected. Received Token: " <<NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}

	if (NewLA.getLexeme() == "$$") {
		NewLA.lexer();
		statementList();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 98: $$ expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}

	if (NewLA.getLexeme() == "$$") {
		NewLA.myOutputFile << "End of Source" << endl;
	}
	else {
		NewLA.myOutputFile << "ERROR at line 108: Source doesn't end with $$. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void optionalFunctionDefinitions() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Opt Function Definitions> -> <Function Definitions> | <Empty>" << endl;
	}

	if (NewLA.getLexeme() == "function") {
		functionDefinitions();
	}
	else {
		empty();
	}
}

void functionDefinitions() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Function Definitions> -> <Function> <Function Definitions'>" << endl;
	}

	function();
	functionDefinitionsPrime();
}

void functionDefinitionsPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Function Definitions'> -> <Empty> | <Funtion Definitions>" << endl;
	}

	if (NewLA.getLexeme() == "$$") {
		empty();
	}
	else {
		functionDefinitions();
	}
}

void function() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
	}

	if (NewLA.getLexeme() == "function") {
		NewLA.lexer();
		if (NewLA.getToken() == "Identifier") {
			if (printSwitch) {
				NewLA.myOutputFile << "<Identifier> -> " << NewLA.getLexeme() << endl;
			}
			NewLA.lexer();
			if (NewLA.getLexeme() == "(") {
				NewLA.lexer();
				optionalParameterList();
				if (NewLA.getLexeme() == ")") {
					NewLA.lexer();
					optionalDeclarationList();
					body();
				}
				else {
					NewLA.myOutputFile << "ERROR at line 171: ) expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
					NewLA.closeReadFile();
					NewLA.closeOutputFile();
					exit(1);
				}
			}
			else {
				NewLA.myOutputFile << "ERROR at line 178: ( expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 185: Identifier expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 192: 'function' expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void optionalParameterList() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Opt Parameter List> -> <Parameter List> | <Empty>" << endl;
	}

	if (NewLA.getToken() == "Identifier") {
		parameterList();
	}
	else {
		empty();
	}
}

void parameterList() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Parameter List> -> <Parameter> <Parameter List'>" << endl;
	}

	parameter();
	parameterListPrime();
}

void parameterListPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Parameter List'> -> <Empty> | , <Parameter List>" << endl;
	}

	if (NewLA.getLexeme() == ",") {
		NewLA.lexer();
		parameterList();
	}
	else {
		empty();
	}
}

void parameter() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Parameter> -> <IDs> : <Qualifier>" << endl;
	}

	IDs();
	if (NewLA.getLexeme() == ":") {
		NewLA.lexer();
		qualifier();
	}
}

void qualifier() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Qualifier> -> integer | boolean | real" << endl;
	}

	if (NewLA.getLexeme() == "integer") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Qualifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "boolean") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Qualifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "real") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Qualifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 271: integer, boolean or real expected. Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void body() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Body> -> { <Statement List> }" << endl;
	}

	if (NewLA.getLexeme() == "{") {
		NewLA.lexer();
		statementList();
		if (NewLA.getLexeme() == "}") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 290: } expected in body(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 297: { expected in body(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void optionalDeclarationList() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Opt Declaration List> -> <Declaration List> | <Empty>" << endl;
	}

	if (NewLA.getLexeme() == "integer" | NewLA.getLexeme() == "boolean" | NewLA.getLexeme() == "real") {
		declarationList();
	}
	else {
		empty();
	}
}

void declarationList() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Declaration List> -> <Declaration> ; <Declaration List'>" << endl;
	}

	declaration();
	if (NewLA.getLexeme() == ";") {
		NewLA.lexer();
		declarationListPrime();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 328: ; expected at end of declaration at declarationList(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void declarationListPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Declaration List'> -> <Empty> | <Declaration List>" << endl;
	}

	if (NewLA.getLexeme() == "integer" | NewLA.getLexeme() == "boolean" | NewLA.getLexeme() == "real") {
		declarationList();
	}
	else {
		empty();
	}
}

void declaration() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Declaration> -> <Qualifier> <IDs>" << endl;
	}

	qualifier();
	IDs();
}

void IDs() {
	if (printSwitch) {
		NewLA.myOutputFile << "<IDs> -> <Identifier> <IDs'>" << endl;
	}

	if (NewLA.getToken() == "Identifier") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Identifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
		IDsPrime();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 370: Identifier expected at IDs(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void IDsPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<IDs'> -> <Empty> | , <IDs>" << endl;
	}

	if (NewLA.getLexeme() == ",") {
		NewLA.lexer();
		IDs();
	}
	else {
		empty();
	}
}

void statementList() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Statement List> -> <Statement> <Statement List'>" << endl;
	}

	statement();
	statementListPrime();
}

void statementListPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Statement List'> -> <Empty> | <Statement List>" << endl;
	}

	if (NewLA.getLexeme() == "$$" | NewLA.getLexeme() == "}") {
		empty();
	}
	else {
		statementList();
	}
}

void statement() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>" << endl;
	}

	if (NewLA.getLexeme() == "{") {
		compound();
	}
	else if (NewLA.getToken() == "Identifier") {
		assign();
	}
	else if (NewLA.getLexeme() == "if") {
		ifRule();
	}
	else if (NewLA.getLexeme() == "return") {
		returnRule();
	}
	else if (NewLA.getLexeme() == "printf") {
		write();
	}
	else if (NewLA.getLexeme() == "scanf") {
		read();
	}
	else if (NewLA.getLexeme() == "while") {
		whileRule();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 440: {, Identifier, if, return, printf, scanf, or while expected at statement(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void compound() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Compound> -> { <Statement List> }" << endl;
	}

	if (NewLA.getLexeme() == "{") {
		NewLA.lexer();
		statementList();
		if (NewLA.getLexeme() == "}") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 495: } expected at compound(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 466: { expected at compound(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void assign() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Assign> -> <Identifier> := <Expression> ;" << endl;
	}

	if (NewLA.getToken() == "Identifier") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Identifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
		if (NewLA.getLexeme() == ":=") {
			NewLA.lexer();
			expression();
			if (NewLA.getLexeme() == ";") {
				NewLA.lexer();
			}
			else {
				NewLA.myOutputFile << "ERROR at line 490: ; expected at assign(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 497: := expected at assign(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 504: Identifier expected at assign(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void ifRule() {
	if (printSwitch) {
		NewLA.myOutputFile << "<If> -> if ( <Condition> ) <Statement> <If'>" << endl;
	}

	if (NewLA.getLexeme() == "if") {
		NewLA.lexer();
		if (NewLA.getLexeme() == "(") {
			NewLA.lexer();
			condition();
			if (NewLA.getLexeme() == ")") {
				NewLA.lexer();
				statement();
				ifRulePrime();
			}
			else {
				NewLA.myOutputFile << "ERROR at line 527: ) expected at ifRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 534: ( expected at ifRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 541: if expected at ifRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void ifRulePrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<If'> -> endif | else <Statement> endif" << endl;
	}

	if (NewLA.getLexeme() == "endif") {
		NewLA.lexer();
	} else if(NewLA.getLexeme() == "else") {
		NewLA.lexer();
		statement();
		if (NewLA.getLexeme() == "endif") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 562: endif expected at ifRulePrime(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 569: endif or else expected at ifRulePrime(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void returnRule() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Return> -> return <Return'>" << endl;
	}

	if (NewLA.getLexeme() == "return") {
		NewLA.lexer();
		returnRulePrime();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 586: return expected in returnRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void returnRulePrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Return'> -> ; | <Expression> ;" << endl;
	}

	if (NewLA.getLexeme() == ";") {
		NewLA.lexer();
	}
	else {
		expression();
		if (NewLA.getLexeme() == ";") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 607: ; expected in returnRulePrime(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
}

void write() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Write> -> printf ( <Expression> ) ;" << endl;
	}

	if (NewLA.getLexeme() == "printf") {
		NewLA.lexer();
		if (NewLA.getLexeme() == "(") {
			NewLA.lexer();
			expression();
			if (NewLA.getLexeme() == ")") {
				NewLA.lexer();
				if (NewLA.getLexeme() == ";") {
					NewLA.lexer();
				}
				else {
					NewLA.myOutputFile << "ERROR at line 631: ; expected in write(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
					NewLA.closeReadFile();
					NewLA.closeOutputFile();
					exit(1);
				}
			}
			else {
				NewLA.myOutputFile << "ERROR at line 638: ) expected in write(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 645: ( expected in write(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 652: printf expected in write(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void read() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Read> -> scanf ( <IDs> ) ;" << endl;
	}

	if (NewLA.getLexeme() == "scanf") {
		NewLA.lexer();
		if (NewLA.getLexeme() == "(") {
			NewLA.lexer();
			IDs();
			if (NewLA.getLexeme() == ")") {
				NewLA.lexer();
				if (NewLA.getLexeme() == ";") {
					NewLA.lexer();
				}
				else {
					NewLA.myOutputFile << "ERROR at line 675: ; expected in read(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
					NewLA.closeReadFile();
					NewLA.closeOutputFile();
					exit(1);
				}
			}
			else {
				NewLA.myOutputFile << "ERROR at line 682: ) expected in read(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 689: ( expected in read(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 696: scanf expected in read(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}

}

void whileRule() {
	if (printSwitch) {
		NewLA.myOutputFile << "<While> -> while ( <Condition> ) <Statement>" << endl;
	}

	if (NewLA.getLexeme() == "while") {
		NewLA.lexer();
		if (NewLA.getLexeme() == "(") {
			NewLA.lexer();
			condition();
			if (NewLA.getLexeme() == ")") {
				NewLA.lexer();
				statement();
			}
			else {
				NewLA.myOutputFile << "ERROR at line 719: ) expected at whileRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
				NewLA.closeReadFile();
				NewLA.closeOutputFile();
				exit(1);
			}
		}
		else {
			NewLA.myOutputFile << "ERROR at line 726: ( expected at whileRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		NewLA.myOutputFile << "ERROR at line 733: while expected at whileRule(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void condition() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Condition> -> <Expression> <Relop> <Expression>" << endl;
	}

	expression();
	relop();
	expression();
}

void relop() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Relop> -> = | /= | > | < | => | <=" << endl;
	}

	if (NewLA.getLexeme() == "=") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> "<< NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "/=") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == ">") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "<") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "=>") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "<=") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Relop> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 792: =, /=, >, <, =>, or <= expected at relop(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void expression() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Expression> -> <Term> <Expression'>" << endl;
	}

	term();
	expressionPrime();
}

void expressionPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Expression'> -> + <Term> <Expression'> | - <Term> <Expression'> | <Empty>" << endl;
	}

	if (NewLA.getLexeme() == "+" | NewLA.getLexeme() == "-") {
		NewLA.lexer();
		term();
		expressionPrime();
	}
	else {
		empty();
	}
}

void term() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Term> -> <Factor> <Term'>" << endl;
	}

	factor();
	termPrime();
}

void termPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | <Empty>" << endl;
	}

	if (NewLA.getLexeme() == "*" | NewLA.getLexeme() == "/") {
		NewLA.lexer();
		factor();
		termPrime();
	}
	else {
		empty();
	}
}

void factor() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Factor> -> - <Primary> | <Primary>" << endl;
	}

	if (NewLA.getLexeme() == "-") {
		NewLA.lexer();
		primary();
	}
	else {
		primary();
	}
}

void primary() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Primary> -> <Identifier> <Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false" << endl;
	}

	if (NewLA.getToken() == "Identifier") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Identifier> -> " << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
		primaryPrime();
	}
	else if (NewLA.getToken() == "Integer") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Integer> ->" << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "(") {
		NewLA.lexer();
		expression();
		if (NewLA.getLexeme() == ")") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 886: ) expected after <Expression> at primary(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else if (NewLA.getToken() == "Real") {
		if (printSwitch) {
			NewLA.myOutputFile << "<Real> ->" << NewLA.getLexeme() << endl;
		}
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "true") {
		NewLA.lexer();
	}
	else if (NewLA.getLexeme() == "false") {
		NewLA.lexer();
	}
	else {
		NewLA.myOutputFile << "ERROR at line 905: Identifier, Integer, (, Real, true, or false expected at primary(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
		NewLA.closeReadFile();
		NewLA.closeOutputFile();
		exit(1);
	}
}

void primaryPrime() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Primary'> -> <Empty> | ( <IDs> )" << endl;
	}

	if (NewLA.getLexeme() == "(") {
		NewLA.lexer();
		IDs();
		if (NewLA.getLexeme() == ")") {
			NewLA.lexer();
		}
		else {
			NewLA.myOutputFile << "ERROR at line 924: ) expected after <IDs> at primaryPrime(). Received Token: " << NewLA.getToken() << " and Lexeme: " << NewLA.getLexeme() << endl;
			NewLA.closeReadFile();
			NewLA.closeOutputFile();
			exit(1);
		}
	}
	else {
		empty();
	}
}

void empty() {
	if (printSwitch) {
		NewLA.myOutputFile << "<Empty> -> <Empty>" << endl;
	}
}