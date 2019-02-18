//
// Copyright (c) 2008-2015 the Urho3D project.
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
//

#pragma once

#include <Urho3D/Core/Object.h>
#include <TurboBadger/tb_widgets_listener.h>

#include "Sample.h"


namespace Urho3D
{
class Node;
class Scene;
}

namespace tb
{
class TBMessageWindow;
}

using namespace Urho3D;
using namespace tb;

//=====================================
URHO3D_EVENT(E_TBMSG, TBMessageNamespace)
{
    URHO3D_PARAM(P_TBWIDGET, Widget);  // TBWidget pointer
}

//=============================================================================
//=============================================================================
class UTBListener : public Object, public TBWidgetListener
{
    URHO3D_OBJECT(UTBListener, Object);
public:
    UTBListener(Context *context);

    ~UTBListener();

    void CreateMsgWindow();
	virtual void OnWidgetRemove(TBWidget *parent, TBWidget *child);
	virtual bool OnWidgetInvokeEvent(TBWidget *widget, const TBWidgetEvent &ev);

    TBWidget* GetTBMessageWidget()
    {
        return (TBWidget*)pTBMessageWindow_;
    }
protected:
    void SendEventMsg();

protected:
    TBMessageWindow     *pTBMessageWindow_;

};

//=============================================================================
//=============================================================================
/// GUI test example.
/// This sample demonstrates:
///     - Creating GUI elements from C++
///     - Loading GUI Style from xml
///     - Subscribing to GUI drag events and handling them.
class UIDrag : public Sample
{
    URHO3D_OBJECT(UIDrag, Sample);

public:
    /// Construct.
    UIDrag(Context* context);
    ~UIDrag();

    /// Setup after engine initialization and before running the main loop.
    virtual void Setup();
    virtual void Start();
    virtual void Stop();

protected:
    void HandleTBMessage(StringHash eventType, VariantMap& eventData);

    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return
        "<patch>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
        "        <attribute name=\"Is Visible\" value=\"false\" />"
        "    </add>"
        "</patch>";
    }

    UTBListener     *pTBListener_;

};
