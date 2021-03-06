﻿/*
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
m_inputLayout( NULL ),
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

    /*
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_9_1,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );
    */

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

    m_characteristics.fullscreen = false;
    m_characteristics.width_resol = p_w_width;
    m_characteristics.height_resol = p_w_height;

    ///////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* backBuffer;
    hRes = m_lpd3dswapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    D3D11_CHECK( GetBuffer )

    hRes = m_lpd3ddevice->CreateRenderTargetView( backBuffer, NULL, &m_screentarget );
    D3D11_CHECK( CreateRenderTargetView )

    backBuffer->Release();

    ////////////////////////////////////////////////////////////////////////

    //projection set, to automatically fit with the screen resolution
    float v_width, v_height;
    if( p_fullscreen )
    {
        v_width = 1.0;
        v_height = v_width * m_config.m_fullscreen_height / m_config.m_fullscreen_width;
    }
    else
    {
        v_width = 1.0;
        v_height = v_width * p_w_height / p_w_width;
    }

	_DSDEBUG( logger, dsstring("projection : v_width = ") << v_width << dsstring( " v_height = " ) << v_height );

    m_characteristics.width_viewport = v_width;
    m_characteristics.height_viewport = v_height;

    //////////////////////////////////////////////////////////////////////////

	IFW1Factory* fW1Factory;
	hRes = FW1CreateFactory( FW1_VERSION, &fW1Factory );
	
	hRes = fW1Factory->CreateFontWrapper( m_lpd3ddevice, L"System", &m_fontWrapper );

    //////////////////////////////////////////////////////////////////////////

    m_lpd3ddevcontext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hRes = m_lpd3ddevice->CreateSamplerState( &sampDesc, &m_samplerState );
    D3D11_CHECK( CreateSamplerState )


    ID3D11SamplerState* ss_array[1] = { m_samplerState };

    // a mettre en option dans l'interface renderer ?
    for( long i = 0; i < 8; i++ )
    {
        m_lpd3ddevcontext->VSSetSamplers( i, 1, ss_array );
        m_lpd3ddevcontext->PSSetSamplers( i, 1, ss_array );
    }


    ////////////////////////////////////////////////////////////////////////////


    D3D11_RASTERIZER_DESC rsDesc;

    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_BACK;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthBias = 0;
    rsDesc.SlopeScaledDepthBias = 0.0f;
    rsDesc.DepthBiasClamp = 0.0f;
    rsDesc.DepthClipEnable = TRUE;
    rsDesc.ScissorEnable = FALSE;
    rsDesc.MultisampleEnable = FALSE;
    rsDesc.AntialiasedLineEnable = FALSE;
    
    hRes = m_lpd3ddevice->CreateRasterizerState( &rsDesc, &m_rsState );
    D3D11_CHECK( CreateRasterizerState )

    m_lpd3ddevcontext->RSSetState( m_rsState );



    ID3D11RasterizerState* curr_rs;
    D3D11_RASTERIZER_DESC curr_rsDesc;

    m_lpd3ddevcontext->RSGetState( &curr_rs );

    curr_rs->GetDesc( &curr_rsDesc );



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
    MesheData* meshe_data = (MesheData*)p_data;

    UINT stride = sizeof( d3d11vertex );
    UINT offset = 0;
    m_lpd3ddevcontext->IASetVertexBuffers( 0, 1, &meshe_data->vertex_buffer, &stride, &offset ); 
    m_lpd3ddevcontext->IASetIndexBuffer( meshe_data->index_buffer, DXGI_FORMAT_R32_UINT, 0 );

    m_next_nbvertices = meshe_data->nb_vertices;
    m_next_nbtriangles = meshe_data->nb_triangles;

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
    DECLARE_D3D11ASSERT_VARS
    ID3D11Texture2D*	    d3dt11;
    TextureInfos*           texture_infos;

    dsstring path;
    p_texture->GetPath( path );

    if( m_textures_base.count( path ) > 0 )
    {
        *p_data = (void*)m_textures_base[path];
        
        long width = m_textures_base[path]->descr.Width;
        long height = m_textures_base[path]->descr.Height;
        long bpp;

        switch( m_textures_base[path]->descr.Format )
        {
            case DXGI_FORMAT_R8G8B8A8_TYPELESS:
            case DXGI_FORMAT_R8G8B8A8_UNORM:
            case DXGI_FORMAT_R8G8B8A8_UINT:
            case DXGI_FORMAT_R8G8B8A8_SNORM:
            case DXGI_FORMAT_R8G8B8A8_SINT:
                bpp = 4;
                break;

            case DXGI_FORMAT_R16_FLOAT:
                bpp = 2;
                break;

            case DXGI_FORMAT_R32_FLOAT:
                bpp = 4;
                break;

            case DXGI_FORMAT_R32G32B32A32_FLOAT:
                bpp = 16;
                break;

            default:
                bpp = -1;
                break;
        }

        // inutile, puisque cette texture est deja "passee" par ici...
        
        //p_texture->SetFormat( width, height, bpp );
        //p_texture->SetRenderData( (void*)m_textures_base[path] );
        
        return true;
    }

    bool setformat_call = true; // si true, appel SetFormat() sur la texture

    if( p_texture->IsRenderTarget() )
    {
        unsigned long rw, rh;
        p_texture->GetRenderTargetDims( rw, rh );

        DXGI_FORMAT format;

        int bpp = 0;
        switch( p_texture->GetRenderPurpose() )
        {
            case Texture::RENDERPURPOSE_COLOR:

                //format = DXGI_FORMAT_R8G8B8A8_UNORM;
                format = DXGI_FORMAT_R8G8B8A8_UINT;
                bpp = 4;
                break;

            case Texture::RENDERPURPOSE_FLOAT:

                format = DXGI_FORMAT_R16_FLOAT;
                bpp = 2;
                break;

            case Texture::RENDERPURPOSE_FLOAT32:

                format = DXGI_FORMAT_R32_FLOAT;
                bpp = 4;
                break;

            case Texture::RENDERPURPOSE_FLOATVECTOR:

                format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                bpp = 8;
                break;
        
        }

        D3D11_TEXTURE2D_DESC textureDesc;

        // Initialize the render target texture description.
        ZeroMemory( &textureDesc, sizeof( textureDesc ) );

        // Setup the render target texture description.
        textureDesc.Width = rw;
        textureDesc.Height = rh;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = format;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        // Create the render target texture.
        hRes = m_lpd3ddevice->CreateTexture2D( &textureDesc, NULL, &d3dt11 );
        D3D11_CHECK( CreateTexture2D )

        // creation du render target view
        D3D11_RENDER_TARGET_VIEW_DESC       renderTargetViewDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC     shaderResourceViewDesc;

        ID3D11RenderTargetView*             rendertextureTargetView;
        ID3D11ShaderResourceView*           rendertextureResourceView;


        ZeroMemory( &renderTargetViewDesc, sizeof( renderTargetViewDesc ) );

	    renderTargetViewDesc.Format = format;
	    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	    renderTargetViewDesc.Texture2D.MipSlice = 0;

	    hRes = m_lpd3ddevice->CreateRenderTargetView( d3dt11, &renderTargetViewDesc, &rendertextureTargetView );
        D3D11_CHECK( CreateRenderTargetView )

        // creation du shader resource view associé 
	    shaderResourceViewDesc.Format = format;
	    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	    shaderResourceViewDesc.Texture2D.MipLevels = 1;

	    hRes = m_lpd3ddevice->CreateShaderResourceView( d3dt11, &shaderResourceViewDesc, &rendertextureResourceView );
        D3D11_CHECK( CreateShaderResourceView )

        D3D11_TEXTURE2D_DESC descr;
        d3dt11->GetDesc( &descr );

        texture_infos = _DRAWSPACE_NEW_( TextureInfos, TextureInfos );
        texture_infos->path = path;
        texture_infos->texture_instance = p_texture;
        texture_infos->texture = d3dt11;
        texture_infos->descr = descr;
        texture_infos->textureShaderResourceView = rendertextureResourceView;
        texture_infos->rendertextureTargetView = rendertextureTargetView;

        m_textures_base[path] = texture_infos;

        *p_data = (void*)texture_infos;
    }
    else
    {
        if( p_texture->GetPurpose() == Texture::PURPOSE_COLORFROMFILE )
        {
            ID3D11ShaderResourceView*           textureResourceView;

            void* data = p_texture->GetData();
            size_t data_size = p_texture->GetDataSize();

            hRes = D3DX11CreateShaderResourceViewFromMemory( m_lpd3ddevice, data, data_size, NULL, NULL, &textureResourceView, NULL );
            D3D11_CHECK( D3DX11CreateShaderResourceViewFromMemory )

            texture_infos = _DRAWSPACE_NEW_( TextureInfos, TextureInfos );
            texture_infos->path = path;
            texture_infos->texture_instance = p_texture;
            texture_infos->texture = NULL;
            texture_infos->rendertextureTargetView = NULL;
            texture_infos->textureShaderResourceView = textureResourceView;

            m_textures_base[path] = texture_infos;

            *p_data = (void*)texture_infos;
        
            setformat_call = false; // ici on ne dispose pas des infos description texture
        }
        else
        {
            DXGI_FORMAT format;

            switch( p_texture->GetPurpose() )
            {
                case Texture::PURPOSE_COLOR:
                    {
                        format = DXGI_FORMAT_R8G8B8A8_UINT;
                    }
                    break;

                case Texture::PURPOSE_FLOAT32:
                    {
                        format = DXGI_FORMAT_R32_FLOAT;
                    }
                    break;

                case Texture::PURPOSE_FLOAT32VECTOR:
                    {
                        format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    }
                    break;
            }

            long w, h, bpp;
            p_texture->GetFormat( w, h, bpp );

            D3D11_TEXTURE2D_DESC desc;
            desc.Width = w;
            desc.Height = h;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = format;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            hRes = m_lpd3ddevice->CreateTexture2D( &desc, NULL, &d3dt11 );

            D3D11_CHECK( CreateTexture2D )

            texture_infos = _DRAWSPACE_NEW_( TextureInfos, TextureInfos );
            texture_infos->path = path;
            texture_infos->texture_instance = p_texture;
            texture_infos->texture = d3dt11;
            texture_infos->rendertextureTargetView = NULL;

            D3D11_TEXTURE2D_DESC descr;
            d3dt11->GetDesc( &descr );

            texture_infos->descr = descr;

            D3D11_SHADER_RESOURCE_VIEW_DESC     shaderResourceViewDesc;
            ID3D11ShaderResourceView*           textureResourceView;

            // creation du shader resource view associé 
	        shaderResourceViewDesc.Format = format;
	        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	        shaderResourceViewDesc.Texture2D.MipLevels = 1;

	        hRes = m_lpd3ddevice->CreateShaderResourceView( d3dt11, &shaderResourceViewDesc, &textureResourceView );
            D3D11_CHECK( CreateShaderResourceView )

            texture_infos->textureShaderResourceView = textureResourceView;

            *p_data = (void*)texture_infos;
                   
            // inutile d'appeler SetFormat() sur la texture
            setformat_call = false;
        }

    }


    if( setformat_call )
    {
        long width = texture_infos->descr.Width;
        long height = texture_infos->descr.Height;
        long bpp;

        switch( texture_infos->descr.Format )
        {
            case DXGI_FORMAT_R8G8B8A8_TYPELESS:
            case DXGI_FORMAT_R8G8B8A8_UNORM:
            case DXGI_FORMAT_R8G8B8A8_UINT:
            case DXGI_FORMAT_R8G8B8A8_SNORM:
            case DXGI_FORMAT_R8G8B8A8_SINT:
                bpp = 4;
                break;

            case DXGI_FORMAT_R16_FLOAT:
                bpp = 2;
                break;

            case DXGI_FORMAT_R32_FLOAT:
                bpp = 4;
                break;

            case DXGI_FORMAT_R32G32B32A32_FLOAT:
                bpp = 16;
                break;

            case DXGI_FORMAT_R16G16B16A16_FLOAT:
                bpp = 8;
                break;

            default:
                bpp = -1;
                break;
        }

        p_texture->SetFormat( width, height, bpp );
    }

    p_texture->SetRenderData( (void*)texture_infos );

    return true;
}

