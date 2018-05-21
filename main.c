#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <math.h>
#define FALSE 0
#define TRUE 1

char ch[]="MWNB8527|i\";+-. ";

int tod(BYTE);
int toturn(int,RGBQUAD*,FILE*);
int main(int argc, char *argv[]) {
	char* str;
	int i=0;
	str=(char*)malloc(sizeof(char)*1);
	while('\n'!=(str[i]=getchar())) {
		i++;
		str=(char*)realloc(str,sizeof(char)*(i+1));
	}
	str[i]='\0';
	printf("%s\n",str);
    FILE* fpwrite=fopen("write.txt","w");
    FILE *fp=fopen(str,"rb");
    free(str);
    if (fp==NULL) {
        printf("open error!");
        return FALSE;
    } else {
        int bmpWidth;
        int bmpHeight;
        int biBitCount;
        RGBQUAD* pColorTable;
        char* pBmpdata;
        fseek(fp,sizeof(BITMAPFILEHEADER),0);//跳过文件头
        BITMAPINFOHEADER head;
        fread(&head,sizeof(BITMAPINFOHEADER),1,fp);//读取信息头
        bmpWidth=head.biWidth;
        bmpHeight=head.biHeight;
        biBitCount=head.biBitCount;
        if (biBitCount==8) {//判断位深
            int length;
            if (bmpWidth%4!=0) {
                length=(bmpWidth/4+1)*4;
            } else {
                length=bmpWidth;
            }
            pColorTable=(RGBQUAD*)malloc(256*sizeof(RGBQUAD));
            fread(pColorTable,256*sizeof(RGBQUAD),1,fp);
            pBmpdata=(BYTE*)malloc(length*bmpHeight);
            fread(pBmpdata,length*bmpHeight,1,fp);
            fclose(fp);
            for (int i=bmpHeight-1; i>=0; i--) {
                for (int j=0; j<bmpWidth; j++) {
                    toturn(tod(*(pBmpdata+j+i*length)),pColorTable,fpwrite);
                }
                fprintf(fpwrite,"\n");
            }
            fclose(fpwrite);
            free(pColorTable);
            free(pBmpdata);
            return TRUE;
        }
		if (biBitCount==1) {
            fseek(fp,8,1);
            int length;
            if ((bmpWidth/8)%4!=0) {
                length=((bmpWidth/8)/4+1)*4;
            } else {
                length=(bmpWidth/8);
            }
            int bmplength=(bmpWidth-1)/8+1;
            char *pBmpBuf;
            pBmpBuf=(char*)malloc(length*bmpHeight);
            fread(pBmpBuf,1,length*bmpHeight,fp);
            fclose(fp);
            for (int i=bmpHeight-1; i>=0; i--) {
                for (int j=0; j<bmplength; j++) {
                    for (int k=0; k<8; k++) {
                        if ((j+1)*8+k+1>bmpWidth) {
                            break;
                        } else {
                            if((int)(((*(pBmpBuf+i*length+j))&(0x1<<(7-k)))>>(7-k))==1) {
                                fprintf(fpwrite,"M");
                            } else {
                                fprintf(fpwrite," ");
                            }
                        }
                    }

                }
                fprintf(fpwrite,"\n");
            }
        } else  {
        fprintf(fpwrite,"type error!");
        return FALSE;
    }
}
}
int tod(BYTE n) {
    int sum=0;
    for (int k=0; k<8; k++) {
        sum+=(int)pow(2,k)*(int)(n&(0x1<<(7-k)))>>(7-k);
    }
    return sum;
}
int toturn(int n,RGBQUAD* p,FILE* fpwrite) {
    int gray=tod((*(p+n)).rgbGreen);
    fprintf(fpwrite,"%c",*(ch+gray*gray/4335));
}
