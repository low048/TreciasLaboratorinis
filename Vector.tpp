#include "Vector.h"
#include <algorithm>
#include <iterator>
#include <utility>
#include <type_traits> // dėl std::enable_if ir std::is_convertible

/// Pagalbinė tipų savybė, skirta aptikti, ar tipas yra iteratorius
template<typename T>
struct is_iterator {
    using iterator_category = typename std::iterator_traits<T>::iterator_category;
    static constexpr bool value = !std::is_same<iterator_category, void>::value;
};

/// Numatytasis konstruktorius, kuris inicializuoja tuščią vektorių.
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector() : mData(nullptr), mSize(0), mCapacity(0), mAllocator(Allocator()) {}

/// Konstruktorius su dydžiu ir reikšme
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_type count, const T& value, const Allocator& alloc)
    : mData(nullptr), mSize(count), mCapacity(count), mAllocator(alloc) {
    mData = mAllocator.allocate(mCapacity);
    std::uninitialized_fill_n(mData, mSize, value);
}

/// Konstruktorius su intervalu
template <typename T, typename Allocator>
template <typename InputIt, typename>
Vector<T, Allocator>::Vector(InputIt first, InputIt last, const Allocator& alloc)
    : mData(nullptr), mSize(std::distance(first, last)), mCapacity(mSize), mAllocator(alloc) {
    mData = mAllocator.allocate(mCapacity);
    std::uninitialized_copy(first, last, mData);
}

/// Kopijavimo konstruktorius
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& other)
    : mData(nullptr), mSize(other.mSize), mCapacity(other.mCapacity), mAllocator(other.mAllocator) {
    mData = mAllocator.allocate(mCapacity);
    std::uninitialized_copy(other.mData, other.mData + mSize, mData);
}

/// Perkėlimo konstruktorius
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept : Vector() {
    move_from(std::move(other));
}

/// Inicializacijos sąrašo konstruktorius
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> init, const Allocator& alloc)
    : mData(nullptr), mSize(init.size()), mCapacity(init.size()), mAllocator(alloc) {
    mData = mAllocator.allocate(mCapacity);
    std::uninitialized_copy(init.begin(), init.end(), mData);
}

/// Destruktorius
template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector() {
    destroy_elements();
    if (mData) {
        mAllocator.deallocate(mData, mCapacity);
    }
    mData = nullptr;
    mCapacity = 0;
}

/// Kopijavimo priskyrimo operatorius
template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other) {
    if (this != &other) {
        destroy_elements();
        mAllocator.deallocate(mData, mCapacity);
        mSize = other.mSize;
        mCapacity = other.mCapacity;
        mAllocator = other.mAllocator;
        mData = mAllocator.allocate(mCapacity);
        std::uninitialized_copy(other.mData, other.mData + mSize, mData);
    }
    return *this;
}

/// Perkėlimo priskyrimo operatorius
template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        destroy_elements();
        move_from(std::move(other));
    }
    return *this;
}

/// Inicializacijos sąrašo priskyrimo operatorius
template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
    return *this;
}

/// Priskyrimas su dydžiu ir reikšme
template <typename T, typename Allocator>
void Vector<T, Allocator>::assign(size_type count, const T& value) {
    if (count > mCapacity) {
        reallocate(count);
    }
    for (size_type i = 0; i < count; ++i) {
        if (i < mSize) {
            mData[i] = value;
        } else {
#if __cplusplus >= 201103L
            std::allocator_traits<Allocator>::construct(mAllocator, mData + i, value);
#else
            mAllocator.construct(mData + i, value);
#endif
        }
    }
    for (size_type i = count; i < mSize; ++i) {
#if __cplusplus >= 201103L
        std::allocator_traits<Allocator>::destroy(mAllocator, mData + i);
#else
        mAllocator.destroy(mData + i);
#endif
    }
    mSize = count;
}

/// Priskyrimas su intervalu
template <typename T, typename Allocator>
template <typename InputIt, typename>
void Vector<T, Allocator>::assign(InputIt first, InputIt last) {
    size_type count = std::distance(first, last);
    if (count > mCapacity) {
        reallocate(count);
    }
    if (count > mSize) {
        std::copy(first, first + mSize, mData);
        std::uninitialized_copy(first + mSize, last, mData + mSize);
    } else {
        std::copy(first, last, mData);
        for (size_type i = count; i < mSize; ++i) {
#if __cplusplus >= 201103L
            std::allocator_traits<Allocator>::destroy(mAllocator, mData + i);
#else
            mAllocator.destroy(mData + i);
#endif
        }
    }
    mSize = count;
}

