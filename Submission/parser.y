%{
#include <stdio.h>
extern "C" int yyerror (const char*);
extern int yylineno;
int yylex();

Data_Type dt, return_type;
Symbol_Table * gst = new Symbol_Table();
Symbol_Table * lst = new Symbol_Table();
Symbol_Table * fst = new Symbol_Table();
int param_num;
Procedure * cur_proc;
%}

%union {
	int integer_value;
	double double_value;
	std::string* string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Procedure * procedure;
	Sequence_Ast * seq_Ast;
	Data_Type data_type;
};

%token <string_value> INTEGER FLOAT VOID NAME ASSIGN IF ELSE AND OR RELOP EQOP NOT DO WHILE PRINT RETURN
%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token UMINUS

%type <symbol_table> param_list comma_type_list variable_list declaration variable_declaration variable_declaration_list optional_variable_declaration_list
%type <ast> assignment_statement print_statement return_statement expression constant boolean_expression rel_expression statement open_statement closed_statement variable
%type <seq_Ast> seq_statement
%type <ast_list> statement_list expression_list
%type <string_value> relop
%type <data_type> type

%start program

%right ASSIGN ':' '?'
%left OR
%left AND
%left EQOP
%left RELOP
%left '+' '-'
%left '*' '/'
%right NOT UMINUS

%%
program					:	optional_declaration_list
							procedure_definition_list
							{
								Procedure * pr = program_object.get_procedure_prototype("main");
								program_object.called_proc_are_defined_check();
								program_object.global_list_in_proc_check();
								program_object.global_list_in_proc_map_check();
								// std::cout << "body7\n";
								if(pr == NULL || !pr->is_proc_defined() || pr->get_return_type() != void_data_type){
									char msg[100] = "Main should be defined and it's type should be void";
									yyerror(msg);
								}
								gst->set_table_scope(global);
								// std::cout << "body8\n";
								program_object.set_global_table(*gst);
								// std::cout << "body9\n";
							}
						;

optional_declaration_list 	:	/* empty */
							|	optional_declaration_list optional_declaration
							;

optional_declaration 	:	declaration ';'
							{
								gst->append_list(*$1, yylineno);
							}
						|	type NAME '(' param_list ')' ';'
							{
								// IF ALREADY DECLARED THEN DO SOMETHING CONDITION REQUIRED
								if(gst->variable_in_symbol_list_check(*$2)){
									char msg[100] = "Procedure name cannot be same as global variable";
									yyerror(msg);
								}
								$4->set_table_scope(formal);
								Procedure * pr = new Procedure($1, *$2, yylineno);
								pr->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, pr);
							}
						|	type NAME '(' comma_type_list ')' ';'
							{
								// IF ALREADY DECLARED THEN DO SOMETHING CONDITION REQUIRED
								if(gst->variable_in_symbol_list_check(*$2)){
									char msg[100] = "Procedure name cannot be same as global variable";
									yyerror(msg);
								}
								$4->set_table_scope(formal);
								Procedure * pr = new Procedure($1, *$2, yylineno);
								pr->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, pr);
							}
						|	type NAME '(' ')' ';'
							{
								// IF ALREADY DECLARED THEN DO SOMETHING CONDITION REQUIRED
								if(gst->variable_in_symbol_list_check(*$2)){
									char msg[100] = "Procedure name cannot be same as global variable";
									yyerror(msg);
								}
								Symbol_Table * st = new Symbol_Table();
								st->set_table_scope(formal);
								Procedure * pr = new Procedure($1, *$2, yylineno);
								pr->set_formal_param_list(*st);
								program_object.set_proc_to_map(*$2, pr);
							}

param_list		:	type NAME
					{
						if ($1 ==  void_data_type){
							char msg[100] = "Cannot parse the input program";
							yyerror(msg);
						}
						$$ = new Symbol_Table();
						$$->push_symbol(new Symbol_Table_Entry(*$2, $1, yylineno));
					}
				|	param_list ',' type NAME
					{
						if ($3 ==  void_data_type){
							char msg[100] = "Cannot parse the input program";
							yyerror(msg);
						}
						$1->push_symbol(new Symbol_Table_Entry(*$4, $3, yylineno));
						$$ = $1;
					}
				;

