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

#include <wx/wx.h>
#include "adapters.h"

using namespace DrawSpace;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

wxWidgetAdapter* wxWidgetAdapter::m_instance = NULL;

wxWidgetAdapter::wxWidgetAdapter( void )
{
    m_applypassshadervalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applypassshadervalues );
    m_applylinearmvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applylinearmvtvalues );
    m_applycircularmvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applycircularmvtvalues );
    m_applyfpsmvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyfpsmvtvalues );
    m_applyfreemvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyfreemvtvalues );
    m_applyheadmvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyheadmvtvalues );
    m_applyspectatormvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyspectatormvtvalues );
    m_applylonglatmvtvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applylonglatmvtvalues );
    m_applycameravalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applycameravalues );
    m_applycameraprops_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applycameraprops );

    m_applyspaceboxvalues_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyspaceboxvalues );
    m_applyspaceboxaddpassslot_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applyspaceboxaddpassslot );

    m_applymatrixstackvalue_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applymatrixstackvalues );
    m_applymatrixstackaddmatrix_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applymatrixstackaddmatrix );
    m_applymatrixstackclearall_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applymatrixstackclearall );

    m_applytransfosourcemodification_callback = new CallBack<wxWidgetAdapter, void, BasicSceneObjectPropertiesDialog*>( this, &wxWidgetAdapter::on_applytransfosourcemodification );
}

wxWidgetAdapter::~wxWidgetAdapter( void )
{
    delete m_applypassshadervalues_callback;
    delete m_applylinearmvtvalues_callback;
    delete m_applycircularmvtvalues_callback;
    delete m_applyfpsmvtvalues_callback;
    delete m_applyfreemvtvalues_callback;
    delete m_applyheadmvtvalues_callback;
    delete m_applyspectatormvtvalues_callback;
    delete m_applylonglatmvtvalues_callback;
    delete m_applycameravalues_callback;
    delete m_applycameraprops_callback;
    delete m_applyspaceboxvalues_callback;
    delete m_applyspaceboxaddpassslot_callback;
    delete m_applymatrixstackvalue_callback;
    delete m_applymatrixstackaddmatrix_callback;
    delete m_applymatrixstackclearall_callback;
    delete m_applytransfosourcemodification_callback;
}

void wxWidgetAdapter::AdaptAssetsList( wxListCtrl* p_listctrl )
{
    // columns creations   
    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Name" );
    col0.SetWidth( 150 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Type" );
    col1.SetWidth( 150 );
    p_listctrl->InsertColumn( 1, col1 );

    ///

    std::map<dsstring, Asset*> assets_list;
    AssetsBase::GetInstance()->GetAssetsList( assets_list );

    long id = 0;

    for( std::map<dsstring, Asset*>::iterator it = assets_list.begin(); it != assets_list.end(); ++it, id++ )
    {

        wxListItem item;
        item.SetId( id );

        dsstring assetname;
        it->second->GetName( assetname );

        item.SetText( assetname.c_str() );

        p_listctrl->InsertItem( item );

        dsstring type_name;

        if( dynamic_cast<Texture*>( it->second ) )
        {
            type_name = TEXTURE_TEXT_KEYWORD;
        }
        else if( dynamic_cast<Font*>( it->second ) )
        {
            type_name = FONT_TEXT_KEYWORD;
        }
        else if( dynamic_cast<Meshe*>( it->second ) )
        {
            type_name = MESHE_TEXT_KEYWORD;
        }
        else if( dynamic_cast<Shader*>( it->second ) )
        {
            type_name = SHADER_TEXT_KEYWORD;
        }
        else
        {
            type_name = "???";
        }
        p_listctrl->SetItem( id, 1, type_name.c_str() );

        p_listctrl->SetItemData( id, (long)it->second );            
    }   
}

void wxWidgetAdapter::AdaptConfigsList( wxListCtrl* p_listctrl )
{
    // columns creations   
    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Name" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Type" );
    col1.SetWidth( 100 );
    p_listctrl->InsertColumn( 1, col1 );

    ///

    std::map<dsstring, DrawSpace::Core::Configurable*> instances_list;

    ConfigsBase::GetInstance()->GetConfigsInstancesList( instances_list );


    long id = 0;
    for( std::map<dsstring, DrawSpace::Core::Configurable*>::iterator it = instances_list.begin(); it != instances_list.end(); ++it, id++ )
    {
        wxListItem item;
        item.SetId( id );

        dsstring configname = it->first;
        item.SetText( configname.c_str() );
        p_listctrl->InsertItem( item );


        dsstring type_name;

        if( dynamic_cast<Fx*>( it->second ) )
        {
            type_name = FX_TEXT_KEYWORD;
        }
        else if( dynamic_cast<IntermediatePass*>( it->second ) )
        {
            type_name = INTERMEDIATEPASS_TEXT_KEYWORD;
        }
        else if( dynamic_cast<FinalPass*>( it->second ) )
        {
            type_name = FINALPASS_TEXT_KEYWORD;
        }
        else
        {
            type_name = "???";
        }

        p_listctrl->SetItem( id, 1, type_name.c_str() );
        p_listctrl->SetItemData( id, (long)it->second );       
    }
}

void wxWidgetAdapter::AdaptPassesList( wxListCtrl* p_listctrl )
{
    // columns creations   
    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Pass Name" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Pass Type" );
    col1.SetWidth( 100 );
    p_listctrl->InsertColumn( 1, col1 );

    ///

    std::map<dsstring, DrawSpace::Core::Configurable*> instances_list;

    ConfigsBase::GetInstance()->GetConfigsInstancesList( instances_list );


    long id = 0;
    for( std::map<dsstring, DrawSpace::Core::Configurable*>::iterator it = instances_list.begin(); it != instances_list.end(); ++it )
    {
        dsstring configname = it->first;                
        bool insert = true;

        dsstring type_name;

        if( dynamic_cast<IntermediatePass*>( it->second ) )
        {
            type_name = INTERMEDIATEPASS_TEXT_KEYWORD;
        }
        else if( dynamic_cast<FinalPass*>( it->second ) )
        {
            type_name = FINALPASS_TEXT_KEYWORD;
        }
        else
        {
            insert = false;
        }

        if( insert )
        {

            wxListItem item;
            item.SetId( id );

            item.SetText( configname.c_str() );
            p_listctrl->InsertItem( item );
            p_listctrl->SetItem( id, 1, type_name.c_str() );
            p_listctrl->SetItemData( id, (long)it->second );

            id++;
        }                
    }
}

void wxWidgetAdapter::AdaptPassesShaderParamsList( DrawSpace::Pass* p_pass, wxListCtrl* p_listctrl )
{
    p_listctrl->ClearAll();

    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Parameter Name" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Shader index" );
    col1.SetWidth( 100 );
    p_listctrl->InsertColumn( 1, col1 );

    wxListItem col2;
    col2.SetId( 2 );
    col2.SetText( "Register" );
    col2.SetWidth( 100 );
    p_listctrl->InsertColumn( 2, col2 );

    wxListItem col3;
    col3.SetId( 3 );
    col3.SetText( "Value" );
    col3.SetWidth( 150 );
    p_listctrl->InsertColumn( 3, col3 );

    ///

    Configurable::PropertiesMap props;
    p_pass->GetPropertiesMap( props );

    
    bool viewportquad = props["viewportquad"].GetPropValue<bool>();

    if( viewportquad )
    {
        long id = 0;
        std::map<dsstring, RenderingNode::ShadersParams> viewportquad_shaderparams = props["viewportquad_shaderparams"].GetPropValue<std::map<dsstring, RenderingNode::ShadersParams>>();
        for( std::map<dsstring, RenderingNode::ShadersParams>::iterator it = viewportquad_shaderparams.begin(); it != viewportquad_shaderparams.end(); ++it, id++ )
        {
            wxListItem item;
            item.SetId( id );
            item.SetText( it->first.c_str() );
            p_listctrl->InsertItem( item );

            char shader_index[32];
            sprintf( shader_index, "%d", it->second.shader_index );

            char param_register[32];
            sprintf( param_register, "%d", it->second.param_register );

            char param_values[64];
            sprintf( param_values, "%.2f %.2f %.2f %.2f", it->second.param_values[0], it->second.param_values[1], it->second.param_values[2], it->second.param_values[3] );

            p_listctrl->SetItem( id, 1, shader_index );
            p_listctrl->SetItem( id, 2, param_register );
            p_listctrl->SetItem( id, 3, param_values );

            p_listctrl->SetItemData( id, (long)p_pass );            
        }
    }
}


void wxWidgetAdapter::AdaptMvtsList( std::map<dsstring, DrawSpace::Core::Movement*>* p_map, wxListCtrl* p_listctrl )
{
    p_listctrl->ClearAll();

    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Movement alias" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Movement type" );
    col1.SetWidth( 100 );
    p_listctrl->InsertColumn( 1, col1 );

    long id = 0;
    for( std::map<dsstring, Movement*>::iterator it = p_map->begin(); it != p_map->end(); ++it, id++ )
    {
        Movement* mvt = it->second;

        dsstring alias = it->first;

        wxListItem item;
        item.SetId( id );
        item.SetText( alias.c_str() );
        p_listctrl->InsertItem( item );


        dsstring type_name;

        if( dynamic_cast<LinearMovement*>( mvt ) )
        {
            type_name = "Linear";
        }
        else if( dynamic_cast<CircularMovement*>( mvt ) )
        {
            type_name = "Circular";
        }
        else if( dynamic_cast<FPSMovement*>( mvt ) )
        {
            type_name = "FPS";
        }
        else if( dynamic_cast<FreeMovement*>( mvt ) )
        {
            type_name = "Free";
        }
        else if( dynamic_cast<HeadMovement*>( mvt ) )
        {
            type_name = "Head";
        }
        else if( dynamic_cast<SpectatorMovement*>( mvt ) )
        {
            type_name = "Spectator";
        }
        else if( dynamic_cast<LongLatMovement*>( mvt ) )
        {
            type_name = "Longlat";
        }
        p_listctrl->SetItem( id, 1, type_name.c_str() );

        p_listctrl->SetItemData( id, (long)mvt );
    }
}

