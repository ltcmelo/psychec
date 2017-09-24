
#ifndef QTCONTEXTKEYWORDS_H
#define QTCONTEXTKEYWORDS_H

#include "FrontendConfig.h"

namespace psyche {

enum {
    Token_not_Qt_context_keyword = 0,
    Token_READ,
    Token_USER,
    Token_FINAL,
    Token_RESET,
    Token_WRITE,
    Token_NOTIFY,
    Token_STORED,
    Token_CONSTANT,
    Token_DESIGNABLE,
    Token_SCRIPTABLE,
    Token_REVISION,
    Token_MEMBER
};

CFE_API int classifyQtContextKeyword(const char *s, int n);
} // namespace psyche;

#endif // QTCONTEXTKEYWORDS_H
