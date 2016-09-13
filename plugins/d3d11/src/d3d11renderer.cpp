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

#include "d3d11renderer.h"
#include "memalloc.h"
#include "misc_utils.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;


extern void TranslateD3DD11Error( HRESULT p_hRes, dsstring &p_str );

_DECLARE_DS_LOGGER( logger, "d3d11", NULL )

D3D11Renderer::D3D11Renderer( void ) :
m_lpd3dswapchain( NULL ),
m_lpd3ddevice( NULL ),
m_lpd3ddevcontext( NULL ),
m_screentarget( NULL ),
m_currentDevice( -1 )
{

}

D3D11Renderer::~D3D11Renderer( void )
{

}


void D3D11Renderer::GetDescr( dsstring& p_descr )
{
    p_descr = "Direct3D11";
}

void D3D11Renderer::GetDeviceDescr( DeviceDescr& p_ddescr )
{
    if( m_currentDevice != -1 )
    {
        p_ddescr = m_devices_descrs[m_currentDevice];
    }
}

void D3D11Renderer::DumpMemoryAllocs( void )
{

}

bool D3D11Renderer::Init( HWND p_hwnd, bool p_fullscreen, long p_w_width, long p_w_height, DrawSpace::Logger::Configuration* p_logconf )
{
    DECLARE_D3D11ASSERT_VARS

    p_logconf->RegisterSink( &logger );
    logger.SetConfiguration( p_logconf );

    p_logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( p_logconf );

    if( true == m_config.Run( "appconfig.txt", "    " ) )
    {
        _DSDEBUG( logger, "plugin configuration file -> OK" )
    }
    else
    {
        _DSWARN( logger, "Failed to parse plugin configuration file !! -> switching to default settings" )
        return false;
    }

    long adapter = m_config.m_adapter_ordinal;

    m_currentDevice = adapter;

    _DSDEBUG( logger, "D3D11 init begin" )

    IDXGIFactory* factory = NULL; 
    CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&factory);

    // enum existing devices

    IDXGIAdapter* curr_adapter;

    for( UINT i = 0; factory->EnumAdapters(i, &curr_adapter) != DXGI_ERROR_NOT_FOUND; ++i )
    {
        //adapters.push_back(adapter);

        DXGI_ADAPTER_DESC   descr;
        curr_adapter->GetDesc( &descr );

        DeviceDescr currentDescr;

        std::wstring wdescr( descr.Description );


        currentDescr.description = DrawSpace::Utils::WString2String( descr.Description ); //"N/A"; //wcharToDsString( descr.Description );
        currentDescr.driver = "N/A";
        currentDescr.deviceName = "N/A";

        m_devices_descrs.push_back( currentDescr );
    } 


    //////////////////////////////

    m_hwnd = p_hwnd;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;

    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = p_hwnd;
    sd.SampleDesc.Count = 1; // de 1 a 4 en D3D11
    sd.SampleDesc.Quality = 0; // en low quality
    sd.Windowed = TRUE;

    hRes = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &sd,
                                  &m_lpd3dswapchain,
                                  &m_lpd3ddevice,
                                  NULL,
                                  &m_lpd3ddevcontext );

    D3D11_CHECK( D3D11CreateDeviceAndSwapChain )

    ///////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* backBuffer;
    hRes = m_lpd3dswapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    D3D11_CHECK( GetBuffer )

    hRes = m_lpd3ddevice->CreateRenderTargetView( backBuffer, NULL, &m_screentarget );
    D3D11_CHECK( CreateRenderTargetView )

    backBuffer->Release();

    ////////////////////////////////////////////////////////////////////////

    /* a faire pour chaque shader ?? */

    /*
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    1, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    2, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    3, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    4, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    5, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    6, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    7, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    8, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

    hRes = m_lpd3ddevice->CreateInputLayout( layout, 11, NULL, 0, &m_inputLayout );
    D3D11_CHECK( CreateInputLayout )
    */

    //////////////////////////////////////////////////////////////////////////

	IFW1Factory* fW1Factory;
	hRes = FW1CreateFactory( FW1_VERSION, &fW1Factory );
	
	hRes = fW1Factory->CreateFontWrapper( m_lpd3ddevice, L"System", &m_fontWrapper );

    return true;
}

