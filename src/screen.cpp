#include "api.h"

namespace screen {

	using namespace pros;
	using namespace std;

	//Create a button descriptor string array
	static const char * red_btnm_map[] = {
		"Alpha", "Bravo", "Charlie", "Delta", "Echo", "\n",
		"Foxtrot", "Golf", "Hotel", "India", "Juliett", ""
	};

	static const char * blue_btnm_map[] = {
		"Kilo", "Lima", "Mike", "November", "Oscar", "\n",
		"Papa", "Quebec", "Romeo", "Sierra", "Tango", ""
	};

	static const char * skill_btnm_map[] = {
		"Uniform", "Victor", "Whiskey", "\n",
		"Xray", "Yankee", "Zulu", ""
	};

	static const char * action_btnm_map[] = {
		"Record", "Clear", ""
	};

	lv_obj_t * btn_matrix_group[3];
	const char * selected_program;

	static lv_res_t action_selector_callback(lv_obj_t *btnm, const char *txt) {
		if (selected_program == NULL) {
			cout << "nothing to do!" << endl;
		} else {
			cout << txt << " on " << selected_program << endl;
		}
		return LV_RES_OK;
	}

	static lv_res_t alphabet_selector_callback(lv_obj_t *btnm, const char *txt) {
		auto toggled_id = lv_btnm_get_toggled(btnm);
		auto pressed_id = lv_btnm_get_pressed(btnm);
		if (toggled_id == pressed_id) {
			return LV_RES_OK;
		} else {
			lv_btnm_set_toggle(btnm, true, pressed_id);
		}

		for (int i=0; i<3; ++i) {
			if (btn_matrix_group[i] != btnm) {
				uint16_t toggled_button_id = lv_btnm_get_toggled(btn_matrix_group[i]);
				if (toggled_button_id != LV_BTNM_PR_NONE) {
					lv_btnm_set_toggle(btn_matrix_group[i], false, toggled_button_id);
				}
			}
		}
		selected_program = txt;
		cout << "Program " << txt << " selected!" << endl;
		return LV_RES_OK;
	}

	lv_obj_t * add_button_matrix_to_tab(lv_obj_t * tab, const char ** btnm_map) {
		lv_obj_t * btn_matrix = lv_btnm_create(tab, NULL);
		lv_btnm_set_map(btn_matrix, btnm_map);
		// lv_btnm_set_toggle(btn_matrix, true, LV_BTNM_PR_NONE);
		lv_btnm_set_action(btn_matrix, alphabet_selector_callback);
		// lv_obj_set_size(btn_matrix, LV_HOR_RES, LV_VER_RES / 2);
		lv_obj_set_size(btn_matrix, 460, 100);
		lv_obj_set_pos(btn_matrix, 0, 0);
		lv_obj_align(btn_matrix, NULL, LV_ALIGN_CENTER, 0, 0);

		lv_obj_t *actBtnm = lv_btnm_create(tab, NULL);
		lv_btnm_set_map(actBtnm, action_btnm_map);
		lv_btnm_set_action(actBtnm, action_selector_callback);
		lv_obj_set_size(actBtnm, 300, 40);
		lv_obj_set_pos(actBtnm, 0, 230);
		lv_obj_align(actBtnm, NULL, LV_ALIGN_CENTER, 0, 0);	

		return btn_matrix;
	}

	void setup_screen()
	{
		pros::delay(50); // sometimes LVGL won't draw the screen if there is no delay or it is not inverted on the brain

		// lvgl theme
		lv_theme_t *th = lv_theme_alien_init(280, NULL); //Set a HUE value and keep font default
		lv_theme_set_current(th);

		// create a tab view object
		lv_obj_t *tabview;
		tabview = lv_tabview_create(lv_scr_act(), NULL);

		// add 3 tabs (the tabs are page (lv_page) and can be scrolled
		const char * tab_names[3] = { "Red", "Blue", "Skills"};
		const char ** tab_button_matrix_names[3] = { red_btnm_map, blue_btnm_map, skill_btnm_map };
		for (int i=0; i<3; ++i) {
			lv_obj_t * tab = lv_tabview_add_tab(tabview, tab_names[i]);
			lv_style_t * style = lv_tabview_get_style(tab, LV_TABVIEW_STYLE_BG);
			style->body.padding.ver = 5;
			style->body.padding.hor = 10;
			btn_matrix_group[i] = add_button_matrix_to_tab(tab, tab_button_matrix_names[i]);
		}
	}
}
