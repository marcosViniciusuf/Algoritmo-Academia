/*
 * SISTEMA DE CADASTRO E GERENCIAMENTO DE EXERCÍCIOS DE MUSCULAÇÃO
 * AUTORES:
 * MARCOS VINÍCIUS DA SILVA SOUZA - 
 * NÉSTOR JESÚS NAPOLES GRAVE DE PERALTA - 202520162
 * PEDRO HENRIQUE RESENDE DOS SANTOS - 202520153
*/

#include <iostream>
#include <fstream>
#include <limits>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

//Struct para representar o exércicio
struct Exercicios{
    string nome_exercicio;
    string musculos_alvo;
    short series;
    float carga;
    string descricao;
};  

// ==============================================================
// INTERFACE: Funções que ajudam a gerar a interface da aplicação.
// ==============================================================

//Define as cores dos textos
void setColor(int tipo) {
    switch (tipo) {
        
        case 1: // título (azul)
        cout << "\033[34m";
        break;

        case 2: // erro (vermelho)
            cout << "\033[31m";
            break;

        case 3: // sucesso (verde)
            cout << "\033[32m";
            break;

        case 4: // destaque (ciano)
            cout << "\033[96m";
            break;

        case 0: // reset
        default:
            cout << "\033[0m";
            break;
    }
}

//Centraliza o header do sistema
void centralizar(string texto) {
    const int largura = 80; // ajuste se quiser
    int espacos = (largura - texto.length()) / 2;
    if (espacos < 0) espacos = 0;

    for (int i = 0; i < espacos; i++)
        cout << ' ';

    cout << texto << endl;
}

//Espera o usuário apertar ENTER para prosseguir
void pausar() {
	setColor(4);
    cout << "Pressione ENTER para continuar...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    setColor(0);
}

//Exibe a animação de loading (...)
void animacao(string texto) {
    pausar();

	setColor(4);
    int ciclos = 3, delay = 300000;
    // delay = tempo entre cada atualização (em microsegundos)
    
    

    for (int i = 1; i <= ciclos; i++) {
        cout << "\r" << texto;          // volta para o início da linha
        cout << string(i, '.');         
        cout << flush;                   // força impressão imediata
        usleep(delay);                   // pausa
    }

    // limpa a linha depois da animação
    cout << "\r" << string(texto.size() + ciclos, ' ') << "\r" << flush;
    setColor(0);
}

//Exibe o menu inicial
void interface_inicial() {
    cout<< endl;
	setColor(4);
	centralizar("=================== MENU PRINCIPAL ===================\n");
	setColor(0);

    cout<< "Escolha uma ação:" << endl
        << "-----------------------------------------" << endl
        << " (1) Listar índices dos exercícios" << endl
        << " (2) Adicionar novo exercício" << endl
        << " (3) Editar exercício existente" << endl
        << " (4) Remover exercício" << endl
        << " (5) Procurar exercício" << endl
        << " (6) Mostrar todos os exercícios" << endl
        << " (7) Mostrar exercícios em um intervalo" << endl
        << " (8) Ordenar a lista de exercícios"<<endl
        << "-----------------------------------------" << endl
        << " (9) Ajuda" << endl
        << " (0) Encerrar aplicação" << endl
        << "-----------------------------------------" << endl;
}

//Exibe o menu de ordenação
void interface_ordenacao(){
    cout<< endl;
		setColor(4);
		centralizar("=================== MENU DE ORDENAÇÃO ===================\n");
		setColor(0);

    cout << "Qual critério deseja usar para ordenar?\n"
        << "-----------------------------------------\n"
        << " (1) Nome do exercício\n"
        << " (2) Grupos musculares\n"
        << " (3) Carga (kg)\n"
        << "-----------------------------------------\n"
        << " (0) Voltar\n"
        << "-----------------------------------------\n";
}

//Exibe o menu de busca
void interface_busca(){
    system("clear");

    cout<< endl;
		setColor(4);
		centralizar("=================== MENU DE BUSCA ===================\n");
		setColor(0);

    cout << "Por qual critério deseja procurar?\n"
        << "-----------------------------------------\n"
        << " (1) Nome do exercício\n"
        << " (2) Grupos musculares\n"
        << "-----------------------------------------\n"
        << " (0) Voltar\n"
        << "-----------------------------------------\n";
}

//Exibe o menu de ajuda
void interface_help() {
    cout<< endl;
		setColor(4);
		centralizar("=================== MENU DE AJUDA ===================\n");
		setColor(0);

    cout<< "Selecione o comando para ver detalhes:" << endl
        << "-----------------------------------------" << endl
        << " (1) Listar índices dos exercícios" << endl
        << " (2) Adicionar novo exercício" << endl
        << " (3) Editar exercício existente" << endl
        << " (4) Remover exercício" << endl
        << " (5) Procurar exercício" << endl
        << " (6) Mostrar todos os exercícios" << endl
        << " (7) Mostrar exercícios em um intervalo" << endl
        << " (8) Ordenar a lista de exercícios"<<endl
        << "-----------------------------------------" << endl
        << " (0) Voltar" << endl
        << "-----------------------------------------" << endl;
}

//Exibe os exercícios em um intervalo
void mostrar_intervalo(const Exercicios *exercicio, int inicio, int fim){
    for (int i = inicio; i <= fim; i++){
        cout << "Exércicio: " << exercicio[i].nome_exercicio << endl
             << "Grupos musculares: " << exercicio[i].musculos_alvo << endl
             << "Numero de séries: " << exercicio[i].series << "x" << endl
             << "Carga: " << exercicio[i].carga << "kg" << endl
             << "Descrição do exércicio: " << exercicio[i].descricao << endl
             << "---------------------------" << endl;
    }
}

//Exibe o menu de edição
void interface_edicao(Exercicios *exercicio, int indice){
    cout<< endl;
	setColor(4);
	centralizar("=================== MENU DE EDIÇÃO ===================\n");
	setColor(0);
    
    cout<< "Informações do exercício: "<<endl<<endl;
    mostrar_intervalo(exercicio, indice, indice);

    cout<< "Qual informação deseja atualizar?\n"
        << "-----------------------------------------\n"
        << " (1) Nome do exercício\n"
        << " (2) Grupos musculares\n"
        << " (3) Séries\n"
        << " (4) Carga (kg)\n"
        << " (5) Descrição\n"
        << "-----------------------------------------\n"
        << " (0) Voltar\n"
        << "-----------------------------------------\n";
}

