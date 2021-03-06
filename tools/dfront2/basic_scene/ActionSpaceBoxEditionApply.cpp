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

#include <wx/wx.h>
#include "BasicSceneMainFrame.h"

#include "ActionSpaceBoxEditionApply.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::Utils;

void ActionSpaceBoxEditionApply::Execute( BasicSceneObjectPropertiesDialog* p_dialog )
{
    DIALOG_GETGRID
    DIALOG_PROPERTIES_VARS

    DrawSpace::Utils::SpaceboxDescriptor sb_descr = BasicSceneMainFrame::GetInstance()->m_spacebox_descriptors[p_dialog->GetTreeItem().GetID()];
    BasicSceneMainFrame::SceneNodeEntry<DrawSpace::Spacebox> sne = BasicSceneMainFrame::GetInstance()->m_spacebox_nodes[p_dialog->GetTreeItem().GetID()];

    for( std::map<dsstring, SpaceboxPassDescriptor>::iterator it = sb_descr.passes_slots.begin(); it != sb_descr.passes_slots.end(); ++it )
    {
        dsstring pass_name = it->first;
        SpaceboxPassDescriptor pass_descr = it->second;
        
        DIALOG_EXPLORE_NODES_BEGIN( pass_name, "shader parameter %d", i, sp_slot )

            DIALOG_GET_STRING_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "param id" ), param_id )
            
            DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.x" ), val_x )
            DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.y" ), val_y )
            DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.z" ), val_z )
            DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.w" ), val_w )
          
            DIALOG_WXSTRING_TO_DSSTRING( param_id, param_id2 )

            Pass* current_pass = dynamic_cast<Pass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( pass_name ) );

            for( int j = 0; j < 6; j++ )
            {
                sne.scene_node->GetContent()->GetNodeFromPass( current_pass, j )->SetShaderRealVector( param_id2, Vector( val_x, val_y, val_z, val_w ) );
            }

            // update descriptor
            sb_descr.passes_slots[pass_name].shader_params[i].value = Vector( val_x, val_y, val_z, val_w );

            BasicSceneMainFrame::GetInstance()->m_spacebox_descriptors[p_dialog->GetTreeItem().GetID()] = sb_descr;


        DIALOG_EXPLORE_NODES_END( i )
    }



    /*

    DIALOG_EXPLORE_NODES_BEGIN( "", "shader parameter %d", i, sp_slot )

        DIALOG_GET_STRING_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "pass name" ), pass_name )
        DIALOG_GET_STRING_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "param id" ), param_id )
        
        DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.x" ), val_x )
        DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.y" ), val_y )
        DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.z" ), val_z )
        DIALOG_GET_FLOAT_PROPERTY( DIALOG_INCREMENT_STRING( sp_slot, "values.w" ), val_w )

       

        DIALOG_WXSTRING_TO_DSSTRING( pass_name, pass_name2 )
        DIALOG_WXSTRING_TO_DSSTRING( param_id, param_id2 )

        Pass* current_pass = dynamic_cast<Pass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( pass_name2 ) );

        for( int j = 0; j < 6; j++ )
        {
            sne.scene_node->GetContent()->GetNodeFromPass( current_pass, j )->SetShaderRealVector( param_id2, Vector( val_x, val_y, val_z, val_w ) );
        }

        // update descriptor
        sb_descr.passes_slots[pass_name2].shader_params[i].value = Vector( val_x, val_y, val_z, val_w );

        BasicSceneMainFrame::GetInstance()->m_spacebox_descriptors[p_dialog->GetTreeItem().GetID()] = sb_descr;

    DIALOG_EXPLORE_NODES_END( i )   

    */
}
