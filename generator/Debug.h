/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "Config.h"
#include <iostream>
#include <string>

namespace psyche {

extern bool debugEnabled;
extern bool debugVisit;

#define PSYCHE_COMPONENT "psyche"

/*!
 * \brief printDebug
 *
 * Print debug info (if debug is enabled) with variable args.
 */
template<typename... Args>
void printDebug(const char* message, Args... args)
{
    if (!debugEnabled)
        return;

    std::cout << "[" << PSYCHE_COMPONENT << "] ";
    std::printf(message, args...);
}

inline void printDebug(const char* message)
{
    if (!debugEnabled)
        return;

    std::cout << "[" << PSYCHE_COMPONENT << "] " << message;
}

/*!
 * \brief The VisitorDebugger struct
 *
 * Helper RAII class to debug visitor's visit methods.
 */
class PSYCHEC_API VisitorDebugger
{
public:
    VisitorDebugger(const std::string& visit)
        : visit_(visit)
    {
        if (debugVisit)
            printf("begin - %s\n", visit_.c_str());
    }

    ~VisitorDebugger()
    {
        if (debugVisit)
            printf("end   - %s\n", visit_.c_str());
    }

private:
    std::string visit_;
};

} // namespace psyche

#define DEBUG_VISIT(METHOD) VisitorDebugger x(VISITOR_NAME"["#METHOD"]")
