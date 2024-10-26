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







