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
lv_obj_set_x( ui_PanelTopWarning, -1 );
lv_obj_set_y( ui_PanelTopWarning, 0 );
lv_obj_set_align( ui_PanelTopWarning, LV_ALIGN_TOP_MID );
lv_obj_add_flag( ui_PanelTopWarning, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_PanelTopWarning, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_PanelTopWarning, lv_color_hex(0xA80000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PanelTopWarning, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_PanelTopWarning, lv_color_hex(0xFF8080), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_PanelTopWarning, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_PanelTopWarning, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_PanelTopWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_PanelTopWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_PanelTopWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_PanelTopWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelRemainingWattsWarning = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelRemainingWattsWarning, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelRemainingWattsWarning, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelRemainingWattsWarning, 30 );
lv_obj_set_y( ui_LabelRemainingWattsWarning, 10 );
lv_obj_set_align( ui_LabelRemainingWattsWarning, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_LabelRemainingWattsWarning,"??? W");
lv_obj_set_style_text_font(ui_LabelRemainingWattsWarning, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label1 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label1, -50 );
lv_obj_set_y( ui_Label1, 10 );
lv_obj_set_align( ui_Label1, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_Label1,"Rezerva:");
lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance1 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance1, 150);
lv_obj_set_height( ui_LabelAppliance1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance1, 6 );
lv_obj_set_y( ui_LabelAppliance1, 100 );
lv_obj_set_style_text_color(ui_LabelAppliance1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance1, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance1, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance1, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance1, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance1, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance1, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance1, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance2 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance2, 150);
lv_obj_set_height( ui_LabelAppliance2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance2, 6 );
lv_obj_set_y( ui_LabelAppliance2, 135 );
lv_obj_set_style_text_color(ui_LabelAppliance2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance2, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance2, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance2, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance2, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance2, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance2, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance2, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance3 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance3, 150);
lv_obj_set_height( ui_LabelAppliance3, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance3, 6 );
lv_obj_set_y( ui_LabelAppliance3, 170 );
lv_obj_set_style_text_color(ui_LabelAppliance3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance3, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance3, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance3, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance3, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance3, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance3, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance3, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance8 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance8, 150);
lv_obj_set_height( ui_LabelAppliance8, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance8, 160 );
lv_obj_set_y( ui_LabelAppliance8, 135 );
lv_obj_set_style_text_color(ui_LabelAppliance8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance8, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance8, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance8, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance8, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance8, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance8, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance8, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance8, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance8, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance8, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance4 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance4, 150);
lv_obj_set_height( ui_LabelAppliance4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance4, 6 );
lv_obj_set_y( ui_LabelAppliance4, 205 );
lv_obj_set_style_text_color(ui_LabelAppliance4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance4, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance4, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance4, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance4, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance4, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance4, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance4, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance5 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance5, 150);
lv_obj_set_height( ui_LabelAppliance5, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance5, 6 );
lv_obj_set_y( ui_LabelAppliance5, 240 );
lv_obj_set_style_text_color(ui_LabelAppliance5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance5, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance5, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance5, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance5, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance5, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance5, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance5, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance5, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance5, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance5, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance6 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance6, 150);
lv_obj_set_height( ui_LabelAppliance6, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance6, 6 );
lv_obj_set_y( ui_LabelAppliance6, 275 );
lv_obj_set_style_text_color(ui_LabelAppliance6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance6, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance6, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance6, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance6, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance6, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance6, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance6, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance6, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance6, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance6, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance7 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance7, 150);
lv_obj_set_height( ui_LabelAppliance7, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance7, 160 );
lv_obj_set_y( ui_LabelAppliance7, 100 );
lv_obj_set_style_text_color(ui_LabelAppliance7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance7, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance7, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance7, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance7, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance7, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance7, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance7, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance7, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance7, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance7, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance9 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance9, 150);
lv_obj_set_height( ui_LabelAppliance9, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance9, 160 );
lv_obj_set_y( ui_LabelAppliance9, 170 );
lv_obj_set_style_text_color(ui_LabelAppliance9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance9, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance9, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance9, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance9, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance9, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance9, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance9, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance9, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance9, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance9, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance10 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance10, 150);
lv_obj_set_height( ui_LabelAppliance10, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance10, 160 );
lv_obj_set_y( ui_LabelAppliance10, 205 );
lv_obj_set_style_text_color(ui_LabelAppliance10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance10, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance10, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance10, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance10, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance10, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance10, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance10, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance10, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance10, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance10, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance10, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance10, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance11 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance11, 150);
lv_obj_set_height( ui_LabelAppliance11, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance11, 160 );
lv_obj_set_y( ui_LabelAppliance11, 240 );
lv_obj_set_style_text_color(ui_LabelAppliance11, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance11, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance11, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance11, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance11, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance11, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance11, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance11, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance11, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance11, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance11, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance11, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance11, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelAppliance12 = lv_label_create(ui_PanelTopWarning);
lv_obj_set_width( ui_LabelAppliance12, 150);
lv_obj_set_height( ui_LabelAppliance12, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelAppliance12, 160 );
lv_obj_set_y( ui_LabelAppliance12, 275 );
lv_obj_set_style_text_color(ui_LabelAppliance12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelAppliance12, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelAppliance12, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_LabelAppliance12, 20, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_LabelAppliance12, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_LabelAppliance12, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_LabelAppliance12, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_LabelAppliance12, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_LabelAppliance12, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_LabelAppliance12, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_LabelAppliance12, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_LabelAppliance12, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_LabelAppliance12, 2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ArcCurrentWattsWarning = lv_arc_create(ui_WarningScreen);
lv_obj_set_width( ui_ArcCurrentWattsWarning, 120);
lv_obj_set_height( ui_ArcCurrentWattsWarning, 120);
lv_obj_set_x( ui_ArcCurrentWattsWarning, 0 );
lv_obj_set_y( ui_ArcCurrentWattsWarning, 6 );
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
lv_obj_set_y( ui_Label2, -124 );
lv_obj_set_align( ui_Label2, LV_ALIGN_BOTTOM_MID );
lv_label_set_text(ui_Label2,"Aktuální spotřeba");
lv_obj_add_flag( ui_Label2, LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TextAreaAppliancesWarning = lv_textarea_create(ui_WarningScreen);
lv_obj_set_height( ui_TextAreaAppliancesWarning, 202);
lv_obj_set_width( ui_TextAreaAppliancesWarning, lv_pct(100));
lv_obj_set_align( ui_TextAreaAppliancesWarning, LV_ALIGN_BOTTOM_MID );
lv_textarea_set_text(ui_TextAreaAppliancesWarning,"Nezapínej:\n- troubu\n- myčku\n- mikrovlnku\n- pračku\n- konvici\n- toustovač\n- kávovar");
lv_obj_add_flag( ui_TextAreaAppliancesWarning, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_set_style_text_font(ui_TextAreaAppliancesWarning, &ui_font_Calibri32, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_TextAreaAppliancesWarning, lv_color_hex(0xA80000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_TextAreaAppliancesWarning, LV_BORDER_SIDE_NONE, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_TextAreaAppliancesWarning, 0, LV_PART_MAIN| LV_STATE_DEFAULT);



ui_Label4 = lv_label_create(ui_WarningScreen);
lv_obj_set_width( ui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label4, 0 );
lv_obj_set_y( ui_Label4, 46 );
lv_obj_set_align( ui_Label4, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_Label4,"Nezapínej:");
lv_obj_set_style_text_font(ui_Label4, &ui_font_Calibri32, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_WarningScreen, ui_event_WarningScreen, LV_EVENT_ALL, NULL);

}
