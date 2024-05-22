#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <type_traits> // std::enable_if

#if __cplusplus >= 202002L
#include <compare> // std::strong_ordering ir <=> operatoriui (C++20)
#endif

#ifdef __cpp_lib_containers_ranges
#include <ranges>
#endif

/// Dinaminis masyvo konteineris, panašus į std::vector.
template <typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    using value_type = T; ///< Elementų tipas, saugomas vektoriuje.
    using allocator_type = Allocator; ///< Paskirstytojo tipas, naudojamas atminties valdymui.
    using size_type = std::size_t; ///< Neigiamas sveikasis tipas, naudojamas dydžiams.
    using difference_type = std::ptrdiff_t; ///< Neigiamas sveikasis tipas, naudojamas skirtumams.
    using reference = value_type&; ///< Nuoroda į elementą.
    using const_reference = const value_type&; ///< Konstantinė nuoroda į elementą.
    using pointer = typename std::allocator_traits<Allocator>::pointer; ///< Rodyklė į elementą.
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer; ///< Konstantinė rodyklė į elementą.
    using iterator = T*; ///< Iteratoriaus tipas nekonstantiniams elementams.
    using const_iterator = const T*; ///< Iteratoriaus tipas konstantiniams elementams.
    using reverse_iterator = std::reverse_iterator<iterator>; ///< Atvirkštinio iteratoriaus tipas nekonstantiniams elementams.
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Atvirkštinio iteratoriaus tipas konstantiniams elementams.

    Vector(); ///< Numatytasis konstruktorius, kuris inicializuoja tuščią vektorių.
    explicit Vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator()); ///< Konstruoja vektorių su nurodytu dydžiu ir reikšme, pasirinktinai naudojant pritaikytą paskirstytoją.
    template<typename InputIt, typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()); ///< Konstruoja vektorių iš intervalo.
    Vector(const Vector& other); ///< Kopijavimo konstruktorius, kuris sukuria naują vektorių kaip kito vektoriaus kopiją.
    Vector(Vector&& other) noexcept; ///< Perkėlimo konstruktorius, kuris perduoda turinį iš kito vektoriaus šiam.
    Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()); ///< Konstruktorius su inicializacijos sąrašu.
    ~Vector(); ///< Destruktorius, kuris atlaisvina vektoriaus elementus ir atmintį.

    Vector& operator=(const Vector& other); ///< Kopijavimo priskyrimo operatorius, kuris kopijuoja turinį iš kito vektoriaus.
    Vector& operator=(Vector&& other) noexcept; ///< Perkėlimo priskyrimo operatorius, kuris perduoda turinį iš kito vektoriaus.
    Vector& operator=(std::initializer_list<T> ilist); ///< Priskyrimo operatorius su inicializacijos sąrašu.

    void assign(size_type count, const T& value); ///< Priskiria naują reikšmę vektoriui.
    template <typename InputIt, typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    void assign(InputIt first, InputIt last); ///< Priskiria naujas reikšmes iš intervalo.
    void assign(std::initializer_list<T> ilist); ///< Priskiria naujas reikšmes iš inicializacijos sąrašo.

    allocator_type get_allocator() const noexcept; ///< Gražina susijusį paskirstytoją.

    reference at(size_type pos); ///< Prieigos metodas prie nurodyto elemento su ribų tikrinimu, meta std::out_of_range, jei pos >= size().
    const_reference at(size_type pos) const; ///< Prieigos metodas prie nurodyto elemento su ribų tikrinimu, meta std::out_of_range, jei pos >= size() (const versija).
    reference operator[](size_type pos); ///< Prieigos metodas prie nurodyto elemento be ribų tikrinimo.
    const_reference operator[](size_type pos) const; ///< Prieigos metodas prie nurodyto elemento be ribų tikrinimo (const versija).
    reference front(); ///< Prieigos metodas prie pirmo elemento, sąlyga: vektorius neturi būti tuščias.
    const_reference front() const; ///< Prieigos metodas prie pirmo elemento, sąlyga: vektorius neturi būti tuščias (const versija).
    reference back(); ///< Prieigos metodas prie paskutinio elemento, sąlyga: vektorius neturi būti tuščias.
    const_reference back() const; ///< Prieigos metodas prie paskutinio elemento, sąlyga: vektorius neturi būti tuščias (const versija).
    T* data() noexcept; ///< Tiesioginė prieiga prie pagrindinio masyvo.
    const T* data() const noexcept; ///< Tiesioginė prieiga prie pagrindinio masyvo (const versija).

    iterator begin() noexcept; ///< Gražina iteratorių į pradžią.
    const_iterator begin() const noexcept; ///< Gražina iteratorių į pradžią (const versija).
    const_iterator cbegin() const noexcept; ///< Gražina konstantinį iteratorių į pradžią.
    iterator end() noexcept; ///< Gražina iteratorių į pabaigą.
    const_iterator end() const noexcept; ///< Gražina iteratorių į pabaigą (const versija).
    const_iterator cend() const noexcept; ///< Gražina konstantinį iteratorių į pabaigą.
    reverse_iterator rbegin() noexcept; ///< Gražina atvirkštinį iteratorių į pradžią.
    const_reverse_iterator rbegin() const noexcept; ///< Gražina atvirkštinį iteratorių į pradžią (const versija).
    const_reverse_iterator crbegin() const noexcept; ///< Gražina konstantinį atvirkštinį iteratorių į pradžią.
    reverse_iterator rend() noexcept; ///< Gražina atvirkštinį iteratorių į pabaigą.
    const_reverse_iterator rend() const noexcept; ///< Gražina atvirkštinį iteratorių į pabaigą (const versija).
    const_reverse_iterator crend() const noexcept; ///< Gražina konstantinį atvirkštinį iteratorių į pabaigą.

    bool empty() const noexcept; ///< Tikrina, ar konteineris yra tuščias.
    size_type size() const noexcept; ///< Gražina elementų skaičių.
    size_type max_size() const noexcept; ///< Gražina didžiausią galimą elementų skaičių.
    void reserve(size_type new_cap); ///< Rezervuoja atmintį.
    size_type capacity() const noexcept; ///< Gražina elementų skaičių, kurį gali laikyti šiuo metu paskirstyta atmintis.
    void shrink_to_fit(); ///< Sumažina atminties naudojimą, atlaisvindamas nenaudojamą atmintį.

    void clear() noexcept; ///< Išvalo turinį.
    iterator insert(const_iterator pos, const T& value); ///< Įterpia elementą į nurodytą poziciją.
    iterator insert(const_iterator pos, T&& value); ///< Įterpia elementą į nurodytą poziciją, naudojant perkėlimo semantiką.
    iterator insert(const_iterator pos, size_type count, const T& value); ///< Įterpia kelis elementus į nurodytą poziciją.
    iterator insert(const_iterator pos, std::initializer_list<T> ilist); ///< Įterpia elementus iš initializer list į nurodytą poziciją.
    template <typename InputIt, typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    iterator insert(const_iterator pos, InputIt first, InputIt last); ///< Įterpia elementus iš intervalo [first, last) į nurodytą poziciją.
    iterator erase(const_iterator pos); ///< Ištrina elementą iš nurodytos pozicijos.
    iterator erase(const_iterator first, const_iterator last); ///< Ištrina elementus intervale [first, last).
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args); ///< Sukuria elementą vietoje nurodytoje pozicijoje.
    template <typename... Args>
    reference emplace_back(Args&&... args); ///< Sukuria elementą vietoje pabaigoje ir gražina nuorodą į jį.
    void push_back(const T& value); ///< Prideda elementą į pabaigą.
    void push_back(T&& value); ///< Prideda elementą į pabaigą, naudojant perkėlimo semantiką.
    void pop_back(); ///< Ištrina paskutinį elementą.
    void resize(size_type count, T value = T()); ///< Keičia konteinerio dydį.
    void swap(Vector& other) noexcept; ///< Sujungia turinį su kitu vektoriumi.
