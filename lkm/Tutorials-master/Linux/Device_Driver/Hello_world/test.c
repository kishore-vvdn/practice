#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define W_VALUE _IOW('A','A',int32_t*)
#define R_VALUE _IOR('A','B',int32_t*)

struct data
{
         int i;
         char c;
         float f;

}dat;

int main()
{
	int f,i;
	char op;
	f=open("/dev/sampledd",O_RDWR);
	if(f < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
	while(1){
                
		printf("****Please Enter the Option******\n");
                printf("        1. Write               \n");
                printf("        2. Read                 \n");
                printf("        3. Exit                 \n");
                printf("*********************************\n");
                scanf(" %c", &op);
                printf("Your Option = %c\n",op);
                

		switch(op) {
                        case '1':
                                printf("Enter the data to write into driver \n");
       			        printf("Enter float value\n");
		                scanf(" %f", &dat.f);
               			printf("Enter integer value\n");
              			scanf(" %d", &dat.i);
           			printf("Enter character value\n");
				scanf(" %c", &dat.c);
                                printf("Data Writing ...");
                               // write(f, &dat, 8);
				ioctl(f,W_VALUE,&dat);
                                printf("Done!\n");
                                break;
                        case '2':
                                printf("Data Reading ...");
                               // read(f, &dat, 8);
				ioctl(f,R_VALUE,&dat);
                                printf("Done!\n\n");
                                printf("Entered Data\n  %f  %d  %c\n",dat.f,dat.i,dat.c);
                                break;
                        case '3':
                                close(f);
                                exit(1);
                                break;
                        default:
                                printf("Enter Valid option = %c\n",op);
                                break;
                }
        }
        close(f);

}
