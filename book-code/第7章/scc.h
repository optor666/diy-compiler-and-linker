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
int is_nodigit(char c);
int is_digit(char c);
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


/*******************************stack.h begin****************************/
/*ջ����*/
typedef struct Stack 
{
	void **base;	// ջ��ָ��
	void **top;		// ջ��ָ��
	int stacksize;	// ջ��ǰ��ʹ�õ��������,��Ԫ��Ϊ��λ	
} Stack;

void stack_init(Stack *stack,int initsize);
void* stack_push(Stack *stack, void *element, int size);
void stack_pop(Stack *stack);
void* stack_get_top(Stack *stack);
int stack_is_empty(Stack *stack);
void stack_destroy(Stack *stack);
/*******************************stack.h begin****************************/

/*******************************symbol.h begin****************************/
/* ���ʹ洢�ṹ���� */
typedef struct Type 
{
    int t;
    struct Symbol *ref;
} Type;

/* ���Ŵ洢�ṹ���� */
typedef struct Symbol 
{
    int v;						// ���ŵĵ��ʱ���
    int r;						// ���Ź����ļĴ���
    int c;						// ���Ź���ֵ
    Type type;					// ��������
    struct Symbol *next;		// �������������ţ��ṹ�嶨�������Ա�������ţ��������������������
    struct Symbol *prev_tok;	// ָ��ǰһ�����ͬ������
} Symbol;

extern Type char_pointer_type, int_type,default_func_type;
extern Symbol *sym_sec_rdata;
Symbol *struct_search(int v);
Symbol *sym_search(int v);
Symbol *sym_direct_push(Stack *ss, int v, Type *type, int c);
Symbol *sym_push(int v, Type *type, int r, int c);
void sym_pop(Stack *ptop, Symbol *b);
void mk_pointer(Type *type);
extern Stack global_sym_stack, local_sym_stack;
Symbol *func_sym_push(int v, Type *type);
Symbol *sec_sym_put(char *sec,int c);
Symbol *var_sym_put(Type *type, int r, int v, int addr);
void check_lvalue();
int type_size(Type *type, int *a);
/*******************************symbol.h end****************************/


/*******************************grammar.h begin****************************/
/* �﷨״̬ */
enum e_SynTaxState
{
	SNTX_NUL,       // ��״̬��û���﷨��������
	SNTX_SP,		// �ո�
	SNTX_LF_HT,		// ���в�������ÿһ���������������塢��������Ҫ��Ϊ��״̬
	SNTX_DELAY      // �ӳ����
};

