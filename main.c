#include <stdio.h>  //lib para utilizar printf(), scanf()
#include <stdlib.h> //lib para utilizar system()
#include <string.h>
#include <unistd.h>
#include <time.h> //lib para utilizar time, ano, mes, dia

// "Identificando o sistema operacional para configurar variáveis correspondentes."
#ifdef _WIN32 // Se o usuário estiver no Windows, serão utilizadas as configurações abaixo
const char *os = "Windows";
const char *atagain = "pause";
const char *atpause = "pause";
const char *atcls = "cls";
#elif __linux__ // Se o usuário estiver no Linux, serão utilizadas as configurações abaixo
const char *os = "Linux";
const char *atpause = "read -p \"Pressione enter para continuar\" saindo";
const char *atcls = "clear";
#elif __APPLE__ // Se o usuário estiver no macOS, serão utilizadas as configurações abaixo
const char *os = "MacOS";
const char *atpause = "read -p \"Pressione enter para continuar\" saindo";
const char *atcls = "clear";
#else           // Se o usuario estiver em outra OS será utilizado os seguintes abaixo
const char *os = "Outro sistema operacional";
#endif

#define MAX_USERS 10000
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

typedef struct User
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int isLoggedIn;
} User;

typedef struct Paciente
{
    char nome[50];
    char cpf[15];
    char telefone[20];
    char endereco[100];
    char dataNascimento[12];
    char email[50];
    char dataDiagnostico[12];
    char comorbidade[100];
} Paciente;

User users[MAX_USERS];
int userCount = 0;
int currentUserIndex = -1;

// Declarações das funções (protótipos)
void telaDeInicializacao();
void header();
void telaMenuSistema();
void telaLogin();
void telaCreateUser();
void telaMenuCadastroPaciente();
void cadastrarPaciente();
void buscarPaciente();
void criarEstatisticas();
void listar();
void listarGpRiscoIdadeComorb();
void listarGpRiscoIdade();

// Protótipo da função Calcular Idade
int calcularIdade(char *data_nascimento);

// Definição da função tela de inicialização
void telaDeInicializacao()
{
    printf("================================================================================\n");
    printf("                               INICIANDO SISTEMA                               \n");
    printf("================================================================================\n\n");
    sleep(1);
    system(atcls);
}

// Definição da função header
void header()
{
    printf("================================================================================\n");
    printf("        SISTEMA PARA CADASTRO DE PACIENTES DIAGINOSTICADOS COM COVID-19        \n");
    printf("================================================================================\n");
}

// Definição da função Menu Sistema
void telaMenuSistema()
{
    system(atcls);
    header();
    printf("================================================================================\n");
    printf("                                   BEM VINDO                                   \n");
    printf("================================================================================\n");
    printf("\n1. Fazer login\n");
    printf("2. Novo aqui? Crie o seu usuario\n");
    printf("0. Sair do programa\n");
    printf("\n================================================================================\n");
    printf("Escolha uma opção: ");
}

// Definição da função Tela de Login
void telaLogin()
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int found = 0;

    FILE *file = fopen("usersprofissionaissaude.txt", "r");
    if (file == NULL)
    {
        printf("\nErro, esse usuario não existe.\n");
        sleep(1);
        return;
    }

    system(atcls);
    header();
    printf("================================================================================\n");
    printf("                                     LOGIN                                     \n");
    printf("================================================================================\n");

    printf("\nCaso queira voltar ao menu inicial digite ' 0 ' \n");
    printf("\nDigite o nome de usuário: ");
    scanf("%s", username);
    if (strcmp(username, "0") == 0)
    {
        sleep(1); // Pausa a execução por 2 segundo
        return;
    }
    printf("Digite a senha: ");
    scanf("%s", password);
    if (strcmp(password, "0") == 0)
    {
        sleep(1); // Pausa a execução por 2 segundo
        return;
    }

    char fileUsername[MAX_USERNAME_LENGTH];
    char filePassword[MAX_PASSWORD_LENGTH];

    while (fscanf(file, "User: %s \n Pass: %s \n\n", fileUsername, filePassword) != EOF)
    {
        if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0)
        {
            system(atcls);
            header();
            printf("\nLogin bem-sucedido!\n");
            sleep(1);
            telaMenuCadastroPaciente();
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nUsuário ou senha incorretos.\n");
        system(atpause);
    }
}

