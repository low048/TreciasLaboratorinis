#include "googletest/include/gtest/gtest.h"
#include "Vector.h"
#include <string>
#include <algorithm>

// Testų rinkinio šablonas Vector klasės testams
class VectorTest : public ::testing::Test {
protected:
    Vector<int> vec;

    void SetUp() override {
        vec = {1, 2, 3, 4, 5};
    }
};

// Testuoja numatytąjį konstruktorių
TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

// Testuoja dydžio konstruktorių
TEST_F(VectorTest, SizeConstructor) {
    Vector<int> v(5, 10);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], 10);
    }
}

// Testuoja kopijavimo konstruktorių
TEST_F(VectorTest, CopyConstructor) {
    Vector<int> v(vec);
    EXPECT_EQ(v.size(), vec.size());
    EXPECT_EQ(v.capacity(), vec.capacity());
    for (int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], vec[i]);
    }
}

// Testuoja perkėlimo konstruktorių
TEST_F(VectorTest, MoveConstructor) {
    Vector<int> movedVec(std::move(vec));
    EXPECT_EQ(movedVec.size(), 5);
    EXPECT_EQ(movedVec.capacity(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(movedVec[i], i + 1);
    }
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.capacity(), 0);
}

// Testuoja priskyrimo sąrašo konstruktorių
TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// Testuoja kopijavimo priskyrimo operatorių
TEST_F(VectorTest, CopyAssignmentOperator) {
    Vector<int> v;
    v = vec;
    EXPECT_EQ(v.size(), vec.size());
    EXPECT_EQ(v.capacity(), vec.capacity());
    for (int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], vec[i]);
    }
}

// Testuoja perkėlimo priskyrimo operatorių
TEST_F(VectorTest, MoveAssignmentOperator) {
    Vector<int> movedVec;
    movedVec = std::move(vec);
    EXPECT_EQ(movedVec.size(), 5);
    EXPECT_EQ(movedVec.capacity(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(movedVec[i], i + 1);
    }
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.capacity(), 0);
}

// Testuoja priskyrimo metodą su kiekiu ir reikšme
TEST_F(VectorTest, AssignCountValue) {
    vec.assign(3, 10);
    EXPECT_EQ(vec.size(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(vec[i], 10);
    }
}

// Testuoja priskyrimo metodą su diapazonu
TEST_F(VectorTest, AssignRange) {
    Vector<int> values = {6, 7, 8};
    vec.assign(values.begin(), values.end());
    EXPECT_EQ(vec.size(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(vec[i], values[i]);
    }
}

// Testuoja priskyrimo metodą su priskyrimo sąrašu
TEST_F(VectorTest, AssignInitializerList) {
    vec.assign({10, 20, 30});
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

// Testuoja get_allocator metodą
TEST_F(VectorTest, GetAllocator) {
    std::allocator<int> alloc = vec.get_allocator();
    EXPECT_NE(&alloc, nullptr);
}

// Testuoja elementų prieigos metodus
TEST_F(VectorTest, ElementAccess) {
    EXPECT_EQ(vec.at(0), 1);
    EXPECT_EQ(vec.at(4), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
    EXPECT_EQ(vec.front(), 1);
    EXPECT_EQ(vec.back(), 5);
    EXPECT_EQ(vec.data()[0], 1);
}

// Testuoja prieigos už ribų metodą
TEST_F(VectorTest, OutOfRangeAccess) {
    EXPECT_THROW(vec.at(5), std::out_of_range);
}

// Testuoja iteratorių metodus
TEST_F(VectorTest, Iterators) {
    EXPECT_EQ(*vec.begin(), 1);
    EXPECT_EQ(*(vec.end() - 1), 5);
    EXPECT_EQ(*vec.rbegin(), 5);
    EXPECT_EQ(*(vec.rend() - 1), 1);
    EXPECT_EQ(vec.cbegin(), vec.begin());
    EXPECT_EQ(vec.cend(), vec.end());
    EXPECT_EQ(vec.crbegin(), vec.rbegin());
    EXPECT_EQ(vec.crend(), vec.rend());
}

// Testuoja talpos metodus
TEST_F(VectorTest, CapacityMethods) {
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_GT(vec.max_size(), 0);
    vec.reserve(10);
    EXPECT_EQ(vec.capacity(), 10);
    vec.shrink_to_fit();
    EXPECT_EQ(vec.capacity(), 5);
}

// Testuoja clear metodą
TEST_F(VectorTest, Clear) {
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}

// Testuoja insert metodus
TEST_F(VectorTest, Insert) {
    vec.insert(vec.begin(), 0);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[0], 0);

    vec.insert(vec.end(), 6);
    EXPECT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[6], 6);

    vec.insert(vec.begin() + 3, 2, 99);
    EXPECT_EQ(vec.size(), 9);
    EXPECT_EQ(vec[3], 99);
    EXPECT_EQ(vec[4], 99);

    Vector<int> toInsert = {7, 8};
    vec.insert(vec.end(), toInsert.begin(), toInsert.end());
    EXPECT_EQ(vec.size(), 11);
    EXPECT_EQ(vec[9], 7);
    EXPECT_EQ(vec[10], 8);
}

// Testuoja erase metodus
TEST_F(VectorTest, Erase) {
    vec.erase(vec.begin());
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 2);

    vec.erase(vec.begin() + 1, vec.begin() + 3);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 5);
}

