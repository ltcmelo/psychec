/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef PSYCHE_FILE_INFO_H__
#define PSYCHE_FILE_INFO_H__

#include <string>

namespace psyche {

/*!
 * \brief The FileInfo class
 *
 * Adapted from another project of mine: https://github.com/ltcmelo/uaiso
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


} // namespace psyche

#endif
