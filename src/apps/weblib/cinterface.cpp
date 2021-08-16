#include "cinterface.h"
#include "interface.h"

using namespace Vizzu;

extern "C" {
	extern char* jsconsolelog(const char*);
	extern void event_invoked(int, const char*);
}

void *chart_create()
{
	return Interface::instance.createChart();
}

const char *vizzu_errorMessage(int exceptionPtr)
{
	return reinterpret_cast<std::exception*>(exceptionPtr)->what();
}

extern const char *vizzu_version()
{
	return Interface::instance.version();
}

void vizzu_setLogging(bool enable)
{
	IO::Log::set(enable 
		? [=](const std::string&msg) { jsconsolelog((msg + "\n").c_str()); }
		: IO::Log::LogFunc());
}

void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift)
{
	Interface::instance.keyPress(key, ctrl, alt, shift);
}

void vizzu_mouseMove(double x, double y)
{
	Interface::instance.mouseMove(x, y);
}

void vizzu_mouseDown(double x, double y)
{
	Interface::instance.mouseDown(x, y);
}

void vizzu_mouseUp(double x, double y)
{
	Interface::instance.mouseUp(x, y);
}

void vizzu_poll()
{
	Interface::instance.poll();
}

void vizzu_update(double scale, double width, double height, bool force)
{
	Interface::instance.update(scale, width, height, force);
}

void style_setValue(void *chartPtr, const char *path, const char *value)
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	if (chart->getChart().getStylesheet().hasParam(path))
		chart->getChart().getStylesheet().setParam(path, value);
	else
		throw std::logic_error(
			"non-existent style parameter: " + std::string(path));
}

void chart_setValue(void *chartPtr, const char *path, const char *value)
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	chart->getChart().getDescriptor().setParam(path, value);
}

void chart_setFilter(void *chartPtr, bool (*filter)(const void *))
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	chart->getChart().getDescriptor().setFilter(filter);
}

const void *record_getValue(void *record, const char *column, bool discrete)
{
	auto &row = *static_cast<const Data::RowWrapper*>(record);
	auto cell = row[column];
	if (discrete)
		return static_cast<const void *>(cell.discreteValue());
	else
		return static_cast<const void *>(&(*cell));
}

void data_addCategories(void *chartPtr,
	const char *name,
    const char **categories,
    int count)
{
	if (categories) 
	{
		std::span<const char *> view(categories, count);
		auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
		auto &table = chart->getChart().getTable();
		table.addColumn(name, view);
	}
}

void data_addValues(void *chartPtr,
	const char *name, double *values, int count)
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	std::span<double> view(values, count);
	auto &table = chart->getChart().getTable();
	table.addColumn(name, view);
}

int addEventListener(void *chartPtr, const char *eventName) 
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	auto& ed = chart->getChart().getEventDispatcher();
	auto id = ed[eventName]->attach([&](Util::EventDispatcher::Params& params) {
		auto jsonStrIn = params.toJsonString();
		event_invoked(params.handler, jsonStrIn.c_str());
	});
	return (int)id;
}

void removeEventListener(void *chartPtr, const char * eventName, int id)  
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	auto& ed = chart->getChart().getEventDispatcher();
	ed[eventName]->detach(id);
}

void event_preventDefault(void *param)
{
	auto *eventParam = static_cast<Util::EventDispatcher::Params*>(param);
	eventParam->preventDefault = true;
}

void chart_animate(void *chartPtr, void (*callback)())
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	if (chart) chart->getChart().animate([=]{ callback(); });
}

void anim_control(void *chartPtr, const char *command, const char *param)
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	auto &ctrl = chart->getChart().getAnimControl();
	std::string cmd(command);
	if (cmd == "seek") ctrl.seek(param);
	else if (cmd == "pause") ctrl.pause();
	else if (cmd == "play") ctrl.play();
	else if (cmd == "stop") ctrl.stop();
	else if (cmd == "reverse") ctrl.reverse();
	else throw std::logic_error("invalid animation command");
}

void anim_setValue(void *chartPtr, const char *path, const char *value)
{
	auto *chart = static_cast<UI::ChartWidget*>(chartPtr);
	chart->getChart().getAnimOptions().set(path, value);
}