//Exibe uma mensagem caso o processo funcione corretamente
void mensagem_sucesso(const string &msg) {
    setColor(3);               // Verde 
    cout << msg << endl;
    setColor(0);               
}

//Exibe uma mensagem caso ocorra algum erro
void mensagem_erro(const string &msg){
    setColor(2);               // vermelho
    cout << msg << endl;
    setColor(0);               // padrão
}

//=============================================
//FUNÇÕES PARA O TRATAMENTO DE ERROS E EXCESSÕES.
//=============================================

//Verifica se a resposta inserida é um caractere válido (S/N)
char verificar_caractere(){   
    bool valido=false;
    string entrada;

    while (!valido){
        cin >> entrada;
        if (entrada=="S" or entrada=="s" or entrada=="N" or entrada=="n"){
            valido=true;
        }
        else {
            mensagem_erro("Resposta inválida, digite um caractere válido");
        }
    }

    return entrada[0];
}

//Converte uma string para letras minúsculas (case insensitive)
string toLower(const string &s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

//Verifica se o valor float lido é válido
float ler_float(){

    string aux;
    bool valido=false;
    
    while(!valido) {
        getline(cin, aux);
        valido=true;
        int n_ponto=0;

        int tamanho_aux=aux.length();

        // impede string vazia
        if(tamanho_aux == 0){
            valido = false;
        }

        if(tamanho_aux > 4){
            valido = false;
        }

        //impede começar ou terminar com ponto
        if (aux[tamanho_aux-1] == '.' or aux[0] == '.') {
            valido = false;
        }

        //valida se há apenas números e no máximo um ponto
        for (int i=0; i<tamanho_aux; i++) {
            if (aux[i]=='.') n_ponto++;
            if ((aux[i]<'0' or aux[i]>'9') and aux[i]!='.') valido = false;
            if (aux[i]==' ') valido=false;
        }

        if (n_ponto > 1) valido = false;
        
        if (!valido) mensagem_erro("Valor inválido! Digite números reais com um tamanho plausível e sem espaços (use ponto para decimais, pontos decimais não são aceitos no início ou final): ");
    }

    return stof(aux);
}

//Verifica se o valor inteiro lido é válido
int ler_inteiro(){ 
    string aux;
    bool valido = false;

    while(!valido){
        getline(cin, aux);
        valido = true;

        int tamanho_aux = aux.length();

        // impede string vazia
        if(tamanho_aux == 0){
            valido = false;
        }

        if(tamanho_aux > 3){
            valido = false;
        }

        else if(aux[0] == '-'){
            if(tamanho_aux == 1){
                valido = false; 
            }else{
                for(int i = 1; i < tamanho_aux; i++){
                    if(aux[i] < '0' || aux[i] > '9') valido = false;
                    if(aux[i] == ' ') valido = false;
                }
            }
        }
        else{
            for(int i = 0; i < tamanho_aux; i++){
                if(aux[i] < '0' || aux[i] > '9') valido = false;
            }
        }

        if(!valido){
            mensagem_erro("Valor inválido! Digite números inteiros com um tamanho plausível, sem espaços: ");
        }
    }

    return stoi(aux);
}

//Verificação de inteiro que exibe mensagem especial se o valor inteiro lido é válido,usada nas funçoes de leitura para os menus
int ler_inteiro_comandos(){ 
    string aux;
    bool valido = false;

    while(!valido){
        getline(cin, aux);
        valido = true;

        int tamanho_aux = aux.length();

        // impede string vazia
        if(tamanho_aux == 0){
            valido = false;
        }
        else if(aux[0] == '-'){
            if(tamanho_aux == 1){
                valido = false; 
            }else{
                for(int i = 1; i < tamanho_aux; i++){
                    if(aux[i] < '0' || aux[i] > '9') valido = false;
                    if(aux[i] == ' ') valido = false;
                }
            }
        }
        else{
            for(int i = 0; i < tamanho_aux; i++){
                if(aux[i] < '0' || aux[i] > '9') valido = false;
            }
        }

        if(!valido){
            mensagem_erro("Resposta inválida! Digite um caractere válido: ");
        }
    }

    return stoi(aux);
}

//Verifica se o comando inserido no menu de ajuda é válido
int ler_comando_help() {
    string aux;
    bool valido=false;
    int comando=0;

    while(!valido) {

        valido=true;
        comando = ler_inteiro_comandos();

            //valida comandos existentes no help
            if (comando!=1 and comando!=2 and comando!=3 and 
                comando!=4 and comando!=5 and comando!=6 and comando!=7
                and comando != 8 and comando != 0)
                valido=false;
                
            if (!valido){
                mensagem_erro("Resposta inválida, digite um caractere válido");
            }
    }

    return comando;
}

//Verifica se o comando inserido no menu de editar exercício é válido
int ler_comando_edicao() {
    string aux;
    bool valido = false;
    int comando = 0;

    while (!valido) {

        valido=true;
        comando = ler_inteiro_comandos();

            if (comando != 0 and
                comando != 1 and
                comando != 2 and
                comando != 3 and
                comando != 4 and
                comando != 5)
            {
                valido = false;
            }
            
            if (!valido){
                mensagem_erro("Resposta inválida, digite um caractere válido");
            }
        }

    return comando;
}

//Verifica se o comando inserido no menu de buscar exercício é válido
int ler_subcomando_busca() {
    string aux;
    bool valido = false;
    int comando = 0;

    while (!valido) {

        valido=true;
        comando = ler_inteiro_comandos();

        if (comando!=1 and comando!=2 and comando!=0){
            valido = false;
        }

        if (!valido){
            mensagem_erro("Resposta inválida, digite um caractere válido");
        }
    }

    return comando;
}

//Verifica se o comando inserido no menu de ordenar a lista de exercícios é valido
int ler_subcomando_ordenacao() {
    string aux;
    bool valido = false;
    int comando = 0;

    while (!valido) {

        valido=true;
        comando = ler_inteiro_comandos();
            
        if (comando!=1 and comando!=2 and comando != 3 and comando!=0){
            valido = false;
        }

        if (!valido){
            mensagem_erro("Resposta inválida, digite um caractere válido");
        }
    }

    return comando;
}

//Verifica se o comando inserido na interface principal é valido
int ler_comando_interface() {
    string aux;
    bool valido=false;
    int comando=0;

    while(!valido) {
        
        valido=true;
        comando = ler_inteiro_comandos();

            if (comando!=0 and comando!=1 and comando!=2 and 
                comando!=3 and comando!=4 and comando!=5 and 
                comando !=6 and comando !=7 and comando != 8 and comando!=9)
                valido=false;
                
            if (!valido){
                mensagem_erro("Resposta inválida, digite um caractere válido");
                }
        }

    return comando;
}

//======================================================================================
//FUNÇÕES BASE: Realizam as ações necessárias para que a aplicação funcione corretamente.
//=====================================================================================

//Lê dados do arquivo CSV e armazena no vetor de Exercicios
void ler_arquivo(ifstream &arquivo, Exercicios *exercicio,int indice_struct){   
    arquivo >> ws;

    getline(arquivo, exercicio[indice_struct].nome_exercicio,','); //lê até a primeira vírgula
            
    char primeiro_caractere;
    arquivo>>primeiro_caractere;
            
    if (primeiro_caractere=='"'){   //caso o campo venha entre aspas
        getline(arquivo, exercicio[indice_struct].musculos_alvo,'"');
        arquivo.ignore(1,',');
    }
    else {  //caso não tenha aspas, faz leitura normal
        exercicio[indice_struct].musculos_alvo=primeiro_caractere;
        string resto;
        getline(arquivo,resto,',');
        exercicio[indice_struct].musculos_alvo+=resto;  
    }                                                       

    arquivo>>exercicio[indice_struct].series;
    arquivo.ignore(1,',');      
    
    arquivo>>exercicio[indice_struct].carga;
    arquivo.ignore(1,',');      

    arquivo.ignore(1,'"');
    getline(arquivo, exercicio[indice_struct].descricao,'"');   //lê a descrição entre aspas
    arquivo.ignore(1,'\n');     //ignora a quebra de linha
}

//Redimensiona dinâmicamente o vetor caso o limite atual seja atingido
void redimensionamento_vetor_ex(Exercicios *&exercicio, int &capacidade){
    Exercicios *temp=new Exercicios[capacidade+10];  

    for (int i=0; i<capacidade; i++){    
        temp[i]=exercicio[i];
    }

    delete[] exercicio; //libera o vetor antigo
    exercicio=temp;     //substitui pelo novo
    capacidade+=10;  //aumenta o tamanho de controle
}

//Redimensiona dinâmicamente o vetor de índices utilizado na busca múltipla
void redimensionamento_vetor_indices(int *&vetor_indices, int &tamanho_vetor){
    int novo_tamanho=tamanho_vetor+5;
    int *novo_vetor=new int[novo_tamanho];

    for (int i=0; i<tamanho_vetor; i++){
        novo_vetor[i]=vetor_indices[i];
    }

    delete[] vetor_indices;
    vetor_indices=novo_vetor;
    tamanho_vetor=novo_tamanho;
}

//Realiza o particionamento para o merge sort
void intercala(Exercicios a[], int inicio, int meio, int fim, int modo){ 
	int i = inicio;
    int j = meio + 1;
	int tamanho = fim - inicio + 1; // numero de elementos que tem no intervalo que se pretende ordenar

    Exercicios *aux = new Exercicios[tamanho];

    if (modo==1){
            for(int k = 0; k < tamanho; k++){
                if(( i <= meio) && (j <= fim)){ // se ainda há elementos para serem ordenados
                    if(toLower(a[i].nome_exercicio) <= toLower(a[j].nome_exercicio)){ //compara o elemento da esquerda com o elemento da direita
                        aux[k] = a[i];
                        i++;
                    }else{
                        aux[k] = a[j];
                        j++;
                    }
                }else if(i > meio){ // se terminou os elementos da metade esquerda(menores), entao o proximo elemento da direita é copiado
                    aux[k] = a[j];
                    j++;
                }else{ // caso nao acabou os da esquerda, mas acabou os da direita(maiores), entao o proximo elemento da esquerda é copiado
                    aux[k] = a[i];
                    i++;
                }
            }
            
        for(int k = 0; k < tamanho; k++){ // copia do aux
            a[inicio + k] = aux[k];
        }
    }

    else if (modo==2){
            for(int k = 0; k < tamanho; k++){
                if(( i <= meio) && (j <= fim)){ // se ainda há elementos para serem ordenados
                    if(toLower(a[i].musculos_alvo) <= toLower(a[j].musculos_alvo)){ //compara o elemento da esquerda com o elemento da direita
                        aux[k] = a[i];
                        i++;
                    }else{
                        aux[k] = a[j];
                        j++;
                    }
                }else if(i > meio){ // se terminou os elementos da metade esquerda(menores), entao o proximo elemento da direita é copiado
                    aux[k] = a[j];
                    j++;
                }else{ // caso nao acabou os da esquerda, mas acabou os da direita(maiores), entao o proximo elemento da esquerda é copiado
                    aux[k] = a[i];
                    i++;
                }
            }
            
        for(int k = 0; k < tamanho; k++){ // copia do aux
            a[inicio + k] = aux[k];
        }
    }
    
    else if(modo == 3){
            for(int k = 0; k < tamanho; k++){
                if(( i <= meio) && (j <= fim)){ // se ainda há elementos para serem ordenados
                    if(a[i].carga <= a[j].carga){ //compara o elemento da esquerda com o elemento da direita
                        aux[k] = a[i];
                        i++;
                    }else{
                        aux[k] = a[j];
                        j++;
                    }
                }else if(i > meio){ // se terminou os elementos da metade esquerda(menores), entao o proximo elemento da direita é copiado
                    aux[k] = a[j];
                    j++;
                }else{ // caso nao acabou os da esquerda, mas acabou os da direita(maiores), entao o proximo elemento da esquerda é copiado
                    aux[k] = a[i];
                    i++;
                }
            }
            
        for(int k = 0; k < tamanho; k++){ // copia do aux
            a[inicio + k] = aux[k];
        }
    }

    delete[] aux;
}

//Busca o índice de um exercício através do nome
int busca_binaria_unica(Exercicios vetor[], int inicio, int fim, string buscado, bool &achou)
{
    string alvoLower = toLower(buscado);
  
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        string nomeLower = toLower(vetor[meio].nome_exercicio);

        if (nomeLower == alvoLower) {
            achou = true;
            return meio;
        }
        else if (alvoLower < nomeLower) {
            fim = meio - 1;
        }
        else {
            inicio = meio + 1;
        }
    }

    achou = false;
    return -1;
}