void D3D11Renderer::Release( void )
{
    _DSDEBUG( logger, "begin" )

    _DSDEBUG( logger, "end" )
}

void D3D11Renderer::SetViewport( bool p_automatic, long p_vpx, long p_vpy, long p_vpwidth, long p_vpheight, float p_vpminz, float p_vpmaxz )
{
    if( p_automatic )
    {
        RECT wndrect;
        ::GetClientRect( m_hwnd, &wndrect );

        m_viewport.Width = wndrect.right - wndrect.left;
        m_viewport.Height = wndrect.bottom - wndrect.top;
        m_viewport.MinDepth = p_vpminz;
        m_viewport.MaxDepth = p_vpmaxz;
        m_viewport.TopLeftX = wndrect.left;
        m_viewport.TopLeftY = wndrect.top;

    }
    else
    {
        m_viewport.Width = p_vpwidth;
        m_viewport.Height = p_vpheight;
        m_viewport.MinDepth = p_vpminz;
        m_viewport.MaxDepth = p_vpmaxz;
        m_viewport.TopLeftX = p_vpx;
        m_viewport.TopLeftY = p_vpy; 
    }

    m_lpd3ddevcontext->RSSetViewports( 1, &m_viewport );
}

void D3D11Renderer::BeginScreen( void )
{
    m_lpd3ddevcontext->OMSetRenderTargets( 1, &m_screentarget, NULL );
}

void D3D11Renderer::EndScreen( void )
{
}

void D3D11Renderer::FlipScreen( void )
{
    m_lpd3dswapchain->Present( 0, 0 );
}


void D3D11Renderer::ClearScreen( unsigned char p_r, unsigned char p_g, unsigned char p_b, unsigned char p_a )
{
    FLOAT clearcolor[4];

    clearcolor[0] = p_r / 255.0;
    clearcolor[1] = p_g / 255.0;
    clearcolor[2] = p_b / 255.0;
    clearcolor[3] = p_a / 255.0;

    m_lpd3ddevcontext->ClearRenderTargetView( m_screentarget, clearcolor );
}

void D3D11Renderer::ClearDepth( dsreal p_value )
{

}

void D3D11Renderer::BeginTarget( DrawSpace::Core::Texture* p_texture )
{

}

void D3D11Renderer::EndTarget( DrawSpace::Core::Texture* p_texture )
{

}

