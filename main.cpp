#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "slem.h"

#define MAX_ENTIDADES 100

Local locais[MAX_ENTIDADES];
Veiculo veiculos[MAX_ENTIDADES];
Pedido pedidos[MAX_ENTIDADES];

int numLocais = 0;
int numVeiculos = 0;
int numPedidos = 0;
int proximoIdPedido = 1;

void exibirMenu();

void gerenciarLocais();
void exibirSubMenuLocais();
void cadastrarLocal();
void listarLocais();
void atualizarLocal();
void excluirLocal();
int buscarLocalPorNome(const char* nome);

void gerenciarVeiculos();
void exibirSubMenuVeiculos();
void cadastrarVeiculo();
void listarVeiculos();
void atualizarVeiculo();
void excluirVeiculo();
int buscarVeiculoPorPlaca(const char* placa);

void gerenciarPedidos();
void exibirSubMenuPedidos();
void cadastrarPedido();
void listarPedidos();
void atualizarPedido();
void excluirPedido();
int buscarPedidoPorId(int id);

void calcularRota();
double calcularDistancia(const Local& local1, const Local& local2);

void fazerBackup();
void restaurarDados();


int main() {
    restaurarDados();

    int opcao;
    do {
        exibirMenu();
        std::cin >> opcao;
        
        // Limpar buffer de entrada para evitar problemas com cin >> char/string
        std::cin.ignore(10000, '\n'); 

        switch (opcao) {
            case 1: gerenciarLocais(); break;
            case 2: gerenciarVeiculos(); break;
            case 3: gerenciarPedidos(); break;
            case 4: calcularRota(); break;
            case 5: fazerBackup(); break;
            case 6: restaurarDados(); break;
            case 0: std::cout << "\nSaindo do sistema... Ate logo!\n"; break;
            default: std::cout << "\nOpcao invalida! Tente novamente.\n"; break;
        }

        if (opcao > 0) {
             std::cout << "\nPressione Enter para continuar...";
             std::cin.get();
        }

    } while (opcao != 0);

    return 0;
}

void exibirMenu() {
    system("cls || clear");
    std::cout << "=====================================================\n";
    std::cout << "   Sistema de Logistica de Entrega de Mercadorias    \n";
    std::cout << "=====================================================\n";
    std::cout << "1. Gerenciar Locais\n";
    std::cout << "2. Gerenciar Veiculos\n";
    std::cout << "3. Gerenciar Pedidos\n";
    std::cout << "4. Calcular Rota de Entrega\n";
    std::cout << "5. Fazer Backup dos Dados\n";
    std::cout << "6. Restaurar Dados\n";
    std::cout << "0. Sair\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Escolha uma opcao: ";
}

// --- FUNÇÕES DE BACKUP E RESTAURAÇÃO ---

void fazerBackup() {
    system("cls || clear");
    std::cout << "--- Backup de Dados ---\n";
    FILE* file;

    file = fopen("locais.bin", "wb");
    if (file == NULL) { std::cout << "Erro ao criar arquivo de backup de locais.\n"; return; }
    fwrite(&numLocais, sizeof(int), 1, file);
    fwrite(locais, sizeof(Local), numLocais, file);
    fclose(file);
    std::cout << "Backup de " << numLocais << " locais realizado.\n";

    file = fopen("veiculos.bin", "wb");
    if (file == NULL) { std::cout << "Erro ao criar arquivo de backup de veiculos.\n"; return; }
    fwrite(&numVeiculos, sizeof(int), 1, file);
    fwrite(veiculos, sizeof(Veiculo), numVeiculos, file);
    fclose(file);
    std::cout << "Backup de " << numVeiculos << " veiculos realizado.\n";

    file = fopen("pedidos.bin", "wb");
    if (file == NULL) { std::cout << "Erro ao criar arquivo de backup de pedidos.\n"; return; }
    fwrite(&numPedidos, sizeof(int), 1, file);
    fwrite(&proximoIdPedido, sizeof(int), 1, file);
    fwrite(pedidos, sizeof(Pedido), numPedidos, file);
    fclose(file);
    std::cout << "Backup de " << numPedidos << " pedidos realizado.\n";
    
    std::cout << "\nBackup de todos os dados concluido!\n";
}

