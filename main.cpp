#include <iostream>
#include <locale>
#include <cctype>
#include <sstream>
#include <time.h>
#include <fstream>
using namespace std;

typedef struct participante{
	int id;
	string primeiro_nome;
	int semestre;
	int ano_ingresso;
	string curso;
	int pagante;
}PARTICIPANTE;

typedef struct node{
	node *proximo;
	PARTICIPANTE participante;
}NODE;

typedef struct comunidade{
	NODE *primeiro;
	NODE *ultimo;
}COMUNIDADE;

typedef struct nodep{
	int id;
	int mes;
	int ano;
	float valor;
	nodep *proximo;
}NODEP;

typedef struct pagante{
	NODEP *primeiro;
	NODEP *ultimo;
}PAGANTE;

void inicializa_comunidade(COMUNIDADE *comunidade);
void adiciona_participante(COMUNIDADE *comunidade);
int valida_semestre(const string &nome);
struct tm* retorna_data();
int valida_ingresso(const int &ano, const string &nome);
string valida_curso(const string &nome);
void imprime_participante(COMUNIDADE *comunidade, PAGANTE *pagante);
PARTICIPANTE* localiza_aluno(COMUNIDADE *comunidade, const int &valor);
void edita_aluno(NODE *aluno);
void chama_edita_aluno(NODE *aluno);
void inicializa_pagante(PAGANTE *pagante);
void adiciona_pagante(PAGANTE *pagante, COMUNIDADE *comunidade);
void grava_participantes(COMUNIDADE *comunidade);
void le_participantes(COMUNIDADE *comunidade);
void grava_contribuentes(PAGANTE *pagante);
void le_contribuentes(PAGANTE *pagante);
void grava_parcom(COMUNIDADE *comunidade, PAGANTE *pagante);
void grava_tudo(ofstream &arquivo, NODE *membro, PAGANTE *pagante);
void destroi_leak(PAGANTE *pagante, COMUNIDADE *comunidade);

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Portuguese");
	COMUNIDADE *comunidade = new COMUNIDADE;
	inicializa_comunidade(comunidade);
	PAGANTE *pagante = new PAGANTE;
	inicializa_pagante(pagante);
	le_participantes(comunidade);
	le_contribuentes(pagante);
	int menu = 0;
	string valida_menu;
	do{
		menu = 0;
		do{
			cout << "Selecione a opção que deseja: "<<endl;
			cout << "1 - Adicionar novo participante à comunidade"<<endl;
			cout << "2 - Editar um participante"<<endl;
			cout << "3 - Exibir todos os participantes"<<endl;
			cout << "4 - Cadastrar novo pagante"<<endl;
			cout << "5 - Gravar dados dos participantes"<<endl;
			cout << "6 - Encerrar à aplicação"<<endl;
			cin >> valida_menu;
			if(valida_menu.length() == 1){
				if(isdigit(valida_menu[0])){
					if(valida_menu != "1" && valida_menu != "2" && valida_menu != "3" && valida_menu != "4" && valida_menu != "5" && valida_menu != "6"){
						cerr << "Selecione uma opção do menu!!"<<endl;
					}else{
						stringstream converte(valida_menu);
						converte >> menu;
					}
				}else{
					cerr << "O valor deve ser numérico!!"<<endl;
				}
			}else{
				cerr << "O menu deve ter apenas um caracter!!"<<endl;
			}
		}while(menu == 0);
		switch(menu){
			case 1: 
				adiciona_participante(comunidade);
				break;
			case 2: 
				localiza_aluno(comunidade, 1);
				break;
			case 3:
				imprime_participante(comunidade, pagante);
				break;
			case 4:
				adiciona_pagante(pagante, comunidade);
				break;
			case 5: 
				grava_participantes(comunidade);
				grava_contribuentes(pagante);
				grava_parcom(comunidade, pagante);
				break;
		}
	}while(menu != 6);
	destroi_leak(pagante, comunidade);
	return 0;
}

void inicializa_comunidade(COMUNIDADE *comunidade){
	comunidade->primeiro = NULL;
	comunidade->ultimo = NULL;
}

