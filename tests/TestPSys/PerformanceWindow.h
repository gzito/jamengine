#pragma once

#include <vector>
#include <string>

class PerformanceWindow {

public:
	static void DrawContent();
	static void ShowOtherInfo(	int partType, const std::vector<std::string>&	m_partname);
private:
	static void DrawFpsPlot();
	
	
};
