#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "predef.h"

class Canvas
{
public :
	Canvas(wxDC * dc, int w, int h);
	~Canvas();

	void	Clear(unsigned long color);
	wxSize	GetTextSize(const wchar_t * text, int fontsize);
	void	Print(int x, int y, const wchar_t * text, unsigned long color, bool rightalign, int fontsize);

	void	DrawRect(int x, int y, int w, int h, unsigned long color, bool outline);
	void	DrawLine(int x1, int y1, int x2, int y2, unsigned long color, int width);
	void	DrawBitmap(int x, int y, wxBitmap &bitmap, bool usealpha = false);
	void	SetClipping(int x, int y, int w, int h);
	void	ClearClipping();

	int		GetWidth() { return m_Size[0]; }
	int		GetHeight() { return m_Size[1]; }

protected :
	int		m_Size[2];
	wxDC*	m_DC;
	std::map <int, wxFont*>	m_Font;
};


#endif