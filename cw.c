#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <png.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#define ARGS "htb:me:p:l:r:RPQCp:d:Dz:u:T:c:f::w:IO:o:i:n:y:x:qA:B:"
#define DOT "."



typedef struct {
    unsigned int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_byte channels;

    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
} Png;
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
    int thickness;
    int line_color[3];
    int is_fill;
    int fill_color[3];
} triangle;

typedef struct{
    int old_color[3];
    int new_color[3];
}rect;


typedef struct{
    int x1;
    int y1;
    int x2;
    int y2;
    char mode;
    int left;
    int right;
    int below;
    int above;    
}Mirror;
typedef struct{
    float alpha;
    int beta;
}Cont;


void read_png_file(char *file_name, Png *image) {
    unsigned char header[8];
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        printf("Error in read_png_file function: file could not be opened.\n");
        exit(40);
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        printf("Error in read_png_file function: file is not recognized as a PNG.\n");
        exit(40);
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        printf("Error in read_png_file function: png_create_read_struct failed.\n");
        exit(40);
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        printf("Error in read_png_file function: png_create_info_struct failed.\n");
        exit(40);
    }
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fclose(fp);
        printf("Error in read_png_file function: error during init_io.\n");
        exit(40);
    }
    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);
    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->channels = png_get_channels(image->png_ptr, image->info_ptr);
    if (image->color_type == PNG_COLOR_TYPE_GRAY) {
        printf("The program does not support working with the PNG_COLOR_TYPE_GRAY color type.\n");
        exit(40);
    } else if (image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        printf("The program does not support working with the PNG_COLOR_TYPE_GRAY_ALPHA color type.\n");
        exit(40);
    } else if (image->color_type == PNG_COLOR_TYPE_PALETTE) {
        printf("The program does not support working with the PNG_COLOR_TYPE_PALETTE color type.\n");
        exit(40);
    }
    png_read_update_info(image->png_ptr, image->info_ptr);

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        printf("Error in read_png_file function: error during read_image.\n");
        exit(40);
    }
    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (int y = 0; y < image->height; y++) {
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    }
    png_read_image(image->png_ptr, image->row_pointers);
    fclose(fp);
}
void free_image_data(Png *img) {
    for (int y = 0; y < img->height; y++) {
        free(img->row_pointers[y]);
    }
    free(img->row_pointers);
}

void write_png_file(char *file_name, Png *image) {
    FILE *fp = fopen(file_name, "wb");
    if (!fp) {
        printf("Error in write_png_file function: file could not be opened.\n");
        exit(40);
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        printf("Error in write_png_file function: png_create_write_struct failed.\n");
        exit(41);
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        printf("Error in write_png_file function: png_create_info_struct failed.\n");
        exit(42);
    }
    if (setjmp(png_jmpbuf(image->png_ptr))){
        printf("Error in write_png_file function: error during init_io.\n");
        exit(43);
    }
    png_init_io(image->png_ptr, fp);

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fclose(fp);
        printf("Error in write_png_file function: error during writing header.\n");
        exit(44);
    }
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height, image->bit_depth, image->color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fclose(fp);
        printf("Error in write_png_file function: error during writing bytes.\n");
        exit(45);
    }
    png_write_image(image->png_ptr, image->row_pointers);

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fclose(fp);
        printf("Error in write_png_file function: error during writing end of file.\n");
        exit(40);
    }
    png_write_end(image->png_ptr, NULL);
    fclose(fp);
}

