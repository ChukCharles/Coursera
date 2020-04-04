#include <iostream>
#include <vector>
#include <string>
#include "profile.h"

using namespace std;

string MakeHeavyString() {
    return string(100000000, 'a');
}

int main()
{
    {
        LOG_DURATION("assignment, with variable");
        string target = "old value";
        string source = MakeHeavyString();  //string(100000000, 'a');
        target = source;
    }
    {
        LOG_DURATION("assignment, without variable");
        string target = "old value";
        target = MakeHeavyString();
    }
    {
        LOG_DURATION("assignment, without fuction");
        string target = "old value";
        target = string(100000000, 'a');
    }

}
