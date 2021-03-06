/**
 * @defgroup Genlist Genlist
 *
 * @image html img/widget/genlist/preview-00.png
 * @image latex img/widget/genlist/preview-00.eps
 * @image html img/genlist.png
 * @image latex img/genlist.eps
 *
 * This widget aims to have more expansive list than the simple list in
 * Elementary that could have more flexible items and allow many more entries
 * while still being fast and low on memory usage. At the same time it was
 * also made to be able to do tree structures. But the price to pay is more
 * complexity when it comes to usage. If all you want is a simple list with
 * icons and a single text, use the normal @ref List object.
 *
 * Genlist has a fairly large API, mostly because it's relatively complex,
 * trying to be both expansive, powerful and efficient. First we will begin
 * an overview on the theory behind genlist.
 *
 * @section Genlist_Item_Class Genlist item classes - creating items
 *
 * In order to have the ability to add and delete items on the fly, genlist
 * implements a class (callback) system where the application provides a
 * structure with information about that type of item (genlist may contain
 * multiple different items with different classes, states and styles).
 * Genlist will call the functions in this struct (methods) when an item is
 * "realized" (i.e., created dynamically, while the user is scrolling the
 * grid). All objects will simply be deleted when no longer needed with
 * evas_object_del(). The #Elm_Genlist_Item_Class structure contains the
 * following members:
 * - @c item_style - This is a constant string and simply defines the name
 *   of the item style. It @b must be specified and the default should be @c
 *   "default".
 *
 * - @c func - A struct with pointers to functions that will be called when
 *   an item is going to be actually created. All of them receive a @c data
 *   parameter that will point to the same data passed to
 *   elm_genlist_item_append() and related item creation functions, and an @c
 *   obj parameter that points to the genlist object itself.
 *
 * The function pointers inside @c func are @c text_get, @c content_get, @c
 * state_get and @c del. The 3 first functions also receive a @c part
 * parameter described below. A brief description of these functions follows:
 *
 * - @c text_get - The @c part parameter is the name string of one of the
 *   existing text parts in the Edje group implementing the item's theme.
 *   This function @b must return a strdup'()ed string, as the caller will
 *   free() it when done. See #Elm_Genlist_Item_Text_Get_Cb.
 * - @c content_get - The @c part parameter is the name string of one of the
 *   existing (content) swallow parts in the Edje group implementing the item's
 *   theme. It must return @c NULL, when no content is desired, or a valid
 *   object handle, otherwise.  The object will be deleted by the genlist on
 *   its deletion or when the item is "unrealized".  See
 *   #Elm_Genlist_Item_Content_Get_Cb.
 * - @c func.state_get - The @c part parameter is the name string of one of
 *   the state parts in the Edje group implementing the item's theme. Return
 *   @c EINA_FALSE for false/off or @c EINA_TRUE for true/on. Genlists will
 *   emit a signal to its theming Edje object with @c "elm,state,XXX,active"
 *   and @c "elm" as "emission" and "source" arguments, respectively, when
 *   the state is true (the default is false), where @c XXX is the name of
 *   the (state) part.  See #Elm_Genlist_Item_State_Get_Cb.
 * - @c func.del - This is intended for use when genlist items are deleted,
 *   so any data attached to the item (e.g. its data parameter on creation)
 *   can be deleted. See #Elm_Genlist_Item_Del_Cb.
 *
 * available item styles:
 * - default
 * - default_style - The text part is a textblock
 *
 * @image html img/widget/genlist/preview-04.png
 * @image latex img/widget/genlist/preview-04.eps
 *
 * - double_label
 *
 * @image html img/widget/genlist/preview-01.png
 * @image latex img/widget/genlist/preview-01.eps
 *
 * - icon_top_text_bottom
 *
 * @image html img/widget/genlist/preview-02.png
 * @image latex img/widget/genlist/preview-02.eps
 *
 * - group_index
 *
 * @image html img/widget/genlist/preview-03.png
 * @image latex img/widget/genlist/preview-03.eps
 *
 * @section Genlist_Items Structure of items
 *
 * An item in a genlist can have 0 or more texts (they can be regular
 * text or textblock Evas objects - that's up to the style to determine), 0
 * or more contents (which are simply objects swallowed into the genlist item's
 * theming Edje object) and 0 or more <b>boolean states</b>, which have the
 * behavior left to the user to define. The Edje part names for each of
 * these properties will be looked up, in the theme file for the genlist,
 * under the Edje (string) data items named @c "labels", @c "contents" and @c
 * "states", respectively. For each of those properties, if more than one
 * part is provided, they must have names listed separated by spaces in the
 * data fields. For the default genlist item theme, we have @b one text
 * part (@c "elm.text"), @b two content parts (@c "elm.swalllow.icon" and @c
 * "elm.swallow.end") and @b no state parts.
 *
 * A genlist item may be at one of several styles. Elementary provides one
 * by default - "default", but this can be extended by system or application
 * custom themes/overlays/extensions (see @ref Theme "themes" for more
 * details).
 *
 * @section Genlist_Manipulation Editing and Navigating
 *
 * Items can be added by several calls. All of them return a @ref
 * Elm_Genlist_Item handle that is an internal member inside the genlist.
 * They all take a data parameter that is meant to be used for a handle to
 * the applications internal data (eg the struct with the original item
 * data). The parent parameter is the parent genlist item this belongs to if
 * it is a tree or an indexed group, and NULL if there is no parent. The
 * flags can be a bitmask of #ELM_GENLIST_ITEM_NONE,
 * #ELM_GENLIST_ITEM_SUBITEMS and #ELM_GENLIST_ITEM_GROUP. If
 * #ELM_GENLIST_ITEM_SUBITEMS is set then this item is displayed as an item
 * that is able to expand and have child items.  If ELM_GENLIST_ITEM_GROUP
 * is set then this item is group index item that is displayed at the top
 * until the next group comes. The func parameter is a convenience callback
 * that is called when the item is selected and the data parameter will be
 * the func_data parameter, obj be the genlist object and event_info will be
 * the genlist item.
 *
 * elm_genlist_item_append() adds an item to the end of the list, or if
 * there is a parent, to the end of all the child items of the parent.
 * elm_genlist_item_prepend() is the same but adds to the beginning of
 * the list or children list. elm_genlist_item_insert_before() inserts at
 * item before another item and elm_genlist_item_insert_after() inserts after
 * the indicated item.
 *
 * The application can clear the list with elm_genlist_clear() which deletes
 * all the items in the list and elm_genlist_item_del() will delete a specific
 * item. elm_genlist_item_subitems_clear() will clear all items that are
 * children of the indicated parent item.
 *
 * To help inspect list items you can jump to the item at the top of the list
 * with elm_genlist_first_item_get() which will return the item pointer, and
 * similarly elm_genlist_last_item_get() gets the item at the end of the list.
 * elm_genlist_item_next_get() and elm_genlist_item_prev_get() get the next
 * and previous items respectively relative to the indicated item. Using
 * these calls you can walk the entire item list/tree. Note that as a tree
 * the items are flattened in the list, so elm_genlist_item_parent_get() will
 * let you know which item is the parent (and thus know how to skip them if
 * wanted).
 *
 * @section Genlist_Muti_Selection Multi-selection
 *
 * If the application wants multiple items to be able to be selected,
 * elm_genlist_multi_select_set() can enable this. If the list is
 * single-selection only (the default), then elm_genlist_selected_item_get()
 * will return the selected item, if any, or NULL if none is selected. If the
 * list is multi-select then elm_genlist_selected_items_get() will return a
 * list (that is only valid as long as no items are modified (added, deleted,
 * selected or unselected)).
 *
 * @section Genlist_Usage_Hints Usage hints
 *
 * There are also convenience functions. elm_genlist_item_genlist_get() will
 * return the genlist object the item belongs to. elm_genlist_item_show()
 * will make the scroller scroll to show that specific item so its visible.
 * elm_genlist_item_data_get() returns the data pointer set by the item
 * creation functions.
 *
 * If an item changes (state of boolean changes, text or contents change),
 * then use elm_genlist_item_update() to have genlist update the item with
 * the new state. Genlist will re-realize the item and thus call the functions
 * in the _Elm_Genlist_Item_Class for that item.
 *
 * To programmatically (un)select an item use elm_genlist_item_selected_set().
 * To get its selected state use elm_genlist_item_selected_get(). Similarly
 * to expand/contract an item and get its expanded state, use
 * elm_genlist_item_expanded_set() and elm_genlist_item_expanded_get(). And
 * again to make an item disabled (unable to be selected and appear
 * differently) use elm_genlist_item_disabled_set() to set this and
 * elm_genlist_item_disabled_get() to get the disabled state.
 *
 * In general to indicate how the genlist should expand items horizontally to
 * fill the list area, use elm_genlist_horizontal_set(). Valid modes are
 * ELM_LIST_LIMIT and ELM_LIST_SCROLL. The default is ELM_LIST_SCROLL. This
 * mode means that if items are too wide to fit, the scroller will scroll
 * horizontally. Otherwise items are expanded to fill the width of the
 * viewport of the scroller. If it is ELM_LIST_LIMIT, items will be expanded
 * to the viewport width and limited to that size. This can be combined with
 * a different style that uses edjes' ellipsis feature (cutting text off like
 * this: "tex...").
 *
 * Items will only call their selection func and callback when first becoming
 * selected. Any further clicks will do nothing, unless you enable always
 * select with elm_genlist_always_select_mode_set(). This means even if
 * selected, every click will make the selected callbacks be called.
 * elm_genlist_no_select_mode_set() will turn off the ability to select
 * items entirely and they will neither appear selected nor call selected
 * callback functions.
 *
 * Remember that you can create new styles and add your own theme augmentation
 * per application with elm_theme_extension_add(). If you absolutely must
 * have a specific style that overrides any theme the user or system sets up
 * you can use elm_theme_overlay_add() to add such a file.
 *
 * @section Genlist_Implementation Implementation
 *
 * Evas tracks every object you create. Every time it processes an event
 * (mouse move, down, up etc.) it needs to walk through objects and find out
 * what event that affects. Even worse every time it renders display updates,
 * in order to just calculate what to re-draw, it needs to walk through many
 * many many objects. Thus, the more objects you keep active, the more
 * overhead Evas has in just doing its work. It is advisable to keep your
 * active objects to the minimum working set you need. Also remember that
 * object creation and deletion carries an overhead, so there is a
 * middle-ground, which is not easily determined. But don't keep massive lists
 * of objects you can't see or use. Genlist does this with list objects. It
 * creates and destroys them dynamically as you scroll around. It groups them
 * into blocks so it can determine the visibility etc. of a whole block at
 * once as opposed to having to walk the whole list. This 2-level list allows
 * for very large numbers of items to be in the list (tests have used up to
 * 2,000,000 items). Also genlist employs a queue for adding items. As items
 * may be different sizes, every item added needs to be calculated as to its
 * size and thus this presents a lot of overhead on populating the list, this
 * genlist employs a queue. Any item added is queued and spooled off over
 * time, actually appearing some time later, so if your list has many members
 * you may find it takes a while for them to all appear, with your process
 * consuming a lot of CPU while it is busy spooling.
 *
 * Genlist also implements a tree structure, but it does so with callbacks to
 * the application, with the application filling in tree structures when
 * requested (allowing for efficient building of a very deep tree that could
 * even be used for file-management). See the above smart signal callbacks for
 * details.
 *
 * @section Genlist_Smart_Events Genlist smart events
 *
 * Signals that you can add callbacks for are:
 * - @c "activated" - The user has double-clicked or pressed
 *   (enter|return|spacebar) on an item. The @c event_info parameter is the
 *   item that was activated.
 * - @c "clicked,double" - The user has double-clicked an item.  The @c
 *   event_info parameter is the item that was double-clicked.
 * - @c "selected" - This is called when a user has made an item selected.
 *   The event_info parameter is the genlist item that was selected.
 * - @c "unselected" - This is called when a user has made an item
 *   unselected. The event_info parameter is the genlist item that was
 *   unselected.
 * - @c "expanded" - This is called when elm_genlist_item_expanded_set() is
 *   called and the item is now meant to be expanded. The event_info
 *   parameter is the genlist item that was indicated to expand.  It is the
 *   job of this callback to then fill in the child items.
 * - @c "contracted" - This is called when elm_genlist_item_expanded_set() is
 *   called and the item is now meant to be contracted. The event_info
 *   parameter is the genlist item that was indicated to contract. It is the
 *   job of this callback to then delete the child items.
 * - @c "expand,request" - This is called when a user has indicated they want
 *   to expand a tree branch item. The callback should decide if the item can
 *   expand (has any children) and then call elm_genlist_item_expanded_set()
 *   appropriately to set the state. The event_info parameter is the genlist
 *   item that was indicated to expand.
 * - @c "contract,request" - This is called when a user has indicated they
 *   want to contract a tree branch item. The callback should decide if the
 *   item can contract (has any children) and then call
 *   elm_genlist_item_expanded_set() appropriately to set the state. The
 *   event_info parameter is the genlist item that was indicated to contract.
 * - @c "realized" - This is called when the item in the list is created as a
 *   real evas object. event_info parameter is the genlist item that was
 *   created. The object may be deleted at any time, so it is up to the
 *   caller to not use the object pointer from elm_genlist_item_object_get()
 *   in a way where it may point to freed objects.
 * - @c "unrealized" - This is called just before an item is unrealized.
 *   After this call content objects provided will be deleted and the item
 *   object itself delete or be put into a floating cache.
 * - @c "drag,start,up" - This is called when the item in the list has been
 *   dragged (not scrolled) up.
 * - @c "drag,start,down" - This is called when the item in the list has been
 *   dragged (not scrolled) down.
 * - @c "drag,start,left" - This is called when the item in the list has been
 *   dragged (not scrolled) left.
 * - @c "drag,start,right" - This is called when the item in the list has
 *   been dragged (not scrolled) right.
 * - @c "drag,stop" - This is called when the item in the list has stopped
 *   being dragged.
 * - @c "drag" - This is called when the item in the list is being dragged.
 * - @c "longpressed" - This is called when the item is pressed for a certain
 *   amount of time. By default it's 1 second.
 * - @c "scroll,anim,start" - This is called when scrolling animation has
 *   started.
 * - @c "scroll,anim,stop" - This is called when scrolling animation has
 *   stopped.
 * - @c "scroll,drag,start" - This is called when dragging the content has
 *   started.
 * - @c "scroll,drag,stop" - This is called when dragging the content has
 *   stopped.
 * - @c "edge,top" - This is called when the genlist is scrolled until
 *   the top edge.
 * - @c "edge,bottom" - This is called when the genlist is scrolled
 *   until the bottom edge.
 * - @c "edge,left" - This is called when the genlist is scrolled
 *   until the left edge.
 * - @c "edge,right" - This is called when the genlist is scrolled
 *   until the right edge.
 * - @c "multi,swipe,left" - This is called when the genlist is multi-touch
 *   swiped left.
 * - @c "multi,swipe,right" - This is called when the genlist is multi-touch
 *   swiped right.
 * - @c "multi,swipe,up" - This is called when the genlist is multi-touch
 *   swiped up.
 * - @c "multi,swipe,down" - This is called when the genlist is multi-touch
 *   swiped down.
 * - @c "multi,pinch,out" - This is called when the genlist is multi-touch
 *   pinched out.  "- @c multi,pinch,in" - This is called when the genlist is
 *   multi-touch pinched in.
 * - @c "swipe" - This is called when the genlist is swiped.
 * - @c "moved" - This is called when a genlist item is moved.
 * - @c "language,changed" - This is called when the program's language is
 *   changed.
 *
 * @section Genlist_Examples Examples
 *
 * Here is a list of examples that use the genlist, trying to show some of
 * its capabilities:
 * - @ref genlist_example_01
 * - @ref genlist_example_02
 * - @ref genlist_example_03
 * - @ref genlist_example_04
 * - @ref genlist_example_05
 */

