// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: Energy_monitor

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_LoadingScreen
void ui_LoadingScreen_screen_init(void);
lv_obj_t *ui_LoadingScreen;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_LoadingLabel;


// SCREEN: ui_OKScreen
void ui_OKScreen_screen_init(void);
void ui_event_OKScreen( lv_event_t * e);
lv_obj_t *ui_OKScreen;
lv_obj_t *ui_PanelTopOK;
lv_obj_t *ui_LabelRemainingWattsOK;
lv_obj_t *ui_Label3;
lv_obj_t *ui_ArcCurrentWattsOK;
lv_obj_t *ui_LabelWattsUsedOK;
lv_obj_t *ui_Label5;


// SCREEN: ui_WarningScreen
void ui_WarningScreen_screen_init(void);
void ui_event_WarningScreen( lv_event_t * e);
lv_obj_t *ui_WarningScreen;
lv_obj_t *ui_PanelTopWarning;
lv_obj_t *ui_TextAreaAppliancesWarning;
lv_obj_t *ui_LabelRemainingWattsWarning;
lv_obj_t *ui_Label1;
lv_obj_t *ui_ArcCurrentWattsWarning;
lv_obj_t *ui_LabelWattsUsedWarning;
lv_obj_t *ui_Label2;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_OKScreen( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      ui_toggleBacklight( e );
}
}
void ui_event_WarningScreen( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      ui_toggleBacklight( e );
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_LoadingScreen_screen_init();
ui_OKScreen_screen_init();
ui_WarningScreen_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_LoadingScreen);
}