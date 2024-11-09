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

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int ret = 1;                /* Error by default */
    int x;
    size_t i;

    if (argc != 1) {
        fprintf(stderr, "Usage: %s (reads binary tree file from stdin)\n",
                *argv);
        return 1;
    }

    /* Header */
    /* tree */
    while (1) {
        x = getchar();
        if (x == EOF) {
            goto error;
        } else if (x == ' ') {
            putchar(':');
            break;
        } else {
            putchar(x);
        }
    }

    /* Size */
    while (1) {
        x = getchar();
        if (x == EOF) {
            goto error;
        } else if (x == '\0') {
            break;
        } else if (x == ':' || x == '\n') {
            fprintf(stderr, "ERROR: Invalid character in size field\n");
            goto error;
        } else {
            putchar(x);
        }
    }

    putchar('\n');

    while (1) {
        /* File permissions */
        x = getchar();          /* Read first char only */
        if (x == EOF) {
            break;              /* OK to be at the end of the file here */
        } else if (x == ' ') {
            fprintf(stderr, "ERROR: Empty file permissions\n");
            goto error;
        } else {
            putchar(x);
        }

        while (1) {
            x = getchar();
            if (x == EOF) {
                goto error;
            } else if (x == ' ') {
                putchar(':');
                break;
            } else {
                putchar(x);
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
                fprintf(stderr, "ERROR: Invalid character in filename\n");
                goto error;
            } else {
                putchar(x);
            }
        }

        /* SHA1 hash */
        for (i = 0; i < 20; ++i) {
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
