#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ext2.h"
#include <time.h>
#include <errno.h>
#include <string.h>

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024
#define IMAGE "image.img"

typedef unsigned char bmap;
#define __NBITS (8 * (int) sizeof (bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap) 1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT (d)] |= __BMMASK (d)))
#define BM_CLR(d, set) ((set[__BMELT (d)] &= ~__BMMASK (d)))
#define BM_ISSET(d, set) ((set[__BMELT (d)] & __BMMASK (d)) != 0)

unsigned int block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

struct ext2_ddir_entry {
        unsigned int   inode;     /* Inode number */
        unsigned short rec_len;   /* Directory entry length */
        unsigned char  name_len;  /* Name length */
        unsigned char  file_type;
        char         * name;    /* File name, up to EXT2_NAME_LEN */
};

int fd;
struct ext2_super_block super;
struct ext2_group_desc group;
char cebrail[1025];
bmap * blockbitmap;
bmap * inodebitmap;
struct ext2_inode lfnode;

unsigned int blockno[1024] = { 0 };

void initBlock(char * imagename){
    if ((fd = open(imagename, O_RDWR)) < 0) {
        perror(imagename);
        exit(1);
    }

    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }
    block_size = 1024 << super.s_log_block_size;
}

void superBlockInfo(){
    printf("Reading from image file " IMAGE ":\n"
           "Blocks count            : %u\n"
           "First non-reserved inode: %u\n",
           super.s_blocks_count,
           super.s_first_ino);
}

void initGroup(){
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));
}

void getInode(int number,struct ext2_inode * inode){
    lseek(fd,BLOCK_OFFSET(group.bg_inode_table)+sizeof(struct ext2_inode)*(number-1),SEEK_SET);
    read(fd, inode, sizeof(struct ext2_inode));
    return;
}

int isFolder(struct ext2_inode * inode){
    if (inode->i_mode >= EXT2_S_IFDIR && inode->i_mode <= EXT2_S_IFBLK){
        return 1;
    } return 0;
}

void inodeInfo(struct ext2_inode * inode){
    printf("Links Count: %u\t",inode->i_links_count);
    printf("Size     : %u bytes\t"
           "Blocks   : %u\t"
           "Mode     : %u\t",
           inode->i_size,
           inode->i_blocks,
           inode->i_mode);
    for(int i=0; i < 20 && inode->i_block[i] != 0; i++){
        if (i < 12)         // direct blocks
            printf("Block %2u : %u\t", i, inode->i_block[i]);
        else if (i == 12)     // single indirect block
            printf("Single   : %u\t", inode->i_block[i]);
        else if (i == 13)    // double indirect block
            printf("Double   : %u\t", inode->i_block[i]);
        else if (i == 14)    // triple indirect block
            printf("Triple   : %u\t", inode->i_block[i]);
    } printf("\t");
    printf("Deletion time : %s", asctime(gmtime((const long int *)&inode->i_dtime)));
}

void getDirInfo(int dirNo,struct ext2_inode * inode,int block,struct ext2_dir_entry * dir) {
    lseek(fd,BLOCK_OFFSET(inode->i_block[block]),SEEK_SET);
    
    for(int i = 0; i<dirNo; i++){
        read(fd,dir,sizeof(struct ext2_dir_entry));
        read(fd,dir->name,dir->rec_len-8);
    }
}

void printDirInfo(struct ext2_dir_entry * dir) {
    printf("Inode No : %u \t",dir->inode );
    printf("Entry Length : %hu \t",dir->rec_len );
    printf("Name Length : %d \t",dir->name_len );
    printf("File Type : %d \t",dir->file_type );
    printf("Name : '");
    for(int k = 0; k<dir->name_len; k++){
        printf("%c",dir->name[k] );
    } printf("'--\n");
}

void getAllDirInfo(struct ext2_inode * inode){
    struct ext2_dir_entry dir;
    unsigned int temp = 0;
    printf("Number of Blocks in dir : %d\n", inode->i_blocks);
    for(int i = 0; inode->i_block[i] != 0 ; i++){
        lseek(fd,BLOCK_OFFSET(inode->i_block[i]),SEEK_SET);
        for(int k = 0; k<1000;){
            read(fd,&dir,sizeof(struct ext2_dir_entry));
            //rintf("Reclen : %d\n", dir.rec_len );
            //printf("%d\n",dir.name_len );
            read(fd,dir.name,dir.name_len);
            //lseek(fd,dir.name_len,SEEK_CUR);
            if (dir.rec_len - dir.name_len - 8 > 0){
                lseek(fd,dir.rec_len - dir.name_len - 8,SEEK_CUR);
            }
            printDirInfo(&dir);
            if (dir.rec_len - dir.name_len - 8 > EXT2_NAME_LEN) {
                return;
            }

            k+=dir.rec_len;
        }
    }
}

