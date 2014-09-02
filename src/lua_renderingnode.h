/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2014                                
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

#ifndef _LUA_RENDERINGNODE_H_
#define _LUA_RENDERINGNODE_H_

#include "lua_binding.h"
#include "renderingnode.h"

namespace DrawSpace
{
class LuaRenderingNode : public LuaBinding
{
protected:

    bool                                m_release_object;
    DrawSpace::Core::RenderingNode*     m_renderingnode;

    void                                cleanup( void );

public:
    LuaRenderingNode( lua_State* p_L );
    ~LuaRenderingNode( void );

    DrawSpace::Core::RenderingNode* GetObject( void ) { return m_renderingnode; };

    int Lua_SetObject( lua_State* p_L );
    int Lua_GetObject( lua_State* p_L );

    int Lua_InstanciateObject( lua_State* p_L );

    static const char className[];
    static const DrawSpace::Luna<LuaRenderingNode>::RegType Register[];
};
}


#endif