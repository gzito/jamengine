/**********************************************************************************
* 
* TestImgui.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2020 Giovanni Zito, Gianluca Sclano
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* 
**********************************************************************************/

#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/InputManager.h>
#include <jam/Camera.h>
#include <jam/Texture2D.h>
#include <jam/Scene.h>
#include <jam/SpriteBatch.h>
#include <jam/DrawItemManager.h>
#include <jam/core/geom.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <stdio.h>

using namespace jam;

class Entity : public RefCountedObject
{
public:
							Entity() = default ;

	Vector2					m_position ;
	Color					m_color ;
	Vector2					m_velocity ;
	float					m_direction ;
	bool					m_directionInited = false ;
	int						m_count = 0;

	static AABB				m_bounds ;

	void					moveRandomly() ;

private:
							Entity( const Entity& ) = delete ;
	Entity&					operator=( const Entity& ) = delete ;

protected:
							~Entity() = default ;
};

AABB Entity::m_bounds ;

void Entity::moveRandomly()
{
	// only the first time
	if( !m_directionInited ) {
		m_direction = RANGERANDF( 0, JAM_TWOPI ) ;
		m_directionInited = true ;
		m_count = 0 ;
	}

	if( m_count == 0 ) {
		m_direction += RANGERANDF( -0.1f, 0.1f ) ;
		m_direction = WrapAngleRadian(m_direction) ;
		if( !m_bounds.contains(m_position) ) {
			m_direction = -m_direction ;
			m_velocity = -m_velocity ;
		}
	}

	m_velocity = m_velocity + fromPolar( ToDegree(m_direction),0.4f) ;
	
	m_position += m_velocity ;

	if( ++m_count > 5 ) {
		m_count = 0 ;
	}
}



class TestImguiApp : public jam::Application
{
public:
							TestImguiApp() = default ;
	virtual					~TestImguiApp() = default ;

protected:
	virtual bool			init() ;
	virtual void			render() ;
	virtual void			destroy() ;

private:
	Texture2D*				m_pTexture = nullptr ;
	Texture2D*				m_pFontTex= nullptr ;
	SpriteBatch*			m_pSpriteBatch= nullptr ;

#ifdef APP_USE_IMGUI
    bool					m_show_demo_window = true;
	bool					m_show_another_window = true ;
#endif

	std::vector<Ref<Entity>>	m_entities ;

private:
};

// #define APP_USE_IMGUI

//*************************************************************************
bool TestImguiApp::init()
{
	Draw3DManager::Origin3D(960,540);

	//m_pFontTex = GetDraw3DMgr().LoadFont3D( "media/Babylon1.png" ) ;

	m_pTexture = GetDrawItemMgr().loadTexture("media/star.png","psys")->getTexture() ;
	m_pSpriteBatch = new SpriteBatch(300) ;

	// set camera
	Camera* pCamera = new Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;
	
	Entity::m_bounds = AABB(0,0,Draw3DManager::VPWidth/2,Draw3DManager::VPHeight/2) ;
	for( int i=0; i<200; i++ ) {
		Vector2 position( RANGERAND(-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2) , RANGERAND(-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2) ) ;
		Color color( RND(256), RND(256), RND(256) ) ;

		Ref<Entity> rEntity( new Entity() ) ;
		rEntity->m_position = position ;
		rEntity->m_color = color ;
	
		m_entities.push_back(rEntity) ;
	}
	
	return true ;
}


void TestImguiApp::render()
{
#ifdef APP_USE_IMGUI

	// draw imgui

	ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(GetAppMgr().getWindowPtr());
    ImGui::NewFrame();
	
	ImGui::ShowDemoWindow(&m_show_demo_window);

    if (m_show_another_window)
    {
        ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me")) { 
			m_show_another_window = false;
		}
        ImGui::End();
    }
	
	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#endif

	// draw sprites

	m_pSpriteBatch->Begin() ;

	for( int i=0; i<m_entities.size(); i++ )
	{
		m_entities[i]->moveRandomly() ;
		m_pSpriteBatch->Draw( m_pTexture, m_entities[i]->m_position, nullptr, m_entities[i]->m_color, 0,
							 Vector2(0,0), Vector2(1.0f,-1.0f), SpriteEffects::None, 0 ) ;
	}

	m_pSpriteBatch->End(); 

}


void TestImguiApp::destroy()
{
	JAM_RELEASE( m_pSpriteBatch ) ;
}


//*************************************************************************

int main( int argc, char* argv[])
{
	try {
		TestImguiApp app  ;
#ifdef APP_USE_IMGUI
		app.setImguiEnabled() ;
#endif
		app.start();
	}
	catch( std::exception& ex )	{
		JAM_TRACE( "Exception thrown:\n%s\n", ex.what() ) ;
		printf( "Exception thrown:\n%s\n", ex.what() ) ; 
	}
	catch(...) {
		JAM_TRACE( "Unknown exception thrown!\n" ) ;
		printf( "Unknown exception thrown!\n" ) ; 
	}
	return 0 ;
}
