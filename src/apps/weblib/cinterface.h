#ifndef LIB_CINTERFACE_H
#define LIB_CINTERFACE_H

extern "C" {

extern void vizzu_init();
extern void vizzu_poll();
extern void vizzu_mouseDown(double x, double y);
extern void vizzu_mouseUp(double x, double y);
extern void vizzu_mouseMove(double x, double y);
extern void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift);
extern void vizzu_setLogging(bool enable);
extern void vizzu_update(double scale, double width, double height, bool force);
extern const char *vizzu_errorMessage(int exceptionPtr);
extern const char *vizzu_version();

extern void data_addCategories(const char *name, const char **categories, int count);
extern void data_addValues(const char *name, double *values, int count);
extern const void *record_getValue(void *record, const char *column, bool discrete);
extern void *chart_store();
extern void chart_restore(void *chart);
extern void chart_free(void *chart);
extern const char *style_getList();
extern void style_setValue(const char *path, const char *value);
extern const char *style_getValue(const char *path);
const char *chart_getList();
const char *chart_getValue(const char *path);
extern void chart_setValue(const char *path, const char *value);
extern void chart_setFilter(bool (*filter)(const void *));
extern void chart_animate(void (*callback)());
extern int addEventListener(const char *name);
extern void removeEventListener(const char *name, int id);
extern void event_preventDefault();
const char *anim_getList();
const char *anim_getValue(const char *path);
extern void anim_control(const char *command, const char *param);
extern void anim_setValue(const char *path, const char *value);

}

#endif
