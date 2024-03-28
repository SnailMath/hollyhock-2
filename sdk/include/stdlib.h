#ifdef __cplusplus
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#include_next <stdlib.h>
#undef _GLIBCXX_INCLUDE_NEXT_C_HEADERS

#undef setenv
#undef itoa

extern "C++"
{
namespace std
{
  using ::setenv;
  using ::itoa;
} // namespace std

using std::setenv;
using std::itoa;
} // extern C++

#endif // __cplusplus
#include_next <stdlib.h>
