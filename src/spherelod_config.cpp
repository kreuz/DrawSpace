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

#include "spherelod_config.h"

using namespace DrawSpace::SphericalLOD;

Config::Config( void ) :
m_patchResol( 11 ),
m_nbLodRanges( 16 ),
m_bottomSlipping_sup( 8 ),
m_bottomSlipping_inf( 0 ),
m_amplitude( 12000.0 ),
m_fbmInputMin( -10.0 ),
m_fbmInputMax( 10.0 ),
m_fbmLacunarity( 2.0 ),
m_fbmRoughness( 0.5 ),
m_fbmClamp( true ),
m_fbmClip( true ),
m_fbmClipValue( 0.0 )
{
}

Config::~Config( void )
{
}