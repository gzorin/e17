
/**************************************************
 **               E  -  N O T E S                **
 **                                              **
 **  The contents of this file are released to   **
 **  the public under the General Public Licence **
 **  Version 2.                                  **
 **                                              **
 **  By  Thomas Fletcher (www.fletch.vze.com)    **
 **                                              **
 **************************************************/


#include "config.h"

/**
 * @return: Returns an allocated MainConfig variable with some default values
 *          set.
 * @brief: Allocate and initialize a new MainConfig variable.
 */
MainConfig     *
mainconfig_new(void)
{
	MainConfig     *p;

	p = malloc(sizeof(MainConfig));

	p->render_method = NULL;
	p->theme = NULL;

	p->debug = 0;
	p->controlcentre = 1;
	p->autosave = 0;
	p->welcome = 0;
	p->ontop = 0;
	p->sticky = 0;

	return (p);
}

/**
 * @param p: The MainConfig variable to free.
 * @brief: Free's an allocated MainConfig variable.
 */
void
mainconfig_free(MainConfig * p)
{
	if (!p) {
		if (p->render_method != NULL)
			free(p->render_method);
		if (p->theme != NULL)
			free(p->theme);

		free(p);
	}
	return;
}

/* LISTENERS */

theme_listener(const char *key, const Ecore_Config_Type type, const int tag,
	       void *data)
{
	main_config->theme = ecore_config_theme_get(key);
	cc_update_theme();
	notes_update_themes();
	return;
}

/**
 * @param p:  The MainConfig variable to store the read settings into.
 * @param fn: The location of the configuration file to read.
 * @brief: Reads the configuration file pointed to by fn, and stores the
 *         settings into p.
 */
int
read_configuration(MainConfig * p)
{
	ecore_config_int_create("enotes.debug", 0, 'd', "debug",
				"Debugging Level [0-2]");
	ecore_config_string_create("enotes.engine", "software", 'r',
				   "render-method",
				   "Rendering Method [GL/Software]");
	ecore_config_boolean_create("enotes.autosave", 0, 'A', "auto-save",
				    "Use the Autosave Feature?");
	ecore_config_boolean_create("enotes.controlcentre", 1, 'C',
				    "control-centre",
				    "Use the Control Centre?");
	ecore_config_boolean_create("enotes.welcome", 1, 'w', "welcome",
				    "Display the Welcome Message?");
	ecore_config_boolean_create("enotes.ontop", 1, 'o', "ontop",
				    "Keep Enotes Windows Ontop?");
	ecore_config_boolean_create("enotes.sticky", 1, 's', "sticky",
				    "Make E-Notes Sticky?");

	ecore_config_theme_create("enotes.theme", "postit", 't', "theme",
				  "GUI Theme");
	ecore_config_theme_preview_group_set("enotes.theme", "preview");
	ecore_config_theme_search_path_append(PACKAGE_DATA_DIR "/themes/");

	ecore_config_load();

	if (ecore_config_args_parse() != ECORE_CONFIG_PARSE_CONTINUE) {
		return (-1);
	}

	p->render_method = ecore_config_string_get("enotes.engine");
	p->theme = ecore_config_theme_get("enotes.theme");
	p->controlcentre = ecore_config_boolean_get("enotes.controlcentre");
	p->debug = ecore_config_boolean_get("enotes.debug");
	p->autosave = ecore_config_boolean_get("enotes.autosave");
	p->welcome = ecore_config_boolean_get("enotes.welcome");
	p->ontop = ecore_config_boolean_get("enotes.ontop");
	p->sticky = ecore_config_boolean_get("enotes.sticky");

	printf("Welcome: %d\n", p->welcome);

	ecore_config_listen("theme", "enotes.theme", theme_listener, 0, NULL);

	return (0);
}