void restaurarDados() {
    system("cls || clear");
    std::cout << "--- Restaurando Dados ---\n";
    FILE* file;

    file = fopen("locais.bin", "rb");
    if (file != NULL) {
        fread(&numLocais, sizeof(int), 1, file);
        fread(locais, sizeof(Local), numLocais, file);
        fclose(file);
        std::cout << numLocais << " locais restaurados.\n";
    } else {
        std::cout << "Arquivo de backup de locais nao encontrado. Comecando do zero.\n";
    }

    file = fopen("veiculos.bin", "rb");
    if (file != NULL) {
        fread(&numVeiculos, sizeof(int), 1, file);
        fread(veiculos, sizeof(Veiculo), numVeiculos, file);
        fclose(file);
        std::cout << numVeiculos << " veiculos restaurados.\n";
    } else {
        std::cout << "Arquivo de backup de veiculos nao encontrado. Comecando do zero.\n";
    }

    file = fopen("pedidos.bin", "rb");
    if (file != NULL) {
        fread(&numPedidos, sizeof(int), 1, file);
        fread(&proximoIdPedido, sizeof(int), 1, file);
        fread(pedidos, sizeof(Pedido), numPedidos, file);
        fclose(file);
        std::cout << numPedidos << " pedidos restaurados.\n";
    } else {
        std::cout << "Arquivo de backup de pedidos nao encontrado. Comecando do zero.\n";
    }
    
    std::cout << "\nRestauracao de dados concluida!\n";
}


// --- FUNÇÕES DE GERENCIAMENTO DE LOCAIS ---

int buscarLocalPorNome(const char* nome) {
    for (int i = 0; i < numLocais; ++i) {
        if (strcmp(locais[i].nome, nome) == 0) return i;
    }
    return -1;
}

void cadastrarLocal() {
    if (numLocais >= MAX_ENTIDADES) { std::cout << "Erro: Limite maximo de locais atingido.\n"; return; }
    Local novoLocal;
    std::cout << "Digite o nome do local: "; std::cin.getline(novoLocal.nome, MAX_NOME);
    if (buscarLocalPorNome(novoLocal.nome) != -1) { std::cout << "Erro: Ja existe um local com este nome.\n"; return; }
    std::cout << "Digite a coordenada X: "; std::cin >> novoLocal.coordenadaX;
    std::cout << "Digite a coordenada Y: "; std::cin >> novoLocal.coordenadaY;
    std::cin.ignore(10000, '\n');
    locais[numLocais++] = novoLocal;
    std::cout << "\nLocal cadastrado com sucesso!\n";
}

void listarLocais() {
    if (numLocais == 0) { std::cout << "Nenhum local cadastrado.\n"; return; }
    for (int i = 0; i < numLocais; ++i) {
        std::cout << "------------------------------------\n";
        std::cout << "Nome: " << locais[i].nome << " | Coordenadas: (" << locais[i].coordenadaX << ", " << locais[i].coordenadaY << ")\n";
    }
}

void atualizarLocal() {
    char nomeBusca[MAX_NOME];
    std::cout << "Digite o nome do local a atualizar: "; std::cin.getline(nomeBusca, MAX_NOME);
    int i = buscarLocalPorNome(nomeBusca);
    if (i == -1) { std::cout << "Erro: Local nao encontrado.\n"; return; }
    std::cout << "Digite o novo nome: "; std::cin.getline(locais[i].nome, MAX_NOME);
    std::cout << "Digite a nova coordenada X: "; std::cin >> locais[i].coordenadaX;
    std::cout << "Digite a nova coordenada Y: "; std::cin >> locais[i].coordenadaY;
    std::cin.ignore(10000, '\n');
    std::cout << "\nLocal atualizado com sucesso!\n";
}

