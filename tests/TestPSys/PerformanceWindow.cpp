
#include "PerformanceWindow.h"

#include <cstdio>

#include <PSys.h>
#include "jam/Application.h"
#include "jam/Draw3dManager.h"
#include "jam/Gfx.h"
#include "jam/SysTimer.h"
#include "jam/thirdparty/imgui/imgui.h"

#include <vector>

namespace {
	const int kNumSamples = 100;
	const uint64_t kSampleRate = 30;
} // namespace


void PerformanceWindow::DrawContent() {
	static 	bool showImguiWindow = true;

	
	DrawFpsPlot();

	ImGui::Spacing();
	ImGui::Separator();

	if(ImGui::Button("Close Me")) {

		showImguiWindow = false;
	}
	


}

void PerformanceWindow::ShowOtherInfo(int partType,const std::vector<std::string>& partname)
{
	static char buffer[256]={};
	const std::string name = partname[partType];
	sprintf(buffer,"%s %d/%zd",name.c_str(),partType+1,partname.size()) ;
	ImGui::Text(buffer) ;

	/*if (lastOptimized < GetParticleSystem().getOptimized())
	{
		//m_txtMgr.addText("Optimized "+itos(GetParticleSystem().getOptimized()),CX3D(0),CY3D_BOTTOM_SCREEN(he*2),1000,1 ) ;
		lastOptimized=GetParticleSystem().getOptimized();
	}*/

	sprintf(buffer,"FPS: %0.2f",jam::GetAppMgr().getFps());
	ImGui::Text(buffer) ;
	sprintf(buffer,"Particles: %d",GetParticleSystem().totalParticles());
	ImGui::Text(buffer) ;

	sprintf(buffer,"Emitters: %d",GetParticleSystem().totalEmitters());
	ImGui::Text(buffer) ;

	sprintf(buffer,"Coords: %d,%d",jam::Draw3DManager::MouseX3D,jam::Draw3DManager::MouseY3D) ;
	ImGui::Text(buffer) ;

	sprintf(buffer,"Batching: %s",jam::GetGfx().getBatch() != nullptr ? "ON" : "OFF");
	ImGui::Text(buffer) ;
}


void PerformanceWindow::DrawFpsPlot() {
	const auto current = jam::GetSysTimer().getTime();
	const ImGuiStyle* const style = &ImGui::GetStyle();
	const float paddingX = style->WindowPadding.x;
	static float maxFPS = 75.0f;
	static float fps[kNumSamples] ={0};
	static uint64_t fpsLastSample = 0;
	static char fpsTextBuffer[64];
	static float currentFPS = 0.0f;
	if(fpsLastSample + kSampleRate < current) {
		fpsLastSample = current;
		for(int i = 0; i < kNumSamples - 1; ++i) {
			fps[i] = fps[i + 1];
		}
		const auto instantFPS =static_cast<float>(jam::GetAppMgr().getFps());
		currentFPS = (instantFPS > 0.0f && instantFPS<2000.0f) ? instantFPS : currentFPS;
		fps[kNumSamples - 1] = instantFPS;
		sprintf(fpsTextBuffer,"FPS: %d (%0.2f)",static_cast<int>(fps[kNumSamples - 1]),currentFPS);
		maxFPS = std::max<float>(currentFPS * 1.25f,maxFPS);

	}



	ImGui::Text("%s",fpsTextBuffer);
	ImGui::PlotLines("",fps,kNumSamples,0,"",0.0f,maxFPS,ImVec2(400 - 2 * paddingX,80));
}
