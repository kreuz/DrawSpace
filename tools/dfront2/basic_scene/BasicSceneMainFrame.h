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

#ifndef __BasicSceneMainFrame__
#define __BasicSceneMainFrame__


#include "panel.h"
#include "drawspace.h"
#include "scripting.h"

class BasicSceneMainFrame : public MainFrame
{
public:

    typedef enum
    {
        TRANSFORMATIONSOURCE_MATRIXSTACK,
        TRANSFORMATIONSOURCE_MOVEMENT,
        TRANSFORMATIONSOURCE_BODY

    } TransformationSourceType;

    typedef enum
    {
        TRANSFORMATIONMATRIX_IDENTITY,
        TRANSFORMATIONMATRIX_SCALE,
        TRANSFORMATIONMATRIX_TRANSLATION,
        TRANSFORMATIONMATRIX_ROTATION

    } TransformationMatrixOperation;

    typedef struct
    {
        dsstring    var_alias;
        dsreal      value;          // si var_alias = ""

    } TransformationMatrixValueLinkage;

    typedef struct
    {
        //DrawSpace::Utils::Vector                translation;
        TransformationMatrixValueLinkage        translation_vals_link[3];

        //DrawSpace::Utils::Vector                rotation;
        TransformationMatrixValueLinkage        rotation_vals_link[3];

        //dsreal                                  angle;
        TransformationMatrixValueLinkage        angle_val_link;

        //DrawSpace::Utils::Vector                scale;
        TransformationMatrixValueLinkage        scale_vals_link[3];

    } TransformationMatrixArg;

    typedef struct
    {
        TransformationMatrixOperation   ope;
        TransformationMatrixArg         arg;

    } TransformationMatrixDescriptor;


    typedef struct
    {
        DrawSpace::Core::TransformNode*                 node;
        TransformationSourceType                        transformation_source_type;

        // les sources de transfo autorisees pour ce node
        bool                                            propose_matrixstack;
        bool                                            propose_movement;
        bool                                            propose_body;
        
        //DrawSpace::Utils::Transformation                matrix_stack;
        std::vector<TransformationMatrixDescriptor>     matrix_stack_descr;
        DrawSpace::Core::Movement*                      movement;
        DrawSpace::Dynamics::Body*                      body;


    } MetadataScenegraphEntry;


    typedef enum
    {
        MOVEMENTCONTROLSOURCE_KEYBMOUSE,
        MOVEMENTCONTROLSOURCE_REGISTER

    } MovementControlSource;


    typedef struct
    {
        DrawSpace::Core::Movement*  movement;

        MovementControlSource       speed_control_source;
        dsstring                    speed_control_register;

        MovementControlSource       yaw_control_source;
        dsstring                    yaw_control_register;

        MovementControlSource       pitch_control_source;
        dsstring                    pitch_control_register;

        MovementControlSource       roll_control_source;
        dsstring                    roll_control_register;

        dsreal                      theta_pos_mouse;
        dsreal                      phi_pos_mouse;

    } MovementEntry;


    typedef enum
    {
        REGISTER_CONSTANT,
        REGISTER_VARIABLE

    } RegisterMode;

    typedef enum
    {
        REGISTER_VARIABLE_TRANSLATION_SIMPLE,
        REGISTER_VARIABLE_TRANSLATION_ROUNDTRIP,
        REGISTER_VARIABLE_ANGULAR_SIMPLE,
        REGISTER_VARIABLE_ANGULAR_ROUNDTRIP,

    } RegisterVariableMode;


    typedef struct
    {
        long                    id;
        RegisterMode            mode;
        dsreal                  const_value;

        RegisterVariableMode    variable_mode;
        dsreal                  variable_initial_value;
        dsreal                  variable_speed;
        dsreal                  variable_range_sup;
        dsreal                  variable_range_inf;

        bool                    state;

        dsreal                  current_value;

       
        bool                    variable_roundtrip_back;
        bool                    discontinuity;

    } RegisterEntry;


protected:

    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, const dsstring&>   TimerCallback;
    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, const dsstring&>   ScriptingErrorCallback;
    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, int>               ScriptingCallback;

    void on_timer( const dsstring& p_timername );
    void on_scripting_error( const dsstring& p_error );

