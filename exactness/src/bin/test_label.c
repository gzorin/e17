#include <Elementary.h>
#include "tsuite.h"
TEST_START(test_label)
{
   Evas_Object *bg, *bx, *lb;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);

   lb = elm_label_add(win);
   elm_object_text_set(lb,
                       "<b>This is a small label</b>"
                       );
   evas_object_size_hint_weight_set(lb, 0.0, 0.0);
   evas_object_size_hint_align_set(lb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, lb);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_object_text_set(lb,
                       "This is a larger label with newlines<br>"
                       "to make it bigger, bit it won't expand or wrap<br>"
                       "just be a block of text that can't change its<br>"
                       "formatting as it's fixed based on text<br>"
                       );
   evas_object_size_hint_weight_set(lb, 0.0, 0.0);
   evas_object_size_hint_align_set(lb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, lb);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_label_line_wrap_set(lb, ELM_WRAP_CHAR);
   elm_object_text_set(lb,
                       "<b>"
                       "This is more text designed to line-wrap here as "
                       "This object is resized horizontally. As it is "
                       "resized vertically though, nothing should change. "
                       "The amount of space allocated vertically should "
                       "change as horizontal size changes."
                       "</b>"
                       );
   evas_object_size_hint_weight_set(lb, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(lb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, lb);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_object_text_set(lb,
                       "This small label set to wrap"
                       );
   evas_object_size_hint_weight_set(lb, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(lb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, lb);
   evas_object_show(lb);

   evas_object_resize(win, 320, 300);

   evas_object_show(bx);
   evas_object_show(win);
   elm_win_resize_object_add(win, bx);
}
TEST_END