/// Priskyrimas su inicializacijos sąrašu
template <typename T, typename Allocator>
void Vector<T, Allocator>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

/// Grąžina susijusį paskirstytoją
template <typename T, typename Allocator>
typename Vector<T, Allocator>::allocator_type Vector<T, Allocator>::get_allocator() const noexcept {
    return mAllocator;
}

/// Prieigos metodas prie nurodyto elemento su ribų tikrinimu, meta std::out_of_range, jei pos >= size().
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::at(size_type pos) {
    if (pos >= mSize) throw std::out_of_range("Indeksas už ribų");
    return mData[pos];
}

/// Prieigos metodas prie nurodyto elemento su ribų tikrinimu, meta std::out_of_range, jei pos >= size() (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::at(size_type pos) const {
    if (pos >= mSize) throw std::out_of_range("Indeksas už ribų");
    return mData[pos];
}

/// Prieigos metodas prie nurodyto elemento be ribų tikrinimo.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator[](size_type pos) {
    return mData[pos];
}

/// Prieigos metodas prie nurodyto elemento be ribų tikrinimo (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::operator[](size_type pos) const {
    return mData[pos];
}

/// Prieigos metodas prie pirmo elemento, sąlyga: vektorius neturi būti tuščias.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::front() {
    return mData[0];
}

/// Prieigos metodas prie pirmo elemento, sąlyga: vektorius neturi būti tuščias (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::front() const {
    return mData[0];
}

/// Prieigos metodas prie paskutinio elemento, sąlyga: vektorius neturi būti tuščias.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::back() {
    return mData[mSize - 1];
}

/// Prieigos metodas prie paskutinio elemento, sąlyga: vektorius neturi būti tuščias (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::back() const {
    return mData[mSize - 1];
}

/// Tiesioginė prieiga prie pagrindinio masyvo.
template <typename T, typename Allocator>
T* Vector<T, Allocator>::data() noexcept {
    return mData;
}

/// Tiesioginė prieiga prie pagrindinio masyvo (const versija).
template <typename T, typename Allocator>
const T* Vector<T, Allocator>::data() const noexcept {
    return mData;
}

/// Gražina iteratorių į pradžią.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::begin() noexcept {
    return mData;
}

/// Gražina iteratorių į pradžią (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::begin() const noexcept {
    return mData;
}

/// Gražina konstantinį iteratorių į pradžią.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cbegin() const noexcept {
    return mData;
}

/// Gražina iteratorių į pabaigą.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::end() noexcept {
    return mData + mSize;
}

/// Gražina iteratorių į pabaigą (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::end() const noexcept {
    return mData + mSize;
}

/// Gražina konstantinį iteratorių į pabaigą.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cend() const noexcept {
    return mData + mSize;
}

/// Gražina atvirkštinį iteratorių į pradžią.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(end());
}

/// Gražina atvirkštinį iteratorių į pradžią (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

/// Gražina konstantinį atvirkštinį iteratorių į pradžią.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

/// Gražina atvirkštinį iteratorių į pabaigą.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rend() noexcept {
    return reverse_iterator(begin());
}

/// Gražina atvirkštinį iteratorių į pabaigą (const versija).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

/// Gražina konstantinį atvirkštinį iteratorių į pabaigą.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

/// Tikrina, ar konteineris yra tuščias.
template <typename T, typename Allocator>
bool Vector<T, Allocator>::empty() const noexcept {
    return mSize == 0;
}

/// Gražina elementų skaičių.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::size() const noexcept {
    return mSize;
}

/// Gražina didžiausią galimą elementų skaičių.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::max_size() const noexcept {
    return std::allocator_traits<Allocator>::max_size(mAllocator);
}

/// Rezervuoja atmintį bent jau naujiems new_cap elementams.
template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_type new_cap) {
    if (new_cap > mCapacity) {
        reallocate(new_cap);
    }
}

/// Gražina elementų skaičių, kurį gali laikyti šiuo metu paskirstyta atmintis.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::capacity() const noexcept {
    return mCapacity;
}

/// Sumažina atminties naudojimą, atlaisvindamas nenaudojamą atmintį.
template <typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit() {
    if (mCapacity > mSize) {
        reallocate(mSize);
    }
}

/// Išvalo turinį.
template <typename T, typename Allocator>
void Vector<T, Allocator>::clear() noexcept {
    destroy_elements();
}

/// Įterpia elementą į nurodytą poziciją.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, const T& value) {
    difference_type offset = pos - begin();
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + 1);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, it, value);
#else
    mAllocator.construct(it, value);
