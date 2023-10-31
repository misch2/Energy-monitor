// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: Energy_monitor

#include "../ui.h"

void ui_ErrorScreen_screen_init(void)
{
ui_ErrorScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ErrorScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Panel1 = lv_obj_create(ui_ErrorScreen);
lv_obj_set_width( ui_Panel1, 480);
lv_obj_set_height( ui_Panel1, 320);
lv_obj_set_align( ui_Panel1, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Panel1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Panel1, lv_color_hex(0x7F3F00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Panel1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0xFF7F3F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_Panel1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Panel1, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Panel1, &ui_font_Calibri16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label1 = lv_label_create(ui_Panel1);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label1, 0 );
lv_obj_set_y( ui_Label1, -132 );
lv_obj_set_align( ui_Label1, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label1,"Rezerva");
lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xC0C03F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelRezervaError = lv_label_create(ui_Panel1);
lv_obj_set_width( ui_LabelRezervaError, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelRezervaError, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelRezervaError, 0 );
lv_obj_set_y( ui_LabelRezervaError, -96 );
lv_obj_set_align( ui_LabelRezervaError, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelRezervaError,"??? W");
lv_obj_set_style_text_color(ui_LabelRezervaError, lv_color_hex(0xC0C03F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelRezervaError, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelRezervaError, &ui_font_Calibri48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label2 = lv_label_create(ui_Panel1);
lv_obj_set_width( ui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label2, 0 );
lv_obj_set_y( ui_Label2, -49 );
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,"Nezapínat:");
lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xC0C03F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PanelAppliance = lv_obj_create(ui_Panel1);
lv_obj_set_width( ui_PanelAppliance, 200);
lv_obj_set_height( ui_PanelAppliance, 50);
lv_obj_set_align( ui_PanelAppliance, LV_ALIGN_BOTTOM_LEFT );
lv_obj_clear_flag( ui_PanelAppliance, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_PanelAppliance, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PanelAppliance, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_PanelAppliance, lv_color_hex(0x7F0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_PanelAppliance, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label4 = lv_label_create(ui_PanelAppliance);
lv_obj_set_width( ui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label4, LV_ALIGN_CENTER );
lv_obj_set_style_text_color(ui_Label4, lv_color_hex(0x3F3F3F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

}