/**
 * @addtogroup Genlist
 * @{
 */

/**
 * Defines if the item is of any special type (has subitems or it's the
 * index of a group), or is just a simple item.
 *
 * @ingroup Genlist
 */
typedef enum
{
   ELM_GENLIST_ITEM_NONE = 0, /**< simple item */
   ELM_GENLIST_ITEM_SUBITEMS = (1 << 0), /**< may expand and have child items */
   ELM_GENLIST_ITEM_GROUP = (1 << 1) /**< index of a group of items */
} Elm_Genlist_Item_Flags;

typedef enum
{
   ELM_GENLIST_ITEM_FIELD_ALL = 0,
   ELM_GENLIST_ITEM_FIELD_LABEL = (1 << 0),
   ELM_GENLIST_ITEM_FIELD_CONTENT = (1 << 1),
   ELM_GENLIST_ITEM_FIELD_STATE = (1 << 2)
} Elm_Genlist_Item_Field_Flags;
typedef struct _Elm_Genlist_Item_Class      Elm_Genlist_Item_Class; /**< Genlist item class definition structs */
#define Elm_Genlist_Item_Class Elm_Gen_Item_Class
typedef struct _Elm_Genlist_Item            Elm_Genlist_Item; /**< Item of Elm_Genlist. Sub-type of Elm_Widget_Item */
#define Elm_Genlist_Item       Elm_Gen_Item /**< Item of Elm_Genlist. Sub-type of Elm_Widget_Item */
typedef struct _Elm_Genlist_Item_Class_Func Elm_Genlist_Item_Class_Func;    /**< Class functions for genlist item class */

