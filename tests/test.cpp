// Copyright 2018 Your Name <your_email>
#include "SharedPtr.hpp"
#include <gtest/gtest.h>

class object {
  int a;
  std::string b;
 public:
  object() : a(0) {}
  friend bool operator == (const object& right, const object& left) {
    if (right.a != left.a) return false;
    if (right.b != left.b) return false;
    return true;
  }
};

template <class T>
bool check_function(SharedPtr<T>& ptr, const int& refs, T* object) {
  if (ptr.get() != object) return false;
  if (static_cast<int>(ptr.use_count()) != refs) return false;
  return true;
}

TEST(Constructing, Test_1){
  auto ptr_to_object = new object;
  SharedPtr ptr_1(ptr_to_object);
  EXPECT_TRUE(check_function<object>(ptr_1, 1, ptr_to_object));
  SharedPtr ptr_2(ptr_1);
  SharedPtr ptr_3(ptr_2);
  EXPECT_TRUE(check_function(ptr_2, 3, ptr_to_object));
  EXPECT_TRUE(check_function(ptr_3, 3, ptr_to_object));
  SharedPtr ptr_4(std::move(ptr_1));
  EXPECT_TRUE(check_function(ptr_4, 3, ptr_to_object));
  SharedPtr<object> ptr_5;
}

TEST(Assignment, Test_2) {
  auto ptr_to_object = new object;
  SharedPtr ptr_1(ptr_to_object);
  SharedPtr<object> ptr_2; // new to count(0)
  ptr_2 = ptr_1; // count of ptr_2 didn't deleted
  EXPECT_TRUE(check_function(ptr_2, 2, ptr_to_object));
  SharedPtr<object> ptr_3;
}

TEST(Storage, Test_3) {
  auto ptr_to_object = new object;
  SharedPtr ptr_1(ptr_to_object);
  EXPECT_TRUE(ptr_1);
  SharedPtr<object> ptr_2;
  EXPECT_FALSE(ptr_2);
  EXPECT_EQ(*ptr_1, *ptr_to_object);
  EXPECT_EQ(ptr_1.operator->(), ptr_to_object);
}

TEST(Controlling, Tets_4) {
  auto ptr_to_object = new object;
  SharedPtr ptr_1(ptr_to_object);
  EXPECT_EQ(ptr_1.get(), ptr_to_object);
  auto ptr_to_object_2 = new object;
  ptr_1.reset(ptr_to_object_2); // ptr_to_object deleted
  EXPECT_TRUE(check_function(ptr_1, 1, ptr_to_object_2));
  auto ptr_to_object_3 = new object;
  SharedPtr ptr_2(ptr_to_object_3);
  SharedPtr ptr_3(ptr_2);
  ptr_2.swap(ptr_1);
  EXPECT_TRUE(check_function(ptr_2, 1, ptr_to_object_2));
  EXPECT_TRUE(check_function(ptr_1, 2, ptr_to_object_3));
}

TEST(Combinated_test, Test_5) {
  auto ptr_to_object = new object;
  SharedPtr ptr_1(ptr_to_object);
  EXPECT_TRUE(ptr_1);
  ptr_1.reset();  // object deleted
  EXPECT_FALSE(ptr_1);
  ptr_to_object = new object;
  SharedPtr ptr_2(ptr_to_object);
  ptr_1 = ptr_2;
  EXPECT_TRUE(check_function(ptr_1, 2, ptr_to_object));
  EXPECT_TRUE(check_function(ptr_2, 2, ptr_to_object));
  for (int i = 0; i < 5; i++) {
    SharedPtr ptr_3(ptr_1);
    EXPECT_TRUE(check_function(ptr_3, 3, ptr_to_object));
  }
  SharedPtr ptr_4(std::move(ptr_2));  // ptr_2 is no more valuable;
  // ptr_4 & ptr_1 is valuable
  EXPECT_TRUE(check_function(ptr_4, 2, ptr_to_object));
}
