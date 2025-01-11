// ���Լ�����д��������������������Դ����

#include "scc.h"
int syntax_state;  //�﷨״̬
int syntax_level;  //��������

/***********************************************************
 * ����:	�������뵥λ
 *
 *  <translation_unit>::={external_declaration}<TK_EOF>
 **********************************************************/
void translation_unit()
{
	while(token != TK_EOF)
	{
		external_declaration(SC_GLOBAL);
	}
}

/***********************************************************
 * ����:	�����ⲿ����
 * l:		�洢���ͣ��ֲ��Ļ���ȫ�ֵ�
 *
 * <external_declaration>::=<function_definition>|<declaration>
 *
 * <function_definition>::= <type_specifier> <declarator><funcbody>
 *
 * <declaration>::= <type_specifier><TK_SEMICOLON>
 *		|<type_specifier>< init_declarator_list><TK_SEMICOLON>
 *
 * <init_declarator_list>::=
 *      <init_declarator>{<TK_COMMA> <init_declarator>}
 *
 * <init_declarator>::=
 *      <declarator>|<declarator> <TK_ASSIGN><initializer>
 *
 * ��д���ķ���
 * <external_declaration>::=
 *  <type_specifier> (<TK_SEMICOLON> 
 *      |<declarator><funcbody>
 *	    |<declarator>[<TK_ASSIGN><initializer>]
 *	     {<TK_COMMA> <declarator>[<TK_ASSIGN><initializer>]}
 *		 <TK_SEMICOLON>
 **********************************************************/
void external_declaration(int l)
{
	if (!type_specifier()) 
	{
		expect("<�������ַ�>");
	}	

	if (token == TK_SEMICOLON) 
	{
		get_token();
		return;
	}
    while (1)// �������������������
	{								
		declarator ();
		if (token == TK_BEGIN) 
		{
			if (l == SC_LOCAL)
				error("��֧�ֺ���Ƕ�׶���");  		
			funcbody();
			break;
		}
		else
		{

			if(token == TK_ASSIGN)	
			{
				get_token();
			    initializer();
			}	
	
		    if (token == TK_COMMA) 
			{
				get_token();
			}
			else
			{
			  syntax_state = SNTX_LF_HT;
			  skip(TK_SEMICOLON);
			  break;
			}			
		}
	}	
}


/***********************************************************
 * ����:	������ֵ��
 *
 * < initializer>::=<assignment_expression>
 **********************************************************/
void initializer()
{
	//	get_token();
	assignment_expression();
}

/***********************************************************
 * ����:		�������ַ�
 * type(���):	��������
 * ����ֵ:		�Ƿ��ֺϷ����������ַ�
 *
 *	<type_specifier>::= <KW_INT> 
 *		| <KW_CHAR> 
 *		| <KW_SHORT> 
 *		| <KW_VOID >
 *		| <struct_specifier>
 **********************************************************/
int type_specifier()
{		
	int type_found = 0;
    switch(token) 
	{
    case KW_CHAR:
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_SHORT:
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_VOID:
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_INT:
		syntax_state = SNTX_SP;
		type_found = 1;
		get_token();
		break;
	case KW_STRUCT:
		syntax_state = SNTX_SP;
		struct_specifier();
        type_found = 1;
		break;
	default:		
		break;
	}
	return type_found;
}

/***********************************************************
 * ����:		�����ṹ���ַ�
 *
 * <struct_specifier>::= 
 *	<KW_STRUCT><IDENTIFIER><TK_BEGIN><struct_declaration_list><TK_END> 
 *		| <KW_STRUCT>  <IDENTIFIER>
 **********************************************************/
void struct_specifier()
{
    int v;
	
    get_token();	
	v = token;
	
	syntax_state = SNTX_DELAY;      // ��ȡ���ʲ���ʱ������ӳٵ�ȡ�����ʺ���ݵ��������ж������ʽ
	get_token();

	if(token == TK_BEGIN)			// �����ڽṹ�嶨��
		syntax_state = SNTX_LF_HT;
	else if(token == TK_CLOSEPA)	// ������ sizeof(struct struct_name)
		syntax_state = SNTX_NUL;
	else							// �����ڽṹ��������
		syntax_state = SNTX_SP;
	syntax_indent();	
	
	if (v < TK_IDENT)				// �ؼ��ֲ�����Ϊ�ṹ����
		expect("�ṹ����");
    
    if (token == TK_BEGIN) 
	{
		struct_declaration_list();
    }
}

