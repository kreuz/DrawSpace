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

#include "dsappclient.h"

dsAppClient* dsAppClient::m_instance = NULL;

using namespace DrawSpace::Interface::Module;


_DECLARE_DS_LOGGER( logger, "vmapp", DrawSpace::Logger::Configuration::GetInstance() )


dsAppClient::dsAppClient( void ) :
m_mainloopservice( NULL )
{    
    _INIT_LOGGER( "logvm.conf" )
    m_w_title = "DrawSpace VM";
}

dsAppClient::~dsAppClient( void )
{
}

void dsAppClient::OnRenderFrame( void )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->Run();
    }
}

bool dsAppClient::OnIdleAppInit( void )
{
    _DSDEBUG(logger, dsstring("VM startup..."))

    Root* root = DrawSpace::Core::SingletonPlugin<Root>::GetInstance()->m_interface;

    m_mainloopservice = root->InstanciateService( "mainloop" );
    if( m_mainloopservice )
    {
        _DSDEBUG(logger, dsstring("mainloop service initialisation"))
        m_mainloopservice->Init( DrawSpace::Logger::Configuration::GetInstance() );
        return true;
    }
    else
    {
        _DSWARN(logger, dsstring("no mainloop service found, leaving..."))
        return false;
    }
}

void dsAppClient::OnAppInit( void )
{    
}

void dsAppClient::OnClose( void )
{
    _DSDEBUG(logger, dsstring("VM shutdown..."))
    if( m_mainloopservice )
    {
        _DSDEBUG(logger, dsstring("closing main loop service..."))
        m_mainloopservice->Release();
    }
}

void dsAppClient::OnKeyPress( long p_key ) 
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnKeyPress( p_key );
    }
}

void dsAppClient::OnEndKeyPress( long p_key )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnEndKeyPress( p_key );
    }
}

void dsAppClient::OnKeyPulse( long p_key )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnKeyPulse( p_key );
    }
}

void dsAppClient::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnMouseMove( p_xm, p_ym, p_dx, p_dy );
    }
}

void dsAppClient::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnMouseLeftButtonDown( p_xm, p_ym );
    }
}

void dsAppClient::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnMouseLeftButtonUp( p_xm, p_ym );
    }
}

void dsAppClient::OnMouseRightButtonDown( long p_xm, long p_ym )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnMouseRightButtonDown( p_xm, p_ym );
    }
}

void dsAppClient::OnMouseRightButtonUp( long p_xm, long p_ym )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnMouseRightButtonUp( p_xm, p_ym );
    }
}

void dsAppClient::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
    if( m_mainloopservice )
    {
        m_mainloopservice->OnAppEvent( p_wParam, p_lParam );
    }
}
