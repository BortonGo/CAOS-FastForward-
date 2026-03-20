void normalize_path(char *buf) {
    char *read = buf;
    char *write = buf;

    int prev_was_slash = 0;

    while (*read) {
        if (*read == '/') {
            if (!prev_was_slash) {
                *write++ = '/';
                prev_was_slash = 1;
            }
        } else {
            *write++ = *read;
            prev_was_slash = 0;
        }
        ++read;
    }

    *write = '\0';
}
