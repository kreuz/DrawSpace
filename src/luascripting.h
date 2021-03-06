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

#ifndef _LUASCRIPTING_H_
#define _LUASCRIPTING_H_

#include "scripting.h"
#include "luacontext.h"

class LuaScripting : public Scripting
{    
public:
    LuaScripting( void );

    bool Initialize( void );
    void Shutdown( void );

    bool ExecChunk( const char* p_cmd );
    bool ExecFile( const char* p_path );
    void GetLastError( dsstring& p_str );


    void RegisterScriptErrorHandler( ErrorHandler* p_error_handler );
    void RegisterScriptCallsHandler( ScriptCallsHandler* p_handler );
};


#endif