void D3D11Renderer::DestroyTexture( void* p_data )
{

}

bool D3D11Renderer::SetTexture( void* p_data, int p_stage )
{
    TextureInfos* ti = (TextureInfos*)p_data;
    m_lpd3ddevcontext->PSSetShaderResources( p_stage, 1, &ti->textureShaderResourceView );

    return true;
}

bool D3D11Renderer::SetVertexTexture( void* p_data, int p_stage )
{
    TextureInfos* ti = (TextureInfos*)p_data;
    m_lpd3ddevcontext->VSSetShaderResources( p_stage, 1, &ti->textureShaderResourceView );

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
    DECLARE_D3D11ASSERT_VARS

    dsstring hash;
    p_fx->GetShadersMD5( hash );

    if( m_shaders_bases.count( hash ) > 0 )
    {
        *p_data = (void *)m_shaders_bases[hash];
        return true;
    }

    ShadersData* sdata = _DRAWSPACE_NEW_( ShadersData, ShadersData );

    if( p_fx->GetShadersListSize() )
    {
        ID3D11VertexShader*         vs;
        ID3D11PixelShader*          ps;

        /////////////////// Shaders loading

        Core::Shader* vertex_shader = p_fx->GetShader( 0 );
        Core::Shader* pixel_shader = p_fx->GetShader( 1 );

        dsstring vshader_path, pshader_path;

        vertex_shader->GetPath( vshader_path );
        pixel_shader->GetPath( pshader_path );

        if( !vertex_shader->IsCompiled() )
        {
            if( NULL == vertex_shader->GetData() )
            {
                _DSFATAL( logger, "no data in vertex shader !" )
                return false;
            }

            ID3DBlob* pVSBlob = NULL;
            ID3DBlob* pVSErrBlob;
            hRes = compile_shader_from_file( vertex_shader->GetData(), vertex_shader->GetDataSize(), vshader_path.c_str(), "vs_main", "vs_4_0", &pVSBlob, &pVSErrBlob );

            if( S_OK != hRes )
            {
                if( pVSErrBlob != NULL )
                {
					_DSFATAL( logger, dsstring( "D3DXCompileShader FAIL : " ) << (char *)pVSErrBlob->GetBufferPointer() )
                    _DSEXCEPTION( "D3DXCompileShader FAIL (vertex) : " << dsstring( (char *)pVSErrBlob->GetBufferPointer() ) )

                    pVSErrBlob->Release();
                }
                return false;
            }

            hRes = m_lpd3ddevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vs );
            D3D11_CHECK( CreateVertexShader );

            if( NULL == m_inputLayout )
            {
                //////////////////////// au premier vshader créé, instancier et setter l'objet inputLayout

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

                hRes = m_lpd3ddevice->CreateInputLayout( layout, 11, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_inputLayout );
                D3D11_CHECK( CreateInputLayout )

                m_lpd3ddevcontext->IASetInputLayout( m_inputLayout );                
            }
        }
        else
        {
            hRes = m_lpd3ddevice->CreateVertexShader( vertex_shader->GetData(), vertex_shader->GetDataSize(), NULL, &vs );
            D3D11_CHECK( CreateVertexShader );
        }

        sdata->vertex_shader = vs; 

        if( !pixel_shader->IsCompiled() )
        {
            if( NULL == pixel_shader->GetData() )
            {
                _DSFATAL( logger, "no data in pixel shader !" )
                return false;
            }


            ID3DBlob* pPSBlob = NULL;
            ID3DBlob* pPSErrBlob;
            hRes = compile_shader_from_file( pixel_shader->GetData(), pixel_shader->GetDataSize(), pshader_path.c_str(), "ps_main", "ps_4_0", &pPSBlob, &pPSErrBlob );

            if( S_OK != hRes )
            {
                if( pPSErrBlob != NULL )
                {
					_DSFATAL( logger, dsstring( "D3DXCompileShader FAIL : " ) << (char *)pPSErrBlob->GetBufferPointer() )
                    _DSEXCEPTION( "D3DXCompileShader FAIL (vertex) : " << dsstring( (char *)pPSErrBlob->GetBufferPointer() ) )

                    pPSErrBlob->Release();
                }
                return false;  
            }

            hRes = m_lpd3ddevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &ps );
            D3D11_CHECK( CreatePixelShader ); 
        }
        else
        {
            hRes = m_lpd3ddevice->CreatePixelShader( pixel_shader->GetData(), pixel_shader->GetDataSize(), NULL, &ps );
            D3D11_CHECK( CreatePixelShader );       
        }

        sdata->pixel_shader = ps;
    }

    *p_data = (void*)sdata;

    m_shaders_bases[hash] = sdata;

    return true;

}