void wxWidgetAdapter::AdaptCamerasList( DrawSpace::Scenegraph* p_scenegraph, wxListCtrl* p_listctrl )
{
    p_listctrl->ClearAll();

    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Camera alias" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "Attached body" );
    col1.SetWidth( 150 );
    p_listctrl->InsertColumn( 1, col1 );

    wxListItem col2;
    col2.SetId( 2 );
    col2.SetText( "Locked on" );
    col2.SetWidth( 100 );
    p_listctrl->InsertColumn( 2, col2 );

    wxListItem col3;
    col3.SetId( 3 );
    col3.SetText( "Movement" );
    col3.SetWidth( 100 );
    p_listctrl->InsertColumn( 3, col3 );

    wxListItem col4;
    col4.SetId( 4 );
    col4.SetText( "Longlat movement" );
    col4.SetWidth( 150 );
    p_listctrl->InsertColumn( 4, col4 );

    wxListItem col5;
    col5.SetId( 5 );
    col5.SetText( "Relative orbiter" );
    col5.SetWidth( 150 );
    p_listctrl->InsertColumn( 5, col5 );

    wxListItem col6;
    col6.SetId( 6 );
    col6.SetText( "Altitud" );
    col6.SetWidth( 70 );
    p_listctrl->InsertColumn( 6, col6 );

    ///////////////////////////////////////

    std::map<dsstring, Core::TransformNode*> cameras_list = p_scenegraph->GetCamerasList();

    long id = 0;
    for( std::map<dsstring, Core::TransformNode*>::iterator it = cameras_list.begin(); it != cameras_list.end(); ++it, id++ )
    {
        CameraPoint* camera = static_cast<CameraPoint*>( it->second );

        dsstring alias = it->first;
        wxListItem item;
        item.SetId( id );
        item.SetText( alias.c_str() );
        p_listctrl->InsertItem( item );

        CameraPoint::Infos infos;
        camera->GetInfos( infos );


        dsstring attached_body_name;
        if( infos.attached_to_body )
        {
            attached_body_name = infos.attached_body_alias;
        }
        else
        {
            attached_body_name = "...";
        }
        p_listctrl->SetItem( id, 1, attached_body_name.c_str() );

        dsstring locking_object_name;
        if( infos.locked_on_body || infos.locked_on_transformnode )
        {
            locking_object_name = infos.locked_object_alias;
        }
        else
        {
            locking_object_name = "...";
        }
        p_listctrl->SetItem( id, 2, locking_object_name.c_str() );

        dsstring movement_name;
        if( infos.has_movement )
        {
            movement_name = infos.movement_alias;
        }
        else
        {
            movement_name = "...";
        }
        p_listctrl->SetItem( id, 3, movement_name.c_str() );

        
        dsstring llmovement_name;
        if( infos.has_longlatmovement )
        {
            llmovement_name = infos.longlatmovement_alias;
        }
        else
        {
            llmovement_name = "...";
        }

        p_listctrl->SetItem( id, 4, llmovement_name.c_str() );

        dsstring relative_orbiter;
        char altitud[32];
        if( infos.relative_orbiter )
        {
            relative_orbiter = "true";
            sprintf( altitud, "%d", infos.altitud );
        }
        else
        {
            relative_orbiter = "false";
            sprintf( altitud, "..." );
        }
        p_listctrl->SetItem( id, 5, relative_orbiter.c_str() );
        p_listctrl->SetItem( id, 6, altitud );

        p_listctrl->SetItemData( id, (long)camera );

    }
}

