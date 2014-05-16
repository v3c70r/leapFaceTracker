#include <string>
#include <vector>
#include <iostream>

#ifndef SPLITSTRING_H
#define SPLITSTRING_H
using namespace std;


class splitstring : public string {
    vector<string> flds;
public:
    splitstring(char *s) : string(s) { };
    splitstring(string s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};
#endif
