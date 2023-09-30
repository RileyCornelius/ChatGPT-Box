// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.4
// Project name: chat_gpt

#include "../ui.h"

void ui_ScreenWifiReset_screen_init(void)
{
ui_ScreenWifiReset = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenWifiReset, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_img_src( ui_ScreenWifiReset, &ui_img_setup_bg_png, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_LabelWifiResetTitle = lv_label_create(ui_ScreenWifiReset);
lv_obj_set_width( ui_LabelWifiResetTitle, 230);
lv_obj_set_height( ui_LabelWifiResetTitle, LV_SIZE_CONTENT);   /// 140
lv_obj_set_x( ui_LabelWifiResetTitle, 45 );
lv_obj_set_y( ui_LabelWifiResetTitle, 15 );
lv_label_set_text(ui_LabelWifiResetTitle,"Info Provisioning");
lv_obj_set_style_text_color(ui_LabelWifiResetTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelWifiResetTitle, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_LabelWifiResetTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelWifiResetTitle, &ui_font_PingFangEN20, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ButtonWifiResetConfirm = lv_btn_create(ui_ScreenWifiReset);
lv_obj_set_width( ui_ButtonWifiResetConfirm, 72);
lv_obj_set_height( ui_ButtonWifiResetConfirm, 24);
lv_obj_set_x( ui_ButtonWifiResetConfirm, 0 );
lv_obj_set_y( ui_ButtonWifiResetConfirm, -20 );
lv_obj_set_align( ui_ButtonWifiResetConfirm, LV_ALIGN_BOTTOM_MID );
lv_obj_add_flag( ui_ButtonWifiResetConfirm, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonWifiResetConfirm, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_ButtonWifiResetConfirm, 17, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_ButtonWifiResetConfirm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonWifiResetConfirm, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelSetupBtn1 = lv_label_create(ui_ButtonWifiResetConfirm);
lv_obj_set_width( ui_LabelSetupBtn1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelSetupBtn1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_LabelSetupBtn1, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelSetupBtn1,"Restart");
lv_obj_set_style_text_color(ui_LabelSetupBtn1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelSetupBtn1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_LabelSetupBtn1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelSetupBtn1, &ui_font_PingFangEN16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ImageWifiResetBack = lv_img_create(ui_ScreenWifiReset);
lv_img_set_src(ui_ImageWifiResetBack, &ui_img_settings_back_png);
lv_obj_set_width( ui_ImageWifiResetBack, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ImageWifiResetBack, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_ImageWifiResetBack, 10 );
lv_obj_set_y( ui_ImageWifiResetBack, 10 );
lv_obj_add_flag( ui_ImageWifiResetBack, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_ImageWifiResetBack, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_LabelWifiResetContent = lv_label_create(ui_ScreenWifiReset);
lv_obj_set_width( ui_LabelWifiResetContent, lv_pct(88));
lv_obj_set_height( ui_LabelWifiResetContent, lv_pct(61));
lv_obj_set_x( ui_LabelWifiResetContent, lv_pct(0) );
lv_obj_set_y( ui_LabelWifiResetContent, lv_pct(20) );
lv_obj_set_align( ui_LabelWifiResetContent, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_LabelWifiResetContent,"1. Press the \"Start\" button to enable USB flash drive.\n2. Connect the box to your PC to add the OpenAI key and Wi-Fi credentials in the configuration.ini file.\n3. Follow the on-screen prompts to complete the setup process.");
lv_obj_set_style_text_color(ui_LabelWifiResetContent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelWifiResetContent, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_letter_space(ui_LabelWifiResetContent, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_LabelWifiResetContent, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelWifiResetContent, &ui_font_PingFangEN14, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_ButtonWifiResetConfirm, ui_event_ButtonWifiResetConfirm, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_ImageWifiResetBack, ui_event_ImageWifiResetBack, LV_EVENT_ALL, NULL);

}
