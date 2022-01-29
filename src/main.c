#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static char* shift(int* argc, char** argv[]) {
    char* res = *argv[0];
    *argc -= 1;
    *argv += 1;
    return res;
}

static void usage(FILE* stream, char* name) {
    fprintf(stream, "Usage: %s -i [INPUT_IMAGE] -o [OUTPUT_IMAGE] [OPTIONS]\n", name);
    fprintf(stream, "  -st [NUMBER] (default 6): Number of pixels between each line.\n");
    fprintf(stream, "  -sz [NUMBER] (default 2): Number of pixels in each line.\n");
}

int main(int argc, char* argv[]) {

    char* exec = shift(&argc, &argv);

    if(argc < 4) {
        usage(stderr, exec);
        exit(1);
    }

    char* input_path  = NULL;
    char* output_path = NULL;
    int step          = 6;
    int size          = 2;

    while(argc > 0) {
        char* flag = shift(&argc, &argv);
    
        if(strcmp(flag, "-i") == 0) {
            if(argc < 1) {
                fprintf(stderr, "[ERROR]: Input path is not provided.\n");
                usage(stderr, exec);
                exit(1);
            }

            input_path = shift(&argc, &argv);
        } else if(strcmp(flag, "-o") == 0) {
            if(argc < 1) {
                fprintf(stderr, "[ERROR]: Output path is not provided.\n");
                usage(stderr, exec);
                exit(1);
            }

            output_path = shift(&argc, &argv);
        } else if(strcmp(flag, "-st") == 0) {
            if(argc < 1) {
                fprintf(stderr, "[WARNING]: Step value is not set. Using default [6].\n");
                continue;
            }

            int val = strtod(shift(&argc, &argv), NULL);
            
            if(val <= 0) {
                fprintf(stderr, "[WARNING]: Step value is incorrcet. Using deafult [6]\n");    
                continue;
            }

            step = val;
        } else if(strcmp(flag, "-sz") == 0) {
            if(argc < 1) {
                fprintf(stderr, "[WARNING]: Size value is not set. Using default [2].\n");
                continue;
            }

            int val = strtod(shift(&argc, &argv), NULL);
            
            if(val <= 0) {
                fprintf(stderr, "[WARNING]: Size value is incorrcet. Using deafult [2]\n");    
                continue;
            }

            size = val;
        }
    }
   
    if(input_path == NULL) {
        fprintf(stderr, "[ERROR]: Input path is not provided.\n");
        usage(stderr, exec);
        exit(1);
    }

    if(output_path == NULL) {
        fprintf(stderr, "[ERROR]: Output path is not provided.\n");
        usage(stderr, exec);
        exit(1);
    }

    int w, h, c;
    uint8_t* data = stbi_load(input_path, &w, &h, &c, 0);

    if(data == NULL) {
        fprintf(stderr, "[ERROR]: Could not read image `%s`: %s\n", input_path, strerror(errno));
        exit(1);
    }

    printf("[LOADED]: `%s` (W: %d, H: %d, C: %d)\n", input_path, w, h, c);
    printf("[PROCESSING]: (STEP: %d, SIZE: %d)\n", step, size);

    for(int y = 0; y < h; y++) {
        if ((y % step) == 0) {
            for(int x = 0; x < w * c; x++) {
                for (int i = 0; i < size; i++) {
                    data[(y + i) * w * c + x] = 0;
                }
            }
          y += size;
        }
    }

    printf("[WRITING]: `%s`\n", output_path);

    stbi_write_png(output_path, w, h, c, data, w * c);
    stbi_image_free(data);

    return 0;
}
