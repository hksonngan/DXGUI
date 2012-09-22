//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define DXUT_AUTOLIB
#include "DXUT\DXUT.h"
#include "GUI\SDKgui.h"
//#include "DXUT\DXUTguiIME.h"
//#include "DXUT\ImeUi.h"

//#pragma comment( lib, "Imm32.Lib" )
//#pragma comment( lib, "Version.Lib" )
//#pragma comment( lib, "dsound.lib" )

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_EDITBOX1            5
#define IDC_EDITBOX2            6
#define IDC_ENABLEIME           7
#define IDC_DISABLEIME          8
#define IDC_COMBOBOX            9
#define IDC_STATIC              10
#define IDC_OUTPUT              12
#define IDC_SLIDER              13
#define IDC_CHECKBOX            14
#define IDC_CLEAREDIT           15
#define IDC_RADIO1A             16
#define IDC_RADIO1B             17
#define IDC_RADIO1C             18
#define IDC_RADIO2A             19
#define IDC_RADIO2B             20
#define IDC_RADIO2C             21
#define IDC_LISTBOX             22
#define IDC_LISTBOXM            23
#define IDC_GROUPBOX			24
#define IDC_PROGRESS			25
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CDXUTDialog             g_GameUI;             // dialog for sample specific controls

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
void    RenderText();

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
//    CDXUTIMEEditBox::Initialize( DXUTGetHWND() );

	V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );

    g_GameUI.SetLocation( 0, 0 );
    g_GameUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    //if( g_GameUI.GetControl( IDC_EDITBOX2 ) )
    //{
    //    g_GameUI.GetControl( IDC_EDITBOX2 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 280 );
    //    g_GameUI.GetControl( IDC_EDITBOX2 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
    //}
    //if( g_GameUI.GetControl( IDC_EDITBOX3 ) )
    //{
    //    g_GameUI.GetControl( IDC_EDITBOX3 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 380 );
    //    g_GameUI.GetControl( IDC_EDITBOX3 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
    //}
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
    
    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 236, 233, 216), 1.0f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		V( g_GameUI.OnRender( fElapsedTime ) );
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly

    // Give the dialogs a chance to handle the message first
	//*pbNoFurtherProcessing = CDXUTIMEEditBox::StaticMsgProc( hWnd, uMsg, wParam, lParam );
 //   if( *pbNoFurtherProcessing )
 //       return 0;

	// Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_GameUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9LostDevice();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9DestroyDevice();
}
//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );

    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    InitApp();

	DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"EmptyProject" );
    DXUTCreateDevice( true, 800, 600 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}
