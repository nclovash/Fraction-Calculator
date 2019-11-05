#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <dictionary.h>
#include "fraction.h"
#include "stack.h"

using namespace std;

Fraction evaluate(string); //Reads in and processes input
void scanForVar(string, int&, string&); //Finds dest and '='
string readStrSequence(string, int&); //Reads in variable names
bool isOperator(char); //Determines if char is an operator
void doOp(Stack<char>&, Stack<Fraction>&); //Preforms all operations
bool hasPrecedence(char a, char b); //Determines precedence
unsigned int hash(string); //Quick and dirty hash func.

Stack<char> opStack;
Stack<Fraction> numStack;
Dictionary<string,Fraction> variables;

int main() {
  string input;
  Fraction var;
  while(true){
    cout << "Enter expression: ";
    cin >> input;
    var = evaluate(input);

    cout << var << endl;
  }

  return 0;
}

Fraction evaluate(string expr){
  numStack.clear();
  opStack.clear();
  opStack.push('$');
  int first = 0;
  string dest = "";

  scanForVar(expr, first, dest);

  while(first < expr.length()){
    if(isdigit(expr[first])){
      Fraction tmp;

      while(isdigit(expr[first])){
        tmp = tmp * 10 + (expr[first] - '0');
        first += 1;
      }
      numStack.push(tmp);
    }
    else if(isOperator(expr[first])){
      while(hasPrecedence(opStack.peek(), expr[first]))
        doOp(opStack, numStack);
      opStack.push(expr[first]);
      first += 1;
    }
    else if(isalpha(expr[first]) || expr[first] == '_'){
      string v = readStrSequence(expr, first);
      Fraction f = variables.search(v);
      numStack.push(f);
    }
    else if(expr[first] == '('){
      opStack.push('(');
      first += 1;
    }
    else if(expr[first] == ')'){
      while(opStack.peek() != '('){
        doOp(opStack, numStack);
      }
      opStack.pop();
      first += 1;
    }
    else{
      first += 1;
    }
  }

  while(opStack.peek() != '$'){
    doOp(opStack, numStack);
  }

  Fraction result = numStack.pop();

  if(dest != ""){
    try {
      variables.insert(dest, result);
    }
    catch (DuplicateKeyException e) {
      variables.update(dest,result);
    }
  }

  return result;
}

void scanForVar(string str, int &first, string &dest) {
  int j;

  dest = readStrSequence(str, first);

  for(j = 0; j < str.length(); j++){
    if(str[j] == '=') {
      first = j + 1;
      break;
    }
  }

  if(j == str.length()) //no '=' found
    return;
}

string readStrSequence(string str, int &first) {
  string s ;

  while(isalnum(str[first])){
    s += str[first];
    first += 1;
  }

  return s;
}

bool isOperator(char op){
  if(op == '+')
    return true;

  else if(op == '-')
    return true;

  else if(op == '*')
    return true;

  else if(op == '/')
    return true;

  else
    return false;
}

void doOp(Stack<char>&opStack, Stack<Fraction>&numStack) {
  Fraction L, R, ans;
  char op;
  R = numStack.pop();
  L = numStack.pop();
  op = opStack.pop();

  if(op == '+')
    ans = L + R;

  if(op == '-')
    ans = L - R;

  if(op == '*')
    ans = L * R;

  if(op == '/')
    ans = L / R;

  numStack.push(ans);
}

bool hasPrecedence(char a, char b){
  if(a == '*')
    return true;
  if(a == '/')
    return true;

  if(a == '$')
    return false;
  if(a == '(')
    return false;

  return b == '+' || b == '-';
}

unsigned int hash(string s) {
  unsigned int pos=0;

  for (int i=0;i<s.length();i++)
    pos = pos * i + s[i];

  return pos;
}