int check_color(Png* img, const int x, const int y,const int* palit)
{
    if(img->color_type==PNG_COLOR_TYPE_RGB)
    {
        if(img->row_pointers[y][x*img->channels+0]==palit[0] &&
           img->row_pointers[y][x*img->channels+1]==palit[1] &&
           img->row_pointers[y][x*img->channels+2]==palit[2])
            return 1;
    }
    else if(img->color_type==PNG_COLOR_TYPE_RGBA)
    {
        if(img->row_pointers[y][x*img->channels+0]==palit[0] &&
           img->row_pointers[y][x*img->channels+1]==palit[1] &&
           img->row_pointers[y][x*img->channels+2]==palit[2] &&
           img->row_pointers[y][x*img->channels+3]==palit[3])
            return 1;
    }
    return 0;
}
int check_position(Png* img, const long long int x, const long long int y)
{
    if (x >= 0 && x < img->width && y >= 0 && y < img->height) {
        return 1;
    }
    return 0;
}
void set_pixel(Png* img,const int x, const int y,const int* palit)
{

    if(palit[0]>255 || palit[1]>255 || palit[2]>255 || palit[0]<0 || palit[1]<0 || palit[2]<0){
        //printf("Неправильное значение цвета\n");
        exit(46);
    }

    if(check_position(img, x,y)){
        if(check_color(img, x, y, palit))
        return;
        img->row_pointers[y][x * img->channels+0]=palit[0];
        img->row_pointers[y][x * img->channels+1]=palit[1];
        img->row_pointers[y][x * img->channels+2]=palit[2];
    }
}



