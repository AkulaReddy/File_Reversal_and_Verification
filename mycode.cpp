#include <unistd.h>   
#include <fcntl.h>   
#include <sys/types.h> 
#include <cstdlib>
#include <cstdio>           
#include <sys/stat.h>   
#include <cstring>
#include <cstdio>
#include <stdio.h>
#include <errno.h>


void flag_0_1(int argc,char* argv[4],int block_size,int flag);
void flag_2(int argc,char* argv[],int flag);
void rev_block(int fd1, char buff[],int block_size);
double rev_file(int fd,int fd1,long long req_size, int block_size,int flag,int ce, long long file_size);

int main(int argc,char* argv[]){
    if(argc<3){
        perror("wrong arguments given!!\nFollow: ./a.out <input file name> <flag> [additional arguments]\n");
        exit(1);
    }
    int flag=atoi(argv[2]); 

    int block_size;
    switch (flag){
        case 0: 
                if(argc!=4){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                block_size= atoi(argv[3]);
                if(block_size==0){
                    perror("Give non zero block size");
                }
                flag_0_1(argc, argv, block_size, flag); 
                break;

        case 1: if(argc!=3){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                block_size= 1024;
                flag_0_1(argc,argv, block_size, flag); 
                break;

        case 2: if(argc!=5){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                flag_2(argc,argv,flag); 
                break;
        default: perror("Wrong flag value");
    }
    return 0;

}

double rev_file(int fd,int fd1, long long req_size, int block_size, int flag, int ce, long long file_size){
    char buff[block_size];
    double percentage,wten=0;
    for(long long i=1;i<=(req_size/block_size); i++){
        int n=read(fd,buff,block_size);
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        rev_block(fd1,buff,block_size);
        int k=write(fd1,buff,block_size);
        if (k<0){
            perror("Error writing in the file");
            exit(1);
        }
        wten+=k;
        if(flag==0||flag==1||ce==2){
        percentage=((double)wten*100.0/(double)file_size);//print percentage of file written
        printf("\rPercentage of file written: %.2f%%", percentage);
        int bars = (int)(percentage / 2);
        printf(" [");
        for (int j = 0; j < 50; j++) {      //To print progress bar
            if (j < bars) printf("*");
            else printf(".");
        }
        printf("] ");
        fflush(stdout);}
        if (flag==1|| flag==2){
        lseek(fd,-(2*block_size),SEEK_CUR);
        }
    }
    if ((req_size%block_size)!=0){
        if (flag==1||ce==1){
            lseek(fd,0,SEEK_SET);
        }
        else if (ce==2){
        //lseek(fd,0,SEEK_SET);//flag-2 part-2 
            lseek(fd,-req_size,SEEK_END);
        }
        int n=read(fd,buff,(req_size%block_size));
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        rev_block(fd1,buff,n);
        int k=write(fd1,buff,n);
        if (k<0){
            perror("Error writing in the file");
            exit(1);
        }
        wten+=k;
        if(flag==0||flag==1||ce==2){
        percentage=((double)wten*100.0/(double)file_size);//print percentage
        printf("\rPercentage of file written: %.2f%%", percentage);
        int bars = (int)(percentage / 2);
        printf(" [");
        for (int j = 0; j < 50; j++) {      //To print progress bar
            if (j < bars) printf("*");
            else printf(".");
        }
        printf("] ");
        }

    }

    return wten;
    //return 0;

}
void rev_block(int fd1, char buff[],int block_size){
       
        for(int i=0;i<(block_size/2);i++){
            char temp;
            temp=buff[i];
            buff[i]=buff[block_size-1-i];
            buff[block_size-1-i]=temp;
        }
        

}

void flag_0_1(int argc,char* argv[],int block_size, int flag ){

    const char* f_name=argv[1];
    int fd=open(f_name,O_RDONLY);
    if (fd<0){
        perror("Could Not Open File ");//return 0
        exit(1);
    }
    char fout_name[1024];
    if (mkdir("Assignment1/", 0700) == -1) {if(errno!=EEXIST){perror("Error creating Directory");}}
    if (flag==0){
        strcpy(fout_name,"Assignment1/0_");
    }
    else if(flag==1){
        strcpy(fout_name,"Assignment1/1_");
    }

    int fd1=open(strcat(fout_name,f_name),O_CREAT|O_RDWR|O_TRUNC,0600);
   
    if (fd1<0){
        perror("Could Not Create File ");
        exit(1);
    }
    char buff[block_size];
    long long file_size=lseek(fd,0,SEEK_END);
    if (flag==0){
    lseek(fd,0,SEEK_SET);}
    else{
        lseek(fd,-block_size,SEEK_END);
    }
    //Rep code
    rev_file(fd,fd1,file_size,block_size,flag,flag,file_size);
    

}

void flag_2(int argc,char* argv[],int flag){
    double written=0, percentage,val;
    int block_size=1024;
    const char* f_name=argv[1];//if st>end return
    long long start_ind=atoi(argv[3]);
    long long end_ind=atoi(argv[4]);
    if(start_ind>end_ind){
        perror("start index should be less than end index!!");
        exit(1);
    }
    int fd=open(f_name,O_RDONLY);
    if (fd<0){
        perror("Could Not Open File ");
        exit(1);
    }
    long long file_size=lseek(fd,0,SEEK_END);
    //lseek(fd,0,SEEK_SET);
    int t=lseek(fd,start_ind,SEEK_SET);
    if(t<0){
        perror("Unable to seek given Index, check file size\n");
        exit(1);
    }
    char fout_name[1024];
    if (mkdir("Assignment1/", 0700) == -1) {if(errno!=EEXIST){perror("Error creating Directory");}}
    strcpy(fout_name,"Assignment1/2_");
    int fd1=open(strcat(fout_name,f_name),O_CREAT|O_RDWR|O_TRUNC,0600);
   
    if (fd1<0){
        perror("Could Not Create File ");
        exit(1);
    }
    char buff[block_size];
    //long long file_size=lseek(fd,0,SEEK_END);
    long long req_size= start_ind;
    (req_size%block_size)? lseek(fd,0,SEEK_SET):lseek(fd,-block_size,SEEK_CUR);
    //Rep code
    
    val=rev_file(fd,fd1,req_size,block_size,flag,1, file_size);
    
    written+=val;
  
    req_size=end_ind-start_ind+1;

    for(long long i=1;i<=(req_size/block_size); i++){
        int n=read(fd,buff,block_size);
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        int k=write(fd1,buff,block_size);
        if (k<0){
            perror("Error writing the file");
            exit(1);
        }
        written+=k;
        percentage=((double)written*100.0/(double)file_size);//print percentage
        printf("\rPercentage of file written: %.2f%%", percentage);
        int bars = (int)(percentage / 2);
        printf(" [");
        for (int j = 0; j < 50; j++) {      //To print progress bar
            if (j < bars) printf("*");
            else printf(".");
        }
        printf("] ");
        fflush(stdout);
    }
    if ((req_size%block_size)!=0){
        int n=read(fd,buff,(req_size%block_size));
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        int k=write(fd1,buff,(req_size%block_size));
        if (k<0){
            perror("Error writing the file");
            exit(1);
        }
        written+=k;
        percentage=((double)written*100.0/(double)file_size);//print percentage
        printf("\rPercentage of file written: %.2f%%", percentage);
        int bars = (int)(percentage / 2);
        printf(" [");
        for (int j = 0; j < 50; j++) {      //To print progress bar
            if (j < bars) printf("*");
            else printf(".");
        }
        printf("] ");
        fflush(stdout);

    }

    req_size=file_size-end_ind-1;
    lseek(fd,-block_size,SEEK_END);
    //Rep code
    written+=rev_file(fd,fd1,req_size,block_size,flag,2, file_size);
    percentage=((double)written*100.0/(double)file_size);//print percentage
    printf("\rPercentage of file written: %.2f%%", percentage);
    int bars = (int)(percentage / 2);
    printf(" [");
    for (int j = 0; j < 50; j++) {      //To print progress bar
        if (j < bars) printf("*");
        else printf(".");
    }
    printf("] ");
    

}
