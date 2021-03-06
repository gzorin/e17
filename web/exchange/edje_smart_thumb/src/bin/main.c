#include "main.h"

static void args_parse(void);
static void help_show(void);
static Eina_Bool signal_exit(void *data, int ev_type, void *ev);
static Eina_Bool thumb_grab(void *data);

Ecore_Evas *ee = NULL, *ee_im = NULL, *ee_im2 = NULL;
Evas *evas = NULL, *evas_im = NULL, *evas_im2 = NULL;
Evas_Object *edje = NULL, *im = NULL, *im2 = NULL;
char *file = NULL;
char *group = NULL;
char *outfile = NULL;
int w = 640;
int h = 480;
int outw = 160;
int outh = 120;

int
main(int argc, char **argv)
{
	if (!ecore_init()) return -1;
	ecore_app_args_set(argc, (const char **)argv);
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, signal_exit, NULL);
	if (!ecore_evas_init()) return -1;
	if (!edje_init()) return -1;

	args_parse();
	
	ee = ecore_evas_buffer_new(outw, outh);
	if (!ee)
	{
		fprintf(stderr, "Error. cannot create buffer engine canvas for image save.\n");
		exit(-1);
	}
	evas = ecore_evas_get(ee);
	
	im = ecore_evas_object_image_new(ee);
	if (!im)
	{
		fprintf(stderr, "Error. cannot create buffer canvas image object for scaling.\n");
		exit(-1);
	}
	evas_object_move(im, 0, 0);
	evas_object_resize(im, outw, outh);
	evas_object_image_fill_set(im, 0, 0, outw, outh);
	evas_object_show(im);
	evas_object_image_size_set(im, outw, outh);
	ee_im = evas_object_data_get(im, "Ecore_Evas");
	evas_im = ecore_evas_get(ee_im);
	
	im2 = ecore_evas_object_image_new(ee_im);
	if (!im2)
	{
		fprintf(stderr, "Error. cannot create buffer canvas image for scaling.\n");
		exit(-1);
	}
	evas_object_move(im2, 0, 0);
	evas_object_resize(im2, outw, outh);
	evas_object_image_fill_set(im2, 0, 0, outw, outh);
	evas_object_show(im2);
	evas_object_image_size_set(im2, w, h);
	ee_im2 = evas_object_data_get(im2, "Ecore_Evas");
	evas_im2 = ecore_evas_get(ee_im2);
	
	setup_scene();
	
	edje_message_signal_process();
	
	ecore_timer_add(2.0, thumb_grab, NULL);
	ecore_main_loop_begin();
	
	evas_object_del(edje);
	ecore_evas_free(ee);	
	edje_shutdown();
	ecore_evas_shutdown();
	ecore_shutdown();
	return 0;
}

