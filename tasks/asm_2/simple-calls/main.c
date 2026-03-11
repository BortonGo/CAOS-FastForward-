extern long long readi64(void);
extern void writei64(long long);
extern long long func(long long x, long long y, long long z);

int main() {
    long long x = readi64();
    long long y = readi64();
    long long z = readi64();

    writei64(func(x - y, y, z));

    writei64(16);
}
