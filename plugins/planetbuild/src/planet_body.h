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

#ifndef _PLANET_BODY_H_
#define _PLANET_BODY_H_

#include <drawable.h>
#include <scenegraph.h>
#include <task.h>
#include <property.h>
#include <mutex.h>
#include "planet_face.h"

typedef enum
{
    QUADTREE_1,
    QUADTREE_2,
    NONE_QUADTREE,

} WorkingQuadtree;

class Body;

class FaceRenderingNode : public DrawSpace::Core::RenderingNode
{
    typedef DrawSpace::Core::CallBack2<FaceRenderingNode, void, int, Patch*>     PatchInstanciationCallback;
    typedef DrawSpace::Core::CallBack2<FaceRenderingNode, void, int, Patch*>     PatchDelCallback;
    typedef DrawSpace::Core::CallBack2<FaceRenderingNode, void, int, Patch*>     PatchSplitCallback;
    typedef DrawSpace::Core::CallBack2<FaceRenderingNode, void, int, Patch*>     PatchMergeCallback;
    
protected:

    DrawSpace::Interface::Renderer* m_renderer;
    //std::map<dsstring, Patch*>      m_patchesleafs;

    std::map<dsstring, Patch*>      m_patchesleafs_1;
    std::map<dsstring, Patch*>      m_patchesleafs_2;

    PatchInstanciationCallback*     m_patchinstanciationcallback;
    PatchDelCallback*               m_patchdelcallback;
    PatchSplitCallback*             m_patchsplitcallback;
    PatchMergeCallback*             m_patchmergecallback;
    
    //Face*                           m_face;

    Body*                           m_owner;
    Face*                           m_face_1;
    Face*                           m_face_2;

    WorkingQuadtree                 m_current_quadtree;
    DrawSpace::Utils::Mutex         m_current_quadtree_mutex;

    WorkingQuadtree                 m_last_current_quadtree;


    void                            on_patchinstanciation( int p_orientation, Patch* p_patch );
    void                            on_patchdel( int p_orientation, Patch* p_patch );
    void                            on_patchsplit( int p_orientation, Patch* p_patch );
    void                            on_patchmerge( int p_orientation, Patch* p_patch );

    void                            complete_patchname( dsstring& p_patchname );
    bool                            is_working_quadtree1( void );
    
public:
    FaceRenderingNode( Body* p_owner, Face* p_face_1, Face* p_face_2, DrawSpace::Interface::Renderer* p_renderer );
    virtual ~FaceRenderingNode( void );

    virtual void Draw( const DrawSpace::Utils::Matrix& p_world, DrawSpace::Utils::Matrix& p_view );    
    virtual Face::PatchInstanciationHandler* GetPatchInstanciationHandler( void );
    virtual Face::PatchDeletionHandler* GetPatchDelHandler( void );
    virtual Face::PatchSplitHandler* GetPatchSplitHandler( void );
    virtual Face::PatchMergeHandler* GetPatchMergeHandler( void );

    virtual void SetWorkingQuadtree( WorkingQuadtree p_working_quadtree );
    
};

class Body : public DrawSpace::Interface::Drawable
{	
protected:

	typedef struct
	{
        FaceRenderingNode*        nodes[6];

	} NodesSet;
       
    typedef DrawSpace::Core::CallBack<Body, void, DrawSpace::Core::RenderingNode*> RenderingNodeDrawCallback;
    typedef DrawSpace::Core::CallBack2<Body, void, int, Patch*>                    PatchInstanciationCallback;    

	std::map<dsstring, NodesSet>                                m_passesnodes;
	std::vector<RenderingNodeDrawCallback*>                     m_callbacks;
    DrawSpace::Scenegraph*                                      m_scenegraph;
    DrawSpace::Interface::Renderer*                             m_renderer;

    //Face*                                                       m_faces[6];

    Face*                                                       m_faces_1[6];
    Face*                                                       m_faces_2[6];


    //// properties
    DrawSpace::Core::TypedProperty<dsreal>                      m_diameter;
    DrawSpace::Core::TypedProperty<DrawSpace::Utils::Vector>    m_hotpoint;
    DrawSpace::Core::TypedProperty<DrawSpace::Utils::Vector>    m_relative_hotpoint;
    DrawSpace::Core::TypedProperty<dsreal>                      m_altitud;
    DrawSpace::Core::TypedProperty<bool>                        m_update_state;

    bool                                                        m_stop_thread;
    DrawSpace::Core::Task<Body>*                                m_update_task;
    
    WorkingQuadtree                                             m_working_quadtree;

    void                                        on_renderingnode_draw( DrawSpace::Core::RenderingNode* p_rendering_node );    

    void                                        start_update( void );
    void                                        stop_update( void );

public:

	Body( void );
	virtual ~Body( void );

    virtual void GetDescr( dsstring& p_descr );
    virtual void DumpMemoryAllocs( void );
    virtual void SetRenderer( DrawSpace::Interface::Renderer * p_renderer );
    virtual void OnRegister( DrawSpace::Scenegraph* p_scenegraph );
    virtual bool LoadAssets( void );
    virtual DrawSpace::Core::Meshe* GetMeshe( const dsstring& p_mesheid );
    virtual void RegisterPassSlot( const dsstring p_passname );
    virtual DrawSpace::Core::RenderingNode* GetNodeFromPass( const dsstring p_passname, const dsstring& p_nodeid );
    virtual void GetNodesIdsList( std::vector<dsstring>& p_ids );
    virtual void ComputeSpecifics( void );
    virtual void GetPropertiesList( std::vector<dsstring>& p_props );
    virtual DrawSpace::Core::Property* GetProperty( const dsstring& p_name );
    virtual void SetProperty( const dsstring& p_name, DrawSpace::Core::Property* p_prop );

    virtual WorkingQuadtree GetWorkingQuadtree( void );


    virtual void Run( void );
   
};
#endif