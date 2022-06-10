// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "SyntaxNodeList.h"

#include "SyntaxNode.h"
#include "SyntaxNodes.h"
#include "SyntaxTree.h"

using namespace psy;
using namespace C;

SyntaxToken SyntaxNodeList::token(LexedTokens::IndexType tkIdx, SyntaxTree* tree)
{
    return tree->tokenAt(tkIdx);
}

template <class SyntaxNodeT>
SyntaxToken
SyntaxNodeSeparatedList<SyntaxNodeT>::delimiterToken() const
{
    return SyntaxNodeList::token(delimTkIdx_, Base::tree_);
}

namespace psy {
namespace C {

template class SyntaxNodeSeparatedList<EnumeratorDeclarationSyntax*>;
template class SyntaxNodeSeparatedList<ParameterDeclarationSyntax*>;
template class SyntaxNodeSeparatedList<ExtGNU_AttributeSyntax*>;
template class SyntaxNodeSeparatedList<DeclaratorSyntax*>;
template class SyntaxNodeSeparatedList<InitializerSyntax*>;
template class SyntaxNodeSeparatedList<ExpressionSyntax*>;
template class SyntaxNodeSeparatedList<GenericAssociationSyntax*>;
template class SyntaxNodeSeparatedList<ExtGNU_AsmOperandSyntax*>;

} // C
} // psy
