#ifndef _STARDICT_PLUG_MANAGER_H_
#define _STARDICT_PLUG_MANAGER_H_

#include "plugin.h"
#include "virtualdictplugin.h"
#include "ttsplugin.h"
#include <glib.h>
#include <gmodule.h>
#include <string>
#include <vector>
#include <list>
#include "dictmask.h"

struct StarDictPluginBaseObject {
	StarDictPluginBaseObject(const char *filename, GModule *module_, plugin_configure_func_t configure_func_);
	std::string plugin_filename;
	GModule *module;
	plugin_configure_func_t configure_func;
};

class StarDictPluginBase {
public:
	StarDictPluginBase(StarDictPluginBaseObject *baseobj_);
	~StarDictPluginBase();
	const char *get_filename();
	void configure();
protected:
	StarDictPluginBaseObject *baseobj;
};

class StarDictVirtualDictPlugin : public StarDictPluginBase {
public:
	StarDictVirtualDictPlugin(StarDictPluginBaseObject *baseobj, StarDictVirtualDictPlugInObject *virtualdict_plugin_obj);
	~StarDictVirtualDictPlugin();
	void lookup(const char *word, char ***pppWord, char ****ppppWordData);
	bool is_instant();
	const char *dict_name();
private:
	StarDictVirtualDictPlugInObject *obj;
};

class StarDictVirtualDictPlugins {
public:
	StarDictVirtualDictPlugins();
	~StarDictVirtualDictPlugins();
	void add(StarDictPluginBaseObject *baseobj, StarDictVirtualDictPlugInObject *virtualdict_plugin_obj);
	void lookup(size_t iPlugin, const gchar *word, char ***pppWord, char ****ppppWordData);
	size_t ndicts() { return oPlugins.size(); }
	const char *dict_name(size_t iPlugin);
	void SetDictMask(std::vector<InstantDictIndex> &dictmask);
	void unload_plugin(const char *filename);
	void configure_plugin(const char *filename);
private:
	std::vector<StarDictVirtualDictPlugin *> oPlugins;
};

class StarDictTtsPlugin : public StarDictPluginBase {
public:
	StarDictTtsPlugin(StarDictPluginBaseObject *baseobj, StarDictTtsPlugInObject *tts_plugin_obj);
	~StarDictTtsPlugin();
	void saytext(const gchar *text);
	const char *tts_name();
private:
	StarDictTtsPlugInObject *obj;
};

class StarDictTtsPlugins {
public:
	StarDictTtsPlugins();
	~StarDictTtsPlugins();
	void add(StarDictPluginBaseObject *baseobj, StarDictTtsPlugInObject *tts_plugin_obj);
	void saytext(size_t iPlugin, const gchar *text);
	const char* tts_name(size_t iPlugin);
	size_t nplugins() { return oPlugins.size(); }
	void unload_plugin(const char *filename);
	void configure_plugin(const char *filename);
private:
	std::vector<StarDictTtsPlugin *> oPlugins;
};

struct StarDictPluginInfo {
	std::string filename;
	StarDictPlugInType plugin_type;
	std::string info_xml;
	bool can_configure;
};

class StarDictPlugins {
public:
	StarDictPlugins(const char *dirpath, const std::list<std::string>& disable_list);
	~StarDictPlugins();
	void get_plugin_list(std::list<std::pair<StarDictPlugInType, std::list<StarDictPluginInfo> > > &plugin_list);
	bool get_loaded(const char *filename);
	void load_plugin(const char *filename);
	void configure_plugin(const char *filename, StarDictPlugInType plugin_type);
	void unload_plugin(const char *filename, StarDictPlugInType plugin_type);
	StarDictVirtualDictPlugins VirtualDictPlugins;
	StarDictTtsPlugins TtsPlugins;
private:
	std::string plugindirpath;
	std::list<std::string> loaded_plugin_list;
	void load(const char *dirpath, const std::list<std::string>& disable_list);
	void get_plugin_info(const char *filename, StarDictPlugInType &plugin_type, std::string &info_xml, bool &can_configure);
};

#endif