comma_type_list 	:	type
						{
							if ($1 ==  void_data_type){
								char msg[100] = "Cannot parse the input program";
								yyerror(msg);
							}
							$$ = new Symbol_Table();
							// Symbol_Table_Entry * ste = new Symbol_Table_Entry("", $1, yylineno);
							// ste->set_symbol_scope(formal);
							param_num = 1;
							std::string* name = new std::string("dummy" + std::to_string(param_num));
							$$->push_symbol(new Symbol_Table_Entry(*name, $1, yylineno));
						}
					|	comma_type_list ',' type
						{
							if ($3 ==  void_data_type){
								char msg[100] = "Cannot parse the input program";
								yyerror(msg);
							}
							param_num += 1;
							std::string* name = new std::string("dummy" + std::to_string(param_num));
							$1->push_symbol(new Symbol_Table_Entry(*name, $3, yylineno));
						}
					;

procedure_definition_list	:	procedure_definition
							|	procedure_definition_list procedure_definition
							;

procedure_definition 		:	type NAME '(' param_list ')'
								{
									if(gst->variable_in_symbol_list_check(*$2)){
										char msg[100] = "Procedure name cannot be same as global variable";
										yyerror(msg);
									}
									if(program_object.is_procedure_exists(*$2)){
										Procedure * pr = program_object.get_procedure_prototype(*$2);
										list<Symbol_Table_Entry *> st1 =  pr->get_formal_param_list().get_table();
										list<Symbol_Table_Entry *> st2 = $4->get_table();
										list<Symbol_Table_Entry *>::iterator it1 = st1.begin(), it2 = st2.begin();
										if(st1.size() != st2.size() || $1 != pr->get_return_type()){
											char msg[100] = "Return types or param count of declaration and definition don't match";
											yyerror(msg);
										}
										while(it1 != st1.end()){
											if((*it1)->get_data_type() != (*it2)->get_data_type()){
												char msg[100] = "Param type of declaration and definition don't match";
												yyerror(msg);
											}
											it1++;
											it2++;
										}
									}
									$4->set_table_scope(formal);
									cur_proc = new Procedure($1, *$2, yylineno);
									cur_proc->set_formal_param_list(*$4);
									program_object.set_proc_to_map(*$2, cur_proc);
									fst = $4;
								}
								'{' 
									optional_variable_declaration_list
									statement_list
								'}'
								{
									// IF ALREADY DECLARED THEN DO SOMETHING CONDITION REQUIRED
									// if(pr->is_proc_defined())
									Procedure * pr = program_object.get_procedure_prototype(*$2);
									pr->set_proc_is_defined();
									pr->set_local_list(*$8);
									pr->set_ast_list(*$9);
								}
							|	type NAME '(' ')'
								{
									if(gst->variable_in_symbol_list_check(*$2)){
										char msg[100] = "Procedure name cannot be same as global variable";
										yyerror(msg);
									}
									if(program_object.is_procedure_exists(*$2)){
										Procedure * pr = program_object.get_procedure_prototype(*$2);
										list<Symbol_Table_Entry *> st1 =  pr->get_formal_param_list().get_table();
										if(st1.size() != 0 || $1 != pr->get_return_type()){
											char msg[100] = "Return types or param count of declaration and definition don't match";
											yyerror(msg);
										}
									}
									Symbol_Table * st = new Symbol_Table();
									st->set_table_scope(formal);
									cur_proc = new Procedure($1, *$2, yylineno);
									cur_proc->set_formal_param_list(*st);
									program_object.set_proc_to_map(*$2, cur_proc);
									fst = st;
								}
								'{' 
									optional_variable_declaration_list
									statement_list
								'}'
								{
									// IF ALREADY DECLARED THEN DO SOMETHING CONDITION REQUIRED
									Procedure * pr = program_object.get_procedure_prototype(*$2);
									pr->set_proc_is_defined();
									pr->set_local_list(*$7);
									pr->set_ast_list(*$8);
								}
							;

