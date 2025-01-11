// ���Լ�����д��������������������Դ����

#include <stdio.h>
#include <windows.h>

/*******************************dynstring.h begin****************************/
/*��̬�ַ�������*/
typedef struct DynString 
{
    int count;		// �ַ�������  
	int capacity;	// �������ַ����Ļ���������
    char *data;		// ָ���ַ�����ָ��
} DynString;

void dynstring_realloc(DynString *cstr, int new_size);
void dynstring_chcat(DynString *cstr, int ch);
void dynstring_reset(DynString *cstr);
/*******************************dynstring.h begin****************************/

/*******************************dynarray.h begin****************************/
/*��̬���鶨��*/
typedef struct DynArray 
{
    int count;			// ��̬����Ԫ�ظ���
    int capacity;		// ��̬���黺�������ȶ�
    void **data;		// ָ������ָ������
} DynArray;

void dynarray_add(DynArray *parr, void *data);
void dynarray_init(DynArray *parr,int initsize);
void dynarray_free(DynArray *parr);                 
int dynarray_search(DynArray *parr, int key);
/*******************************dynarray.h begin****************************/

/********************************lex.h begin*****************************/
/* ���ʱ��� */
enum e_TokenCode
{  
	/* ��������ָ��� */
	TK_PLUS,		// + �Ӻ�
    TK_MINUS,		// - ����
    TK_STAR,		// * �Ǻ�
    TK_DIVIDE,		// / ����
    TK_MOD,			// % ���������
    TK_EQ,			// == ���ں�
    TK_NEQ,			// != �����ں�
    TK_LT,			// < С�ں�
    TK_LEQ,			// <= С�ڵ��ں�
    TK_GT,			// > ���ں�
    TK_GEQ,			// >= ���ڵ��ں�
    TK_ASSIGN,		// = ��ֵ����� 
    TK_POINTSTO,	// -> ָ��ṹ���Ա�����
    TK_DOT,			// . �ṹ���Ա�����
	TK_AND,         // & ��ַ�������
	TK_OPENPA,		// ( ��Բ����
	TK_CLOSEPA,		// ) ��Բ����
	TK_OPENBR,		// [ ��������
	TK_CLOSEBR,		// ] ��Բ����
	TK_BEGIN,		// { �������
	TK_END,			// } �Ҵ�����
    TK_SEMICOLON,	// ; �ֺ�    
    TK_COMMA,		// , ����
	TK_ELLIPSIS,	// ... ʡ�Ժ�
	TK_EOF,			// �ļ�������

    /* ���� */
    TK_CINT,		// ���ͳ���
    TK_CCHAR,		// �ַ�����
    TK_CSTR,		// �ַ�������

	/* �ؼ��� */
	KW_CHAR,		// char�ؼ���
	KW_SHORT,		// short�ؼ���
	KW_INT,			// int�ؼ���
    KW_VOID,		// void�ؼ���  
    KW_STRUCT,		// struct�ؼ���   
	KW_IF,			// if�ؼ���
	KW_ELSE,		// else�ؼ���
	KW_FOR,			// for�ؼ���
	KW_CONTINUE,	// continue�ؼ���
    KW_BREAK,		// break�ؼ���   
    KW_RETURN,		// return�ؼ���
    KW_SIZEOF,		// sizeof�ؼ���

    KW_ALIGN,		// __align�ؼ���	
    KW_CDECL,		// __cdecl�ؼ��� standard c call
	KW_STDCALL,     // __stdcall�ؼ��� pascal c call
	
	/* ��ʶ�� */
	TK_IDENT
};

/* �ʷ�״̬ */
enum e_LexState
{
	LEX_NORMAL,
	LEX_SEP
};

/* ���ʴ洢�ṹ���� */
typedef struct TkWord
{
    int  tkcode;					// ���ʱ��� 
    struct TkWord *next;			// ָ���ϣ��ͻ����������
    char *spelling;					// �����ַ���
    struct Symbol *sym_struct;		// ָ�򵥴�����ʾ�Ľṹ����
    struct Symbol *sym_identifier;	// ָ�򵥴�����ʾ�ı�ʶ��
} TkWord;

#define MAXKEY	1024				// ��ϣ������

