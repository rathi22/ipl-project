Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast & r_code = rhs->compile();
	Register_Descriptor * red = r_code.get_reg();
	Code_For_Ast & l_code = lhs->create_store_stmt(red);

	red->reset_use_for_expr_result();
	
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();
	rl.splice(rl.end(),ll);

	Code_For_Ast * code = new Code_For_Ast(rl, red);
	return *code;
}

Code_For_Ast & Name_Ast::compile(){
	Register_Descriptor * red;
	Mem_Addr_Opd * opd1 = new Mem_Addr_Opd(*variable_symbol_entry);
	Tgt_Op op;
	list<Icode_Stmt * > l;
	Move_IC_Stmt * ic;
	Code_For_Ast * code = NULL;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = load;
	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = load_d;
	}

// variable_symbol_entry->set_register(red);

	Register_Addr_Opd * opd2 = new Register_Addr_Opd(red);
	ic = new Move_IC_Stmt(op, opd1, opd2);

	l.push_back(ic);
	code = new Code_For_Ast(l, red);
	return *code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
// variable_symbol_entry->set_register(store_register);
// store_register->update_symbol_information(*variable_symbol_entry);

	Mem_Addr_Opd * opd1 = new Mem_Addr_Opd(*variable_symbol_entry);
	Tgt_Op op;
	list<Icode_Stmt * > l;
	Move_IC_Stmt * ic;
	Code_For_Ast * code = NULL;

	if(node_data_type == int_data_type){
		op = store;
	} else if(node_data_type == double_data_type) {
		op = store_d;
	}

	Register_Addr_Opd * opd2 = new Register_Addr_Opd(store_register);
	ic = new Move_IC_Stmt(op, opd2, opd1);

	l.push_back(ic);
	code = new Code_For_Ast(l, store_register);
	return *code;
}

template <class T>
Code_For_Ast & Number_Ast<T>::compile(){
	Register_Descriptor * red;
	Tgt_Op op;
	list<Icode_Stmt * > l;
	Move_IC_Stmt * ic;
	Code_For_Ast * code = NULL;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = imm_load;
		Const_Opd<int> * opd1 = new Const_Opd<int>(constant);
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(red);
		ic = new Move_IC_Stmt(op, opd1, opd2);

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = imm_load_d;
		Const_Opd<double> * opd1 = new Const_Opd<double>(constant);
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(red);
		ic = new Move_IC_Stmt(op, opd1, opd2);

	} 
	// if(is_value_zero()){
	// 	red = machine_desc_object.get_new_register<fixed_reg>();
	// }

	l.push_back(ic);
	code = new Code_For_Ast(l, red);
	return *code;
}

Code_For_Ast & Plus_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs->compile(); 
	Code_For_Ast & r_code = rhs->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Tgt_Op op;
	Compute_IC_Stmt * ic;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = add;

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = add_d;
	}

	red->set_use_for_expr_result();
	l_code.get_reg()->reset_use_for_expr_result();
	r_code.get_reg()->reset_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
	Register_Addr_Opd * result = new Register_Addr_Opd(red);
	ic = new Compute_IC_Stmt(op, opd1, opd2, result);

	ll.splice(ll.end(),rl);
	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs->compile(); 
	Code_For_Ast & r_code = rhs->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Tgt_Op op;
	Compute_IC_Stmt * ic;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = sub;

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = sub_d;
	}

	red->set_use_for_expr_result();
	l_code.get_reg()->reset_use_for_expr_result();
	r_code.get_reg()->reset_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
	Register_Addr_Opd * result = new Register_Addr_Opd(red);
	ic = new Compute_IC_Stmt(op, opd1, opd2, result);

	ll.splice(ll.end(),rl);
	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs->compile(); 
	Code_For_Ast & r_code = rhs->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Tgt_Op op;
	Compute_IC_Stmt * ic;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = mult;

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = mult_d;
	}

	red->set_use_for_expr_result();
	l_code.get_reg()->reset_use_for_expr_result();
	r_code.get_reg()->reset_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
	Register_Addr_Opd * result = new Register_Addr_Opd(red);
	ic = new Compute_IC_Stmt(op, opd1, opd2, result);

	ll.splice(ll.end(),rl);
	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs->compile(); 
	Code_For_Ast & r_code = rhs->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Tgt_Op op;
	Compute_IC_Stmt * ic;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = divd;

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = div_d;
	}

	red->set_use_for_expr_result();
	l_code.get_reg()->reset_use_for_expr_result();
	r_code.get_reg()->reset_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
	Register_Addr_Opd * result = new Register_Addr_Opd(red);
	ic = new Compute_IC_Stmt(op, opd1, opd2, result);

	ll.splice(ll.end(),rl);
	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}


