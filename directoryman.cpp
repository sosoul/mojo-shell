#include <cctype>
#include <algorithm>
#include "directoryman.h"
#include "stringutil.h"
#include "trace.h"


CDirectoryManager::CDirectoryManager()
{
	ResetParam();
}

CDirectoryManager::~CDirectoryManager()
{

}


void	CDirectoryManager::Init()
{
	wchar dir[MAX_PATH];
	GetCurrentDirectory(_countof(dir), dir);
	m_currentDir = dir;

	CollectCurrentDirInfo();
}


void	CDirectoryManager::ResetParam()
{
	m_selectedpos = 0;
	m_displaytop = 0;
	m_displaylines = 0;
}

std::wstring splitfilename(const wchar_t* path) 
{ 
	wchar t[_MAX_FNAME]; 
	_wsplitpath_s(path, NULL, 0, NULL, 0, t, _MAX_FNAME, NULL, 0); 
	return std::wstring(t); 
}

std::wstring splitfileext(const wchar* path) 
{ 
	wchar t[_MAX_EXT]; 
	_wsplitpath_s(path, NULL, 0, NULL, 0, NULL, 0, t, _MAX_EXT); 
	return std::wstring(t); 
}

std::wstring splitfilepath(const wchar_t* path) 
{ 
	wchar t[_MAX_DRIVE], d[_MAX_DIR], f[_MAX_FNAME]; 
	_wsplitpath_s(path, t, _MAX_DRIVE, d, _MAX_DIR, f, _MAX_FNAME, NULL, 0); 
	return std::wstring(t) + std::wstring(d) + std::wstring(f); 
}

void	CDirectoryManager::AnalizeDirectory(const wchar *path)
{
	HANDLE hSrch;
	WIN32_FIND_DATAW wfd;
	BOOL bResult=TRUE;
	wchar drive[_MAX_DRIVE];
	wchar dir[MAX_PATH];
	wchar temppath[MAX_PATH];
	
	unicode::strcpy(temppath, _countof(temppath), path);
	unicode::strcat(temppath, _countof(temppath), _T("\\*.*"));
 
	hSrch=FindFirstFile(temppath, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) return;

	_wsplitpath_s(path, drive, _countof(drive), dir, _countof(dir), NULL, 0, NULL, 0);

	while (bResult) 
	{
		if( unicode::strcmp(wfd.cFileName, _T(".") ) == 0 )
		{
			_TRACE( _T("current dir : %s\n"), wfd.cFileName);
		}
		else if( unicode::strcmp(wfd.cFileName, _T("..") ) == 0 && wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_parent.m_type = TYPE_DIRECTORY;
			m_parent.m_name = _T("..");

			int len= unicode::strlen(dir);
			if( dir[len-1] == '\\')
				dir[len-1] = 0;

			m_parent.m_fullpath = String(drive) + String(dir);

			//m_directoryinfolist.push_back(info);
			_TRACE( _T("up dir : %s : %s\n"), m_parent.m_name.c_str(), m_parent.m_fullpath.c_str());
		}
		else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			FILEINFO info;
			info.m_type = TYPE_DIRECTORY;
			info.m_name = wfd.cFileName;

			transform(info.m_name.begin(), info.m_name.end(), info.m_name.begin(), toupper);

			int len = unicode::strlen(path);
			if(path[len-1] == '\\')
			info.m_fullpath = String(path) + info.m_name;
			else
			info.m_fullpath = String(path) + String(_T("\\")) + info.m_name;
			//info.m_fullpath = String(path) + info.m_name;

			_TRACE( _T("dir : %s\n"), info.m_name.c_str());
			//_wsplitpath(path, drive, dir, NULL, NULL);
			m_directoryinfolist.push_back(info);
		} 
		else 
		{
			FILEINFO info;
			info.m_type = TYPE_FILE;
			info.m_name = splitfilename(wfd.cFileName);
			info.m_ext = splitfileext(wfd.cFileName);

			info.m_fullpath = String(path) + String(_T("\\")) + info.m_name;

			transform(info.m_name.begin(), info.m_name.end(), info.m_name.begin(), toupper);

			_TRACE( _T("file : %s\n"), info.m_name.c_str());
			m_fileinfolist.push_back(info);
		}

		bResult=FindNextFile(hSrch,&wfd);
	}

	FindClose(hSrch);
}

bool sortfunc( FILEINFO& elem1, FILEINFO& elem2 )
{
	return elem1.m_name > elem2.m_name;	// 알파벳순
}

