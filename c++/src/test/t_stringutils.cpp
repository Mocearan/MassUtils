#include "stringutils.h"

#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    cout << CStringUtils::to_string(12) << endl;
    //cout << to_string(12.0)<< endl;
    
    
    cout << CStringUtils::trim("    aaaaaaaaaaddddddd        ") << endl;
    
    
    vector<string> a;
    CStringUtils::split("   asd adsa sdasd   dafas   ", a, " ");
    std::for_each(a.begin(), a.end(), [](const string& i){
        cout << i << endl;
    });


    cout << CStringUtils::join({"aaa", "bbb", "ccc"}, "xx") << endl;
    
    cout << CStringUtils::upper("sadf") << endl;

    cout << CStringUtils::hexchar2int('c') << endl;

    cout << CStringUtils::hex("123", 3) << endl;

    return 0;
}