#include <string>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
#define GLUE2(x) id##x
#define GLUE1(x) GLUE2(x)
#define UNIQ_ID GLUE1(__LINE__)
// Реализуйте этот макрос так, чтобы функция main компилировалась
#line 1 "logger.cpp"
int main() {
    int UNIQ_ID = 0;
    string UNIQ_ID = "hello";
    vector<string> UNIQ_ID = {"hello", "world"};
    vector<int> UNIQ_ID = {1, 2, 3, 4};
}