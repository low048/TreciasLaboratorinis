#include <iostream>
#include <string>
#include <numeric>
#include <cassert>
#include <iomanip>
#include <complex>
#include <list>
#include <algorithm>
#include "Vector.h"

#ifdef __cpp_lib_containers_ranges
#include <ranges>
#endif

// Function to print the contents of the Vector
template<typename T>
std::ostream& operator<<(std::ostream& s, const Vector<T>& v) {
    s.put('{');
    for (char comma[]{'\0', ' ', '\0'}; const auto& e : v)
        s << comma << e, comma[0] = ',';
    return s << "}\n";
}

// Helper function to print std::initializer_list<int>
std::ostream& operator<<(std::ostream& s, std::initializer_list<int> il) {
    s.put('{');
    for (auto it = il.begin(); it != il.end(); ++it) {
        if (it != il.begin()) s << ", ";
        s << *it;
    }
    return s << '}';
}

int main() {
    // std::vector<T,Allocator>::vector
    std::cout << "std::vector<T,Allocator>::vector Example\n";

    // C++11 initializer list syntax:
    Vector<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "1: " << words1;

    // words2 == words1
    Vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "2: " << words2;

    // words3 == words1
    Vector<std::string> words3(words1);
    std::cout << "3: " << words3;

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    Vector<std::string> words4(5, "Mo");
    std::cout << "4: " << words4;

    auto const rg = {"cat", "cow", "crow"};
    Vector<std::string> words5(rg.begin(), rg.end()); // overload (5)
    std::cout << "5: " << words5;

    // std::vector<T,Allocator>::operator=
    std::cout << "\nstd::vector<T,Allocator>::operator= Example\n";

    auto print = [](const auto& comment, const auto& container) {
        std::cout << comment << container << '\n';
    };

    Vector<int> x{1, 2, 3}, y, z;
    const auto w = {4, 5, 6, 7};

    std::cout << "Initially:\n";
    print("x = ", x);
    print("y = ", y);
    print("z = ", z);

    std::cout << "Copy assignment copies data from x to y:\n";
    y = x;
    print("x = ", x);
    print("y = ", y);

    std::cout << "Move assignment moves data from x to z, modifying both x and z:\n";
    z = std::move(x);
    print("x = ", x);
    print("z = ", z);

    std::cout << "Assignment of initializer_list w to z:\n";
    z = w;
    print("w = ", w);
    print("z = ", z);

    // std::vector<T,Allocator>::assign
    std::cout << "\nstd::vector<T,Allocator>::assign Example\n";

    Vector<char> characters;
    auto print_vector = [&]() {
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';
    };

    characters.assign(5, 'a');
    print_vector();

    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_vector();

    characters.assign({'C', '+', '+', '1', '1'});
    print_vector();

    // std::vector<T,Allocator>::at
    std::cout << "\nstd::vector<T,Allocator>::at Example\n";

    Vector<int> data = {1, 2, 4, 5, 5, 6};
    data.at(1) = 88;

    std::cout << "Element at index 2 has value " << data.at(2) << '\n';
    std::cout << "data size = " << data.size() << '\n';

    try {
        data.at(6) = 666;
    } catch (const std::out_of_range& exc) {
        std::cout << exc.what() << '\n';
    }

    std::cout << "data:";
    for (int elem : data)
        std::cout << ' ' << elem;
    std::cout << '\n';

    // std::vector<T,Allocator>::operator[]
    std::cout << "\nstd::vector<T,Allocator>::operator[] Example\n";

    Vector<int> numbers1{2, 4, 6, 8};
    std::cout << "Second element: " << numbers1[1] << '\n';

    numbers1[0] = 5;
    std::cout << "All numbers:";
    for (auto i : numbers1)
        std::cout << ' ' << i;
    std::cout << '\n';

    // std::vector<T,Allocator>::front
    std::cout << "\nstd::vector<T,Allocator>::front Example\n";

    Vector<char> letters_char{'a', 'b', 'c', 'd', 'e', 'f'};
    if (!letters_char.empty())
        std::cout << "The first character is '" << letters_char.front() << "'.\n";

    // std::vector<T,Allocator>::back
    std::cout << "\nstd::vector<T,Allocator>::back Example\n";

    if (!letters_char.empty())
        std::cout << "The last character is '" << letters_char.back() << "'.\n";

    // std::vector<T,Allocator>::data
    std::cout << "\nstd::vector<T,Allocator>::data Example\n";

    Vector<int> container{1, 2, 3, 4};
    auto pointer_func = [](const int* p, std::size_t size) {
        std::cout << "data = ";
        for (std::size_t i = 0; i < size; ++i)
            std::cout << p[i] << ' ';
        std::cout << '\n';
    };

    pointer_func(container.data(), container.size());

    // std::vector<T,Allocator>::begin, std::vector<T,Allocator>::cbegin
    std::cout << "\nstd::vector<T,Allocator>::begin, std::vector<T,Allocator>::cbegin Example\n";

    Vector<int> nums{1, 2, 4, 8, 16};
    Vector<std::string> fruits{"orange", "apple", "raspberry"};
    Vector<char> empty;

    std::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';

    std::cout << "Sum of nums: " << std::accumulate(nums.begin(), nums.end(), 0) << '\n';

    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.begin() << '\n';

    if (empty.begin() == empty.end())
        std::cout << "vector 'empty' is indeed empty.\n";

    // std::vector<T,Allocator>::end, std::vector<T,Allocator>::cend
    std::cout << "\nstd::vector<T,Allocator>::end, std::vector<T,Allocator>::cend Example\n";

    std::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';

    std::cout << "Sum of nums: " << std::accumulate(nums.begin(), nums.end(), 0) << '\n';

    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.begin() << '\n';

    if (empty.begin() == empty.end())
        std::cout << "vector 'empty' is indeed empty.\n";

    // std::vector<T,Allocator>::rbegin, std::vector<T,Allocator>::crbegin
    std::cout << "\nstd::vector<T,Allocator>::rbegin, std::vector<T,Allocator>::crbegin Example\n";

    std::for_each(nums.rbegin(), nums.rend(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';

    std::cout << "Sum of nums: " << std::accumulate(nums.rbegin(), nums.rend(), 0) << '\n';

    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.rbegin() << '\n';

    if (empty.rbegin() == empty.rend())
        std::cout << "vector 'empty' is indeed empty.\n";

    // std::vector<T,Allocator>::rend, std::vector<T,Allocator>::crend
    std::cout << "\nstd::vector<T,Allocator>::rend, std::vector<T,Allocator>::crend Example\n";

    std::for_each(nums.rbegin(), nums.rend(), [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';

    std::cout << "Sum of nums: " << std::accumulate(nums.rbegin(), nums.rend(), 0) << '\n';

    if (!fruits.empty())
        std::cout << "First fruit: " << *fruits.rbegin() << '\n';

    if (empty.rbegin() == empty.rend())
        std::cout << "vector 'empty' is indeed empty.\n";

    // std::vector<T,Allocator>::empty
    std::cout << "\nstd::vector<T,Allocator>::empty Example\n";

    Vector<int> numbers2;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers2.empty() << '\n';

    numbers2.push_back(42);
    std::cout << "After adding elements, numbers.empty(): " << numbers2.empty() << '\n';

    // std::vector<T,Allocator>::size
    std::cout << "\nstd::vector<T,Allocator>::size Example\n";

    Vector<int> nums_size{1, 3, 5, 7};
    std::cout << "nums contains " << nums_size.size() << " elements.\n";

    // std::vector<T,Allocator>::max_size
    std::cout << "\nstd::vector<T,Allocator>::max_size Example\n";

    Vector<char> p;
    Vector<long> q;

    //std::cout.imbue(std::locale("en_US.UTF-8")); //kažkodėl sukelia "silent error"
    std::cout << std::uppercase
              << "p.max_size() = " << std::dec << p.max_size() << " = 0x"
              << std::hex << p.max_size() << '\n'
              << "q.max_size() = " << std::dec << q.max_size() << " = 0x"
              << std::hex << q.max_size() << '\n';

    std::cout << std::dec;

    // std::vector<T,Allocator>::reserve
    std::cout << "\nstd::vector<T,Allocator>::reserve Example\n";

    constexpr int max_elements = 32;

    std::cout << "using reserve: \n";
    {
        Vector<int> v1;
        v1.reserve(max_elements);

        for (int n = 0; n < max_elements; ++n)
            v1.push_back(n);
    }

    std::cout << "not using reserve: \n";
    {
        Vector<int> v1;

        for (int n = 0; n < max_elements; ++n) {
            if (v1.size() == v1.capacity())
                std::cout << "size() == capacity() == " << v1.size() << '\n';
            v1.push_back(n);
        }
    }

    // std::vector<T,Allocator>::capacity
    std::cout << "\nstd::vector<T,Allocator>::capacity Example\n";

    int sz = 100;
    Vector<int> v;

    auto cap = v.capacity();
    std::cout << "Initial size: " << v.size() << ", capacity: " << cap << '\n';

    std::cout << "\nDemonstrate the capacity's growth policy."
                 "\nSize:  Capacity:  Ratio:\n" << std::left;
    while (sz-- > 0) {
        v.push_back(sz);
        if (cap != v.capacity()) {
            std::cout << std::setw(7) << v.size()
                      << std::setw(11) << v.capacity()
                      << std::setw(10) << v.capacity() / static_cast<float>(cap) << '\n';
            cap = v.capacity();
        }
    }

    std::cout << "\nFinal size: " << v.size() << ", capacity: " << v.capacity() << '\n';

    // std::vector<T,Allocator>::shrink_to_fit
    std::cout << "\nstd::vector<T,Allocator>::shrink_to_fit Example\n";

    Vector<int> shrink_v;
    std::cout << "Default-constructed capacity is " << shrink_v.capacity() << '\n';
    shrink_v.resize(100);
    std::cout << "Capacity of a 100-element vector is " << shrink_v.capacity() << '\n';
    shrink_v.resize(50);
    std::cout << "Capacity after resize(50) is " << shrink_v.capacity() << '\n';
    shrink_v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << shrink_v.capacity() << '\n';
    shrink_v.clear();
    std::cout << "Capacity after clear() is " << shrink_v.capacity() << '\n';
    shrink_v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << shrink_v.capacity() << '\n';
    for (int i = 1000; i < 1300; ++i)
        shrink_v.push_back(i);
    std::cout << "Capacity after adding 300 elements is " << shrink_v.capacity() << '\n';
    shrink_v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << shrink_v.capacity() << '\n';

    // std::vector<T,Allocator>::clear
    std::cout << "\nstd::vector<T,Allocator>::clear Example\n";

    auto print_info = [](std::string_view rem, const Vector<int>& v) {
        std::cout << rem << "{ ";
        for (const auto& value : v)
            std::cout << value << ' ';
        std::cout << "}\n";
        std::cout << "Size=" << v.size() << ", Capacity=" << v.capacity() << '\n';
    };

    Vector<int> container_clear{1, 2, 3};
    print_info("Before clear: ", container_clear);
    container_clear.clear();
    print_info("After clear: ", container_clear);

    // std::vector<T,Allocator>::insert
    std::cout << "\nstd::vector<T,Allocator>::insert Example\n";

    auto print_insert = [](int id, const Vector<int>& container) {
        std::cout << id << ". ";
        for (const int x : container)
            std::cout << x << ' ';
        std::cout << '\n';
    };

    Vector<int> c1(3, 100);
    print_insert(1, c1);

    auto it = c1.begin();
    it = c1.insert(it, 200);
    print_insert(2, c1);

    c1.insert(it, 2, 300);
    print_insert(3, c1);

    it = c1.begin();
    Vector<int> c2(2, 400);
    c1.insert(std::next(it, 2), c2.begin(), c2.end());
    print_insert(4, c1);

    int arr[] = {501, 502, 503};
    c1.insert(c1.begin(), arr, arr + std::size(arr));
    print_insert(5, c1);

    c1.insert(c1.end(), {601, 602, 603});
    print_insert(6, c1);

    // std::vector<T,Allocator>::emplace
    std::cout << "\nstd::vector<T,Allocator>::emplace Example\n";

    struct A {
        std::string s;
        A(std::string str) : s(std::move(str)) { std::cout << " constructed\n"; }
        A(const A& o) : s(o.s) { std::cout << " copy constructed\n"; }
        A(A&& o) noexcept : s(std::move(o.s)) { std::cout << " move constructed\n"; }
        A& operator=(const A& other) {
            s = other.s;
            std::cout << " copy assigned\n";
            return *this;
        }
        A& operator=(A&& other) noexcept {
            s = std::move(other.s);
            std::cout << " move assigned\n";
            return *this;
        }
    };

    Vector<A> container_emplace;
    container_emplace.reserve(10);
    std::cout << "construct 2 times A:\n";
    A two { "two" };
    A three { "three" };

    std::cout << "emplace:\n";
    container_emplace.emplace(container_emplace.end(), "one");

    std::cout << "emplace with A&:\n";
    container_emplace.emplace(container_emplace.end(), two);

    std::cout << "emplace with A&&:\n";
    container_emplace.emplace(container_emplace.end(), std::move(three));

    std::cout << "content:\n";
    for (const auto& obj : container_emplace)
        std::cout << ' ' << obj.s;
    std::cout << '\n';

    // std::vector<T,Allocator>::erase
    std::cout << "\nstd::vector<T,Allocator>::erase Example\n";

    auto print_container = [](const Vector<int>& c) {
        for (int i : c)
            std::cout << i << ' ';
        std::cout << '\n';
    };

    Vector<int> c_erase{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(c_erase);

    c_erase.erase(c_erase.begin());
    print_container(c_erase);

    c_erase.erase(c_erase.begin() + 2, c_erase.begin() + 5);
    print_container(c_erase);

    for (auto it = c_erase.begin(); it != c_erase.end(); ) {
        if (*it % 2 == 0)
            it = c_erase.erase(it);
        else
            ++it;
    }
    print_container(c_erase);

    // std::vector<T,Allocator>::push_back
    std::cout << "\nstd::vector<T,Allocator>::push_back Example\n";

    Vector<std::string> letters_str;
    letters_str.push_back("abc");
    std::string s{"def"};
    letters_str.push_back(std::move(s));

    std::cout << "std::vector letters_str holds: ";
    for (auto&& e : letters_str)
        std::cout << std::quoted(e) << ' ';

    std::cout << "\nMoved-from string s holds: " << std::quoted(s) << '\n';

    // std::vector<T,Allocator>::emplace_back
    std::cout << "\nstd::vector<T,Allocator>::emplace_back Example\n";

    struct President {
        std::string name;
        std::string country;
        int year;
        President(std::string p_name, std::string p_country, int p_year)
            : name(std::move(p_name)), country(std::move(p_country)), year(p_year) {
            std::cout << "I am being constructed.\n";
        }
        President(President&& other) noexcept
            : name(std::move(other.name)), country(std::move(other.country)), year(other.year) {
            std::cout << "I am being moved.\n";
        }
        President& operator=(const President& other) = default;
    };

    Vector<President> elections;
    std::cout << "emplace_back:\n";
    auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
    assert(ref.year == 1994);

    Vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

    std::cout << "\nContents:\n";
    for (const President& president : elections)
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";

    for (const President& president : reElections)
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";

    // std::vector<T,Allocator>::pop_back
    std::cout << "\nstd::vector<T,Allocator>::pop_back Example\n";

    Vector<int> numbers_pop;
    auto print_pop = [](const auto& xs) {
        std::cout << "[ ";
        for (const auto& x : xs)
            std::cout << x << ' ';
        std::cout << "]\n";
    };

    print_pop(numbers_pop);
    numbers_pop.push_back(5);
    numbers_pop.push_back(3);
    numbers_pop.push_back(4);
    print_pop(numbers_pop);

    numbers_pop.pop_back();
    print_pop(numbers_pop);

    // std::vector<T,Allocator>::resize
    std::cout << "\nstd::vector<T,Allocator>::resize Example\n";

    auto print_resize = [](auto rem, const Vector<int>& c) {
        std::cout << rem;
        for (auto it = c.begin(); it != c.end(); ++it) {
            const int el = *it;
            std::cout << el << ' ';
        }
        std::cout << '\n';
    };

    Vector<int> c_resize = {1, 2, 3};
    print_resize("The vector holds: ", c_resize);

    c_resize.resize(5);
    print_resize("After resize up to 5: ", c_resize);

    c_resize.resize(2);
    print_resize("After resize down to 2: ", c_resize);

    c_resize.resize(6, 4);
    print_resize("After resize up to 6 (initializer = 4): ", c_resize);

    // std::vector<T,Allocator>::swap
    std::cout << "\nstd::vector<T,Allocator>::swap Example\n";

    auto print_swap = [](const auto& os, const auto& co) {
        os << '{';
        for (auto const& i : co)
            os << ' ' << i;
        return os << " } ";
    };

    Vector<int> a1{1, 2, 3}, a2{4, 5};
    auto it1 = std::next(a1.begin());
    auto it2 = std::next(a2.begin());
    int& ref1 = a1.front();
    int& ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

    // std::vector<T,Allocator>::assign_range
#ifdef __cpp_lib_containers_ranges
    std::cout << "\nstd::vector<T,Allocator>::assign_range Example\n";
    const auto source = std::list{2, 7, 1};
    auto destination = Vector{3, 1, 4};
    destination.assign_range(source);
    assert(std::ranges::equal(source, destination));
#endif

    // std::vector<T,Allocator>::insert_range
#ifdef __cpp_lib_containers_ranges
    std::cout << "\nstd::vector<T,Allocator>::insert_range Example\n";
    auto container2 = Vector{1, 2, 3, 4};
    auto pos = std::next(container.begin(), 2);
    assert(*pos == 3);
    const auto rg2 = std::list{-1, -2, -3};
    container.insert_range(pos, rg2);
    assert(std::ranges::equal(container, std::vector{1, 2, -1, -2, -3, 3, 4}));
#endif

    // std::vector<T,Allocator>::append_range
#ifdef __cpp_lib_containers_ranges
    std::cout << "\nstd::vector<T,Allocator>::append_range Example\n";
    auto head = Vector{1, 2, 3, 4};
    const auto tail = std::list{-5, -6, -7};
    head.append_range(tail);
    assert(std::ranges::equal(head, std::vector{1, 2, 3, 4, -5, -6, -7}));
#endif
    
    // operator==,!=,<,<=,>,>=,<=>(std::vector)
    std::cout << "\noperator==,!=,<,<=,>,>=,<=>(std::vector) Example\n";

    const Vector<int> a_cmp{1, 2, 3}, b_cmp{1, 2, 3}, c_cmp{7, 8, 9, 10};
    assert((a_cmp == b_cmp) && (a_cmp != c_cmp) && (a_cmp < c_cmp));

    // std::swap(std::vector)
    std::cout << "\nstd::swap(std::vector) Example\n";

    Vector<int> alice{1, 2, 3};
    Vector<int> bob{7, 8, 9, 10};

    auto print_swap_std = [](const auto& container) {
        for (const int& n : container)
            std::cout << ' ' << n;
    };

    std::cout << "Alice:";
    print_swap_std(alice);
    std::cout << "\nBob  :";
    print_swap_std(bob);
    std::cout << '\n';

    std::cout << "-- SWAP\n";
    std::swap(alice, bob);

    std::cout << "Alice:";
    print_swap_std(alice);
    std::cout << "\nBob  :";
    print_swap_std(bob);
    std::cout << '\n';

    // std::erase, std::erase_if (std::vector)
    std::cout << "\nstd::erase, std::erase_if (std::vector) Example\n";

    auto println = [](std::string_view comment, const auto& c) {
        std::cout << comment << "{ ";
        for (const auto& x : c)
            std::cout << x << ' ';
        std::cout << "}\n";
    };

    Vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    println("Initially, cnt = ", cnt);

    erase(cnt, '3');
    println("After erase '3', cnt = ", cnt);

    auto erased = erase_if(cnt, [](char x) { return (x - '0') % 2 == 0; });
    println("After erase all even numbers, cnt = ", cnt);
    std::cout << "Erased even numbers: " << erased << '\n';

    Vector<std::complex<double>> nums_erase{{2, 2}, {4, 2}, {4, 8}, {4, 2}};
    erase(nums_erase, std::complex<double>{4, 2});
    println("After erase {4, 2}, nums = ", nums_erase);

    return 0;
}
