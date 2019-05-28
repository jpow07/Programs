
#define COLORTEXT_FG(r,g,b) \
       	printf("\033[48;2;%d;%d;%dm", r, g, b) \

#define COLORTEXT_BG(r,g,b) \
       	printf("\033[38;2;%d;%d;%dm", r, g, b) \

#define DEFAULTCOLOR_BG printf("\033[39m")
#define DEFAULTCOLOR_FG printf("\033[49m")
