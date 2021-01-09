#include <iostream>
#include <SharedPtr.hpp>

struct example
{
  int a;
  std::string b;
  example() : a(0) {}
};

int main() {
  SharedPtr<example> ptr1(new example);
  SharedPtr<example> ptr2(ptr1);
  SharedPtr<example> ptr3(ptr2);
  SharedPtr<example> ptr4(new example);
  ptr3 = ptr4;
  ptr2 = ptr3;
  ptr1.print_info();
  ptr2.print_info();
  ptr4.print_info();
  ptr1 = std::move(ptr3);
  ptr1.print_info();
  ptr2.print_info();
  ptr4.print_info();
  std::cout << "for all ptr's ______" << std::endl;
  ptr1.print_info();
  ptr2.print_info();
  ptr4.print_info();
  return 0;
}