// Definição da função Criar Usuário Profissional da Saúde
void telaCreateUser()
{
    if (userCount >= MAX_USERS)
    {
        printf("\nLimite de usuários atingido.\n");
        return;
    }

    FILE *file = fopen("usersprofissionaissaude.txt", "a");
    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return;
    }

    system(atcls);
    header();
    printf("================================================================================\n");
    printf("                  CADASTRO DE USUÁRIO PROFISSIONAL DA SAÚDE                  \n");
    printf("================================================================================\n");

    printf("\nCrie um nome de usuário: ");
    scanf("%s", users[userCount].username);
    printf("Crie uma senha: ");
    scanf("%s", users[userCount].password);
    users[userCount].isLoggedIn = 0;
    userCount++;

    fprintf(file, "User: %s \n Pass: %s \n\n", users[userCount - 1].username, users[userCount - 1].password);
    fclose(file);

    printf("\nUsuário criado com sucesso! USER: %s\n", users[userCount - 1].username);
    sleep(1);
}

// Definição da função Menu de Cadastro de Paciente
void telaMenuCadastroPaciente()
{
    int escolha;

    do
    {
        // Exibição do menu de opções
        system(atcls); // Limpa a Tela
        header();
        printf("================================================================================\n");
        printf("                                      MENU                                      \n");
        printf("================================================================================\n\n");
        printf("1. Cadastrar paciente\n");
        printf("2. Buscar Paciente cadastrado\n");
        printf("3. Estatisticas de pacientes cadastrados\n");
        printf("4. Listar base de dados dos pacientes cadastrados\n");
        printf("5. Listar CEPs e Idades dos pacientes em grupo de risco por Idade e Comorbidade\n");
        printf("6. Listar CEPs e Idades dos pacientes em grupo de risco por idade\n");
        printf("0. Fazer Logout\n\n");
        printf("Selecione uma opção digitando o número\n");
        printf("Opção: ");
        scanf("%d", &escolha);

        switch (escolha) // O Usuário terá que escolher uma opção entre 0 - 6
        {
        case 1:
            cadastrarPaciente(); // Chama a função para cadastrar paciente
            break;
        case 2:
            buscarPaciente(); // Chama a função para buscar dados do paciente cadastrado
            break;
        case 3:
            criarEstatisticas(); // Chama a função para criar arquivo de estatistica
            break;
        case 4:
            listar(); // Chama a função para listar os dados dos pacientes
            break;
        case 5:
            listarGpRiscoIdadeComorb(); // Chama a função para listar grupo de risco por idade e comorbidade
            break;
        case 6:
            listarGpRiscoIdade(); // Chama a função para listar grupo de risco por idade
            break;
        case 0:
            printf("\nDeslogando, Aguarde...\n");
            sleep(1); // Pausa a execução por 2 segundo
            // system(atcls);
            break; // Sai do loop
        default:
            printf("Opção inválida. Tente novamente.\n");
            sleep(2); // Pausa a execução por 2 segundo
        }
    } while (escolha != 0); // Loop enquanto a escolha não for 0 (opção de sair)
}

void sdc()
{
    printf("\nSaindo do cadastramento...\n");
}