/**
 * Text fetching class function for Elm_Gen_Item_Class.
 * @param data The data passed in the item creation function
 * @param obj The base widget object
 * @param part The part name of the swallow
 * @return The allocated (NOT stringshared) string to set as the text
 */
typedef char *(*Elm_Genlist_Item_Text_Get_Cb)(void *data, Evas_Object *obj, const char *part);

/**
 * Content (swallowed object) fetching class function for Elm_Gen_Item_Class.
 * @param data The data passed in the item creation function
 * @param obj The base widget object
 * @param part The part name of the swallow
 * @return The content object to swallow
 */
typedef Evas_Object *(*Elm_Genlist_Item_Content_Get_Cb)(void *data, Evas_Object *obj, const char *part);

/**
 * State fetching class function for Elm_Gen_Item_Class.
 * @param data The data passed in the item creation function
 * @param obj The base widget object
 * @param part The part name of the swallow
 * @return The hell if I know
 */
typedef Eina_Bool (*Elm_Genlist_Item_State_Get_Cb)(void *data, Evas_Object *obj, const char *part);

/**
 * Deletion class function for Elm_Gen_Item_Class.
 * @param data The data passed in the item creation function
 * @param obj The base widget object
 */
typedef void (*Elm_Genlist_Item_Del_Cb)(void *data, Evas_Object *obj);

/**
 * @struct _Elm_Genlist_Item_Class
 *
 * Genlist item class definition structs.
 *
 * This struct contains the style and fetching functions that will define the
 * contents of each item.
 *
 * @see @ref Genlist_Item_Class
 */
struct _Elm_Genlist_Item_Class
{
   const char *item_style; /**< style of this class. */
   struct Elm_Genlist_Item_Class_Func
   {
      Elm_Genlist_Item_Text_Get_Cb    text_get; /**< Text fetching class function for genlist item classes.*/
      Elm_Genlist_Item_Content_Get_Cb content_get; /**< Content fetching class function for genlist item classes. */
      Elm_Genlist_Item_State_Get_Cb   state_get; /**< State fetching class function for genlist item classes. */
      Elm_Genlist_Item_Del_Cb         del; /**< Deletion class function for genlist item classes. */
   } func;
};
#define Elm_Genlist_Item_Class_Func Elm_Gen_Item_Class_Func
/**
 * Add a new genlist widget to the given parent Elementary
 * (container) object
 *
 * @param parent The parent object
 * @return a new genlist widget handle or @c NULL, on errors
 *
 * This function inserts a new genlist widget on the canvas.
 *
 * @see elm_genlist_item_append()
 * @see elm_genlist_item_del()
 * @see elm_genlist_clear()
 *
 * @ingroup Genlist
 */
EAPI Evas_Object                  *elm_genlist_add(Evas_Object *parent);

/**
 * Remove all items from a given genlist widget.
 *
 * @param obj The genlist object
 *
 * This removes (and deletes) all items in @p obj, leaving it empty.
 *
 * @see elm_genlist_item_del(), to remove just one item.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_clear(Evas_Object *obj);

/**
 * Enable or disable multi-selection in the genlist
 *
 * @param obj The genlist object
 * @param multi Multi-select enable/disable. Default is disabled.
 *
 * This enables (@c EINA_TRUE) or disables (@c EINA_FALSE) multi-selection in
 * the list. This allows more than 1 item to be selected. To retrieve the list
 * of selected items, use elm_genlist_selected_items_get().
 *
 * @see elm_genlist_selected_items_get()
 * @see elm_genlist_multi_select_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_multi_select_set(Evas_Object *obj, Eina_Bool multi);

/**
 * Gets if multi-selection in genlist is enabled or disabled.
 *
 * @param obj The genlist object
 * @return Multi-select enabled/disabled
 * (@c EINA_TRUE = enabled/@c EINA_FALSE = disabled). Default is @c EINA_FALSE.
 *
 * @see elm_genlist_multi_select_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_multi_select_get(const Evas_Object *obj);

/**
 * This sets the horizontal stretching mode.
 *
 * @param obj The genlist object
 * @param mode The mode to use (one of #ELM_LIST_SCROLL or #ELM_LIST_LIMIT).
 *
 * This sets the mode used for sizing items horizontally. Valid modes
 * are #ELM_LIST_LIMIT and #ELM_LIST_SCROLL. The default is
 * ELM_LIST_SCROLL. This mode means that if items are too wide to fit,
 * the scroller will scroll horizontally. Otherwise items are expanded
 * to fill the width of the viewport of the scroller. If it is
 * ELM_LIST_LIMIT, items will be expanded to the viewport width and
 * limited to that size.
 *
 * @see elm_genlist_horizontal_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_horizontal_set(Evas_Object *obj, Elm_List_Mode mode);

/**
 * Gets the horizontal stretching mode.
 *
 * @param obj The genlist object
 * @return The mode to use
 * (#ELM_LIST_LIMIT, #ELM_LIST_SCROLL)
 *
 * @see elm_genlist_horizontal_set()
 *
 * @ingroup Genlist
 */
EAPI Elm_List_Mode                 elm_genlist_horizontal_get(const Evas_Object *obj);

