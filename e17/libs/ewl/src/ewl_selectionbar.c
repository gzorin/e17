
#include <Ewl.h>

void            __ewl_selectionbar_realize(Ewl_Widget * w, void *ev_data,
					   void *user_data);
void            __ewl_selectionbar_show(Ewl_Widget * w, void *ev_data,
					void *user_data);
void            __ewl_selectionbar_configure(Ewl_Widget * w, void *ev_data,
					     void *user_data);
void            __ewl_selectionbar_parent_configure(Ewl_Widget * w,
						    void *ev_data,
						    void *user_data);
void            __focus_in(Ewl_Widget * w, void *ev_data, void *user_data);
void            __focus_out(Ewl_Widget * w, void *ev_data, void *user_data);

void            __children_animator(Ewl_Widget * w, void *ev_data,
				    void *user_data);

void            __mouse_move_over_children(Ewl_Widget * w, void *user_data,
					   void *ev_data);

void            __child_add(Ewl_Container * parent, Ewl_Widget * child);

int             __open_bar_cb(void *ev_data);
int             __close_bar_cb(void *ev_data);

Ecore_Timer    *open_timer;
Ecore_Timer    *close_timer;

/**
 * ewl_selectionbar_new - create a new selectionbar
 *
 * Returns a pointer to a newly allocated scrollpane on success, NULL on
 * failure.
 */
Ewl_Widget     *ewl_selectionbar_new(Ewl_Widget * parent)
{
	Ewl_Selectionbar *s;

	DENTER_FUNCTION(DLEVEL_STABLE);

	s = NEW(Ewl_Selectionbar, 1);
	if (!s)
		DRETURN_PTR(NULL, DLEVEL_STABLE);

	memset(s, 0, sizeof(Ewl_Selectionbar));

	ewl_selectionbar_init(s, parent);


	DRETURN_PTR(EWL_WIDGET(s), DLEVEL_STABLE);
}


/**
 * ewl_selectionbar_init - initialize a new selectionbar
 * @s: the selectionbar to initialize
 * 
 * Returns no value. Sets up the default callbacks and values for the
 * the selectionbar
 */
void ewl_selectionbar_init(Ewl_Selectionbar * s, Ewl_Widget * parent)
{
	Ewl_Widget     *w;
	Ewl_Embed      *embed;


	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("s", s);


	w = EWL_WIDGET(s);

	ewl_floater_init(EWL_FLOATER(s), parent);
	ewl_callback_append(w, EWL_CALLBACK_CONFIGURE,
			    __ewl_selectionbar_configure, NULL);
	ewl_callback_append(w, EWL_CALLBACK_REALIZE,
			    __ewl_selectionbar_realize, NULL);
	ewl_callback_append(w, EWL_CALLBACK_SHOW, __ewl_selectionbar_show,
			    NULL);

	s->bar = NEW(Ewl_Container, 1);
	memset(s->bar, 0, sizeof(Ewl_Container));


	ewl_container_init(EWL_CONTAINER(s->bar), "/selectionbar", NULL, NULL,
			NULL);
	ewl_object_set_fill_policy(EWL_OBJECT(s->bar), EWL_FILL_POLICY_HFILL |
				   EWL_FILL_POLICY_HSHRINK);
	ewl_container_append_child(EWL_CONTAINER(w), EWL_WIDGET(s->bar));
	ewl_callback_append(EWL_WIDGET(s->bar), EWL_CALLBACK_FOCUS_OUT,
			    __focus_out, w);
	ewl_callback_append(EWL_WIDGET(s->bar), EWL_CALLBACK_FOCUS_IN,
			    __focus_in, w);


	embed = ewl_embed_find_by_widget(parent);
	ewl_callback_append(EWL_WIDGET(embed), EWL_CALLBACK_CONFIGURE,
			    __ewl_selectionbar_parent_configure, w);


	s->scroller.top = ewl_vbox_new();
	ewl_object_request_size(EWL_OBJECT(s->scroller.top), 30, 10);
	ewl_container_append_child(s->bar, s->scroller.top);

/*	ewl_callback_append(s->scroller.top, EWL_CALLBACK_MOUSE_MOVE,
			__mouse_move_over_children, s);
*/
	ewl_widget_set_appearance(s->scroller.top, "/selectionbar/scroller/top");


	s->scroller.bottom = ewl_vbox_new();
	ewl_object_request_size(EWL_OBJECT(s->scroller.bottom), 30, 10);
	ewl_container_append_child(s->bar, s->scroller.bottom);

/*	ewl_callback_append(s->scroller.bottom, EWL_CALLBACK_MOUSE_MOVE,
			__mouse_move_over_children, s);
*/
	ewl_widget_set_appearance(s->scroller.bottom, "/selectionbar/scroller/bottom");


	ewl_container_add_notify(EWL_CONTAINER(w), __child_add);

	s->OPEN = 1;
	s->mouse_x = 0;

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}


