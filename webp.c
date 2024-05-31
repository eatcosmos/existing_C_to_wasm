#include "emscripten.h"
#include "src/webp/encode.h"

EMSCRIPTEN_KEEPALIVE
int version() {
  return WebPGetEncoderVersion();
}
/*
emcc -O3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
    -I libwebp \
    webp.c \
    libwebp/src/{dec,dsp,demux,enc,mux,utils}/*.c \
    libwebp/sharpyuv/*.c
*/
int result[2];
EMSCRIPTEN_KEEPALIVE
int encode(uint8_t* img_in, int width, int height, int quality) {
  uint8_t* img_out = NULL;
  size_t size_out = 0;

  size_out = WebPEncodeRGBA(img_in, width, height, width * 4, quality, &img_out);

  result[0] = (int)img_out; //返回的不就是int吗？为什么还要转换？img_out通常是一个指向编码后WebP图像数据的指针，它的类型是uint8_t*，即无符号8位整数的指针。这个指针指向的内存区域包含了编码后的WebP图像数据。   
  result[1] = size_out;
}

EMSCRIPTEN_KEEPALIVE
void free_result(uint8_t* result) {
 WebPFree(result);
}

EMSCRIPTEN_KEEPALIVE
int get_result_size() {
  return result[1];
}

EMSCRIPTEN_KEEPALIVE
int get_result_pointer() {
  return result[0];
}

EMSCRIPTEN_KEEPALIVE
uint8_t* create_buffer(int width, int height) {
  return malloc(width * height * 4 * sizeof(uint8_t));
}

EMSCRIPTEN_KEEPALIVE
void destroy_buffer(uint8_t* p) {
  free(p);
}

