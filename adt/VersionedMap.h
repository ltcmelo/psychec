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

#ifndef PSYCHE_VERSIONED_MAP_H__
#define PSYCHE_VERSIONED_MAP_H__

#include <algorithm>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace psy {

/*!
 * A versioned map.
 */
template <class KeyT, class ValueT>
class VersionedMap
{
private:
    using BaseMap = std::unordered_map<KeyT, ValueT>;

public:
    using iterator = typename BaseMap::iterator;
    using const_iterator = typename BaseMap::const_iterator;

    VersionedMap() = default;

    void insertOrAssign(const KeyT& key, const ValueT& value);
    void insertOrAssign(const KeyT& key, ValueT&& value);

    void applyRevision(uint32_t revision);
    uint32_t revision() const { return curRevision_; }

    // Basic traversal.
    const_iterator begin() const { return map_.begin(); }
    const_iterator end() const { return map_.end(); }
    const_iterator cbegin() const { return map_.begin(); }
    const_iterator cend() const { return map_.end(); }
    const_iterator find(const KeyT& key) const { return map_.find(key); }

private:
    void insertOrAssignCore(const KeyT& key, const ValueT& value);
    void insertOrAssignCore(const KeyT& key, ValueT&& value);

    // Encapsulates the action, as GoF's command other.
    struct Command
    {
        enum OpCode
        {
            Insert,
            Remove
        };

        Command(const KeyT& key, const ValueT& value, OpCode code)
            : key_(key), value_(value), opCode_(code)
        {}
        const KeyT key_;
        const ValueT value_;
        OpCode opCode_;
    };

    void storeCommand(const Command& command);

    //! Commands for each given revision.
    std::vector<Command> commands_;

    //!@{
    /*!
     * Revisioning mechanism.
     */
    uint32_t curRevision_ { 0 };
    uint32_t revisionCnt_ { 0 };
    std::unordered_map<uint32_t, uint32_t> reverts_;
    //!@}

    //! The actual underlying map.
    BaseMap map_;
};

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::insertOrAssign(const KeyT& key,
                                                const ValueT& value)
{
    storeCommand(Command(key, value, Command::Insert));
    insertOrAssignCore(key, value);
}

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::insertOrAssignCore(const KeyT& key,
                                                    const ValueT& value)
{
    map_[key] = value;
}

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::insertOrAssign(const KeyT& key,
                                                ValueT&& value)
{
    storeCommand(Command(key, value, Command::Insert));
    insertOrAssignCore(key, std::move(value));
}

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::insertOrAssignCore(const KeyT& key,
                                                    ValueT&& value)
{
    // We don't want to use operator[] because this would require ValueT to
    // have a default constructor (Range, for instance, doesn't have one, and
    // this is by design). C++17 has already insert_or_assign.
    auto it = map_.find(key);
    if (it != map_.end())
        it->second = std::move(value);
    else
        map_.insert(std::make_pair(key, std::move(value)));
}

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::storeCommand(const Command& command)
{
    commands_.push_back(command);
    reverts_.insert(std::make_pair(++revisionCnt_, curRevision_));
    curRevision_ = revisionCnt_;
}

template <class KeyT, class ValueT>
void VersionedMap<KeyT, ValueT>::applyRevision(uint32_t revision)
{
    std::vector<uint32_t> ordered;
    auto it = reverts_.find(revision);
    while (it != reverts_.end()) {
        ordered.push_back(it->second);
        it = reverts_.find(it->second);
    }

    curRevision_ = revision;
    if (ordered.empty())
        return; // Nothing to do.

    std::reverse(ordered.begin(), ordered.end());
    map_.clear();
    for (auto current : ordered) {
        const auto& command = commands_[current];
        ValueT value = command.value_;
        if (command.opCode_ == Command::Insert)
            insertOrAssignCore(command.key_, std::move(value));
    }
}

} // psy

#endif