/***********************************************************
 * ����:		�����ṹ��������
 *
 * <struct_declaration_list>::=<struct_declaration>{<struct_declaration>}
 **********************************************************/
void struct_declaration_list()
{
    int maxalign, offset;
	
	syntax_state = SNTX_LF_HT;	// ��һ���ṹ���Ա��'{'��д��һ��
	syntax_level++;				// �ṹ���Ա������������������һ��

	get_token();
	while (token != TK_END) 
	{
		struct_declaration(&maxalign, &offset);
	}			
	skip(TK_END);

	syntax_state = SNTX_LF_HT;	
}

/***********************************************************
 * ����:				�����ṹ����
 *
 * <struct_declaration>::=
 *		<type_specifier><struct_declarator_list><TK_SEMICOLON>
 *
 * <struct_declarator_list>::=<declarator>{<TK_COMMA><declarator>}
 **********************************************************/
void struct_declaration()
{
	type_specifier();
	while (1) 
	{
		declarator ();                  
			
		if (token == TK_SEMICOLON || token == TK_EOF)
			break;
		skip(TK_COMMA);
	}
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);
}


/***********************************************************
 * ����:				����������
 *
 * <declarator>::={<pointer>}{<function_calling_convention>}
 *	{<struct_member_alignment>}<direct_declarator>
 *
 * <pointer>::=<TK_STAR>
 **********************************************************/
void declarator()
{
    while (token == TK_STAR) 
	{
		get_token();
    }
	function_calling_convention ();
	struct_member_alignment();
	direct_declarator();  
}

/***********************************************************
 * ����:	������������Լ��
 *
 * <function_calling_convention>::=<KW_CDECL>|<KW_STDCALL>
 * ���ں��������ϣ��������������Ϻ��Ե�
 **********************************************************/
void function_calling_convention ()
{
	if(token == KW_CDECL || token == KW_STDCALL)
	{
		syntax_state = SNTX_SP;
		get_token();		
	}
}

/***********************************************************
 * ����:				�����ṹ��Ա����
 *
 * <struct_member_alignment>::=<KW_ALIGN><TK_OPENPA><TK_CINT><TK_CLOSEPA>
 **********************************************************/
void struct_member_alignment()
{
	if(token == KW_ALIGN)
	{
		get_token();
		skip(TK_OPENPA);
		if(token == TK_CINT)
		{
			 get_token();
		}
		else
			expect("��������");
		skip(TK_CLOSEPA);
	}
}

/***********************************************************
 * ����:			����ֱ��������
 *
 * <direct_declarator>::=  <IDENTIFIER><direct_declarator_postfix>
 **********************************************************/
void direct_declarator()
{
	if (token >= TK_IDENT) 
	{		 
		get_token();
	} 
	else
	{
		expect("��ʶ��");
	}
	direct_declarator_postfix();
}

/***********************************************************
 * ����:			ֱ����������׺
 *
 *<direct_declarator_ postfix>::= {<TK_OPENBR><TK_CINT><TK_CLOSEBR>
 * 		|<TK_OPENBR><TK_CLOSEBR>
 *		|<TK_OPENPA><parameter_type_list><TK_CLOSEPA> 
 *		|<TK_OPENPA><TK_CLOSEPA>}
 **********************************************************/
void direct_declarator_postfix()
{
	int n;

    if (token == TK_OPENPA) 
	{ 
		parameter_type_list();
	}
	else if (token == TK_OPENBR)
	{
		get_token();
		if(token == TK_CINT)
		{
			get_token();
			n = tkvalue;
		}
		skip(TK_CLOSEBR);
		direct_declarator_postfix();
	}
}

/***********************************************************
 * ����:			�����β����ͱ�
 * func_call:		��������Լ��
 *
 * <parameter_type_list>::=<parameter_list>
 *        |<parameter_list><TK_COMMA><TK_ELLIPSIS>
 *
 *  <parameter_list>::=<parameter_declaration>
 *		  {<TK_COMMA ><parameter_declaration>}
 **********************************************************/
