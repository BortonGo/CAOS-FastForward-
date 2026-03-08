extern int readi32(void);

int func(int x, int y) {
    int d = readi32();
    if (d % 2) {
        return x + y;
    } else {
        return x - y;
    }
}