//Executa a primeira busca binaria tradicional para o musculo, mesmo se o "buscado" for escrito com letras minúsculas
int busca_binaria_musculo_rec(Exercicios vetor[], int inicio, int fim, string buscado) {
    
    if (inicio > fim)
        return -1;

    int meio = (inicio + fim) / 2;

   string alvoLower = toLower(buscado);
	string muscLower = toLower(vetor[meio].musculos_alvo);

	if (muscLower == alvoLower){
		return meio;
	}else if (alvoLower < muscLower){
		return busca_binaria_musculo_rec(vetor, inicio, meio - 1, buscado);
	}else{
		return busca_binaria_musculo_rec(vetor, meio + 1, fim, buscado);
	}

}

//Aloca em um vetor índices do(s) exercício(s) que possuem o grupo muscular buscado, e lateraliza a busca binaria para achar todas as ocorrencias.
void busca_binaria_musculo(Exercicios vetor[],int inicio,int fim,string buscado,int *&vetor_indices,int &qtd,int &tam_indices){
 
    qtd = 0;
    int pos = busca_binaria_musculo_rec(vetor, inicio, fim, buscado);
 
    if (pos == -1)
        return;

    // garante espaço
    if (qtd == tam_indices)
        redimensionamento_vetor_indices(vetor_indices, tam_indices);

    vetor_indices[qtd] = pos;
    qtd++;

   string buscadoLower = toLower(buscado);

    // esquerda
    int i = pos - 1;
    while (i >= inicio && toLower(vetor[i].musculos_alvo) == buscadoLower) {

        if (qtd == tam_indices)
            redimensionamento_vetor_indices(vetor_indices, tam_indices);

        vetor_indices[qtd] = i;
        qtd++;
        i--;
    }

    // direita
    int j = pos + 1;
    while (j <= fim && toLower(vetor[j].musculos_alvo) == buscadoLower) {

        if (qtd == tam_indices)
            redimensionamento_vetor_indices(vetor_indices, tam_indices);

    vetor_indices[qtd] = j;
    qtd++;
    j++;
    }
}