void	CDirectoryManager::CollectCurrentDirInfo()
{
	ResetParam();
	m_finalresult.clear();

	AnalizeDirectory(m_currentDir.c_str());

	sort( m_directoryinfolist.begin(), m_directoryinfolist.end(), sortfunc );
	sort( m_fileinfolist.begin(), m_fileinfolist.end(), sortfunc );

	m_finalresult.push_back(m_parent);

	std::vector<FILEINFO>::iterator it1 = m_directoryinfolist.begin();
	for (; it1 != m_directoryinfolist.end(); it1++)
	{
		m_finalresult.push_back(*it1);
	}

	std::vector<FILEINFO>::iterator it2 = m_fileinfolist.begin();
	for (; it2 != m_fileinfolist.end(); it2++)
	{
		m_finalresult.push_back(*it2);
	}	

	m_directoryinfolist.clear();
	m_fileinfolist.clear();
}

void	CDirectoryManager::Up()
{
	if( m_selectedpos > 0 )
		m_selectedpos--;

	if( m_selectedpos <= m_displaytop)
		m_displaytop = m_selectedpos;

//	if( m_displaytop + m_displaylines <= m_selectedpos )
//		m_displaytop++;
}

void	CDirectoryManager::Down()
{
	if(m_finalresult.size() > m_selectedpos+1 )
		m_selectedpos++;

	if( m_displaytop + m_displaylines <= m_selectedpos )
		m_displaytop++;
}

void	CDirectoryManager::PressKeyboard(int keycode)
{
	if( keycode == WXK_RIGHT )
	{

	}
	else if( keycode == WXK_LEFT )
	{

	}
	else if( keycode == WXK_UP )
	{
		Up();
	}
	else if( keycode == WXK_DOWN )
	{
		Down();
	}
	else if( keycode == WXK_RETURN )
	{
		if( m_finalresult[m_selectedpos].m_type & TYPE_DIRECTORY)
		{
			// change directory
			m_currentDir = m_finalresult[m_selectedpos].m_fullpath;
			CollectCurrentDirInfo();
			m_selectedpos = 0;
		}
	}
}


void	CDirectoryManager::DrawFile(Canvas *c)
{
	size_t nstring = m_finalresult.size();

/*
	for (size_t i=0; i<nstring; i++)
	{
		unsigned long color = m_finalresult[i].m_type & TYPE_DIRECTORY ? 0xffff0000 : 0xffffffff;
		c->Print(5, i * LINE_HEIGHT, m_finalresult[i].m_fullpath.c_str(), color, false, 9);

		if( m_selectedpos == i )
		{
			c->DrawRect(0, i * LINE_HEIGHT, c->GetWidth(), LINE_HEIGHT, 0xff00ffff, true);
		}
	}
*/

// 	int h = m_windowSize.y;
// 	if( nstring > (h/LINE_HEIGHT)+1 )
// 	{
// 		m_displaylines = h/LINE_HEIGHT+1;
// 	}
// 	else
// 		m_displaylines = nstring;


	CalcDisplayInfo();


	//for (int i=m_displaytop ; i<m_displaylines; i++)
	for (int i=0 ; i<m_displaylines; i++)
	{
		int pos = m_displaytop + i;
		unsigned long color = m_finalresult[pos].m_type & TYPE_DIRECTORY ? 0xffff0000 : 0xffffffff;
		//c->Print(5, i * LINE_HEIGHT, m_finalresult[pos].m_fullpath.c_str(), color, false, 9);
		c->Print(5, i * LINE_HEIGHT, m_finalresult[pos].m_name.c_str(), color, false, 9, 370, 20);

		if( m_selectedpos == pos )
		{
			c->DrawRect(3, i * LINE_HEIGHT, c->GetWidth(), LINE_HEIGHT, 0xff00ffff, true);
		}
	}

	wchar_t temp[64];
	unicode::sprintf(temp, _countof(temp), _T("%d"), m_displaylines);
	c->Print(450, 10, temp, 0xffffffff, false, 9);

}

void	CDirectoryManager::CalcDisplayInfo()
{
	size_t nstring = m_finalresult.size();
	int h = m_windowSize.y;

	if( nstring - m_displaytop >= (h/LINE_HEIGHT) )
	{
		m_displaylines = h/LINE_HEIGHT;
		//_TRACE(_T("1> %d\n"),m_displaylines);
	}
	else
	{
		m_displaylines = nstring;
		//_TRACE(_T("2> %d\n"),m_displaylines);
	}
}


