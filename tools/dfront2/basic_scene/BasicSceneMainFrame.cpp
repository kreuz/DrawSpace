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

#include "BasicSceneMainFrame.h"
#include "drawspace.h"
#include "adapters.h"

#include "BasicSceneObjectPropertiesDialog.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;

BasicSceneMainFrame::BasicSceneMainFrame( wxWindow* parent ) : MainFrame( parent ),
m_glready( false )
{

}

void BasicSceneMainFrame::OnClose( wxCloseEvent& event )
{
    Destroy();
}

void BasicSceneMainFrame::OnIdle( wxIdleEvent& event )
{
    if( m_glready )
    {

        DrawSpace::Interface::Renderer* renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
        renderer->ClearScreen( 0, 0, 120 );


        renderer->BeginScreen();
                 
        renderer->DrawText( 255, 0, 0, 10, 20, "%d fps", m_timer.GetFPS() );

        renderer->EndScreen();

        renderer->FlipScreen();

        m_timer.Update();

        event.RequestMore( true );
    }
}

void BasicSceneMainFrame::SetGLReady( void )
{
    m_glready = true;
}

void BasicSceneMainFrame::Update( void )
{
    wxWidgetAdapter::GetInstance()->AdaptAssetsList( m_assets_listCtrl );
    wxWidgetAdapter::GetInstance()->AdaptConfigsList( m_configs_listCtrl );
    wxWidgetAdapter::GetInstance()->AdaptPassesList( m_passes_listCtrl );
    wxWidgetAdapter::GetInstance()->AdaptMvtsList( &m_movements, m_mvts_listCtrl );
}

void BasicSceneMainFrame::OnAssetsListItemActivated( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();

    Asset* asset = (Asset*)m_assets_listCtrl->GetItemData( sel_index );

    Texture* texture = dynamic_cast<Texture*>( asset );
    if( texture )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Textures properties" );
        wxWidgetAdapter::GetInstance()->AdaptTextureProps( texture, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    Shader* shader = dynamic_cast<Shader*>( asset );
    if( shader )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Shader properties" );
        wxWidgetAdapter::GetInstance()->AdaptShaderProps( shader, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    Font* font = dynamic_cast<Font*>( asset );
    if( font )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Font properties" );
        wxWidgetAdapter::GetInstance()->AdaptFontProps( font, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    Meshe* meshe = dynamic_cast<Meshe*>( asset );
    if( meshe )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Meshe properties" );
        wxWidgetAdapter::GetInstance()->AdaptMesheProps( meshe, dialog->GetPropertyGrid() );
        dialog->Show();
    }
}

void BasicSceneMainFrame::OnConfigsListItemActivated( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();

    Configurable* config = (Configurable*)m_configs_listCtrl->GetItemData( sel_index );

    Fx* fx = dynamic_cast<Fx*>( config );
    if( fx )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Fx properties" );
        wxWidgetAdapter::GetInstance()->AdaptFxProps( fx, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    IntermediatePass* ipass = dynamic_cast<IntermediatePass*>( config );
    if( ipass )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Intermediate Pass properties" );
        wxWidgetAdapter::GetInstance()->AdaptPassProps( true, ipass, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    FinalPass* fpass = dynamic_cast<FinalPass*>( config );
    if( fpass )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Final Pass properties" );
        wxWidgetAdapter::GetInstance()->AdaptPassProps( false, fpass, dialog->GetPropertyGrid() );
        dialog->Show();

    }
}

void BasicSceneMainFrame::OnPassesListItemActivated( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();

    Configurable* config = (Configurable*)m_passes_listCtrl->GetItemData( sel_index );

    IntermediatePass* ipass = dynamic_cast<IntermediatePass*>( config );
    if( ipass )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Intermediate Pass properties" );
        wxWidgetAdapter::GetInstance()->AdaptPassProps( true, ipass, dialog->GetPropertyGrid() );
        dialog->Show();
    }

    FinalPass* fpass = dynamic_cast<FinalPass*>( config );
    if( fpass )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Final Pass properties" );
        wxWidgetAdapter::GetInstance()->AdaptPassProps( false, fpass, dialog->GetPropertyGrid() );
        dialog->Show();
    }
}

void BasicSceneMainFrame::OnPassesListItemSelected( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();
    Configurable* config = (Configurable*)m_passes_listCtrl->GetItemData( sel_index );

    wxWidgetAdapter::GetInstance()->AdaptPassesShaderParamsList( static_cast<Pass*>( config ), m_shadersparams_listCtrl );
}

void BasicSceneMainFrame::OnShadersListItemActivated( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();
    Pass* pass = (Pass*)m_shadersparams_listCtrl->GetItemData( sel_index );

    wxString shader_name = m_shadersparams_listCtrl->GetItemText( sel_index );

    wxCharBuffer buffer = shader_name.ToAscii();

    BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Shader parameter values" );

    dialog->SetData( "ctrl", m_shadersparams_listCtrl );
    wxWidgetAdapter::GetInstance()->AdaptPassShaderValuesProps( pass, buffer.data(), dialog );
    dialog->EnableApplyButton();
    dialog->Show();
}

void BasicSceneMainFrame::OnCreateMvtButtonClicked( wxCommandEvent& p_event )
{
    int index = m_mvttype_comboBox->GetSelection();

    switch( index )
    {
        case 0:
            // Linear
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Linear movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptLinearMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;

        case 1:
            // Circular
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Circular movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptCircularMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;


        case 2:
            // FPS
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "FPS movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptFPSMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;

        case 3:
            // Free
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Free movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptFreeMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;

        case 4:
            // Head
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Head movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptHeadMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;

        case 5:
            // Spectator
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Spectator movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptSpectatorMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }
            break;

        case 6:
            // LongLat
            {
                BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Longlat movement values" );

                dialog->SetData( "mvts_map", &m_movements );
                dialog->SetData( "ctrl", m_mvts_listCtrl );

                wxWidgetAdapter::GetInstance()->AdaptLongLatMvtCreationProps( dialog );
                dialog->EnableApplyButton();
                dialog->Show();
            }

            break;

    }
}

void BasicSceneMainFrame::OnMvtsListItemActivated( wxListEvent& p_event )
{
    long sel_index = p_event.GetIndex();

    Movement* mvt = (Movement*)m_mvts_listCtrl->GetItemData( sel_index );

    LinearMovement* linearmvt = dynamic_cast<LinearMovement*>( mvt );
    if( linearmvt )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Linear movement properties" );
        wxWidgetAdapter::GetInstance()->AdaptLinearMvtProps( linearmvt, dialog );
        dialog->Show();
    }

    CircularMovement* circularmvt = dynamic_cast<CircularMovement*>( mvt );
    if( circularmvt )
    {
        BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, "Circular movement properties" );
        wxWidgetAdapter::GetInstance()->AdaptCircularMvtProps( circularmvt, dialog );
        dialog->Show();
    }

}