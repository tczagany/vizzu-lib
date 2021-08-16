#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include <unordered_map>

#include "chart/main/version.h"
#include "chart/ui/chart.h"

namespace Vizzu
{

class Interface
{
public:
	static Interface instance;

	Interface();
	const char *version() const;
	void keyPress(int key, bool ctrl, bool alt, bool shift);
	void mouseMove(double x, double y);
	void mouseDown(double x, double y);
	void mouseUp(double x, double y);
	void update(double scale, double width, double height, bool force);
	void poll();

	void *createChart();

private:
	std::string versionStr;
	std::shared_ptr<GUI::TaskQueue> taskQueue;
	std::unordered_map<void*, std::shared_ptr<UI::ChartWidget>> charts;
	bool needsUpdate;
};

}

#endif
