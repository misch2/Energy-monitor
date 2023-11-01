// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: Energy_monitor

#include "../ui.h"

void ui_OKScreen_screen_init(void)
{
ui_OKScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_OKScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_text_font(ui_OKScreen, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PanelTopOK = lv_obj_create(ui_OKScreen);
lv_obj_set_width( ui_PanelTopOK, 320);
lv_obj_set_height( ui_PanelTopOK, 115);
lv_obj_set_x( ui_PanelTopOK, 0 );
lv_obj_set_y( ui_PanelTopOK, 70 );
lv_obj_set_align( ui_PanelTopOK, LV_ALIGN_TOP_MID );
lv_obj_add_flag( ui_PanelTopOK, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_PanelTopOK, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_PanelTopOK, lv_color_hex(0x007F00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PanelTopOK, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_PanelTopOK, lv_color_hex(0x7FFF7F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_PanelTopOK, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_PanelTopOK, 1, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelRemainingWattsOK = lv_label_create(ui_PanelTopOK);
lv_obj_set_width( ui_LabelRemainingWattsOK, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelRemainingWattsOK, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelRemainingWattsOK, 0 );
lv_obj_set_y( ui_LabelRemainingWattsOK, -3 );
lv_obj_set_align( ui_LabelRemainingWattsOK, LV_ALIGN_BOTTOM_MID );
lv_label_set_text(ui_LabelRemainingWattsOK,"??? W");
lv_obj_set_style_text_font(ui_LabelRemainingWattsOK, &ui_font_Calibri48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label3 = lv_label_create(ui_PanelTopOK);
lv_obj_set_width( ui_Label3, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label3, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label3, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_Label3,"Rezerva");
lv_obj_set_style_text_color(ui_Label3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ArcCurrentWattsOK = lv_arc_create(ui_OKScreen);
lv_obj_set_width( ui_ArcCurrentWattsOK, 200);
lv_obj_set_height( ui_ArcCurrentWattsOK, 200);
lv_obj_set_x( ui_ArcCurrentWattsOK, 0 );
lv_obj_set_y( ui_ArcCurrentWattsOK, -20 );
lv_obj_set_align( ui_ArcCurrentWattsOK, LV_ALIGN_BOTTOM_MID );
lv_obj_add_flag( ui_ArcCurrentWattsOK, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_ArcCurrentWattsOK, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE );    /// Flags
lv_arc_set_value(ui_ArcCurrentWattsOK, 15);
lv_obj_set_style_arc_color(ui_ArcCurrentWattsOK, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_ArcCurrentWattsOK, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_arc_color(ui_ArcCurrentWattsOK, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_ArcCurrentWattsOK, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_ArcCurrentWattsOK, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ArcCurrentWattsOK, 0, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_LabelWattsUsedOK = lv_label_create(ui_ArcCurrentWattsOK);
lv_obj_set_width( ui_LabelWattsUsedOK, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelWattsUsedOK, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_LabelWattsUsedOK, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelWattsUsedOK,"??? W");
lv_obj_set_style_text_font(ui_LabelWattsUsedOK, &ui_font_Calibri32, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label5 = lv_label_create(ui_OKScreen);
lv_obj_set_width( ui_Label5, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label5, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label5, 0 );
lv_obj_set_y( ui_Label5, -233 );
lv_obj_set_align( ui_Label5, LV_ALIGN_BOTTOM_MID );
lv_label_set_text(ui_Label5,"Aktuální spotřeba");
lv_obj_add_flag( ui_Label5, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label5, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_OKScreen, ui_event_OKScreen, LV_EVENT_ALL, NULL);

}