void adiciona_participante(COMUNIDADE *comunidade){
	NODE *novo = new NODE;
	if(comunidade->primeiro == NULL){
		novo->participante.id = 1;
	}else{
		novo->participante.id = comunidade->ultimo->participante.id + 1;
	}
	cout << "digite o primeiro nome do participante: ";
	cin >> novo->participante.primeiro_nome;
	novo->participante.semestre = valida_semestre(novo->participante.primeiro_nome); // Recebe o retorno da função que valida o semestre
	struct tm *data_atual;
	data_atual = retorna_data();
	int ano = data_atual->tm_year+1900;
	novo->participante.ano_ingresso = valida_ingresso(ano, novo->participante.primeiro_nome); // Função para verificar se o ano que o participante ingressou está dentro do prazo de conclusão de curso
	novo->participante.curso = valida_curso(novo->participante.primeiro_nome); // Função para verificar se o curso do usuário é válido
	novo->participante.pagante = 0;
	if(comunidade->primeiro == NULL){
		comunidade->primeiro = novo;
	}else{
		comunidade->ultimo->proximo = novo;
	}
	novo->proximo = NULL;
	comunidade->ultimo = novo;
}

int valida_semestre(const string &nome){
	PARTICIPANTE participante;
	participante.semestre = 0;
	string valida_semestre;
	do{ // Validação para verificar se semestre é numero e se está entre 1 e 6
		cout << "Digite o semestre em que "<<nome<<" está: ";
		cin >> valida_semestre;
		if(valida_semestre.length() > 1){ // Verificação para ver a quantidade de caracteres
			cerr << "O semestre deve ter apenas um dígito!!"<<endl;
		}else{
			if(isdigit(valida_semestre[0])){ // Verificação para analisar se é numérico
				stringstream converte(valida_semestre);
				converte >> participante.semestre;
				if(!(participante.semestre >= 1 && participante.semestre <= 6)){ // Verificação para ver se está entre 1 e 6;
					cerr << "O aluno deve estar entre o primeiro e sexto semestre!!"<<endl;
					participante.semestre = 0;
				}
			}else{
				cerr << "O valor para o semestre deve ser numérico!!"<<endl;
			}
		}
	}while(participante.semestre == 0); // Fim da validação
	return participante.semestre;
}

struct tm* retorna_data(){
	struct tm *tempo; // Declara uma struct comumente usada para representar data
	time_t segundos; // Declara uma variável usada para armazenar segundos
	time(&segundos); // Armazena em segundos o tanto de segundo que passou desde 01/01/1970
	tempo = localtime(&segundos); // para o tm tempo a função retorna os dados de hora e data(para ano representa o tanto que passou-se desde 1900)
	return tempo;
}

int valida_ingresso(const int &ano, const string &nome){
	int retorno_ano = 0;
	string valida_ano;
	do{
		cout << "Digite o ano que o participante "<<nome<<" ingressou na Fatec: ";
		cin >> valida_ano;
		if(valida_ano.length() != 4){ // Verifica se o ano tem 4 dígitos, caso não, executa o do while novamente
			cerr << "O ano de ingresso deve ter 4 dígitos!!"<<endl;
		}else{
			bool quebra_if = true;
			for(int x = 0; x < valida_ano.length(); x++){ // Neste for verifica se todos os dígitos são números, caso não, para o for e executa o do while novamente
				if(!(isdigit(valida_ano[x]))){
					cout << "Todos os dígitos do ano deve ser numérico"<<endl;
					quebra_if = false;
				}
				if(!(quebra_if)){
					break;
				}
			}
			if(quebra_if){ // Se o if dentro do for não for executado, quebra_if será true que executará este if
				int ano_convertido;
				stringstream converte(valida_ano);
				converte >> ano_convertido; // converte a string para ano
				if(ano_convertido <= ano && ano_convertido >= ano - 5){ // verifica se está dentro do prazo de conclusão de curso
					retorno_ano = ano_convertido;
				}else{
					cerr << "O participante "<<nome<<" deve estar dentro do prazo máximo de 10 semestres para a conclusão do curso!!"<<endl;
					// Caso não esteja dentro do prazo, executa o do while novamente
				}
			}
		}
	}while(retorno_ano == 0);
	return retorno_ano;
}

string valida_curso(const string &nome){
	string curso;
	do{
		cout << "Digite o curso (DSM/SI/GE) do participante "<<nome<<": ";
		cin >> curso;
		for(int x = 0; x < curso.length(); x++){
			curso[x] = toupper(curso[x]); // Transforma todas as letras em maiusculas, para caso o usuario tenha digitado um curso válido conclua o do while
		}
		if(curso != "DSM" && curso != "GE" && curso != "SI"){
			cout << "O curso deve ser DSM, SI ou GE"<<endl; // Exibe mensagem para caso o cursjo não seja válido
		}
	}while(curso != "DSM" && curso != "GE" && curso != "SI");
	return curso;
}

