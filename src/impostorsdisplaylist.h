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

#ifndef _IMPOSTORSDISPLAYLIST_H_
#define _IMPOSTORSDISPLAYLIST_H_

#include "vector.h"

namespace DrawSpace
{
typedef struct
{
    float                       width_scale;
    float                       height_scale;
    DrawSpace::Utils::Vector    localpos;
    float                       spherical_ray;
    float                       spherical_longitud;
    float                       spherical_latitud;

    dsreal                      u1, v1;
    dsreal                      u2, v2;
    dsreal                      u3, v3;
    dsreal                      u4, v4;        

} ImpostorsDisplayListEntry;
   
typedef std::vector<ImpostorsDisplayListEntry> ImpostorsDisplayList;
}

#endif