//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_GameUI.Init( &g_DialogResourceManager,true,L"UI/ui.png" );
	g_GameUI.SetCallback( OnGUIEvent );

    g_GameUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
    g_GameUI.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	
	// GroupBox
	g_GameUI.AddGroupBox( IDC_GROUPBOX, L"title", 10, 10, 750, 550 );

    // Radio buttons
    g_GameUI.AddRadioButton( IDC_RADIO1A, 1, L"Radio group 1 Amy (1)", 30, 30,  GUI_SIZE_SMALL, false, L'1' );
    g_GameUI.AddRadioButton( IDC_RADIO1B, 1, L"Radio group 1 Brian (2)", 30, 60, GUI_SIZE_MIDDLE, false, L'2' );
    g_GameUI.AddRadioButton( IDC_RADIO1C, 1, L"Radio group 1 Clark (3)", 30, 90, GUI_SIZE_LARGE, false, L'3' );

    // Checkbox
    g_GameUI.AddCheckBox( IDC_CHECKBOX, L"BUTTON A", 180, 30, GUI_SIZE_SMALL, false, L'A', false );
    g_GameUI.AddCheckBox( IDC_CHECKBOX, L"BUTTON B", 180, 60, GUI_SIZE_MIDDLE, false, L'B', false );
    g_GameUI.AddCheckBox( IDC_CLEAREDIT, L"BUTTON C",180, 90, GUI_SIZE_LARGE, false, L'C', false );


    // Buttons
    g_GameUI.AddButton( IDC_ENABLEIME, L"Enable (I)ME", 300, 30, 100, 13, L'I' );
    g_GameUI.AddButton( IDC_DISABLEIME, L"Disable I(M)E", 300, 60, 100, 16, L'M' );
    g_GameUI.AddButton( IDC_DISABLEIME, L"Disable IM(E)", 300, 90, 100, 25, L'E' );

    // Slider
    g_GameUI.AddSlider( IDC_SLIDER, 450, 30, 200, 24, 12, 3, 0, 10, 5, false );

    // Slider
    g_GameUI.AddProgress( IDC_PROGRESS, 450, 60, 200, 24, 20 );

    // Combobox
    CDXUTComboBox *pCombo;
    g_GameUI.AddComboBox( IDC_COMBOBOX, 450, 90, 200, 20, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 100 );
        pCombo->AddItem( L"Combobox item (O)", (LPVOID)0x11111111 );
        pCombo->AddItem( L"Placeholder (O)", (LPVOID)0x12121212 );
        pCombo->AddItem( L"One more (O)", (LPVOID)0x13131313 );
        pCombo->AddItem( L"I can't get enough (O)", (LPVOID)0x14141414 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151515 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151516 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151517 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151518 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151519 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151520 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151521 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151522 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151523 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151524 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151525 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151526 );
    }

	// Static
	g_GameUI.AddStatic( IDC_OUTPUT, L"This static control provides feedback for your action.  It will change as you interact with the UI controls.", 20, 120, 620, 30 );

    // Edit box
    g_GameUI.AddEditBox( IDC_EDITBOX1, L"Edit control with default styles. Type text here and press Enter", 20, 160, 600, 30 );


    // List box
    g_GameUI.AddListBox( IDC_LISTBOX, 450, 200, 200, 150 );
    for( int i = 0; i < 5; ++i )
    {
        WCHAR wszText[50];
        StringCchPrintf( wszText, 50, L"Single-selection listbox item %d", i );
        g_GameUI.GetListBox( IDC_LISTBOX )->AddItem( wszText, (LPVOID)(size_t)i );
    }
    g_GameUI.AddListBox( IDC_LISTBOXM, 450, 360, 200, 150, CDXUTListBox::MULTISELECTION );
    for( int i = 0; i < 15; ++i )
    {
        WCHAR wszText[50];
        StringCchPrintf( wszText, 50, L"Multi-selection listbox item %d", i );
        g_GameUI.GetListBox( IDC_LISTBOXM )->AddItem( wszText, (LPVOID)(size_t)i );
    }

	//CDXUTIMEEditBox *pIMEEdit;
 //   CDXUTIMEEditBox::InitDefaultElements( &g_GameUI );
	//if( SUCCEEDED( CDXUTIMEEditBox::CreateIMEEditBox( &g_GameUI, IDC_EDITBOX2, L"IME-capable edit control with custom styles. Type and press Enter", 20, 390, 600, 32, false, &pIMEEdit ) ) )
 //   {
 //       g_GameUI.AddControl( pIMEEdit );		
 //       //pIMEEdit->GetElement( 0 )->iFont = 1;//Set the TextFild font.
 //       //pIMEEdit->GetElement( 1 )->iFont = 1;//Set the IMESelect font.
 //       //pIMEEdit->GetElement( 9 )->iFont = 1;//Set the CTF font.
 //       //pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
 //       //pIMEEdit->SetBorderWidth( 7 );
 //       //pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
 //       //pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
 //       //pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
 //       //pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
 //   }

}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    WCHAR wszOutput[1024];
	switch( nControlID )
    {
        //case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        //case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        //case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
        //case IDC_ENABLEIME:
        //    CDXUTIMEEditBox::SetImeEnableFlag( true );
        //    g_GameUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Enable IME' button.\nIME text input is enabled for IME-capable edit controls." );
        //    break;
        //case IDC_DISABLEIME:
        //    CDXUTIMEEditBox::SetImeEnableFlag( false );
        //    g_GameUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Disable IME' button.\nIME text input is disabled for IME-capable edit controls." );
        //    break;
        case IDC_EDITBOX1:
        case IDC_EDITBOX2:
        //case IDC_EDITBOX3:
            switch( nEvent )
            {
                case EVENT_EDITBOX_STRING:
                {
                   StringCchPrintf( wszOutput, 1024, L"You have pressed Enter in edit control (ID %u).\nThe content of the edit control is:\n\"%s\"",
                               nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_GameUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    // Clear the text if needed
                    //if( g_GameUI.GetCheckBox( IDC_CLEAREDIT )->GetChecked() )
                   ((CDXUTEditBox*)pControl)->SetText( L"" );
                    break;
                }

                case EVENT_EDITBOX_CHANGE:
                {
                    StringCchPrintf( wszOutput, 1024, L"You have changed the content of an edit control (ID %u).\nIt is now:\n\"%s\"",
                                nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_GameUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    break;
                }
            }
            break;
    }
}
