// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_FILE_INFO_H__
#define PSYCHE_FILE_INFO_H__

#include <string>

namespace psy {

/*!
 * \brief The FileInfo class
 *
 * \note
 * From https://github.com/ltcmelo/uaiso
 */
class FileInfo final
{
public:
    ~FileInfo();

    /*!
     * \brief FileInfo
     * \param fullFileName
     *
     * Within the docs, suppose an instance of this class is created as:
     *
     * \code
     * FileInfo fileInfo("/home/user/project/code.cpp");
     * \endcode
     */
    FileInfo(const std::string& fullFileName);

    /*!
     * \brief fileName
     * \return
     *
     * According to constructor's assumption, return is:
     * "code.cpp"
     */
    std::string fileName() const;

    /*!
     * \brief fileBaseName
     * \return
     *
     * According to constructor's assumption, return is:
     * "code"
     */
    std::string fileBaseName() const;

    /*!
     * \brief fullFileBaseName
     * \return
     *
     * According to constructor's assumption, return is:
     * "/home/user/project/code"
     */
    std::string fullFileBaseName() const;

    /*!
     * \brief fullFileName
     * \return
     *
     * According to constructor's assumption, return is:
     * "/home/user/project/code.cpp"
     */
    std::string fullFileName() const;

    /*!
     * \brief dir
     * \return
     *
     * According to constructor's assumption, return is:
     * "project"
     */
    std::string dir() const;

    /*!
     * \brief fullDir
     * \return
     *
     * According to constructor's assumption, return is:
     * "/home/user/project"
     */
    std::string fullDir() const;

    /*!
     * \brief dirSeparator
     * \return
     */
    static char dirSeparator();

private:
    struct FileInfoImpl;
    FileInfoImpl* impl_;
};

bool operator==(const FileInfo& a, const FileInfo& b);
bool operator!=(const FileInfo& a, const FileInfo& b);


} // psy

#endif