void parameter_type_list(int func_call)
{

	get_token();	
	while(token != TK_CLOSEPA)
	{
		if (!type_specifier()) 
		{
			error("��Ч���ͱ�ʶ��");
		}
		declarator();
		if (token == TK_CLOSEPA)
			break;
		skip(TK_COMMA);
		if (token == TK_ELLIPSIS) 
		{
			func_call = KW_CDECL;
			get_token();
			break;
		}
	}
	syntax_state = SNTX_DELAY;
	skip(TK_CLOSEPA);
	if(token == TK_BEGIN)			// ��������
		syntax_state = SNTX_LF_HT;
	else							// ��������
		syntax_state = SNTX_NUL;
	syntax_indent();
}

/***********************************************************
 * ����:	����������
 *
 * <funcbody>::=<compound_statement>
 **********************************************************/
void funcbody()
{
	/* ��һ���������ھֲ����ű��� */
    compound_statement(); 
}

/***********************************************************
 * ����:	�ж��Ƿ�Ϊ�������ַ�
 * v:		���ʱ��
 **********************************************************/
int is_type_specifier(int v)
{
	switch(v)
	{
	case KW_CHAR:		
	case KW_SHORT: 
	case KW_INT:		
	case KW_VOID:		
	case KW_STRUCT:
		return 1;
	default:
		break;
	}
	return 0;
}

/***********************************************************
 * ����:	�������
 * bsym:	break��תλ��
 * csym:	continue��תλ��
 *
 * <statement >::=<compound_statement> 
 *		| <if_statement> 
 *		| <return_statement> 
 *		| <break_statement> 
 *		| <continue_statement> 
 *		| <for_statement> 
 *		| <expression_statement>
 **********************************************************/
void statement()
{
	switch(token)
	{
	case TK_BEGIN:
		compound_statement();
		break;
	case KW_IF:
		if_statement();
		break;
	case KW_RETURN:
		return_statement();
		break;
	case KW_BREAK:
		break_statement();
		break;
	case KW_CONTINUE:
		continue_statement();
		break;
	case KW_FOR:
		for_statement();
		break;
	default:
		expression_statement();
		break;
	}	
}

/***********************************************************
 * ����:	�����������
 * bsym:	break��תλ��
 * csym:	continue��תλ��
 * 
 * <compound_statement>::=<TK_BEGIN>{<declaration>}{<statement>}<TK_END>
 **********************************************************/
void compound_statement()
{ 	
	syntax_state = SNTX_LF_HT;
	syntax_level++;						// ������䣬��������һ��
	
	get_token();
	while(is_type_specifier(token))
	{
		external_declaration(SC_LOCAL);
	}
	while (token != TK_END) 
	{		
		statement();
	}		
	
	syntax_state = SNTX_LF_HT;
	get_token();	
}

/***********************************************************
 * ����:	����if���
 * bsym:	break��תλ��
 * csym:	continue��תλ��
 *
 * <if_statement>::=<KW_IF><TK_OPENPA><expression>
 *	<TK_CLOSEPA><statement>[<KW_ELSE><statement>]
 **********************************************************/
void if_statement()
{
	syntax_state = SNTX_SP;
	get_token();
	skip(TK_OPENPA);
	expression();
	syntax_state = SNTX_LF_HT;		
	skip(TK_CLOSEPA);
	statement();		
	if (token == KW_ELSE) 
	{
		syntax_state = SNTX_LF_HT;
		get_token();
		statement();
	}
}

/***********************************************************
 * ����:	����for���
 * bsym:	break��תλ��
 * csym:	continue��תλ��
 * 
 * <for_statement>::=<KW_FOR><TK_OPENPA><expression_statement>
 *	<expression_statement><expression><TK_CLOSEPA><statement>
 **********************************************************/
void for_statement()
{
	get_token();
	skip(TK_OPENPA);
	if (token != TK_SEMICOLON) 
	{
		expression();
	}
	skip(TK_SEMICOLON);
	if (token != TK_SEMICOLON) 
	{
		expression();
	}
	skip(TK_SEMICOLON);
	if (token != TK_CLOSEPA) 
	{
	    expression();
	}
	syntax_state = SNTX_LF_HT;
	skip(TK_CLOSEPA);
	statement();//ֻ�д˴��õ�break,��continue,һ��ѭ���п����ж��break,����continue,����Ҫ�����Ա�����
}