void wxWidgetAdapter::AdaptScenegraphList( DrawSpace::Scenegraph* p_scenegraph, wxListCtrl* p_listctrl )
{
    p_listctrl->ClearAll();

    wxListItem col0;
    col0.SetId( 0 );
    col0.SetText( "Scene name" );
    col0.SetWidth( 110 );
    p_listctrl->InsertColumn( 0, col0 );

    wxListItem col1;
    col1.SetId( 1 );
    col1.SetText( "type" );
    col1.SetWidth( 150 );
    p_listctrl->InsertColumn( 1, col1 );

    /////////////////////////////////////////////

    std::map<dsstring, TransformNode*> nodes_list = p_scenegraph->GetNodesList();

    long id = 0;
    for( std::map<dsstring, TransformNode*>::iterator it = nodes_list.begin(); it != nodes_list.end(); ++it, id++ )
    {
        dsstring scenename = it->first;
        wxListItem item;
        item.SetId( id );
        item.SetText( scenename.c_str() );
        p_listctrl->InsertItem( item );


        TransformNode* node = it->second;

        dsstring type_name;

        if( dynamic_cast<CameraPoint*>( node ) )
        {
            type_name = "Camera";
        }
        else if( dynamic_cast<Spacebox*>( node ) )
        {
            type_name = "Spacebox";
        }
        else
        {
            type_name = "???";    
        }

        p_listctrl->SetItem( id, 1, type_name.c_str() );

        p_listctrl->SetItemData( id, (long)it->second );
    }    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void wxWidgetAdapter::AdaptTextureProps( DrawSpace::Core::Texture* p_texture, wxPropertyGrid* p_propertygrid )
{
    Asset::PropertiesMap props;
    p_texture->GetPropertiesMap( props );

    dsstring assetname = props["assetname"].GetPropValue<dsstring>();
    dsstring filepath = props["filepath"].GetPropValue<dsstring>();
    bool rendertarget = props["rendertarget"].GetPropValue<bool>();
    unsigned long rendetarget_w = props["rendertarget_size"].GetPropValue<unsigned long>( "width" );
    unsigned long rendetarget_h = props["rendertarget_size"].GetPropValue<unsigned long>( "height" );


    p_propertygrid->Append( new wxStringProperty( "assetname", wxPG_LABEL, assetname.c_str() ) );
    //p_propertygrid->DisableProperty( "assetname" );

    p_propertygrid->Append( new wxStringProperty( "filepath", wxPG_LABEL, filepath.c_str() ) );
    //p_propertygrid->DisableProperty( "filepath" );

    p_propertygrid->Append( new wxBoolProperty( "rendertarget", wxPG_LABEL, rendertarget ) );
    //p_propertygrid->DisableProperty( "rendertarget" );

    if( rendertarget )
    {
        p_propertygrid->Append( new wxIntProperty( "rendertarget_size/width", wxPG_LABEL, rendetarget_w ) );
        //p_propertygrid->DisableProperty( "rendertarget_size/width" );
        p_propertygrid->Append( new wxIntProperty( "rendertarget_size/height", wxPG_LABEL, rendetarget_h ) );
        //p_propertygrid->DisableProperty( "rendertarget_size/height" );
    }
    
}

void wxWidgetAdapter::AdaptShaderProps( DrawSpace::Core::Shader* p_shader, wxPropertyGrid* p_propertygrid )
{
    Asset::PropertiesMap props;
    p_shader->GetPropertiesMap( props );

    dsstring assetname = props["assetname"].GetPropValue<dsstring>();
    dsstring filepath = props["filepath"].GetPropValue<dsstring>();
    bool compiled = props["compiled"].GetPropValue<bool>();

    p_propertygrid->Append( new wxStringProperty( "assetname", wxPG_LABEL, assetname.c_str() ) );
    p_propertygrid->Append( new wxStringProperty( "filepath", wxPG_LABEL, filepath.c_str() ) );
    p_propertygrid->Append( new wxBoolProperty( "compiled", wxPG_LABEL, compiled ) );
}

void wxWidgetAdapter::AdaptFontProps( DrawSpace::Core::Font* p_font, wxPropertyGrid* p_propertygrid )
{
    Asset::PropertiesMap props;
    p_font->GetPropertiesMap( props );

    dsstring assetname = props["assetname"].GetPropValue<dsstring>();
    dsstring plugin = props["plugin"].GetPropValue<dsstring>();
    dsstring texturefilepath = props["filespath"].GetPropValue<dsstring>( "texturefilepath" );
    dsstring metricsfilepath = props["filespath"].GetPropValue<dsstring>( "metricsfilepath" );

    p_propertygrid->Append( new wxStringProperty( "assetname", wxPG_LABEL, assetname.c_str() ) );
    p_propertygrid->Append( new wxStringProperty( "texturefilepath", wxPG_LABEL, texturefilepath.c_str() ) );
    p_propertygrid->Append( new wxStringProperty( "metricsfilepath", wxPG_LABEL, metricsfilepath.c_str() ) );
    p_propertygrid->Append( new wxStringProperty( "plugin", wxPG_LABEL, plugin.c_str() ) );   
}

void wxWidgetAdapter::AdaptMesheProps( DrawSpace::Core::Meshe* p_meshe, wxPropertyGrid* p_propertygrid )
{
    Asset::PropertiesMap props;
    p_meshe->GetPropertiesMap( props );

    dsstring assetname = props["assetname"].GetPropValue<dsstring>();
    dsstring filepath = props["filepath"].GetPropValue<dsstring>();
    long index = props["index"].GetPropValue<long>();
    dsstring plugin = props["plugin"].GetPropValue<dsstring>();

    p_propertygrid->Append( new wxStringProperty( "assetname", wxPG_LABEL, assetname.c_str() ) );
    p_propertygrid->Append( new wxStringProperty( "filepath", wxPG_LABEL, filepath.c_str() ) );
    p_propertygrid->Append( new wxIntProperty( "index", wxPG_LABEL, index ) );
    p_propertygrid->Append( new wxStringProperty( "plugin", wxPG_LABEL, plugin.c_str() ) );      
}

void wxWidgetAdapter::AdaptFxProps( DrawSpace::Core::Fx* p_fx, wxPropertyGrid* p_propertygrid )
{
    Configurable::PropertiesMap props;
    p_fx->GetPropertiesMap( props );

    std::vector<dsstring> shaders_list = props["shaders"].GetPropValue<std::vector<dsstring>>();

    if( shaders_list.size() > 0 )
    {
        wxPGProperty* shaderslist_prop = p_propertygrid->Append( new wxStringProperty( "shaders", wxPG_LABEL, "<composed>" ) );

        for( size_t i = 0; i < shaders_list.size(); i++ )
        {
            char shader_index[32];
            sprintf( shader_index, "%d", i );

            p_propertygrid->AppendIn( shaderslist_prop, new wxStringProperty( shader_index, wxPG_LABEL, shaders_list[i].c_str() ) );
        }
    }

    std::vector<RenderState> rsin_list = props["renderstates_in"].GetPropValue<std::vector<RenderState>>();

    if( rsin_list.size() > 0 )
    {
        wxPGProperty* rsinlist_prop = p_propertygrid->Append( new wxStringProperty( "renderstates_in", wxPG_LABEL, "<composed>" ) );
        for( size_t i = 0; i < rsin_list.size(); i++ )
        {
            dsstring renderstate_op;
            dsstring renderstate_arg;

            RenderState rs = rsin_list[i];
            
            char rsin_index[32];
            
            sprintf( rsin_index, "%d", i );

            wxPGProperty* rsinlistindex_prop = p_propertygrid->AppendIn( rsinlist_prop, new wxStringProperty( rsin_index, wxPG_LABEL, "<composed>" ) );

            
            rs.GetOperationToString( renderstate_op );
            rs.GetArg( renderstate_arg );

            p_propertygrid->AppendIn( rsinlistindex_prop, new wxStringProperty( "Operation", wxPG_LABEL, renderstate_op.c_str() ) );
            p_propertygrid->AppendIn( rsinlistindex_prop, new wxStringProperty( "Argument", wxPG_LABEL, renderstate_arg.c_str() ) );
        }
    }

    std::vector<RenderState> rsout_list = props["renderstates_out"].GetPropValue<std::vector<RenderState>>();
    if( rsout_list.size() > 0 )
    {
        wxPGProperty* rsoutlist_prop = p_propertygrid->Append( new wxStringProperty( "renderstates_out", wxPG_LABEL, "<composed>" ) );
        for( size_t i = 0; i < rsout_list.size(); i++ )
        {
            dsstring renderstate_op;
            dsstring renderstate_arg;

            RenderState rs = rsout_list[i];
            
            char rsout_index[32];

            sprintf( rsout_index, "%d", i );

            wxPGProperty* rsoutlistindex_prop = p_propertygrid->AppendIn( rsoutlist_prop, new wxStringProperty( rsout_index, wxPG_LABEL, "<composed>" ) );
            
            rs.GetOperationToString( renderstate_op );
            rs.GetArg( renderstate_arg );

            p_propertygrid->AppendIn( rsoutlistindex_prop, new wxStringProperty( "Operation", wxPG_LABEL, renderstate_op.c_str() ) );
            p_propertygrid->AppendIn( rsoutlistindex_prop, new wxStringProperty( "Argument", wxPG_LABEL, renderstate_arg.c_str() ) );

        }
    }

    p_propertygrid->ResetColumnSizes();
    p_propertygrid->CollapseAll();
}

void wxWidgetAdapter::AdaptPassProps( bool p_intermediate_pass, DrawSpace::Pass* p_pass, wxPropertyGrid* p_propertygrid )
{
    Configurable::PropertiesMap props;
    p_pass->GetPropertiesMap( props );

    dsstring passname = props["passname"].GetPropValue<dsstring>();
    p_propertygrid->Append( new wxStringProperty( "passname", wxPG_LABEL, passname.c_str() ) );

    if( p_intermediate_pass )
    {
        bool targetdimsfromrenderer = props["targetdimsfromrenderer"].GetPropValue<bool>();
        long targetdims_width = props["targetdims"].GetPropValue<long>( "width" );
        long targetdims_height = props["targetdims"].GetPropValue<long>( "height" );

        p_propertygrid->Append( new wxBoolProperty( "targetdimsfromrenderer", wxPG_LABEL, targetdimsfromrenderer ) );

        if( !targetdimsfromrenderer )
        {
            wxPGProperty* targetdims_prop = p_propertygrid->Append( new wxStringProperty( "targetdims", wxPG_LABEL, "<composed>" ) );
            p_propertygrid->AppendIn( targetdims_prop, new wxIntProperty( "width", wxPG_LABEL, targetdims_width ) );
            p_propertygrid->AppendIn( targetdims_prop, new wxIntProperty( "height", wxPG_LABEL, targetdims_height ) );
        }
    }

    p_propertygrid->Append( new wxBoolProperty( "enabledepthclear", wxPG_LABEL, props["enabledepthclear"].GetPropValue<bool>() ) );
    p_propertygrid->Append( new wxBoolProperty( "enabletargetclear", wxPG_LABEL, props["enabletargetclear"].GetPropValue<bool>() ) );

    if( props["enabletargetclear"].GetPropValue<bool>() )
    {
        wxPGProperty* targetclearcolor_prop = p_propertygrid->Append( new wxStringProperty( "targetclearcolor", wxPG_LABEL, "<composed>" ) );
        p_propertygrid->AppendIn( targetclearcolor_prop, new wxIntProperty( "r", wxPG_LABEL, props["targetclearcolor"].GetPropValue<unsigned char>( "r" ) ) );
        p_propertygrid->AppendIn( targetclearcolor_prop, new wxIntProperty( "g", wxPG_LABEL, props["targetclearcolor"].GetPropValue<unsigned char>( "g" ) ) );
        p_propertygrid->AppendIn( targetclearcolor_prop, new wxIntProperty( "b", wxPG_LABEL, props["targetclearcolor"].GetPropValue<unsigned char>( "b" ) ) );
    }

    bool viewportquad = props["viewportquad"].GetPropValue<bool>();

    p_propertygrid->Append( new wxBoolProperty( "viewportquad", wxPG_LABEL, viewportquad ) );

    if( viewportquad )
    {
        p_propertygrid->Append( new wxStringProperty( "viewportquad_fx", wxPG_LABEL, props["viewportquad_fx"].GetPropValue<dsstring>() ) );
        
        std::vector<std::pair<long, Pass::TextureSourceName>> viewportquad_textures;
        viewportquad_textures = props["viewportquad_textures"].GetPropValue<std::vector<std::pair<long, Pass::TextureSourceName>>>();

        if( viewportquad_textures.size() > 0 )
        {
            wxPGProperty* vpqtextures_prop = p_propertygrid->Append( new wxStringProperty( "viewportquad_textures", wxPG_LABEL, "<composed>" ) );

            for( size_t i = 0; i < viewportquad_textures.size(); i++ )
            {
                /*
                Pass::TextureSourceName texture_source_name = viewportquad_textures[i].second;
                long stage = viewportquad_textures[i].first;

                char texture_entry[128];

                sprintf( texture_entry, "textures/%d, stage %d", i, stage );

                dsstring final;
                if( Pass::PASS_NAME == texture_source_name.source )
                {
                    final = "Pass : " + texture_source_name.name;
                }
                else
                {
                    final = "Texture : " + texture_source_name.name;
                }

                p_propertygrid->Append( new wxStringProperty( texture_entry, wxPG_LABEL, final.c_str() ) );
                */

                Pass::TextureSourceName texture_source_name = viewportquad_textures[i].second;
                long stage = viewportquad_textures[i].first;

                char stage_entry[32];
                sprintf( stage_entry, "stage %d", stage );
                wxPGProperty* vpqtexturestage_prop = p_propertygrid->AppendIn( vpqtextures_prop, new wxStringProperty( stage_entry, wxPG_LABEL, "<composed>" ) );

                if( Pass::PASS_NAME == texture_source_name.source )
                {
                    p_propertygrid->AppendIn( vpqtexturestage_prop, new wxStringProperty( "Source type", wxPG_LABEL, "Pass" ) );
                }
                else
                {
                    p_propertygrid->AppendIn( vpqtexturestage_prop, new wxStringProperty( "Source type", wxPG_LABEL, "Texture" ) );
                }

                p_propertygrid->AppendIn( vpqtexturestage_prop, new wxStringProperty( "Source name", wxPG_LABEL, texture_source_name.name.c_str() ) );
            }

        }

        std::map<dsstring, RenderingNode::ShadersParams> viewportquad_shaderparams = props["viewportquad_shaderparams"].GetPropValue<std::map<dsstring, RenderingNode::ShadersParams>>();

        if( viewportquad_shaderparams.size() )
        {
            wxPGProperty* vpqshaderparams_prop = p_propertygrid->Append( new wxStringProperty( "viewportquad_shaderparams", wxPG_LABEL, "<composed>" ) );

            for( std::map<dsstring, RenderingNode::ShadersParams>::iterator it = viewportquad_shaderparams.begin(); it != viewportquad_shaderparams.end(); ++ it )
            {
                /*
                char param_entry[128];
                sprintf( param_entry, "shaderparams/%s, shader %d register %d", it->first.c_str(), it->second.shader_index, it->second.param_register );

                char param_value[64];

                sprintf( param_value, "%.2f %.2f %.2f %.2f", it->second.param_values[0], it->second.param_values[1], it->second.param_values[2], it->second.param_values[3] );

                p_propertygrid->Append( new wxStringProperty( param_entry, wxPG_LABEL, param_value ) );
                */

                wxPGProperty* vpqshaderparamname_prop = p_propertygrid->AppendIn( vpqshaderparams_prop, new wxStringProperty( it->first.c_str(), wxPG_LABEL, "<composed>" ) );

                p_propertygrid->AppendIn( vpqshaderparamname_prop, new wxIntProperty( "shader index", wxPG_LABEL, it->second.shader_index ) );
                p_propertygrid->AppendIn( vpqshaderparamname_prop, new wxIntProperty( "register", wxPG_LABEL, it->second.param_register ) );

                wxPGProperty* vpqshaderparamvalue_prop = p_propertygrid->AppendIn( vpqshaderparamname_prop, new wxStringProperty( "value", wxPG_LABEL, "<composed>" ) );

                p_propertygrid->AppendIn( vpqshaderparamvalue_prop, new wxFloatProperty( "x", wxPG_LABEL, it->second.param_values[0] ) );
                p_propertygrid->AppendIn( vpqshaderparamvalue_prop, new wxFloatProperty( "y", wxPG_LABEL, it->second.param_values[1] ) );
                p_propertygrid->AppendIn( vpqshaderparamvalue_prop, new wxFloatProperty( "z", wxPG_LABEL, it->second.param_values[2] ) );
                p_propertygrid->AppendIn( vpqshaderparamvalue_prop, new wxFloatProperty( "w", wxPG_LABEL, it->second.param_values[3] ) );

            }
        }
    }

    p_propertygrid->ResetColumnSizes();
    p_propertygrid->CollapseAll();
}

void wxWidgetAdapter::AdaptCameraProps( DrawSpace::Dynamics::CameraPoint* p_camera, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    dsstring scene_name;
    p_camera->GetSceneName( scene_name );

    propertygrid->Append( new wxStringProperty( "scene_name", wxPG_LABEL, scene_name.c_str() ) );

    CameraPoint::Infos infos;
    p_camera->GetInfos( infos );

    dsstring attached_body_name;
    if( infos.attached_to_body )
    {
        attached_body_name = infos.attached_body_alias;
    }
    else
    {
        attached_body_name = "...";
    }
    propertygrid->Append( new wxStringProperty( "attached_body_name", wxPG_LABEL, attached_body_name.c_str() ) );

    dsstring locking_object_name;
    if( infos.locked_on_body || infos.locked_on_transformnode )
    {
        locking_object_name = infos.locked_object_alias;
    }
    else
    {
        locking_object_name = "...";
    }
    propertygrid->Append( new wxStringProperty( "locked_object_name", wxPG_LABEL, locking_object_name.c_str() ) );

    dsstring movement_name;
    if( infos.has_movement )
    {
        movement_name = infos.movement_alias;
    }
    else
    {
        movement_name = "...";
    }
    propertygrid->Append( new wxStringProperty( "movement_name", wxPG_LABEL, movement_name.c_str() ) );

    dsstring llmovement_name;
    if( infos.has_longlatmovement )
    {
        llmovement_name = infos.longlatmovement_alias;
    }
    else
    {
        llmovement_name = "...";
    }
    propertygrid->Append( new wxStringProperty( "longlat_movement_name", wxPG_LABEL, llmovement_name.c_str() ) );

    bool relative_orbiter;
    char altitud[32];
    if( infos.relative_orbiter )
    {
        relative_orbiter = true;
        sprintf( altitud, "%d", infos.altitud );
    }
    else
    {
        relative_orbiter = false;
        sprintf( altitud, "..." );
    }

    propertygrid->Append( new wxBoolProperty( "relative_orbiter", wxPG_LABEL, relative_orbiter ) );
    propertygrid->Append( new wxStringProperty( "altitud", wxPG_LABEL, altitud ) );

    dsreal znear = p_camera->GetZNear();
    propertygrid->Append( new wxFloatProperty( "znear", wxPG_LABEL, znear ) );

}

void wxWidgetAdapter::AdaptPassShaderValuesPropsModification( Pass* p_pass, char* p_param_id, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    Configurable::PropertiesMap props;
    p_pass->GetPropertiesMap( props );

    std::map<dsstring, RenderingNode::ShadersParams> viewportquad_shaderparams = props["viewportquad_shaderparams"].GetPropValue<std::map<dsstring, RenderingNode::ShadersParams>>();

    dsreal x = viewportquad_shaderparams[p_param_id].param_values[0];
    dsreal y = viewportquad_shaderparams[p_param_id].param_values[1];
    dsreal z = viewportquad_shaderparams[p_param_id].param_values[2];
    dsreal w = viewportquad_shaderparams[p_param_id].param_values[3];

    propertygrid->Append( new wxFloatProperty( "x", wxPG_LABEL, x ) );
    propertygrid->Append( new wxFloatProperty( "y", wxPG_LABEL, y ) );
    propertygrid->Append( new wxFloatProperty( "z", wxPG_LABEL, z ) );
    propertygrid->Append( new wxFloatProperty( "w", wxPG_LABEL, w ) );

    char* param_id = new char[strlen( p_param_id ) + 1];
    strcpy( param_id, p_param_id );

    p_dialog->RegisterApplyButtonHandler( m_applypassshadervalues_callback );
    p_dialog->SetData( "pass", p_pass );

    p_dialog->SetData( "param_id", param_id );
}

void wxWidgetAdapter::on_applypassshadervalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxAny value;
    dsreal x, y, z, w;
    
    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "x" ) );
    value = prop->GetValue();   
    value.GetAs<double>( &x );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "y" ) );
    value = prop->GetValue();   
    value.GetAs<double>( &y );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "z" ) );
    value = prop->GetValue();   
    value.GetAs<double>( &z );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "w" ) );
    value = prop->GetValue();   
    value.GetAs<double>( &w );

    Pass* pass = (Pass*)p_dialog->GetData( "pass" );
    char* param_id = (char*)p_dialog->GetData( "param_id" );

    // apply pass viewportquad shader values modifications
    pass->GetViewportQuad()->SetShaderRealVector( param_id, Vector( x, y, z, w ) );

    // update pass properties
    Configurable::PropertiesMap props;
    pass->GetPropertiesMap( props );

    std::map<dsstring, RenderingNode::ShadersParams> viewportquad_shaderparams = props["viewportquad_shaderparams"].GetPropValue<std::map<dsstring, RenderingNode::ShadersParams>>();

    viewportquad_shaderparams[param_id].param_values[0] = x;
    viewportquad_shaderparams[param_id].param_values[1] = y;
    viewportquad_shaderparams[param_id].param_values[2] = z;
    viewportquad_shaderparams[param_id].param_values[3] = w;

    props["viewportquad_shaderparams"].SetPropValue<std::map<dsstring, RenderingNode::ShadersParams>>( viewportquad_shaderparams );

    pass->SetPropertiesMap( props );


    // release string allocated in wxWidgetAdapter::AdaptPassShaderValuesPropsModification()
    delete[] param_id;


    // update mainframe list ctrl
    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );
    AdaptPassesShaderParamsList( pass, ctrl );

    p_dialog->Close();
}


