/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2016                              
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*
*/

#ifndef _DRAWSPACE_COMMONS_H_
#define _DRAWSPACE_COMMONS_H_

#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <exception>

#include "flags.h"

#define dsstring    std::string
#define dswstring   std::wstring
#define dsreal      double
#define dslonglong  long long
#define dstime      __time64_t
#define dsexception std::exception

#define DRAWSPACE_GUI_WIDTH 200
#define DRAWSPACE_GUI_HEIGHT 150

dsstring operator<< ( dsstring& p_s1, dsstring& p_s2 );
dsstring operator<< ( dsstring& p_s1, const char* p_s2 );
dsstring operator<< ( const char* p_s1, dsstring& p_s2 );
dsstring operator<< ( dsstring& p_s1, int p_s2 );
dsstring operator<< ( dsstring& p_s1, size_t p_s2 );
dsstring operator<< ( dsstring& p_s1, void* p_s2 );
dsstring operator<< ( dsstring& p_s1, bool p_s2 );
dsstring operator<< ( dsstring& p_s1, DWORD p_s2 );
dsstring operator<< ( dsstring& p_s1, float p_s2 );

#endif