#ifdef __cpp_lib_containers_ranges
    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    constexpr void assign_range(R&& rg); ///< Priskiria vektoriui naujas reikšmes iš intervalo

    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    constexpr iterator insert_range(const_iterator pos, R&& rg); ///< Įterpia elementus iš intervalo į nurodytą poziciją

    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    constexpr void append_range(R&& rg); ///< Prideda elementus iš intervalo į vektoriaus pabaigą
#endif

private:
    T* mData; ///< Rodyklė į masyvą, kuriame saugomi elementai.
    size_type mSize; ///< Dabartinis elementų skaičius vektoriuje.
    size_type mCapacity; ///< Dabartinė paskirstyta vektoriaus talpa.
    Allocator mAllocator; ///< Paskirstytojo instancija, naudojama atminties valdymui.

    void reallocate(size_type new_capacity); ///< Paskirsto naują atmintį su didesne talpa.
    void destroy_elements(); ///< Sunaikina visus vektoriaus elementus ir atlaisvina atmintį.
    void move_from(Vector&& other) noexcept; ///< Perkelia duomenis iš kito vektoriaus į šį.
};

// Išorės funkcijos

/// Lyginimo operatoriai
template <typename T, typename Allocator>
bool operator==(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

template <typename T, typename Allocator>
bool operator!=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

template <typename T, typename Allocator>
bool operator<(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

template <typename T, typename Allocator>
bool operator<=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

template <typename T, typename Allocator>
bool operator>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

template <typename T, typename Allocator>
bool operator>=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);

#if __cplusplus >= 202002L
/// Trijų krypčių lyginimo operatorius (C++20)
template <typename T, typename Allocator>
std::strong_ordering operator<=>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs);
#endif

/// std::swap specializacija
template <typename T, typename Allocator>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept;

/// Ištrinimo funkcijos (C++20)
template <typename T, typename Allocator>
typename Vector<T, Allocator>::size_type erase(Vector<T, Allocator>& c, const T& value);

template <typename T, typename Allocator, typename Pred>
typename Vector<T, Allocator>::size_type erase_if(Vector<T, Allocator>& c, Pred pred);

#include "Vector.tpp"

#endif // MY_VECTOR_H