void excluirLocal() {
    char nomeBusca[MAX_NOME];
    std::cout << "Digite o nome do local a excluir: "; std::cin.getline(nomeBusca, MAX_NOME);
    int indice = buscarLocalPorNome(nomeBusca);
    if (indice == -1) { std::cout << "Erro: Local nao encontrado.\n"; return; }
    for (int i = indice; i < numLocais - 1; ++i) { locais[i] = locais[i + 1]; }
    numLocais--;
    std::cout << "\nLocal excluido com sucesso!\n";
}

void gerenciarLocais() {
    int opcao;
    do {
        system("cls || clear");
        std::cout << "--- Gerenciamento de Locais ---\n1. Cadastrar\n2. Listar\n3. Atualizar\n4. Excluir\n0. Voltar\nEscolha: ";
        std::cin >> opcao;
        std::cin.ignore(10000, '\n');
        system("cls || clear");
        switch (opcao) {
            case 1: cadastrarLocal(); break;
            case 2: listarLocais(); break;
            case 3: atualizarLocal(); break;
            case 4: excluirLocal(); break;
        }
        if(opcao != 0){ std::cout << "\nPressione Enter para continuar..."; std::cin.get(); }
    } while (opcao != 0);
}

// --- FUNÇÕES DE GERENCIAMENTO DE VEÍCULOS ---

int buscarVeiculoPorPlaca(const char* placa) {
    for (int i = 0; i < numVeiculos; ++i) {
        if (strcmp(veiculos[i].placa, placa) == 0) return i;
    }
    return -1;
}

void cadastrarVeiculo() {
    if (numVeiculos >= MAX_ENTIDADES) { std::cout << "Erro: Limite de veiculos atingido.\n"; return; }
    if (numLocais == 0) { std::cout << "Erro: Cadastre um local primeiro.\n"; return; }
    Veiculo novo;
    std::cout << "Digite a placa: "; std::cin.getline(novo.placa, MAX_PLACA);
    if (buscarVeiculoPorPlaca(novo.placa) != -1) { std::cout << "Erro: Placa ja existe.\n"; return; }
    std::cout << "Digite o modelo: "; std::cin.getline(novo.modelo, MAX_MODELO);
    std::cout << "Digite o local atual: "; std::cin.getline(novo.localAtual, MAX_NOME);
    if (buscarLocalPorNome(novo.localAtual) == -1) { std::cout << "Erro: Local nao existe.\n"; return; }
    strcpy(novo.status, "disponivel");
    veiculos[numVeiculos++] = novo;
    std::cout << "\nVeiculo cadastrado com sucesso!\n";
}

void listarVeiculos() {
    if (numVeiculos == 0) { std::cout << "Nenhum veiculo cadastrado.\n"; return; }
    for (int i = 0; i < numVeiculos; ++i) {
        std::cout << "------------------------------------\n";
        std::cout << "Placa: " << veiculos[i].placa << " | Modelo: " << veiculos[i].modelo << "\n";
        std::cout << "Status: " << veiculos[i].status << " | Local: " << veiculos[i].localAtual << "\n";
    }
}

void atualizarVeiculo() {
    char placaBusca[MAX_PLACA];
    std::cout << "Digite a placa do veiculo a atualizar: "; std::cin.getline(placaBusca, MAX_PLACA);
    int i = buscarVeiculoPorPlaca(placaBusca);
    if (i == -1) { std::cout << "Erro: Veiculo nao encontrado.\n"; return; }
    std::cout << "Digite o novo modelo: "; std::cin.getline(veiculos[i].modelo, MAX_MODELO);
    std::cout << "Digite o novo status (disponivel/ocupado): "; std::cin.getline(veiculos[i].status, MAX_STATUS);
    std::cout << "Digite o novo local: "; std::cin.getline(veiculos[i].localAtual, MAX_NOME);
    if (buscarLocalPorNome(veiculos[i].localAtual) == -1) { std::cout << "Erro: Novo local nao existe.\n"; return; }
    std::cout << "\nVeiculo atualizado com sucesso!\n";
}

