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

#ifndef _TRANFORMNODE_H_
#define _TRANFORMNODE_H_

#include "drawspace_commons.h"
#include "matrix.h"
#include "vsphere.h"
#include "lod.h"
#include "timemanager.h"

namespace DrawSpace
{
class Scenegraph;
namespace Core
{
class TransformNode
{
protected:

    dsstring                        m_scenename;

    Utils::Matrix                   m_localtransformation;
    Utils::Matrix                   m_globaltransformation;
    
    std::vector<LodStep*>           m_lodsteps;

    
public:
    TransformNode( const dsstring& p_name );
    TransformNode( void );
    virtual ~TransformNode( void );

    virtual void        OnRegister( Scenegraph* p_scenegraph ) = 0;
    
    virtual void        ComputeFinalTransform( Utils::TimeManager& p_timemanager );
    virtual void        SetLocalTransform( const DrawSpace::Utils::Matrix& p_mat );
    virtual void        GetSceneName( dsstring& p_name );
    virtual void        SetSceneName( const dsstring& p_name );
    virtual void        GetSceneWorld( Utils::Matrix& p_mat );    
    virtual void        ComputeLod( void );
    virtual void        ClearLodStepsList( void );
    virtual void        AddLodStep( LodStep* p_step );
    
    friend class TransformQueue;
};
}
}
#endif