void print_png_info(Png *image){
    printf("Высота изображения: %d\n", image->height);
    printf("Ширина изображения: %d\n", image->width);
}
void drawCircle(Png* img,const int x0, const int y0, const int radius, const int* color) {

    if(radius==1 || radius==2){
        set_pixel(img,x0,y0, color);
        set_pixel(img,x0+1,y0, color);
        set_pixel(img,x0,y0+1, color);
        set_pixel(img,x0-1,y0, color);
        set_pixel(img,x0,y0-1, color);
        if(radius==2){
            set_pixel(img,x0+1,y0+1, color);
            set_pixel(img,x0+1,y0-1, color);
            set_pixel(img,x0-1,y0+1, color);
            set_pixel(img,x0-1,y0-1, color);
            set_pixel(img,x0+2,y0, color);
            set_pixel(img,x0,y0+2, color);
            set_pixel(img,x0-2,y0, color);
            set_pixel(img,x0,y0-2, color);
        }
        return;
    }

	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while(y >= 0) {
		set_pixel(img,x0 + x, y0 + y, color);
		set_pixel(img,x0 + x, y0 - y, color);
		set_pixel(img,x0 - x, y0 + y, color);
		set_pixel(img, x0 - x, y0 - y, color);
		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		if(delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}

}


void draw_line(Png *img, int x1, int y1, int x2, int y2, int thick, const int *line_color) {
    const long long int deltaX = abs(x2 - x1);
    const long long int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    long long int error = deltaX - deltaY;
	drawCircle(img, x2, y2, thick, line_color);
    while (x1 != x2 || y1 != y2) {
            drawCircle(img, x1, y1,thick, line_color);
            if(thick!=0){
                drawCircle(img, x1, y1,thick-1, line_color);
            }
        int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

int isInside1(int x1, int y1, int x2, int y2, int x3, int y3, int x0, int y0) {
    int prod_1 = (x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0);
    int prod_2 = (x2 - x0) * (y3 - y2) - (x3 - x2) * (y2 - y0);
    int prod_3 = (x3 - x0) * (y1 - y3) - (x1 - x3) * (y3 - y0);
    if ((prod_1 > 0 && prod_2 > 0 && prod_3 > 0) || (prod_1 < 0 && prod_2 < 0 && prod_3 < 0)) {
        return 1;
    }
    return 0;
}

void fill_triangle(Png* img, int X1, int Y1, int X2, int Y2, int X3, int Y3, int* palit)
{
    for(int y=0;y<img->height;y++)
    {
        for(int x=0;x<img->width;x++)
        {
            if(isInside1(X1,Y1,X2,Y2,X3,Y3, x, y) && check_position(img, x,y))
            {
                set_pixel(img, x, y, palit);
            }
        }
    }
}


void draw_triangle(Png* img, int X1, int Y1, int X2, int Y2, int X3, int Y3, int thickness, int* palit, int is_fill, int* palit_fill){
    if((Y1==Y2 && Y2==Y3)||(X1==X2 && X2==X3)){
        printf("Error: ircorrect values of triangle\n");
        exit(18);
    }
    if(is_fill==1){
        fill_triangle(img, X1, Y1, X2, Y2, X3, Y3, palit_fill);
    }

    int a=0;
    if(thickness==1){
        draw_line(img,X1, Y1, X2, Y2,thickness-1, palit);
        draw_line(img,X2, Y2, X3, Y3,thickness-1, palit);
        draw_line(img,X3, Y3, X1, Y1,thickness-1, palit);
        return;
    }
    if(thickness%2)
    a=1;
    draw_line(img,X1, Y1, X2, Y2,thickness/2+a, palit);
    draw_line(img,X2, Y2, X3, Y3,thickness/2+a, palit);
    draw_line(img,X1, Y1, X3, Y3,thickness/2+a, palit);

}

int check_color_on_area(Png* img, int x1, int y1, int x2, int y2, int* palit)
{
    for(int y=y1;y<y2;y++)
    {
        for(int x=x1;x<x2;x++)
        {
            if(check_color(img, x, y, palit)==0)
            {
                return 0;
            }
        }
    }
    return 1;
}
void recolor_biggest_rectangle(Png* img, int* old_color, int* new_color){
    unsigned long long int max_area=0;
    int x_rh, y_rh, x_lw, y_lw;
    for(int y=0;y<img->height;y++)
    {
        for(int x=0;x<img->width;x++)
        {
            if(check_color(img, x,y, old_color))
            {
                int x_max=x;
                int y_max=y;
                while(check_color(img, x_max, y, old_color) && x_max<img->width){
                    x_max++;
                }
                while(check_color(img, x, y_max, old_color) && y_max<img->height){
                    y_max++;
                }


                if((y_max-y)*(x_max-x)>max_area)
                {
                    if(check_color_on_area(img,x,y,x_max,y_max,old_color))
                    {
                            unsigned int area = (y_max-y)*(x_max-x);
                            max_area=area;
                            x_rh=x;
                            x_lw=x_max;
                            y_rh=y;
                            y_lw=y_max;
                    }
                }
                //x=x_max;
            }   
        }
    }

    if(max_area!=0)
    {
        for(int y=y_rh;y<y_lw;y++)
        {
            for(int x=x_rh;x<x_lw;x++)
            {
                set_pixel(img, x, y, new_color);
            }
        }
    }
    else{
        printf("We are not found rectangle with this color\n");
    }

}
void change_color(Png * image, int * new_color) {

    int number_of_channels = 3;
    int bit_depth = image->bit_depth;
    int stride = number_of_channels * bit_depth / 8;

    int *** colors = calloc(256, sizeof(int**));
    for (int i = 0; i < 256; i++) {
        colors[i] = calloc(256, sizeof(int*));
        for (int j = 0; j < 256; j++) {
            colors[i][j] = calloc(256, sizeof(int));
        }
    }

    for (int y = 0; y < image->height; y++) {
        png_bytep row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++) {
            png_bytep ptr = &(row[x * stride]);
            colors[ptr[0]][ptr[1]][ptr[2]]++;
        }
    }

    int freq = colors[0][0][0];
    int max_colors[] = {0,0,0};
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            for (int k = 0; k < 256; k++) {
                if (colors[i][j][k] > freq) {
                    max_colors[0] = i;
                    max_colors[1] = j;
                    max_colors[2] = k;
                    freq = colors[i][j][k];
                }
            }
        }
    }

    for (int j = 0; j < image->height; j++) {
        png_bytep row = image->row_pointers[j];
        for (int i = 0; i < image->width; i++) {
            png_bytep ptr = &(row[i * stride]);
            if (ptr[0] == max_colors[0] && ptr[1] == max_colors[1] && ptr[2] == max_colors[2]) {
                ptr[0] = new_color[0];
                ptr[1] = new_color[1];
                ptr[2] = new_color[2];
               //ptr[3] = new_color[3];
            }
        }
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            free(colors[i][j]);
        }
        free(colors[i]);
    }
    free(colors);
}
void swap_pixels(Png* img, int x1, int y1, int x2, int y2){
    int tmp1 = img->row_pointers[y1][x1 * img->channels+0];
    int tmp2 = img->row_pointers[y1][x1 * img->channels+1];
    int tmp3 = img->row_pointers[y1][x1 * img->channels+2];
    img->row_pointers[y1][x1 * img->channels+0]=img->row_pointers[y2][x2 * img->channels+0];
    img->row_pointers[y1][x1 * img->channels+1]=img->row_pointers[y2][x2 * img->channels+1];
    img->row_pointers[y1][x1 * img->channels+2]=img->row_pointers[y2][x2 * img->channels+2];
    img->row_pointers[y2][x2 * img->channels+0]=tmp1;
    img->row_pointers[y2][x2 * img->channels+1]=tmp2;
    img->row_pointers[y2][x2 * img->channels+2]=tmp3;
}
void mirror_function(Png *img, int x1, int y1, int x2, int y2, char mode){
    if(x1<0 || y1<0 || x2<0 || y2<0 || x1>img->width || x2>img->width || y1>img->height || y2>img->height){
        printf("Неверные значения координат\n");
        exit(40);
    }
    if(x2 < x1){
        int tmp = x2;
        x2=x1;
        x1-=x1-tmp;
    }
    if(y2 < y1){
        int tmp = y2;
        y2=y1;
        y1-=y1-tmp;
    }
    if(mode=='y'){
        for(int x=x1;x<(x2+x1)/2;x++){
            for(int y=y1; y<y2;y++){
                swap_pixels(img, x, y, x2-x+x1, y);
            }
        }
    }
    else if(mode=='x'){
        for(int y=y1;y<(y2+y1)/2;y++){
            for(int x=x1; x<x2;x++){
                swap_pixels(img, x, y, x, y2-y+y1);
            }
        }
    }

}