bool D3D11Renderer::CreateMeshe( DrawSpace::Core::Meshe* p_meshe, void** p_data )
{
    DECLARE_D3D11ASSERT_VARS

    D3D11_SUBRESOURCE_DATA id = {0};
    D3D11_BUFFER_DESC vertexBufferDesc = {0};
    D3D11_BUFFER_DESC indexBufferDesc = {0};
    
    Core::Meshe* meshe = p_meshe;

    dsstring hash;
    p_meshe->GetMD5( hash );

    if( m_meshes_base.count( hash ) > 0 )
    {
        *p_data = (void *)m_meshes_base[hash];
        p_meshe->SetRenderData( (void *)m_meshes_base[hash] );
        return true;
    }

    long nb_vertices = meshe->GetVertexListSize();
    long nb_triangles = meshe->GetTrianglesListSize();

    MesheData* meshe_data = _DRAWSPACE_NEW_( MesheData, MesheData );

    // vertex buffer creation
    d3d11vertex* v = new d3d11vertex[nb_vertices];
    for( long i = 0; i < nb_vertices; i++ )
    {
        Core::Vertex vertex;
        meshe->GetVertex( i, vertex );

        v[i].x = (float)vertex.x;
        v[i].y = (float)vertex.y;
        v[i].z = (float)vertex.z;

        v[i].nx = (float)vertex.nx;
        v[i].ny = (float)vertex.ny;
        v[i].nz = (float)vertex.nz;

        v[i].tu0 = vertex.tu[0];
        v[i].tu1 = vertex.tu[1];
        v[i].tu2 = vertex.tu[2];
        v[i].tu3 = vertex.tu[3];
        v[i].tu4 = vertex.tu[4];
        v[i].tu5 = vertex.tu[5];
        v[i].tu6 = vertex.tu[6];
        v[i].tu7 = vertex.tu[7];
        v[i].tu8 = vertex.tu[8];

        v[i].tv0 = vertex.tv[0];
        v[i].tv1 = vertex.tv[1];
        v[i].tv2 = vertex.tv[2];
        v[i].tv3 = vertex.tv[3];
        v[i].tv4 = vertex.tv[4];
        v[i].tv5 = vertex.tv[5];
        v[i].tv6 = vertex.tv[6];
        v[i].tv7 = vertex.tv[7];
        v[i].tv8 = vertex.tv[8];
        
        v[i].tw0 = vertex.tw[0];
        v[i].tw1 = vertex.tw[1];
        v[i].tw2 = vertex.tw[2];
        v[i].tw3 = vertex.tw[3];
        v[i].tw4 = vertex.tw[4];
        v[i].tw5 = vertex.tw[5];
        v[i].tw6 = vertex.tw[6];
        v[i].tw7 = vertex.tw[7];
        v[i].tw8 = vertex.tw[8];
        
        v[i].ta0 = vertex.ta[0];
        v[i].ta1 = vertex.ta[1];
        v[i].ta2 = vertex.ta[2];
        v[i].ta3 = vertex.ta[3];
        v[i].ta4 = vertex.ta[4];
        v[i].ta5 = vertex.ta[5];
        v[i].ta6 = vertex.ta[6];
        v[i].ta7 = vertex.ta[7];
        v[i].ta8 = vertex.ta[8];
    }

	id.pSysMem = v;
	id.SysMemPitch = sizeof( d3d11vertex );
	id.SysMemSlicePitch = 0;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = nb_vertices * sizeof( d3d11vertex );
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0; 

    hRes = m_lpd3ddevice->CreateBuffer( &vertexBufferDesc, &id, &meshe_data->vertex_buffer );
    D3D11_CHECK( CreateBuffer )

    delete[] v;

    // index buffer creation

    d3d11triangle *t = new d3d11triangle[nb_triangles];

    for( long i = 0; i < nb_triangles; i++ )
    {
        Core::Triangle triangle;
        meshe->GetTriangles( i, triangle );

        t[i].vertex1 = triangle.vertex1;
        t[i].vertex2 = triangle.vertex2;
        t[i].vertex3 = triangle.vertex3;
    }

	id.pSysMem = t;
	id.SysMemPitch = sizeof( d3d11triangle );
	id.SysMemSlicePitch = 0;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = nb_triangles * sizeof( d3d11triangle );
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0; 

    hRes = m_lpd3ddevice->CreateBuffer( &indexBufferDesc, &id, &meshe_data->index_buffer );
    D3D11_CHECK( CreateBuffer )

    delete[] t;

    meshe_data->nb_vertices = nb_vertices;
    meshe_data->nb_triangles = nb_triangles;

    *p_data = (void *)meshe_data;

    meshe->SetRenderData( (void *)meshe_data );

    m_meshes_base[hash] = meshe_data;

    return true;
}

void D3D11Renderer::RemoveMeshe( DrawSpace::Core::Meshe* p_meshe, void* p_data )
{

}

bool D3D11Renderer::SetMeshe( void* p_data )
{
    return true;
}

bool D3D11Renderer::UpdateMesheIndexes( DrawSpace::Core::Meshe* p_meshe, void* p_data )
{
    return true;
}

bool D3D11Renderer::UpdateMesheVertices( DrawSpace::Core::Meshe* p_meshe, void* p_data )
{
    return true;
}

bool D3D11Renderer::UpdateMesheVerticesFromImpostors( const DrawSpace::ImpostorsDisplayList& p_list, void* p_data )
{
    return true;
}

bool D3D11Renderer::CreateTexture( DrawSpace::Core::Texture* p_texture, void** p_data )
{
    return true;
}

