#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ext2.h"
#include <errno.h>
#include <string.h>

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024
#define IMAGE "image_big.img"

typedef unsigned char bmap;
#define __NBITS (8 * (int) sizeof (bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap) 1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT (d)] |= __BMMASK (d)))
#define BM_CLR(d, set) ((set[__BMELT (d)] &= ~__BMMASK (d)))
#define BM_ISSET(d, set) ((set[__BMELT (d)] & __BMMASK (d)) != 0)

unsigned int block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

bmap *bitmap;
bmap *bitmap2;
bmap *bitmap3;
char *image_name;
int dir_count = 2;
int dir_offset = 0;
int config = 0;
int inc = 1;

struct ext2_inode getInode(unsigned int index)
{
    int fd;
    struct ext2_super_block super;
    if ((fd = open(image_name, O_RDONLY)) < 0) {
        perror(IMAGE);
        exit(1);
    }
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    struct ext2_inode inode;
    struct ext2_group_desc group;
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));
    lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*(index-1), SEEK_SET);
    read(fd, &inode, sizeof(struct ext2_inode));
    return inode;
}

void recover()
{
    struct ext2_super_block super;
    struct ext2_group_desc group;
    int flag = 0;
    int file_no = 0;
    int fd;
    unsigned int *read_block = malloc(block_size);
    unsigned int *read_block2 = malloc(block_size);
    unsigned int *read_block3 = malloc(block_size);
    if ((fd = open(image_name, O_RDWR)) < 0) {
        perror(IMAGE);
        exit(1);
    }
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));
    int at = super.s_inodes_per_group/8;
    int murat = BLOCK_OFFSET(group.bg_inode_table)/block_size;
    int off = at + murat;
    
    int ln = 1;
    int out_number[super.s_inodes_per_group+1];
    struct ext2_inode inode;
    int dtime[super.s_inodes_per_group+1];
    for(int i = 0; i<super.s_inodes_per_group+1; i++)
    {
        out_number[i] = 0;
        dtime[i] = 0;
    }
    for (int i = 12; i < super.s_inodes_per_group+1; i++)
    {
        if (!BM_ISSET(i-1,bitmap2) && dtime[i] == 0)
        {
            inode = getInode(i);
            if(inode.i_size != 0 && inode.i_dtime > 0)
            {
                out_number[i] = ln;
                printf("file%d%d %u %u\n", ln/10, (ln%10), inode.i_dtime, (inode.i_blocks*512)/block_size);
                ln++;
            }
        }
    }
    printf("###\n");
    while(1)
    {
        int dt = 0;
        int ind = -1;
        for (int i = 12; i < super.s_inodes_per_group+1; i++)
        {
            if (!BM_ISSET(i-1,bitmap2) && dtime[i] == 0)
            {
                //printf("girdim\n");
                inode = getInode(i);
                if(inode.i_size != 0 && inode.i_dtime > 0)
                {
                    if(dt < inode.i_dtime)
                    {
                        dt = inode.i_dtime;
                        ind = i;
                    }
                }
            }
        }
        if(ind == -1)
        {
            break;
        }
        dtime[ind] = 1;
        inode = getInode(ind);
        int block_c = 12;
        //printf("imode: %x\n", inode.i_mode);
        int flag = 0;
        int ex = 0;
        int counter = 0;
        file_no++;
        for(int j=0; j < 12; j++)
        {
            if(inode.i_block[j] != 0)
            {
                counter++;
                if(BM_ISSET(inode.i_block[j]-1,bitmap))
                {
                    out_number[ind] = 0;
                    flag = -1;
                    break;
                }
                flag++;
            }
        }
        if(inode.i_block[12] != 0)
        {
            int xd;
            lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[12]-off), SEEK_SET);
            read(fd, read_block, block_size);
            for(int k = 0; k<block_size/4; k++)
            {
                block_c++;
                if(block_c > (inode.i_size / block_size));
                {
                    break;
                }
                counter++;
                if(BM_ISSET(read_block[k]-1,bitmap))
                {
                    out_number[ind] = 0;
                    flag = -1;
                    break;
                }
                flag++;
            }
        }
        if(inode.i_block[13] != 0)
        {
            lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[13]-off), SEEK_SET);
            read(fd, read_block, block_size);
            for(int k = 0; k<block_size/4; k++)
            {
                if(ex)
                {
                    break;
                }
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block[k]-off), SEEK_SET);
                read(fd, read_block2, block_size);
                for(int m = 0; m<block_size/4; m++)
                {
                    block_c++;
                    if(block_c > (inode.i_size / block_size));
                    {
                        ex = 1;
                        break;
                    }
                    counter++;
                    if(BM_ISSET(read_block2[m]-1,bitmap))
                    {
                        out_number[ind] = 0;
                        flag = -1;
                        break;
                    }
                    flag++;
                }
            }
        }
        if(inode.i_block[14] != 0)
        {
            lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[14]-off), SEEK_SET);
            read(fd, read_block, block_size);
            for(int k = 0; k<block_size/4; k++)
            {
                if(ex)
                {
                    break;
                }
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block[k]-off), SEEK_SET);
                read(fd, read_block2, block_size);
                for(int m = 0; m<block_size/4; m++)
                {
                    if(ex)
                    {
                        break;
                    }
                    lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block2[m]-off), SEEK_SET);
                    read(fd, read_block3, block_size);
                    for(int n = 0; n<block_size/4; n++)
                    {
                        block_c++;
                        if(block_c > (inode.i_size / block_size));
                        {
                            ex = 1;
                            break;
                        }
                        counter++;
                        if(BM_ISSET(read_block3[n]-1,bitmap))
                        {
                            out_number[ind] = 0;
                            flag = -1;
                            break;
                        }
                        flag++;
                    }
                }
            }
        }
        block_c = 12;
        ex = 0;
        if(flag == counter)
        {
            for(int j=0; j < 12; j++)
            {
                if(inode.i_block[j] != 0)
                {
                    BM_SET(inode.i_block[j]-1, bitmap);
                }
            }
            if(inode.i_block[12] != 0)
            {
                BM_SET(inode.i_block[12]-1, bitmap);
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[12]-off), SEEK_SET);
                read(fd, read_block, block_size);
                for(int k = 0; k<block_size/4; k++)
                {
                    if(!read_block[k])//if(block_c > (inode.i_size / block_size))
                    {
                        break;
                    }
                    block_c++;
                    BM_SET(read_block[k]-1, bitmap);
                }
            }
            if(inode.i_block[13] != 0)
            {
                BM_SET(inode.i_block[13]-1, bitmap);
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[13]-off), SEEK_SET);
                read(fd, read_block, block_size);
                for(int k = 0; k<block_size/4; k++)
                {
                    if(ex)
                    {
                        break;
                    }
                    BM_SET(read_block[k]-1, bitmap);
                    lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block[k]-off), SEEK_SET);
                    read(fd, read_block2, block_size);
                    for(int m = 0; m<block_size/4; m++)
                    {
                        if(!read_block2[m])//if(block_c > (inode.i_size / block_size));
                        {
                            ex = 1;
                            break;
                        }
                        block_c++;
                        BM_SET(read_block2[m]-1, bitmap);
                    }
                }
            }
            if(inode.i_block[14] != 0)
            {
                BM_SET(inode.i_block[14], bitmap);
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(inode.i_block[14]-off), SEEK_SET);
                read(fd, read_block, block_size);
                for(int k = 0; k<block_size/4; k++)
                {
                    if(ex)
                    {
                        break;
                    }
                    lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block[k]-off), SEEK_SET);
                    read(fd, read_block2, block_size);
                    for(int m = 0; m<block_size/4; m++)
                    {
                        if(ex)
                        {
                            break;
                        }
                        lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + block_size*(read_block2[m]-off), SEEK_SET);
                        read(fd, read_block3, block_size);
                        for(int n = 0; n<block_size/4; n++)
                        {
                            if(!read_block3[n])//if(block_c > (inode.i_size / block_size));
                            {
                                ex = 1;
                                break;
                            }
                            block_c++;
                            BM_SET(read_block3[n]-1, bitmap);
                        }
                    }
                }
            }
            unsigned short moderator = EXT2_S_IFREG | EXT2_S_IRUSR;
            lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*(ind-1), SEEK_SET);
            write(fd, &moderator, sizeof(unsigned short));
            //for(int l = 0; l<100; l++)
            //{
            struct ext2_dir_entry dir;
            struct ext2_dir_entry dir2;
            char name2[255];
            struct ext2_inode inode = getInode(11);
            dir_offset = 0;
            lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + (inode.i_block[0]-off)*block_size, SEEK_SET);
            for(int i = 0; i<dir_count-1; i++)
            {
                read(fd, &dir, sizeof(struct ext2_dir_entry));
                dir_offset += dir.rec_len;
                read(fd, &name2, dir.rec_len-8);
            }
            char dname[255];
            read(fd, &dir, sizeof(struct ext2_dir_entry));
            int nlen = dir.name_len;
            if(block_size - dir_offset  - 16 > 16)
            {
                dir.rec_len = 10 + nlen;
                dir_offset += dir.rec_len;
                read(fd, &dname, nlen);
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + (inode.i_block[0]-off)*block_size + (dir_offset % block_size) - dir.rec_len + sizeof(unsigned int), SEEK_SET);
                write(fd, &dir.rec_len, sizeof(unsigned short));
                dir2.inode = ind;
                dir2.rec_len = (block_size-dir_offset) % block_size;
                dir2.name_len = 6;
                dir2.file_type = 1;
                dir2.name[0] = 'f';
                dir2.name[1] = 'i';
                dir2.name[2] = 'l';
                dir2.name[3] = 'e';
                dir2.name[4] = (out_number[ind]/10) + 48;
                dir2.name[5] = (out_number[ind] % 10) + 48;
                lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size*super.s_inodes_per_group + (inode.i_block[0]-off)*block_size + (dir_offset % block_size), SEEK_SET);
                write(fd, &dir2, dir2.rec_len);
                dir_count++;
                BM_SET(ind-1, bitmap2);
            }
            /*else
            {
                lseek(fd, BLOCK_OFFSET(inode.i_block[1]), SEEK_SET);
                read(fd, &dir, sizeof(struct ext2_dir_entry));
                read(fd, &name2, dir.rec_len-8);
                printf("inode: %u, reclen: %hu, namelen: %c, file_type: %c, name: %s\n", dir.inode, dir.rec_len, dir.name_len, dir.file_type, name2);
            }*/
            else
            {
                if(config == 0)
                {
                    lseek(fd, BLOCK_OFFSET(inode.i_block[inc]) + 4, SEEK_SET);
                    int s = 0;
                    write(fd, &s, 2);
                    dir2.inode = ind;
                    dir2.rec_len = block_size - config*16;
                    dir2.name_len = 6;
                    dir2.file_type = '1';
                    dir2.name[0] = 'f';
                    dir2.name[1] = 'i';
                    dir2.name[2] = 'l';
                    dir2.name[3] = 'e';
                    dir2.name[4] = (out_number[ind]/10) + 48;
                    dir2.name[5] = (out_number[ind] % 10) + 48;
                    lseek(fd, BLOCK_OFFSET(inode.i_block[1]), SEEK_SET);
                    write(fd, &dir2, dir2.rec_len);
                    config++;
                    BM_SET(ind-1, bitmap2);
                }
                else
                {
                    lseek(fd, BLOCK_OFFSET(inode.i_block[inc]) + 16*(config-1) + 4, SEEK_SET);
                    int kk = 16;
                    write(fd, &kk, 2);
                    dir2.inode = ind;
                    dir2.rec_len = block_size - config*16;
                    dir2.name_len = 6;
                    dir2.file_type = '1';
                    dir2.name[0] = 'f';
                    dir2.name[1] = 'i';
                    dir2.name[2] = 'l';
                    dir2.name[3] = 'e';
                    dir2.name[4] = (out_number[ind]/10) + 48;
                    dir2.name[5] = (out_number[ind] % 10) + 48;
                    lseek(fd, BLOCK_OFFSET(inode.i_block[inc]) + 16*(config), SEEK_SET);
                    write(fd, &dir2, dir2.rec_len);
                    config++;
                    BM_SET(ind-1, bitmap2);
                    if(dir2.rec_len < 32)
                    {
                        inc++;
                        config = 0;
                    }
                }
            }
        //}
        }
    }
    for (int i = 12; i < super.s_inodes_per_group+1; i++)
    {
        if (out_number[i] > 0)
        {
            printf("file%d%d\n", out_number[i]/10, (out_number[i]%10));
        }
    }
    //free(read_block);
    //free(read_block2);
    //free(read_block3);
}

int main(int argc, char *argv[])
{
    struct ext2_super_block super;
    struct ext2_group_desc group;
    int fd;
    image_name = malloc(sizeof(argv[1]));
    image_name = argv[1];
    if ((fd = open(image_name, O_RDWR)) < 0) {
        perror(IMAGE);
        exit(1);
    }

    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }
    block_size = 1024 << super.s_log_block_size;
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));

    bitmap = malloc(block_size);
    lseek(fd, BLOCK_OFFSET(group.bg_block_bitmap), SEEK_SET);
    read(fd, bitmap, block_size);

    bitmap2 = malloc(block_size);
    lseek(fd, BLOCK_OFFSET(group.bg_inode_bitmap), SEEK_SET);
    read(fd, bitmap2, block_size);
    recover();
    lseek(fd, BLOCK_OFFSET(group.bg_block_bitmap), SEEK_SET);
    write(fd, bitmap, block_size);
    lseek(fd, BLOCK_OFFSET(group.bg_inode_bitmap), SEEK_SET);
    write(fd, bitmap2, block_size);
    free(bitmap);
    free(bitmap2);
    //free(image_name);
    close(fd);
    return 0;
}
