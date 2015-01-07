#include "stdafx.h"
#include "test_snappy.h"
#include "google/snappy/snappy-c.h"

void test_snappy()
{
    const char* s = "hello";

    size_t output_length = snappy_max_compressed_length(6);
    char* output = (char*)malloc(output_length);
    if (snappy_compress(s, 6, output, &output_length) == SNAPPY_OK)
    {
        size_t output_length2;
        snappy_status state = snappy_uncompressed_length(output, output_length, &output_length2);
        char* output2 = (char*)malloc(output_length2);
        if (snappy_uncompress(output, output_length, output2, &output_length2) == SNAPPY_OK)
        {
            printf(output2);
        }
        free(output2);
    }
    free(output);
}
