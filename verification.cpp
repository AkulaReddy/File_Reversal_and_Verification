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
void rev_block(char buff[],int block_size);
void file_dir_ver(char* path);
bool verify_rev(int fd,int fd1,long long req_size, int block_size,int flag,int ce);
bool comp(int n,const char* buff,const char* buff1);

int main(int argc,char* argv[]){

    if(argc<5){
        perror("wrong arguments given!!\nFollow: ./a.out <newfilepath> <oldfilepath> <directory> <flag> [<blockSize>|<start> <end>]\n");
        exit(1);
    }

    int flag=atoi(argv[4]); 

    char* newfilepath = argv[1];
    char* oldfilepath = argv[2];
    char* directory = argv[3];
    if (mkdir("Assignment1/", 0700) == -1) {
        if(errno==EEXIST){printf("Directory is created: Yes\n");
        }
        else{
            write(1,"Directory is created: No\n",24);
            printf("Directory not created, cannot proceed.\n");
            return 0;
        }
    }
    int block_size;
    switch (flag){
        case 0: 
                if(argc!=6){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                block_size= atoi(argv[5]);
                if(block_size==0){
                    perror("Give non zero block size");
                }
                flag_0_1(argc, argv, block_size, flag); 
                break;

        case 1: 
                if(argc!=5){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                block_size= 1024;              //block size 1024
                flag_0_1(argc,argv, block_size, flag); 
                break;

        case 2: if(argc!=7){
                    perror("Inconsisstent flag value and arguments");
                    return 0;
                }
                flag_2(argc,argv,flag); 
                break;
        default: perror("Wrong flag value");return 0;
    }
    
    file_dir_ver(newfilepath);
    file_dir_ver(oldfilepath);
    file_dir_ver(directory);

    return 0;

}

void file_dir_ver(char* path){
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat error");
    }

    printf("User has read permission on %s: %s\n",path,((st.st_mode & S_IRUSR) ? "Yes " : "No"));
    printf("User has write permission on %s: %s\n",path,((st.st_mode & S_IWUSR) ? "Yes " : "No"));
    printf("User has execute permission on %s: %s\n",path,((st.st_mode & S_IXUSR) ? "Yes " : "No"));
    printf("Group has read permission on %s: %s\n",path,((st.st_mode & S_IRGRP) ? "Yes " : "No"));
    printf("Group has write permission on %s: %s\n",path,((st.st_mode & S_IWGRP) ? "Yes " : "No"));
    printf("Group has execute permission on %s: %s\n",path,((st.st_mode & S_IXGRP) ? "Yes " : "No"));
    printf("Other has read permission on  %s: %s\n",path,((st.st_mode & S_IROTH) ? "Yes " : "No"));
    printf("Other has write permission on  %s: %s\n",path,((st.st_mode & S_IWOTH) ? "Yes " : "No"));
    printf("Other has execute permission on %s: %s\n",path,((st.st_mode & S_IXOTH) ? "Yes " : "No"));
    
    
}

bool comp(int n, const char *buff, const char *buff1) {
    for (int i = 0; i < n; i++) {
        if (buff[i] != buff1[i]) {
            return false;
        }
    }
    return true;
}

void rev_block(char buff[],int block_size){
        
        for(int i=0;i<(block_size/2);i++){
            char temp;
            temp=buff[i];
            buff[i]=buff[block_size-1-i];
            buff[block_size-1-i]=temp;
        }
        

}

bool verify_rev(int fd,int fd1,long long req_size, int block_size,int flag,int ce){
    char buff[block_size],buff1[block_size];
    bool ver;
    for(long long i=1;i<=(req_size/block_size); i++){
        int n=read(fd,buff,block_size);       // read newfile block wise
        
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        rev_block(buff,n);       // re-rev the rev contents
        int k=read(fd1,buff1,block_size);
        
        if (k<0){
            perror("Error writing in the file");
            exit(1);
        }

        if (flag==1|| flag==2){
        lseek(fd,-(2*block_size),SEEK_CUR);
        }
        
        ver=comp(n,buff,buff1);
        if(!ver){return false;}
    }
    if ((req_size%block_size)!=0){
        if (flag==1||ce==1){            //if flag 1 (or) flag-2: part-1 ie ce==1
            lseek(fd,0,SEEK_SET);
        }
        else if (ce==2){                //if flag-2: part-2 ie ce==2
        
            lseek(fd,-req_size,SEEK_END);
        }
        int n=read(fd,buff,(req_size%block_size));
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        rev_block(buff,n);
        int k=read(fd1,buff1,n);
        if (k<0){
            perror("Error writing in the file");
        }
        ver=comp(n,buff,buff1);
        if(!ver){return false;}
    }
    return true;
}


void flag_0_1(int argc,char* argv[4],int block_size,int flag){
    char* newfilepath = argv[1];
    char* oldfilepath = argv[2];
    int fd, fd1;
    fd=open(newfilepath,O_RDONLY);
    if(fd<1){
        perror("Error openning newfile, give correct path\n");
        exit(1);
    }
    fd1=open(oldfilepath,O_RDONLY);
    if(fd1<1){
        perror("Error openning oldfile, give correct path\n");
        exit(1);
    }

    long long file_size_new = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    long long file_size_old=lseek(fd1,0,SEEK_END);
    lseek(fd1,0,SEEK_SET);
    printf("Both Files Sizes are Same: %s\n",((file_size_new==file_size_old)? "Yes":"No"));
    if(file_size_new!=file_size_old){
        printf("File sizes do not match, Cannot verify further.\n");
        exit(1);
    }

    //File content Processing
    lseek(fd1,0,SEEK_SET);
    if (flag==0){
    lseek(fd,0,SEEK_SET);
    }
    else{
        lseek(fd,-block_size,SEEK_END);
    }

    bool ver = verify_rev(fd,fd1,file_size_new,block_size,flag,flag);
    printf("Whether file contents are correctly processed: %s\n",((ver)? "Yes":"No"));
    
}



void flag_2(int argc,char* argv[],int flag){
    int block_size=1024;
    char* newfilepath = argv[1];
    char* oldfilepath = argv[2];
    int fd, fd1;
    long long start_ind=atoi(argv[5]);
    long long end_ind=atoi(argv[6]);
    if(start_ind>end_ind){
        perror("start index should be less than end index!!");
        exit(1);
    }
    //rep code
    fd=open(newfilepath,O_RDONLY);
    if(fd<1){
        perror("Error openning newfile, give correct path\n");
        exit(1);
    }
    fd1=open(oldfilepath,O_RDONLY);
    if(fd1<1){
        perror("Error openning oldfile, give correct path\n");
        exit(1);
    }
    long long file_size_new = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    long long file_size_old=lseek(fd1,0,SEEK_END);
    lseek(fd1,0,SEEK_SET);
    printf("Both Files Sizes are Same: %s\n",((file_size_new==file_size_old)? "Yes":"No"));
    if(file_size_new!=file_size_old){
        perror("File sizes do not match, Cannot verify further.");
        exit(1);
    }

//File processing

    
    char buff[block_size],buff1[block_size];
    
    long long req_size= start_ind;
    (req_size%block_size)? lseek(fd,0,SEEK_SET):lseek(fd,-block_size,SEEK_CUR);
    //Rep code
    
    verify_rev(fd,fd1,req_size,block_size,flag,1);
    
  
    req_size=end_ind-start_ind+1;

    bool ver;

    lseek(fd,start_ind,SEEK_SET);
    lseek(fd1,start_ind,SEEK_SET);

    for(long long i=1;i<=(req_size/block_size); i++){
        int n=read(fd,buff,block_size);
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        int k=read(fd1,buff1,block_size);
        if (k<0){
            perror("Error writing the file");
            exit(1);
        }
        ver=comp(n,buff,buff1);

    }
    if ((req_size%block_size)!=0){
        int n=read(fd,buff,(req_size%block_size));
        if (n<0){
            perror("Error reading the file");
            exit(1);
        }
        int k=read(fd1,buff1,(req_size%block_size));
        if (k<0){
            perror("Error writing the file");
            exit(1);
        }
        ver=comp(n,buff,buff1);

    }
    
    req_size=file_size_new-end_ind-1;
    lseek(fd,-block_size,SEEK_END);
    lseek(fd1,end_ind+1,SEEK_SET);
    //Rep code
    ver=verify_rev(fd,fd1,req_size,block_size,flag,2);
    printf("Whether file contents are correctly processed: %s\n",((ver)? "Yes":"No"));
    

}
