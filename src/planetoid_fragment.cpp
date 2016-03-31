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

#include "planetoid_fragment.h"
#include "planetoid_body.h"
#include "spherelod_drawing.h"
#include "renderer.h"
#include "plugin.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Planetoid;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::SphericalLOD;

Fragment::Fragment( DrawSpace::SphericalLOD::Config* p_config, DrawSpace::Dynamics::World* p_world, DrawSpace::SphericalLOD::Body* p_planetbody, 
                Collider* p_collider, dsreal p_planetray, bool p_collisions, Fragment::SubPassCreationHandler* p_handler ) :
m_world( p_world ),
m_config( p_config ),
m_planetbody( p_planetbody ), 
m_collider( p_collider ),
m_collision_state( false ),
m_planetray( p_planetray ),
m_hot( false ),
m_camera( NULL ),
m_inertbody( NULL ),
m_collisions( p_collisions ),
m_nb_collisionmeshebuild_done( 0 ),
m_current_patch( NULL ),
m_draw_collidinghm( false ),
m_handler( p_handler ),
m_current_collisions_hm( NULL )
{
    if( m_collisions )
    {
        m_patch_update_cb = _DRAWSPACE_NEW_( PatchUpdateCb, PatchUpdateCb( this, &Fragment::on_patchupdate ) );
        m_planetbody->RegisterPatchUpdateHandler( m_patch_update_cb );

        for( int i = 0; i < 6; i++ )
        {
            m_collisions_hms[i] = _DRAWSPACE_NEW_( Collisions, Collisions( this, p_config, i ) );
            m_collisions_hms[i]->Disable();        
        }

    }
    p_planetbody->Initialize();
}

Fragment::~Fragment( void )
{    
}

Fragment::SubPassCreationHandler* Fragment::GetSubPassCreationHandler( void )
{
    return m_handler;
}

void Fragment::on_patchupdate( DrawSpace::SphericalLOD::Patch* p_patch, int p_patch_lod )
{
    m_current_patch = p_patch;

    if( m_collisions )
    {
        std::vector<DrawSpace::SphericalLOD::Patch*> display_list;
        if( m_current_patch && p_patch_lod == 0 )
        {
            display_list.push_back( m_current_patch );

            if( p_patch->GetOrientation() == m_planetbody->GetCurrentFace() )
            {
                // ce patch appartient bien a la face "courante"

                m_draw_collidinghm = true;

                m_current_collisions_hm = m_collisions_hms[p_patch->GetOrientation()];
                m_current_collisions_hm->Enable();
               
                DrawSpace::SphericalLOD::FaceDrawingNode* node = static_cast<DrawSpace::SphericalLOD::FaceDrawingNode*>( m_current_collisions_hm->GetNode() );
                node->SetDisplayList( display_list );
            }
        }
    }
}

void Fragment::build_meshe( DrawSpace::Core::Meshe& p_patchmeshe, SphericalLOD::Patch* p_patch, DrawSpace::Core::Meshe& p_outmeshe, float* p_heightmap )
{
    for( int y = 0; y < PATCH_RESOLUTION; y++ )
    {
        for( int x = 0; x < PATCH_RESOLUTION; x++ )
        {
            Vertex v, vertex_out;
            int index = ( PATCH_RESOLUTION * y ) + x;

            int index_hm = ( PATCH_RESOLUTION * ( PATCH_RESOLUTION - 1 - y ) ) + x;

            p_patchmeshe.GetVertex( index, v );

            double alt = *( p_heightmap + index_hm );
            //alt *= m_config->m_amplitude;
      
            Vector v_out;

            p_patch->ProjectVertex( Vector( v.x, v.y, v.z, 1.0 ), v_out );
            v_out.Scale( m_planetray + alt );

            vertex_out.x = v_out[0];
            vertex_out.y = v_out[1];
            vertex_out.z = v_out[2];

            p_outmeshe.AddVertex( vertex_out );        
        }
    }

    for( long i = 0; i < p_patchmeshe.GetTrianglesListSize(); i++ )
    {
        Triangle t;
        p_patchmeshe.GetTriangles( i, t );
        p_outmeshe.AddTriangle( t );
    }
}

