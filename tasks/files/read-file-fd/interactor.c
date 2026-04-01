#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int a = 64, u = 0;
    char *v = malloc(a * sizeof(v[0]));

    signal(SIGPIPE, SIG_IGN);

    if (argc < 3) {
        fprintf(stderr, "wrong number of arguments\n");
        exit(6);
    }
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        fprintf(stderr, "cannot open '%s' for reading\n", argv[1]);
        exit(6);
    }
    int c;
    while ((c = getc_unlocked(in)) != EOF) {
        if (u == a) {
            v = realloc(v, (a *= 2) * sizeof(v[0]));
        }
        v[u++] = c;
    }
    fclose(in);
    in = 0;

    // write buffer
    const unsigned char *p = (const unsigned char *)v;
    int r = u * sizeof(*v);

    if (!r) {
        close(1);
    }

    // read buffer
    unsigned char b[65536];
    int sa = sizeof(b), su = 0;
    unsigned char *s = malloc(sa);

    // need non-blocking mode
    fcntl(1, F_SETFL, fcntl(1, F_GETFL) | O_NONBLOCK);

    fd_set rfd, wfd;
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    while (1) {
        FD_SET(0, &rfd);
        if (r > 0) {
            FD_SET(1, &wfd);
        } else {
            FD_CLR(1, &wfd);
        }
        int n = select(2, &rfd, &wfd, NULL, NULL);
        if (n <= 0) {
            continue;
        }
        if (FD_ISSET(0, &rfd)) {
            int rr = read(0, b, sizeof(b));
            if (rr < 0) {
                fprintf(stderr, "input error on standard input\n");
                exit(6);
            }
            if (!rr) {
                if (r > 0) {
                    fprintf(stderr, "unexpected EOF on standard input\n");
                }
                break;
            }
            if (su + rr > sa) {
                while (su + rr > sa) {
                    sa *= 2;
                }
                s = realloc(s, sa * sizeof(*s));
            }
            memcpy(s + su, b, rr);
            su += rr;
        }
        if (FD_ISSET(1, &wfd)) {
            int ww = write(1, p, r);
            if (ww < 0) {
                if (errno == EAGAIN) {
                    continue;
                }

                fprintf(stderr, "write error on standard output\n");
                close(1);
                r = 0;
            } else {
                p += ww;
                r -= ww;
                if (!r) {
                    close(1);
                }
            }
        }
    }

    FILE *out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "cannot open '%s' for writing\n", argv[2]);
        exit(6);
    }
    for (int i = 0; i < su; ++i) {
        putc_unlocked(s[i], out);
    }
    fclose(out);
    return 0;
}
