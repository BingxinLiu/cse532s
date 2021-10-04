#include <atomic>
#define ZERO 0

using namespace std;

int
main (int, char * [])
{
    std::atomic<bool> atomic_bool_(true);
    std::atomic<char> atomic_char_('a');
    std::atomic<signed char> atomic_schar_('a');
    std::atomic<unsigned char> atomic_uchar_('a');
    std::atomic<int> atomic_int_(1);
    std::atomic<unsigned> atomic_uint_(1);
    std::atomic<short> atomic_short_(1);
    std::atomic<unsigned short> atomic_ushort_(1);
    std::atomic<long> atomic_long_(1);
    std::atomic<unsigned long> atomic_ulong_(1);
    std::atomic<long long> atomic_llong_(1);
    std::atomic<unsigned long long> atomic_ullong_(1);
    std::atomic<char16_t> atomic_char16_t_('a');
    std::atomic<char32_t> atomic_char32_t_('a');
    std::atomic<wchar_t> atomic_wchar_t('a');
    atomic_int_least8_t int_least8_t_(1);
    atomic_uint_least8_t uint_least8_t_(1);
    atomic_int_least16_t int_least16_t_(1);
    atomic_uint_least16_t uint_least16_t_(1);
    atomic_int_least32_t int_least32_t_(1);
    atomic_uint_least32_t uint_least32_t_(1);
    atomic_int_least64_t int_least64_t_(1);
    atomic_uint_least64_t uint_least64_t_(1);
    atomic_int_fast8_t int_fast8_t_(1);
    atomic_uint_fast8_t uint_fast8_t_(1);
    atomic_int_fast16_t int_fast16_t_(1);
    atomic_uint_fast16_t uint_fast16_t_(1);
    atomic_int_fast32_t int_fast32_t_(1);
    atomic_uint_fast32_t uint_fast32_t_(1);
    atomic_int_fast64_t int_fast64_t_(1);
    atomic_uint_fast64_t uint_fast64_t_(1);
    atomic_intptr_t intptr_t_(1);
    atomic_uintptr_t uintptr_t_(1);
    atomic_size_t size_t_(1);
    atomic_ptrdiff_t ptrdiff_t_(1);
    atomic_intmax_t intmax_t_(1);
    atomic_uintmax_t uintmax_t_(1);
    return ZERO;
}