//Ordena o vetor de exercícios através do nome
void ordena_nome(Exercicios v[], int inicio, int fim){
	int meio;
	if(inicio < fim){
		meio = (inicio + fim) / 2;
		ordena_nome(v, inicio, meio); // chamado uma ordenaçao para cada metade
		ordena_nome(v, meio+1, fim);
		intercala(v, inicio, meio, fim, 1);
	}
}

//Realiza a ordenação pelos valores do grupo muscular, em ordem alfabética
void ordena_musculos(Exercicios v[], int inicio, int fim){
	int meio;
	if(inicio < fim){
		meio = (inicio + fim) / 2;
		ordena_musculos(v, inicio, meio); 
		ordena_musculos(v, meio+1, fim);
		intercala(v, inicio, meio, fim, 2);
	}
}

//Realiza a ordenação pelos valores da carga de cada exercício, em ordem crescente
void ordena_carga(Exercicios vetor[], int inicio, int fim){
	if(inicio < fim){
		int meio = (inicio+fim)/2;
		ordena_carga(vetor,inicio,meio);
		ordena_carga(vetor,meio+1,fim);
		intercala(vetor,inicio,meio,fim, 3);
	}
}

//Sobreescreve o arquivo caso o usuário deseje salvar os dados na base de dados.
void sobreescrever_arquivo(Exercicios *exercicio,string cabecalho, int linhas_alocadas){
    
    ofstream arquivo_backup("trabalho.csv");
		arquivo_backup << cabecalho << endl;

		for (int i = 0; i < linhas_alocadas; i++) {
			string linha = "";

			linha += exercicio[i].nome_exercicio + ",";

			if (exercicio[i].musculos_alvo.find(',') != string::npos) {
				linha += '"' + exercicio[i].musculos_alvo + '"';
			} else {
				linha += exercicio[i].musculos_alvo;
			}
			linha += ",";

			linha += to_string(exercicio[i].series) + ",";
			linha += to_string(exercicio[i].carga) + ",";
			linha += '"' + exercicio[i].descricao + '"';

			arquivo_backup << linha << '\n';
		}

		arquivo_backup.close();
}

//======================================================================
//FUNÇÕES DE COMANDO: Realizam as ações descritas em cada menu da aplicação
//=====================================================================

//(1) Exibe o índice e o nome de cada exercício
void listar_indices(Exercicios *exercicio, int linhas_alocadas){
    for (int i=0; i<linhas_alocadas; i++){
        cout<<i+1<<"-"<<exercicio[i].nome_exercicio<<endl;
    }
}