void excluirVeiculo() {
    char placaBusca[MAX_PLACA];
    std::cout << "Digite a placa do veiculo a excluir: "; std::cin.getline(placaBusca, MAX_PLACA);
    int indice = buscarVeiculoPorPlaca(placaBusca);
    if (indice == -1) { std::cout << "Erro: Veiculo nao encontrado.\n"; return; }
    for (int i = indice; i < numVeiculos - 1; ++i) { veiculos[i] = veiculos[i + 1]; }
    numVeiculos--;
    std::cout << "\nVeiculo excluido com sucesso!\n";
}

void gerenciarVeiculos() {
    int opcao;
    do {
        system("cls || clear");
        std::cout << "--- Gerenciamento de Veiculos ---\n1. Cadastrar\n2. Listar\n3. Atualizar\n4. Excluir\n0. Voltar\nEscolha: ";
        std::cin >> opcao;
        std::cin.ignore(10000, '\n');
        system("cls || clear");
        switch (opcao) {
            case 1: cadastrarVeiculo(); break;
            case 2: listarVeiculos(); break;
            case 3: atualizarVeiculo(); break;
            case 4: excluirVeiculo(); break;
        }
        if(opcao != 0){ std::cout << "\nPressione Enter para continuar..."; std::cin.get(); }
    } while (opcao != 0);
}

// --- FUNÇÕES DE GERENCIAMENTO DE PEDIDOS ---

int buscarPedidoPorId(int id) {
    for (int i = 0; i < numPedidos; ++i) {
        if (pedidos[i].id == id) return i;
    }
    return -1;
}

void cadastrarPedido() {
    if (numPedidos >= MAX_ENTIDADES) { std::cout << "Erro: Limite de pedidos atingido.\n"; return; }
    if (numLocais < 2) { std::cout << "Erro: Cadastre pelo menos 2 locais.\n"; return; }
    Pedido novo;
    novo.id = proximoIdPedido;
    std::cout << "Digite o local de ORIGEM: "; std::cin.getline(novo.localOrigem, MAX_NOME);
    if (buscarLocalPorNome(novo.localOrigem) == -1) { std::cout << "Erro: Local de origem nao existe.\n"; return; }
    std::cout << "Digite o local de DESTINO: "; std::cin.getline(novo.localDestino, MAX_NOME);
    if (buscarLocalPorNome(novo.localDestino) == -1) { std::cout << "Erro: Local de destino nao existe.\n"; return; }
    std::cout << "Digite o peso (kg): "; std::cin >> novo.peso;
    std::cin.ignore(10000, '\n');
    pedidos[numPedidos++] = novo;
    proximoIdPedido++;
    std::cout << "\nPedido ID " << novo.id << " cadastrado com sucesso!\n";
}

void listarPedidos() {
    if (numPedidos == 0) { std::cout << "Nenhum pedido cadastrado.\n"; return; }
    for (int i = 0; i < numPedidos; ++i) {
        std::cout << "------------------------------------\n";
        std::cout << "ID: " << pedidos[i].id << " | Peso: " << pedidos[i].peso << " kg\n";
        std::cout << "Origem: " << pedidos[i].localOrigem << " -> Destino: " << pedidos[i].localDestino << "\n";
    }
}

void atualizarPedido() {
    int idBusca;
    std::cout << "Digite o ID do pedido a atualizar: "; std::cin >> idBusca;
    std::cin.ignore(10000, '\n');
    int i = buscarPedidoPorId(idBusca);
    if (i == -1) { std::cout << "Erro: Pedido nao encontrado.\n"; return; }
    std::cout << "Digite a nova ORIGEM: "; std::cin.getline(pedidos[i].localOrigem, MAX_NOME);
    if (buscarLocalPorNome(pedidos[i].localOrigem) == -1) { std::cout << "Erro: Origem nao existe.\n"; return; }
    std::cout << "Digite o novo DESTINO: "; std::cin.getline(pedidos[i].localDestino, MAX_NOME);
    if (buscarLocalPorNome(pedidos[i].localDestino) == -1) { std::cout << "Erro: Destino nao existe.\n"; return; }
    std::cout << "Digite o novo peso (kg): "; std::cin >> pedidos[i].peso;
    std::cin.ignore(10000, '\n');
    std::cout << "\nPedido atualizado com sucesso!\n";
}