void uBlockMap(){
    if (!blockbitmap) blockbitmap = malloc(block_size);

    lseek(fd, BLOCK_OFFSET(group.bg_block_bitmap), SEEK_SET);
    read(fd, blockbitmap, block_size);
}

void uiNodeMap(){
    if (!inodebitmap) inodebitmap = malloc(block_size);

    lseek(fd, BLOCK_OFFSET(group.bg_inode_bitmap), SEEK_SET);
    read(fd, inodebitmap, block_size);

}

void printBMAP() {
    //uBlockMap();
    int fr=0; int nfr=0;
    printf("Free block bitmap:\n");
    for (int i = 0; i < /*super.s_blocks_count*/600; i++){
        if (BM_ISSET(i,blockbitmap)){
            printf("+");    // in use
            nfr++;
        }
        else{
            printf("-");    // empty
            fr++;
        }
    }
    printf("\n");
    printf("Free blocks count       : %u\n"
           "Non-Free block count    : %u\n"
           , fr, nfr);
    printf("\n");
}

void printInodeMap(){
    
    //uiNodeMap();
    int fr = 0;
    int nfr = 0;
    printf("Inode Bitmap:\n");
    //BM_SET(5,bitmap); //IF THAT BLOCK IS USING BY AN INODE OR NOT
    for (int i = 0; i < super.s_inodes_count; i++) {
        if (BM_ISSET(i,inodebitmap)) {
            printf("+");    // in use
            nfr++;
        }
        else {
            printf("-");    // empty
            fr++;
        }
    }
    printf("\n");
    printf("Free inode count       : %u\n"
           "Non-Free inode count    : %u\n"
           , fr, nfr);
    printf("\n");
}

void takepointers(unsigned int datablock, unsigned int * indPointer) {
    lseek(fd,BLOCK_OFFSET(datablock),SEEK_SET);
    read(fd,indPointer,1024);
    return;
}

void printData(unsigned int datablock) {
    struct ext2_dir_entry;
    int howmuch;
    lseek(fd,BLOCK_OFFSET(datablock),SEEK_SET);
    howmuch = read(fd,cebrail,1024);
    for(int k = 0; k<1024 ; k++,howmuch--) {
        printf("%c",cebrail[k]);
    }
}

void readData(struct ext2_inode * inode) {
    
    unsigned int * spointer;
    unsigned int * dpointer;
    unsigned int * tpointer;
    unsigned int size = inode->i_size / 1024 + 1;

    for (int i = 0;i<15 && inode->i_block[i]; i++) {

        if (i<12) { 
            printData(inode->i_block[i]);
            size--;
        } else if (i == 12) {
            takepointers(inode->i_block[i],spointer);
            for(int k = 0; spointer[k] && size; k++){
                //printf("%d\n",spointer[k] );
                printData(spointer[k]);   
                size--;
            }
        } else if (i == 13) {
            takepointers(inode->i_block[i],dpointer);
            for(int d = 0; d < 256 && dpointer[d] != 0; d++){
                takepointers(dpointer[d],spointer);
                for(int s = 0; s<256 && spointer[s] != 0; s++) {
                    printData(spointer[s]);
                }
            }
            
        } else if (i == 14) {
            takepointers(inode->i_block[i],tpointer);
            for(int t = 0; t < 256 && tpointer[t] ; t++){
                takepointers(tpointer[t],dpointer);
                for(int d = 0; d < 256 && dpointer[d] ; d++){
                    takepointers(dpointer[d],spointer);
                    for(int s = 0; s<256 && spointer[s] ; s++) {
                        printData(spointer[s]);
                    }
                }
            }
        }
    }
}

int isDeleted(struct ext2_inode * inode,int inodeno) {
    if (inode->i_dtime && !BM_ISSET(inodeno-1,inodebitmap)) {
        return 1;
    } else {
        return 0;
    }
}

int sortdeleted(int * array,int len) {
    int sortemp = 0;
    struct ext2_inode inode[len];

    for (int i = 0; i< len; i++) {
        getInode(array[i],&inode[i]);
    }
    for (int i = 0; i< len; i++) {
        for (int k = 0; k < i; k++) {
            if(inode[k].i_dtime < inode[i].i_dtime) {
                sortemp = array[i];
                array[i] = array[k];
                array[k] = sortemp;
            }
        }
    }
    /*for (int i = 0; i< len; i++) {
    	printf("%d ",array[i] );
    } printf("\n");*/
}