#endif
    ++mSize;
    return it;
}

/// Įterpia elementą į nurodytą poziciją, naudojant perkėlimo semantiką.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, T&& value) {
    difference_type offset = pos - begin();
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + 1);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, it, std::move(value));
#else
    mAllocator.construct(it, std::move(value));
#endif
    ++mSize;
    return it;
}

/// Įterpia kelis elementus į nurodytą poziciją.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, size_type count, const T& value) {
    difference_type offset = pos - begin();
    if (mSize + count > mCapacity) {
        reallocate(mSize + count > mCapacity * 2 ? mSize + count : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + count);
    }
    std::uninitialized_fill_n(it, count, value);
    mSize += count;
    return it;
}

/// Įterpia elementus iš intervalo [first, last) į nurodytą poziciją.
template <typename T, typename Allocator>
template <typename InputIt, typename>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last) {
    difference_type offset = pos - begin();
    size_type count = std::distance(first, last);
    if (mSize + count > mCapacity) {
        reallocate(mSize + count > mCapacity * 2 ? mSize + count : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + count);
    }
    std::uninitialized_copy(first, last, it);
    mSize += count;
    return it;
}

/// Įterpia elementus iš initializer list į nurodytą poziciją.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    difference_type offset = pos - begin();
    size_type count = ilist.size();
    if (mSize + count > mCapacity) {
        reallocate(mSize + count > mCapacity * 2 ? mSize + count : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + count);
    }
    std::uninitialized_copy(ilist.begin(), ilist.end(), it);
    mSize += count;
    return it;
}

/// Sukuria elementą vietoje nurodytoje pozicijoje.
template <typename T, typename Allocator>
template <typename... Args>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::emplace(const_iterator pos, Args&&... args) {
    difference_type offset = pos - begin();
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
    iterator it = begin() + offset;
    if (it != end()) {
        std::move_backward(it, end(), end() + 1);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, it, std::forward<Args>(args)...);
#else
    mAllocator.construct(it, std::forward<Args>(args)...);
#endif
    ++mSize;
    return it;
}


/// Sukuria elementą vietoje pabaigoje ir gražina nuorodą į jį.
template <typename T, typename Allocator>
template <typename... Args>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::emplace_back(Args&&... args) {
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, mData + mSize, std::forward<Args>(args)...);
#else
    mAllocator.construct(mData + mSize, std::forward<Args>(args)...);
#endif
    ++mSize;
    return mData[mSize - 1];
}

/// Ištrina elementą iš nurodytos pozicijos.
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator pos) {
    return erase(pos, pos + 1);
}

/// Ištrina elementus intervale [first, last).
template <typename T, typename Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
    iterator it_first = const_cast<iterator>(first);
    iterator it_last = const_cast<iterator>(last);
    iterator it_new_end = std::move(it_last, end(), it_first);
    for (auto it = it_new_end; it != end(); ++it) {
#if __cplusplus >= 201103L
        std::allocator_traits<Allocator>::destroy(mAllocator, it);
#else
        mAllocator.destroy(it);
#endif
    }
    mSize -= std::distance(first, last);
    return it_first;
}

/// Prideda elementą į pabaigą.
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& value) {
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, mData + mSize, value);
#else
    mAllocator.construct(mData + mSize, value);
#endif
    ++mSize;
}

/// Prideda elementą į pabaigą, naudojant perkėlimo semantiką.
template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(T&& value) {
    if (mSize == mCapacity) {
        reallocate(mCapacity == 0 ? 1 : mCapacity * 2);
    }
#if __cplusplus >= 201103L
    std::allocator_traits<Allocator>::construct(mAllocator, mData + mSize, std::move(value));
#else
    mAllocator.construct(mData + mSize, std::move(value));
#endif
    ++mSize;
}

/// Ištrina paskutinį elementą.
template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back() {
    if (mSize > 0) {
#if __cplusplus >= 201103L
        std::allocator_traits<Allocator>::destroy(mAllocator, mData + --mSize);
#else
        mAllocator.destroy(mData + --mSize);
#endif
    }
}

/// Keičia konteinerio dydį, kad jame būtų count elementų.
template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_type count, T value) {
    if (count > mSize) {
        reserve(count);
        std::uninitialized_fill(mData + mSize, mData + count, value);
    } else {
        for (size_type i = count; i < mSize; ++i) {
#if __cplusplus >= 201103L
            std::allocator_traits<Allocator>::destroy(mAllocator, mData + i);
#else
            mAllocator.destroy(mData + i);
#endif
        }
    }
    mSize = count;
}

