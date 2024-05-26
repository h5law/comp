// Copyright 2024 Harry Law (h5law)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

#include "./cmd.h"

void zerr(int, FILE *, FILE *);

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr,
                "usage: zlib <-c[1-9]/-d> <input_file> <output_file>\n");
        exit(1);
    }
    FILE *source, *dest;
    source = fopen(argv[2], "r");
    dest = fopen(argv[3], "w");

    int res;
    if (strncmp(argv[1], "-c", 2) == 0) {
        int level;
        if (strlen(argv[1]) == 2) {
            level = 5;
        }
        if (strlen(argv[1]) == 3) {
            level = atoi(&argv[1][2]);
        } else {
            fprintf(stderr,
                    "usage: zlib <-c[1-9]/-d> <input_file> <output_file>\n");
            exit(1);
        }
        if (level < 1 || level > 9) {
            fprintf(stderr,
                    "usage: zlib <-c[1-9]/-d> <input_file> <output_file>\n");
            exit(1);
        }
        printf("Compressing file: %s into %s with compression level %d\n",
               argv[2], argv[3], level);
        res = def(source, dest, level);
    } else if (strcmp(argv[1], "-d") == 0) {
        printf("Decompressing file: %s into %s\n", argv[2], argv[3]);
        res = inf(source, dest);
    } else {
        fprintf(stderr,
                "usage: zlib <-c[1-9]/-d> <input_file> <output_file>\n");
        exit(1);
    }
    if (res != Z_OK) {
        zerr(res, source, dest);
    }

    fclose(source);
    fclose(dest);

    return res == Z_OK ? 0 : -1;
}

void zerr(int ret, FILE *source, FILE *dest) {
    fputs("zcomp: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(source))
            fputs("error reading source file\n", stderr);
        if (ferror(dest))
            fputs("error writing to output file\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}
