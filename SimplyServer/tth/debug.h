/*
 * Copyright (C) 2001-2013 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DCPP_DCPLUSPLUS_DEBUG_H_
#define DCPP_DCPLUSPLUS_DEBUG_H_

#ifdef _DEBUG

#include <cassert>
#include <stdarg.h>
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

inline void debugTrace(const char* format, ...)
{
	va_list args;
	va_start(args, format); //-V111
#if defined _WIN32 && defined _MSC_VER
	char buf[512];
	buf[0] = 0;
	_vsnprintf(buf, _countof(buf), format, args);
	OutputDebugStringA(buf);
#else // _WIN32
	vprintf(format, args);
#endif // _WIN32
	va_end(args);
}

#define dcdebug debugTrace
#ifdef _MSC_VER


#define dcassert(exp) \
	{ if (!(exp)) { \
			dcdebug("Assertion hit in %s(%d): " #exp "\n", __FILE__, __LINE__); \
			if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #exp)) \
				_CrtDbgBreak(); } }
#else
#define dcassert(exp) assert(exp)
#endif
#define dcdrun(exp) exp
#else //_DEBUG
#define dcdebug if (false) printf
#define dcassert(exp)
#define dcdrun(exp)
#endif //_DEBUG

#endif /* DCPP_DCPLUSPLUS_DEBUG_H_ */
