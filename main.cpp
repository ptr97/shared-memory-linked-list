#include <iostream>
#include "List.h"
#include <string>


int main(int argc, char const * argv[])
{
  std::cout << "Shared memory linked list - UNIX 2nd project" << std::endl;
  List<std::string> list = List<std::string>();
  list.add("Haskell");
  list.add("Scala");
  list.add("Clojure");
  list.add("Swift");
  list.add("Kotlin");

  list.print();
  list.serialize("database.db");

  List<std::string> deserializedList = List<std::string>();
  deserializedList.deserialize("database.db");
  deserializedList.print();

  return 0;
}
