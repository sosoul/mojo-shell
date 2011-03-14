#ifndef _DIRECTORY_MANAGER_H_
#define _DIRECTORY_MANAGER_H_

#include "predef.h"
#include "canvas.h"

#define LINE_HEIGHT		15


#define	TYPE_DIRECTORY	1 << 30
#define	TYPE_FILE		1 << 29

#define ATTR_READONLY	1 << 16
#define ATTR_ACHAIVE	1 << 15
#define ATTR_SYSTEM		1 << 14
#define ATTR_HIDDEN		1 << 13


struct  FILEINFO
{
	bool			m_selected;
	int				m_type;

	std::wstring	m_name;
	std::wstring	m_ext;
	std::wstring	m_fullpath;

	FILEINFO()
	{
		m_selected = false;
	}
};


class CDirectoryManager
{
public :
	CDirectoryManager();
	~CDirectoryManager();

	void	Init();
	void	CollectCurrentDirInfo();
	void	AnalizeDirectory(const wchar_t *path);
	void	ResetParam();

	void	Up();
	void	Down();
	void	PressKeyboard(int PressKey);
	void	DrawFile(Canvas *c);
	void	SetWindowSize(wxSize size)
	{
		m_windowSize = size;
		//m_windowSize.y -= 50;
	}

private :
	void	CalcDisplayInfo();

private :
	FILEINFO				m_parent;

	std::wstring			m_currentDir;
	std::vector<String>		m_filelists;

	std::vector<FILEINFO>	m_directoryinfolist;
	std::vector<FILEINFO>	m_fileinfolist;


	//////////////////////////////////////////////////////////////////////////
	wxSize					m_windowSize;

	int						m_displaytop;
	int						m_displaylines;
	int						m_selectedpos;

	std::vector<FILEINFO>	m_finalresult;
};



#endif