extern TkWord* tk_hashtable[MAXKEY];// ���ʹ�ϣ��
extern DynArray tktable;			// ���ʶ�̬����
#define CH_EOF   (-1)				// �ļ�β��ʶ

TkWord* tkword_direct_insert(TkWord* tp);
TkWord* tkword_insert(char * p);
int  is_nodigit(char c);
int  is_digit(char c);
void getch();
void parse_num();
void parse_string(char sep);
void parse_comment();
void get_token();
char *get_tkstr(int v);
void init_lex();
void test_lex();
void color_token(int lex_state);
/*****************************lex.h end*************************************/


/*****************************error.h begin***********************************/
/* ���󼶱� */
enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR,
};

/* �����׶� */
enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK,
};
void warning(char *fmt, ...);
void error(char *fmt, ...);
void expect(char *msg);
void skip(int c);
void link_error(char *fmt, ...);
/*****************************error.h end*************************************/


/*******************************grammar.h begin****************************/
/* �﷨״̬ */
enum e_SynTaxState
{
	SNTX_NUL,       // ��״̬��û���﷨��������
	SNTX_SP,		// �ո� int a; int __stdcall MessageBoxA(); return 1;
	SNTX_LF_HT,		// ���в�������ÿһ���������������塢��������Ҫ��Ϊ��״̬
	SNTX_DELAY      // �ӳ�ȡ����һ���ʺ�ȷ�������ʽ��ȡ����һ�����ʺ󣬸��ݵ������͵�������syntax_indentȷ����ʽ������� 
};

/* �洢���� */
enum e_StorageClass
{
	SC_GLOBAL =   0x00f0,		// �������������ͳ������ַ��������ַ�������,ȫ�ֱ���,��������          
	SC_LOCAL  =   0x00f1,		// ջ�б���
	SC_LLOCAL =   0x00f2,       // �Ĵ���������ջ��
	SC_CMP    =   0x00f3,       // ʹ�ñ�־�Ĵ���
	SC_VALMASK=   0x00ff,       // �洢��������             
	SC_LVAL   =   0x0100,       // ��ֵ       
	SC_SYM    =   0x0200,       // ����	

	SC_ANOM	  = 0x10000000,     // ��������
	SC_STRUCT = 0x20000000,     // �ṹ�����
	SC_MEMBER = 0x40000000,     // �ṹ��Ա����
	SC_PARAMS = 0x80000000,     // ��������
};

/* ���ͱ��� */
enum e_TypeCode
{
	T_INT    =  0,			// ����                     
	T_CHAR   =  1,			// �ַ���                 
	T_SHORT  =  2,			// ������                       
	T_VOID   =  3,			// ������                        
	T_PTR    =  4,			// ָ��                          
	T_FUNC   =  5,			// ����                    
	T_STRUCT =  6,			// �ṹ�� 
	
	T_BTYPE  =  0x000f,		// ������������          
	T_ARRAY  =  0x0010,		// ����
};

#define ALIGN_SET 0x100  // ǿ�ƶ����־

extern int syntax_state;
extern int syntax_level;

void translation_unit();
void external_declaration(int l);
int type_specifier();
void init_declarator_list(int l);
void initializer();
void struct_specifier();
void struct_declaration_list();
void struct_declaration();
void declarator();
void function_calling_convention ();
void struct_member_alignment();
void direct_declarator();
void direct_declarator_postfix();
void parameter_type_list();
void funcbody();
void statement();
void compound_statement();
int is_type_specifier(int v);
void statement();
void expression_statement();
void for_statement();
void break_statement();
void continue_statement();
void if_statement();
void return_statement();
void assignment_expression();
void expression();
void equality_expression();
void relational_expression();
void additive_expression();
void multiplicative_expression();
void unary_expression();
void argument_expression_list();
void postfix_expression();
void primary_expression();
void sizeof_expression();
void syntax_indent();
/*******************************grammar.h end****************************/

/*******************************scc.h begin****************************/
extern DynString tkstr;
extern FILE *fin;
extern char ch;
extern char *filename;
extern int token;
extern int tkvalue;
extern int line_num;
int elf_hash(char *name);
void *mallocz(int size);
/******************************scc.h end*************************/