// UMinus I used Move stmt, but use Compute_Stmt with NULL 
// and take cases in printing 
Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs->compile();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Tgt_Op op;
	Move_IC_Stmt * ic;

	if(node_data_type == int_data_type){
		red = machine_desc_object.get_new_register<int_reg>();
		op = uminus;

	} else if(node_data_type == double_data_type) {
		red = machine_desc_object.get_new_register<float_reg>();
		op = uminus_d;
	}

	red->set_use_for_expr_result();
	l_code.get_reg()->reset_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * result = new Register_Addr_Opd(red);
	ic = new Move_IC_Stmt(op, opd1, result);

	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Conditional_Expression_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & cond_code = cond->compile();
	Code_For_Ast & l_code = lhs->compile();
	Code_For_Ast & r_code = rhs->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();
	list<Icode_Stmt * > cl = cond_code.get_icode_list();

	Register_Descriptor * red;

	Tgt_Op op_r; int flag = 0;
	if(node_data_type == int_data_type){
		op_r = or_t;
		red = machine_desc_object.get_new_register<int_reg>();
	}	
	else if(node_data_type == double_data_type){
		op_r = add_d; flag = 1;
		red = machine_desc_object.get_new_register<float_reg>();
	}

	string label1 = get_new_label();
	string label2 = get_new_label();

	red->set_use_for_expr_result();	
	

	Register_Addr_Opd * opdl = new Register_Addr_Opd(l_code.get_reg());
	Register_Addr_Opd * opdr = new Register_Addr_Opd(r_code.get_reg());
	Register_Descriptor * redc = machine_desc_object.spim_register_table[zero];
	Register_Addr_Opd * opdc = new Register_Addr_Opd(redc);	
	Register_Addr_Opd * result = new Register_Addr_Opd(red);

	if(cond_code.get_reg() != NULL){
		cond_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd_cond = new Register_Addr_Opd(cond_code.get_reg());
		cl.push_back(new Control_Flow_IC_Stmt(beq, opd_cond, NULL, label1));

	} else {
		Tgt_Op op, bcs;
		list<Icode_Stmt * >::iterator it = cl.end();
		it--;
		op = (*it)->get_op().get_op();
		if (op == sge_d || op == sgt_d || op == sne_d) bcs = bc1t;
		else bcs = bc1f;

		cl.push_back(new Label_IC_Stmt(bcs, label1));
	}
	

	cl.splice(cl.end(),ll);
	if(flag){
		Register_Descriptor* red_t = machine_desc_object.get_new_register<float_reg>();
		Const_Opd<double> * opd1 = new Const_Opd<double>(0.0);
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(red_t);
		cl.push_back(new Move_IC_Stmt(imm_load_d, opd1, opd2));
		cl.push_back(new Compute_IC_Stmt(op_r, opdl, opd2, result));
	} else {
		cl.push_back(new Compute_IC_Stmt(op_r, opdl, opdc, result));
	}

	
	cl.push_back(new Label_IC_Stmt(j, label2));
	cl.push_back(new Label_IC_Stmt(label, label1));
	cl.splice(cl.end(),rl);
	if(flag){
		Register_Descriptor* red_t = machine_desc_object.get_new_register<float_reg>();
		Const_Opd<double> * opd1 = new Const_Opd<double>(0.0);
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(red_t);
		cl.push_back(new Move_IC_Stmt(imm_load_d, opd1, opd2));
		cl.push_back(new Compute_IC_Stmt(op_r, opdr, opd2, result));
	} else {
		cl.push_back(new Compute_IC_Stmt(op_r, opdr, opdc, result));
	}
	cl.push_back(new Label_IC_Stmt(label, label2));

	l_code.get_reg()->reset_use_for_expr_result();
	r_code.get_reg()->reset_use_for_expr_result();
	
	code = new Code_For_Ast(cl, red);
	return *code;
}