    void compute_scenegraph_transforms( void );
    void compute_regs( void );
    void compute_movements( void );

    void on_scripting( int p_value );

    long                                                    m_w_width;
    long                                                    m_w_height;

    bool                                                    m_glready;
    DrawSpace::Utils::TimeManager                           m_timer;
    std::map<dsstring, MovementEntry>                       m_movements;

    DrawSpace::Scenegraph                                   m_scenegraph;
    std::map<dsstring, MetadataScenegraphEntry>             m_metada_scenegraph;

    std::vector<DrawSpace::Core::Configurable*>             m_ordered_configs;
    long                                                    m_scenegraphlistctrl_currentindex;
    long                                                    m_mvtslistctrl_currentindex;
    long                                                    m_cameraslistctrl_currentindex;
    long                                                    m_regslistctrl_currentindex;

    std::map<dsstring, RegisterEntry>                       m_registers;

    TimerCallback*                                          m_timercb;
    ScriptingErrorCallback*                                 m_scripting_error_cb;

    wxCoord                                                 m_last_xmouse;
    wxCoord                                                 m_last_ymouse;

    DrawSpace::Dynamics::CameraPoint*                       m_current_camera;

    Scripting*                                              m_scripting;

    ScriptingCallback*                                      m_scripting_cb;

    

    virtual void OnClose( wxCloseEvent& p_event );
    virtual void OnIdle( wxIdleEvent& p_event );
	virtual void OnKeyDown( wxKeyEvent& p_event );
	virtual void OnKeyUp( wxKeyEvent& p_event );
	virtual void OnMouseMotion( wxMouseEvent& p_event );
    virtual void OnAssetsListItemActivated( wxListEvent& p_event );
    virtual void OnConfigsListItemActivated( wxListEvent& p_event );
    virtual void OnPassesListItemActivated( wxListEvent& p_event );
    virtual void OnPassesListItemSelected( wxListEvent& p_event );
    virtual void OnShadersListItemActivated( wxListEvent& p_event );
    virtual void OnCreateMvtButtonClicked( wxCommandEvent& p_event );
    virtual void OnMvtsListItemActivated( wxListEvent& p_event );
    virtual void OnMvtsListDeleteAllItems( wxListEvent& p_event );
    virtual void OnMvtsListItemSelected( wxListEvent& p_event );
    virtual void OnCreateCameraButtonClicked( wxCommandEvent& p_event );
    virtual void OnCamerasListItemActivated( wxListEvent& p_event );
    virtual void OnCamerasListDeleteAllItems( wxListEvent& p_event );
    virtual void OnCamerasListItemSelected( wxListEvent& p_event );
    virtual void OnScenegraphItemActivated( wxListEvent& p_event );
    virtual void OnScenegraphListItemSelected( wxListEvent& p_event );
    virtual void OnCreateDrawableButtonClicked( wxCommandEvent& p_event );
    virtual void OnTransfTypeButtonClicked( wxCommandEvent& p_event );
	virtual void OnTransfoEditButtonClicked( wxCommandEvent& p_event );
    virtual void OnScenegraphListItemDeselected( wxListEvent& p_event );
    virtual void OnScenegraphListDeleteItem( wxListEvent& p_event );
    virtual void OnScenegraphListDeleteAllItems( wxListEvent& p_event );
	virtual void OnControlButtonClicked( wxCommandEvent& p_event );
    virtual void OnMouseKeyboardOutputCombobox( wxCommandEvent& p_event );
    virtual void OnCreateRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnCameraEditButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegistersListItemActivated( wxListEvent& p_event );
	virtual void OnRegsListDeleteAllItems( wxListEvent& p_event );
	virtual void OnRegsListItemSelected( wxListEvent& p_event );
	virtual void OnModRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnResetRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegOnButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegOffButtonClicked( wxCommandEvent& p_event );

public:
	BasicSceneMainFrame( wxWindow* parent );

    void SetGLReady( void );
    void Update( void );
    wxNotebook* GetNoteBook( void );

    void SetWindowDims( long p_w_width, long p_w_height );


    void ExecStartupScript( const dsstring& p_scriptfilepath );
};

#endif // __BasicSceneMainFrame__