/**
 * Set the always select mode.
 *
 * @param obj The genlist object
 * @param always_select The always select mode (@c EINA_TRUE = on, @c
 * EINA_FALSE = off). Default is @c EINA_FALSE.
 *
 * Items will only call their selection func and callback when first
 * becoming selected. Any further clicks will do nothing, unless you
 * enable always select with elm_genlist_always_select_mode_set().
 * This means that, even if selected, every click will make the selected
 * callbacks be called.
 *
 * @see elm_genlist_always_select_mode_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_always_select_mode_set(Evas_Object *obj, Eina_Bool always_select);

/**
 * Get the always select mode.
 *
 * @param obj The genlist object
 * @return The always select mode
 * (@c EINA_TRUE = on, @c EINA_FALSE = off)
 *
 * @see elm_genlist_always_select_mode_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_always_select_mode_get(const Evas_Object *obj);

/**
 * Enable/disable the no select mode.
 *
 * @param obj The genlist object
 * @param no_select The no select mode
 * (EINA_TRUE = on, EINA_FALSE = off)
 *
 * This will turn off the ability to select items entirely and they
 * will neither appear selected nor call selected callback functions.
 *
 * @see elm_genlist_no_select_mode_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_no_select_mode_set(Evas_Object *obj, Eina_Bool no_select);

/**
 * Gets whether the no select mode is enabled.
 *
 * @param obj The genlist object
 * @return The no select mode
 * (@c EINA_TRUE = on, @c EINA_FALSE = off)
 *
 * @see elm_genlist_no_select_mode_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_no_select_mode_get(const Evas_Object *obj);

/**
 * Enable/disable compress mode.
 *
 * @param obj The genlist object
 * @param compress The compress mode
 * (@c EINA_TRUE = on, @c EINA_FALSE = off). Default is @c EINA_FALSE.
 *
 * This will enable the compress mode where items are "compressed"
 * horizontally to fit the genlist scrollable viewport width. This is
 * special for genlist.  Do not rely on
 * elm_genlist_horizontal_set() being set to @c ELM_LIST_COMPRESS to
 * work as genlist needs to handle it specially.
 *
 * @see elm_genlist_compress_mode_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_compress_mode_set(Evas_Object *obj, Eina_Bool compress);

/**
 * Get whether the compress mode is enabled.
 *
 * @param obj The genlist object
 * @return The compress mode
 * (@c EINA_TRUE = on, @c EINA_FALSE = off)
 *
 * @see elm_genlist_compress_mode_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_compress_mode_get(const Evas_Object *obj);

/**
 * Enable/disable height-for-width mode.
 *
 * @param obj The genlist object
 * @param setting The height-for-width mode (@c EINA_TRUE = on,
 * @c EINA_FALSE = off). Default is @c EINA_FALSE.
 *
 * With height-for-width mode the item width will be fixed (restricted
 * to a minimum of) to the list width when calculating its size in
 * order to allow the height to be calculated based on it. This allows,
 * for instance, text block to wrap lines if the Edje part is
 * configured with "text.min: 0 1".
 *
 * @note This mode will make list resize slower as it will have to
 *       recalculate every item height again whenever the list width
 *       changes!
 *
 * @note When height-for-width mode is enabled, it also enables
 *       compress mode (see elm_genlist_compress_mode_set()) and
 *       disables homogeneous (see elm_genlist_homogeneous_set()).
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_height_for_width_mode_set(Evas_Object *obj, Eina_Bool height_for_width);

/**
 * Get whether the height-for-width mode is enabled.
 *
 * @param obj The genlist object
 * @return The height-for-width mode (@c EINA_TRUE = on, @c EINA_FALSE =
 * off)
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_height_for_width_mode_get(const Evas_Object *obj);

/**
 * Enable/disable horizontal and vertical bouncing effect.
 *
 * @param obj The genlist object
 * @param h_bounce Allow bounce horizontally (@c EINA_TRUE = on, @c
 * EINA_FALSE = off). Default is @c EINA_FALSE.
 * @param v_bounce Allow bounce vertically (@c EINA_TRUE = on, @c
 * EINA_FALSE = off). Default is @c EINA_TRUE.
 *
 * This will enable or disable the scroller bouncing effect for the
 * genlist. See elm_scroller_bounce_set() for details.
 *
 * @see elm_scroller_bounce_set()
 * @see elm_genlist_bounce_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_bounce_set(Evas_Object *obj, Eina_Bool h_bounce, Eina_Bool v_bounce);

/**
 * Get whether the horizontal and vertical bouncing effect is enabled.
 *
 * @param obj The genlist object
 * @param h_bounce Pointer to a bool to receive if the bounce horizontally
 * option is set.
 * @param v_bounce Pointer to a bool to receive if the bounce vertically
 * option is set.
 *
 * @see elm_genlist_bounce_set()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_bounce_get(const Evas_Object *obj, Eina_Bool *h_bounce, Eina_Bool *v_bounce);

/**
 * Enable/disable homogeneous mode.
 *
 * @param obj The genlist object
 * @param homogeneous Assume the items within the genlist are of the
 * same height and width (EINA_TRUE = on, EINA_FALSE = off). Default is @c
 * EINA_FALSE.
 *
 * This will enable the homogeneous mode where items are of the same
 * height and width so that genlist may do the lazy-loading at its
 * maximum (which increases the performance for scrolling the list). This
 * implies 'compressed' mode.
 *
 * @see elm_genlist_compress_mode_set()
 * @see elm_genlist_homogeneous_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_homogeneous_set(Evas_Object *obj, Eina_Bool homogeneous);

/**
 * Get whether the homogeneous mode is enabled.
 *
 * @param obj The genlist object
 * @return Assume the items within the genlist are of the same height
 * and width (EINA_TRUE = on, EINA_FALSE = off)
 *
 * @see elm_genlist_homogeneous_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_homogeneous_get(const Evas_Object *obj);

/**
 * Set the maximum number of items within an item block
 *
 * @param obj The genlist object
 * @param n   Maximum number of items within an item block. Default is 32.
 *
 * This will configure the block count to tune to the target with
 * particular performance matrix.
 *
 * A block of objects will be used to reduce the number of operations due to
 * many objects in the screen. It can determine the visibility, or if the
 * object has changed, it theme needs to be updated, etc. doing this kind of
 * calculation to the entire block, instead of per object.
 *
 * The default value for the block count is enough for most lists, so unless
 * you know you will have a lot of objects visible in the screen at the same
 * time, don't try to change this.
 *
 * @see elm_genlist_block_count_get()
 * @see @ref Genlist_Implementation
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_block_count_set(Evas_Object *obj, int n);

/**
 * Get the maximum number of items within an item block
 *
 * @param obj The genlist object
 * @return Maximum number of items within an item block
 *
 * @see elm_genlist_block_count_set()
 *
 * @ingroup Genlist
 */
EAPI int                           elm_genlist_block_count_get(const Evas_Object *obj);

/**
 * Set the timeout in seconds for the longpress event.
 *
 * @param obj The genlist object
 * @param timeout timeout in seconds. Default is 1.
 *
 * This option will change how long it takes to send an event "longpressed"
 * after the mouse down signal is sent to the list. If this event occurs, no
 * "clicked" event will be sent.
 *
 * @see elm_genlist_longpress_timeout_set()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_longpress_timeout_set(Evas_Object *obj, double timeout);

/**
 * Get the timeout in seconds for the longpress event.
 *
 * @param obj The genlist object
 * @return timeout in seconds
 *
 * @see elm_genlist_longpress_timeout_get()
 *
 * @ingroup Genlist
 */
EAPI double                        elm_genlist_longpress_timeout_get(const Evas_Object *obj);

/**
 * Append a new item in a given genlist widget.
 *
 * @param obj The genlist object
 * @param itc The item class for the item
 * @param data The item data
 * @param parent The parent item, or NULL if none
 * @param flags Item flags
 * @param func Convenience function called when the item is selected
 * @param func_data Data passed to @p func above.
 * @return A handle to the item added or @c NULL if not possible
 *
 * This adds the given item to the end of the list or the end of
 * the children list if the @p parent is given.
 *
 * @see elm_genlist_item_prepend()
 * @see elm_genlist_item_insert_before()
 * @see elm_genlist_item_insert_after()
 * @see elm_genlist_item_del()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_append(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item_Flags flags, Evas_Smart_Cb func, const void *func_data);

/**
 * Prepend a new item in a given genlist widget.
 *
 * @param obj The genlist object
 * @param itc The item class for the item
 * @param data The item data
 * @param parent The parent item, or NULL if none
 * @param flags Item flags
 * @param func Convenience function called when the item is selected
 * @param func_data Data passed to @p func above.
 * @return A handle to the item added or NULL if not possible
 *
 * This adds an item to the beginning of the list or beginning of the
 * children of the parent if given.
 *
 * @see elm_genlist_item_append()
 * @see elm_genlist_item_insert_before()
 * @see elm_genlist_item_insert_after()
 * @see elm_genlist_item_del()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_prepend(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item_Flags flags, Evas_Smart_Cb func, const void *func_data);

/**
 * Insert an item before another in a genlist widget
 *
 * @param obj The genlist object
 * @param itc The item class for the item
 * @param data The item data
 * @param before The item to place this new one before.
 * @param flags Item flags
 * @param func Convenience function called when the item is selected
 * @param func_data Data passed to @p func above.
 * @return A handle to the item added or @c NULL if not possible
 *
 * This inserts an item before another in the list. It will be in the
 * same tree level or group as the item it is inserted before.
 *
 * @see elm_genlist_item_append()
 * @see elm_genlist_item_prepend()
 * @see elm_genlist_item_insert_after()
 * @see elm_genlist_item_del()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_insert_before(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item *before, Elm_Genlist_Item_Flags flags, Evas_Smart_Cb func, const void *func_data);

/**
 * Insert an item after another in a genlist widget
 *
 * @param obj The genlist object
 * @param itc The item class for the item
 * @param data The item data
 * @param after The item to place this new one after.
 * @param flags Item flags
 * @param func Convenience function called when the item is selected
 * @param func_data Data passed to @p func above.
 * @return A handle to the item added or @c NULL if not possible
 *
 * This inserts an item after another in the list. It will be in the
 * same tree level or group as the item it is inserted after.
 *
 * @see elm_genlist_item_append()
 * @see elm_genlist_item_prepend()
 * @see elm_genlist_item_insert_before()
 * @see elm_genlist_item_del()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_insert_after(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item *after, Elm_Genlist_Item_Flags flags, Evas_Smart_Cb func, const void *func_data);

/**
 * Insert a new item into the sorted genlist object
 *
 * @param obj The genlist object
 * @param itc The item class for the item
 * @param data The item data
 * @param parent The parent item, or NULL if none
 * @param flags Item flags
 * @param comp The function called for the sort
 * @param func Convenience function called when item selected
 * @param func_data Data passed to @p func above.
 * @return A handle to the item added or NULL if not possible
 *
 * @ingroup Genlist
 */