/***********************************************************
 * ����:	����continue���
 * csym:	continue��תλ��
 * 
 * <continue_statement>::=<KW_CONTINUE><TK_SEMICOLON>
 **********************************************************/
void continue_statement()
{
	get_token();
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);	
}

/***********************************************************
 * ����:	����break���
 * bsym:	break��תλ��
 * 
 * <break_statement>::=<KW_BREAK><TK_SEMICOLON>
 **********************************************************/
void break_statement()
{
	get_token();
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);	
}
/***********************************************************
 * ����:	����return���
 *  
 * <return_statement>::=<KW_RETURN><TK_SEMICOLON>
 *			|<KW_RETURN><expression><TK_SEMICOLON>
 **********************************************************/
void return_statement()
{
	syntax_state = SNTX_DELAY;
	get_token();
	if(token == TK_SEMICOLON)	// ������ return;
		syntax_state = SNTX_NUL;
	else						// ������ return <expression>;
		syntax_state = SNTX_SP;
	syntax_indent();
	
	if (token != TK_SEMICOLON) 
	{
		expression();
	}
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);		
}

/***********************************************************
 * ����:	�������ʽ���
 *  
 * <expression_statement>::= <TK_SEMICOLON>|<expression> <TK_SEMICOLON> 
 **********************************************************/
void expression_statement()
{	
	if (token != TK_SEMICOLON) 
	{
		expression();
	}
	syntax_state = SNTX_LF_HT;
	skip(TK_SEMICOLON);
}

/***********************************************************
 * ����:	�������ʽ
 * 
 * <expression>::=<assignment_expression>{<TK_COMMA><assignment_expression>}
 **********************************************************/
void expression()
{
    while (1) 
	{
        assignment_expression();
        if (token != TK_COMMA)
            break;
        get_token();
    }
}


/***********************************************************
 * ����:	������ֵ���ʽ
 *
 * <assignment_expression>::= <equality_expression>
 *		|<unary_expression><TK_ASSIGN> <equality_expression> 
 **********************************************************/
void assignment_expression()
{
    equality_expression();
    if (token == TK_ASSIGN) 
	{
        get_token();
        assignment_expression();
    }
}

/***********************************************************
 * ����:	�����������ʽ
 *
 * < equality_expression >::=<relational_expression>
 *		{<TK_EQ> <relational_expression>
 *		|<TK_NEQ><relational_expression>}
 **********************************************************/
void equality_expression()
{

    int t;
    relational_expression();
    while (token == TK_EQ || token == TK_NEQ) 
	{
        t = token;
        get_token();
        relational_expression();
    }
}

/***********************************************************
 * ����:	������ϵ���ʽ
 *
 * <relational_expression>::=<additive_expression>{
 *		<TK_LT><additive_expression> 
 *		|<TK_GT><additive_expression> 
 *		|<TK_LEQ><additive_expression> 
 *		|<TK_GEQ><additive_expression>}
 **********************************************************/
void relational_expression()
{
    additive_expression();
    while ((token == TK_LT || token == TK_LEQ) ||
           token == TK_GT || token == TK_GEQ) 
	{
        get_token();
        additive_expression();
    }
}

/***********************************************************
 * ����:	�����Ӽ�����ʽ
 *
 * <additive_expression>::=< multiplicative_expression> 
 *		{<TK_PLUS> <multiplicative_expression>
 *		<TK_MINUS>< multiplicative_expression>}
 **********************************************************/
void additive_expression()
{
    multiplicative_expression();
    while (token == TK_PLUS || token == TK_MINUS)
	{
        get_token();
        multiplicative_expression();
    }
}

/***********************************************************
 * ����:	�����˳�����ʽ
 *
 * <multiplicative_expression>::=<unary_expression>
 *		{<TK_STAR>  < unary_expression >
 *		|<TK_DIVIDE>< unary_expression > 
 *		|<TK_MOD>  < unary_expression >}
 **********************************************************/
void multiplicative_expression()
{
    int t;
    unary_expression();
    while (token == TK_STAR || token == TK_DIVIDE || token == TK_MOD) 
	{
        t = token;
        get_token();
        unary_expression();
    }
}