void Fragment::Compute( DrawSpace::Planetoid::Body* p_owner )
{
    if( m_hot )
    {
        Matrix camera_pos;
        bool inject_hotpoint = false;

        if( m_camera )
        {
            dsstring camera_scenename;
            
            DrawSpace::Core::SceneNode<CameraPoint>* camera_node = m_camera->GetOwner();
            camera_node->GetSceneName( camera_scenename );

            p_owner->GetCameraHotpoint( camera_scenename, camera_pos );
            
            inject_hotpoint = true;
        }
        else if( m_inertbody )
        {
            m_inertbody->GetLastLocalWorldTrans( camera_pos );
            inject_hotpoint = true;
        }

        if( inject_hotpoint )
        {
            DrawSpace::Utils::Vector hotpoint;

            hotpoint[0] = camera_pos( 3, 0 );
            hotpoint[1] = camera_pos( 3, 1 );
            hotpoint[2] = camera_pos( 3, 2 );

            m_planetbody->UpdateHotPoint( hotpoint );
            m_planetbody->Compute();
        }
    }
}

void Fragment::SetHotState( bool p_hotstate )
{
    m_hot = p_hotstate;
    m_planetbody->SetHotState( m_hot );
    m_nb_collisionmeshebuild_done = 0;
}

bool Fragment::GetHotState( void )
{
    return m_hot;
}

void Fragment::SetCamera( CameraPoint* p_camera )
{
    m_camera = p_camera;
}

void Fragment::SetInertBody( DrawSpace::Dynamics::InertBody* p_body )
{
    m_inertbody = p_body;
}


CameraPoint* Fragment::GetCamera( void )
{
    return m_camera;
}

InertBody* Fragment::GetInertBody( void )
{
    return m_inertbody;
}

void Fragment::RemoveColliderFromWorld( void )
{
    if( m_collision_state )
    {
        m_collider->RemoveFromWorld();
        m_collider->UnsetKinematic();
        m_collision_state = false;
    }
}

DrawSpace::SphericalLOD::Body* Fragment::GetPlanetBody( void )
{
    return m_planetbody;
}

void Fragment::GetCollisionMesheBuildStats( long& p_nb_collisionmeshebuild_done )
{
    p_nb_collisionmeshebuild_done = m_nb_collisionmeshebuild_done;
}

void Fragment::UpdateRelativeAlt( dsreal p_alt )
{
    m_planetbody->UpdateRelativeAlt( p_alt );
}

void Fragment::SubPassDone( DrawSpace::SphericalLOD::Collisions* p_collider )
{
    if( m_draw_collidinghm )
    {        
        m_current_collisions_hm->GetHMTexture()->CopyTextureContent();

        float* heightmap = (float*)m_current_collisions_hm->GetHMTextureContent();

        Meshe final_meshe;
        build_meshe( *( m_planetbody->GetPatcheMeshe() ), m_current_patch, final_meshe, heightmap );

        Dynamics::InertBody::Body::Parameters params;

        params.mass = 0.0;

        params.initial_attitude.Translation( 0.0, 0.0, 0.0 );

        params.shape_descr.shape = DrawSpace::Dynamics::Body::MESHE_SHAPE;
        params.shape_descr.meshe = final_meshe;

        RemoveColliderFromWorld();

        m_collider->SetKinematic( params );
        m_collider->AddToWorld( m_world );

        m_collision_state = true;
        m_nb_collisionmeshebuild_done++;

        m_draw_collidinghm = false;

        //m_collisions_hm->Disable();
        m_current_collisions_hm->Disable();
        m_current_collisions_hm = NULL;
    }
}

void Fragment::ResetPlanetBody( void )
{
    m_planetbody->Reset();
}
