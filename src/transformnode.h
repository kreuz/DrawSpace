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
    

    TransformNode*                  m_parent;
    std::vector<TransformNode*>     m_children;
    bool                            m_request_camera_view;

public:
    TransformNode( const dsstring& p_name );
    virtual ~TransformNode( void );

    virtual void OnRegister( Scenegraph* p_scenegraph ) = 0;

    virtual void AddChild( TransformNode* p_node );
    virtual void ComputeFinalTransform( void );
    virtual void SetLocalTransform( const DrawSpace::Utils::Matrix& p_mat );
    virtual void GetName( dsstring& p_name );
    virtual void GetSceneWorld( Utils::Matrix& p_mat );
    

    friend class TransformQueue;
};
}
}
#endif