//(2) Adiciona um novo exercício na base de dados
void adicionar_exercicio(Exercicios *exercicio, int &linhas_alocadas, bool &arquivo_alterado, bool ja_ordenou){
    cin.clear();

    bool achou = true;
	string nome_temp = "";

	while (achou == true) {
		cout << "Insira o nome do exercício (OBS: Cada exercício pode ser cadastrado apenas uma vez!).\n";

		getline(cin, nome_temp);

		achou = false;
        if (arquivo_alterado or !ja_ordenou){
            ordena_nome(exercicio, 0, linhas_alocadas-1);
        }
        
		busca_binaria_unica(exercicio, 0, linhas_alocadas - 1, nome_temp, achou);

		if (achou == true) {
			setColor(2);
			cout << "Esse exercício já está cadastrado! Digite outro nome.\n";
			setColor(0);
		} else {
			exercicio[linhas_alocadas].nome_exercicio = nome_temp;
		}
	}

    system ("clear");
    cout << "Insira os músculos alvo do exercício:\n";
    getline(cin, exercicio[linhas_alocadas].musculos_alvo);

    system ("clear");
    cout << "Insira o número de séries a serem feitas (máximo de até 999 séries por exercício):\n";
    exercicio[linhas_alocadas].series = ler_inteiro();

    while (exercicio[linhas_alocadas].series <= 0) {
        setColor(2);
        cout << "Valor inválido! O número de séries não pode ser nulo ou negativo. Digite novamente: ";
        setColor(0);
        exercicio[linhas_alocadas].series = ler_inteiro();
    }

    system ("clear");
    cout << "Insira a carga usada em kg (máximo de 9999.99kg por exercício):\n";
    exercicio[linhas_alocadas].carga = ler_float();

    system ("clear");
    cout << "Insira a descrição do exercício:\n";
    getline(cin, exercicio[linhas_alocadas].descricao);

    char save;
    system ("clear");
    cout<<"Novo exercício:"<<endl
        << "---------------------------" << endl;
    mostrar_intervalo(exercicio, linhas_alocadas, linhas_alocadas);
    cout << "Deseja salvar o novo exercício? (S/N): ";
    save = verificar_caractere();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpa o \n

    if (save == 'N' || save == 'n') {
		setColor(2);
        cout << "O exercício não foi salvo." << endl;
        setColor(0);
        // limpa o espaço usado para que não sobre lixo na memória
        exercicio[linhas_alocadas].nome_exercicio = "";
        exercicio[linhas_alocadas].musculos_alvo = "";
        exercicio[linhas_alocadas].series = 0;
        exercicio[linhas_alocadas].carga = 0;
        exercicio[linhas_alocadas].descricao = "";
    }
    else if (save == 'S' || save == 's') {
        arquivo_alterado = true;
        ofstream arquivo_mudanca("trabalho.csv", ios::app); //Edita o arquivo sem apagar o conteúdo anterior
        string linha = "";

        linha += exercicio[linhas_alocadas].nome_exercicio + ",";

        if (exercicio[linhas_alocadas].musculos_alvo.find(',') != string::npos) {
            linha += '"' + exercicio[linhas_alocadas].musculos_alvo + '"';
        }
        else {
            linha += exercicio[linhas_alocadas].musculos_alvo;
        }
        linha += ",";

        linha += to_string(exercicio[linhas_alocadas].series);
        linha += ",";

        linha += to_string(exercicio[linhas_alocadas].carga);
        linha += ",";

        linha += '"' + exercicio[linhas_alocadas].descricao + '"';

        arquivo_mudanca << linha << '\n';
        arquivo_mudanca.close();

        mensagem_sucesso("Exercício salvo com sucesso!");
        linhas_alocadas++;      // atualiza o número de linhas alocadas
    }
}

//(3) Edita um exercício já cadastrado na base de dados
void editar_exercicio(Exercicios *exercicio, int indice, string cabecalho, int linhas_alocadas, bool &alterou_vetor){
    
    alterou_vetor=false;    //Flag passada por referência para caso haja alterações o vetor seja ordenado antes de qualquer busca

    system("clear");
    cout << "Você deseja editar o exercício " << exercicio[indice].nome_exercicio << "? (S/N)" << endl;
    char resposta = verificar_caractere();

    if (resposta == 'N' or resposta == 'n') {
        cout << "Não foram feitas mudanças." << endl;
        pausar();
        system("clear");
        return;
    }

    else if (resposta == 'S' or resposta == 's') {
        bool parar = false;
        while (!parar) {
            system("clear");

            interface_edicao(exercicio, indice);

            cin>>ws;    //Limpeza total do buffer antes de ler o subcomando
            int comando = ler_comando_edicao();

            if (comando == 1) {
                string novo;
                system("clear");
                cout << "Insira o novo nome do exercício: ";
                getline(cin, novo);

                system("clear");
                cout << "Nome atual: "<< exercicio[indice].nome_exercicio<< endl;
                cout << "Novo nome: "<< novo<<endl;
                cout << "Salvar essa alteração? (S/N): ";
                char save = verificar_caractere();

                if (save == 'S' or save == 's') {
                    exercicio[indice].nome_exercicio = novo;
                    alterou_vetor=true;

                    cin.ignore();
                    mensagem_sucesso("Alteração realizada com sucesso!");
                    animacao("Carregando...");
                }
                else{
                    cout << "Alteração descartada." << endl;
                    cin.ignore();
                    animacao("Carregando...");
                    system("clear");
				}
            }

            else if (comando == 2) {
                string novo;
                system("clear");
                cout << "Insira o novo grupo muscular do exercício: ";
                getline(cin, novo);


                system("clear");
                cout << "Grupo muscular atual: "<< exercicio[indice].musculos_alvo<< endl;
                cout << "Novo grupo muscular: "<<novo<<endl;
                cout << "Salvar essa alteração? (S/N): ";
                char save = verificar_caractere();

                if (save == 'S' or save == 's') {
                    exercicio[indice].musculos_alvo = novo;
                    alterou_vetor=true;

                    cin.ignore();
                    mensagem_sucesso("Alteração realizada com sucesso!");
                    animacao("Carregando...");
                }
                else{
                    cout << "Alteração descartada." << endl;
                    cin.ignore();
                    animacao("Carregando...");
                    system("clear");
				}
            }

            else if (comando == 3) {
                int novo;
                system("clear");
                cout << "Insira o novo número de séries a serem feitas: ";
                novo = ler_inteiro();

                while (novo < 0 or novo == 0) {
                    mensagem_erro("Valor inválido! O número de séries não pode ser nulo ou negativo. Digite novamente: ");
                    novo = ler_inteiro();
                }

                system("clear");
                cout << "Número de séries atual: "<< exercicio[indice].series<< endl;
                cout << "Novo número de séries: " << novo << endl;
                cout << "Salvar essa alteração? (S/N): ";
                char save = verificar_caractere();

               if (save == 'S' or save == 's') {
                    exercicio[indice].series = novo;
                    alterou_vetor=true;

                    cin.ignore();
                    mensagem_sucesso("Alteração realizada com sucesso!");
                    animacao("Carregando...");
                }
                else{
                    cout << "Alteração descartada." << endl;
                    cin.ignore();
                    animacao("Carregando...");
                    system("clear");
				}
            }

            else if (comando == 4) {
                float novo;
                system("clear");
                cout << "Insira a nova carga usada (kg): ";
                novo = ler_float();

                while (novo < 0) {
                    cout << "Valor inválido! A carga não pode ser negativa. Digite novamente: ";
                    novo = ler_float();
                }

                system("clear");
                cout << "Carga atual(kg): "<< exercicio[indice].carga<< endl;
                cout<< "Nova carga(kg): "<<novo<<endl;
                cout << "Salvar essa alteração? (S/N): ";
                char save = verificar_caractere();

                if (save == 'S' or save == 's') {
                    exercicio[indice].carga = novo;
                    alterou_vetor=true;

                    cin.ignore();
                    mensagem_sucesso("Alteração realizada com sucesso!");
                    animacao("Carregando...");
                }
                else{
                    cout << "Alteração descartada." << endl;
                    cin.ignore();
                    animacao("Carregando...");
                    system("clear");
				}
            }

            else if (comando == 5) {
                string novo;    //limpeza adicional do buffer
                system("clear");
                cout << "Insira a nova descrição do exercício: ";
                getline(cin, novo);

                system("clear");
                cout << "Descrição atual: "<< exercicio[indice].descricao<< endl;
                cout<< "Nova descrição: "<<novo<<endl;
                cout << "Salvar essa alteração? (S/N): " << endl;
                char save = verificar_caractere();

                if (save == 'S' or save == 's') {
                    exercicio[indice].descricao = novo;
                    alterou_vetor=true;

                    cin.ignore();
                    mensagem_sucesso("Alteração realizada com sucesso!");
                    animacao("Carregando...");
                }
                else{
                    cout << "Alteração descartada." << endl;
                    cin.ignore();
                    animacao("Carregando...");
                    system("clear");
				}
            }

            else if (comando == 0) {
                parar = true;
            }
        }
    }
    
    if (alterou_vetor){
        sobreescrever_arquivo(exercicio, cabecalho, linhas_alocadas);   //O novo registro é inserido no final do vetor
    }
}