void wxWidgetAdapter::AdaptCameraPropsModification( DrawSpace::Dynamics::CameraPoint* p_camera, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    dsreal znear = p_camera->GetZNear();
    propertygrid->Append( new wxFloatProperty( "znear", wxPG_LABEL, znear ) );

    p_dialog->RegisterApplyButtonHandler( m_applycameraprops_callback );
}

void wxWidgetAdapter::on_applycameraprops( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxAny value;
    dsreal znear;
    
    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "znear" ) );
    value = prop->GetValue();   
    value.GetAs<double>( &znear );

    CameraPoint* camera = (CameraPoint*)p_dialog->GetData( "camera" );

    camera->UpdateProjectionZNear( znear );

    p_dialog->Close();
}



void wxWidgetAdapter::AdaptLinearMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );


    wxPGProperty* initpos_prop = propertygrid->Append( new wxStringProperty( "Initial position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    wxPGProperty* direction_prop = propertygrid->Append( new wxStringProperty( "Direction", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "x", wxPG_LABEL, 1.0 ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );


    propertygrid->Append( new wxFloatProperty( "Theta", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Phi", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applylinearmvtvalues_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applylinearmvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal rval;
    Vector init_pos;
    Vector direction;
    dsreal theta, phi;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;

    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[2] = rval;

    init_pos[3] = 1.0;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Direction.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    direction[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Direction.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    direction[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Direction.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    direction[2] = rval;

    direction[3] = 1.0;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Theta" ) );
    value = prop->GetValue();
    value.GetAs<double>( &theta );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Phi" ) );
    value = prop->GetValue();
    value.GetAs<double>( &phi );

    LinearMovement* linear_mvt = new LinearMovement();
    linear_mvt->Init( init_pos, direction, theta, phi );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = linear_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();
}

void wxWidgetAdapter::AdaptCircularMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    wxPGProperty* centerpos_prop = propertygrid->Append( new wxStringProperty( "Center position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    wxPGProperty* deltacenter_prop = propertygrid->Append( new wxStringProperty( "Delta center", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "x", wxPG_LABEL, 10.0 ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    wxPGProperty* rotaxis_prop = propertygrid->Append( new wxStringProperty( "Rotation axis", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "y", wxPG_LABEL, 1.0 ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );


    propertygrid->Append( new wxFloatProperty( "Initial angle", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Theta", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Phi", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applycircularmvtvalues_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applycircularmvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal rval;
    Vector center_pos;
    Vector delta_center;
    Vector rot_axis;
    dsreal init_angle, theta, phi;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Center position.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    center_pos[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Center position.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    center_pos[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Center position.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    center_pos[2] = rval;

    center_pos[3] = 1.0;


    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Delta center.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    delta_center[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Delta center.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    delta_center[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Delta center.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    delta_center[2] = rval;

    delta_center[3] = 1.0;


    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Rotation axis.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    rot_axis[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Rotation axis.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    rot_axis[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Rotation axis.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    rot_axis[2] = rval;

    rot_axis[3] = 1.0;


    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial angle" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_angle );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Theta" ) );
    value = prop->GetValue();
    value.GetAs<double>( &theta );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Phi" ) );
    value = prop->GetValue();
    value.GetAs<double>( &phi );

    CircularMovement* circular_mvt = new CircularMovement();
    circular_mvt->Init( center_pos, delta_center, rot_axis, init_angle, theta, phi );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = circular_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptFPSMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    wxPGProperty* initpos_prop = propertygrid->Append( new wxStringProperty( "Initial position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );


    propertygrid->Append( new wxFloatProperty( "Initial yaw", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Initial pitch", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applyfpsmvtvalues_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applyfpsmvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal rval;
    Vector init_pos;
    dsreal init_yaw, init_pitch;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[2] = rval;

    init_pos[3] = 1.0;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial yaw" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_yaw );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial pitch" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_pitch );


    FPSMovement* fps_mvt = new FPSMovement();
    fps_mvt->Init( init_pos, init_yaw, init_pitch );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = fps_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptFreeMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    wxPGProperty* initpos_prop = propertygrid->Append( new wxStringProperty( "Initial position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applyfreemvtvalues_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applyfreemvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal rval;
    Vector init_pos;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial position.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    init_pos[2] = rval;

    init_pos[3] = 1.0;

    FreeMovement* free_mvt = new FreeMovement();
    free_mvt->Init( init_pos );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = free_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptHeadMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    propertygrid->Append( new wxFloatProperty( "Scale factor", wxPG_LABEL, 1.0 ) );
    propertygrid->Append( new wxFloatProperty( "Ref force", wxPG_LABEL, 10.0 ) );

    wxPGProperty* headpos_prop = propertygrid->Append( new wxStringProperty( "Head position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applyheadmvtvalues_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applyheadmvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal rval;
    Vector head_pos;
    dsreal scale_factor;
    dsreal ref_force;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Head position.x" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    head_pos[0] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Head position.y" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    head_pos[1] = rval;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Head position.z" ) );
    value = prop->GetValue();
    value.GetAs<double>( &rval );
    head_pos[2] = rval;

    head_pos[3] = 1.0;

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Scale factor" ) );
    value = prop->GetValue();
    value.GetAs<double>( &scale_factor );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Ref force" ) );
    value = prop->GetValue();
    value.GetAs<double>( &ref_force );


    HeadMovement* head_mvt = new HeadMovement();
    head_mvt->Init( scale_factor, ref_force, head_pos );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = head_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptSpectatorMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    propertygrid->Append( new wxFloatProperty( "Scale pos", wxPG_LABEL, 20.0 ) );
    propertygrid->Append( new wxIntProperty( "Period", wxPG_LABEL, 10 ) );
    propertygrid->Append( new wxBoolProperty( "Attached to Orbiter", wxPG_LABEL, false ) );

    p_dialog->RegisterApplyButtonHandler( m_applyspectatormvtvalues_callback );
}

void wxWidgetAdapter::on_applyspectatormvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;    
    wxStringProperty* prop2;
    wxIntProperty* prop3;
    wxBoolProperty* prop4;
    wxAny value;

    dsreal  scale_pos;
    int     period;
    bool    orbiter_link;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Scale pos" ) );
    value = prop->GetValue();
    value.GetAs<double>( &scale_pos );

    prop3 = static_cast<wxIntProperty*>( propertygrid->GetProperty( "Period" ) );
    value = prop3->GetValue();
    value.GetAs<int>( &period );

    prop4 = static_cast<wxBoolProperty*>( propertygrid->GetProperty( "Attached to Orbiter" ) );
    value = prop4->GetValue();
    value.GetAs<bool>( &orbiter_link );

    SpectatorMovement* spectator_mvt = new SpectatorMovement();
    spectator_mvt->Init( scale_pos, period, orbiter_link );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = spectator_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptLongLatMvtCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    propertygrid->Append( new wxFloatProperty( "Initial longitud", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Initial latitud", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Initial altitud", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Initial theta", wxPG_LABEL, 0.0 ) );
    propertygrid->Append( new wxFloatProperty( "Initial phi", wxPG_LABEL, 0.0 ) );

    p_dialog->RegisterApplyButtonHandler( m_applylonglatmvtvalues_callback );
}

void wxWidgetAdapter::on_applylonglatmvtvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxFloatProperty* prop;
    wxStringProperty* prop2;
    wxAny value;

    dsreal init_longit;
    dsreal init_latit;
    dsreal init_altitud;
    dsreal init_theta;
    dsreal init_phi;

    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial longitud" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_longit );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial latitud" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_latit );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial altitud" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_altitud );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial theta" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_theta );

    prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( "Initial phi" ) );
    value = prop->GetValue();
    value.GetAs<double>( &init_phi );

    LongLatMovement* longlat_mvt = new LongLatMovement();
    longlat_mvt->Init( init_longit, init_latit, init_altitud, init_theta, init_phi );

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    (*mvts_map)[alias] = longlat_mvt;

    wxListCtrl* ctrl = (wxListCtrl*)p_dialog->GetData( "ctrl" );

    AdaptMvtsList( mvts_map, ctrl );

    p_dialog->Close();

}

void wxWidgetAdapter::AdaptCameraCreationProps( std::map<dsstring, DrawSpace::Core::Movement*>* p_mvts_map, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, "" ) );

    wxArrayString availables_bodies_labels;
    // TODO : completer availables_bodies_labels
    propertygrid->Append( new wxEnumProperty( "Attached body", wxPG_LABEL, availables_bodies_labels ));

    wxArrayString availables_bodies_transformnodes_labels;
    // TODO : completer availables_bodies_transformnodes_labels
    propertygrid->Append( new wxEnumProperty( "Lock target", wxPG_LABEL, availables_bodies_transformnodes_labels ));


    wxArrayString availables_movements_labels;
    wxArrayString availables_longlatmovements_labels;

    availables_movements_labels.Add( "..." );
    availables_longlatmovements_labels.Add( "..." );

    for( std::map<dsstring, DrawSpace::Core::Movement*>::iterator it = p_mvts_map->begin(); it != p_mvts_map->end(); ++it )
    {
        availables_movements_labels.Add( it->first.c_str() );

        Movement* mvt = it->second;

        if( dynamic_cast<LongLatMovement*>( mvt ) )
        {
            availables_longlatmovements_labels.Add( it->first );
        }
    }

    propertygrid->Append( new wxEnumProperty( "Movement", wxPG_LABEL, availables_movements_labels ));
    propertygrid->Append( new wxEnumProperty( "Longlat movement", wxPG_LABEL, availables_longlatmovements_labels ));


    p_dialog->RegisterApplyButtonHandler( m_applycameravalues_callback );

}

void wxWidgetAdapter::on_applycameravalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxStringProperty* prop2;
    wxAny value;


    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Alias" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Alias' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }


    wxEnumProperty* prop3;

    prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( "Movement" ) );
    wxString movement_name = prop3->GetValueAsString();   
    buffer = movement_name.ToAscii();
    dsstring movement_name_2 = buffer.data();

    prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( "Longlat movement" ) );
    wxString llmovement_name = prop3->GetValueAsString();   
    buffer = llmovement_name.ToAscii();
    dsstring llmovement_name_2 = buffer.data();

    std::map<dsstring, DrawSpace::Core::Movement*>* mvts_map = (std::map<dsstring, DrawSpace::Core::Movement*>*)p_dialog->GetData( "mvts_map" );

    CameraPoint* camera_point = new CameraPoint( alias, NULL, "" );

    if( movement_name_2 != "..." )
    {
        camera_point->RegisterMovement( movement_name_2, (*mvts_map)[movement_name_2] );
    }

    if( llmovement_name_2 != "..." )
    {
        camera_point->RegisterLongLatMovement( llmovement_name_2, static_cast<LongLatMovement*>( (*mvts_map)[llmovement_name_2] ) );
    }

    DrawSpace::Scenegraph* scenegraph = (DrawSpace::Scenegraph*)p_dialog->GetData( "scenegraph" );
    scenegraph->RegisterNode( camera_point );

    wxListCtrl* cameraslistctrl = (wxListCtrl*)p_dialog->GetData( "cameraslistctrl" );
    wxListCtrl* scenegraphctrl = (wxListCtrl*)p_dialog->GetData( "scenegraphctrl" );
    wxComboBox* cameraslistcombobox = (wxComboBox*)p_dialog->GetData( "cameraslistcombobox" );

    AdaptCamerasList( scenegraph, cameraslistctrl );
    AdaptScenegraphList( scenegraph, scenegraphctrl );
    AdaptCameraListComboBox( scenegraph, cameraslistcombobox );

    p_dialog->Close();
}

void wxWidgetAdapter::AdaptSpaceBoxCreationProps( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    propertygrid->Append( new wxStringProperty( "Scene name", wxPG_LABEL, "" ) );

    p_dialog->RegisterApplyButtonHandler( m_applyspaceboxvalues_callback );
    p_dialog->RegisterSpecificButton0Handler( m_applyspaceboxaddpassslot_callback );

    m_pass_slot_index = 0;
}


void wxWidgetAdapter::on_applyspaceboxvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxStringProperty* prop2;
    wxAny value;


    dsstring alias;
    wxString alias2;
    wxCharBuffer buffer;


    prop2 = static_cast<wxStringProperty*>( propertygrid->GetProperty( "Scene name" ) );
    value = prop2->GetValue();
    value.GetAs<wxString>( &alias2 );
    buffer = alias2.ToAscii();
    alias = buffer.data();

    if( "" == alias )
    {
        wxMessageBox( "'Scene name' attribute cannot be void", "DrawFront error", wxICON_ERROR );
        return;
    }


    Spacebox* spacebox = new Spacebox();

    spacebox->SetSceneName( alias );

    long pass_slot_count = 0;
    wxEnumProperty* prop3;
    wxIntProperty* prop4;
    wxStringProperty* prop5;

    while( 1 )
    {
        char pass_slot_index[32];
        sprintf( pass_slot_index, "pass_slot_%d", pass_slot_count++ );

        // check if root pass slot exists
        prop5 = static_cast<wxStringProperty*>( propertygrid->GetProperty( pass_slot_index ) );
        if( NULL == prop5 )
        {
            break;
        }
                
        dsstring curr_id;

        curr_id = pass_slot_index;
        curr_id += ".pass_name";

        
        prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( curr_id ) );


        wxString pass_slot_name = prop3->GetValueAsString();   
        buffer = pass_slot_name.ToAscii();
        dsstring pass_slot_name_2 = buffer.data();

        // ICI : pass slot name
        spacebox->RegisterPassSlot( pass_slot_name_2 );

        for( long i = 0; i < 6; i++ )
        {
            dsstring sb_type;

            switch( i )
            {
                case Spacebox::FrontQuad:

                    sb_type = "front";
                    break;

                case Spacebox::RearQuad:

                    sb_type = "rear";
                    break;

                case Spacebox::TopQuad:

                    sb_type = "top";
                    break;

                case Spacebox::BottomQuad:

                    sb_type = "bottom";
                    break;

                case Spacebox::LeftQuad:

                    sb_type = "left";
                    break;

                case Spacebox::RightQuad:

                    sb_type = "right";
                    break;

            }

            for( long j = 0; j < RenderingNode::NbMaxTextures; j++ )
            {
                char texture_stage_index[32];
                sprintf( texture_stage_index, "stage %d", j );
    
                curr_id = pass_slot_index;
                curr_id += ".textures.";
                curr_id += sb_type;
                curr_id += ".";
                curr_id += dsstring( texture_stage_index );


                prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( curr_id ) );

                wxString pass_slot_texture_name = prop3->GetValueAsString();   
                buffer = pass_slot_texture_name.ToAscii();
                dsstring pass_slot_texture_name_2 = buffer.data();

                if( pass_slot_texture_name_2 != "..." )
                {                               
                    Texture* texture = static_cast<Texture*>( AssetsBase::GetInstance()->GetAsset( pass_slot_texture_name_2 ) );
                    spacebox->GetNodeFromPass( pass_slot_name_2, i )->SetTexture( texture, j );
                }
            }
        }


        curr_id = pass_slot_index;
        curr_id += ".fx";
        
        prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( curr_id ) );

        wxString pass_slot_fxname = prop3->GetValueAsString();   
        buffer = pass_slot_fxname.ToAscii();
        dsstring pass_slot_fxname_2 = buffer.data();
        // ICI : pass slot fx name

        Fx* fx = static_cast<Fx*>( ConfigsBase::GetInstance()->GetConfigurableInstance( pass_slot_fxname_2 ) );
        for( long i = 0; i < 6; i++ )
        {
            spacebox->GetNodeFromPass( pass_slot_name_2, i )->SetFx( fx );
        }

        curr_id = pass_slot_index;
        curr_id += ".rendering_order";
        
        prop4 = static_cast<wxIntProperty*>( propertygrid->GetProperty( curr_id ) );        
        value = prop4->GetValue();
        int rendering_order;
        value.GetAs<int>( &rendering_order );

        for( long i = 0; i < 6; i++ )
        {
            spacebox->GetNodeFromPass( pass_slot_name_2, i )->SetOrderNumber( rendering_order );
        }                
    }

    // register spacebox in scenegraph
    DrawSpace::Scenegraph* scenegraph = (DrawSpace::Scenegraph*)p_dialog->GetData( "scenegraph" );
    scenegraph->RegisterNode( spacebox );
    std::map<dsstring, BasicSceneMainFrame::MetadataScenegraphEntry>* metadata_scenegraph = (std::map<dsstring, BasicSceneMainFrame::MetadataScenegraphEntry>*)p_dialog->GetData( "metadata_scenegraph" );

    BasicSceneMainFrame::MetadataScenegraphEntry metadata_scenegraph_entry;
    metadata_scenegraph_entry.node = spacebox;
    metadata_scenegraph_entry.transformation_source_type = BasicSceneMainFrame::TRANSFORMATIONSOURCE_MATRIXSTACK;
    metadata_scenegraph_entry.propose_matrixstack = true;
    metadata_scenegraph_entry.propose_body = false;
    metadata_scenegraph_entry.propose_movement = false;
    DrawSpace::Utils::Matrix scale_mat;
    scale_mat.Scale( 20.0, 20.0, 20.0 );
    BasicSceneMainFrame::TransformationMatrixDescriptor descr;
    descr.ope = BasicSceneMainFrame::TRANSFORMATIONMATRIX_SCALE;
    descr.arg.angle = 0.0;    
    descr.arg.scale[0] = scale_mat( 0, 0 );
    descr.arg.scale[1] = scale_mat( 1, 1 );
    descr.arg.scale[2] = scale_mat( 2, 2 );
    descr.arg.scale[3] = 1.0;
    metadata_scenegraph_entry.matrix_stack_descr.push_back( descr );

    metadata_scenegraph_entry.matrix_stack.PushMatrix( scale_mat );

    (*metadata_scenegraph)[alias] = metadata_scenegraph_entry;

    // call UpdateOutputQueue() for all passes
    std::vector<DrawSpace::Core::Configurable*>* configs = (std::vector<DrawSpace::Core::Configurable*>*)p_dialog->GetData( "configs" );
    for( size_t i = 0; i < configs->size(); i++ )
    {
        Pass* pass = dynamic_cast<Pass*>( (*configs)[i] );
        if( pass )
        {
            pass->GetRenderingQueue()->UpdateOutputQueue();
        }
    }

    wxListCtrl* scenegraphctrl = (wxListCtrl*)p_dialog->GetData( "scenegraphctrl" );
    AdaptScenegraphList( scenegraph, scenegraphctrl );
    p_dialog->Close();
}

void wxWidgetAdapter::on_applyspaceboxaddpassslot( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    std::map<dsstring, DrawSpace::Core::Configurable*> configs;
    char pass_slot_index[32];
    wxArrayString availables_passes_labels;

    sprintf( pass_slot_index, "pass_slot_%d", m_pass_slot_index++ );
    wxPGProperty* slot_prop = propertygrid->Append( new wxStringProperty( pass_slot_index, wxPG_LABEL, "<composed>" ) );

 

    ConfigsBase::GetInstance()->GetConfigsInstancesList( configs );
    for( std::map<dsstring, DrawSpace::Core::Configurable*>::iterator it = configs.begin(); it != configs.end(); ++it )
    {
        Pass* pass = dynamic_cast<Pass*>( it->second );
        if( pass )
        {
            dsstring pass_name;
            
            availables_passes_labels.Add( it->first.c_str() );
        }
    }

    propertygrid->AppendIn( slot_prop, new wxEnumProperty( "pass_name", wxPG_LABEL, availables_passes_labels ) );

    ////

    wxArrayString availables_textures_labels;

    /// retrieve all textures assets name

    std::map<dsstring, Asset*> assets_list;
    AssetsBase::GetInstance()->GetAssetsList( assets_list );

    availables_textures_labels.Add( "..." );
    for( std::map<dsstring, Asset*>::iterator it = assets_list.begin(); it != assets_list.end(); ++it )
    {
        if( dynamic_cast<Texture*>( it->second ) )
        {
            availables_textures_labels.Add( it->first.c_str() );
        }
    }
    ////

    wxPGProperty* slot_textures_prop = propertygrid->AppendIn( slot_prop, new wxStringProperty( "textures", wxPG_LABEL, "<composed>" ) );
    for( long i = 0; i < 6; i++ )
    {
        dsstring sb_type;

        switch( i )
        {
            case Spacebox::FrontQuad:

                sb_type = "front";
                break;

            case Spacebox::RearQuad:

                sb_type = "rear";
                break;

            case Spacebox::TopQuad:

                sb_type = "top";
                break;

            case Spacebox::BottomQuad:

                sb_type = "bottom";
                break;

            case Spacebox::LeftQuad:

                sb_type = "left";
                break;

            case Spacebox::RightQuad:

                sb_type = "right";
                break;

        }

        wxPGProperty* slot_texture_stage_prop = propertygrid->AppendIn( slot_textures_prop, new wxStringProperty( sb_type.c_str(), wxPG_LABEL, "<composed>" ) );
        
        for( long j = 0; j < RenderingNode::NbMaxTextures; j++ )
        {
            char texture_stage_index[32];

            sprintf( texture_stage_index, "stage %d", j );
            propertygrid->AppendIn( slot_texture_stage_prop, new wxEnumProperty( texture_stage_index, wxPG_LABEL, availables_textures_labels ) );
        }        
    }


    wxArrayString availables_fx_labels;

    /// retrieve all fx config name

    std::map<dsstring, DrawSpace::Core::Configurable*> configs_list;

    ConfigsBase::GetInstance()->GetConfigsInstancesList( configs_list );
    
    for( std::map<dsstring, DrawSpace::Core::Configurable*>::iterator it = configs_list.begin(); it != configs_list.end(); ++it )
    {
        if( dynamic_cast<Fx*>( it->second ) )
        {
            availables_fx_labels.Add( it->first.c_str() );
        }
    }


    propertygrid->AppendIn( slot_prop, new wxEnumProperty( "fx", wxPG_LABEL, availables_fx_labels ) );

    //////////////////

    propertygrid->AppendIn( slot_prop, new wxIntProperty( "rendering_order", wxPG_LABEL, 200 ) );


    ////

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void wxWidgetAdapter::AdaptLinearMvtProps( const dsstring& p_mvtname, DrawSpace::Core::LinearMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    Vector init_pos;
    Vector current_pos;
    Vector direction;
    dsreal current_theta;
    dsreal current_phi;

    p_movement->GetInitPos( init_pos );
    p_movement->GetDirection( direction );
    p_movement->GetCurrentPos( current_pos );
    current_theta = p_movement->GetCurrentTheta();
    current_phi = p_movement->GetCurrentPhi();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );

    wxPGProperty* initpos_prop = propertygrid->Append( new wxStringProperty( "Initial position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "x", wxPG_LABEL, init_pos[0] ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "y", wxPG_LABEL, init_pos[1] ) );
    propertygrid->AppendIn( initpos_prop, new wxFloatProperty( "z", wxPG_LABEL, init_pos[2] ) );


    wxPGProperty* direction_prop = propertygrid->Append( new wxStringProperty( "Direction", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "x", wxPG_LABEL, direction[0] ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "y", wxPG_LABEL, direction[1] ) );
    propertygrid->AppendIn( direction_prop, new wxFloatProperty( "z", wxPG_LABEL, direction[2] ) );

    wxPGProperty* currentpos_prop = propertygrid->Append( new wxStringProperty( "Current position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( currentpos_prop, new wxFloatProperty( "x", wxPG_LABEL, current_pos[0] ) );
    propertygrid->AppendIn( currentpos_prop, new wxFloatProperty( "y", wxPG_LABEL, current_pos[1] ) );
    propertygrid->AppendIn( currentpos_prop, new wxFloatProperty( "z", wxPG_LABEL, current_pos[2] ) );

    propertygrid->Append( new wxFloatProperty( "Current theta", wxPG_LABEL, current_theta ) );
    propertygrid->Append( new wxFloatProperty( "Current phi", wxPG_LABEL, current_phi ) );

}

void wxWidgetAdapter::AdaptCircularMvtProps( const dsstring& p_mvtname, DrawSpace::Core::CircularMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    Vector center_pos;
    Vector delta_center;
    Vector rot_axis;
    dsreal init_angle;
    dsreal current_angle;
    dsreal current_theta;
    dsreal current_phi;

    p_movement->GetCenterPos( center_pos );
    p_movement->GetDeltaCenter( delta_center );
    p_movement->GetRotAxis( rot_axis );
    init_angle = p_movement->GetInitAngle();
    current_angle = p_movement->GetCurrentAngle();
    current_theta = p_movement->GetCurrentTheta();
    current_phi = p_movement->GetCurrentPhi();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );

    wxPGProperty* centerpos_prop = propertygrid->Append( new wxStringProperty( "Center position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "x", wxPG_LABEL, center_pos[0] ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "y", wxPG_LABEL, center_pos[1] ) );
    propertygrid->AppendIn( centerpos_prop, new wxFloatProperty( "z", wxPG_LABEL, center_pos[2] ) );

    wxPGProperty* deltacenter_prop = propertygrid->Append( new wxStringProperty( "Delta center", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "x", wxPG_LABEL, delta_center[0] ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "y", wxPG_LABEL, delta_center[1] ) );
    propertygrid->AppendIn( deltacenter_prop, new wxFloatProperty( "z", wxPG_LABEL, delta_center[2] ) );

    wxPGProperty* rotaxis_prop = propertygrid->Append( new wxStringProperty( "Rotation axis", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "x", wxPG_LABEL, rot_axis[0] ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "y", wxPG_LABEL, rot_axis[1] ) );
    propertygrid->AppendIn( rotaxis_prop, new wxFloatProperty( "z", wxPG_LABEL, rot_axis[2] ) );


    propertygrid->Append( new wxFloatProperty( "Initial angle", wxPG_LABEL, init_angle ) );
    propertygrid->Append( new wxFloatProperty( "Current angle", wxPG_LABEL, current_angle ) );
    propertygrid->Append( new wxFloatProperty( "Current theta", wxPG_LABEL, current_theta ) );
    propertygrid->Append( new wxFloatProperty( "Current phi", wxPG_LABEL, current_phi ) );
}

void wxWidgetAdapter::AdaptFpsMvtProps( const dsstring& p_mvtname, DrawSpace::Core::FPSMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();
  
    Vector current_pos;
    dsreal current_yaw, current_pitch;

    p_movement->GetCurrentPos( current_pos );
    current_yaw = p_movement->GetCurrentYaw();
    current_pitch = p_movement->GetCurrentPitch();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );

    wxPGProperty* currpos_prop = propertygrid->Append( new wxStringProperty( "Current position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "x", wxPG_LABEL, current_pos[0] ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "y", wxPG_LABEL, current_pos[1] ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "z", wxPG_LABEL, current_pos[2] ) );

    propertygrid->Append( new wxFloatProperty( "Current yaw", wxPG_LABEL, current_yaw ) );
    propertygrid->Append( new wxFloatProperty( "Current pitch", wxPG_LABEL, current_pitch ) );

}

void wxWidgetAdapter::AdaptFreeMvtProps( const dsstring& p_mvtname, DrawSpace::Core::FreeMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();
  
    Vector current_pos;

    p_movement->GetCurrentPos( current_pos );

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );

    wxPGProperty* currpos_prop = propertygrid->Append( new wxStringProperty( "Current position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "x", wxPG_LABEL, current_pos[0] ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "y", wxPG_LABEL, current_pos[1] ) );
    propertygrid->AppendIn( currpos_prop, new wxFloatProperty( "z", wxPG_LABEL, current_pos[2] ) );
}

void wxWidgetAdapter::AdaptHeadMvtProps( const dsstring& p_mvtname, DrawSpace::Core::HeadMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    Vector head_pos;
    dsreal ref_force;
    dsreal scale_factor;

    p_movement->GetHeadPos( head_pos );
    scale_factor = p_movement->GetScaleFactor();
    ref_force = p_movement->GetRefForce();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );

    propertygrid->Append( new wxFloatProperty( "Scale factor", wxPG_LABEL, scale_factor ) );
    propertygrid->Append( new wxFloatProperty( "Ref force", wxPG_LABEL, ref_force ) );


    wxPGProperty* headpos_prop = propertygrid->Append( new wxStringProperty( "Head position", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "x", wxPG_LABEL, head_pos[0] ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "y", wxPG_LABEL, head_pos[1] ) );
    propertygrid->AppendIn( headpos_prop, new wxFloatProperty( "z", wxPG_LABEL, head_pos[2] ) );

}

void wxWidgetAdapter::AdaptSpectatorMvtProps( const dsstring& p_mvtname, DrawSpace::Core::SpectatorMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    dsreal  scale_pos;
    long    pos_period;
    bool    orbiter_link;

    scale_pos = p_movement->GetScalePos();
    pos_period = p_movement->GetPosPeriod();
    orbiter_link = p_movement->GetOrbiterLinkState();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );
    
    propertygrid->Append( new wxFloatProperty( "Scale pos", wxPG_LABEL, scale_pos ) );
    propertygrid->Append( new wxIntProperty( "Position period", wxPG_LABEL, pos_period ) );
    propertygrid->Append( new wxBoolProperty( "Orbiter link", wxPG_LABEL, orbiter_link ) );
}

void wxWidgetAdapter::AdaptLongLatMvtProps( const dsstring& p_mvtname, DrawSpace::Core::LongLatMovement* p_movement, BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    dsreal longitud;
    dsreal latitud;
    dsreal altitud;
    dsreal theta;
    dsreal phi;

    longitud = p_movement->GetCurrentLongitud();
    latitud = p_movement->GetCurrentLatitud();
    altitud = p_movement->GetCurrentAltitud();
    theta = p_movement->GetCurrentTheta();
    phi = p_movement->GetCurrentPhi();

    propertygrid->Append( new wxStringProperty( "Alias", wxPG_LABEL, p_mvtname.c_str() ) );
    
    propertygrid->Append( new wxFloatProperty( "Longitud", wxPG_LABEL, longitud ) );
    propertygrid->Append( new wxFloatProperty( "Latitud", wxPG_LABEL, latitud ) );
    propertygrid->Append( new wxFloatProperty( "Altitud", wxPG_LABEL, altitud ) );
    propertygrid->Append( new wxFloatProperty( "Theta", wxPG_LABEL, theta ) );
    propertygrid->Append( new wxFloatProperty( "Phi", wxPG_LABEL, phi ) );

}

void wxWidgetAdapter::AdaptTransfoSourceModification( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();
    wxArrayString transfosources_labels;

    BasicSceneMainFrame::MetadataScenegraphEntry* entry = (BasicSceneMainFrame::MetadataScenegraphEntry*)p_dialog->GetData( "metadata_scenegraph_entry" );

    if( entry->propose_matrixstack )
    {
        transfosources_labels.Add( "matrix_stack" );
    }
    if( entry->propose_movement )
    {
        transfosources_labels.Add( "movement" );
    }
    if( entry->propose_body )
    {
        transfosources_labels.Add( "body" );
    }

    propertygrid->Append( new wxEnumProperty( "transformation_source", wxPG_LABEL, transfosources_labels ) );
    p_dialog->RegisterApplyButtonHandler( m_applytransfosourcemodification_callback );
}

void wxWidgetAdapter::on_applytransfosourcemodification( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    BasicSceneMainFrame::MetadataScenegraphEntry* entry = (BasicSceneMainFrame::MetadataScenegraphEntry*)p_dialog->GetData( "metadata_scenegraph_entry" );

    wxCharBuffer buffer;
    wxEnumProperty* prop;
    prop = static_cast<wxEnumProperty*>( propertygrid->GetProperty( "transformation_source" ) );
    wxString source_name = prop->GetValueAsString();   
    buffer = source_name.ToAscii();
    dsstring source_name_2 = buffer.data();
    
    if( "matrix_stack" == source_name_2 )
    {
        entry->transformation_source_type = BasicSceneMainFrame::TRANSFORMATIONSOURCE_MATRIXSTACK;
    }
    else if( "movement" == source_name_2 )
    {
        entry->transformation_source_type = BasicSceneMainFrame::TRANSFORMATIONSOURCE_MOVEMENT;
    }
    else if( "body" == source_name_2 )
    {
        entry->transformation_source_type = BasicSceneMainFrame::TRANSFORMATIONSOURCE_BODY;
    }
    p_dialog->Close();
}

void wxWidgetAdapter::AdaptMatrixStackEdition( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    BasicSceneMainFrame::MetadataScenegraphEntry* entry = (BasicSceneMainFrame::MetadataScenegraphEntry*)p_dialog->GetData( "metadata_scenegraph_entry" );

    size_t i;
    for( i = 0; i < entry->matrix_stack_descr.size(); i++ )
    {
        char matrix_index[32];
        wxArrayString matrix_type_labels;

        sprintf( matrix_index, "matrix_%d", i );
        wxPGProperty* mat_prop = propertygrid->Append( new wxStringProperty( matrix_index, wxPG_LABEL, "<composed>" ) );

        matrix_type_labels.Add( "identity" );
        matrix_type_labels.Add( "scaling" );
        matrix_type_labels.Add( "translation" );
        matrix_type_labels.Add( "rotation" );
        

        wxArrayInt arrIds;
        arrIds.Add( BasicSceneMainFrame::TRANSFORMATIONMATRIX_IDENTITY );
        arrIds.Add( BasicSceneMainFrame::TRANSFORMATIONMATRIX_SCALE );
        arrIds.Add( BasicSceneMainFrame::TRANSFORMATIONMATRIX_TRANSLATION );
        arrIds.Add( BasicSceneMainFrame::TRANSFORMATIONMATRIX_ROTATION );


        wxEnumProperty* matrix_type_combo = new wxEnumProperty( "matrix_type", wxPG_LABEL, matrix_type_labels, arrIds, entry->matrix_stack_descr[i].ope ); 
        propertygrid->AppendIn( mat_prop, matrix_type_combo );
        
        wxPGProperty* mat_prop_translation = propertygrid->AppendIn( mat_prop, new wxStringProperty( "translation", wxPG_LABEL, "<composed>" ) );
        propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "x", wxPG_LABEL, entry->matrix_stack_descr[i].arg.translation[0] ) );
        propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "y", wxPG_LABEL, entry->matrix_stack_descr[i].arg.translation[1] ) );
        propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "z", wxPG_LABEL, entry->matrix_stack_descr[i].arg.translation[2] ) );

        wxPGProperty* mat_prop_rotation = propertygrid->AppendIn( mat_prop, new wxStringProperty( "rotation", wxPG_LABEL, "<composed>" ) );
        wxPGProperty* mat_prop_rotation_axis = propertygrid->AppendIn( mat_prop_rotation, new wxStringProperty( "axis", wxPG_LABEL, "<composed>" ) );
        propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "x", wxPG_LABEL, entry->matrix_stack_descr[i].arg.rotation[0] ) );
        propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "y", wxPG_LABEL, entry->matrix_stack_descr[i].arg.rotation[1] ) );
        propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "z", wxPG_LABEL, entry->matrix_stack_descr[i].arg.rotation[2] ) );
        propertygrid->AppendIn( mat_prop_rotation, new wxFloatProperty( "angle", wxPG_LABEL, entry->matrix_stack_descr[i].arg.angle ) );
        
        wxPGProperty* mat_prop_scale = propertygrid->AppendIn( mat_prop, new wxStringProperty( "scaling", wxPG_LABEL, "<composed>" ) );
        propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "x", wxPG_LABEL, entry->matrix_stack_descr[i].arg.scale[0] ) );
        propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "y", wxPG_LABEL, entry->matrix_stack_descr[i].arg.scale[1] ) );
        propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "z", wxPG_LABEL, entry->matrix_stack_descr[i].arg.scale[2] ) );

    }

    propertygrid->ResetColumnSizes();

    m_matrix_slot_index = i;
    p_dialog->RegisterApplyButtonHandler( m_applymatrixstackvalue_callback );
    p_dialog->RegisterSpecificButton0Handler( m_applymatrixstackaddmatrix_callback );
    p_dialog->RegisterSpecificButton1Handler( m_applymatrixstackclearall_callback );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applymatrixstackvalues( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

    wxStringProperty* prop;
    wxFloatProperty* prop2;
    wxEnumProperty* prop3;
    wxCharBuffer buffer;
    wxAny value;


    BasicSceneMainFrame::MetadataScenegraphEntry* entry = (BasicSceneMainFrame::MetadataScenegraphEntry*)p_dialog->GetData( "metadata_scenegraph_entry" );
    entry->matrix_stack_descr.clear();
    entry->matrix_stack.ClearAll();

    long matrix_count = 0;
    while( 1 )
    {
        char matrix_index[32];

        sprintf( matrix_index, "matrix_%d", matrix_count++ );

        // check if root exists
        prop = static_cast<wxStringProperty*>( propertygrid->GetProperty( matrix_index ) );
        if( NULL == prop )
        {
            break;
        }

        dsstring curr_id;

        curr_id = matrix_index;
        curr_id += ".matrix_type";

        
        prop3 = static_cast<wxEnumProperty*>( propertygrid->GetProperty( curr_id ) );


        wxString matrix_type_name = prop3->GetValueAsString();   
        buffer = matrix_type_name.ToAscii();
        dsstring matrix_type_name_2 = buffer.data();


        curr_id = matrix_index;
        curr_id += ".translation.x";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float translation_x;
        value.GetAs<float>( &translation_x );

        curr_id = matrix_index;
        curr_id += ".translation.y";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float translation_y;
        value.GetAs<float>( &translation_y );

        curr_id = matrix_index;
        curr_id += ".translation.z";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float translation_z;
        value.GetAs<float>( &translation_z );


        curr_id = matrix_index;
        curr_id += ".rotation.axis.x";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float rotation_axis_x;
        value.GetAs<float>( &rotation_axis_x );

        curr_id = matrix_index;
        curr_id += ".rotation.axis.y";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float rotation_axis_y;
        value.GetAs<float>( &rotation_axis_y );


        curr_id = matrix_index;
        curr_id += ".rotation.axis.z";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float rotation_axis_z;
        value.GetAs<float>( &rotation_axis_z );


        curr_id = matrix_index;
        curr_id += ".rotation.angle";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float rotation_angle;
        value.GetAs<float>( &rotation_angle );



        curr_id = matrix_index;
        curr_id += ".scaling.x";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float scaling_x;
        value.GetAs<float>( &scaling_x );

        curr_id = matrix_index;
        curr_id += ".scaling.y";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float scaling_y;
        value.GetAs<float>( &scaling_y );

        curr_id = matrix_index;
        curr_id += ".scaling.z";
        prop2 = static_cast<wxFloatProperty*>( propertygrid->GetProperty( curr_id ) );
        value = prop2->GetValue();
        float scaling_z;
        value.GetAs<float>( &scaling_z );


        ////////////////////////

        BasicSceneMainFrame::TransformationMatrixDescriptor descr;

        if( "identity" == matrix_type_name_2 )
        {
            descr.ope = BasicSceneMainFrame::TRANSFORMATIONMATRIX_IDENTITY;
        }
        else if( "translation" == matrix_type_name_2 )
        {
            descr.ope = BasicSceneMainFrame::TRANSFORMATIONMATRIX_TRANSLATION;
        }
        else if( "rotation" == matrix_type_name_2 )
        {
            descr.ope = BasicSceneMainFrame::TRANSFORMATIONMATRIX_ROTATION;
        }
        else if( "scaling" == matrix_type_name_2 )
        {
            descr.ope = BasicSceneMainFrame::TRANSFORMATIONMATRIX_SCALE;
        }

        descr.arg.translation[0] = translation_x;
        descr.arg.translation[1] = translation_y;
        descr.arg.translation[2] = translation_z;
        descr.arg.translation[3] = 1.0;


        descr.arg.rotation[0] = rotation_axis_x;
        descr.arg.rotation[1] = rotation_axis_y;
        descr.arg.rotation[2] = rotation_axis_z;
        descr.arg.rotation[3] = 1.0;

        descr.arg.angle = rotation_angle;

        descr.arg.scale[0] = scaling_x;
        descr.arg.scale[1] = scaling_y;
        descr.arg.scale[2] = scaling_z;
        descr.arg.scale[3] = 1.0;

        entry->matrix_stack_descr.push_back( descr );
        
        /////

        Matrix mat;

        if( "identity" == matrix_type_name_2 )
        {           
            mat.Identity();
            entry->matrix_stack.PushMatrix( mat );
        }
        else if( "translation" == matrix_type_name_2 )
        {
            mat.Translation( translation_x, translation_y, translation_z );
        }
        else if( "rotation" == matrix_type_name_2 )
        {
            mat.Rotation( Vector( rotation_axis_x, rotation_axis_y, rotation_axis_z, 1.0 ), Utils::Maths::DegToRad( rotation_angle ) );
        }
        else if( "scaling" == matrix_type_name_2 )
        {
            mat.Scale( scaling_x, scaling_y, scaling_z );
        }

        entry->matrix_stack.PushMatrix( mat );
    }

    p_dialog->Close();
}

