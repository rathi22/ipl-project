Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	inst_op = op;
	mnemonic = mnn;
	ic_symbol = ics;
	ic_format = icf;
	assem_format = af;
	this->name = name;
}

Instruction_Descriptor::Instruction_Descriptor(){}

Tgt_Op Instruction_Descriptor::get_op(){
	return inst_op;
}

string Instruction_Descriptor::get_name(){
	return name;
}

string Instruction_Descriptor::get_mnemonic(){
	if(inst_op == sge_d) return "c.lt.d";
	else if(inst_op == sgt_d) return "c.le.d";
	else if(inst_op == sne_d) return "c.eq.d";
	else return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
	return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return assem_format;
}
 
void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
	if(inst_op == sge_d) file_buffer << "slt.d" << ":";
	else if(inst_op == sgt_d) file_buffer << "sle.t" << ":";
	else if(inst_op == sne_d) file_buffer << "seq.d" << ":";
	else file_buffer << name << ":" ;
}

/*------------------------------------------------------------------------------*/

Register_Descriptor * Ics_Opd::get_reg(){}

/*------------------------------------------------------------------------------*/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	if(symbol_entry->get_symbol_scope() == global)
		file_buffer << symbol_entry->get_variable_name();
	else{
		file_buffer << symbol_entry->get_start_offset();
		if(symbol_entry->get_ref_offset() == fp_ref) file_buffer << "($fp)";
		else file_buffer << "($sp)";	
	}
}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

/*------------------------------------------------------------------------------*/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer << "$" << register_description->get_name();	
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	this->register_description = rhs.register_description;
	return *this;
}

/*------------------------------------------------------------------------------*/

template <class T>
Const_Opd<T>::Const_Opd (T num){
	this->num = num;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	file_buffer << num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer << num;	
}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){
	this->num = rhs.num;
	return *this;
}

/*------------------------------------------------------------------------------*/

///////////////////////////////// Intermediate code statements //////////////////////////
	
Instruction_Descriptor & Icode_Stmt::get_op(){
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){}
Ics_Opd * Icode_Stmt::get_opd2(){}
Ics_Opd * Icode_Stmt::get_result(){}

void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}

/*------------------------------------------------------------------------------*/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	this->opd1 = opd1;
	this->result = result;
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	this->op_desc = rhs.op_desc;
	this->opd1 = rhs.opd1;
	this->result = rhs.result;
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return result;
}

void Move_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "\t";
	op_desc.print_instruction_descriptor(file_buffer);
	file_buffer << "    \t";
	result->print_ics_opd(file_buffer);
	file_buffer << " <- ";
	opd1->print_ics_opd(file_buffer);
	file_buffer << "\n";
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\t" << op_desc.get_mnemonic();
	file_buffer  << " ";
	if(op_desc.get_op() == uminus || op_desc.get_op() == uminus_d){
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else if(op_desc.get_op() == store || op_desc.get_op() == store_d){
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";	
	}	
}

/*------------------------------------------------------------------------------*/

// TODO -> write code for other Icode_Stmt derived classes

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->result = rhs.result;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return opd1;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return opd2;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "\t";
	op_desc.print_instruction_descriptor(file_buffer);
	file_buffer << "    \t";
	if(result != NULL){
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
	}
	opd1->print_ics_opd(file_buffer);
	file_buffer << " , ";
	opd2->print_ics_opd(file_buffer);
	file_buffer << "\n";
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\t" << op_desc.get_mnemonic();
	file_buffer  << " ";
	if(result != NULL){
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
	}
	opd1->print_asm_opd(file_buffer);
	file_buffer << ", ";
	opd2->print_asm_opd(file_buffer);
	file_buffer << "\n";
}

/*------------------------------------------------------------------------------*/

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label, int size){
	this->opd1 = o1;
	this->opd2 = o2;
	this->offset = label;
	this->actual_param_size = size;
	this->op_desc = *machine_desc_object.spim_instruction_table[op];
}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->actual_param_size = rhs.actual_param_size;
	this->offset = rhs.offset;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
	return opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
	return opd2;
}

void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}
 	
string Control_Flow_IC_Stmt::get_Offset(){
	return offset;
}

void Control_Flow_IC_Stmt::set_Offset(string label){
	offset = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "\t";
	op_desc.print_instruction_descriptor(file_buffer);
	file_buffer << "    \t";
	opd1->print_ics_opd(file_buffer);
	file_buffer << " , zero : goto ";
	file_buffer << offset << "\n";
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\t" << op_desc.get_mnemonic();
	file_buffer  << " ";
	opd1->print_asm_opd(file_buffer);
	file_buffer << ", $zero, ";
	file_buffer << offset << " \n";
}

/*------------------------------------------------------------------------------*/

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label) {
	this->label = label;
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs) {
	this->label = rhs.label;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics() {
	return op_desc;
}

string Label_IC_Stmt::get_label(){
	return label;
}

void Label_IC_Stmt::set_label(string label){
	this->label = label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	if(op_desc.get_name() == "jump")
		file_buffer << "\tgoto " << label <<"\n";
	else if(op_desc.get_name() == "print")
		file_buffer << "\t" << op_desc.get_name() << "\n";
	else if(op_desc.get_name() == "")
		file_buffer << "\n" << label << ":    \t\n";
	else 
		file_buffer << "\t" << op_desc.get_name() << " " << label <<"\n";
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){
	if(op_desc.get_name() == "jump")
		file_buffer << "\tj " << label <<"\n";
	else if(op_desc.get_name() == "print")
		file_buffer << "\t" << op_desc.get_mnemonic() << "\n";
	else if(op_desc.get_name() == "")
		file_buffer << "\n" << label << ":    \t\n";
	else 
		file_buffer << "\t" << op_desc.get_name() << " " << label <<"\n";	
}

//////////////////////// Intermediate code for Ast statements ////////////////////////

Code_For_Ast::Code_For_Ast(){
	result_register = NULL;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics){
	ics_list.push_back(&ics);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg){
	result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	this->result_register = rhs.result_register;
	this->ics_list = rhs.ics_list;
	return *this;
}

/*------------------------------------------------------------------------------*/

template class Const_Opd<double>;
template class Const_Opd<int>;
