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

#include <Urho3D/Urho3D.h>

#include "UIDrag.h"
#include "UTBRendererBatcher.h"
#include "TBDemo.h"

#include <Urho3D/DebugNew.h>

//=============================================================================
//=============================================================================
URHO3D_DEFINE_APPLICATION_MAIN(UIDrag)


//=============================================================================
//=============================================================================
UIDrag::UIDrag(Context* context) 
    : Sample(context)
    , pTBListener_( NULL )
{
}

UIDrag::~UIDrag()
{
    if ( pTBListener_ )
    {
        delete pTBListener_;
        pTBListener_ = NULL;
    }
}

//=============================================================================
//=============================================================================
void UIDrag::Setup()
{
    // Modify engine startup parameters
    engineParameters_["WindowTitle"] = GetTypeName();
    engineParameters_["LogName"]     = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    engineParameters_["FullScreen"]  = false;
    engineParameters_["Headless"]    = false;
    engineParameters_["WindowWidth"] = 1280; 
    engineParameters_["WindowHeight"] = 720;
}

//=============================================================================
//=============================================================================
void UIDrag::Start()
{
    // Execute base class startup
    Sample::Start();

    // Set mouse visible
    String platform = GetPlatform();
    if (platform != "Android" && platform != "iOS")
        GetSubsystem<Input>()->SetMouseVisible(true);

    // create TB render batcher
    Graphics *graphics = GetSubsystem<Graphics>();
    UTBRendererBatcher::Create( GetContext(), graphics->GetWidth(), graphics->GetHeight() );
    UTBRendererBatcher::Singleton().Init( "Data/TB/" );
    
    // create a msg win
    pTBListener_ = new UTBListener( GetContext() );
    pTBListener_->CreateMsgWindow();

    SubscribeToEvent(E_TBMSG, URHO3D_HANDLER(UIDrag, HandleTBMessage));
}

void UIDrag::Stop()
{
    TBDemo::Destroy();
    UTBRendererBatcher::Destroy();
    Sample::Stop();
}

void UIDrag::HandleTBMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace TBMessageNamespace;

    TBWidget *pTBWidget = (TBWidget*)eventData[P_TBWIDGET].GetVoidPtr();

    if ( pTBListener_->GetTBMessageWidget() == pTBWidget )
    {
        TBDemo::Init();
    }
}

//=============================================================================
//=============================================================================
UTBListener::UTBListener(Context *context)
    : Object(context)
    , pTBMessageWindow_( NULL )
{
}

UTBListener::~UTBListener()
{
    if ( pTBMessageWindow_ )
    {
        pTBMessageWindow_ = NULL;
    }
}

void UTBListener::CreateMsgWindow()
{
    TBStr text("\nHellow World from Turbo Badger.\n\nClick OK to get started.");
    pTBMessageWindow_ = new TBMessageWindow( &UTBRendererBatcher::Singleton().Root(), TBIDC("") );
    pTBMessageWindow_->Show("Start Message", text);

    TBWidgetListener::AddGlobalListener( this );
}

void UTBListener::OnWidgetRemove(TBWidget *parent, TBWidget *child)
{
    if ( pTBMessageWindow_ == child )
    {
        TBWidgetListener::RemoveGlobalListener( this );
    }
}

bool UTBListener::OnWidgetInvokeEvent(TBWidget *widget, const TBWidgetEvent &ev) 
{ 
    if ( pTBMessageWindow_ && pTBMessageWindow_ == ev.target )
    {
        if ( ev.type == EVENT_TYPE_CLICK )
        {
            TBWidgetListener::RemoveGlobalListener( this );

            SendEventMsg();
        }
    }

    return false; 
}

void UTBListener::SendEventMsg()
{
    using namespace TBMessageNamespace;

    VariantMap& eventData = GetEventDataMap();
    eventData[P_TBWIDGET] = GetTBMessageWidget();

    SendEvent( E_TBMSG, eventData );
}