//(3) Verifica se o exercício procurado está cadastrado na base de dados
int loop_busca_exercicio(Exercicios *exercicio, int linhas_alocadas, bool &achou) {
    bool primeira = true;
    bool parar = false;
    string nome;
    int indice = -1;
    bool entrada_igual_zero=false;

    while (!parar) {
        
        if (primeira)
        cout << "Digite o nome do exercício: ";
        else
        cout << "Digite outro exercício (ou 0 para voltar): ";
     
        getline(cin, nome);
        primeira=false;
        
        if (!primeira){ 
            if (entrada_igual_zero or nome == "0") {
                parar=true;  // usuário desistiu
                return -1;
                system("clear");
            }
        }
        

        if (!parar){
            indice = busca_binaria_unica(exercicio,0,linhas_alocadas-1,nome,achou);
            
            if (achou) {
                return indice;  // sucesso!
            }
            
            setColor(2);
            cout << "Exercício \"" << nome << "\" não encontrado." << endl;
            setColor(0);
        }
    }

    return -1;      
}

//(4) Remove um exercício buscado através do nome
void remover_exercicios(Exercicios *exercicio, int &linhas_alocadas, string cabecalho, int indice){ 
    char resposta;
    cout << "Remoção de exercício: "<<endl;
    mostrar_intervalo(exercicio, indice, indice);

	cout << "Deseja realmente apagar o exercício " << exercicio[indice].nome_exercicio << " ?(S/N)" << endl;
	resposta = verificar_caractere();
	
    if (resposta == 'N' || resposta == 'n'){
        cout<<"O exercício não foi removido." << endl;
    }

    else if (resposta == 'S' || resposta == 's') {

		for (int i = indice; i < linhas_alocadas - 1; i++) {
			exercicio[i] = exercicio[i + 1];
		}
		linhas_alocadas--;

		sobreescrever_arquivo(exercicio, cabecalho, linhas_alocadas);

        system("clear");
		mensagem_sucesso("Exercício removido com sucesso!");
    }
}

//(5) Loop de verificação da existência de exercício(s) com o grupo muscular procurado
void loop_busca_musculo(Exercicios *exercicio, int linhas_alocadas,int *&vetor_indices, int &indices_alocados, int &tamanho_vetor_indices, bool &achou){
    bool primeira = true;
    bool parar = false;
    string nome;
    bool entrada_igual_zero=false;

    while (!parar) {
        if (primeira)
            cout << "Digite o nome do grupo muscular que deseja procurar: ";
        else
            cout << "Digite outros grupos musculares (ou 0 para voltar): ";
            
        getline(cin, nome);
        primeira = false;
        
        if (!primeira){ 
            if (entrada_igual_zero or nome == "0") {
                parar = true;
                return;
            }
        }
        
        // A partir daqui já não é mais primeira vez

        if (!parar){
            indices_alocados = 0;
            busca_binaria_musculo(exercicio, 0, linhas_alocadas-1,nome, vetor_indices, indices_alocados,tamanho_vetor_indices);

            if (indices_alocados > 0) {
                achou = true;
                return;
            }

            setColor(2);
            cout << "Grupo muscular \"" << nome << "\" não encontrado." << endl;
            setColor(0);
        }
    }
}

//(5.1) Exibe o(s) exercício(s), busca através do grupo muscular
void mostrar_achados_busca(Exercicios *exercicio, int *vetor_indices, int qtd_indices){

    for (int i = 0; i <= qtd_indices; i++){
        cout << "Exércicio: " << exercicio[vetor_indices[i]].nome_exercicio << endl
             << "Grupos musculares: " << exercicio[vetor_indices[i]].musculos_alvo << endl
             << "Numero de séries: " << exercicio[vetor_indices[i]].series << "x" << endl
             << "Carga: " << exercicio[vetor_indices[i]].carga << "kg" << endl
             << "Descrição do exércicio: " << exercicio[vetor_indices[i]].descricao << endl
             << "---------------------------" << endl;
    }
}

//(6) Exibe todos os exercícios carregados no arquivo
void mostrar_todos_os_dados(Exercicios *exercicio, int linhas_alocadas){
    
    mostrar_intervalo(exercicio, 0, linhas_alocadas - 1);
}

//(7) Recebe os valores do intervalo e verifica se eles são válidos
void mostrar_intervalo_usuario(Exercicios *exercicio,int linhas_alocadas){
    int indice_1=0,indice_2=0;

    cout<<"Insira o índice do 1º (primeiro) exercício: ";
    indice_1=ler_inteiro();    //verificacao do N1 com a funcao utilitaria para inteiro
    
    while(indice_1<1||indice_1>linhas_alocadas){   //se for solicitado um trecho invalido exibe mensagem 
        cout<<"Valor inválido! O 1º (primeiro) índice deve estar entre 1 e "<<linhas_alocadas<<": ";
        indice_1=ler_inteiro();
    }

    system("clear");
    cout<<"Insira o índice do 2º (segundo) exercício: ";
    indice_2=ler_inteiro();     //verificacao do N2 com a funcao utilitaria para inteiro

    while(indice_2<1||indice_2>linhas_alocadas||indice_2<=indice_1){    //se for solicitado um trecho invalido exibe mensagem 
        if(indice_2<1||indice_2>linhas_alocadas)
            cout<<"Valor inválido! O 2º (segundo) índice deve estar entre 1 e "<<linhas_alocadas<<": ";
        else if(indice_2<=indice_1)
            cout<<"Valor inválido! O 2º (segundo) índice deve ser maior que o 1º (primeiro): ";
        indice_2=ler_inteiro();
    }

    //mostra o trecho selecionado
    system("clear");
    mostrar_intervalo(exercicio, indice_1 - 1, indice_2 - 1);
}

