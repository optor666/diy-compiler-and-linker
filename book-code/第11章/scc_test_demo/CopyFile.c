/* CopyFile.cԴ�ļ� */
/*********************************************************** 
 * ����:	main������
 **********************************************************/
 struct _iobuf 
 {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
};

int main(int argc, char **argv, char **env)
{
	int ch;
	struct _iobuf  *srcfile;
	struct _iobuf  *destfile;
	char srcfname[300];    
	char destfname[300]; 
	int i=0;
	char buf[2000]; 
	int NULL = 0;
  int EOF  = -1; 
  int SEEK_SET = 0;

		printf("Դ�ļ�����");
	scanf("%s",srcfname);

	printf("Ŀ���ļ�����");
	scanf("%s",destfname);

	srcfile=fopen(srcfname,"rb");
	if(srcfile==NULL)
	{
		printf("�����ļ�δ�ҵ���\n");
		printf("�����������...");
		getchar();
		exit(1); 
	}

	destfile = fopen(destfname, "wb");
	if(destfile==NULL) 
	{
		printf("д���ļ�δ�ҵ���\n");
		printf("�����������...");
		getchar();
		fclose(srcfile);
		exit(1);
	}

	for(;(ch=getc(srcfile))!=EOF;)
	{
	  putchar(ch);
		fwrite(&ch,1,1,destfile);
	}

	fclose(srcfile);
	fclose(destfile);
	
	printf("�����������...");
	getchar();
	return 0;
}

/*********************************************************** 
 * ����:	��ں���
 **********************************************************/
void  _entry()
{
	int argc; char **argv; char **env; int ret;
	int start_info = 0;

	__getmainargs(&argc, &argv, &env, 0, &start_info);
	ret = main(argc, argv, env);
	exit(ret);
}