int findDeletediNodes(int * array) {
    struct ext2_inode inode;
    int filecounter = 1;
    for (int i = 1; i < super.s_inodes_per_group+1; i++) {
        getInode(i, &inode);
        if ( isDeleted(&inode,i) ) {
            printf("file%d%d\n",filecounter/10,filecounter%10);
            array[filecounter-1] = i;
            filecounter++;
        }
    }
    return filecounter-1;
}


void printDeletediNodes(int * array,int fcounter){
    struct ext2_inode inode;
    int curNode = 0;
    for(int i = 1; i < fcounter+1; i++) {
        curNode = array[i-1];
        getInode(curNode,&inode);
        if(inode.i_blocks == 0) break;
        //printf("%d\n",BM_ISSET(curNode,inodebitmap) );
        if(!isDeleted(&inode,curNode) ) {
            printf("file%d%d %d %d\n",i/10,i%10,inode.i_dtime,inode.i_blocks / 2 );
            //inodeInfo(&inode);
        }
    }
}

void recFilter(int * array) {

    unsigned int filec ;
    struct ext2_inode anode;
    uBlockMap();
    int ctr = 0;
    for (int i=0; array[i] > 0; i++) {
        getInode(array[i],&anode);
        for (int k = 0; anode.i_block[k]; k++) {
            filec = anode.i_block[k];
            if (BM_ISSET(filec,blockbitmap)) {
                blockno[ctr] = filec;
                ctr++;

                //array[i] = -3;
            }
        }
    }
    printf("###");
    //for(int c=0; c < ctr; c++) printf("%d, ",blockno[c] );
    printf("\n");
}

void updateiNode(int iNodeNo) {
    struct ext2_inode inode;
    getInode(iNodeNo,&inode);
    //inodeInfo(&inode);
    unsigned int imode = EXT2_S_IFREG | EXT2_S_IRUSR;
    //inode.i_dtime = 0;
    inode.i_links_count = 1;
    int dpointer[256];
    int spointer[256];

    int filec = 0;
    BM_SET(iNodeNo-1,inodebitmap);

    for (int k = 0; inode.i_block[k]; k++) {
    	if( k < 12 ) {
	        filec = inode.i_block[k];
	        BM_SET(filec-1,blockbitmap);    		
    	} else if ( k == 12) {
    		filec = inode.i_block[k];
    		BM_SET(filec-1,blockbitmap);
		    takepointers(inode.i_block[k],spointer);
		    for(int s = 0; s<256 && spointer[s] != 0; s++) {
		        filec = spointer[s];
		        BM_SET(filec-1,blockbitmap);  
		    }
    	} else if ( k == 13) {
    		takepointers(inode.i_block[k],dpointer);
    		filec = inode.i_block[k];
    		BM_SET(filec-1,blockbitmap);
    		for(int d = 0; d < 256 && dpointer[d] != 0; d++){
    		    takepointers(dpointer[d],spointer);
    			filec = dpointer[d];
    		    BM_SET(filec-1,blockbitmap);
    		    for(int s = 0; s<256 && spointer[s] != 0; s++) {
    		        filec = spointer[s];
    		        BM_SET(filec-1,blockbitmap); 
    		    }
    		}
    	}
    }

    lseek(fd,BLOCK_OFFSET(group.bg_inode_table)+sizeof(struct ext2_inode)*iNodeNo,SEEK_SET);
    
    write(fd, &imode, sizeof(unsigned short));

    getInode(iNodeNo,&inode);
    //inodeInfo(&inode);

    lseek(fd, BLOCK_OFFSET(group.bg_block_bitmap), SEEK_SET);
    write(fd,blockbitmap,block_size);

    lseek(fd, BLOCK_OFFSET(group.bg_inode_bitmap), SEEK_SET);
    write(fd,inodebitmap,block_size);
    
}

void writeToLF(struct ext2_dir_entry ** dirlist,int lenlist){
    struct ext2_dir_entry dir;
    struct ext2_dir_entry twodot;
    int t = 0;
    int if1024 = 24;
    unsigned int reclen1= 12;
    int blockno = lfnode.i_block[t];
    
    lseek(fd, BLOCK_OFFSET(blockno)+16, SEEK_SET);
   

    if (write(fd,&reclen1,sizeof(unsigned short)) == -1 ){
        ;
    }

    lseek(fd,BLOCK_OFFSET(blockno)+24,SEEK_SET);

    for(int k = 0; k < lenlist; k++) {
        
        if(if1024 >= block_size ) {
            t++;
            blockno = lfnode.i_block[t];
            lseek(fd,BLOCK_OFFSET(blockno),SEEK_SET);
            if1024 = 0;
        }

        if(k == lenlist-1) {
            dirlist[k]->rec_len = block_size-if1024;
        }

        if1024 += 20;
        printDirInfo(dirlist[k]);
        if (write(fd,(dirlist[k]),20) == -1) {
            printf("Yazmadi %s\n", strerror(errno));
        }

    }
    //getAllDirInfo(&lfnode);
}

