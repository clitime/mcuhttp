#include <string.h>


#include "fs.h"
#include "fsdata.h"


err_t fs_open(struct fs_file *file, const char *name) {
    const struct fsdata_file *f;

    if ((file == NULL) || (name == NULL)) {
        return -16;
    }

    for (f = &FS_ROOT; f != NULL; f = f->next) {
        if (!strcmp(name, (const char *)f->name)) {
            file->data = (const char *)f->data;
            file->len = f->len;
            file->index = f->len;
            file->pextension = NULL;
            return 0;
        }
    }
    /* file not found */
    return -6;
}


void fs_close(struct fs_file *file) {
    memset(file, 0, sizeof(struct fs_file));
}


int fs_bytes_left(struct fs_file *file) {
    return file->len - file->index;
}