optional_variable_declaration_list	:	/* empty */
										{
											$$ = new Symbol_Table();
											lst = $$;
										}
									|	variable_declaration_list
										{
											$1->set_table_scope(local);
											$$ = $1;
											lst = $$;
										}
									;

variable_declaration_list	:	variable_declaration {$$ = $1;}
							|	variable_declaration_list 
								variable_declaration
								{
									$1->append_list(*$2, yylineno);
									$$ = $1;
								}
							;

variable_declaration	:	declaration ';' {$$ = $1;}
						;

declaration			:	type variable_list 
						{
							if($1 == void_data_type){
								char msg[100] = "Attempt to seek size of type void";
								yyerror(msg);
							}
							$$ = $2;
						}
					;
                    
type				:	INTEGER {dt = int_data_type; $$ = dt;}
					|	FLOAT {dt = double_data_type; $$ = dt;}
					|	VOID {dt = void_data_type; $$ = dt;}
					;

variable_list       :	NAME
						{
							Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$1, dt, yylineno);
							// ste->set_symbol_scope(scope);
							$$ = new Symbol_Table();
							$$->push_symbol(ste);
						}
                    |	variable_list ',' NAME
                    	{
                    		Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$3, dt, yylineno);
                    		// ste->set_symbol_scope(scope);
                    		if($1->variable_in_symbol_list_check(*$3)){
                    			$$ = NULL;
								char msg[100] = "Variable is declared twice";
								yyerror(msg);
                    		}
                    		else{
                    			$1->push_symbol(ste);
                    			$$ = $1;	
                    		}	
                    	}
					;

statement_list	    :	/* empty */ {$$ = new list<Ast *>();}
					|	seq_statement
						{
							list<Ast * >* body = new list<Ast * >();
							body->push_back($1);
							$$ = body;
						}
					;

closed_statement	:	IF '(' boolean_expression ')' closed_statement ELSE closed_statement
						{
							if($3 == NULL || $5 == NULL || $7 == NULL)
								$$ = NULL;
							else {
								Selection_Statement_Ast * Sel_ast = new Selection_Statement_Ast($3, $5, $7, yylineno);
								if(!Sel_ast->check_ast()){
									Sel_ast->set_data_type(void_data_type);
									$$ = Sel_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "if seq open";
									yyerror(msg);
								}
							}
						}
					|	WHILE '(' boolean_expression ')' closed_statement
						{
							if($3 == NULL || $5 == NULL)
								$$ = NULL;
							else {
								Iteration_Statement_Ast * While_Ast = new Iteration_Statement_Ast($3, $5, yylineno, false);
								if(!While_Ast->check_ast()){
									While_Ast->set_data_type(void_data_type);
									$$ = While_Ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "while single";
									yyerror(msg);
								}
							}
						}
					|	DO statement WHILE '(' boolean_expression ')' ';'
						{
							if($5 == NULL || $2 == NULL)
								$$ = NULL;
							else {
								Iteration_Statement_Ast * While_Ast = new Iteration_Statement_Ast($5, $2, yylineno, true);
								if(!While_Ast->check_ast()){
									While_Ast->set_data_type(void_data_type);
									$$ = While_Ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "do seq";
									yyerror(msg);
								}
							}
						}
					|	assignment_statement {$$ = $1;}
					|	print_statement {$$ = $1;}
					|	'{' seq_statement '}' {$$ = $2;}
					|	return_statement {$$ = $1;}
					|	NAME '(' expression_list ')' ';'
						{
							Procedure * pr = program_object.get_procedure_prototype(*$1);
							if(pr != NULL){
								pr->set_proc_is_called();
								Call_Ast * c_ast = new Call_Ast(*$1, yylineno);
								c_ast->set_actual_param_list(*$3);
								c_ast->set_data_type(void_data_type);
								// Check if parameter list matches
								c_ast->check_actual_formal_param(pr->get_formal_param_list());
								$$ = c_ast;
							}
							else{
								$$ = NULL;
								char msg[100] = "Function not defined";
								yyerror(msg);
							}
						}
					|	NAME '(' ')' ';'
						{
							Procedure * pr = program_object.get_procedure_prototype(*$1);
							if(pr != NULL){
								pr->set_proc_is_called();
								Call_Ast * c_ast = new Call_Ast(*$1, yylineno);
								list<Ast *> * elist = new list<Ast *>();
								c_ast->set_actual_param_list(*elist);
								c_ast->set_data_type(void_data_type);
								// Check if parameter list matches
								c_ast->check_actual_formal_param(pr->get_formal_param_list());
								$$ = c_ast;
							}
							else{
								$$ = NULL;
								char msg[100] = "Function not defined";
								yyerror(msg);
							}
						}
					;