// XXX: deprecate elm_genlist_item_sorted_insert() and rename
// elm_genlist_item_direct_sorted_insert() 
EAPI Elm_Genlist_Item             *elm_genlist_item_sorted_insert(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item_Flags flags, Eina_Compare_Cb comp, Evas_Smart_Cb func, const void *func_data);
EAPI Elm_Genlist_Item             *elm_genlist_item_direct_sorted_insert(Evas_Object *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Genlist_Item *parent, Elm_Genlist_Item_Flags flags, Eina_Compare_Cb comp, Evas_Smart_Cb func, const void *func_data);

/* operations to retrieve existing items */
/**
 * Get the selectd item in the genlist.
 *
 * @param obj The genlist object
 * @return The selected item, or NULL if none is selected.
 *
 * This gets the selected item in the list (if multi-selection is enabled, only
 * the item that was first selected in the list is returned - which is not very
 * useful, so see elm_genlist_selected_items_get() for when multi-selection is
 * used).
 *
 * If no item is selected, NULL is returned.
 *
 * @see elm_genlist_selected_items_get()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_selected_item_get(const Evas_Object *obj);

/**
 * Get a list of selected items in the genlist.
 *
 * @param obj The genlist object
 * @return The list of selected items, or NULL if none are selected.
 *
 * It returns a list of the selected items. This list pointer is only valid so
 * long as the selection doesn't change (no items are selected or unselected, or
 * unselected implicitly by deletion). The list contains Elm_Genlist_Item
 * pointers. The order of the items in this list is the order which they were
 * selected, i.e. the first item in this list is the first item that was
 * selected, and so on.
 *
 * @note If not in multi-select mode, consider using function
 * elm_genlist_selected_item_get() instead.
 *
 * @see elm_genlist_multi_select_set()
 * @see elm_genlist_selected_item_get()
 *
 * @ingroup Genlist
 */
EAPI const Eina_List              *elm_genlist_selected_items_get(const Evas_Object *obj);

/**
 * Get the mode item style of items in the genlist
 * @param obj The genlist object
 * @return The mode item style string, or NULL if none is specified
 *
 * This is a constant string and simply defines the name of the
 * style that will be used for mode animations. It can be
 * @c NULL if you don't plan to use Genlist mode. See
 * elm_genlist_item_mode_set() for more info.
 *
 * @ingroup Genlist
 */
EAPI const char                   *elm_genlist_mode_item_style_get(const Evas_Object *obj);

/**
 * Set the mode item style of items in the genlist
 * @param obj The genlist object
 * @param style The mode item style string, or NULL if none is desired
 *
 * This is a constant string and simply defines the name of the
 * style that will be used for mode animations. It can be
 * @c NULL if you don't plan to use Genlist mode. See
 * elm_genlist_item_mode_set() for more info.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_mode_item_style_set(Evas_Object *obj, const char *style);

/**
 * Get a list of realized items in genlist
 *
 * @param obj The genlist object
 * @return The list of realized items, nor NULL if none are realized.
 *
 * This returns a list of the realized items in the genlist. The list
 * contains Elm_Genlist_Item pointers. The list must be freed by the
 * caller when done with eina_list_free(). The item pointers in the
 * list are only valid so long as those items are not deleted or the
 * genlist is not deleted.
 *
 * @see elm_genlist_realized_items_update()
 *
 * @ingroup Genlist
 */
EAPI Eina_List                    *elm_genlist_realized_items_get(const Evas_Object *obj);

/**
 * Get the item that is at the x, y canvas coords.
 *
 * @param obj The gelinst object.
 * @param x The input x coordinate
 * @param y The input y coordinate
 * @param posret The position relative to the item returned here
 * @return The item at the coordinates or NULL if none
 *
 * This returns the item at the given coordinates (which are canvas
 * relative, not object-relative). If an item is at that coordinate,
 * that item handle is returned, and if @p posret is not NULL, the
 * integer pointed to is set to a value of -1, 0 or 1, depending if
 * the coordinate is on the upper portion of that item (-1), on the
 * middle section (0) or on the lower part (1). If NULL is returned as
 * an item (no item found there), then posret may indicate -1 or 1
 * based if the coordinate is above or below all items respectively in
 * the genlist.
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_at_xy_item_get(const Evas_Object *obj, Evas_Coord x, Evas_Coord y, int *posret);

/**
 * Get the first item in the genlist
 *
 * This returns the first item in the list.
 *
 * @param obj The genlist object
 * @return The first item, or NULL if none
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_first_item_get(const Evas_Object *obj);

/**
 * Get the last item in the genlist
 *
 * This returns the last item in the list.
 *
 * @return The last item, or NULL if none
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_last_item_get(const Evas_Object *obj);

/**
 * Set the scrollbar policy
 *
 * @param obj The genlist object
 * @param policy_h Horizontal scrollbar policy.
 * @param policy_v Vertical scrollbar policy.
 *
 * This sets the scrollbar visibility policy for the given genlist
 * scroller. #ELM_SMART_SCROLLER_POLICY_AUTO means the scrollbar is
 * made visible if it is needed, and otherwise kept hidden.
 * #ELM_SMART_SCROLLER_POLICY_ON turns it on all the time, and
 * #ELM_SMART_SCROLLER_POLICY_OFF always keeps it off. This applies
 * respectively for the horizontal and vertical scrollbars. Default is
 * #ELM_SMART_SCROLLER_POLICY_AUTO
 *
 * @see elm_genlist_scroller_policy_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_scroller_policy_set(Evas_Object *obj, Elm_Scroller_Policy policy_h, Elm_Scroller_Policy policy_v);

/**
 * Get the scrollbar policy
 *
 * @param obj The genlist object
 * @param policy_h Pointer to store the horizontal scrollbar policy.
 * @param policy_v Pointer to store the vertical scrollbar policy.
 *
 * @see elm_genlist_scroller_policy_set()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_scroller_policy_get(const Evas_Object *obj, Elm_Scroller_Policy *policy_h, Elm_Scroller_Policy *policy_v);

/**
 * Get the @b next item in a genlist widget's internal list of items,
 * given a handle to one of those items.
 *
 * @param item The genlist item to fetch next from
 * @return The item after @p item, or @c NULL if there's none (and
 * on errors)
 *
 * This returns the item placed after the @p item, on the container
 * genlist.
 *
 * @see elm_genlist_item_prev_get()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_next_get(const Elm_Genlist_Item *item);

/**
 * Get the @b previous item in a genlist widget's internal list of items,
 * given a handle to one of those items.
 *
 * @param item The genlist item to fetch previous from
 * @return The item before @p item, or @c NULL if there's none (and
 * on errors)
 *
 * This returns the item placed before the @p item, on the container
 * genlist.
 *
 * @see elm_genlist_item_next_get()
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_prev_get(const Elm_Genlist_Item *item);

/**
 * Get the genlist object's handle which contains a given genlist
 * item
 *
 * @param item The item to fetch the container from
 * @return The genlist (parent) object
 *
 * This returns the genlist object itself that an item belongs to.
 *
 * @ingroup Genlist
 */