void make_empty_image(Png *img, Png *new_img, int height, int width, int is_have_background) {
    new_img->width = width;
    new_img->height = height;
    new_img->color_type = img->color_type;
    new_img->bit_depth = img->bit_depth;
    new_img->channels = img->channels;
    new_img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!new_img->png_ptr) {
        printf("Error in make_empty_image function: png_create_read_struct failed.\n");
        exit(40);
    }
    new_img->info_ptr = png_create_info_struct(new_img->png_ptr);
    if (!new_img->info_ptr) {
        png_destroy_read_struct(&new_img->png_ptr,(png_infopp)NULL, (png_infopp)NULL);
        printf("Error in make_empty_image function: png_create_info_struct failed.\n");
        exit(41);
    }
    png_read_update_info(new_img->png_ptr, new_img->info_ptr);
    new_img->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * new_img->height);
    for (int y = 0; y < new_img->height; y++) {
        new_img->row_pointers[y] = (png_byte *) malloc((png_get_rowbytes(img->png_ptr, img->info_ptr) / img->width) * new_img->width);
    }
    if (is_have_background) {
        for (int y = 0; y < new_img->height; y++) {
            for (int x = 0; x < new_img->width; x++) {
                int white_pixel[4] = {255, 255, 255, 255};
                set_pixel(new_img, x, y, white_pixel);
            }
        }
    }
}
void insert_part_of_image(Png *img, Png *new_img, int x1, int y1, int x2, int y2, int i, int j, int insert_to_new_image_fl) {
    for (int y = y1; y <= y2; y++) 
    {
        for (int x = x1; x <= x2; x++) 
        {
            int color_arr[3];
            color_arr[0] = img->row_pointers[y][x * img->channels + 0];
            color_arr[1] = img->row_pointers[y][x * img->channels + 1];
            color_arr[2] = img->row_pointers[y][x * img->channels + 2];
            if (insert_to_new_image_fl) {
                set_pixel(new_img, x + i, y + j, color_arr);
            } else {
                set_pixel(img, x + i, y + j, color_arr);
            }
        }
    }
}
void make_collage(Png *img, Png *new_img, int n, int m) 
{
    make_empty_image(img, new_img, (int) img->height * m, (int) img->width * n, 0);
    for (int i = 0; i < new_img->width; i+=(int)img->width) {

        insert_part_of_image(img, new_img, 0, 0, (int)img->width - 1, (int)img->height - 1, i, 0, 1);
    }
    for (int j = (int) img->height; j < new_img->height; j += (int) img->height) {
        insert_part_of_image(new_img, NULL, 0, 0, (int)new_img->width - 1, (int) img->height - 1, 0, j, 0);
    }
}