open_statement		:	IF '(' boolean_expression ')' statement
						{
							if($3 == NULL || $5 == NULL)
								$$ = NULL;
							else {
								Selection_Statement_Ast * Sel_ast = new Selection_Statement_Ast($3, $5, NULL, yylineno);
								if(!Sel_ast->check_ast()){
									Sel_ast->set_data_type(void_data_type);
									$$ = Sel_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "if seq open";
									yyerror(msg);
								}
							}
						}
					|	IF '(' boolean_expression ')' closed_statement ELSE open_statement
						{
							if($3 == NULL || $5 == NULL || $7 == NULL)
								$$ = NULL;
							else {
								Selection_Statement_Ast * Sel_ast = new Selection_Statement_Ast($3, $5, $7, yylineno);
								if(!Sel_ast->check_ast()){
									Sel_ast->set_data_type(void_data_type);
									$$ = Sel_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "if seq open";
									yyerror(msg);
								}
							}
						}
					|	WHILE '(' boolean_expression ')' open_statement
						{
							if($3 == NULL || $5 == NULL)
								$$ = NULL;
							else {
								Iteration_Statement_Ast * While_Ast = new Iteration_Statement_Ast($3, $5, yylineno, false);
								if(!While_Ast->check_ast()){
									While_Ast->set_data_type(void_data_type);
									$$ = While_Ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "while single";
									yyerror(msg);
								}
							}
						}
					;

statement			:	open_statement {$$ = $1;}
					|	closed_statement {$$ = $1;}
					;

seq_statement		:	statement 
						{
							$$ = new Sequence_Ast(yylineno);
							if($1 != NULL) $$->ast_push_back($1);
						}
					|	seq_statement
						statement
						{
							if($2 != NULL) $1->ast_push_back($2);
							$$ = $1;
						}
					;

variable 		:	NAME
					{
						if(lst->variable_in_symbol_list_check(*$1)){
							Symbol_Table_Entry & var = lst->get_symbol_table_entry(*$1);
							Name_Ast * lhs = new Name_Ast(*$1, var, yylineno);
							lhs->set_data_type(var.get_data_type());
							$$ = lhs;
						}
						else if(fst->variable_in_symbol_list_check(*$1)){
							Symbol_Table_Entry & var = fst->get_symbol_table_entry(*$1);
							Name_Ast * lhs = new Name_Ast(*$1, var, yylineno);
							lhs->set_data_type(var.get_data_type());
							$$ = lhs;
						}
						else if (gst->variable_in_symbol_list_check(*$1)){
							Symbol_Table_Entry & var = gst->get_symbol_table_entry(*$1);
							Name_Ast * lhs = new Name_Ast(*$1, var, yylineno);
							lhs->set_data_type(var.get_data_type());
							$$ = lhs;
						}
						else {
							$$ = NULL;
							char msg[100] = "Variable has not been declared";
							yyerror(msg);
						}
					}
				;

assignment_statement	:	variable ASSIGN expression ';'
							{
								if($3 == NULL || $1 == NULL)
									$$ = NULL;
								else {
									// std::cout << "c4\n";
									$$ = new Assignment_Ast($1, $3, yylineno);
									if($$->check_ast())
									{
										$$ = NULL;
										char msg[100] = "Assignment statement data type not compatible";
										yyerror(msg);
									}
								} 
							}
						;