EAPI Evas_Object                  *elm_genlist_item_genlist_get(const Elm_Genlist_Item *item);

/**
 * Get the parent item of the given item
 *
 * @param it The item
 * @return The parent of the item or @c NULL if it has no parent.
 *
 * This returns the item that was specified as parent of the item @p it on
 * elm_genlist_item_append() and insertion related functions.
 *
 * @ingroup Genlist
 */
EAPI Elm_Genlist_Item             *elm_genlist_item_parent_get(const Elm_Genlist_Item *it);

/**
 * Remove all sub-items (children) of the given item
 *
 * @param it The item
 *
 * This removes all items that are children (and their descendants) of the
 * given item @p it.
 *
 * @see elm_genlist_clear()
 * @see elm_genlist_item_del()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_subitems_clear(Elm_Genlist_Item *item);

/**
 * Set whether a given genlist item is selected or not
 *
 * @param it The item
 * @param selected Use @c EINA_TRUE, to make it selected, @c
 * EINA_FALSE to make it unselected
 *
 * This sets the selected state of an item. If multi selection is
 * not enabled on the containing genlist and @p selected is @c
 * EINA_TRUE, any other previously selected items will get
 * unselected in favor of this new one.
 *
 * @see elm_genlist_item_selected_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_selected_set(Elm_Genlist_Item *item, Eina_Bool selected);

/**
 * Get whether a given genlist item is selected or not
 *
 * @param it The item
 * @return @c EINA_TRUE, if it's selected, @c EINA_FALSE otherwise
 *
 * @see elm_genlist_item_selected_set() for more details
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_item_selected_get(const Elm_Genlist_Item *item);

/**
 * Sets the expanded state of an item.
 *
 * @param it The item
 * @param expanded The expanded state (@c EINA_TRUE expanded, @c EINA_FALSE not expanded).
 *
 * This function flags the item of type #ELM_GENLIST_ITEM_SUBITEMS as
 * expanded or not.
 *
 * The theme will respond to this change visually, and a signal "expanded" or
 * "contracted" will be sent from the genlist with a pointer to the item that
 * has been expanded/contracted.
 *
 * Calling this function won't show or hide any child of this item (if it is
 * a parent). You must manually delete and create them on the callbacks fo
 * the "expanded" or "contracted" signals.
 *
 * @see elm_genlist_item_expanded_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_expanded_set(Elm_Genlist_Item *item, Eina_Bool expanded);

/**
 * Get the expanded state of an item
 *
 * @param it The item
 * @return The expanded state
 *
 * This gets the expanded state of an item.
 *
 * @see elm_genlist_item_expanded_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_item_expanded_get(const Elm_Genlist_Item *item);

/**
 * Get the depth of expanded item
 *
 * @param it The genlist item object
 * @return The depth of expanded item
 *
 * @ingroup Genlist
 */
EAPI int                           elm_genlist_item_expanded_depth_get(const Elm_Genlist_Item *it);

/**
 * Set whether a given genlist item is disabled or not.
 *
 * @param it The item
 * @param disabled Use @c EINA_TRUE, true disable it, @c EINA_FALSE
 * to enable it back.
 *
 * A disabled item cannot be selected or unselected. It will also
 * change its appearance, to signal the user it's disabled.
 *
 * @see elm_genlist_item_disabled_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_disabled_set(Elm_Genlist_Item *item, Eina_Bool disabled);

/**
 * Get whether a given genlist item is disabled or not.
 *
 * @param it The item
 * @return @c EINA_TRUE, if it's disabled, @c EINA_FALSE otherwise
 * (and on errors).
 *
 * @see elm_genlist_item_disabled_set() for more details
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_item_disabled_get(const Elm_Genlist_Item *item);

/**
 * Sets the display only state of an item.
 *
 * @param it The item
 * @param display_only @c EINA_TRUE if the item is display only, @c
 * EINA_FALSE otherwise.
 *
 * A display only item cannot be selected or unselected. It is for
 * display only and not selecting or otherwise clicking, dragging
 * etc. by the user, thus finger size rules will not be applied to
 * this item.
 *
 * It's good to set group index items to display only state.
 *
 * @see elm_genlist_item_display_only_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_display_only_set(Elm_Genlist_Item *it, Eina_Bool display_only);

/**
 * Get the display only state of an item
 *
 * @param it The item
 * @return @c EINA_TRUE if the item is display only, @c
 * EINA_FALSE otherwise.
 *
 * @see elm_genlist_item_display_only_set()
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_item_display_only_get(const Elm_Genlist_Item *it);

/**
 * Show the portion of a genlist's internal list containing a given
 * item, immediately.
 *
 * @param it The item to display
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * immediately scrolling to that position), if it is not fully visible.
 *
 * @see elm_genlist_item_bring_in()
 * @see elm_genlist_item_top_show()
 * @see elm_genlist_item_middle_show()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_show(Elm_Genlist_Item *item);

/**
 * Animatedly bring in, to the visible are of a genlist, a given
 * item on it.
 *
 * @param it The item to display
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * animatedly scrolling), if it is not fully visible. This may use animation
 * to do so and take a period of time
 *
 * @see elm_genlist_item_show()
 * @see elm_genlist_item_top_bring_in()
 * @see elm_genlist_item_middle_bring_in()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_bring_in(Elm_Genlist_Item *item);

/**
 * Show the portion of a genlist's internal list containing a given
 * item, immediately.
 *
 * @param it The item to display
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * immediately scrolling to that position), if it is not fully visible.
 *
 * The item will be positioned at the top of the genlist viewport.
 *
 * @see elm_genlist_item_show()
 * @see elm_genlist_item_top_bring_in()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_top_show(Elm_Genlist_Item *item);

/**
 * Animatedly bring in, to the visible are of a genlist, a given
 * item on it.
 *
 * @param it The item
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * animatedly scrolling), if it is not fully visible. This may use animation
 * to do so and take a period of time
 *
 * The item will be positioned at the top of the genlist viewport.
 *
 * @see elm_genlist_item_bring_in()
 * @see elm_genlist_item_top_show()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_top_bring_in(Elm_Genlist_Item *item);

/**
 * Show the portion of a genlist's internal list containing a given
 * item, immediately.
 *
 * @param it The item to display
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * immediately scrolling to that position), if it is not fully visible.
 *
 * The item will be positioned at the middle of the genlist viewport.
 *
 * @see elm_genlist_item_show()
 * @see elm_genlist_item_middle_bring_in()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_middle_show(Elm_Genlist_Item *it);

/**
 * Animatedly bring in, to the visible are of a genlist, a given
 * item on it.
 *
 * @param it The item
 *
 * This causes genlist to jump to the given item @p it and show it (by
 * animatedly scrolling), if it is not fully visible. This may use animation
 * to do so and take a period of time
 *
 * The item will be positioned at the middle of the genlist viewport.
 *
 * @see elm_genlist_item_bring_in()
 * @see elm_genlist_item_middle_show()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_middle_bring_in(Elm_Genlist_Item *it);

/**
 * Remove a genlist item from the its parent, deleting it.
 *
 * @param item The item to be removed.
 * @return @c EINA_TRUE on success or @c EINA_FALSE, otherwise.
 *
 * @see elm_genlist_clear(), to remove all items in a genlist at
 * once.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_del(Elm_Genlist_Item *item);

/**
 * Return the data associated to a given genlist item
 *
 * @param item The genlist item.
 * @return the data associated to this item.
 *
 * This returns the @c data value passed on the
 * elm_genlist_item_append() and related item addition calls.
 *
 * @see elm_genlist_item_append()
 * @see elm_genlist_item_data_set()
 *
 * @ingroup Genlist
 */
