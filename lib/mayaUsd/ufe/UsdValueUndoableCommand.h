//
// Copyright 2020 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once

#include "UsdUndoableInteractiveCommand.h"

#include <pxr/base/vt/value.h>
#include <pxr/usd/usdGeom/xformOp.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace MAYAUSD_NS_DEF {
namespace ufe {

// Templated helper class to factor out common code for setting USD values.
//
// Implement the execute, undo and redo of the UFE command interfaces, with
// common code protecting against early undo/redo preceeding the initial
// execution and declaring the UsdUndoBlock during the execution.
//
// Sub-classes should *not* have to override execute(), undo() nor redo().
//
// Sub-classes only need to implement the handleSet() function.
//
// Sub-classes can override set() if the command support interactive
// manipulations. Since each set() in specific to the specific Cmd base
// class, we cannot provide a generic implementation in this template.
//
// A typical set() implementation should call setNewValue() with the
// new value and then execute() to actually set the value on the USD
// attribute.
template <typename Cmd> class UsdValueUndoableCommand : public UsdUndoableInteractiveCommand<Cmd>
{
public:
    using BaseClass = UsdUndoableInteractiveCommand<Cmd>;

    UsdValueUndoableCommand(
        const VtValue&     newOpValue,
        const Ufe::Path&   path,
        const UsdTimeCode& writeTime_)
        : BaseClass(path)
        // Always read from proxy shape time.
        , _readTime(getTime(path))
        , _writeTime(writeTime_)
        , _newValue(newOpValue)
    {
    }

    UsdTimeCode readTime() const { return _readTime; }

    UsdTimeCode writeTime() const { return _writeTime; }

protected:
    // Update the new value that will be set by execute().
    void setNewValue(const VtValue& v) { _newValue = v; }

    // Concrete implementation of execute, undo and redo.
    //
    // Execute creates an undo block with the undoable item and calls handleSet().
    void executeUndoBlock() override { handleSet(_newValue); }

    // Overridable method for derived classes to actually set the value on
    // the USD attribute. The handleSet() call will be within a USD undo block
    // as necessary. You don't need to declare such a block.
    virtual void handleSet(const VtValue& v) = 0;

private:
    const UsdTimeCode _readTime;
    const UsdTimeCode _writeTime;
    VtValue           _newValue;
};

} // namespace ufe
} // namespace MAYAUSD_NS_DEF