void print_help() {
    printf("Course work for option 4.19, created by Aleksandr Snigirev.\n");
    /*FILE *file = fopen("help", "rb");
    if (!file) {
        printf("Help_info_file is not found.\n");
        exit(22);
    }
    char ch = (char) fgetc(file);
    while (ch != EOF) {
        printf("%c", ch);
        ch = (char) fgetc(file);
    }
    fclose(file);*/
}

void take_coords(const char* inputString, triangle* points) {
    char* token;
    int i = 0;

    char str[100];
    strcpy(str, inputString);

    token = strtok(str, DOT);
    while(token != NULL && i < 6) {
        switch(i) {
            case 0:
                points->x1 = atoi(token);
                break;
            case 1:
                points->y1 = atoi(token);
                break;
            case 2:
                points->x2 = atoi(token);
                break;
            case 3:
                points->y2 = atoi(token);
                break;
            case 4:
                points->x3 = atoi(token);
                break;
            case 5:
                points->y3 = atoi(token);
                break;
        }
        token = strtok(NULL, ".");
        i++;
    }
}

void take_color(const char* inputString, int* color_arr){
    char* token;
    int i = 0;

    char str[100];
    strcpy(str, inputString);

    token = strtok(str, DOT);
    while(token != NULL && i < 3) {
        if(atoi(token)<0 || atoi(token)>255){
            printf("Ircorrect value of color\n");
            exit(40);
        }
        switch(i) {
            case 0:
                color_arr[0] = atoi(token);
                break;
            case 1:
                color_arr[1] = atoi(token);
                break;
            case 2:
                color_arr[2] = atoi(token);
                break;
        }
        token = strtok(NULL, ".");
        i++;
    }
}
void take_points(const char* inputString, int* x, int* y){
    char* token;
    int i = 0;

    char str[100];
    strcpy(str, inputString);

    token = strtok(str, DOT);
    while(token != NULL && i < 2) {
        switch(i) {
            case 0:
                *x = atoi(token);
                break;
            case 1:
                *y = atoi(token);
                break;
        }
        token = strtok(NULL, ".");
        i++;
    }
}

int check_str(char* string){
    int num=0;
    for(int i=0;i<strlen(string);i++)
    {
        if(string[i]==DOT)
            num++;
        if(isdigit(string[i])==0 && string[i]!=DOT && string[i]!='-')
            return 0;
        if(string[i]==DOT && string[i+1]==DOT)
            return 0;
    }
    if(num!=5)
        return 0;
    return 1;
}
int check_rgb(char* str){
    int num=0;
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='.')
            num++;  
        if(isdigit(str[i]) || str[i]==DOT || str[i]=='-'){
            continue;   
        } 
        else{
            return 0;
        }
        if(str[i]==DOT && str[i+1]==DOT)

            return 0;

    }

    if(num!=2)
        return 0;
    return 1;
}


void contrast(Png* img, float alpha, int beta){
    //new=alphA*old+beta;
    int arr[3];
    for(int y=0;y<img->height;y++){
        for(int x=0;x<img->width;x++){
            /*img->row_pointers[y][x * img->channels+0]=(int)(img->row_pointers[y][x * img->channels+0]*alpha+beta);
            img->row_pointers[y][x * img->channels+1]=(int)(img->row_pointers[y][x * img->channels+1]*alpha+beta);
            img->row_pointers[y][x * img->channels+2]=(int)(img->row_pointers[y][x * img->channels+2]*alpha+beta);*/

            arr[0]=(int)(img->row_pointers[y][x * img->channels+0]*alpha+beta);
            arr[1]=(int)(img->row_pointers[y][x * img->channels+1]*alpha+beta);
            arr[2]=(int)(img->row_pointers[y][x * img->channels+2]*alpha+beta);
            if(arr[0]>255){
                arr[0]=255;
            }
            if(arr[1]>255){
                arr[1]=255;
            }
            if(arr[2]>255){
                arr[2]=255;
            }
            set_pixel(img, x,y, arr);
        }
    }
}