void excluirPedido() {
    int idBusca;
    std::cout << "Digite o ID do pedido a excluir: "; std::cin >> idBusca;
    std::cin.ignore(10000, '\n');
    int indice = buscarPedidoPorId(idBusca);
    if (indice == -1) { std::cout << "Erro: Pedido nao encontrado.\n"; return; }
    for (int i = indice; i < numPedidos - 1; ++i) { pedidos[i] = pedidos[i + 1]; }
    numPedidos--;
    std::cout << "\nPedido excluido com sucesso!\n";
}

void gerenciarPedidos() {
    int opcao;
    do {
        system("cls || clear");
        std::cout << "--- Gerenciamento de Pedidos ---\n1. Cadastrar\n2. Listar\n3. Atualizar\n4. Excluir\n0. Voltar\nEscolha: ";
        std::cin >> opcao;
        std::cin.ignore(10000, '\n');
        system("cls || clear");
        switch (opcao) {
            case 1: cadastrarPedido(); break;
            case 2: listarPedidos(); break;
            case 3: atualizarPedido(); break;
            case 4: excluirPedido(); break;
        }
        if(opcao != 0){ std::cout << "\nPressione Enter para continuar..."; std::cin.get(); }
    } while (opcao != 0);
}

// --- FUNÇÃO DE ROTA E DISTÂNCIA ---

double calcularDistancia(const Local& local1, const Local& local2) {
    long long dx = local1.coordenadaX - local2.coordenadaX;
    long long dy = local1.coordenadaY - local2.coordenadaY;
    return sqrt(dx * dx + dy * dy);
}

void calcularRota() {
    if (numPedidos == 0) { std::cout << "Nao ha pedidos pendentes.\n"; return; }
    if (numVeiculos == 0) { std::cout << "Nao ha veiculos cadastrados.\n"; return; }
    
    int idPedido;
    std::cout << "Digite o ID do pedido: "; std::cin >> idPedido;
    std::cin.ignore(10000, '\n');
    int iPedido = buscarPedidoPorId(idPedido);
    if (iPedido == -1) { std::cout << "Erro: Pedido nao encontrado.\n"; return; }

    int iOrigem = buscarLocalPorNome(pedidos[iPedido].localOrigem);
    int iDestino = buscarLocalPorNome(pedidos[iPedido].localDestino);

    int iVeiculo = -1;
    double menorDist = -1.0;

    for (int i = 0; i < numVeiculos; ++i) {
        if (strcmp(veiculos[i].status, "disponivel") == 0) {
            int iLocalVeiculo = buscarLocalPorNome(veiculos[i].localAtual);
            double d = calcularDistancia(locais[iLocalVeiculo], locais[iOrigem]);
            if (iVeiculo == -1 || d < menorDist) {
                menorDist = d;
                iVeiculo = i;
            }
        }
    }

    if (iVeiculo == -1) { std::cout << "Nao ha veiculos disponiveis.\n"; return; }

    double distEntrega = calcularDistancia(locais[iOrigem], locais[iDestino]);
    double distTotal = menorDist + distEntrega;

    std::cout << "\n--- Rota Calculada ---\n";
    std::cout << "Veiculo: " << veiculos[iVeiculo].modelo << " (" << veiculos[iVeiculo].placa << ")\n";
    std::cout << "Origem: " << pedidos[iPedido].localOrigem << "\n";
    std::cout << "Destino: " << pedidos[iPedido].localDestino << "\n";
    std::cout << "Distancia Total: " << distTotal << " unidades\n\n";
    
    std::cout << "Simulando entrega...\n";
    strcpy(veiculos[iVeiculo].localAtual, pedidos[iPedido].localDestino);
    std::cout << "Entrega finalizada! Veiculo agora esta em '" << veiculos[iVeiculo].localAtual << "'.\n";

    excluirPedido(); // Usando a mesma lógica de exclusão por ID para remover o pedido concluído
}