EAPI void                         *elm_genlist_item_data_get(const Elm_Genlist_Item *item);

/**
 * Set the data associated to a given genlist item
 *
 * @param item The genlist item
 * @param data The new data pointer to set on it
 *
 * This @b overrides the @c data value passed on the
 * elm_genlist_item_append() and related item addition calls. This
 * function @b won't call elm_genlist_item_update() automatically,
 * so you'd issue it afterwards if you want to hove the item
 * updated to reflect the that new data.
 *
 * @see elm_genlist_item_data_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_data_set(Elm_Genlist_Item *it, const void *data);

/**
 * Tells genlist to "orphan" contents fetchs by the item class
 *
 * @param it The item
 *
 * This instructs genlist to release references to contents in the item,
 * meaning that they will no longer be managed by genlist and are
 * floating "orphans" that can be re-used elsewhere if the user wants
 * to.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_contents_orphan(Elm_Genlist_Item *it);

/**
 * Get the real Evas object created to implement the view of a
 * given genlist item
 *
 * @param item The genlist item.
 * @return the Evas object implementing this item's view.
 *
 * This returns the actual Evas object used to implement the
 * specified genlist item's view. This may be @c NULL, as it may
 * not have been created or may have been deleted, at any time, by
 * the genlist. <b>Do not modify this object</b> (move, resize,
 * show, hide, etc.), as the genlist is controlling it. This
 * function is for querying, emitting custom signals or hooking
 * lower level callbacks for events on that object. Do not delete
 * this object under any circumstances.
 *
 * @see elm_genlist_item_data_get()
 *
 * @ingroup Genlist
 */
EAPI const Evas_Object            *elm_genlist_item_object_get(const Elm_Genlist_Item *it);

/**
 * Update the contents of an item
 *
 * @param it The item
 *
 * This updates an item by calling all the item class functions again
 * to get the contents, texts and states. Use this when the original
 * item data has changed and the changes are desired to be reflected.
 *
 * Use elm_genlist_realized_items_update() to update all already realized
 * items.
 *
 * @see elm_genlist_realized_items_update()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_update(Elm_Genlist_Item *item);

/**
 * Promote an item to the top of the list
 *
 * @param it The item
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_promote(Elm_Gen_Item *it);

/**
 * Demote an item to the end of the list
 *
 * @param it The item
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_demote(Elm_Gen_Item *it);

/**
 * Update the part of an item
 *
 * @param it The item
 * @param parts The name of item's part
 * @param itf The flags of item's part type
 *
 * This updates an item's part by calling item's fetching functions again
 * to get the contents, texts and states. Use this when the original
 * item data has changed and the changes are desired to be reflected.
 * Second parts argument is used for globbing to match '*', '?', and '.'
 * It can be used at updating multi fields.
 *
 * Use elm_genlist_realized_items_update() to update an item's all
 * property.
 *
 * @see elm_genlist_item_update()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_fields_update(Elm_Genlist_Item *it, const char *parts, Elm_Genlist_Item_Field_Flags itf);

/**
 * Update the item class of an item
 *
 * @param it The item
 * @param itc The item class for the item
 *
 * This sets another class fo the item, changing the way that it is
 * displayed. After changing the item class, elm_genlist_item_update() is
 * called on the item @p it.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_item_class_update(Elm_Genlist_Item *it, const Elm_Genlist_Item_Class *itc);
EAPI const Elm_Genlist_Item_Class *elm_genlist_item_item_class_get(const Elm_Genlist_Item *it);

/**
 * Set the text to be shown in a given genlist item's tooltips.
 *
 * @param item The genlist item
 * @param text The text to set in the content
 *
 * This call will setup the text to be used as tooltip to that item
 * (analogous to elm_object_tooltip_text_set(), but being item
 * tooltips with higher precedence than object tooltips). It can
 * have only one tooltip at a time, so any previous tooltip data
 * will get removed.
 *
 * In order to set a content or something else as a tooltip, look at
 * elm_genlist_item_tooltip_content_cb_set().
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_tooltip_text_set(Elm_Genlist_Item *item, const char *text);

/**
 * Set the content to be shown in a given genlist item's tooltips
 *
 * @param item The genlist item.
 * @param func The function returning the tooltip contents.
 * @param data What to provide to @a func as callback data/context.
 * @param del_cb Called when data is not needed anymore, either when
 *        another callback replaces @p func, the tooltip is unset with
 *        elm_genlist_item_tooltip_unset() or the owner @p item
 *        dies. This callback receives as its first parameter the
 *        given @p data, being @c event_info the item handle.
 *
 * This call will setup the tooltip's contents to @p item
 * (analogous to elm_object_tooltip_content_cb_set(), but being
 * item tooltips with higher precedence than object tooltips). It
 * can have only one tooltip at a time, so any previous tooltip
 * content will get removed. @p func (with @p data) will be called
 * every time Elementary needs to show the tooltip and it should
 * return a valid Evas object, which will be fully managed by the
 * tooltip system, getting deleted when the tooltip is gone.
 *
 * In order to set just a text as a tooltip, look at
 * elm_genlist_item_tooltip_text_set().
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_tooltip_content_cb_set(Elm_Genlist_Item *item, Elm_Tooltip_Item_Content_Cb func, const void *data, Evas_Smart_Cb del_cb);

/**
 * Unset a tooltip from a given genlist item
 *
 * @param item genlist item to remove a previously set tooltip from.
 *
 * This call removes any tooltip set on @p item. The callback
 * provided as @c del_cb to
 * elm_genlist_item_tooltip_content_cb_set() will be called to
 * notify it is not used anymore (and have resources cleaned, if
 * need be).
 *
 * @see elm_genlist_item_tooltip_content_cb_set()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_tooltip_unset(Elm_Genlist_Item *item);

/**
 * Set a different @b style for a given genlist item's tooltip.
 *
 * @param item genlist item with tooltip set
 * @param style the <b>theme style</b> to use on tooltips (e.g. @c
 * "default", @c "transparent", etc)
 *
 * Tooltips can have <b>alternate styles</b> to be displayed on,
 * which are defined by the theme set on Elementary. This function
 * works analogously as elm_object_tooltip_style_set(), but here
 * applied only to genlist item objects. The default style for
 * tooltips is @c "default".
 *
 * @note before you set a style you should define a tooltip with
 *       elm_genlist_item_tooltip_content_cb_set() or
 *       elm_genlist_item_tooltip_text_set()
 *
 * @see elm_genlist_item_tooltip_style_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_tooltip_style_set(Elm_Genlist_Item *item, const char *style);

/**
 * Get the style set a given genlist item's tooltip.
 *
 * @param item genlist item with tooltip already set on.
 * @return style the theme style in use, which defaults to
 *         "default". If the object does not have a tooltip set,
 *         then @c NULL is returned.
 *
 * @see elm_genlist_item_tooltip_style_set() for more details
 *
 * @ingroup Genlist
 */
EAPI const char                   *elm_genlist_item_tooltip_style_get(const Elm_Genlist_Item *item);

/**
 * @brief Disable size restrictions on an object's tooltip
 * @param item The tooltip's anchor object
 * @param disable If EINA_TRUE, size restrictions are disabled
 * @return EINA_FALSE on failure, EINA_TRUE on success
 *
 * This function allows a tooltip to expand beyond its parant window's canvas.
 * It will instead be limited only by the size of the display.
 */
EAPI Eina_Bool                     elm_genlist_item_tooltip_window_mode_set(Elm_Genlist_Item *item, Eina_Bool disable);

/**
 * @brief Retrieve size restriction state of an object's tooltip
 * @param item The tooltip's anchor object
 * @return If EINA_TRUE, size restrictions are disabled
 *
 * This function returns whether a tooltip is allowed to expand beyond
 * its parant window's canvas.
 * It will instead be limited only by the size of the display.
 */
EAPI Eina_Bool                     elm_genlist_item_tooltip_window_mode_get(const Elm_Genlist_Item *item);

