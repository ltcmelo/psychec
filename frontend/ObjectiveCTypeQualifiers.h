#ifndef CFE_OBJC_TYPEQUALIFIERS_H
#define CFE_OBJC_TYPEQUALIFIERS_H

#include "FrontendConfig.h"


namespace psyche {

enum {
  Token_in,
  Token_out,
  Token_copy,
  Token_byref,
  Token_inout,
  Token_assign,
  Token_bycopy,
  Token_getter,
  Token_retain,
  Token_setter,
  Token_oneway,
  Token_readonly,
  Token_nonatomic,
  Token_readwrite,
  Token_identifier
};

CFE_API int classifyObjectiveCContextKeyword(const char *s, int n);

} // namespace psyche


#endif // CFE_OBJC_TYPEQUALIFIERS_H