void wxWidgetAdapter::on_applymatrixstackaddmatrix( BasicSceneObjectPropertiesDialog* p_dialog )
{
    wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();
    char matrix_index[32];
    wxArrayString matrix_type_labels;

    sprintf( matrix_index, "matrix_%d", m_matrix_slot_index++ );
    wxPGProperty* mat_prop = propertygrid->Append( new wxStringProperty( matrix_index, wxPG_LABEL, "<composed>" ) );

    matrix_type_labels.Add( "identity" );
    matrix_type_labels.Add( "translation" );
    matrix_type_labels.Add( "rotation" );
    matrix_type_labels.Add( "scaling" );
    propertygrid->AppendIn( mat_prop, new wxEnumProperty( "matrix_type", wxPG_LABEL, matrix_type_labels ) );

    wxPGProperty* mat_prop_translation = propertygrid->AppendIn( mat_prop, new wxStringProperty( "translation", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( mat_prop_translation, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );

    wxPGProperty* mat_prop_rotation = propertygrid->AppendIn( mat_prop, new wxStringProperty( "rotation", wxPG_LABEL, "<composed>" ) );
    wxPGProperty* mat_prop_rotation_axis = propertygrid->AppendIn( mat_prop_rotation, new wxStringProperty( "axis", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "x", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "y", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( mat_prop_rotation_axis, new wxFloatProperty( "z", wxPG_LABEL, 0.0 ) );
    propertygrid->AppendIn( mat_prop_rotation, new wxFloatProperty( "angle", wxPG_LABEL, 0.0 ) );
    
    wxPGProperty* mat_prop_scale = propertygrid->AppendIn( mat_prop, new wxStringProperty( "scaling", wxPG_LABEL, "<composed>" ) );
    propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "x", wxPG_LABEL, 1.0 ) );
    propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "y", wxPG_LABEL, 1.0 ) );
    propertygrid->AppendIn( mat_prop_scale, new wxFloatProperty( "z", wxPG_LABEL, 1.0 ) );

    propertygrid->ResetColumnSizes();
    propertygrid->CollapseAll();
}

void wxWidgetAdapter::on_applymatrixstackclearall( BasicSceneObjectPropertiesDialog* p_dialog )
{
    p_dialog->GetPropertyGrid()->Clear();
    m_matrix_slot_index = 0;
}

void wxWidgetAdapter::AdaptCameraListComboBox( DrawSpace::Scenegraph* p_scenegraph, wxComboBox* p_combobox )
{
    p_combobox->Clear();

    p_combobox->Append( wxString( "<none>" ), (void *)NULL );

    std::map<dsstring, Core::TransformNode*> cameras_list = p_scenegraph->GetCamerasList();
    for( std::map<dsstring, Core::TransformNode*>::iterator it = cameras_list.begin(); it != cameras_list.end(); ++it )
    {
        p_combobox->Append( it->first.c_str(), (void *)it->second );
    }

    p_combobox->SetSelection( 0 );
}