Code_For_Ast & Relational_Expr_Ast::compile(){
	Code_For_Ast * code = NULL;
	Code_For_Ast & l_code = lhs_condition->compile();
	Code_For_Ast & r_code = rhs_condition->compile();
	list<Icode_Stmt * > rl = r_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();

	Register_Descriptor * red;
	Compute_IC_Stmt * ic = NULL;
	Tgt_Op op;

	if(lhs_condition->get_data_type() == int_data_type){
		switch(rel_op){
			case less_equalto: {op = sle; break;}
			case less_than:  {op = slt; break;}
			case greater_equalto:  {op = sge; break;}
			case greater_than:  {op = sgt; break;}
			case equalto:  {op = seq; break;}
			case not_equalto:  {op = sne; break;}
			default: break;
		}
		red = machine_desc_object.get_new_register<int_reg>();
		red->set_use_for_expr_result();
		l_code.get_reg()->reset_use_for_expr_result();
		r_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
		Register_Addr_Opd * result = new Register_Addr_Opd(red);

		ic = new Compute_IC_Stmt(op, opd1, opd2, result);
		
	} else {
		switch(rel_op){
			case less_equalto: {op = sle_d; break;}
			case less_than:  {op = slt_d; break;}
			case greater_equalto:  {op = sge_d; break;}
			case greater_than:  {op = sgt_d;  break;}
			case equalto:  {op = seq_d;  break;}
			case not_equalto:  {op = sne_d;  break;}
			default: break;
		}
		red = NULL;
		l_code.get_reg()->reset_use_for_expr_result();
		r_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());

		ic = new Compute_IC_Stmt(op, opd1, opd2, NULL);
	}

	ll.splice(ll.end(),rl);
	ll.push_back(ic);
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Logical_Expr_Ast::compile(){
	Code_For_Ast * code = NULL;
	Register_Descriptor * red, * rnot;
	Tgt_Op op;
	list<Icode_Stmt * > rl, ll;
	Compute_IC_Stmt * ic;

	switch(bool_op){
		case _logical_and: {op = and_t; break;}
		case _logical_or:  {op = or_t; break;}
		case _logical_not:  {op = not_t; break;}
	}

	if (op != not_t) {
		Code_For_Ast & l_code = lhs_op->compile();
		Code_For_Ast & r_code = rhs_op->compile();	
		ll = l_code.get_icode_list();
		rl = r_code.get_icode_list();
		red = machine_desc_object.get_new_register<int_reg>();
		red->set_use_for_expr_result();

		l_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd1 = new Register_Addr_Opd(l_code.get_reg());
		r_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());

		Register_Addr_Opd * result = new Register_Addr_Opd(red);
		ic = new Compute_IC_Stmt(op, opd1, opd2, result);			
		ll.splice(ll.end(),rl);
		ll.push_back(ic);

	} else{
		Const_Opd<int> * con = new Const_Opd<int>(1);
		rnot = machine_desc_object.get_new_register<int_reg>();
		rnot->set_use_for_expr_result();
		Register_Addr_Opd * opd1 = new Register_Addr_Opd(rnot);
		ll.push_back(new Move_IC_Stmt(imm_load, con, opd1));

		Code_For_Ast & r_code = rhs_op->compile();
		rl = r_code.get_icode_list();
		
		red = machine_desc_object.get_new_register<int_reg>();
		red->set_use_for_expr_result();
		
		rnot->reset_use_for_expr_result();
		r_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd2 = new Register_Addr_Opd(r_code.get_reg());
		Register_Addr_Opd * result = new Register_Addr_Opd(red);
		ic = new Compute_IC_Stmt(op, opd2, opd1, result);			
		ll.splice(ll.end(),rl);
		ll.push_back(ic);
	}
	
	code = new Code_For_Ast(ll, red);
	return *code;
}