print_statement		:	PRINT variable ';'
						{
							if($2 == NULL)
								$$ = NULL;
							else 
								$$ = new Print_Ast($2, yylineno);
						}
					;

return_statement	:	RETURN expression ';'
						{
							if(cur_proc->get_return_type() != $2->get_data_type()){
								$$ = NULL;
								char msg[100] = "Return type not matching";
								yyerror(msg);
							}
							else{
								$$ = new Return_Ast($2, cur_proc->get_proc_name(), yylineno);
								$$->set_data_type(cur_proc->get_return_type());
							}
						}
					|	RETURN ';'
						{
							if(cur_proc->get_return_type() == void_data_type){
								$$ = new Return_Ast(NULL, cur_proc->get_proc_name(), yylineno);
								$$->set_data_type(void_data_type);
							}
							else{
								$$ = NULL;
								char msg[100] = "Return type not matching";
								yyerror(msg);
							}
						}

expression_list	:	expression
					{
						$$ = new list<Ast *>();
						$$->push_back($1);
					}
				|	expression_list ',' expression
					{
						$1->push_back($3);
						$$ = $1;
					}
				;

expression	:	variable
			|	constant {$$ = $1;}
			|	'(' expression ')' {$$ = $2;}
			|	NAME '(' expression_list ')'
				{
					Procedure * pr = program_object.get_procedure_prototype(*$1);
					if(pr != NULL){
						pr->set_proc_is_called();
						Call_Ast * c_ast = new Call_Ast(*$1, yylineno);
						c_ast->set_actual_param_list(*$3);
						c_ast->set_data_type(pr->get_return_type());
						// Check if parameter list matches
						c_ast->check_actual_formal_param(pr->get_formal_param_list());
						$$ = c_ast;
					}
					else{
						$$ = NULL;
						char msg[100] = "Function not defined";
						yyerror(msg);
					}
				}
			|	NAME '(' ')'
				{
					Procedure * pr = program_object.get_procedure_prototype(*$1);
					if(pr != NULL){
						pr->set_proc_is_called();
						Call_Ast * c_ast = new Call_Ast(*$1, yylineno);
						list<Ast *> * elist = new list<Ast *>();
						c_ast->set_actual_param_list(*elist);
						c_ast->set_data_type(pr->get_return_type());
						// Check if parameter list matches
						c_ast->check_actual_formal_param(pr->get_formal_param_list());
						$$ = c_ast;
					}
					else{
						$$ = NULL;
						char msg[100] = "Function not defined";
						yyerror(msg);
					}
				}
			|	expression '+' expression
				{
					if($1 == NULL || $3 == NULL)
						$$ = NULL;
					else{
						Plus_Ast * Arith_st = new Plus_Ast($1, $3, yylineno);
						if(!Arith_st->check_ast()){
							Arith_st->set_data_type($1->get_data_type());
							$$ = Arith_st;
						}
						else {
							$$ = NULL;
							char msg[100] = "Arithmetic statement data type not compatible";
							yyerror(msg);
						}
					} 
				}
			|	expression '-' expression
				{
					if($1 == NULL || $3 == NULL)
						$$ = NULL;
					else{
						Minus_Ast * Arith_st = new Minus_Ast($1, $3, yylineno);
						if(!Arith_st->check_ast()){
							Arith_st->set_data_type($1->get_data_type());
							$$ = Arith_st;
						}
						else {
							$$ = NULL;
							char msg[100] = "Arithmetic statement data type not compatible";
							yyerror(msg);
						}
					} 
				}
			|	expression '*' expression
				{
					if($1 == NULL || $3 == NULL)
						$$ = NULL;
					else{
						Mult_Ast * Arith_st = new Mult_Ast($1, $3, yylineno);
						if(!Arith_st->check_ast()){		
							Arith_st->set_data_type($1->get_data_type());
							$$ = Arith_st;
						}
						else {
							$$ = NULL;
							char msg[100] = "Arithmetic statement data type not compatible";
							yyerror(msg);
						}
					} 
				}
			|	expression '/' expression
				{
					if($1 == NULL || $3 == NULL)
						$$ = NULL;
					else{
						Divide_Ast * Arith_st = new Divide_Ast($1, $3, yylineno);						
						if(!Arith_st->check_ast()){
							Arith_st->set_data_type($1->get_data_type());
							$$ = Arith_st;
						}
						else {
							$$ = NULL;
							char msg[100] = "Arithmetic statement data type not compatible";
							yyerror(msg);
						}
					} 
				}
			|	'-' expression	%prec UMINUS
				{
					if($2 == NULL)
						$$ = NULL;
					else{
						UMinus_Ast * Arith_st = new UMinus_Ast($2, NULL, yylineno);
						Arith_st->set_data_type($2->get_data_type());
						$$ = Arith_st;
					}
				}
			|	boolean_expression '?' expression ':' expression
				{
					// std::cout << "c1\n";
					Conditional_Expression_Ast * Cond_ast = new Conditional_Expression_Ast($1, $3, $5, yylineno);
					if(!Cond_ast->check_ast()){
						Cond_ast->set_data_type($3->get_data_type());
						$$ = Cond_ast;
					}
					else {
						$$ = NULL;
						char msg[100] = "Arithmetic statement data type not compatible";
						yyerror(msg);
					}
				}
			;

