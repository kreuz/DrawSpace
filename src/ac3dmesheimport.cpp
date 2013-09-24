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

#include "ac3dmesheimport.h"
#include "tracedefs.h"
_DECLARE_DS_LOGGER( logger, "ac3dimport" )

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

AC3DMesheImport::AC3DMesheImport( void )
{

}

AC3DMesheImport::~AC3DMesheImport( void )
{
}

bool AC3DMesheImport::on_new_line( const dsstring& p_line, long p_line_num, std::vector<dsstring>& p_words )
{
    switch( m_state )
    {
        case SEARCH_OBJECT_BEGIN:

            if( "OBJECT" == p_words[0] )
            {
                if( "world" != p_words[1] )
                {					
                    if( m_object_count == m_object_index )
                    {
                        //m_state	= SEARCH_OBJECT_LOC;
                        m_state	= SEARCH_VERT_BEGIN;
                    }
                    m_object_count++;
                }
            }
            break;

            /*
        case SEARCH_OBJECT_LOC:

            if( "loc" == p_words[0] )
            {
                m_object_loc[0] = atof( p_words[1].c_str() );
                m_object_loc[1] = atof( p_words[2].c_str() );
                m_object_loc[2] = atof( p_words[3].c_str() );

                m_state = SEARCH_VERT_BEGIN;
            }
            break;
            */

        case SEARCH_VERT_BEGIN:

            if( "loc" == p_words[0] )
            {
                m_object_loc[0] = atof( p_words[1].c_str() );
                m_object_loc[1] = atof( p_words[2].c_str() );
                m_object_loc[2] = atof( p_words[3].c_str() );
            }
            if( "numvert" == p_words[0] )
            {
                m_state = VERT_INPUT;
                m_numvert = atoi( p_words[1].c_str() );
                m_vertcount = 0;
            }
            break;

        case VERT_INPUT:
            {
                DrawSpace::Core::Vertex vertex;
                vertex.x = atof( p_words[0].c_str() );
                vertex.x += m_object_loc[0];

                vertex.y = atof( p_words[1].c_str() );
                vertex.y += m_object_loc[1];

                vertex.z = atof( p_words[2].c_str() );
                vertex.z += m_object_loc[2];

                m_meshe->AddVertex( vertex );				
                m_vertcount++;

                if( m_vertcount == m_numvert )
                {
                    m_state = SEARCH_TRILIST_BEGIN;
                }
            }
            break;

        case SEARCH_TRILIST_BEGIN:

            if( "numsurf" == p_words[0] )
            {				
                m_state = SEARCH_TRI_BEGIN;
                m_numtri = atoi( p_words[1].c_str() );
                m_tricount = 0;
            }
            break;

        case SEARCH_TRI_BEGIN:

            if( "refs" == p_words[0] )
            {
                m_trilinecount = 0;
                m_state = TRI_INPUT;	
            }
            break;

        case TRI_INPUT:
            {
                long index = atoi( p_words[0].c_str() );
                float u = (float)atof( p_words[1].c_str() );
                float v = 1.0f - (float)atof( p_words[2].c_str() );

                _DSTRACE( logger, "TRI_INPUT : index = " << index << " u = " << u << " v = " << v )
                if( 0 == m_vertices_uv_mem.count( index ) )
                {
                    Vertex vertex;
                    m_meshe->GetVertex( index, vertex );
                    vertex.tu[0] = u;
                    vertex.tv[0] = v;
                    m_meshe->SetVertex( index, vertex );
                    _DSTRACE( logger, "index not memorized, updating corresponding vertex and storing it..." )
                    // memoriser les uv pour cet index
                    m_vertices_uv_mem[index].first = u;
                    m_vertices_uv_mem[index].second = v;
                }
                else
                {
                    if( u != m_vertices_uv_mem[index].first || 
                        v != m_vertices_uv_mem[index].second )
                    {
                        _DSTRACE( logger, "known index with different UV pair !!! -> new vertex" )
                        Vertex vertex;
                        m_meshe->GetVertex( index, vertex );
                        vertex.tu[0] = u;
                        vertex.tv[0] = v;
                        m_meshe->AddVertex( vertex );

                        index = m_meshe->GetVertexListSize() - 1;
                    }
                    else
                    {
                        _DSTRACE( logger, "known index, same UV pair, continue..." )
                    }
                }
        
                switch( m_trilinecount )
                {
                    case 0:
                        m_triangle.vertex1 = index;
                        break;
                    case 1:
                        m_triangle.vertex2 = index;
                        break;
                    case 2:
                        m_triangle.vertex3 = index;
                        break;
                }

                m_trilinecount++;
                if( 3 == m_trilinecount )
                {
                    // storing triangle...
                    _DSTRACE( logger, "new triangle : " << m_triangle.vertex1 
                                        << " " << m_triangle.vertex2 << " " << m_triangle.vertex3 )
                    m_meshe->AddTriangle( m_triangle );
                    m_tricount++;
                    if( m_tricount == m_numtri )
                    {
                        m_state = DONE;
                    }
                    else
                    {
                        m_state = SEARCH_TRI_BEGIN;
                    }
                }
            }
            break;
    }

    return true;
}

bool AC3DMesheImport::LoadFromFile( const dsstring& p_filepath, long p_index, Core::Meshe* p_meshe )
{
    _DSDEBUG( logger, "file = " << p_filepath.c_str() << " index = " << p_index )

    m_meshe = p_meshe;
    m_object_index = p_index;
    m_state = SEARCH_OBJECT_BEGIN;
    m_object_count = 0;
    return Parser::Run( p_filepath, " " );
}
