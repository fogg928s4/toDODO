#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <leif/leif.h>

static task_entry* entries[1024]; //amount of tasks possibledwd
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