void D3D11Renderer::DestroyTexture( void* p_data )
{

}

bool D3D11Renderer::SetTexture( void* p_data, int p_stage )
{
    return true;
}

bool D3D11Renderer::SetVertexTexture( void* p_data, int p_stage )
{
    return true;
}

bool D3D11Renderer::UnsetTexture( int p_stage )
{
    return true;
}

bool D3D11Renderer::UnsetVertexTexture( int p_stage )
{
    return true;
}

bool D3D11Renderer::AllocTextureContent( void* p_texturedata )
{
    return true;
}

void D3D11Renderer::ReleaseTextureContent( void* p_texturedata )
{

}

void* D3D11Renderer::GetTextureContentPtr( void* p_texturedata )
{
    return NULL;
}

bool D3D11Renderer::CopyTextureContent( void* p_texturedata )
{
    return true;
}

bool D3D11Renderer::UpdateTextureContent( void* p_texturedata )
{
    return true;
}

bool D3D11Renderer::CreateShaders( DrawSpace::Core::Fx* p_fx, void** p_data )
{
    return true;
}

bool D3D11Renderer::SetShaders( void* p_data )
{
    return true;
}

bool D3D11Renderer::CreateRenderStatesSet( DrawSpace::Core::Fx* p_fx, void** p_data )
{
    return true;
}

bool D3D11Renderer::ApplyRenderStatesIn( void* p_data )
{
    return true;
}

bool D3D11Renderer::ApplyRenderStatesOut( void* p_data )
{
    return true;
}

bool D3D11Renderer::SetFxShaderParams( int p_shader_index, long p_register, DrawSpace::Utils::Vector& p_vector )
{
    return true;
}

bool D3D11Renderer::SetFxShaderMatrix( int p_shader_index, long p_register, DrawSpace::Utils::Matrix& p_mat )
{
    return true;
}

bool D3D11Renderer::DrawMeshe( DrawSpace::Utils::Matrix p_world, DrawSpace::Utils::Matrix p_view, DrawSpace::Utils::Matrix p_proj )
{
    return true;
}

void D3D11Renderer::SetRenderState( DrawSpace::Core::RenderState* p_renderstate )
{

}

void D3D11Renderer::GetRenderCharacteristics( Characteristics& p_characteristics )
{

}

void D3D11Renderer::DrawText( long p_r, long p_g, long p_b, int p_posX, int p_posY, const char* p_format, ... )
{
    char buffer[512];
    _vsnprintf( buffer, 512, p_format, (va_list)( &p_format + 1 ) );

    unsigned long color32 = (((0xff)<<24)|(((p_b)&0xff)<<16)|(((p_g)&0xff)<<8)|((p_r)&0xff));

    m_fontWrapper->DrawString(
		m_lpd3ddevcontext,
        DrawSpace::Utils::String2WString( std::string( buffer ) ).c_str(),
		15.0f,// Font size
		p_posX,// X position
		p_posY,// Y position
		color32,// Text color, 0xAaBbGgRr
		0// Flags
	);
}

void D3D11Renderer::PointProjection( DrawSpace::Utils::Matrix p_view, DrawSpace::Utils::Matrix p_proj, DrawSpace::Utils::Vector& p_point, dsreal& p_outx, dsreal& p_outy, dsreal& p_outz )
{

}


bool D3D11Renderer::Config::on_new_line( const dsstring& p_line, long p_line_num, std::vector<dsstring>& p_words )
{
    if( 2 == p_words.size() )
    {
        if( "dx11adapterordinal" == p_words[0] )
        {
            m_adapter_ordinal = atoi( p_words[1].c_str() );
        }
    }
    if( 5 == p_words.size() )
    {
        if( "dx9fullscreen" == p_words[0] )
        {
            m_fullscreen_width = atoi( p_words[1].c_str() );
            m_fullscreen_height = atoi( p_words[2].c_str() );
            m_refreshrate = atoi( p_words[3].c_str() );
            m_fullscreen_format = (DXGI_FORMAT)atoi( p_words[4].c_str() );
        }
    }

    return true;
}