/// Sujungia turinį su kitu vektoriumi.
template <typename T, typename Allocator>
void Vector<T, Allocator>::swap(Vector& other) noexcept {
    std::swap(mData, other.mData);
    std::swap(mSize, other.mSize);
    std::swap(mCapacity, other.mCapacity);
    std::swap(mAllocator, other.mAllocator);
}

#ifdef __cpp_lib_containers_ranges
/// Priskiria vektoriui naujas reikšmes iš intervalo
template <typename T, typename Allocator>
template <std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_reference_t<R>, T>
constexpr void Vector<T, Allocator>::assign_range(R&& rg) {
    auto first = std::ranges::begin(rg);
    auto last = std::ranges::end(rg);
    assign(first, last);
}

/// Įterpia elementus iš intervalo į nurodytą poziciją
template <typename T, typename Allocator>
template <std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_reference_t<R>, T>
constexpr typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert_range(const_iterator pos, R&& rg) {
    auto first = std::ranges::begin(rg);
    auto last = std::ranges::end(rg);
    return insert(pos, first, last);
}

/// Prideda elementus iš intervalo į vektoriaus pabaigą
template <typename T, typename Allocator>
template <std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_reference_t<R>, T>
constexpr void Vector<T, Allocator>::append_range(R&& rg) {
    auto first = std::ranges::begin(rg);
    auto last = std::ranges::end(rg);
    insert(end(), first, last);
}
#endif

/// Paskirsto naują atmintį su didesne talpa.
template <typename T, typename Allocator>
void Vector<T, Allocator>::reallocate(size_type new_capacity) {
    if (new_capacity < mSize) {
        new_capacity = mSize;
    }
    pointer new_data = mAllocator.allocate(new_capacity);
    if (mData) {
        for (size_type i = 0; i < mSize; ++i) {
#if __cplusplus >= 201103L
            std::allocator_traits<Allocator>::construct(mAllocator, new_data + i, std::move(mData[i]));
            std::allocator_traits<Allocator>::destroy(mAllocator, mData + i);
#else
            mAllocator.construct(new_data + i, std::move(mData[i]));
            mAllocator.destroy(mData + i);
#endif
        }
        mAllocator.deallocate(mData, mCapacity);
    }

    mData = new_data;
    mCapacity = new_capacity;
}

/// Sunaikina visus vektoriaus elementus.
template <typename T, typename Allocator>
void Vector<T, Allocator>::destroy_elements() {
    if (mData) {
        for (size_type i = 0; i < mSize; ++i) {
#if __cplusplus >= 201103L
            std::allocator_traits<Allocator>::destroy(mAllocator, mData + i);
#else
            mAllocator.destroy(mData + i);
#endif
        }
    }
    mSize = 0;
    /*pagal cppreference pavyzdžius, neturėtų būti atlaisvinama atmintis
    mData = nullptr;
    mCapacity = 0;*/
}

/// Perkelia duomenis iš kito vektoriaus į šį.
template <typename T, typename Allocator>
void Vector<T, Allocator>::move_from(Vector&& other) noexcept {
    mData = other.mData;
    mSize = other.mSize;
    mCapacity = other.mCapacity;
    mAllocator = std::move(other.mAllocator);
    other.mSize = 0;
    other.mCapacity = 0;
    other.mData = nullptr;
}

/// Lyginimo operatoriai
template <typename T, typename Allocator>
bool operator==(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Allocator>
bool operator!=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename Allocator>
bool operator<(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Allocator>
bool operator<=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return !(rhs < lhs);
}

template <typename T, typename Allocator>
bool operator>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return rhs < lhs;
}

template <typename T, typename Allocator>
bool operator>=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return !(lhs < rhs);
}

#if __cplusplus >= 202002L
/// Trijų krypčių lyginimo operatorius (C++20)
template <typename T, typename Allocator>
std::strong_ordering operator<=>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
#endif

template <typename T, typename Allocator>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept {
    lhs.swap(rhs);
}

/// Ištrynimo funkcijos (C++20)
template <typename T, typename Allocator>
typename Vector<T, Allocator>::size_type erase(Vector<T, Allocator>& c, const T& value) {
    auto old_size = c.size();
    auto it = std::remove(c.begin(), c.end(), value);
    c.erase(it, c.end());
    return old_size - c.size();
}

template <typename T, typename Allocator, typename Pred>
typename Vector<T, Allocator>::size_type erase_if(Vector<T, Allocator>& c, Pred pred) {
    auto old_size = c.size();
    auto it = std::remove_if(c.begin(), c.end(), pred);
    c.erase(it, c.end());
    return old_size - c.size();
}