Code_For_Ast & Selection_Statement_Ast::compile(){
	Code_For_Ast * code = NULL;
	Register_Descriptor * red = NULL;
	if(else_part != NULL){
		Code_For_Ast & cond_code = cond->compile();
		Code_For_Ast & l_code = then_part->compile();
		Code_For_Ast & r_code = else_part->compile();
		list<Icode_Stmt * > cl = cond_code.get_icode_list();
		list<Icode_Stmt * > ll = l_code.get_icode_list();
		list<Icode_Stmt * > rl = r_code.get_icode_list();

		string label1 = get_new_label();
		string label2 = get_new_label();

		if(cond_code.get_reg() != NULL){
			cond_code.get_reg()->reset_use_for_expr_result();
			Register_Addr_Opd * opd_cond = new Register_Addr_Opd(cond_code.get_reg());
			
			cl.push_back(new Control_Flow_IC_Stmt(beq, opd_cond, NULL, label1));
		} else {
			Tgt_Op op, bcs;
			list<Icode_Stmt * >::iterator it = cl.end();
			it--;
			op = (*it)->get_op().get_op();
			if (op == sge_d || op == sgt_d || op == sne_d) bcs = bc1t;
			else bcs = bc1f;

			cl.push_back(new Label_IC_Stmt(bcs, label1));
		}

		cl.splice(cl.end(),ll);
		cl.push_back(new Label_IC_Stmt(j, label2));
		cl.push_back(new Label_IC_Stmt(label, label1));
		cl.splice(cl.end(),rl);
		cl.push_back(new Label_IC_Stmt(label, label2));	
		code = new Code_For_Ast(cl, red);
		
	}
	else{
		Code_For_Ast & cond_code = cond->compile();
		Code_For_Ast & l_code = then_part->compile();
		list<Icode_Stmt * > cl = cond_code.get_icode_list();
		list<Icode_Stmt * > ll = l_code.get_icode_list();

		string label1 = get_new_label();

		if(cond_code.get_reg() != NULL){
			cond_code.get_reg()->reset_use_for_expr_result();
			Register_Addr_Opd * opd_cond = new Register_Addr_Opd(cond_code.get_reg());
			cl.push_back(new Control_Flow_IC_Stmt(beq, opd_cond, NULL, label1));
		} else {
			Tgt_Op op, bcs;
			list<Icode_Stmt * >::iterator it = cl.end();
			it--;
			op = (*it)->get_op().get_op();
			if (op == sge_d || op == sgt_d || op == sne_d) bcs = bc1t;
			else bcs = bc1f;

			cl.push_back(new Label_IC_Stmt(bcs, label1));
		}
		
		cl.splice(cl.end(),ll);
		cl.push_back(new Label_IC_Stmt(label, label1));
		code = new Code_For_Ast(cl, red);
	}
	return *code;
}

Code_For_Ast & Iteration_Statement_Ast::compile(){
	Code_For_Ast * code = NULL;
	
	Code_For_Ast & cond_code = cond->compile();
	Code_For_Ast & l_code = body->compile();
	list<Icode_Stmt * > cl = cond_code.get_icode_list();
	list<Icode_Stmt * > ll = l_code.get_icode_list();
	string label1 = get_new_label();
	string label2 = get_new_label();

	list<Icode_Stmt * > ansl;
	if(!is_do_form)
		ansl.push_back(new Label_IC_Stmt(j, label2));
	ansl.push_back(new Label_IC_Stmt(label, label1));
	ansl.splice(ansl.end(),ll);
	ansl.push_back(new Label_IC_Stmt(label, label2));
	ansl.splice(ansl.end(),cl);
	
	if(cond_code.get_reg() != NULL){
		cond_code.get_reg()->reset_use_for_expr_result();
		Register_Addr_Opd * opd_cond = new Register_Addr_Opd(cond_code.get_reg());
		ansl.push_back(new Control_Flow_IC_Stmt(bne, opd_cond, NULL, label1));

	} else {
		Tgt_Op op, bcs;
		list<Icode_Stmt * >::iterator it = ansl.end();
		it--;
		op = (*it)->get_op().get_op();
		if (op == sge_d || op == sgt_d || op == sne_d) bcs = bc1f;
		else bcs = bc1t;

		ansl.push_back(new Label_IC_Stmt(bcs, label1));
	}
	
	code = new Code_For_Ast(ansl, NULL);
	return *code;
}

Code_For_Ast & Sequence_Ast::compile(){
	Code_For_Ast * code = NULL;
	for(list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end(); it++){
		Code_For_Ast & l_code = (*it)->compile();
		if(l_code.get_reg() != NULL)
			l_code.get_reg()->reset_use_for_expr_result();
		sa_icode_list.splice(sa_icode_list.end(),l_code.get_icode_list());
	}
	code = new Code_For_Ast(sa_icode_list, NULL);
	return *code;
}

