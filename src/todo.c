#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <leif/leif.h>

#include "enums.c"
#include "elements.c"



int main() {
        printf("queso cottage");
        glfwInit();
        GLFWwindow* window = glfwCreateWindow(1280, 720, "toDODO", NULL, NULL);
        glfwMakeContextCurrent(window);
        lf_init_glfw(winw, winh, window);
	//initialize font
	titlefont = lf_load_font("./fonts/Poppins-Bold.ttf", 40);
	smallfont = lf_load_font("./fonts/Poppins-Regular.ttf", 28);
	//icons
	removetexture = lf_load_texture("../icons/remove.png", true, LF_TEX_FILTER_LINEAR); //file, flip and filter
	
	task_entry* entry = (task_entry*)malloc(sizeof(*entry)); //a new entry
	entry->priority = PRIORITY_LOW; //C uses -> to designate atributes of a pointer, instead of a .
	entry->completed = false;
	entry->date = "nothing";
	entry->desc = "Eat 7 soup";
	entries[numentries++] = entry;
	

        while(!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

                //setting the theme
		LfTheme theme = lf_get_theme();
		theme.div_props.color = LF_NO_COLOR;
		lf_set_theme(theme);

		lf_begin();
		lf_div_begin(((vec2s){WIN_MARGIN,WIN_MARGIN})
				,((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}), true);
		//windows margin minux scale
		//making it scrollable
		//topbar
		renderTop();
		lf_next_line(); //new line after top
		renderFilters();
		lf_next_line();
		{
			//create a new div for the entries
			lf_div_begin(((vec2s){lf_get_ptr_x(), lf_get_ptr_y()}),
				((vec2s){winw - lf_get_ptr_x() - WIN_MARGIN,
				 (winh - lf_get_ptr_y()- WIN_MARGIN)}), true); 
			//scrollable is true
			//where the fun begins
			for (uint32_t i = 0; i< numentries; i++) {
				task_entry* entry = entries[i];
				float prity_size = 15.0f; //size of icon
				float ptry_before = lf_get_ptr_y(), ptrx_before = lf_get_ptr_x();
				//switch to colour the priorit
				lf_set_ptr_y_absolute(lf_get_ptr_y() + 5.0f);
				lf_set_ptr_x_absolute(lf_get_ptr_x() + 5.0f);
				switch(entry->priority) {
					case PRIORITY_LOW:
						lf_rect(prity_size, prity_size, (LfColor){45,202,229,255}, 4.0f);
						break;
					case PRIORITY_MEDIUM:
						lf_rect(prity_size, prity_size, (LfColor){253,164,2,255}, 4.0f);
						break;
					case PRIORITY_HIGH:
						lf_rect(prity_size, prity_size, (LfColor){229,72,45,255}, 4.0f);
						break;
			
				}
				lf_set_ptr_y_absolute(ptry_before);
				//buttons
				{
					LfUIElementProps btprops = lf_get_theme().button_props;
					btprops.color = LF_NO_COLOR;
					btprops.border_width = 0.0f; btprops.padding = 0.0f; 
					btprops.margin_top = 0.0f; btprops.margin_left = 20.0f;
					lf_push_style_props(btprops);
					if(lf_image_button(((LfTexture){.id = removetexture.id, .width = 25, .height= 25})) == LF_CLICKED){

						//placeholder
					}
					lf_pop_style_props(btprops);

				}
				//checkbox
				{
					LfUIElementProps chprops = lf_get_theme().checkbox_props;
					chprops.border_width = 1.0f;
					chprops.corner_radius = 0.01f;
					chprops.margin_top = 0.0f; chprops.margin_left = 10.0f; chprops.padding = 5.0f;
					chprops.color = lf_color_from_zto((vec4s){0.05f, 0.05f, 0.05f, 1.0f});
					lf_push_style_props(chprops);
					if(lf_checkbox("", &entry->completed, LF_NO_COLOR, ((LfColor){65,167,204,255}))== LF_CLICKED) {
						
					}
					lf_pop_style_props(chprops);
				}
				//desciption
				//properly style and align text
				LfUIElementProps eprops = lf_get_theme().text_props;
				eprops.margin_left = 10.0f; eprops.margin_right = 10.0f;
				eprops.margin_top = 1.0f;
				lf_push_font(&smallfont);
				lf_push_style_props(eprops);
				lf_text(entry->desc);
				lf_pop_font();
				lf_pop_style_props(eprops);
				lf_next_line();
			}


		}
		lf_div_end();		
                lf_end();

                glfwPollEvents();
                glfwSwapBuffers(window);
       }
        lf_free_font(&titlefont); //terminate font pointer
	glfwDestroyWindow(window);
        glfwTerminate();
        return 0;

}
