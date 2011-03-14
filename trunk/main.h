#ifndef _MOJODIR_H_
#define _MOJODIR_H_

#include "predef.h"
#include "directoryman.h"


#define MOJOWINDOW_POS_X		400
#define MOJOWINDOW_POS_Y		100

#define MOJOWINDOW_SIZEX		800
#define MOJOWINDOW_SIZEY		600


class CApp : public wxApp 
{
public:
	CApp() {}
	virtual bool OnInit();
	virtual int OnExit();
};


class MojoShell : public wxFrame
{

private :
	MojoShell(const wxString& title, wxWindowID id, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
	static MojoShell*	m_Instance;

	~MojoShell();

public:
	static MojoShell *GetInstance()
	{
		if (m_Instance == NULL)
		{
			wxDisplay	disp;
			wxRect rc = disp.GetGeometry();
			wxSize	ssize(MOJOWINDOW_SIZEX, MOJOWINDOW_SIZEY);
			wxPoint pos(MOJOWINDOW_POS_X , MOJOWINDOW_POS_Y);
			m_Instance = new MojoShell(_T("Mojo Shell"), wxID_ANY, pos, ssize);
		}
		return m_Instance;
	}

	void	OnPaint(wxPaintEvent &event);
	void	OnSize(wxSizeEvent& event);
	void	OnClose(wxCloseEvent& WXUNUSED(event));
	void	OnKeyDown(wxKeyEvent& event);

	DECLARE_EVENT_TABLE()

private :
	CDirectoryManager	m_directoryMan;
	wxBitmap*			m_backBuffer;
};



#endif