// Also check spaces in Print -ast stmt
// print_assembly(), print_icode() functions in Sequence_Ast() ??
Code_For_Ast & Print_Ast::compile(){
	Code_For_Ast * code = NULL;
	Register_Descriptor * rnot, * rarg;
	list <Icode_Stmt * > ll;
	Tgt_Op op;

	Const_Opd<int> * con = NULL;
	if (node_data_type == int_data_type){
		con = new Const_Opd<int>(1);
		rarg = machine_desc_object.spim_register_table[a0];
		op = load;
	}
	else if (node_data_type == double_data_type){
		con = new Const_Opd<int>(3);
		rarg = machine_desc_object.spim_register_table[f12];
		op = load_d;
	}
	
	rnot = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(rnot);
	ll.push_back(new Move_IC_Stmt(imm_load, con, opd1));
	rnot->reset_use_for_expr_result();

	Register_Addr_Opd * opd2 = new Register_Addr_Opd(rarg);
	Mem_Addr_Opd * opd3 = new Mem_Addr_Opd(var->get_symbol_entry());

	ll.push_back(new Move_IC_Stmt(op, opd3, opd2));
	// rarg->reset_register_occupied();

	ll.push_back(new Label_IC_Stmt(Tgt_Op::print, "print"));

	code = new Code_For_Ast(ll, NULL);
	return *code;
}

Code_For_Ast & Return_Ast::compile(){
	Code_For_Ast * code = NULL;
	list<Icode_Stmt * > retl;
	if(return_value != NULL){
		Code_For_Ast & ret_code = return_value->compile();
		retl = ret_code.get_icode_list();
		Register_Addr_Opd * reg = new Register_Addr_Opd(ret_code.get_reg());
		Register_Addr_Opd * res = NULL;
		Tgt_Op op;
		if(node_data_type == int_data_type){
			res = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
			op = mov;
		}
		else {
			res = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
			op = move_d;
		}
		retl.push_back(new Move_IC_Stmt(op, reg, res));
		ret_code.get_reg()->reset_use_for_expr_result();
	}
	retl.push_back(new Label_IC_Stmt(j,"epilogue_"+proc_name));
	code = new Code_For_Ast(retl, NULL);
	return *code;
}

Code_For_Ast & Call_Ast::compile(){
	Code_For_Ast * code = NULL;
	list<Icode_Stmt * > retl;
	Register_Descriptor * reg;
	Register_Addr_Opd * res = NULL;
	int offset = 8;
	//int offset = 0;
	Tgt_Op op;
	
	Procedure * pr = program_object.get_procedure_prototype(procedure_name);
	list<Symbol_Table_Entry *> table_got =  pr->get_formal_param_list().get_table();
	list<Symbol_Table_Entry *>::iterator it_st = table_got.end();
	list<Ast *>::iterator it_ast = actual_param_list.end();

	while(it_ast != actual_param_list.begin()){
		--it_ast;
		--it_st;
		Code_For_Ast & param_code = (*it_ast)->compile();
		retl.splice(retl.end(), param_code.get_icode_list());

		string name = (*it_st)->get_variable_name();
		Symbol_Table_Entry * arg = new Symbol_Table_Entry(name, (*it_st)->get_data_type(), (*it_st)->get_lineno(), sp_ref);
		arg->set_start_offset(-offset);
		arg->set_symbol_scope(formal);

		if((*it_ast)->get_data_type() == int_data_type){
			offset += 4;
			op = store;
		}
		else {
			offset += 8;
			op = store_d;
		}
		retl.push_back(new Move_IC_Stmt(op, new Register_Addr_Opd(param_code.get_reg()), new Mem_Addr_Opd(*arg)));
		param_code.get_reg()->reset_use_for_expr_result();
	}

	reg = machine_desc_object.spim_register_table[sp];
	retl.push_back(new Compute_IC_Stmt(sub, new Register_Addr_Opd(reg), new Const_Opd<int>(offset), new Register_Addr_Opd(reg)));
	retl.push_back(new Label_IC_Stmt(jal, procedure_name));
	retl.push_back(new Compute_IC_Stmt(add, new Register_Addr_Opd(reg), new Const_Opd<int>(offset), new Register_Addr_Opd(reg)));

	if(get_data_type() != void_data_type){
		if(get_data_type() == int_data_type){
			return_value_reg = machine_desc_object.get_new_register<int_reg>();
			res = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
			op = mov;
		}
		else{
			return_value_reg = machine_desc_object.get_new_register<float_reg>();
			res = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
			op = move_d;
		}
		return_value_reg->set_use_for_expr_result();
		retl.push_back(new Move_IC_Stmt(op, res, new Register_Addr_Opd(return_value_reg)));
	}
	code = new Code_For_Ast(retl, return_value_reg);
	return *code;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast * code = NULL;
	return *code;
}
	

template class Number_Ast<double>;
template class Number_Ast<int>;

