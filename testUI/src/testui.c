#include "testui.h"

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *label;
	Evas_Object *box;
	Evas_Object *list;
	/*Naviframe*/
	Evas_Object *navi;
	Elm_Object_Item *navi_item;

} appdata_s;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
prev_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    Evas_Object *nf = data;
    elm_naviframe_item_pop(nf);
}

static void
list_item_doubleclicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    Evas_Object *navi_button;
    Evas_Object *nf = data;
    Elm_Object_Item *nf_it;

    navi_button = elm_button_add(nf);
    elm_object_text_set(navi_button, "Prev");
    elm_object_style_set(navi_button, "bottom");
    evas_object_smart_callback_add(navi_button, "clicked",
                                   prev_btn_clicked_cb, nf);

    nf_it = elm_naviframe_item_push(nf, "Second view", NULL,
                                    NULL, navi_button, NULL);
}


static void
create_base_gui(appdata_s *ad)
{
		/*
	       	  윈도우 구성요소고, 기기가 닫혀지거나, 요청받았을 때 삭제한다.

	    */
	    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	    elm_win_autodel_set(ad->win, EINA_TRUE);

	    if (elm_win_wm_rotation_supported_get(ad->win)) {
	        int rots[4] = {0, 90, 180, 270};
	        elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	    }
	    evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	    eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	    /*
           Conformant
           Create and initialize elm_conformant, which is mandatory for the base GUI
           to have a proper size when an indicator or virtual keypad is visible
	        */
        ad->conform = elm_conformant_add(ad->win);
        elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
        elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
        evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
        elm_win_resize_object_add(ad->win, ad->conform);
        evas_object_show(ad->conform);

        //        navi 추가

                ad->navi = elm_naviframe_add(ad->conform);
                evas_object_show(ad->navi);
                elm_object_content_set(ad->conform, ad->navi);


        /*
            Box
        */
                ad->box = elm_box_add(ad->navi);
                evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
                evas_object_show(ad->box);
                elm_object_content_set(ad->navi, ad->box);

                ad->navi_item = elm_naviframe_item_push(ad->navi, "First view", NULL, NULL, ad->box, NULL);

        /*
            Label
            Create an actual view of the base GUI
            Modify this part to change the view
        */
        ad->label = elm_label_add(ad->box);
        elm_object_text_set(ad->label, "<align=center>전정환 병신</align>");
        evas_object_size_hint_weight_set(ad->label, 0.0, 0.0);
        //elm_object_content_set(ad->conform, ad->label);
        evas_object_size_hint_align_set(ad->label, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_min_set(ad->label, 50, 50);
        evas_object_show(ad->label);
        elm_box_pack_end(ad->box, ad->label);

        int i;

        /* Create the list */
        ad->list = elm_list_add(ad->box);
        /* Set the list size */
        evas_object_size_hint_weight_set(ad->list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(ad->list, EVAS_HINT_FILL, EVAS_HINT_FILL);

        for (i = 0; i < 4; i++){
        	char tmp[8];
        	snprintf(tmp,sizeof(tmp), "정환 %d", i + 1);
        	elm_list_item_append(ad->list, tmp, NULL, NULL, NULL, NULL);
        }

        evas_object_show(ad->list);
        elm_box_pack_end(ad->box, ad->list);


        Elm_Object_Item *it;
        it = elm_list_first_item_get(ad->list);
        elm_object_item_text_set(it, "Go to Second");
        // add a callback
        evas_object_smart_callback_add(ad->list, "clicked,double", list_item_doubleclicked_cb, ad->navi);

        evas_object_show(ad->win);

}


// 이 create 함수가 true를 반환하면 프로그램을 시작함. false면 종료됨.
static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
