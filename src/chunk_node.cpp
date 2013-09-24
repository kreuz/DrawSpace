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

#include "chunk_node.h"
#include "renderer.h"
#include "plugin.h"
#include "memalloc.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;

ChunkNode::ChunkNode( const dsstring& p_name ) : ScenegraphNode( p_name )
{
    m_meshe = _DRAWSPACE_NEW_( Core::Meshe, Core::Meshe );
}

ChunkNode::~ChunkNode( void )
{
    _DRAWSPACE_DELETE_( m_meshe );
}

void ChunkNode::on_renderingnode_draw( Core::RenderingNode* p_rendering_node )
{
    DrawSpace::Interface::Renderer* renderer = DrawSpace::Core::Plugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
    renderer->RenderNodeMeshe( m_globaltransformation, m_view, p_rendering_node, 0 );
}

Meshe* ChunkNode::GetMeshe( void )
{
    return m_meshe;
}

void ChunkNode::RegisterPassRenderingNode( const dsstring p_passname, Core::RenderingNode* p_rendering_node  )
{
    m_passes[p_passname] = p_rendering_node;

    RenderingNodeDrawCallback* cb = _DRAWSPACE_NEW_( RenderingNodeDrawCallback, RenderingNodeDrawCallback( this, &ChunkNode::on_renderingnode_draw ) );
    p_rendering_node->RegisterHandler( cb );
    m_callbacks.push_back( cb );
}

void ChunkNode::OnRegister( Scenegraph* p_scenegraph )
{
    for( std::map<dsstring, Core::RenderingNode*>::iterator it = m_passes.begin(); it != m_passes.end(); ++it )
    {
        Pass* current_pass = p_scenegraph->GetPass( (*it).first );
        if( current_pass != NULL )
        {
            current_pass->GetRenderingQueue()->Add( (*it).second );
        }
    }
}

bool ChunkNode::LoadAssets( void )
{
    DrawSpace::Interface::Renderer* renderer = DrawSpace::Core::Plugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    for( std::map<dsstring, Core::RenderingNode*>::iterator it = m_passes.begin(); it != m_passes.end(); ++it )
    {
        if( false == renderer->CreateRenderingNode( (*it).second ) )
        {
            return false;
        }
        if( -1 == renderer->AddMesheToNode( m_meshe, (*it).second ) )
        {
            return false;
        }
    }
    return true;		
}
