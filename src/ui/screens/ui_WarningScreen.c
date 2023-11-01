// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: Energy_monitor

#include "../ui.h"

void ui_WarningScreen_screen_init(void)
{
ui_WarningScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_WarningScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_text_font(ui_WarningScreen, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PanelTopWarning = lv_obj_create(ui_WarningScreen);
lv_obj_set_width( ui_PanelTopWarning, 320);
lv_obj_set_height( ui_PanelTopWarning, 320);
lv_obj_set_align( ui_PanelTopWarning, LV_ALIGN_TOP_MID );
lv_obj_add_flag( ui_PanelTopWarning, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_PanelTopWarning, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_PanelTopWarning, lv_color_hex(0xA80000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PanelTopWarning, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_PanelTopWarning, lv_color_hex(0xFF8080), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_PanelTopWarning, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_PanelTopWarning, 1, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TextAreaAppliancesWarning = lv_textarea_create(ui_PanelTopWarning);
lv_obj_set_height( ui_TextAreaAppliancesWarning, 202);
lv_obj_set_width( ui_TextAreaAppliancesWarning, lv_pct(100));
lv_obj_set_align( ui_TextAreaAppliancesWarning, LV_ALIGN_BOTTOM_MID );
lv_textarea_set_text(ui_TextAreaAppliancesWarning,"Zde bude seznam toho, co se nemá zapínat.");
lv_obj_add_flag( ui_TextAreaAppliancesWarning, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_set_style_text_font(ui_TextAreaAppliancesWarning, &ui_font_Calibri32, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_TextAreaAppliancesWarning, lv_color_hex(0xA80000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_TextAreaAppliancesWarning, LV_BORDER_SIDE_NONE, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);



ui_LabelRemainingWattsWarning = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelRemainingWattsWarning, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelRemainingWattsWarning, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelRemainingWattsWarning, 0 );
lv_obj_set_y( ui_LabelRemainingWattsWarning, 30 );
lv_obj_set_align( ui_LabelRemainingWattsWarning, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_LabelRemainingWattsWarning,"??? W");
lv_obj_set_style_text_font(ui_LabelRemainingWattsWarning, &ui_font_Calibri48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label1 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label1, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_Label1,"Rezerva");
lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ArcCurrentWattsWarning = lv_arc_create(ui_WarningScreen);
lv_obj_set_width( ui_ArcCurrentWattsWarning, 120);
lv_obj_set_height( ui_ArcCurrentWattsWarning, 120);
lv_obj_set_x( ui_ArcCurrentWattsWarning, 0 );
lv_obj_set_y( ui_ArcCurrentWattsWarning, 14 );
lv_obj_set_align( ui_ArcCurrentWattsWarning, LV_ALIGN_BOTTOM_MID );
lv_obj_add_flag( ui_ArcCurrentWattsWarning, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_ArcCurrentWattsWarning, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE );    /// Flags
lv_arc_set_value(ui_ArcCurrentWattsWarning, 15);
lv_obj_set_style_arc_color(ui_ArcCurrentWattsWarning, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_ArcCurrentWattsWarning, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_arc_color(ui_ArcCurrentWattsWarning, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_ArcCurrentWattsWarning, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_ArcCurrentWattsWarning, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ArcCurrentWattsWarning, 0, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_LabelWattsUsedWarning = lv_label_create(ui_ArcCurrentWattsWarning);
lv_obj_set_width( ui_LabelWattsUsedWarning, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelWattsUsedWarning, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_LabelWattsUsedWarning, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelWattsUsedWarning,"??? W");
lv_obj_set_style_text_font(ui_LabelWattsUsedWarning, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label2 = lv_label_create(ui_WarningScreen);
lv_obj_set_width( ui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label2, 0 );
lv_obj_set_y( ui_Label2, -116 );
lv_obj_set_align( ui_Label2, LV_ALIGN_BOTTOM_MID );
lv_label_set_text(ui_Label2,"Aktuální spotřeba");
lv_obj_add_flag( ui_Label2, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_WarningScreen, ui_event_WarningScreen, LV_EVENT_ALL, NULL);

}