/**
 * Set the type of mouse pointer/cursor decoration to be shown,
 * when the mouse pointer is over the given genlist widget item
 *
 * @param item genlist item to customize cursor on
 * @param cursor the cursor type's name
 *
 * This function works analogously as elm_object_cursor_set(), but
 * here the cursor's changing area is restricted to the item's
 * area, and not the whole widget's. Note that that item cursors
 * have precedence over widget cursors, so that a mouse over @p
 * item will always show cursor @p type.
 *
 * If this function is called twice for an object, a previously set
 * cursor will be unset on the second call.
 *
 * @see elm_object_cursor_set()
 * @see elm_genlist_item_cursor_get()
 * @see elm_genlist_item_cursor_unset()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_cursor_set(Elm_Genlist_Item *item, const char *cursor);

/**
 * Get the type of mouse pointer/cursor decoration set to be shown,
 * when the mouse pointer is over the given genlist widget item
 *
 * @param item genlist item with custom cursor set
 * @return the cursor type's name or @c NULL, if no custom cursors
 * were set to @p item (and on errors)
 *
 * @see elm_object_cursor_get()
 * @see elm_genlist_item_cursor_set() for more details
 * @see elm_genlist_item_cursor_unset()
 *
 * @ingroup Genlist
 */
EAPI const char                   *elm_genlist_item_cursor_get(const Elm_Genlist_Item *item);

/**
 * Unset any custom mouse pointer/cursor decoration set to be
 * shown, when the mouse pointer is over the given genlist widget
 * item, thus making it show the @b default cursor again.
 *
 * @param item a genlist item
 *
 * Use this call to undo any custom settings on this item's cursor
 * decoration, bringing it back to defaults (no custom style set).
 *
 * @see elm_object_cursor_unset()
 * @see elm_genlist_item_cursor_set() for more details
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_cursor_unset(Elm_Genlist_Item *item);

/**
 * Set a different @b style for a given custom cursor set for a
 * genlist item.
 *
 * @param item genlist item with custom cursor set
 * @param style the <b>theme style</b> to use (e.g. @c "default",
 * @c "transparent", etc)
 *
 * This function only makes sense when one is using custom mouse
 * cursor decorations <b>defined in a theme file</b> , which can
 * have, given a cursor name/type, <b>alternate styles</b> on
 * it. It works analogously as elm_object_cursor_style_set(), but
 * here applied only to genlist item objects.
 *
 * @warning Before you set a cursor style you should have defined a
 *       custom cursor previously on the item, with
 *       elm_genlist_item_cursor_set()
 *
 * @see elm_genlist_item_cursor_engine_only_set()
 * @see elm_genlist_item_cursor_style_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_cursor_style_set(Elm_Genlist_Item *item, const char *style);

/**
 * Get the current @b style set for a given genlist item's custom
 * cursor
 *
 * @param item genlist item with custom cursor set.
 * @return style the cursor style in use. If the object does not
 *         have a cursor set, then @c NULL is returned.
 *
 * @see elm_genlist_item_cursor_style_set() for more details
 *
 * @ingroup Genlist
 */
EAPI const char                   *elm_genlist_item_cursor_style_get(const Elm_Genlist_Item *item);

/**
 * Set if the (custom) cursor for a given genlist item should be
 * searched in its theme, also, or should only rely on the
 * rendering engine.
 *
 * @param item item with custom (custom) cursor already set on
 * @param engine_only Use @c EINA_TRUE to have cursors looked for
 * only on those provided by the rendering engine, @c EINA_FALSE to
 * have them searched on the widget's theme, as well.
 *
 * @note This call is of use only if you've set a custom cursor
 * for genlist items, with elm_genlist_item_cursor_set().
 *
 * @note By default, cursors will only be looked for between those
 * provided by the rendering engine.
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_cursor_engine_only_set(Elm_Genlist_Item *item, Eina_Bool engine_only);

/**
 * Get if the (custom) cursor for a given genlist item is being
 * searched in its theme, also, or is only relying on the rendering
 * engine.
 *
 * @param item a genlist item
 * @return @c EINA_TRUE, if cursors are being looked for only on
 * those provided by the rendering engine, @c EINA_FALSE if they
 * are being searched on the widget's theme, as well.
 *
 * @see elm_genlist_item_cursor_engine_only_set(), for more details
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_item_cursor_engine_only_get(const Elm_Genlist_Item *item);

/**
 * Get the index of the item. It is only valid once displayed.
 *
 * @param item a genlist item
 * @return the position inside the list of item.
 *
 * @ingroup Genlist
 */
EAPI int                           elm_genlist_item_index_get(Elm_Gen_Item *it);

/**
 * Update the contents of all realized items.
 *
 * @param obj The genlist object.
 *
 * This updates all realized items by calling all the item class functions again
 * to get the contents, texts and states. Use this when the original
 * item data has changed and the changes are desired to be reflected.
 *
 * To update just one item, use elm_genlist_item_update().
 *
 * @see elm_genlist_realized_items_get()
 * @see elm_genlist_item_update()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_realized_items_update(Evas_Object *obj);

/**
 * Activate a genlist mode on an item
 *
 * @param item The genlist item
 * @param mode Mode name
 * @param mode_set Boolean to define set or unset mode.
 *
 * A genlist mode is a different way of selecting an item. Once a mode is
 * activated on an item, any other selected item is immediately unselected.
 * This feature provides an easy way of implementing a new kind of animation
 * for selecting an item, without having to entirely rewrite the item style
 * theme. However, the elm_genlist_selected_* API can't be used to get what
 * item is activate for a mode.
 *
 * The current item style will still be used, but applying a genlist mode to
 * an item will select it using a different kind of animation.
 *
 * The current active item for a mode can be found by
 * elm_genlist_mode_item_get().
 *
 * The characteristics of genlist mode are:
 * - Only one mode can be active at any time, and for only one item.
 * - Genlist handles deactivating other items when one item is activated.
 * - A mode is defined in the genlist theme (edc), and more modes can easily
 *   be added.
 * - A mode style and the genlist item style are different things. They
 *   can be combined to provide a default style to the item, with some kind
 *   of animation for that item when the mode is activated.
 *
 * When a mode is activated on an item, a new view for that item is created.
 * The theme of this mode defines the animation that will be used to transit
 * the item from the old view to the new view. This second (new) view will be
 * active for that item while the mode is active on the item, and will be
 * destroyed after the mode is totally deactivated from that item.
 *
 * @see elm_genlist_mode_get()
 * @see elm_genlist_mode_item_get()
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_item_mode_set(Elm_Genlist_Item *it, const char *mode_type, Eina_Bool mode_set);

/**
 * Get the last (or current) genlist mode used.
 *
 * @param obj The genlist object
 *
 * This function just returns the name of the last used genlist mode. It will
 * be the current mode if it's still active.
 *
 * @see elm_genlist_item_mode_set()
 * @see elm_genlist_mode_item_get()
 *
 * @ingroup Genlist
 */
EAPI const char                   *elm_genlist_mode_get(const Evas_Object *obj);

/**
 * Get active genlist mode item
 *
 * @param obj The genlist object
 * @return The active item for that current mode. Or @c NULL if no item is
 * activated with any mode.
 *
 * This function returns the item that was activated with a mode, by the
 * function elm_genlist_item_mode_set().
 *
 * @see elm_genlist_item_mode_set()
 * @see elm_genlist_mode_get()
 *
 * @ingroup Genlist
 */
EAPI const Elm_Genlist_Item       *elm_genlist_mode_item_get(const Evas_Object *obj);

/**
 * Set reorder mode
 *
 * @param obj The genlist object
 * @param reorder_mode The reorder mode
 * (EINA_TRUE = on, EINA_FALSE = off)
 *
 * @ingroup Genlist
 */
EAPI void                          elm_genlist_reorder_mode_set(Evas_Object *obj, Eina_Bool reorder_mode);

/**
 * Get the reorder mode
 *
 * @param obj The genlist object
 * @return The reorder mode
 * (EINA_TRUE = on, EINA_FALSE = off)
 *
 * @ingroup Genlist
 */
EAPI Eina_Bool                     elm_genlist_reorder_mode_get(const Evas_Object *obj);

/**
 * @}
 */
