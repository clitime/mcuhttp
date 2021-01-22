#ifndef FS_H__
#define FS_H__

#include <stdint.h>


#define FS_READ_EOF -1
#define FS_READ_DELAYED -2

struct fs_file {
    const char *data;
    int len;
    int index;
    void *pextension;
};

typedef int8_t err_t;

err_t fs_open(struct fs_file *file, const char *name);
void fs_close(struct fs_file *file);
int fs_bytes_left(struct fs_file *file);

#endif /* __FS_H__ */
