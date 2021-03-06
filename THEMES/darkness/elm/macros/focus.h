#define FOCUS_GLOW(x1, y1, offx1, offy1, to1, x2, y2, offx2, offy2, to2) \
part { \
   name: to1"_focus_glow"; \
   mouse_events: 0; \
   description { \
      state: "default" 0.0; \
      visible: 0; \
      color: 255 255 255 0; \
      rel1 { \
         relative: x1 y1; \
         offset: offx1 offy1; \
         to: to1;\
      } \
      rel2 { \
         relative: x2 y2; \
         offset: offx2 offy2; \
         to: to2; \
      } \
      image { \
         normal: "focus_glow.png"; \
         border: 7 7 7 7; \
         middle: 0; \
      } \
      fill { \
         smooth: 1; \
      } \
   } \
   description { \
      state: "focused" 0.0; \
      inherit: "default" 0.0; \
      visible: 1; \
      color: 255 255 255 200; \
   } \
}

#define FOCUS_SIGNAL \
signal: "elm,action,focus"; \
source: "elm";

#define UNFOCUS_SIGNAL \
signal: "elm,action,unfocus"; \
source: "elm";

#define FOCUS_GLOW_ACTION(NAME) \
action: STATE_SET "focused" 0.0; \
transition: DECELERATE 0.2; \
target: NAME"_focus_glow";

#define UNFOCUS_GLOW_ACTION(NAME) \
action: STATE_SET "default" 0.0; \
transition: ACCELERATE 0.5; \
target: NAME"_focus_glow";

#define FOCUS_RECT_PART(part_name, to_part, x1, y1, offx1, offy1, x2, y2, offx2, offy2) \
part { \
   name: part_name; \
   mouse_events: 0; \
   description { \
      state: "default" 0.0; \
      visible: 0; \
      color: 255 255 255 0; \
      rel1 { \
         relative: x1 y1; \
         offset: offx1 offy1; \
         to: to_part;\
      } \
      rel2 { \
         relative: x2 y2; \
         offset: offx2 offy2; \
         to: to_part; \
      } \
      image { \
         normal: "focus_rect.png"; \
      } \
      fill { \
         smooth: 0; \
         size { \
            relative: 0.0 0.0; \
            offset: 32 32; \
         } \
      } \
   } \
   description { \
      state: "focused" 0.0; \
      inherit: "default" 0.0; \
      visible: 1; \
      color: 255 255 255 155; \
   } \
}

#define FOCUS_RECT(to_part, x1, y1, offx1, offy1, x2, y2, offx2, offy2) \
FOCUS_RECT_PART("focus_rect_top", to_part, x1, y1, offx1, offy1, x2, y1, offx2, offy1) \
FOCUS_RECT_PART("focus_rect_left", to_part, x1, y1, offx1, (offy1 + 1), x1, y2, offx1, (offy2 - 1)) \
FOCUS_RECT_PART("focus_rect_right", to_part, x2, y1, offx2, (offy1 + 1), x2, y2, offx2, (offy2 - 1)) \
FOCUS_RECT_PART("focus_rect_bottom", to_part, x1, y2, offx1, offy2, x2, y2, offx2, offy2)

#define FOCUS_RECT_ACTION \
action: STATE_SET "focused" 0.0; \
transition: LINEAR 0.2; \
target: "focus_rect_top"; \
target: "focus_rect_left"; \
target: "focus_rect_right"; \
target: "focus_rect_bottom";

#define UNFOCUS_RECT_ACTION \
action: STATE_SET "default" 0.0; \
transition: LINEAR 0.2; \
target: "focus_rect_top"; \
target: "focus_rect_left"; \
target: "focus_rect_right"; \
target: "focus_rect_bottom";
