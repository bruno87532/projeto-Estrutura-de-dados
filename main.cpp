#include <iostream>
#include <locale>
#include <cctype>
#include <sstream>
#include <time.h>
using namespace std;

typedef struct participante{
	int id;
	string primeiro_nome;
	int semestre;
	int ano_ingresso;
	string curso;
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
int valida_ingresso(const int &ano, const string &nome, int valor);
string valida_curso(const string &nome);
void imprime_participante(COMUNIDADE *comunidade);
int localiza_aluno(COMUNIDADE *comunidade, int valor);
void edita_aluno(NODE *aluno);
void chama_edita_aluno(NODE *aluno);
void inicializa_pagante(PAGANTE *pagante);
void adiciona_pagante(PAGANTE *pagante, COMUNIDADE *comunidade);

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Portuguese");
	COMUNIDADE *comunidade = new COMUNIDADE;
	inicializa_comunidade(comunidade);
	PAGANTE *pagante = new PAGANTE;
	inicializa_pagante(pagante);
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
			cout << "5 - Encerrar à aplicação"<<endl;
			cin >> valida_menu;
			if(valida_menu.length() == 1){
				if(isdigit(valida_menu[0])){
					if(valida_menu != "1" && valida_menu != "2" && valida_menu != "3" && valida_menu != "4" && valida_menu != "5"){
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
				imprime_participante(comunidade);
				break;
			case 4:
				adiciona_pagante(pagante, comunidade);
				break;
		}
	}while(menu != 5);
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
	novo->participante.ano_ingresso = valida_ingresso(ano, novo->participante.primeiro_nome, 1); // Função para verificar se o ano que o participante ingressou está dentro do prazo de conclusão de curso
	novo->participante.curso = valida_curso(novo->participante.primeiro_nome); // Função para verificar se o curso do usuário é válido
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

int valida_ingresso(const int &ano, const string &nome = "", int valor = 0){
	int retorno_ano = 0;
	string valida_ano;
	do{
		if(valor == 1){
			cout << "Digite o ano que o participante "<<nome<<" ingressou na Fatec: ";
		}else{
			cout << "Digite o ano da contribuição do participante: ";
		}
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
					if(valor == 1){
						cerr << "O participante "<<nome<<" deve estar dentro do prazo máximo de 10 semestres para a conclusão do curso!!"<<endl;
					}else{
						cout << "A contribuição do participante deve estar entre o prazo que o participante permaneceu na FATEC!!"<<endl;
					}
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
		cout << "Digite o curso (DSM/GE) do participante "<<nome<<": ";
		cin >> curso;
		for(int x = 0; x < curso.length(); x++){
			curso[x] = toupper(curso[x]); // Transforma todas as letras em maiusculas, para caso o usuario tenha digitado um curso válido conclua o do while
		}
		if(curso != "DSM" && curso != "GE"){
			cout << "O curso deve ser DSM ou GE"<<endl; // Exibe mensagem para caso o cursjo não seja válido
		}
	}while(curso != "DSM" && curso != "GE");
	return curso;
}

void imprime_participante(COMUNIDADE *comunidade){
	NODE *aux = comunidade->primeiro;
	while(aux != NULL){
		cout << "Número de identificação do aluno: "<<aux->participante.id<<endl;
		cout << "Nome do aluno: "<<aux->participante.primeiro_nome<<endl;
		cout << "Semestre do aluno: "<<aux->participante.semestre<<endl;
		cout << "Ano de ingresso do aluno: "<<aux->participante.ano_ingresso<<endl;
		cout << "Curso do aluno: "<<aux->participante.curso<<endl;
		aux = aux->proximo;
		cout <<"================"<<endl;
	}
}

int localiza_aluno(COMUNIDADE *comunidade, int valor = 0){
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
		}
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
				edita_aluno(percorre_participante);
			}
			return id;
		}
		percorre_participante = percorre_participante->proximo;
	}
	cerr << "Não há um participante com este ID!!"<<endl;
	return 0;
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
	}while(menu == 0);
	struct tm *data_atual = retorna_data();
	int ano = data_atual->tm_year + 1900;
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
			aluno->participante.ano_ingresso = valida_ingresso(ano, aluno->participante.primeiro_nome, 1);
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
}

void inicializa_pagante(PAGANTE *pagante){
	pagante->primeiro = NULL;
	pagante->ultimo = NULL;
}

void adiciona_pagante(PAGANTE *pagante, COMUNIDADE *comunidade){
	NODEP *novo = new NODEP;
	int id = 0;
	int mes = 0;
	int ano = 0;
	do{
		id = localiza_aluno(comunidade);
	}while(id == 0);
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
				cin >> mes;
				if(mes < 1 || mes > 12){
					cerr << "O mes deve estar entre 1 e 12!!"<<endl;
					mes = 0;
				}
			}
		}else{
			cerr << "O mes deve ter no máximo 2 caracteres!!"<<endl;
		}
	}while(mes == 0);
	struct tm *data_atual;
	data_atual = retorna_data();
	ano = data_atual->tm_year+1900;
	novo->ano = valida_ingresso(ano);
}