void cadastrarPaciente()
{
    while (1)
    {
        Paciente pessoa;

        // Ponteiros para os arquivos
        FILE *arquivo;
        FILE *arquivoRiscoIdadeComorb;
        FILE *arquivoRiscoPorIdade;

        // Abre o arquivo em modo de leitura
        arquivo = fopen("dados.txt", "a");
        arquivoRiscoIdadeComorb = fopen("riscoIdadeComorb.txt", "a");
        arquivoRiscoPorIdade = fopen("riscoIdade.txt", "a");

        // Verificação se os arquivos foram abertos corretamente
        if (arquivo == NULL || arquivoRiscoIdadeComorb == NULL || arquivoRiscoPorIdade == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            // pausecls(); // Chama a função para pausa e limpar a tela
        }

        system(atcls); // Limpa a Tela
        header();
        printf("================================================================================\n");
        printf("                              CADASTRO DE PACIENTE                              \n");
        printf("================================================================================\n\n");
        printf("Informe os dados do paciente ou digite '0' para sair\n\n");

        // Recebendo dados do usuário
        printf("Digite o nome completo do paciente: ");
        scanf(" %[^\n]s", pessoa.nome);
        if (strcmp(pessoa.nome, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite a data de nascimento (DD/MM/AAAA): ");
        scanf("%s", pessoa.dataNascimento);
        if (strcmp(pessoa.dataNascimento, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite o CPF: ");
        scanf("%s", pessoa.cpf);
        if (strcmp(pessoa.cpf, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite o e-mail: ");
        scanf("%s", pessoa.email);
        if (strcmp(pessoa.email, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite o telefone: ");
        scanf("%s", pessoa.telefone);
        if (strcmp(pessoa.telefone, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite o endereço (Rua, Número, Bairro, Cidade, Estado, CEP): ");
        scanf(" %[^\n]s", pessoa.endereco);
        if (strcmp(pessoa.endereco, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Digite a data do diagnóstico (DD/MM/AAAA): ");
        scanf("%s", pessoa.dataDiagnostico);
        if (strcmp(pessoa.dataDiagnostico, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }

        printf("Informe as comorbidades (separadas por vírgula), caso não tenha coloque (nao): ");
        scanf(" %[^\n]s", pessoa.comorbidade);
        if (strcmp(pessoa.comorbidade, "0") == 0)
        {
            sdc();
            sleep(1); // Pausa a execução por 2 segundo
            break;
        }
        printf("\n");

        // Calcula a idade
        int idade = calcularIdade(pessoa.dataNascimento);

        // Verifica se pertence ao grupo de risco idade e comorbidade
        if (idade >= 65 && (strcmp(pessoa.comorbidade, "nao") != 0 && strcmp(pessoa.comorbidade, "NAO") != 0))
        {
            // Salvar no arquivo de risco
            fprintf(arquivoRiscoIdadeComorb, "CEP: %s\nIdade: %d\n\n", pessoa.endereco, idade);
        }

        // if (idade >= 65 && (strcmp(pessoa.comorbidade, "nao") || strcmp(pessoa.comorbidade, "NAO")) != 0)
        // {
        //     // Salvar no arquivo de risco
        //     fprintf(arquivoRiscoIdadeComorb, "CEP: %s\nIdade: %d\n\n", pessoa.endereco, idade);
        // }

        // Verifica se pertence ao grupo de risco maior de 65 anos
        if (idade >= 65)
        {
            // Salvar no arquivo de risco idade
            fprintf(arquivoRiscoPorIdade, "CEP: %s\nIdade: %d\n\n", pessoa.endereco, idade);
        }

        // Salva os dados no arquivo de dados dos pacientes
        fprintf(arquivo, "Nome: %s\nData de Nascimento: %s\nCPF: %s\nE-mail: %s\nTelefone:  %s\nEndereço:  %s\nData do Diagnóstico: %s\nComorbidades: %s\n\n\n",
                pessoa.nome, pessoa.dataNascimento, pessoa.email, pessoa.cpf, pessoa.telefone, pessoa.endereco, pessoa.dataDiagnostico, pessoa.comorbidade);

        printf("Paciente adicionado com sucesso.\n\n");
        system(atpause);
        system(atcls);

        // Fecha os arquivos
        fclose(arquivo);
        fclose(arquivoRiscoIdadeComorb);
        fclose(arquivoRiscoPorIdade);

        break;
    }
}

// Definição da função Calcular Idade
int calcularIdade(char *data_nascimento)
{
    // Obter a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int ano_atual = tm.tm_year + 1900; // Obtém o ano atual
    int mes_atual = tm.tm_mon + 1;     // Obtém o mês atual
    int dia_atual = tm.tm_mday;        // Obtém o dia atual

    int dia, mes, ano;
    sscanf(data_nascimento, "%d/%d/%d", &dia, &mes, &ano); // Extrai o dia, mês e ano da data de nascimento fornecida

    int idade = ano_atual - ano; // Calcula a idade inicialmente como a diferença entre os anos

    // Verifica se o aniversário deste ano ainda não aconteceu
    if (mes > mes_atual || (mes == mes_atual && dia > dia_atual))
    {
        idade--; // Se ainda não tiver feito aniversário este ano, diminui a idade
    }

    return idade; // Retorna a idade calculada
}

// Função para buscar dados por nome, data de nascimento ou data de diagnóstico
void buscarPaciente()
{
    char chave[100];
    int sair = 0;

    while (!sair)
    {

        FILE *arquivo = fopen("dados.txt", "r");
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            sleep(2); // Pausa a execução por 2 segundo
            return;
        }

        system(atcls); // Limpa a tela
        header();
        printf("================================================================================\n");
        printf("                            BUSCAR DADOS DE PACIENTE                            \n");
        printf("================================================================================\n\n");

        printf("Digite o termo de busca (NOME, CPF ou DATA DE NASCIMENTO) ou '0' para sair: ");
        scanf(" %[^\n]", chave);

        if (strcmp(chave, "0") == 0)
        {
            printf("Saindo da busca...\n");
            sleep(1);
            system(atcls);
            break; // Sai do loop
        }

        Paciente pessoa;
        int encontrado = 0;

        system(atcls); // Limpa a tela
        header();
        printf("================================================================================\n");
        printf("                            BUSCAR DADOS DE PACIENTE                            \n");
        printf("================================================================================\n");

        // Loop para ler e comparar dados do arquivo
        while (fscanf(arquivo, "Nome: %[^\n]\nData de Nascimento: %[^\n]\nCPF: %[^\n]\nE-mail: %[^\n]\nTelefone: %[^\n]\nEndereço: %[^\n]\nData do Diagnóstico: %[^\n]\nComorbidades: %[^\n]\n\n\n",
                      pessoa.nome, pessoa.dataNascimento, pessoa.cpf, pessoa.email, pessoa.telefone, pessoa.endereco, pessoa.dataDiagnostico, pessoa.comorbidade) != EOF)
        {

            // Verifica se a chave de busca está presente nos dados
            if (strstr(pessoa.nome, chave) != NULL ||
                strstr(pessoa.cpf, chave) != NULL ||
                strstr(pessoa.dataNascimento, chave) != NULL)
            {
                encontrado = 1;
                printf("\nPaciente encontrado:\n\n");
                printf("Nome: %s\n", pessoa.nome);
                printf("Data de Nascimento: %s\n", pessoa.dataNascimento);
                printf("CPF: %s\n", pessoa.cpf);
                printf("E-mail: %s\n", pessoa.email);
                printf("Telefone: %s\n", pessoa.telefone);
                printf("Endereço: %s\n", pessoa.endereco);
                printf("Data do Diagnóstico: %s\n", pessoa.dataDiagnostico);
                printf("Comorbidades: %s\n", pessoa.comorbidade);
                printf("\n");
                system(atpause);
                system(atcls);
            }
        }

        if (!encontrado)
        {
            printf("Pessoa não encontrada.\n");
            system(atpause);
            system(atcls);
        }

        // Fecha o arquivo
        fclose(arquivo);
    }
}

// Função para criar estatísticas dos diagnósticos e comorbidades
void criarEstatisticas()
{
    FILE *arquivo;
    arquivo = fopen("dados.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        sleep(1);
        return;
    }

    int totalPessoas = 0;
    int totalDiagnosticadas = 0;
    int totalDiagnosticadasIdade65 = 0;
    int totalDiagnosticadasComorbidade = 0;
    int totalDiagnosticadasComorbidadeIdade65 = 0;

    Paciente pessoa;

    while (fscanf(arquivo, "Nome: %[^\n]\nData de Nascimento: %s\nCPF: %s\nE-mail: %s\nTelefone: %s\nEndereço: %[^\n]\nData do Diagnóstico: %s\nComorbidades: %[^\n]\n\n\n",
                  pessoa.nome, pessoa.dataNascimento, pessoa.cpf, pessoa.email, pessoa.telefone, pessoa.endereco, pessoa.dataDiagnostico, pessoa.comorbidade) != EOF)
    {
        totalPessoas++;

        if (strcmp(pessoa.dataDiagnostico, "") != 0)
        {
            totalDiagnosticadas++;

            int idade = calcularIdade(pessoa.dataNascimento);

            if (idade > 65)
            {
                totalDiagnosticadasIdade65++;
            }

            if (strcmp(pessoa.comorbidade, "nao") != 0)
            {
                totalDiagnosticadasComorbidade++;

                if (idade > 65)
                {
                    totalDiagnosticadasComorbidadeIdade65++;
                }
            }
        }
    }

    // Cria ou abre o arquivo de estatísticas para atualizar as informações
    FILE *estatisticasArquivo;
    estatisticasArquivo = fopen("estatisticas.txt", "w");

    if (estatisticasArquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de estatísticas.\n");
        fclose(arquivo);
        sleep(1);
        return;
    }

    system(atcls); // Limpa a Tela
    header();
    printf("================================================================================\n");
    printf("                                 ESTATÍSTICAS                                 \n");
    printf("================================================================================\n");
    printf("\nEstatísticas de Diagnóstico a partir do arquivo de dados:\n\n");
    printf("Total de pessoas cadastradas: %d\n", totalPessoas);
    printf("Total de pessoas diagnosticadas: %d\n", totalDiagnosticadas);
    printf("Total de pessoas diagnosticadas com comorbidade: %d\n", totalDiagnosticadasComorbidade);
    printf("Total de pessoas diagnosticadas sem comorbidade: %d\n", totalDiagnosticadas - totalDiagnosticadasComorbidade);
    printf("Total de pessoas diagnosticadas com idade maior que 65 anos: %d\n", totalDiagnosticadasIdade65);
    printf("Total de pessoas diagnosticadas com idade maior que 65 anos e comorbidade: %d\n", totalDiagnosticadasComorbidadeIdade65);
    printf("\n");
    system(atpause);
    system(atcls);

    // Salva os dados no arquivo de dados dos pacientes
    fprintf(estatisticasArquivo, "Estatísticas de Diagnóstico a partir do arquivo de dados:\n\nTotal de pessoas cadastradas: %d\nTotal de pessoas diagnosticadas: %d\nTotal de pessoas diagnosticadas com comorbidade: %d\nTotal de pessoas diagnosticadas sem comorbidade: %d\nTotal de pessoas diagnosticadas com idade maior que 65 anos: %d\nTotal de pessoas diagnosticadas com idade maior que 65 anos e comorbidade: %d\n\n\n",
            totalPessoas, totalDiagnosticadas, totalDiagnosticadasComorbidade, totalDiagnosticadas - totalDiagnosticadasComorbidade, totalDiagnosticadasIdade65, totalDiagnosticadasComorbidadeIdade65);

    // Fecha o arquivo de estatísticas
    fclose(estatisticasArquivo);

    // Fecha o arquivo
    fclose(arquivo);
}

// Função para listar os dados dos pacientes
void listar()
{
    FILE *arquivo;
    char linha[255];

    // Abre o arquivo em modo de leitura
    arquivo = fopen("dados.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        system(atpause);
        system(atcls);
        return;
    }

    system(atcls); // Limpa a tela
    header();
    printf("================================================================================\n");
    printf("                               DADOS DE PACIENTES                               \n");
    printf("================================================================================\n\n");
    printf("Listando os dados do(s) paciente(s) cadastrados:\n\n");

    // Lê e exibe as linhas do arquivo
    while (fgets(linha, sizeof(linha), arquivo))
    {
        printf("%s", linha);
    }

    fclose(arquivo); // Fecha o arquivo

    system(atpause);
    system(atcls);
}

// Função para listar dados de grupo de risco por idade e comorbidade
void listarGpRiscoIdadeComorb()
{
    FILE *arquivo;
    char linha[255];

    // Abre o arquivo em modo de leitura
    arquivo = fopen("riscoIdadeComorb.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        system(atpause);
        system(atcls);
        return;
    }

    system(atcls); // Limpa a tela
    header();
    printf("================================================================================\n");
    printf(" LISTAR CEPS E IDADES DOS PACIENTES  EM GRUPO DE RISCO POR IDADE E COMORBIDADE  \n");
    printf("================================================================================\n\n");
    printf("Listando Ceps e Idades dos pacientes em de grupo de risco por idade maior que 65 anos e comorbidade:\n\n");

    // Lê e exibe as linhas do arquivo
    while (fgets(linha, sizeof(linha), arquivo))
    {
        printf("%s", linha);
    }

    fclose(arquivo); // Fecha o arquivo

    system(atpause);
    system(atcls);
}

// Função para listar dados de grupo de risco por idade
void listarGpRiscoIdade()
{
    FILE *arquivo;
    char linha[255];

    // Abre o arquivo em modo de leitura
    arquivo = fopen("riscoIdade.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        system(atpause);
        system(atcls);
        return;
    }

    system(atcls); // Limpa a tela
    header();
    printf("================================================================================\n");
    printf("        LISTAR CEPS E IDADES DOS PACIENTES  EM GRUPO DE RISCO POR IDADE         \n");
    printf("================================================================================\n\n");
    printf("Listando Ceps e Idades dos pacientes em de grupo de risco por idade maior que 65 anos:\n\n");

    // Lê e exibe as linhas do arquivo
    while (fgets(linha, sizeof(linha), arquivo))
    {
        printf("%s", linha);
    }

    fclose(arquivo); // Fecha o arquivo

    system(atpause);
    system(atcls);
}

int main()
{
    int choice;
    telaDeInicializacao();

    do
    {

        telaMenuSistema();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            telaLogin();
            break;
        case 2:
            telaCreateUser();
            break;
        case 0:
            printf("\nSaindo do programa...\n");
            sleep(2);
            break;
        default:
            printf("\nOpção inválida. Tente novamente.\n");
            sleep(1);
        }
    } while (choice != 0);

    return 0;
}

