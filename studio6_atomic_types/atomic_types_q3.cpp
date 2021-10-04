#include <atomic>
#include <typeinfo>
#include <iostream>
#define ZERO 0

using namespace std;

template<class T>
void is_lock_free(T type)
{
    std::cout << typeid(type).name() << " is " << (type.is_lock_free() ? "" : "not clock free") << std::endl;

}

int
main (int, char * [])
{
    std::atomic<bool> atomic_bool_(true);
    std::cout << typeid(atomic_bool_).name() << " is " << (atomic_bool_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<char> atomic_char_('a');
    std::cout << typeid(atomic_char_).name() << " is " << (atomic_char_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<signed char> atomic_schar_('a');
    std::cout << typeid(atomic_schar_).name() << " is " << (atomic_schar_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<unsigned char> atomic_uchar_('a');
    std::cout << typeid(atomic_uchar_).name() << " is " << (atomic_uchar_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<int> atomic_int_(1);
    std::cout << typeid(atomic_int_).name() << " is " << (atomic_int_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<unsigned> atomic_uint_(1);
    std::cout << typeid(atomic_uint_).name() << " is " << (atomic_uint_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<short> atomic_short_(1);
    std::cout << typeid(atomic_short_).name() << " is " << (atomic_short_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<unsigned short> atomic_ushort_(1);
    std::cout << typeid(atomic_ushort_).name() << " is " << (atomic_ushort_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<long> atomic_long_(1);
    std::cout << typeid(atomic_long_).name() << " is " << (atomic_long_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<unsigned long> atomic_ulong_(1);
    std::cout << typeid(atomic_ulong_).name() << " is " << (atomic_ulong_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<long long> atomic_llong_(1);
    std::cout << typeid(atomic_llong_).name() << " is " << (atomic_llong_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<unsigned long long> atomic_ullong_(1);
    std::cout << typeid(atomic_ullong_).name() << " is " << (atomic_ullong_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<char16_t> atomic_char16_t_('a');
    std::cout << typeid(atomic_char16_t_).name() << " is " << (atomic_char16_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<char32_t> atomic_char32_t_('a');
    std::cout << typeid(atomic_char32_t_).name() << " is " << (atomic_char32_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    std::atomic<wchar_t> atomic_wchar_t('a');
    std::cout << typeid(atomic_wchar_t).name() << " is " << (atomic_wchar_t.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_least8_t int_least8_t_(1);
    std::cout << typeid(int_least8_t_).name() << " is " << (int_least8_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_least8_t uint_least8_t_(1);
    std::cout << typeid(uint_least8_t_).name() << " is " << (uint_least8_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_least16_t int_least16_t_(1);
    std::cout << typeid(int_least16_t_).name() << " is " << (int_least16_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_least16_t uint_least16_t_(1);
    std::cout << typeid(uint_least16_t_).name() << " is " << (uint_least16_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_least32_t int_least32_t_(1);
    std::cout << typeid(int_least32_t_).name() << " is " << (int_least32_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_least32_t uint_least32_t_(1);
    std::cout << typeid(uint_least32_t_).name() << " is " << (uint_least32_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_least64_t int_least64_t_(1);
    std::cout << typeid(int_least64_t_).name() << " is " << (int_least64_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_least64_t uint_least64_t_(1);
    std::cout << typeid(uint_least64_t_).name() << " is " << (uint_least64_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_fast8_t int_fast8_t_(1);
    std::cout << typeid(int_fast8_t_).name() << " is " << (int_fast8_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_fast8_t uint_fast8_t_(1);
    std::cout << typeid(uint_fast8_t_).name() << " is " << (uint_fast8_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_fast16_t int_fast16_t_(1);
    std::cout << typeid(int_fast16_t_).name() << " is " << (int_fast16_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_fast16_t uint_fast16_t_(1);
    std::cout << typeid(uint_fast16_t_).name() << " is " << (uint_fast16_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_fast32_t int_fast32_t_(1);
    std::cout << typeid(int_fast32_t_).name() << " is " << (int_fast32_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_fast32_t uint_fast32_t_(1);
    std::cout << typeid(uint_fast32_t_).name() << " is " << (uint_fast32_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_int_fast64_t int_fast64_t_(1);
    std::cout << typeid(int_fast64_t_).name() << " is " << (int_fast64_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uint_fast64_t uint_fast64_t_(1);
    std::cout << typeid(uint_fast64_t_).name() << " is " << (uint_fast64_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_intptr_t intptr_t_(1);
    std::cout << typeid(intptr_t_).name() << " is " << (intptr_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uintptr_t uintptr_t_(1);
    std::cout << typeid(uintptr_t_).name() << " is " << (uintptr_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_size_t size_t_(1);
    std::cout << typeid(size_t_).name() << " is " << (size_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_ptrdiff_t ptrdiff_t_(1);
    std::cout << typeid(ptrdiff_t_).name() << " is " << (ptrdiff_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_intmax_t intmax_t_(1);
    std::cout << typeid(intmax_t_).name() << " is " << (intmax_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    atomic_uintmax_t uintmax_t_(1);
    std::cout << typeid(uintmax_t_).name() << " is " << (uintmax_t_.is_lock_free() ? "" : "not clock free") << std::endl;
    return ZERO;
}