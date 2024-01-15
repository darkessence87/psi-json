#include "psi/json/JsonParser.h"

#include <iostream>

using namespace psi::json;

int main()
{
    auto jtree = JsonParser::loadFromFile("test.json");
    std::cout << jtree.toString() << std::endl;
    std::cout << jtree.asObject().value()->getString("last_name") << std::endl;
    std::wcout << jtree.asObject().value()->getStringW("last_name") << std::endl;
    std::cout << jtree.asObject().value()->getArray("children").value()->toString() << std::endl;
    std::cout << jtree.asObject().value()->getArrayString("children").back() << std::endl;
}