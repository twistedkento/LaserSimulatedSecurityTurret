#include "pngtest.h"

int main() {
	buffer buf;
	file_to_buffer(&buf, "test.png");
	printf(buf);
	return 0;
}

#define CONTROLL_BYTES 8

int is_png(FILE* file) {
	char buffer[CONTROLL_BYTES];
	fread(buffer, 1, CONTROLL_BYTES, file);
	assert(file);
	return !png_sig_cmp(buffer, 0, CONTROLL_BYTES);
}

void _abort(char* error) {
	puts(error);
	abort();
}

void alloc(buffer* pBuffer,int size){
	*pBuffer = malloc(size * sizeof(char));
}

png_bytep* file_to_bytes(char* fileName) {
	FILE* f = fopen(fileName, "rb");
	if(is_png(f)) {
		png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(!png_pointer)
			_abort("png_create_read_struct failed");
		png_infop png_info = png_create_info_struct(png_pointer);
		if(!png_info)
			_abort("png_create_info_struct failed");
		// DARK MAGIC, dunnolol
		png_read_info(png_pointer, png_info);
		
		int width = png_get_image_width(png_pointer, png_info);
		int height = png_get_image_height(png_pointer, png_info);

		png_bytep* row_pointers = (png_byte*) malloc(sizeof(png_bytep) * heigth);
		int y = 0;
		for(y=0;y < height;y++) {
			row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_pointer,png_info));
		}

	}
}

int file_to_buffer(buffer* buf, char* fileName) {
	FILE* f = fopen(fileName, "rb");
	puts("hej");
	if (is_png(f) != 0) {
		// set return buffer size
		fseek(f, 0, SEEK_END);
		int buf_length = ftell(f);
		alloc(buf, buf_length);
		fseek(f, 0, 0);
		fread(*buf, sizeof(char), buf_length, f);
		fclose(f);
		return 0;
	}

	fclose(f);
	return 0;
}
