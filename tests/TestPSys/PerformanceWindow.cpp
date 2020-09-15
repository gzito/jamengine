
#include "PerformanceWindow.h"

#include <cstdio>


#include "jam/Application.h"
#include "jam/SysTimer.h"
#include "jam/thirdparty/imgui/imgui.h"

namespace {
    const int kNumSamples = 100;
    const uint64_t kSampleRate = 30;
} // namespace


void PerformanceWindow::DrawContent() {
    static 	bool showImguiWindow = true;

    ImGui::Begin("PSYS Info Window", &showImguiWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)


    DrawFpsPlot();

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("Close Me")) {

        showImguiWindow = false;
    }
    ImGui::End();


}

void PerformanceWindow::DrawFpsPlot() {
    const auto current = jam::GetSysTimer().getTime();
    const ImGuiStyle* const style = &ImGui::GetStyle();
    const float paddingX = style->WindowPadding.x;
    static float maxFPS = 75.0f;
    static float fps[kNumSamples] = { 0 };
    static uint64_t fpsLastSample = 0;
    static char fpsTextBuffer[64];
    static float currentFPS = 0.0f;
    if (fpsLastSample + kSampleRate < current) {
        fpsLastSample = current;
        for (int i = 0; i < kNumSamples - 1; ++i) {
            fps[i] = fps[i + 1];
        }
        const auto instantFPS =static_cast<float>(jam::GetAppMgr().getFps());
        currentFPS = (instantFPS > 0.0f && instantFPS<2000.0f) ? instantFPS : currentFPS;
        fps[kNumSamples - 1] = instantFPS;
        sprintf(fpsTextBuffer, "FPS: %d (%0.2f)", static_cast<int>(fps[kNumSamples - 1]), currentFPS);
        maxFPS = std::max<float>(currentFPS * 1.25f, maxFPS);
      
    }

 

    ImGui::Text("%s", fpsTextBuffer);
    ImGui::PlotLines("", fps, kNumSamples, 0, "", 0.0f, maxFPS, ImVec2(400 - 2 * paddingX, 80));
}
