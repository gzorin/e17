#define DRAG_GLOW(x1, y1, offx1, offy1, to1, x2, y2, offx2, offy2, to2) \
part { \
   name: to1"_drag_glow"; \
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
         normal: "drag_glow.png"; \
         border: 7 7 7 7; \
      } \
      fill { \
         smooth: 0; \
      } \
   } \
   description { \
      state: "visible" 0.0; \
      inherit: "default" 0.0; \
      visible: 1; \
      color: 255 255 255 255; \
   } \
}

#define DRAG_ENTER_ACTION(NAME) \
action: STATE_SET "visible" 0.0; \
transition: DECELERATE 0.2; \
target: NAME"_drag_glow";

#define DRAG_LEAVE_ACTION(NAME) \
action: STATE_SET "default" 0.0; \
transition: ACCELERATE 0.5; \
target: NAME"_drag_glow";