bool D3D11Renderer::SetShaders( void* p_data )
{
	ShadersData* sdata = (ShadersData*)p_data;

    m_lpd3ddevcontext->VSSetShader( sdata->vertex_shader, NULL, 0 );
    m_lpd3ddevcontext->PSSetShader( sdata->pixel_shader, NULL, 0 );

    return true;
}

bool D3D11Renderer::CreateRenderStatesSet( DrawSpace::Core::Fx* p_fx, void** p_data )
{
    dsstring hash;
    p_fx->GetRenderStatesSetMD5( hash );

    if( m_fx_bases.count( hash ) > 0 )
    {
        *p_data = (void *)m_fx_bases[hash];
        return true;
    }

    *p_data = (void*)p_fx;
    m_fx_bases[hash] = p_fx;

    return true;
}

bool D3D11Renderer::ApplyRenderStatesIn( void* p_data )
{
	DrawSpace::Core::Fx* fx = (DrawSpace::Core::Fx*)p_data;

    long nb_rs_in = fx->GetRenderStatesInListSize();
    for( long i = 0; i < nb_rs_in; i++ )
    {
        DrawSpace::Core::RenderState rs = fx->GetRenderStateIn( i );
        SetRenderState( &rs );
    }
    return true;
}

bool D3D11Renderer::ApplyRenderStatesOut( void* p_data )
{
	DrawSpace::Core::Fx* fx = (DrawSpace::Core::Fx*)p_data;

    long nb_rs_out = fx->GetRenderStatesOutListSize();
    for( long i = 0; i < nb_rs_out; i++ )
    {
        DrawSpace::Core::RenderState rs = fx->GetRenderStateOut( i );
        SetRenderState( &rs );
    }
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
    dsstring arg;
    p_renderstate->GetArg( arg );

    switch( p_renderstate->GetOperation() )
    {
        case DrawSpace::Core::RenderState::SETCULLING:
            
            if( "none" == arg )
            {

            }
            else if( "cw" == arg )
            {

            }
            else
            {

            }
            break;

        case DrawSpace::Core::RenderState::ENABLEZBUFFER:
            if( "true" == arg )
            {

            }
            else
            {

            }
            break;

        case DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE:
            {

                if( "none" == arg )
                {

                }
                else if( "point" == arg )
                {

                }
                else if( "linear" == arg )
                {

                }
                else if( "anisotropic" == arg )
                {

                }

                for( long i = 0; i < 8; i++ )
                {

                }
            }
            break;


        case DrawSpace::Core::RenderState::SETVERTEXTEXTUREFILTERTYPE:
            {


                if( "none" == arg )
                {

                }
                else if( "point" == arg )
                {

                }
                else if( "linear" == arg )
                {

                }
                else if( "anisotropic" == arg )
                {

                }

                for( long i = 0; i < 8; i++ )
                {

                }
            }
            break;

        case DrawSpace::Core::RenderState::SETFILLMODE:
            {

                if( "point" == arg )
                {

                }
                else if( "line" == arg )
                {
  
                }
                else if( "solid" == arg )
                {
   
                }
   
            }
            break;

        case DrawSpace::Core::RenderState::ALPHABLENDENABLE:
            {
                if( "true" == arg )
                {

                }
                else if( "false" == arg )
                {

                }
            }
            break;

        case DrawSpace::Core::RenderState::ALPHABLENDOP:
            {
                if( "add" == arg )
                {

                }
                else if( "sub" == arg )
                {
 
                }
                else if( "revsub" == arg )
                {

                }
                else if( "min" == arg )
                {

                }
                else if( "max" == arg )
                {

                }
            }
            break;

        case DrawSpace::Core::RenderState::ALPHABLENDFUNC:
            {
                if( "never" == arg )
                {

                }
                else if( "less" == arg )
                {

                }
                else if( "equal" == arg )
                {

                }
                else if( "lessequal" == arg )
                {

                }
                else if( "greater" == arg )
                {
   
                }
                else if( "notequal" == arg )
                {
      
                }
                else if( "greaterequal" == arg )
                {

                }
                else if( "always" == arg )
                {

                }
            }
            break;

        case DrawSpace::Core::RenderState::ALPHABLENDDEST:
            {
                if( "zero" == arg )
                {

                }
                else if( "one" == arg )
                {

                }
                else if( "srccolor" == arg )
                {
 
                }
                else if( "invsrccolor" == arg )
                {

                }
                else if( "srcalpha" == arg )
                {
    
                }
                else if( "invsrcalpha" == arg )
                {
      
                }
                else if( "destalpha" == arg )
                {
   
                }
                else if( "invdestalpha" == arg )
                {
    
                }
                else if( "destcolor" == arg )
                {
 
                }
                else if( "invdestcolor" == arg )
                {

                }
            }
            break;

        case DrawSpace::Core::RenderState::ALPHABLENDSRC:

                if( "zero" == arg )
                {
  
                }
                else if( "one" == arg )
                {

                }
                else if( "srccolor" == arg )
                {

                }
                else if( "invsrccolor" == arg )
                {

                }
                else if( "srcalpha" == arg )
                {
       
                }
                else if( "invsrcalpha" == arg )
                {
    
                }
                else if( "destalpha" == arg )
                {
    
                }
                else if( "invdestalpha" == arg )
                {
   
                }
                else if( "destcolor" == arg )
                {
 
                }
                else if( "invdestcolor" == arg )
                {
 
                }

            break;

    }
}

void D3D11Renderer::GetRenderCharacteristics( Characteristics& p_characteristics )
{
    p_characteristics = m_characteristics;
    if( !m_characteristics.fullscreen )
    {
        // prendre en compte les bords fenetre

        RECT rect;
        GetClientRect( m_hwnd, &rect );

        p_characteristics.width_resol = rect.right;
        p_characteristics.height_resol = rect.bottom;
    }
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

HRESULT D3D11Renderer::compile_shader_from_file( void* p_data, int p_size, LPCTSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut, ID3DBlob** ppBlobErrOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
/*
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    */
    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromMemory( (LPCTSTR)p_data, p_size, szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {


        if( pErrorBlob != NULL )
        {
            *ppBlobErrOut = pErrorBlob;
        }

        return hr;
    }
    return S_OK;
}