void imprime_participante(COMUNIDADE *comunidade, PAGANTE *pagante){ // Função para imprimir participantes
	NODE *aux = comunidade->primeiro;
	while(aux != NULL){
		cout << "Número de identificação do aluno: "<<aux->participante.id<<endl;
		cout << "Nome do aluno: "<<aux->participante.primeiro_nome<<endl;
		cout << "Semestre do aluno: "<<aux->participante.semestre<<endl;
		cout << "Ano de ingresso do aluno: "<<aux->participante.ano_ingresso<<endl;
		cout << "Curso do aluno: "<<aux->participante.curso<<endl;
		if(aux->participante.pagante == 1){ // Verificando se é um pagante
			NODEP *pagantep = pagante->primeiro;
			while(pagantep->id != aux->participante.id){ // Se for um pagante vai percorrer a lista de pagantes até encontrar o nó que corresponde ao seu e imprimir
				pagantep = pagantep->proximo;
			}
			cout << "Mes da contribuição: "<<pagantep->mes<<endl;
			cout << "Ano da contribuição: "<<pagantep->ano<<endl;
			cout << "Valor da contribuição: "<<pagantep->valor<<endl;
		}
		aux = aux->proximo;
		cout <<"================"<<endl;
	}
}

PARTICIPANTE* localiza_aluno(COMUNIDADE *comunidade, const int &valor = 0){
	int id = 0;
	string valida_id;
	bool verifica_if = false;
	do{
		verifica_if = false;
		cout << "Digite o ID do participante: ";
		cin >> valida_id;
		for(int x = 0; x < valida_id.length(); x++){
			if(!(isdigit(valida_id[x]))){
				cerr << "O ID do participante deve ser numérico!!"<<endl;
				verifica_if = true;
				break;
			}
		} // Este for verifica se tem algum digito não numérico no id do participante, se tiver vai tornar verifica_if true, impedindo a execução do if abaixo, pedindo o id novamente
		if(!verifica_if){
			stringstream converte(valida_id); 
			converte >> id; 
			if(id == 0){
				cerr << "Não há participante com o ID 0!!"<<endl;
			}
		}
	}while(id == 0);
	NODE *percorre_participante = comunidade->primeiro;
	while(percorre_participante != NULL){
		if(percorre_participante->participante.id == id){
			if(valor == 1){
				edita_aluno(percorre_participante); // Passa o aluno para pode editar;
				return NULL;
			}
			return &percorre_participante->participante; // Retorna um id valido, permitindo adicionar um pagante
		}
		percorre_participante = percorre_participante->proximo;
	}
	cerr << "Não há um participante com este ID!!"<<endl;
	return NULL;
}

void edita_aluno(NODE *aluno){
	int menu = 0;
	string valida_menu;
	do{
		cout << "Digite o que deseja alterar do participante "<<aluno->participante.primeiro_nome<<":"<<endl;
		cout << "1 - Primeiro nome"<<endl;
		cout << "2 - Semestre"<<endl;
		cout << "3 - Ano de ingresso"<<endl;
		cout << "4 - Curso"<<endl;
		cout << "5 - Sair"<<endl;
		cin >> valida_menu;
		if(valida_menu != "1" && valida_menu != "2" && valida_menu != "3" && valida_menu != "4" && valida_menu != "5"){
			cerr << "Selecione uma opção válida!!"<<endl;
		}else{
			stringstream converte(valida_menu);
			converte >> menu;
		}
	}while(menu == 0); // Validação do menu, semelhante a do outro menu
	struct tm *data_atual = retorna_data();
	int ano = data_atual->tm_year + 1900; // Soma com 1900 pois ele retorna quantos anos se passou desde 1900
	switch(menu){
		case 1: 
			cout << "Digite o novo nome: ";
			cin >> aluno->participante.primeiro_nome;
			break;
		case 2: 
			cout << "Digite o novo semestre: ";
			aluno->participante.semestre = valida_semestre(aluno->participante.primeiro_nome);
			break;
		case 3:
			cout << "Digite o novo ano de ingresso: ";
			aluno->participante.ano_ingresso = valida_ingresso(ano, aluno->participante.primeiro_nome);
			break;
		case 4:
			cout << "Digite o novo curso do aluno: ";
			aluno->participante.curso = valida_curso(aluno->participante.primeiro_nome);
			break;
		case 5: 
			return;
	}
	chama_edita_aluno(aluno);
}

