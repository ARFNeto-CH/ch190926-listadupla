#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct  aluno
{
	char	nome[80];
	char	cpf[50];
};
typedef struct aluno		Aluno;

struct no
{
	Aluno* aluno;
	struct no* ant;
	struct no* prox;
};
typedef struct no			No;

struct lista
{
	No* inicio;
	char* nome;
	int		quantos;
};
typedef struct lista		Cadastro;

int				compara_criterio_asc(Aluno*, Aluno*);
Cadastro*		cria_cadastro(char*);
int				da_um_fim_na_lista(Cadastro*);
int				insere_aluno(Aluno*, Cadastro*);
static Aluno*	le_aluno();
int				lista_cadastro(Cadastro*);

int main()
{
	Cadastro* cadastro = NULL;	// a lista
	Aluno* aluno_lido = NULL;
	int			total = 0;
	int			r = 0;

	cadastro = cria_cadastro("Novo Cadastro");
	aluno_lido = le_aluno();
	r = insere_aluno(aluno_lido, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", aluno_lido->nome, aluno_lido->cpf, r);
	aluno_lido = le_aluno();
	r = insere_aluno(aluno_lido, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", aluno_lido->nome, aluno_lido->cpf, r);
	aluno_lido = le_aluno();
	r = insere_aluno(aluno_lido, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", aluno_lido->nome, aluno_lido->cpf, r);
	lista_cadastro(cadastro);
	// testa para o mesmo
	r = insere_aluno(aluno_lido, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", aluno_lido->nome, aluno_lido->cpf, r);
	lista_cadastro(cadastro);

	// testando para alunos com o mesmo nome: deve classificar por CPF
	Aluno a1, a2;
	strcpy(a1.nome, "Teste");
	strcpy(a2.nome, "Teste");
	strcpy(a1.cpf, "111.222");
	strcpy(a2.cpf, "222.333");

	r = insere_aluno(&a1, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", a1.nome, a1.cpf, r);

	r = insere_aluno(&a2, cadastro);
	printf("insere('%s'-'%s') retornou %d\n", a2.nome, a2.cpf, r);
	lista_cadastro(cadastro);

	return 0;
}	// end main()


int				compara_criterio_asc(Aluno* a, Aluno* b)
{
	int i = 0;
	printf(
		"comparando [%s:%s] e [%s:%s]\n",
		a->nome, a->cpf,
		b->nome, b->cpf);
	// se tem o mesmo nome classifica pelo cpf
	i = strcmp(a->nome, b->nome);
	if (i == 0)
	{
		return (strcmp(a->cpf, b->cpf));
	};
	return i;	// compara por nome apenas
}	// end compara_riterio_asc()


Cadastro* cria_cadastro(char* nome)
{

	// cria cadastro vazio e devolve o endereco
	char* nome_padrao = "Lista";
	Cadastro* p = NULL;
	char* n = NULL;

	p = (Cadastro*)malloc(sizeof(Cadastro));
	if (p == NULL) return NULL;	// deu m.
	n = (char*)malloc(80);
	if (n == NULL)
	{
		free(p);
		return NULL;	// deu m.
	}

	if ((nome == NULL) || (strlen(nome) == 0))
	{
		strcpy(n, nome_padrao);
	}
	else
	{
		strcpy(n, nome);
	}

	p->inicio = NULL;
	p->nome = n;
	p->quantos = 0;

	printf("***** Criado cadastro [%s]\n", p->nome);
	return p;
}	// end cria_cadastro()


int				da_um_fim_na_lista(Cadastro* cadastro)
{
	return 0;
}	// end da_um_fim_na_lista()


int				insere_aluno(Aluno* aluno, Cadastro* cadastro)
{
	int			rel = 0;	// vai guardar o resultado da comparacao
	No* p = NULL;	// p e o ponteiro para a lista aqui
	No* pNovo = NULL;

	if (cadastro == NULL) return -1;	// sem cadastro
	if (aluno == NULL) return -2;		// sem aluno

	printf(
		"***** insere('%s':'%s')\n",
		aluno->nome,
		aluno->cpf
	);
	p = cadastro->inicio;
	if (p == NULL)			// vazio ainda
	{
		p = (No*)malloc(sizeof(No));
		p->aluno = aluno;
		p->ant = NULL;
		p->prox = NULL;
		cadastro->quantos = 1;
		cadastro->inicio = p;
		return cadastro->quantos;
	}	// end if
	// ok tem alguem. Segue a lista ate achar o lugar para inserir
	// 'aluno'			e o cara pra inserir
	// 'cadastro->c		e o primeiro aluno na lista
	//
	// vamos comparar os dois alunos segundo o criterio desejado
	// fora daqui assim podemos mudar o criterio ou testar o criterio
	// em separado... a comparacao retorna 
	// <0 - se o aluno vai ficar antes desse na lista
	//  0 - se os alunos tem a mesma posicao
	// >0 - se o aluno a inserir vai ficar depois desse na lista
	//
	// se sao iguais insere o novo depois para manter 
	// na ordem de entrada o mais novo depois
	//
	// antes de tudo: sigo a lista enquanto --- while --- nao achar
	// uma posicao de valor maior ou igual a do aluno que vou inserir
	// ex numerico: tenho 1 2 3 4 e vou inserir 3 sigo a lista ate 3
	// e ai vai depender da minha necesidade e disciplina
	// se a lista nao aceita duplicidades ao encontrar um igual apenas
	// retorna. Mas se aceita pode ser que eu precise manter a ordem
	// com o mais antigo antes ou ao contrario com o mais antigo depois
	//
	// mas pode ser que esse elemento a va ser o ultimo da lista...
	//
	do
	{

		rel = compara_criterio_asc(aluno, p->aluno);	// retorna -1 0 ou 1

		if (rel == 0)
		{	// tem a mesma posicao na sequencia
			return cadastro->quantos;	// nesse cadastro nao tem duplicidade	
		}

		if (rel > 0)
		{	// o aluno novo vem depois na sequencia
			if (p->prox == NULL)
			{	// fim da linha: vai inserir ao final
				No* pNovo = (No*)malloc(sizeof(No));	// cria novo no
				pNovo->aluno = aluno;					// insere o aluno
				pNovo->ant = p;
				p->prox = pNovo;
				pNovo->prox = NULL;	// novo fim da lista
				cadastro->quantos += 1;
				return cadastro->quantos;
			}
			p = p->prox;	// segue adiante
			continue;
		}
		// rl < 0 ==> vai inserir aqui 
		No* pNovo = (No*)malloc(sizeof(No));	// cria novo no
		pNovo->aluno = aluno;					// insere o aluno
		pNovo->ant = p->ant;
		pNovo->prox = p;						// antes de p
		if (p->ant != NULL)
		{	// p nao era o primeiro entao o anterior a p
			// vai ser seguido por pNovo
			(p->ant)->prox = pNovo;
		}
		else
		{	// esse vai ser o novo inicio
			cadastro->inicio = pNovo;
		}
		p->ant = pNovo;	// novo fim da lista
		cadastro->quantos += 1;
		return cadastro->quantos;

	} while (1);
}	// end insere_aluno()


static Aluno* le_aluno()
{
	// essa rotina pode salvar horas: a cada vez que e chamada
	// um aluno bem comportado com nome a partir de
	// "Aluno-0000" e CPF "123.456.7XY- ZT" 
	// limitando o teste a apenas 10.000 alunos :(
	// so que não precisa ler nenhum
	static int		quantos = -1;
	char* pref_nome = "Aluno-";
	char* pref_CPF = "123.456.7";
	char			valor[50];

	quantos++;
	int X = quantos / 1000;
	int Y = (quantos % 1000) / 100;
	int Z = (quantos % 100) / 10;
	int T = quantos % 10;
	// monta o nome do aluno
	Aluno* pAluno = (Aluno*)malloc(sizeof(Aluno));
	if (pAluno == NULL) return NULL;
	sprintf(valor, "%s%d%d%d%d", pref_nome, X, Y, Z, T);
	strcpy(pAluno->nome, valor);
	sprintf(valor, "%s%1d%1d-%1d%1d", pref_CPF, X, Y, Z, T);
	strcpy(pAluno->cpf, valor);
	return pAluno;
}	// end le_aluno()


int				lista_cadastro(Cadastro* cadastro)
{
	int n = 0;
	if (cadastro == NULL)
	{
		printf("Cadastro nao existe\n");
		return -1;
	}
	No* p = cadastro->inicio;	// inicio da lista
	if (p == NULL)
	{
		printf("Cadastro vazio\n");
		return -1;
	}
	printf(
		"Lista [%s] com %d alunos\n",
		cadastro->nome,
		cadastro->quantos
	);
	while (p != NULL)
	{
		n += 1;
		printf("%4d:'%s'-'%s'\n", n, p->aluno->nome, p->aluno->cpf);
		p = p->prox;
	};
	return 0;
};	// end lista_cadastro()
