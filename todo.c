#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <leif/leif.h>

#define WIN_MARGIN 20.0f
static int winw = 1280, winh = 720; //keep size of window
static LfFont titlefont, bodyfont, smallfont;
static LfTexture removetexture; 

//enums to be used
typedef enum {
	FILTER_ALL=0,
	FILTER_CURRENTLY, 
	FILTER_COMPLETED, 
	FILTER_LOW, 
	FILTER_MEDIUM, 
	FILTER_HIGH 
} entry_filter;
typedef enum {
	PRIORITY_LOW = 0,
	PRIORITY_MEDIUM,
	PRIORITY_HIGH 
} entry_priority;
typedef struct {
	bool completed;
	char* desc, *date;
	entry_priority priority;
} task_entry;

static task_entry* entries[1024]; //AMOUNT OF TASKS POSSIBLEdwd
static uint32_t numentries = 0; //keeps count of entries
static entry_filter currentFilter;


//method that renders topbar
static void renderTop(){
	lf_push_font(&titlefont);
	lf_text("ToDODO");
        lf_pop_font();
        
        //the lil butn
        {
       const float btnwidth = 150.0f;
       lf_set_ptr_x_absolute(winw - btnwidth - WIN_MARGIN * 2.0f);
       LfUIElementProps props = lf_get_theme().button_props;
       //style it
       props.margin_left = 0.1f;
       props.margin_right = 0.1f;
       props.color = (LfColor){230, 86, 238, 255};
       props.border_width = 0.0f;
       props.corner_radius = 4.0f;

       lf_push_style_props(props);
       lf_set_line_should_overflow(false); //avoids throwing element to the other row
       lf_button_fixed("New Task", btnwidth, -1); //with -1 takes the default height
       lf_set_line_should_overflow(true);
       lf_pop_style_props();
        }

}
//render filter buttons
static void renderFilters(){
	//the filters
	const uint32_t numfilters = 6;
	static const char* filters[] = {
		"ALL",
		"CURRENTLY",
		"COMPLETED",
		"LOW", "MEDIUM", "HIGH"};

	LfUIElementProps fprops = lf_get_theme().button_props;
	fprops.color = LF_NO_COLOR;
	fprops.margin_top = 23.0f;
	fprops.margin_left = 10.0f;
	fprops.margin_right = 35.0f;
	fprops.padding = 15.0f;
	fprops.corner_radius = 8.0f;
	fprops.border_width = 0.0f;
	fprops.text_color = LF_WHITE;
	//remember to move filters to
	//center
	//we want to render but not render the filters to put them on the right
	float width = 0.0f, prtx_before = lf_get_ptr_x();
	float ptrx_before = lf_get_ptr_x();
	float ptry_before = lf_get_ptr_y();

	lf_push_style_props(fprops);
	lf_set_no_render(true);
	for(uint32_t i = 0; i < numfilters; i++) {
		lf_button(filters[i]);
	}

	lf_pop_style_props(fprops);
	lf_set_no_render(false); //method to no render
	width = lf_get_ptr_x() - prtx_before - fprops.margin_right - fprops.padding;
	lf_set_ptr_x_absolute(winw - width - WIN_MARGIN);
	lf_set_line_should_overflow(false);
	//display buttons one by one
	for(uint32_t i = 0; i <numfilters; i++) {
		fprops.color = (currentFilter == i) ? (LfColor){120,120,120,255} : LF_NO_COLOR;
		//it's an if condition
		lf_push_style_props(fprops);
		if(lf_button(filters[i]) == LF_CLICKED) {
			currentFilter = (entry_filter)i;
		}
		//lf_button(filters[i]);
		lf_pop_style_props(fprops);
	}
	lf_set_line_should_overflow(true);
}

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
	removetexture = lf_load_texture("./icons/remove.png", true, LF_TEX_FILTER_LINEAR); //file, flip and filter					
	
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
