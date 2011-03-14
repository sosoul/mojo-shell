#include "main.h"
#include "canvas.h"


#ifdef _DEBUG
#pragma  comment (lib, "wxmsw28ud_xrc.lib")
#pragma  comment (lib, "wxmsw28ud_html.lib")
#pragma  comment (lib, "wxbase28ud_xml.lib")
#pragma  comment (lib, "wxmsw28ud_adv.lib")
#pragma  comment (lib, "wxmsw28ud_core.lib")
#pragma  comment (lib, "wxbase28ud.lib")
#pragma  comment (lib, "wxregexud.lib")

#pragma  comment (lib, "wxtiffd.lib")
#pragma  comment (lib, "wxjpegd.lib")
#pragma  comment (lib, "wxpngd.lib")
#pragma  comment (lib, "wxzlibd.lib")
#pragma  comment (lib, "wxexpatd.lib")
#pragma  comment (lib, "wxcode_msw28ud_propgrid.lib")

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "rpcrt4.lib")

#else
#pragma  comment (lib, "wxmsw28u_xrc.lib")
#pragma  comment (lib, "wxmsw28u_html.lib")
#pragma  comment (lib, "wxbase28u_xml.lib")
#pragma  comment (lib, "wxmsw28u_adv.lib")
#pragma  comment (lib, "wxmsw28u_core.lib")
#pragma  comment (lib, "wxbase28u.lib")
#pragma  comment (lib, "wxregexu.lib")

#pragma  comment (lib, "wxtiff.lib")
#pragma  comment (lib, "wxjpeg.lib")
#pragma  comment (lib, "wxpng.lib")
#pragma  comment (lib, "wxzlib.lib")
#pragma  comment (lib, "wxexpat.lib")
#pragma  comment (lib, "wxcode_msw28u_propgrid.lib")

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "rpcrt4.lib")

#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(CApp)


MojoShell*	MojoShell::m_Instance;


bool CApp::OnInit()
{
	//_CrtSetBreakAlloc(304329);
	timeBeginPeriod(1);
	srand(timeGetTime());

	wxInitAllImageHandlers();

	MojoShell *frame = MojoShell::GetInstance();
	
	frame->Centre();
	frame->Show(TRUE);

	SetTopWindow(frame);
	timeEndPeriod(1);

	return true;
}

int CApp::OnExit()
{

	return 0;
}


//////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MojoShell, wxFrame)
	EVT_CLOSE(MojoShell::OnClose)
	EVT_SIZE(MojoShell::OnSize)
	EVT_PAINT(MojoShell::OnPaint)

	EVT_KEY_DOWN( MojoShell::OnKeyDown )

END_EVENT_TABLE()


MojoShell::MojoShell(const wxString& title, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxFrame(NULL, id, title, pos, size, style),
m_backBuffer(NULL)
{
	//SetIcon(wxICON(mondrian));
	m_directoryMan.Init();
}


MojoShell::~MojoShell()
{
	if( m_backBuffer )
		delete m_backBuffer;
}

void	MojoShell::OnSize(wxSizeEvent& event)
{
//	wxSize size = event.GetSize();
	wxSize csize = GetClientSize();

 	if (m_backBuffer != NULL)
	{
		m_backBuffer->SetWidth(csize.x);
		m_backBuffer->SetHeight(csize.y);
	}
	else
	{
		m_backBuffer = new wxBitmap(csize.x, csize.y);
	}
// 		delete m_backBuffer;
// 	m_backBuffer = new wxBitmap(size.x, size.y);

	m_directoryMan.SetWindowSize(csize);

	//Refresh(false);
}

void	MojoShell::OnClose(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
}

void	MojoShell::OnKeyDown(wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	m_directoryMan.PressKeyboard(keycode);

	Refresh(false);
}

void	MojoShell::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc( this );
	wxSize size = GetClientSize();

	if (m_backBuffer == NULL)
		m_backBuffer= new wxBitmap(size.x, size.y);

	wxMemoryDC memdc;
	memdc.SelectObject(*m_backBuffer);
	memdc.SetBackground(*wxBLACK_BRUSH);
	memdc.Clear();

	Canvas c(&memdc, size.x, size.y);
	c.Clear(0xff000000);
	m_directoryMan.DrawFile(&c);
	dc.Blit(0, 0, size.x, size.y, &memdc, 0, 0);		// 전체화면 dump
}


