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

#include "test01root.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface::Module;

Test01Root::Test01Root( void )
{
    m_services["mainloop"] = new MainLoopService();
}

Test01Root::~Test01Root( void )
{
}

dsstring Test01Root::GetModuleName( void )
{
    return "Test01Mod";
}

dsstring Test01Root::GetModuleDescr( void )
{
    return "test01 module";
}

std::vector<dsstring> Test01Root::GetServicesList( void )
{
    std::vector<dsstring> list;

    for( auto it = m_services.begin(); it != m_services.end(); ++it )
    {
        list.push_back( it->first );
    }
    return list;
}

Service* Test01Root::InstanciateService( const dsstring& p_id )
{
    if( m_services.count( p_id ) > 0 )
    {
        return m_services[p_id];
    }
    return NULL;
}