//(9.2) Exibe o exemplo do comando 2 do menu de ajuda
void mostrar_trecho_help(Exercicios *exercicio){
    int indice_1=20;
    int indice_2=22;

    for (int i=indice_1-1; i<indice_2; i++){
        cout<<"Exércicio: "<<exercicio[i].nome_exercicio<<endl
            <<"Grupos musculares: "<<exercicio[i].musculos_alvo<<endl
            <<"Numero de séries: " <<exercicio[i].series<<"x"<<endl
            <<"Carga: "<<exercicio[i].carga<<"kg"<<endl
            <<"Descrição do exércicio: "<<exercicio[i].descricao<<endl
            <<"---------------------------"<<endl;
    }
}

//=======
//MAIN
//=======

int main(){
	ifstream arquivo("trabalho.csv");

    if (!arquivo){  //Verifica se foi possível abrir o arquivo
        setColor(2);
        cout<<"Erro: não foi possível abrir o arquivo";
        return 0;                          
    }

    int tamanho_vetor=40, n_linhas=0, linhas_alocadas=0, indice_struct=0;
    string cabecalho, linhas_validas;
    
    Exercicios *exercicio=new Exercicios[tamanho_vetor];
   
    //conta quantas linhas o arquivo tem (excluindo cabeçalho)
    getline(arquivo,cabecalho);
    while (getline(arquivo,linhas_validas)){    
        n_linhas++;
    }
     
    //volta o ponteiro pro início do arquivo pra fazer a leitura
    arquivo.clear();
    arquivo.seekg(0);
    getline(arquivo, cabecalho);
    
    //Faz a leitura de cada linha do arquivo csv, e redimensiona se o vetor for preenchido
    while (linhas_alocadas < n_linhas){
        if (linhas_alocadas == tamanho_vetor) {     
            redimensionamento_vetor_ex(exercicio, tamanho_vetor);
        }

        ler_arquivo(arquivo, exercicio, indice_struct);

        linhas_alocadas++;      
        indice_struct++;
    }

    //Variáveis auxiliares para o funcionamento correto do programa
    bool parar = false;
    bool ja_ordenou = false;
    bool arquivo_alterado = false;

    //As variáveis ja_ordenou e arquivo_alterado são atualizadas após ordenações ou a adição de novos registros,
    //para fazer a chamada do merge sort apenas quando necessário

    //Loop principal do programa
    while (!parar) {
        system("clear");
        
        interface_inicial(); 
        int comando = ler_comando_interface(); //Ler e verificar o comando do usuário
        system("clear");

        if (comando == 1) { //Listar indices
			listar_indices(exercicio, linhas_alocadas);
            animacao("Carregando");
        }
        
        else if (comando == 2) { //Adicionar exercício
			if (linhas_alocadas + 1 == tamanho_vetor) {
                redimensionamento_vetor_ex(exercicio, tamanho_vetor);
            }
            
            adicionar_exercicio(exercicio, linhas_alocadas, arquivo_alterado, ja_ordenou);
            animacao("Carregando");
        }
        
        else if (comando == 3) { //Editar exercício
            bool achou_busca=true;

            if (!ja_ordenou or arquivo_alterado) {
                ordena_nome(exercicio, 0, linhas_alocadas - 1);
                ja_ordenou = true;
            }

            system("clear");
            int indice = loop_busca_exercicio(exercicio, linhas_alocadas, achou_busca);

            if (!achou_busca) {
            } 
            else {
                editar_exercicio(exercicio, indice, cabecalho, linhas_alocadas, arquivo_alterado);
            }
        }
        
        else if (comando == 4) { //Remover exercício
			bool achou_busca = true;
			if (!ja_ordenou or arquivo_alterado) {
				ordena_nome(exercicio, 0, linhas_alocadas - 1);
				ja_ordenou = true;
            }
            int indice = loop_busca_exercicio(exercicio, linhas_alocadas, achou_busca); //Retorna o indice do exercicio que eu estou procurando
			
            if (!achou_busca) {
            }
            else {
                system("clear");
				remover_exercicios(exercicio, linhas_alocadas, cabecalho, indice);
                cin.ignore();
				animacao("Carregando");
			}
        }

        else if (comando == 5) { //Procurar exercício
            int subcomando;
            bool parar_busca = false;

            while (!parar_busca){
                interface_busca();
                subcomando=ler_subcomando_busca();
                if(subcomando == 1){ //Busca por nome
                    
                    bool achou_busca=true;  
                    if (!ja_ordenou or arquivo_alterado) {
                    ordena_nome(exercicio, 0, linhas_alocadas - 1);
                    ja_ordenou = true;
                    }

                    system("clear");
                    int indice = loop_busca_exercicio(exercicio, linhas_alocadas, achou_busca);

                    if (!achou_busca) {
                    } 
                    else if (achou_busca){
                    system("clear");
                    mostrar_intervalo(exercicio, indice, indice);
                    animacao("Carregando");
                    }
                }
        
                else if(subcomando == 2){ //Busca por grupo muscular
                    bool achou_busca=true;
                    if (!ja_ordenou or arquivo_alterado) {
                    ordena_musculos(exercicio, 0, linhas_alocadas-1);
                    ja_ordenou = true;
                    }

                    int tamanho_vetor_indices = 5;
                    int *vetor_indices = new int[tamanho_vetor_indices];
                    int indices_alocados = 0;

                    system("clear");
                    loop_busca_musculo(exercicio, linhas_alocadas, vetor_indices, indices_alocados, tamanho_vetor_indices, achou_busca);
                    
                    if (!achou_busca) {
                    } 
                    
                    if (achou_busca){
                    system("clear");
                    mostrar_achados_busca(exercicio, vetor_indices, indices_alocados-1);
                    animacao("Carregando");
                    }
                }

                else if (subcomando == 0){ //Voltar
                    parar_busca=true;
                }
            }
        }
        
        else if (comando == 6) { //Mostrar todos os exercícios
            mostrar_todos_os_dados(exercicio, linhas_alocadas);
            animacao("Carregando");
        }		

        else if (comando == 7){ //Mostrar exercícios em um intervalo
            mostrar_intervalo_usuario(exercicio, linhas_alocadas);
            animacao("Carregando");
        }

        else if(comando == 8){ //Ordenar a lista de exercícios
			int modo;
			char save;
			bool parar_ord = false;

			while (!parar_ord){
                
                system("clear");
                interface_ordenacao();
                modo=ler_subcomando_ordenacao();

				if(modo == 1){ //Ordenar pelo nome
					ordena_nome(exercicio,0,linhas_alocadas-1);
                    mostrar_todos_os_dados(exercicio, linhas_alocadas);

				    cout << "Deseja salvar essa ordenacao no arquivo? (S/N)" << endl;
                    save=verificar_caractere();
                    system("clear");

                    if (save == 'S' or save == 's'){
                        sobreescrever_arquivo(exercicio, cabecalho, linhas_alocadas);

                        mensagem_sucesso("O arquivo foi ordenado com sucesso!");
                        cin.ignore();
                        animacao("Carregando");
                    }
                    else{
                        cout<<"A ordenação não foi salva no arquivo."<<endl;
                        cin.ignore();
                        animacao("Carregando");
                    }
                }
				
				if(modo == 2){ //Ordenar pelos grupos musculares
                    ordena_musculos(exercicio, 0, linhas_alocadas-1);
                    mostrar_todos_os_dados(exercicio, linhas_alocadas);

                    cout << "Deseja salvar essa ordenacao no arquivo? (S/N)" << endl;
                    save=verificar_caractere();
                    system("clear");

                    if (save == 'S' or save == 's'){
                        sobreescrever_arquivo(exercicio, cabecalho, linhas_alocadas);

                        mensagem_sucesso("O arquivo foi ordenado com sucesso!");
                        cin.ignore();
                        animacao("Carregando");
                    }
                    else{
                        cout<<"A ordenação não foi salva no arquivo."<<endl;
                        cin.ignore();
                        animacao("Carregando");
                    }
                }
				
                else if(modo == 3){ //Ordenar pela carga
                    ordena_carga(exercicio, 0, linhas_alocadas-1);
                    mostrar_todos_os_dados(exercicio, linhas_alocadas);
                    
                    cout << "Deseja salvar essa ordenacao no arquivo? (S/N)" << endl;
                    save=verificar_caractere();     
                    system("clear");

                    if (save == 'S' or save == 's'){
                        sobreescrever_arquivo(exercicio, cabecalho, linhas_alocadas);

                        mensagem_sucesso("O arquivo foi ordenado com sucesso!");        
                        cin.ignore();
                        animacao("Carregando");
                    }
                    else{
                        cout<<"A ordenação não foi salva no arquivo."<<endl;
                        cin.ignore();
                        animacao("Carregando");
                    }
                }
                else if (modo == 0){
                    parar_ord = true;
                }
			}
        }
        else if (comando == 9) { //Ajuda
            bool sair_help = false;

            while (!sair_help) {
				system("clear");
                interface_help();
                int resposta = ler_comando_help();

                if (resposta == 1) {
                    system("clear");
                    cout<< "Tem a função de listar o índice de todos os elementos presentes atualmente no arquivo.csv "
                        <<"e seus respectivos índices, útil para trabalhar com o comando #2.\n";
                    
                        animacao("Carregando");
                }

                else if (resposta == 2) {
                    system("clear");
                    cout << "Tem a função de adicionar um novo elemento no final do arquivo. "
                            "Serão mostrados os tópicos a serem inseridos, que devem ser preenchidos com valores válidos. "
                            "Em seguida será perguntado se é desejado salvar os dados no arquivo.csv.\n";

                    animacao("Carregando");
                }
                
               else if (resposta == 3){
                    system("clear");
					cout <<"Tem a função de alterar os dados de um determinado exercício já existente através do nome dele. Os dados editados podem ser salvos no arquivo ou descartados.\n";

                    animacao("Carregando...");
				}
				
                else if (resposta == 4) {
                    system("clear");
                    cout << "Tem a função de remover um exercicio através do nome dele.\n";

                    animacao("Carregando");
                }

                else if (resposta == 5) {
                    system("clear");
                    cout << "Tem a função de procurar um exercicio através do nome,ou retornar os exercicios que trabalham determinado grupo muscular.\n";


                    animacao("Carregando");
                }

                else if (resposta == 6) {
                    system("clear");
                    cout << "Tem a função de listar TODOS os elementos presentes atualmente no arquivo.csv aberto pelo programa, "
                            "listando o respectivo nome do exercício, os músculos alvos, a quantidade de séries, "
                            "a carga utilizada em kg, e a descrição do exercício.\n";

                    animacao("Carregando");
                }

                else if (resposta == 7) {
                    system("clear");
                    cout << "Tem a função de listar os dados dos elementos no intervalo [N1....N2]. "
                            "Para isso é pedido ao usuário inserir N1 e N2, para assim mostrar o trecho desejado.\n"
                            "Exemplo do intervalo 20 a 23:\n\n";
                    mostrar_trecho_help(exercicio);
                    
                    animacao("Carregando");
                }
				
				else if (resposta == 8){
                    system("clear");
					cout <<"Tem a função de ordenar os exercicios, é possível ordenar em ordem alfabética de acordo com o seu nome ou grupo muscular, " 
                         <<"ou também ordenar por carga em ordem crescente. A nova ordenação dos exercícios pode ser salva no arquivo ou descartada.\n";
                    
                    animacao("Carregando...");
				}

                else if (resposta == 0) {
                    sair_help = true;
                    system("clear");
                }
            }
        }
        else if (comando == 0) {
            cout << "Deseja parar o programa? (S/N)" << endl;
            char resposta = verificar_caractere();
            cin.ignore();

            if (resposta == 'S' or resposta == 's') {
                system("clear");
                mensagem_sucesso("O programa parou com sucesso!");
                delete[] exercicio;
                return 0;
            }
        }
    }
}