void __child_add(Ewl_Container * parent, Ewl_Widget * child)
{
	ewl_container_append_child(EWL_SELECTIONBAR(parent)->bar, child);
	ewl_container_remove_child(parent, child);

	ewl_callback_append(child, EWL_CALLBACK_MOUSE_MOVE,
			    __mouse_move_over_children, parent);

}

void __ewl_selectionbar_realize(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;

	DENTER_FUNCTION(DLEVEL_STABLE);


	s = EWL_SELECTIONBAR(w);

	if (!s->w)
		s->w = CURRENT_W(EWL_OBJECT(w->parent));
	if (!s->h)
		s->h = 80;

	ewl_object_request_size(EWL_OBJECT(w), s->w, s->h);
	ewl_object_request_size(EWL_OBJECT(s->bar), s->w, s->h);

	ewl_object_set_maximum_h(EWL_OBJECT(s->bar), s->h);

	ewl_widget_show(EWL_WIDGET(EWL_SELECTIONBAR(w)->scroller.top));
	ewl_widget_show(EWL_WIDGET(EWL_SELECTIONBAR(w)->scroller.bottom));
	ewl_widget_show(EWL_WIDGET(EWL_SELECTIONBAR(w)->bar));


	DLEAVE_FUNCTION(DLEVEL_STABLE);
}


void __ewl_selectionbar_show(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;

	s = EWL_SELECTIONBAR(w);

	ecore_timer_add(0.01, __close_bar_cb, s->bar);
}


void
__ewl_selectionbar_configure(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;
	Ewl_Widget     *child;
	Ewd_List       *children;

	DENTER_FUNCTION(DLEVEL_STABLE);

	s = EWL_SELECTIONBAR(w);

	if (s->w < CURRENT_W(EWL_OBJECT(EWL_FLOATER(s)->follows)))
		s->w = CURRENT_W(EWL_OBJECT(EWL_FLOATER(s)->follows));

	if (s->w > CURRENT_W(EWL_OBJECT(EWL_FLOATER(s)->follows)))
		s->w = CURRENT_W(EWL_OBJECT(EWL_FLOATER(s)->follows));

	if (CURRENT_H(EWL_OBJECT(s)) < 5)
		ewl_object_request_h(EWL_OBJECT(s), 5);


	ewl_widget_configure(EWL_WIDGET(s->bar));

	children = EWL_CONTAINER(EWL_FLOATER(w)->follows)->children;

	/*
	if (children) {
		ewd_list_goto_first(children);
		while ((child = ewd_list_next(children)) != NULL)
			printf("child layer = %d\n", LAYER(child));
		s = s;
	}
	*/


	DLEAVE_FUNCTION(DLEVEL_STABLE);
}


