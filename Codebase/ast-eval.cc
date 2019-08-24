void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){ }

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){ }

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){ }

/**************************************************************************************************/

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	print(file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);
	lhs->set_value_of_evaluation(eval_env, result2);

	lhs->print_value(eval_env, file_buffer);
	return *result;
}

/**************************************************************************************************/

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	string name = variable_symbol_entry->get_variable_name();
	Eval_Result & result1 = get_value_of_evaluation(eval_env);

	if(&result1 != NULL){
		file_buffer << "\n" << AST_SPACE << name << " : ";
		if(result1.is_variable_defined()) {
			if(result1.get_result_enum() == double_result) file_buffer << result1.get_double_value() << "\n\n";
			else if(result1.get_result_enum() == int_result) file_buffer << result1.get_int_value() << "\n\n";
		}
		else file_buffer << "undefined\n\n";
	}
	else {
		std::cout << "cs316: Error: Line: " << lineno << " : Variable has not been declared\n";
		exit(0);
	}
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	Eval_Result * result = NULL;
	string name = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(name) && eval_env.is_variable_defined(name))
		result = eval_env.get_variable_value(name);
	else if(interpreter_global_table.does_variable_exist(name)){
		result = interpreter_global_table.get_variable_value(name);
		if(!result->is_variable_defined()){
			result->set_value(0);
			set_value_of_evaluation(interpreter_global_table, *result);
		}
	}
	else {
		std::cout << "cs316: Error: Line: " << lineno << " : Variable should be defined before its use\n";
		exit(0);
	}
	return *result;	
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	string name = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(name)){
		result.set_variable_status(true);
		eval_env.put_variable_value(result, name);
	}
	else if(interpreter_global_table.does_variable_exist(name)){
		result.set_variable_status(true);
		interpreter_global_table.put_variable_value(result, name);
	}
	else {
		std::cout << "cs316: Error: Line: " << lineno << " : Variable has not been declared\n";
		exit(0);
	}
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = NULL;
	string name = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(name) && eval_env.is_variable_defined(name))
		result = eval_env.get_variable_value(name);
	else if(interpreter_global_table.does_variable_exist(name)){
		result = interpreter_global_table.get_variable_value(name);
		if(!result->is_variable_defined()){
			result->set_value(0);
			set_value_of_evaluation(interpreter_global_table, *result);
		}
	}
	else {
		std::cout << "cs316: Error: Line: " << lineno << " : Variable should be defined before its use\n";
		exit(0);
	}
	return *result;
}

/**************************************************************************************************/

template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		result->set_result_enum(int_result);
	}
	else{
		result = new Eval_Result_Value_Double();
		result->set_result_enum(double_result);
	}
	result->set_value(constant);
	return *result;
}

