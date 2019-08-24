
/*********************************************************************************************

								cfglp : A CFG Language Processor
								--------------------------------

		   About:

		   Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
		   Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
		   cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
		   Bombay.

		   Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
		   Khedker. This  implemenation  has been made  available purely
		   for academic purposes without any warranty of any kind.

		   Documentation (functionality, manual, and design) and related
		   tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/
int Ast::labelCounter = 1;
using namespace std;

Ast::Ast(){ }

Ast::~Ast(){ }

void Ast::print(ostream & file_buffer){ }

Data_Type Ast::get_data_type(){
	return node_data_type;
 }

void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
 }

bool Ast::is_value_zero(){ }

bool Ast::check_ast(){ }

Symbol_Table_Entry & Ast::get_symbol_entry(){ }

/*---------------------------------------------------------------------*/

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
	ast_num_child = binary_arity;
}

Assignment_Ast:: ~Assignment_Ast(){ }

void Assignment_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "Asgn:\n";
	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

bool Assignment_Ast::check_ast(){
	return lhs == NULL || rhs == NULL || lhs->get_data_type() != rhs->get_data_type();
}

/*---------------------------------------------------------------------*/

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	lineno = line;
	ast_num_child = zero_arity;
	node_data_type = var_entry.get_data_type();
	variable_symbol_entry = &var_entry;
}

Name_Ast:: ~Name_Ast(){ }

void Name_Ast::print(ostream & file_buffer){
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

Data_Type Name_Ast::get_data_type(){
	return node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

/*---------------------------------------------------------------------*/

template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	constant = number;
	ast_num_child = zero_arity;
	node_data_type = constant_data_type;
	lineno = line;
}

template <class T>
Number_Ast<T>:: ~Number_Ast(){ }

template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer << "Num : " << constant;
}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
	return constant == 0;
}

/*---------------------------------------------------------------------*/

Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
	return lhs->get_data_type() != rhs->get_data_type();
}

/*---------------------------------------------------------------------*/

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Plus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Minus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Mult_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Divide_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = unary_arity;
}

void UMinus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
	lhs = l;
	rhs = r;
	this->cond = cond;
	lineno = line;
	ast_num_child = ternary_arity;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast(){ }

void Conditional_Expression_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "Cond:\n";
	file_buffer << AST_NODE_SPACE << "IF_ELSE";
	cond->print(file_buffer);
	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}
	
/*---------------------------------------------------------------------*/

Call_Ast::Call_Ast(string name, int line){
	procedure_name = name;
	lineno = line;
	return_value_reg = NULL;
}

Call_Ast::~Call_Ast(){ }

Data_Type Call_Ast::get_data_type(){
	return program_object.get_procedure_prototype(procedure_name)->get_return_type();
}

void Call_Ast::set_register(Register_Descriptor * reg){
	return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
	list<Symbol_Table_Entry *> table_got =  formal_param_list.get_table();
	if(actual_param_list.size() != table_got.size()){
		char msg[100] = "cs316: Error: Actual and formal parameter count do not match";
		std::cout << msg << "\n";
		exit(0);
	}
	list<Symbol_Table_Entry *>::iterator it_st = table_got.begin();
	list<Ast *>::iterator it_ast = actual_param_list.begin();
	while(it_st != table_got.end()) {
		if ((*it_ast)->get_data_type() != (*it_st)->get_data_type()){
			char msg[100] = "cs316: Error: Actual and formal parameters data types are not matching";
			std::cout << msg << "\n";
			exit(0);
		}
		it_st++;
		it_ast++;
	}
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "FN CALL: "<<procedure_name<<"(\n";
	list<Ast *>::iterator it_ast = actual_param_list.begin();
	while(it_ast != actual_param_list.end()){
		file_buffer << "\n";
		(*it_ast)->print(file_buffer);
	}
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Return_Ast::Return_Ast(Ast * ret_val, string name, int line){
	return_value = ret_val;
	proc_name = name;
	lineno = line;
	if(return_value != NULL) node_data_type = return_value->get_data_type();
	else node_data_type = void_data_type;
}

Return_Ast::~Return_Ast(){ }

Data_Type Return_Ast::get_data_type(){
	return node_data_type;
}

void Return_Ast::print(ostream & file_buffer){
	if(return_value == NULL){
		file_buffer << "\n" << AST_SPACE << "RETURN <NOTHING>\n";
	}
	else {
		file_buffer << "\n" << AST_SPACE << "RETURN ";
		return_value->print(file_buffer);
	}
}

/*---------------------------------------------------------------------*/

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	lineno = line;
	ast_num_child = binary_arity;
}

