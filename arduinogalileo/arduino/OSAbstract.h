
#ifndef __OS_ABSTRACT_H__
#define __OS_ABSTRACT_H__

void init(int argc, char * argv[]);
void setup(void);
void loop(void);

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define word(x, y)	((unsigned long)x>>8 | (unsigned long)y <<8)
#endif /* __OS_ABSTRACT_H__ */
