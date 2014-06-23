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


#include "camerapoint.h"
#include "maths.h"
#include "rocket.h"
#include "fpsmovement.h"
#include "freemovement.h"
#include "linearmovement.h"
#include "circularmovement.h"


using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::Utils;

CameraPoint::CameraPoint( const dsstring& p_name, Body* p_body ) : TransformNode( p_name ), 
m_attached_body( p_body ), 
m_movement( NULL ),
m_locked_body( NULL ),
m_locked_node( NULL ),
m_longlatmovement( NULL ),
m_relative_orbiter( NULL ),
m_relative_altitud( 0.0 )
{
}


CameraPoint::~CameraPoint( void )
{

}


void CameraPoint::OnRegister( Scenegraph* p_scenegraph )
{
    std::map<dsstring, Core::TransformNode*>& camera_list = p_scenegraph->GetCamerasList();
    camera_list[m_scenename] = this; 
}


void CameraPoint::RegisterMovement( DrawSpace::Core::Movement* p_movement )
{
    m_movement = p_movement;
}

void CameraPoint::RegisterLongLatMovement( DrawSpace::Core::LongLatMovement* p_longlatmovement )
{
    m_longlatmovement = p_longlatmovement;
}

void CameraPoint::GetInfos( CameraPoint::Infos& p_infos )
{
    if( m_attached_body )
    {
        p_infos.attached_to_body = true;
        Orbiter* orbiter = dynamic_cast<Orbiter*>( m_attached_body );
        if( orbiter )
        {
            p_infos.attached_body_classname = "Orbiter";
            p_infos.attached_to_body = true;

        }
        else
        {
            Rocket* rocket = dynamic_cast<Rocket*>( m_attached_body );
            if( rocket )
            {
                p_infos.attached_body_classname = "Rocket";
                p_infos.attached_to_body = true;
            }
            else
            {
                InertBody* inert_body = dynamic_cast<InertBody*>( m_attached_body );
                if( inert_body )
                {
                    p_infos.attached_body_classname = "InertBody";
                    p_infos.attached_to_body = true;
                }
                else
                {
                    p_infos.attached_to_body = true;
                    p_infos.attached_body_classname = "<Unknown body class>";
                }
            }
        }
    }
    else
    {
        p_infos.attached_to_body = false;
        p_infos.attached_body_classname = "";
    }

    if( m_locked_body )
    {
        p_infos.locked_on_body = true;
        p_infos.locked_on_transformnode = false;
    }
    else if( m_locked_node )
    {
        p_infos.locked_on_body = false;
        p_infos.locked_on_transformnode = true;
    }
    else
    {
        p_infos.locked_on_body = false;
        p_infos.locked_on_transformnode = false;
    }

    if( m_movement )
    {
        p_infos.has_movement = true;

        if( dynamic_cast<FPSMovement*>( m_movement ) )
        {
            p_infos.movement_classname = "fps movement";
        }
        else if( dynamic_cast<FreeMovement*>( m_movement ) )
        {
            p_infos.movement_classname = "free movement";
        }
        else if( dynamic_cast<CircularMovement*>( m_movement ) )
        {
            p_infos.movement_classname = "circular movement";
        }
        else if( dynamic_cast<LinearMovement*>( m_movement ) )
        {
            p_infos.movement_classname = "linear movement";
        }
        else if( dynamic_cast<LongLatMovement*>( m_movement ) )
        {
            p_infos.movement_classname = "longlat movement";
        }
        else
        {
            p_infos.movement_classname = "<Unknown movement class>";
        }
    }
    else
    {
        p_infos.has_movement = false;
        p_infos.movement_classname = "";
    }

    if( m_longlatmovement )
    {
        p_infos.has_longlatmovement = true;
    }
    else
    {
        p_infos.has_longlatmovement = false;
    }

    p_infos.relative_orbiter = m_relative_orbiter;
    p_infos.altitud = m_relative_altitud;
}

void CameraPoint::ComputeFinalTransform( Utils::TimeManager& p_timemanager )
{
    if( m_movement )
    {
        m_movement->Compute( p_timemanager );
        m_movement->GetResult( m_localtransformation );
    }

    if( m_longlatmovement )
    {
        m_longlatmovement->Compute( p_timemanager );

        Matrix longlatmvtres;
        m_longlatmovement->GetResult( longlatmvtres );

        Matrix res = m_localtransformation * longlatmvtres;
        m_localtransformation = res;
    }

    if( m_locked_body || m_locked_node )
    {
        Matrix temp_global;

        Matrix body_transf;

        if( m_attached_body )
        {
            
            m_attached_body->GetLastWorldTransformation( body_transf );
            temp_global = m_localtransformation * body_transf;
        }
        else
        {
            temp_global = m_localtransformation;
        }

        if( m_locked_body )
        {
            m_locked_body->GetLastWorldTransformation( body_transf );
        }
        else if( m_locked_node )
        {
            m_locked_node->GetSceneWorld( body_transf );
        }

        Matrix camera_transf = temp_global;
        camera_transf.Inverse();

        Matrix res = body_transf * camera_transf;

        // point (0,0,0) local au body, exprim� dans le repere de la camera

        Vector body_center( 0.0, 0.0, 0.0, 1.0 );
        Vector body_center_2;

        res.Transform( &body_center, &body_center_2 );

        m_locked_body_center = body_center_2;


        body_center_2.Normalize();

        dsreal theta = atan2( body_center_2[0], body_center_2[2] );        
        theta = 3.1415927 + theta;
 
        Matrix roty;
        roty.Rotation( Vector( 0.0, 1.0, 0.0, 1.0 ), theta );

        Vector theta_dir( body_center_2[0], 0.0, body_center_2[2], 1.0 );

        dsreal phi = atan2( body_center_2[1], theta_dir.Length() );
        
        Matrix rotx;
        rotx.Rotation( Vector( 1.0, 0.0, 0.0, 1.0 ), phi );

        Matrix final_lock;
        final_lock = rotx * roty;

        Matrix final_res = final_lock * m_localtransformation;
        m_localtransformation = final_res;        
    }

    if( m_attached_body )
    {
        Matrix body_trans;            
        m_attached_body->GetLastWorldTransformation( body_trans );
        m_globaltransformation = m_localtransformation * body_trans;
    }
    else
    {
        m_globaltransformation = m_localtransformation;
    }

}

void CameraPoint::LockOnBody( Body* p_locked_body )
{
    m_locked_body = p_locked_body;
}

void CameraPoint::LockOnTransformNode( TransformNode* p_locked_node )
{
    m_locked_node = p_locked_node;
}

void CameraPoint::GetLockedBodyCenter( Vector& p_vector )
{
    p_vector = m_locked_body_center;
}

void CameraPoint::GetLocalTransform( DrawSpace::Utils::Matrix& p_localtransf )
{
    p_localtransf = m_localtransformation;
}

Body* CameraPoint::GetAttachedBody( void )
{
    return m_attached_body;
}

void CameraPoint::SetRelativeOrbiter( DrawSpace::Dynamics::Orbiter* p_relative_orbiter )
{
    m_relative_orbiter = p_relative_orbiter;
}

void CameraPoint::SetRelativeAltitude( dsreal p_relative_altitud )
{
    m_relative_altitud = p_relative_altitud;
}
