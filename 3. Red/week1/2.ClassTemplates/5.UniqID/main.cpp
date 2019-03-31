#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define UNIQ_ID g(var, __LINE__)
#define g(a,b) f(a,b) 
#define f(a,b) a##b

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}