// Testuoja emplace metodus
TEST_F(VectorTest, Emplace) {
    vec.emplace(vec.begin(), 0);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[0], 0);

    vec.emplace_back(6);
    EXPECT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[6], 6);
}

// Testuoja push_back ir pop_back metodus
TEST_F(VectorTest, PushPopBack) {
    vec.push_back(6);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[5], 6);

    vec.pop_back();
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.back(), 5);
}

// Testuoja resize metodą
TEST_F(VectorTest, Resize) {
    vec.resize(3);
    EXPECT_EQ(vec.size(), 3);

    vec.resize(5, 10);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[3], 10);
    EXPECT_EQ(vec[4], 10);
}

// Testuoja swap metodą
TEST_F(VectorTest, Swap) {
    Vector<int> other = {10, 20, 30};
    vec.swap(other);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);

    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other[0], 1);
    EXPECT_EQ(other[1], 2);
    EXPECT_EQ(other[2], 3);
    EXPECT_EQ(other[3], 4);
    EXPECT_EQ(other[4], 5);
}

// Testuoja palyginimo operatorius
TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {1, 2, 3};
    Vector<int> v3 = {1, 2, 4};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);

    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);

    EXPECT_FALSE(v1 < v2);
    EXPECT_TRUE(v1 < v3);

    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 <= v3);

    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v1 > v3);

    EXPECT_TRUE(v1 >= v2);
    EXPECT_FALSE(v1 >= v3);
}

#if __cplusplus >= 202002L
// Testuoja trijų krypčių palyginimo operatorių
TEST_F(VectorTest, ThreeWayComparisonOperator) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {1, 2, 3};
    Vector<int> v3 = {1, 2, 4};

    EXPECT_EQ(v1 <=> v2, std::strong_ordering::equal);
    EXPECT_EQ(v1 <=> v3, std::strong_ordering::less);
    EXPECT_EQ(v3 <=> v1, std::strong_ordering::greater);
}
#endif

#if __cplusplus >= 202002L
// Testuoja erase ir erase_if metodus
TEST_F(VectorTest, EraseEraseIf) {
    Vector<int> v = {1, 2, 3, 4, 5, 6};

    auto erased_count = erase(v, 3);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(erased_count, 1);
    EXPECT_EQ(std::count(v.begin(), v.end(), 3), 0);

    auto erased_if_count = erase_if(v, [](int val) { return val % 2 == 0; });
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(erased_if_count, 3);
    EXPECT_EQ(std::count_if(v.begin(), v.end(), [](int val) { return val % 2 == 0; }), 0);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);
}
#endif

#ifdef __cpp_lib_containers_ranges
// Testuoja assign_range, insert_range ir append_range metodus
TEST_F(VectorTest, RangeOperations) {
    Vector<int> v;

    Vector<int> source = {10, 20, 30};

    v.assign_range(source);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);

    v.insert_range(v.begin(), source);
    EXPECT_EQ(v.size(), 6);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);

    v.append_range(source);
    EXPECT_EQ(v.size(), 9);
    EXPECT_EQ(v[6], 10);
    EXPECT_EQ(v[7], 20);
    EXPECT_EQ(v[8], 30);
}
#endif
