/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2015                              
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

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include "drawspace_commons.h"
#include "renderingnode.h"
#include "vector.h"

namespace DrawSpace
{
namespace Utils
{

typedef struct
{
    dsstring                    id;
    long                        shader_index;
    long                        shader_register;
    DrawSpace::Utils::Vector    value;

} PassShaderParam;

typedef struct 
{
    dsstring                        fx_name;
    long                            rendering_order;
    dsstring                        textures[6][DrawSpace::Core::RenderingNode::NbMaxTextures];
    std::vector<PassShaderParam>    shader_params;

} PassDescriptor;

typedef struct
{
    dsstring                            scene_name;
    std::map<dsstring, PassDescriptor>  passes_slots;

} SpaceboxDescriptor;

/////////////////////////////////////////////////////////////////

typedef struct
{    
    bool            leftbutton_down;
    bool            rightbutton_down;
    int             xmouse;
    int             ymouse;
    int             delta_xmouse;
    int             delta_ymouse;

} MouseMovementsDescriptor;

}
}

#endif