/***********************************************************
 * ����:	����һԪ���ʽ
 *
 * <unary_expression>::= <postfix_expression> 
 *			|<TK_AND><unary_expression> 
 *			|<TK_STAR><unary_expression> 
 *			|<TK_PLUS><unary_expression> 
 *			|<TK_MINUS><unary_expression> 
 *			|<KW_SIZEOF><TK_OPENPA><type_specifier><TK_ CLOSEPA> 
 **********************************************************/
void unary_expression()
{
    switch(token) 
	{
   	case TK_AND:
        get_token();
        unary_expression();       
        break;
	case TK_STAR:
        get_token();
		unary_expression();
        break;
	case TK_PLUS:
        get_token();       
        unary_expression();
        break;
	case TK_MINUS:
        get_token();
        unary_expression();
        break;
    case KW_SIZEOF:
		sizeof_expression();
        break;
	default:
		postfix_expression();
		break;
	}

}

/***********************************************************
 * ����:	����sizeof���ʽ
 *
 * <sizeof_expression>::= 
 *		<KW_SIZEOF><TK_OPENPA><type_specifier><TK_ CLOSEPA>
 **********************************************************/
void sizeof_expression()
{	
	get_token();
	skip(TK_OPENPA);
	type_specifier();
	skip(TK_CLOSEPA);
}

/***********************************************************
 * ����:	������׺���ʽ
 *
 * <postfix_expression>::=  <primary_expression> 
 *		{<TK_OPENBR><expression> <TK_CLOSEBR> 
 *		|<TK_OPENPA><TK_CLOSEPA>
 *		|<TK_OPENPA><argument_expression_list><TK_CLOSEPA>
 *		|<TK_DOT><IDENTIFIER> 
 *		|<TK_POINTSTO><IDENTIFIER>}
 **********************************************************/
void postfix_expression()
{
	primary_expression();
    while (1) 
	{
		if (token == TK_DOT || token == TK_POINTSTO) 
		{		
            get_token();
            token |= SC_MEMBER;
            get_token();
		} 
		else if (token == TK_OPENBR) 
		{
            get_token();
            expression();
            skip(TK_CLOSEBR);
        } 
		else if (token == TK_OPENPA) 
		{
			argument_expression_list();
		}
		else
			break;
	}
}

/***********************************************************
 * ����:	�������ȱ��ʽ
 *
 * <primary_expression>::=<IDENTIFIER>
 *		|<TK_CINT>
 *		|<TK_CSTR>
 *		|<TK_CCHAR>
 *		|<TK_OPENPA><expression><TK_CLOSEPA>
 **********************************************************/
void primary_expression()
{
	int t;
	switch(token) 
	{
    case TK_CINT: 
    case TK_CCHAR:
		get_token();
        break;
	case TK_CSTR:
		get_token();
        break;
	case TK_OPENPA:
		get_token();
		expression();
		skip(TK_CLOSEPA);
		break;
	default:
		 t = token;		
		 get_token();
		 if(t < TK_IDENT)
			expect("��ʶ������");
		break;
	}
}

/***********************************************************
 * ����:	����ʵ�α��ʽ��
 *
 * <argument_expression_list >::=<assignment_expression>
 *		{<TK_COMMA> <assignment_expression>}
 **********************************************************/
void argument_expression_list()
{
	get_token();
	if (token != TK_CLOSEPA) 
	{
		for(;;) 
		{
			assignment_expression();
			if (token == TK_CLOSEPA)
				break;
			skip(TK_COMMA);
		}
	}   
	skip(TK_CLOSEPA);
	// return value
}

/***********************************************************
 * ����:	����n��tab��
 * n:		��������
 **********************************************************/
void print_tab(int n)
{
	int i = 0;
	for(; i < n; i++)
	    printf("\t");
}

/***********************************************************
 * ����:	�﷨����
 **********************************************************/
void syntax_indent()
{
	switch(syntax_state)
	{
		case SNTX_NUL:
			color_token(LEX_NORMAL);
			break;	
		case SNTX_SP:
			printf(" ");
			color_token(LEX_NORMAL);
			break;	
		case SNTX_LF_HT:
			{	
				if(token == TK_END)		// ����'}',��������һ��
					syntax_level--;
				printf("\n");
				print_tab(syntax_level);				
			}
			color_token(LEX_NORMAL);
			break;		
		case SNTX_DELAY:
			break;
	}	
	syntax_state = SNTX_NUL;
}