/* �洢���� */
enum e_StorageClass
{
	SC_GLOBAL =   0x00f0,		// �������������ͳ������ַ��������ַ�������,ȫ�ֱ���,��������          
	SC_LOCAL  =   0x00f1,		// ջ�б���
	SC_LLOCAL =   0x00f2,       // �Ĵ���������ջ��
	SC_CMP    =   0x00f3,       // ʹ�ñ�־�Ĵ���
	SC_VALMASK=   0x00ff,       // �洢������             
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
	/* types */                                                     
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
int type_specifier(Type *type);
void init_declarator_list(int l,Type *btype);
void initializer(Type *type, int c, struct Section *sec);
void struct_specifier(Type *type);
void struct_declaration_list(Type *type);
void struct_declaration(int *maxalign,int *offset,Symbol ***ps);
void declarator(Type *type, int *v,int *force_align);
void function_calling_convention (int *fc);
void struct_member_alignment(int *force_align);
void direct_declarator(Type *type,int *v,int func_call);
void direct_declarator_postfix(Type *type,int func_call);
void parameter_type_list(Type *type,int func_call);
void funcbody(Symbol *sym);
void statement(int *bsym, int *csym);
void compound_statement(int *bsym, int *csym);
int is_type_specifier(int v);
void statement(int *bsym, int *csym);
void expression_statement();
void for_statement(int *bsym, int *csym);
void break_statement(int *bsym);
void continue_statement(int *csym);
void if_statement(int *bsym, int *csym);
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
/* ������� */
enum e_OutType
{	
    OUTPUT_OBJ,		// Ŀ���ļ�
	OUTPUT_EXE,		// EXE��ִ���ļ�
    OUTPUT_MEMORY	// �ڴ���ֱ�����У������
};
                                 
extern DynString tkstr;
extern DynArray sections;
extern FILE *fin;
extern char ch;
extern char *filename;
extern int token;
extern int tkvalue;
extern int line_num;
int elf_hash(char *name);
void *mallocz(int size);
int calc_align(int n, int align);
/******************************scc.h end*************************/

/*******************************outcoff.h begin****************************/
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#pragma pack(push, 1)
/* �ڽṹ���� */
typedef struct Section 
{ 
	int data_offset;			// ��ǰ����ƫ��λ��
    char *data;					// ������
    int data_allocated;			// �����ڴ�ռ�
	char  index;				// �����
	struct Section *link;       // ������������
	int *hashtab;				// ��ϣ��ֻ���ڴ洢���ű�
	IMAGE_SECTION_HEADER sh;    // ��ͷ
} Section;

/* ���ű��¼ 
ԭ����
// Symbol format.
//

typedef struct _IMAGE_SYMBOL {
    union {
        BYTE    ShortName[8];
        struct {
            DWORD   Short;     // if 0, use LongName
            DWORD   Long;      // offset into string table
        } Name;
        PBYTE   LongName[2];
    } N;
    DWORD   Value;
    SHORT   SectionNumber;
    WORD    Type;
    BYTE    StorageClass;
    BYTE    NumberOfAuxSymbols;
} IMAGE_SYMBOL;
*/
/* �������ݣ�Name ��������ԭ��Ϊ8���ֽڳ�����*/
/* COFF���Žṹ���� */
typedef struct CoffSym 
{

    DWORD Name;					// ��������
	DWORD Next;					// ���ڱ����ͻ����*/
    /* 
    struct {
		DWORD   Short;			// if 0, use LongName
        DWORD   Long;			// offset into string table
    } name;
	*/
    DWORD   Value;				// �������ص�ֵ
    short   Section;			// �ڱ������(��1��ʼ),���Ա�ʶ����˷��ŵĽ�*/
    WORD    Type;				// һ����ʾ���͵�����
    BYTE    StorageClass;		// ����һ����ʾ�洢����ö������ֵ
    BYTE    NumberOfAuxSymbols;	// ���ڱ���¼����ĸ������ű���ĸ���
} CoffSym;

#define CST_FUNC    0x20  //Coff�������ͣ�����
#define CST_NOTFUNC 0     //Coff�������ͣ��Ǻ���

/* COFF�ض�λ��¼ 
ԭ����:
//
// Relocation format.
//

typedef struct _IMAGE_RELOCATION {
    union {
        DWORD   VirtualAddress;
        DWORD   RelocCount;             // Set to the real count when IMAGE_SCN_LNK_NRELOC_OVFL is set
    };
    DWORD   SymbolTableIndex;
    WORD    Type;
} IMAGE_RELOCATION;
*/
/* �ض�λ�ṹ���� */
typedef struct CoffReloc 
{
    DWORD offset;	// ��Ҫ�����ض�λ�Ĵ�������ݵĵ�ַ
    DWORD cfsym;				// ���ű������(��0��ʼ)
	BYTE  section;  // �˴���һ��Ϊʲô��COFF�ض�λ�ṹ�����޸ļ�¼Section��Ϣ*/
    BYTE  type;    
} CoffReloc;

#pragma pack(pop)

extern Section *sec_text, *sec_data, *sec_bss, *sec_idata,*sec_rdata,*sec_rel,*sec_symtab,*sec_dynsymtab;
extern DynArray sections;
extern int nsec_image;
void init_coff();
void section_realloc(Section *sec, int new_size);
void *section_ptr_add(Section *sec, int size);
void coffsym_add_update(Symbol *s, int val, int sec_index,short type, char StorageClass);
void coffreloc_add(Section *sec, Symbol *sym, int offset, char type);
void fpad(FILE *fp, int new_pos);
void free_sections();
void coffreloc_direct_add(int offset, int cfsym,char section, char type);
int coffsym_add(Section *symtab,char* name, int val, int sec_index,
							short type,  char StorageClass);
int coffsym_search(Section *symtab,char *name);
void write_obj(char *name);
/*******************************outcoff.h end****************************/