static void
setup_scene(void)
{
	if (!strcmp(group, "e/desktop/background"))
	{
		edje = edje_object_add(evas_im2);
		if (!edje_object_file_set(edje, file, group))
		{
			fprintf(stderr, "Error. cannot load file %s, group %s\n", file, group);
			exit(-1);
		}
		evas_object_move(edje, 0, 0);
		evas_object_resize(edje, w, h);
		evas_object_show(edje);
	}
	else if (!strcmp(group, "e/fileman"))
	{
		
	}
	else if (!strcmp(group, "e/gadman"))
	{
		
	}
	else if (!strcmp(group, "e/icons"))
	{
		
	}
	else if (!strcmp(group, "e/modules/battery"))
	{
		
	}
	else if (!strcmp(group, "e/modules/clock"))
	{
		
	}
	else if (!strcmp(group, "e/modules/cpufreq"))
	{
		
	}
	else if (!strcmp(group, "e/modules/ibar"))
	{
		
	}
	else if (!strcmp(group, "e/modules/ibox"))
	{
		
	}
	else if (!strcmp(group, "e/modules/pager"))
	{
		
	}
	else if (!strcmp(group, "e/modules/start"))
	{
		
	}
	else if (!strcmp(group, "e/modules/temperature"))
	{
		
	}
	else if (!strcmp(group, "e/modules/winlist"))
	{
		
	}
	else if (!strcmp(group, "e/pointer"))
	{
		
	}
	else if (!strcmp(group, "e/shelf"))
	{
		
	}
	else if (!strcmp(group, "e/theme/about"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/default"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/dialog"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/noresize"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/noresize_dialog"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/pixel"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/border/shaped"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/about"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/button"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/check"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/color_well"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/configure"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/cslider"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/deskpreview"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/dialog"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/entry"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/exebuf"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/fileselector"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/fontpreview"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/frame"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/gradpreview"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/ilist"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/label"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/menu"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/pager"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/preview"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/radio"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/scrollframe"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/slider"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/spectrum"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/textblock"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/tlist"))
	{
		
	}
	else if (!strcmp(group, "e/widgets/winlist"))
	{
		
	}
	else
	{
		edje = edje_object_add(evas_im2);
		if (!edje_object_file_set(edje, file, group))
		{
			fprintf(stderr, "Error. cannot load file %s, group %s\n", file, group);
			exit(-1);
		}
		evas_object_move(edje, 0, 0);
		evas_object_resize(edje, w, h);
		evas_object_show(edje);
	}
}

static void
args_parse(void)
{
	int argc;
	char **argv;
	int i;
	
	ecore_app_args_get(&argc, &argv);
	for (i = 1; i < argc; i++)
	{
		if ((!strcmp(argv[i], "-h")) ||
			 (!strcmp(argv[i], "-help")) ||
			 (!strcmp(argv[i], "--help")))
		{
			help_show();
			exit(-1);
		}
		else if (((!strcmp(argv[i], "-g")) ||
			(!strcmp(argv[i], "-geometry")) ||
			(!strcmp(argv[i], "--geometry"))) && (i < (argc - 1)))
		{
			int n;
			char buf[16], buf2[16];
			
			n = sscanf(argv[i +1], "%10[^x]x%10s", buf, buf2);
			if (n == 2)
			{
				w = atoi(buf);
				h = atoi(buf2);
			}
			i++;
		}
		else if (((!strcmp(argv[i], "-og")) ||
			(!strcmp(argv[i], "-out-geometry")) ||
			(!strcmp(argv[i], "--out-geometry"))) && (i < (argc - 1)))
		{
			int n;
			char buf[16], buf2[16];
			
			n = sscanf(argv[i +1], "%10[^x]x%10s", buf, buf2);
			if (n == 2)
			{
				outw = atoi(buf);
				outh = atoi(buf2);
			}
			i++;
		}
		else if (!file)
		{
			file = argv[i];
		}
		else if (!group)
		{
			group = argv[i];
		}
		else if (!outfile)
		{
			outfile = argv[i];
		}
	}
	if ((!file) || (!group) || (!outfile))
	{
		help_show();
		exit(-1);
	}
}

static void
help_show(void)
{
	printf("Usage: edje_thumb INPUT_EDJE GROUP_TO_LOAD OUT_FILE [OPTIONS]\n"
		"\n"
		"Where required parameters are:\n"
		"  INPUT_EDJE    the edje file to look at\n"
		"  GROUP_TO_LOAD the group name in the edge\n"
		"  OUT_FILE      the output file or format\n"
		"\n"
		"Where optional OPTIONS are:\n"
		"  -h          this help\n"	 "  -g WxH      rendering geometry\n"
		"  -og WxH     output file geometry\n"
		);
}

static Eina_Bool
signal_exit(void *data, int ev_type, void *ev)
{
	ecore_main_loop_quit();
	return EINA_TRUE;
}

static Eina_Bool
thumb_grab(void *data)
{
	char buf[4096];
	
	snprintf(buf, sizeof(buf), outfile);
	ecore_evas_buffer_pixels_get(ee);
	if (!evas_object_image_save(im, buf, NULL, "quality=100 compress=9"))
	{
		fprintf(stderr, "Error. cannot load file \"%s\". Missing saver module for Evas or literally cannot write the file (permissions or directory doesnt exist).\n", buf);
		exit(-1);
	}
	ecore_main_loop_quit();
	return EINA_FALSE;
}