rel_expression		:	expression relop expression
						{
							if($1 == NULL || $3 == NULL)
								$$ = NULL;
							else {
								// std::cout << "c2\n";
								Relational_Op op;
								if(*$2 == "<=") op = less_equalto;
								else if(*$2 == "<") op = less_than;
								else if(*$2 == ">=") op = greater_equalto;
								else if(*$2 == ">") op = greater_than;
								else if(*$2 == "==") op = equalto;
								else op = not_equalto;
								Relational_Expr_Ast * Rel_ast = new Relational_Expr_Ast($1, op, $3, yylineno);
								if(!Rel_ast->check_ast()){
									Rel_ast->set_data_type(void_data_type);
									$$ = Rel_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "Relational statement data type not compatible";
									yyerror(msg);
								}
							}
						}
					;

boolean_expression 	:	rel_expression {$$ = $1;}
					|	boolean_expression AND boolean_expression
						{
							if($1 == NULL || $3 == NULL)
								$$ = NULL;
							else{
								Logical_Expr_Ast * Log_ast = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
								if(!Log_ast->check_ast()){
									Log_ast->set_data_type(void_data_type);
									$$ = Log_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "Syntax Error";
									yyerror(msg);
								}
							}
						}
					|	boolean_expression OR boolean_expression
						{
							if($1 == NULL || $3 == NULL)
								$$ = NULL;
							else {
								Logical_Expr_Ast * Log_ast = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
								if(!Log_ast->check_ast()){
									Log_ast->set_data_type(void_data_type);
									$$ = Log_ast;
								}
								else {
									$$ = NULL;
									char msg[100] = "Syntax Error";
									yyerror(msg);
								}
							}
						}
					|	NOT boolean_expression
						{
							if($2 == NULL)
								$$ = NULL;
							else {
								Logical_Expr_Ast * Log_ast = new Logical_Expr_Ast(NULL, _logical_not, $2, yylineno);
								Log_ast->set_data_type(void_data_type);
								$$ = Log_ast;
							}
						}
					|	'(' boolean_expression ')' {$$ = $2;}
					;

relop 		:	RELOP {$$ = $1;}
			|	EQOP {$$ = $1;}
			;

constant		:	INTEGER_NUMBER {$$ = new Number_Ast<int>($1, int_data_type, yylineno);}
				|	DOUBLE_NUMBER {$$ = new Number_Ast<double>($1, double_data_type, yylineno);}
				;

%%
int yyerror(const char* msg){
	fprintf(stderr, "cs316: Error: Line: %d: %s\n",yylineno, msg);
	exit(0);
}
// https://www.parsifalsoft.com/ifelse.html	https://en.wikipedia.org/wiki/Dangling_else	https://en.cppreference.com/w/cpp/language/operator_precedence
