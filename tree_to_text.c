/*
 * Copyright (c) 2024 Logan Ryan McLintock
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * tree_to_text: Converts a tree from binary to text format.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BINARY_HASH_LEN 20
#define HEADER_STR "tree "
#define HEADER_LEN 5

int main(int argc, char **argv)
{
    int ret = 1;                /* Error by default */
    char buf[HEADER_LEN + 1];
    int x;

    size_t i;

    if (argc != 1) {
        fprintf(stderr, "Usage: %s (reads binary tree file from stdin)\n",
                *argv);
        return 1;
    }

    /* Header -- tree */
    if (fread(buf, 1, HEADER_LEN, stdin) != HEADER_LEN)
        goto error;

    buf[HEADER_LEN] = '\0';

    if (strcmp(buf, HEADER_STR)) {
        fprintf(stderr, "%s: ERROR: Invalid tree header\n", *argv);
        goto error;
    }

    buf[HEADER_LEN - 1] = ':';
    printf("%s", buf);

    /* Size */
    while (1) {
        x = getchar();
        if (x == EOF) {
            goto error;
        } else if (x == '\0') {
            break;
        } else if (isdigit(x)) {
            putchar(x);
        } else {
            fprintf(stderr, "ERROR: Invalid character in size field\n");
            goto error;
        }
    }

    putchar('\n');

    while (1) {
        /* File permissions -- First char */
        x = getchar();
        if (x == EOF) {
            break;              /* OK to be at the end of the file here */
        } else if (x == ' ') {
            fprintf(stderr, "%s: ERROR: Empty file permissions\n", *argv);
            goto error;
        } else if (isdigit(x)) {
            putchar(x);
        } else {
            fprintf(stderr, "%s: ERROR: Invalid file permissions\n",
                    *argv);
            goto error;
        }

        /* File permissions -- Other chars */
        while (1) {
            x = getchar();
            if (x == EOF) {
                goto error;
            } else if (x == ' ') {
                putchar(':');
                break;
            } else if (isdigit(x)) {
                putchar(x);
            } else {
                fprintf(stderr, "%s: ERROR: Invalid file permissions\n",
                        *argv);
                goto error;
            }
        }

        /* Filename */
        while (1) {
            x = getchar();
            if (x == EOF) {
                goto error;
            } else if (x == '\0') {
                putchar(':');
                break;
            } else if (x == ':' || x == '\n') {
                fprintf(stderr,
                        "%s: ERROR: Filename "
                        "has a semicolon or newline char\n", *argv);
                goto error;
            } else {
                putchar(x);
            }
        }

        /* SHA1 hash -- Convert binary to hex */
        for (i = 0; i < BINARY_HASH_LEN; ++i) {
            x = getchar();
            if (x == EOF) {
                goto error;
            } else {
                printf("%02x", x);
            }
        }

        putchar('\n');
    }

    ret = 0;                    /* OK */

  error:

    return ret;
}