Relational_Expr_Ast::~Relational_Expr_Ast(){ }

Data_Type Relational_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){
	return lhs_condition == NULL || rhs_condition == NULL || lhs_condition->get_data_type() != rhs_condition->get_data_type();
}

void Relational_Expr_Ast::print(ostream & file_buffer){
	string op;
	if(rel_op == less_equalto) op = "LE";	
	else if(rel_op == less_than)op = "LT";
	else if(rel_op == greater_equalto) op = "GE";
	else if(rel_op == greater_than)op = "GT";
	else if(rel_op == equalto) op = "EQ";
	else if(rel_op == not_equalto) op = "NE";

	file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << op << "\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print(file_buffer);
	file_buffer << ")\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs_condition->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	if(bool_op == _logical_not) ast_num_child = unary_arity;
	else ast_num_child = binary_arity;
}

Logical_Expr_Ast::~Logical_Expr_Ast(){ }

Data_Type Logical_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){
	return lhs_op->get_data_type() != rhs_op->get_data_type();
}

void Logical_Expr_Ast::print(ostream & file_buffer){
	string op;
	if(bool_op == _logical_and) op = "AND";	
	else if(bool_op == _logical_or)op = "OR";
	else if(bool_op == _logical_not) op = "NOT";

	file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << op << "\n";
	if(bool_op != _logical_not){
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")\n";
	}
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")";
}

/*---------------------------------------------------------------------*/	

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond, Ast* then_part, Ast* else_part, int line){
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	lineno = line;
	ast_num_child = ternary_arity;
}

Selection_Statement_Ast::~Selection_Statement_Ast(){ }

Data_Type Selection_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){
	return cond == NULL;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "IF : ";
	file_buffer << "\n" << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")";
	if(then_part != NULL){
		file_buffer << "\n" << AST_SPACE << "THEN (";
		then_part->print(file_buffer);
		file_buffer << ")";
	}
	if(else_part != NULL){
		file_buffer << "\n" << AST_SPACE << "ELSE (";
		else_part->print(file_buffer);
		file_buffer << ")";
	}
}

/*---------------------------------------------------------------------*/

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
	this->cond = cond;
	this->body = body;
	is_do_form = do_form;
	lineno = line;
	ast_num_child = binary_arity;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast(){ }

Data_Type Iteration_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast(){
	return cond == NULL;
}

void Iteration_Statement_Ast::print(ostream & file_buffer){
	if(is_do_form){
		file_buffer << "\n" << AST_SPACE << "DO (";
		if(body != NULL) body->print(file_buffer);
		file_buffer << ")";

		file_buffer << "\n" << AST_SPACE << "WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
	}
	else {
		file_buffer << "\n" << AST_SPACE << "WHILE : ";
		file_buffer << "\n" << AST_SPACE << "CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
		if(body != NULL){
			file_buffer << "\n" << AST_SPACE << "BODY (";
			body->print(file_buffer);
			file_buffer << ")";
		}
	}
}

/*---------------------------------------------------------------------*/

Sequence_Ast::Sequence_Ast(int line){
	lineno = line;
}

Sequence_Ast::~Sequence_Ast(){ }

void Sequence_Ast::ast_push_back(Ast * ast){
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
	for (list <Ast *>::iterator i = statement_list.begin(); i != statement_list.end(); i++){
		file_buffer << "\n" << AST_NODE_SPACE;
		(*i)->print(file_buffer);
	}
}

/*---------------------------------------------------------------------*/

Print_Ast::Print_Ast(Ast *v, int line){
	var = v;
	lineno = line;
	node_data_type = v->get_data_type();
}

Print_Ast::~Print_Ast(){ }

void Print_Ast::print(ostream & file_buffer){
	file_buffer << "\n";
	var->print(file_buffer);
}

/*------------------------------------------------------------------------------*/

template class Number_Ast<double>;
template class Number_Ast<int>;