int main(int argc, char **argw){
    if(argc<=1){
        print_help();
        return 0;
    }
    Png image, new_image;
    int opt=0;
    triangle trgl;
    rect rct;
    Mirror my_mir;
    char input_file[255];
    char output_file[255];
    int triangle_param_c = 0;
    int rectangle_param_c = 0;
    int collage_param_c = 0;
    int png_info=0;
    int num_x=0; 
    int num_y=0;
    int fl=0;
    
    char key;
    int our_color[3];
    int fill_color[3];
    fill_color[0]=-1;
    fill_color[1]=-1;
    fill_color[2]=-1;
    int x1,y1,x2,y2;

    Cont my_cont;



    char* flags=ARGS;
    int idx=0;
    struct option long_flags[]={
        {"contrast", no_argument, NULL, 'q'},
        {"alpha", required_argument, NULL, 'A'},
        {"beta", required_argument, NULL, 'B'},
        {"help", no_argument,NULL, 'h'},//
        {"triangle", no_argument, NULL, 't'},//
        {"mirror", no_argument, NULL, 'm'},
        {"collage", no_argument, NULL, 'C'},//
        {"biggest_rect", no_argument, NULL,'R'},
        {"change_color", no_argument, NULL, 'D'},//
        {"info", no_argument, NULL,'I'},
        {"resize", no_argument, NULL,'P'},
        {"outside_rect", no_argument, NULL,'Q'},
        {"right", required_argument, NULL, 'b'},
        {"left", required_argument, NULL, 'd'},
        {"above", required_argument, NULL, 'e'},
        {"below", required_argument, NULL, 'z'},
        {"points", required_argument, NULL, 'p'},//
        {"left_up", required_argument, NULL, 'l'},
        {"right_down", required_argument, NULL, 'r'},
        {"axis", required_argument, NULL, 'a'},
        {"thickness", required_argument, NULL, 'T'},//
        {"color", required_argument, NULL, 'c'},//
        {"fill", optional_argument, NULL, 'f'},//
        {"fill_color", required_argument, NULL, 'w'},
        {"output", required_argument, NULL, 'o'},//
        {"input", required_argument, NULL, 'i'},
        {"old_color", required_argument, NULL, 'O'},
        {"new_color", required_argument, NULL, 'n'},
        {"number_y", required_argument, NULL, 'y'},
        {"number_x", required_argument, NULL, 'x'},
        {0,0,0,0}
    };

    opt=getopt_long(argc, argw, flags,long_flags, &idx);

    while(opt!=-1){

        switch(opt){
            case 'h':
                print_help();
                return 0;

                //CONTRAAAAAST
            case 'q':
                key='q';
                break;
///////////////////////////////////////////////

            case 't'://draw triangle
                key='t';
                break;
            case 'Q'://draw triangle
                key='Q';
                break;    
            case 'D':
                key = 'D';
                break;
            case 'm':
                key='m';
                break;        
            case 'R'://recolor rectangle
                key='R';
                break;
            case 'C'://Collage
                key='C';
                break;
             case 'P'://Collage
                key='P';
                break;    
            case 'o':
                strcpy(output_file, optarg);
                break;
            case 'i':
                strcpy(input_file, optarg);
                break;    
            case 'I':
                png_info++;
                break;    
            case 'p':
                if(check_str(optarg)==0){
                    printf("Некорректное значение координат вершин треугольника\n");
                    exit(43);
                }
                take_coords(optarg, &trgl);
                triangle_param_c++;
                break;
            case 'T':
                if(atoi(optarg)<=0){
                    printf("Некорректное значение thickness\n");
                    exit(44);
                }

                trgl.thickness=atoi(optarg);
                triangle_param_c++;

                break;
            case 'c':
                if(check_rgb(optarg)==0){
                    printf("Некорректное значение цвета линии\n");
                    exit(45);
                }
                take_color(optarg, our_color);
                
                triangle_param_c++;
                break;
            case 'f':
                if (optarg) {
                    if (strcmp(optarg, "true") == 0) {
                        trgl.is_fill = 1;
                       // fill_option = 1;
                    } else if (strcmp(optarg, "false") == 0) {
                        trgl.is_fill = 0;
                       // fill_option = 1;
                    } else {
                        printf("Unknown argument for --fill\n");
                        return 1;
                    }
                } else {
                    trgl.is_fill = 1;
                    //fill_option = 1;
                }
                break;
            case 'w':
                if(check_rgb(optarg)==0){
                    printf("Некорректное значение цвета\n");
                    exit(45);
                }
                
                take_color(optarg, fill_color);
                triangle_param_c++;
                fl=1;
                break;
            case 'O':
                if(check_rgb(optarg)==0){
                    printf("Некорректное значение цвета\n");
                    exit(45);
                }
                take_color(optarg, rct.old_color);
                rectangle_param_c++;
                break;
            case 'n':
                if(check_rgb(optarg)==0){
                    printf("Некорректное значение цвета\n");
                    exit(45);
                }
                take_color(optarg, rct.new_color);
                rectangle_param_c++;
                break;
            case 'y':
                if(atoi(optarg)>0){
                    num_y=atoi(optarg);
                    collage_param_c++;
                }
                else{
                printf("Некорректное значение коллажа\n");
                exit(46);
                }
                break;    
            case 'x':
                if(atoi(optarg)>0){
                    num_x=atoi(optarg);
                    collage_param_c++;
                }
                else{
                printf("Некорректное значение коллажа\n");
                exit(46);  
                }   
                break;
            case 'l':
                take_points(optarg, &x1, &y1);
                break;    
            case 'r':
                take_points(optarg, &x2, &y2);
                break;
            case 'a':
                if(strcmp(optarg, "x")==0){
                    my_mir.mode = 'x';
                }         
                else if(strcmp(optarg, "y")==0){
                    my_mir.mode = 'y';
                }
                break;
            case 'A':
                my_cont.alpha = atof(optarg);
                break;
                
            case 'B':
                my_cont.beta=atoi(optarg);    
                break;
            case '?':
                printf("Arguments entered incorrectly.\n");
                return 0;    
        }
        opt=getopt_long(argc, argw, flags,long_flags, &idx);
    }
    if(strlen(input_file)==0)
        strcpy(input_file, argw[argc-1]);
    if(strlen(output_file)==0){
        strcpy(output_file, "out.png");
    }
    if(strcmp(input_file,output_file)==0){
        printf("Имена входного и выходного файлов совпадают\n");
        exit(40);
    }

    read_png_file(input_file, &image);
    if(png_info){
        printf("Тип файла %s: PNG\n", input_file);
        printf("Высота изображения: %d\n", image.height);
        printf("Высота изображения: %d\n", image.width);
        exit(41);
    }
    switch(key){
        case 't':
        
        if(trgl.is_fill==1 && fl==0){
            printf("Error.Expected flag --fill_color\n");
            exit(40);
        }
        if(trgl.is_fill==0 && fill_color[0]!=-1){
            trgl.is_fill=0;
            printf("Warning! Ircorrect args\n");
        }
            draw_triangle(&image, trgl.x1, trgl.y1,trgl.x2, trgl.y2,trgl.x3, trgl.y3, trgl.thickness, our_color, trgl.is_fill,fill_color);
            write_png_file(output_file, &image);
            break;
        case 'R':
            recolor_biggest_rectangle(&image, rct.old_color, rct.new_color);
            write_png_file(output_file, &image);
            break;
        case 'C':
            make_collage(&image,&new_image, num_x,num_y);
            write_png_file(output_file, &new_image);
            free_image_data(&new_image);
            break;

        case 'm':
            mirror_function(&image, my_mir.x1, my_mir.y1, my_mir.x2, my_mir.y2, my_mir.mode);
            write_png_file(output_file, &image);
            break;
        case 'D':
            change_color(&image, rct.new_color);
            write_png_file(output_file, &image);
            break;    
        case 'q':
            contrast(&image, my_cont.alpha, my_cont.beta);
            write_png_file(output_file, &image);    
            break;

    }
    free_image_data(&image);
}