void
__ewl_selectionbar_parent_configure(Ewl_Widget * w, void *ev_data,
				    void *user_data)
{
	Ewl_Selectionbar *s;
	Ewl_Object     *o;

	DENTER_FUNCTION(DLEVEL_STABLE);


	s = EWL_SELECTIONBAR(user_data);
	o = EWL_OBJECT(s);

	printf("bar height: %d\n", CURRENT_H(EWL_OBJECT(s->bar)));


	ewl_object_request_size(o, s->w, CURRENT_H(EWL_OBJECT(s->bar)));


	if (CURRENT_H(EWL_OBJECT(s->bar)) < 5)
		ewl_object_request_size(o, s->w, 5);
	__focus_out(EWL_WIDGET(s->bar), NULL, s);


	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void __focus_in(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;
	Ewl_Object     *o;
	Ewd_List       *children;
	Ewl_Widget     *child;
	int             cum_width;


	DENTER_FUNCTION(DLEVEL_STABLE);


	s = EWL_SELECTIONBAR(user_data);
	o = EWL_OBJECT(w);
	children = EWL_CONTAINER(w)->children;
	cum_width = 0;


	if (s->OPEN)
		return;


	ecore_timer_add(0.01, __open_bar_cb, w);

	ewl_widget_show(s->scroller.top);
	ewl_widget_show(s->scroller.bottom);

	ewd_list_goto_first(children);
	while ((child = ewd_list_next(children)) != NULL) {
		if (child != EWL_WIDGET(s->scroller.top) &&
		    child != EWL_WIDGET(s->scroller.bottom)) {

			ewl_object_request_size(EWL_OBJECT(child), 60, 40);

			ewl_object_request_position(EWL_OBJECT(child),
						    CURRENT_X(o) + cum_width,
						    CURRENT_Y(o) +
						    ((CURRENT_H(o) -
						      CURRENT_H(child)) / 2));

			ewl_widget_show(child);

			cum_width += CURRENT_W(child) + 5;
		}
	}

	s->OPEN = 1;

/*	ewl_callback_append(w, EWL_CALLBACK_MOUSE_MOVE,
			__children_animator, s);

*/
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}


void __focus_out(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;
	Ewd_List       *children;
	Ewl_Widget     *child;
	Ewl_Embed      *embed;
	Ewl_Object     *o;

	int             mx, my;
	int             wx, wy;
	int             ww, wh;

	DENTER_FUNCTION(DLEVEL_STABLE);

	s = EWL_SELECTIONBAR(user_data);
	o = EWL_OBJECT(w);
	children = EWL_CONTAINER(w)->children;


	/*
	 * First check that the mouse left the selectionbar and that
	 * focus didn't just go to one of it's children
	 */
	embed = ewl_embed_find_by_widget(w);

	/*
	 * FIXME: This is not in ecore yet.
	 */
	/* ecore_pointer_xy(embed->evas_window, &mx, &my); */
	mx = 0;
	my = 0;

	wx = CURRENT_X(o);
	wy = CURRENT_Y(o);
	ww = CURRENT_W(o);
	wh = CURRENT_H(o);


	if ((((mx > wx) && (mx < (wx + ww))) &&
	     ((my > wy) && (my < (wy + wh)))))
		return;


	ewl_callback_del(w, EWL_CALLBACK_MOUSE_MOVE, __children_animator);

	ewl_widget_hide(s->scroller.top);
	ewl_widget_hide(s->scroller.bottom);

	ewd_list_goto_first(children);
	while ((child = ewd_list_next(children)) != NULL)
		if (child != EWL_WIDGET(s->scroller.top) &&
		    child != EWL_WIDGET(s->scroller.bottom))
			ewl_widget_hide(child);


	s->OPEN = 0;

	ecore_timer_add(0.01, __close_bar_cb, w);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}



int __open_bar_cb(void *ev_data)
{
	Ewl_Widget     *w;
	Ewl_Object     *o;
	Ewl_Object     *so;
	Ewl_Selectionbar *s;
	int retval = 0;

	ecore_timer_del(close_timer);

	w = EWL_WIDGET(ev_data);
	o = EWL_OBJECT(w);
	s = EWL_SELECTIONBAR(w->parent);
	so = EWL_OBJECT(s);

	if (CURRENT_H(o) < s->h) {
		ewl_object_request_size(o, s->w, CURRENT_H(o) + s->h / 10);
		ewl_object_request_size(so, CURRENT_W(o), CURRENT_H(o));

		retval = TRUE;

		ewl_widget_configure(EWL_WIDGET(s));
	} else {
		ewl_callback_append(w, EWL_CALLBACK_MOUSE_MOVE,
				    __children_animator, s);
		ewl_callback_append(s->scroller.top, EWL_CALLBACK_MOUSE_MOVE,
				    __mouse_move_over_children, s);
		ewl_callback_append(s->scroller.bottom, EWL_CALLBACK_MOUSE_MOVE,
				    __mouse_move_over_children, s);
	}

	DRETURN_INT(retval, DLEVEL_STABLE);
}


int __close_bar_cb(void *ev_data)
{
	Ewl_Widget     *w;
	Ewl_Object     *o;
	Ewl_Object     *so;
	Ewl_Selectionbar *s;
	int retval = 0;

	ecore_timer_del(open_timer);

	w = EWL_WIDGET(ev_data);
	o = EWL_OBJECT(w);
	s = EWL_SELECTIONBAR(w->parent);
	so = EWL_OBJECT(s);

	if (CURRENT_H(o) > 10) {
		ewl_object_request_size(o, s->w,
					CURRENT_H(o) - (s->h / 10) + 5);
		ewl_object_request_size(so, s->w,
					CURRENT_H(o) - (s->h / 10) + 5);

		retval = TRUE;
	} else {
		ewl_object_request_size(o, s->w, 5);
		ewl_object_request_size(so, s->w, 5);
	}

	DRETURN_INT(retval, DLEVEL_STABLE);
}


void __mouse_move_over_children(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Selectionbar *s;

	s = EWL_SELECTIONBAR(user_data);

	__children_animator(EWL_WIDGET(s->bar), ev_data, s);
}

void __children_animator(Ewl_Widget * w, void *ev_data, void *user_data)
{

	Ewl_Widget     *child;
	Ewl_Object     *o;
	Ewd_List       *children;
	Ecore_X_Event_Mouse_Move *ev;
	Ewl_Selectionbar *s;
	int             x, old_x;

	s = EWL_SELECTIONBAR(user_data);

	o = EWL_OBJECT(w);
	children = EWL_CONTAINER(w)->children;

	ev = ev_data;
	x = ev->x - CURRENT_X(o);

	if (!s->mouse_x)
		old_x = x;
	else
		old_x = s->mouse_x;
	s->mouse_x = x;

	ewl_object_request_position(EWL_OBJECT(s->scroller.top), x - 15,
				    CURRENT_Y(o) + 5);

	ewl_object_request_position(EWL_OBJECT(s->scroller.bottom),
				    x - 15, CURRENT_Y(o) + CURRENT_H(o) - 15);


	ewd_list_goto_first(children);
	while ((child = ewd_list_next(children)) != NULL) {
		if (child != EWL_WIDGET(s->scroller.bottom) &&
		    child != EWL_WIDGET(s->scroller.top)) {

			if (CURRENT_W(EWL_OBJECT(child)) < 2 ||
			    CURRENT_H(EWL_OBJECT(child)) < 2) {
				ewl_object_request_size(EWL_OBJECT(child),
						ewl_object_get_preferred_w
							(EWL_OBJECT(child)),
						ewl_object_get_preferred_h
							(EWL_OBJECT(child)));
			}

			ewl_object_request_position(EWL_OBJECT(child),
						    CURRENT_X(child) +
						    (old_x - x),
						    CURRENT_Y(o) +
						    ((CURRENT_H(o) -
						      CURRENT_H(child)) / 2));
		}
	}

}
