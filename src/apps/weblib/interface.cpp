#include "interface.h"

#include <sstream>
#include <span>

#include "base/io/log.h"
#include "base/io/memorystream.h"
#include "jscriptcanvas.h"

extern "C" {
	extern void setMouseCursor(const char *cursor);
}

using namespace Vizzu;

Interface Interface::instance;

Interface::Interface() : versionStr(std::string(Main::version))
{
	needsUpdate = false;
}

const char *Interface::version() const
{
	return versionStr.c_str();
}

void *Interface::createChart()
{
	auto chart = std::make_shared<UI::ChartWidget>();
	chart->doChange = [&]{ needsUpdate = true; };
	chart->setMouseCursor = [&](GUI::Cursor cursor) {
		::setMouseCursor(GUI::toCSS(cursor));
	};
	charts.emplace(chart.get(), chart);
	return chart.get();
}

void Interface::poll()
{
	if (taskQueue) taskQueue->poll();
}

void Interface::update(double, double width, double height, bool force)
{
	for (auto chart: charts)
	{
		auto now = std::chrono::steady_clock::now();
		chart.second->getChart().getAnimControl().update(now);

		if (needsUpdate || force) {
			try {
				Geom::Size size(width, height);
				Vizzu::Main::JScriptCanvas canvas;
				canvas.frameBegin();
				chart.second->updateSize(canvas, size);
				chart.second->draw(canvas);
				canvas.frameEnd();
			}
			catch (std::exception &e) {
				IO::log() << "error" << e.what();
			}
			needsUpdate = false;
		}
	}
}

void Interface::mouseDown(double x, double y)
{
	for (auto chart: charts)
	{
		chart.second->onMouseDown(Geom::Point(x, y));
		needsUpdate = true;
	}
}

void Interface::mouseUp(double x, double y)
{
	for (auto chart: charts)
	{
		chart.second->onMouseUp(Geom::Point(x, y), GUI::DragObjectPtr());
		needsUpdate = true;
	}
}

void Interface::mouseMove(double x, double y)
{
	for (auto chart: charts)
	{
		GUI::DragObjectPtr nodrag;
		chart.second->onMouseMove(Geom::Point(x, y), nodrag);
		needsUpdate = true;
	}
}

void Interface::keyPress(int key, bool ctrl, bool alt, bool shift)
{
	for (auto chart: charts)
	{
		GUI::KeyModifiers keyModifiers(shift, ctrl, alt);
		chart.second->onKeyPress(GUI::Key(key), keyModifiers);
		needsUpdate = true;
	}
}