void recoverFiles(int * array,int toBeRec, int * secArray) {
    size_t sizeofs = sizeof(unsigned int) + sizeof(unsigned short) + sizeof(unsigned char)*14;
    struct ext2_inode anode;
    int lostt = 0;
    int lblockt = 0;
    int filec = 0;
    int ctr = 0;
    struct ext2_dir_entry * lostandfound;
    struct ext2_dir_entry * lfarray[100];
    int lafCtr = 1;
    int thisCtr = 1;
    int curNode = 0;
    unsigned int spointer[256];
    unsigned int dpointer[256];

    //lseek(fd,BLOCK_OFFSET(lfnode.i_block[lblockt]),SEEK_SET);

    uBlockMap();
    for (int i=0; i< toBeRec; i++) {
        getInode(array[i],&anode);

        if(anode.i_size == 0 ){
        	continue;
        }
        if(anode.i_dtime == 0 ) {
        	continue;
        }

        int nRec = 1;
        for (int t = 0; anode.i_block[t]; t++) {

            if(t < 12){
                filec = anode.i_block[t]-1;
                if (BM_ISSET(filec,blockbitmap) ) { 
                    nRec = 0;
                    break;
                }
            } else if (t == 12){
                takepointers(anode.i_block[t],spointer);
                for(int s = 0; s<256 && spointer[s] ; s++){
                    filec = spointer[s]-1;
                    if (BM_ISSET(filec,blockbitmap) ) {      
                        nRec = 0;
                        break;
                    }
                }
            } else if (t == 13){
                takepointers(anode.i_block[t],dpointer);
                for(int d = 0; d < 256 && dpointer[d] != 0; d++){
                    takepointers(dpointer[d],spointer);
                    for(int s = 0; s<256 && spointer[s] != 0; s++) {
                        filec = spointer[s]-1;
                        if (BM_ISSET(filec,blockbitmap) ) {     
                            nRec = 0;
                            break;
                        }
                    }
                }
            }
        }

        if (nRec){ 
            updateiNode(array[i]);
            lostandfound = malloc(sizeofs);
            
            char filename[10];

            lostandfound->inode = array[i];
            lostandfound->name_len = 12;  
            
            if(isFolder(&anode)) lostandfound->file_type = 2;
            else                 lostandfound->file_type = 1;
            
            //printf("file%d%d %d %d\n",lafCtr/10,lafCtr%10,anode.i_dtime,anode.i_blocks / 2 );
			snprintf(lostandfound->name,12, "file%d%d", lafCtr / 10, lafCtr % 10  );
            lostandfound->rec_len = sizeofs;
            lfarray[thisCtr-1] = lostandfound;
            thisCtr++;
            lostt += sizeofs;
        }
        lafCtr++;
    }
    /*for(int k = 0; k < lafCtr ; k++) {
         printDirInfo(lfarray[k]);
    }*/

    writeToLF(lfarray,thisCtr-1);
}

int main(int argc, char **argv) {
    if(argc == 1){
        return -1;
    } else if (argc == 2) {
        initBlock(argv[1]);
    }
    int toBeRec = 0;

    //superBlockInfo();
    initGroup();
    
    getInode(11,&lfnode);

    struct ext2_inode inode;
    int d_inodes[100];
    int d_inodes2[100];
    for(int i = 0; i < 100; i++){
       d_inodes[i] = 0;  
    }
    uBlockMap();
    uiNodeMap();
    //printInodeMap();
    toBeRec = findDeletediNodes(d_inodes);
    for(int i = 0; i<100 ; i++){
    	d_inodes2[i] = d_inodes[i];
    }
    sortdeleted(d_inodes2,toBeRec);
    recFilter(d_inodes);
    recoverFiles(d_inodes2,toBeRec,d_inodes);
    printDeletediNodes(d_inodes,toBeRec);

    //printInodeMap();
    getInode(11,&lfnode);
    getAllDirInfo(&lfnode);
    free(blockbitmap);
    free(inodebitmap);
    close(fd);
    return 0;
}


/*void getAllDirInfo2(struct ext2_inode * inode){
    struct ext2_ddir_entry * dir;
    unsigned int temp = 0;
    int k = 0;
    printf("Number of Blocks in dir : %d\n", inode->i_blocks);
    for(int i = 0; inode->i_block[i] != 0 ; i++){
        lseek(fd,BLOCK_OFFSET(inode->i_block[i]),SEEK_SET);
        read(fd,dir,EXT2_BLOCK_SIZE);
        while((dir+k)){
            printDirInfo((dir+k));
            k++;
        }
    }
}*/