void chama_edita_aluno(NODE *aluno){
	string nova_alteracao;
	do{
		cout << "Deseja realizar outras alterações no "<<aluno->participante.primeiro_nome<<"? "<<endl;
		cout << "1 - Sim"<<endl;
		cout << "2 - Não"<<endl;
		cin >> nova_alteracao;
		if(nova_alteracao == "1"){
			edita_aluno(aluno);
		}else if(nova_alteracao == "2"){
			return;
		}else{
			cerr << "Selecione uma opção válida!!"<<endl;
		}
	}while(nova_alteracao != "1" && nova_alteracao != "2");
} // Função para verificar se quer editar mais coisas do aluno
void inicializa_pagante(PAGANTE *pagante){
	pagante->primeiro = NULL;
	pagante->ultimo = NULL;
}

void adiciona_pagante(PAGANTE *pagante, COMUNIDADE *comunidade){
	NODEP *novo = new NODEP;
	PARTICIPANTE *participante;
	int id = 0;
	int mes = 0;
	int ano = 0;
	do{
		participante = localiza_aluno(comunidade);
        if(participante != NULL){
			id = participante->id; 
		}
	}while(id == 0);
	novo->id = id;
	string valida_mes;
	bool verifica_if;
	do{
		cout << "Digite o mes da contribuição: "<<endl;
		cout << "1 - Janeiro"<<endl;
		cout << "2 - Fevereiro"<<endl;
		cout << "3 - Março"<<endl;
		cout << "4 - Abril"<<endl;
		cout << "5 - Maio"<<endl;
		cout << "6 - Junho"<<endl;
		cout << "7 - Julho"<<endl;
		cout << "8 - Agosto"<<endl;
		cout << "9 - Setembro"<<endl;
		cout << "10 - Outubro"<<endl;
		cout << "11 - Novembro"<<endl;
		cout << "12 - Dezembro"<<endl;
		cin >> valida_mes;
		if(valida_mes.length() <= 2){
			verifica_if = false;
			for(int x = 0; x < valida_mes.length(); x++){
				if(!(isdigit(valida_mes[x]))){
					cerr << "O mes deve ser numérico!!"<<endl;
					verifica_if = true;
					break;
				}
			}
			if(!verifica_if){
				stringstream converte(valida_mes);
				converte >> mes;
				if(mes < 1 || mes > 12){
					cerr << "O mes deve estar entre 1 e 12!!"<<endl;
					mes = 0;
				}
			}
		}else{
			cerr << "O mes deve ter no máximo 2 caracteres!!"<<endl;
		}
	}while(mes == 0);
	novo->mes = mes;
	struct tm *data_atual;
	data_atual = retorna_data();
	ano = data_atual->tm_year+1900;
	string valida_ano;
	do{
		verifica_if = false;
		cout << "Digite o ano da contribuição: ";
		cin >> valida_ano;
		if(valida_ano.length() != 4){
			cerr << "O ano da doação deve ter 4 dígitos!!"<<endl;
		}else{
			for(int x = 0; x < valida_ano.length(); x++){
				if(!(isdigit(valida_ano[x]))){
					cerr << "Todos os dígitos do ano devem ser numérico!!"<<endl;
					verifica_if = true;
					break;
				}
			}
			if(!verifica_if){
				stringstream converte(valida_ano);
				converte >> novo->ano;
				if(novo->ano < participante->ano_ingresso){
					cerr << "O ano da contribuição deve ser maior que o ano de ingresso!!"<<endl;
					novo->ano = 0;
				}else{	
					if(!(novo->ano <= ano && novo->ano >= ano - 5)){
						cerr << "A contribuição deve ter ocorrido dentro do prazo de 10 semestres para a conclusão do curso!!"<<endl;
						novo->ano = 0;	
					}
				}
			}
		}
	}while(novo->ano == 0);
	novo->valor = 0;
	string valor;
	int dot;
	do{
		dot = 0;
		verifica_if = false;
		cout << "Digite o valor da contribuição: ";
		cin >> valor;
		for(int x = 0; x < valor.length(); x++){
			if(!(isdigit(valor[x]))){
				dot++;
				if(valor[x] == ',' || valor[x] == '.'){
					valor[x] = '.';
				}
				if(dot > 1){
					cerr << "Formato de valor inválido!"<<endl;
					verifica_if = true;
					break;
				}
				if(valor[x] != ',' && valor[x] != '.'){
					cerr << "Não é permitido letras ou símbolos no valor (exceto ',' e '.')!!"<<endl;
					verifica_if = true;
					break;
				}
			}
		}
		if(!(verifica_if)){
			stringstream converte(valor);
			converte >> novo->valor;
		}
	}while(novo->valor == 0);
	participante->pagante = 1;
	if(pagante->primeiro == NULL){
		pagante->primeiro = novo;
	}else{
		pagante->ultimo->proximo = novo;
	}
	novo->proximo = NULL;
	pagante->ultimo = novo;
}