/**************************************************************************************************/

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		result->set_value(result1.get_int_value() + result2.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		result->set_value(result1.get_double_value() + result2.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		result->set_value(result1.get_int_value() - result2.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		result->set_value(result1.get_double_value() - result2.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		if(result2.get_int_value() == 0){
			std::cout << "cs316: Error: Line: " << lineno << " : Divide by 0\n";
			exit(0);
		}
		else
			result->set_value(result1.get_int_value() / result2.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		if(result2.get_double_value() == 0){
			std::cout << "cs316: Error: Line: " << lineno << " : Divide by 0\n";
			exit(0);
		}
		else
			result->set_value(result1.get_double_value() / result2.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		result->set_value(result1.get_int_value() * result2.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		result->set_value(result1.get_double_value() * result2.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		result->set_value(-result1.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		result->set_value(-result1.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	Eval_Result * result = NULL;
	if(get_data_type() == int_data_type){
		result = new Eval_Result_Value_Int();
		if(cond->evaluate(eval_env, file_buffer).get_int_value() == 1)
			result->set_value(result1.get_int_value());
		else
			result->set_value(result2.get_int_value());
	}
	else if(get_data_type() == double_data_type){
		result = new Eval_Result_Value_Double();
		if(cond->evaluate(eval_env, file_buffer).get_int_value() == 1)
			result->set_value(result1.get_double_value());
		else
			result->set_value(result2.get_double_value());
	}
	result->set_result_enum(result1.get_result_enum());
	return *result;
}

/**************************************************************************************************/

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = NULL;
	return *result;
}

/**************************************************************************************************/

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = NULL;
	return *result;
}

/**************************************************************************************************/

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & result1 = lhs_condition->evaluate(eval_env, file_buffer);
	Eval_Result & result2 = rhs_condition->evaluate(eval_env, file_buffer);
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);

	if(lhs_condition->get_data_type() == int_data_type){
		int val1 = result1.get_int_value();
		int val2 = result2.get_int_value();
		if(rel_op == less_equalto) result->set_value(val1 <= val2);	
		else if(rel_op == less_than) result->set_value(val1 < val2);
		else if(rel_op == greater_equalto) result->set_value(val1 >= val2);
		else if(rel_op == greater_than) result->set_value(val1 > val2);
		else if(rel_op == equalto) result->set_value(val1 == val2);
		else if(rel_op == not_equalto) result->set_value(val1 != val2);
	}
	else if(lhs_condition->get_data_type() == double_data_type){
		double val1 = result1.get_double_value();
		double val2 = result2.get_double_value();
		if(rel_op == less_equalto) result->set_value(val1 <= val2);	
		else if(rel_op == less_than) result->set_value(val1 < val2);
		else if(rel_op == greater_equalto) result->set_value(val1 >= val2);
		else if(rel_op == greater_than) result->set_value(val1 > val2);
		else if(rel_op == equalto) result->set_value(val1 == val2);
		else if(rel_op == not_equalto) result->set_value(val1 != val2);
	}
	return *result;
}

/**************************************************************************************************/

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);
	int rhs_res = rhs_op->evaluate(eval_env, file_buffer).get_int_value();
	if(bool_op == _logical_not)
		result->set_value(!rhs_res);
	else if(bool_op == _logical_or){
		int lhs_res = lhs_op->evaluate(eval_env, file_buffer).get_int_value();	
		result->set_value(lhs_res || rhs_res);
	}
	else if(bool_op == _logical_and){
		int lhs_res = lhs_op->evaluate(eval_env, file_buffer).get_int_value();
		result->set_value(lhs_res && rhs_res);
	}
	return *result;
}

/**************************************************************************************************/

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);
	Eval_Result & result1 = cond->evaluate(eval_env, file_buffer);
	if(result1.get_int_value() == 1){
		if(then_part != NULL) then_part->evaluate(eval_env, file_buffer);
	}
	else{
		if(else_part !=NULL) else_part->evaluate(eval_env, file_buffer);
	}
	return *result;
}

/**************************************************************************************************/

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);
	if(body != NULL){
		if(is_do_form)
			body->evaluate(eval_env, file_buffer);
		while(cond->evaluate(eval_env, file_buffer).get_int_value() == 1)
			body->evaluate(eval_env, file_buffer);
	}
	else {
		while(cond->evaluate(eval_env, file_buffer).get_int_value() == 1){ }
	}
	return *result;
}

/**************************************************************************************************/

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result * result = new Eval_Result_Value_Int();
	result->set_result_enum(void_result);
	for (list <Ast *>::iterator i = statement_list.begin(); i != statement_list.end(); i++){
		(*i)->evaluate(eval_env, file_buffer);	
	}
	return *result;
}

/**************************************************************************************************/


template class Number_Ast<double>;
template class Number_Ast<int>;

/**************************************************************************************************/
void Symbol_Table::create(Local_Environment & local_global_variables_table){
	for (list <Symbol_Table_Entry *>::iterator i = variable_table.begin(); i != variable_table.end(); i++){
		Data_Type dt = (*i)->get_data_type();
		string name = (*i)->get_variable_name();
		Table_Scope ts = (*i)->get_symbol_scope();
		if(dt == int_data_type){
			Eval_Result * result = new Eval_Result_Value_Int();
			if(ts == global) {
				result->set_value(0);
				result->set_variable_status(true);
			}
			local_global_variables_table.put_variable_value(*result, name);
		}
		else if(dt == double_data_type){
			Eval_Result * result = new Eval_Result_Value_Double();
			if(ts == global) {
				result->set_value(0.00);
				result->set_variable_status(true);
			}
			local_global_variables_table.put_variable_value(*result, name);
		}
	}
}
