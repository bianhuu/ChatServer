#include <iostream>
#include <vector>
#include <map>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

void func1()
{
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "what are you doing now?";

    cout << js.dump() << endl;
}

int main()
{
    func1();
    return 0;
}