void grava_participantes(COMUNIDADE *comunidade){ // Função para gravar participantes
	NODE *membro = comunidade->primeiro;
	ofstream grava_dados("participantes.txt"); // Cria e abre o arquivo
	if(!(grava_dados.is_open())){
		cerr << "Não foi possível abrir o arquivo!!"<<endl;
		return;
	}
	while(membro != NULL){
		grava_dados << membro->participante.id << " " << membro->participante.primeiro_nome << " " << membro->participante.semestre << " " << membro->participante.ano_ingresso << " " << membro->participante.curso << " " << membro->participante.pagante<<endl;
		membro = membro->proximo;
	}
	grava_dados.close();
}

void grava_contribuentes(PAGANTE *pagante){
	NODEP *contribuente = pagante->primeiro;
	ofstream grava_dados("contribuentes.txt");
	if(!(grava_dados.is_open())){
		cerr << "Não foi possível abrir o arquivo!!"<<endl;
		return;
	}
	while(contribuente != NULL){
		grava_dados << contribuente->id << " " << contribuente->mes << " " << contribuente->ano << " " << contribuente->valor<<endl;
		contribuente = contribuente->proximo;
	}
	grava_dados.close();
}

void grava_parcom(COMUNIDADE *comunidade, PAGANTE *pagante){
	NODE *membro = comunidade->primeiro;
	ofstream dsm("contribuentes_DSM.txt");
	ofstream si("contribuentes_SI.txt");
	ofstream ge("contribuentes_GE.txt");
	if(!(dsm.is_open() || si.is_open() || ge.is_open())){
		cerr << "Não foi possível abrir o arquivo!!"<<endl;
		return;
	}
	while(membro != NULL){
		if(membro->participante.pagante == 1){
			if(membro->participante.curso == "DSM"){
				grava_tudo(dsm, membro, pagante);
			}else if(membro->participante.curso == "SI"){
				grava_tudo(si, membro, pagante);
			}else{
				grava_tudo(ge, membro, pagante);
			}
		}
		membro = membro->proximo;
	}
	dsm.close();
	si.close();
	ge.close();
}

void grava_tudo(ofstream &arquivo, NODE *membro, PAGANTE *pagante){
	NODEP *contribuente = pagante->primeiro;
	while(membro->participante.id != contribuente->id){
		contribuente = contribuente->proximo;
	}
	arquivo << membro->participante.id << " " << membro->participante.primeiro_nome << " " << membro->participante.semestre << " " << membro->participante.ano_ingresso << " " << contribuente->mes << " " << contribuente->ano << " " << contribuente->valor<<endl;
}

void le_participantes(COMUNIDADE *comunidade){
	ifstream le_dados("participantes.txt");
	if(!(le_dados.is_open())){
		return;
	}
	string membro;
	while(getline(le_dados, membro)){
		NODE *novo = new NODE;
		istringstream converte(membro);
		converte >> novo->participante.id >> novo->participante.primeiro_nome >> novo->participante.semestre >> novo->participante.ano_ingresso >> novo->participante.curso >> novo->participante.pagante;
		if(comunidade->primeiro == NULL){
			comunidade->primeiro = novo;
		}else{
			comunidade->ultimo->proximo = novo;
		}
		novo->proximo = NULL;
		comunidade->ultimo = novo;
	}
	le_dados.close();
}

void le_contribuentes(PAGANTE *pagante){
	ifstream le_dados("contribuentes.txt");
	if(!(le_dados.is_open())){
		return;
	}
	string contribuente;
	while(getline(le_dados, contribuente)){
		NODEP *novo = new NODEP;
		istringstream converte(contribuente);
		converte >> novo->id >> novo->mes >> novo->ano >> novo->valor;
		if(pagante->primeiro == NULL){
			pagante->primeiro = novo;
		}else{
			pagante->ultimo->proximo = novo;
		}
		novo->proximo = NULL;
		pagante->ultimo = novo;
	}
	le_dados.close();
}

void destroi_leak(PAGANTE *pagante, COMUNIDADE *comunidade){
	NODE *node = comunidade->primeiro;
	while(node != NULL){
		NODE *temp = node;
		node = node->proximo;
		delete temp;
	}
	delete comunidade;
	NODEP *nodep = pagante->primeiro;
	while(nodep != NULL){
		NODEP *temp = nodep;
		nodep = nodep->proximo;
		delete temp;
	}
	delete pagante;
}


