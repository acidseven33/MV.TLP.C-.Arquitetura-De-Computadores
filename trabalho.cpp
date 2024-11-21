#include <iostream>
//Simução de memória Virtual// 
using namespace std;

// Estrutura para representar uma entrada na tabela de páginas
struct Pagina {
    int quadroFisico; // Quadro físico correspondente
    bool valido;      // Indica se a página está válida
};

// Estrutura para representar uma entrada na TLB
struct EntradaTLB {
    int paginaVirtual; // Número da página virtual
    int quadroFisico;  // Quadro físico correspondente
};

// Constantes
const int TAMANHO_PAGINA = 4;   // Tamanho de cada página (em unidades fictícias)
const int NUM_PAGINAS = 8;      // Número de páginas virtuais
const int NUM_QUADROS = 4;      // Número de quadros físicos
const int TAMANHO_TLB = 2;      // Número máximo de entradas na TLB

// Tabela de Páginas
Pagina tabelaPaginas[NUM_PAGINAS];

// TLB (Tabela de Tradução Rápida)
EntradaTLB tlb[TAMANHO_TLB];
int tlbIndex = 0; // Índice para manter controle da entrada mais antiga (FIFO)

// Função para inicializar a tabela de páginas
void inicializarTabelaPaginas() {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        tabelaPaginas[i].valido = false; // Todas as páginas começam inválidas
    }

    // Exemplo de mapeamentos fictícios
    tabelaPaginas[0] = {2, true}; // Página 0 -> Quadro 2
    tabelaPaginas[1] = {3, true}; // Página 1 -> Quadro 3
    tabelaPaginas[2] = {0, true}; // Página 2 -> Quadro 0
    tabelaPaginas[3] = {1, true}; // Página 3 -> Quadro 1
}

// Função para buscar uma página na TLB
int buscarNaTLB(int paginaVirtual) {
    for (int i = 0; i < TAMANHO_TLB; i++) {
        if (tlb[i].paginaVirtual == paginaVirtual) {
            return tlb[i].quadroFisico;
        }
    }
    return -1; // Página não encontrada na TLB
}

// Função para adicionar uma entrada na TLB
void adicionarNaTLB(int paginaVirtual, int quadroFisico) {
    // Substitui a entrada mais antiga se a TLB estiver cheia
    tlb[tlbIndex] = {paginaVirtual, quadroFisico};
    tlbIndex = (tlbIndex + 1) % TAMANHO_TLB; // Atualiza o índice circularmente
}

// Função para traduzir endereço virtual em físico
int traduzirEndereco(int enderecoVirtual) {
    int paginaVirtual = enderecoVirtual / TAMANHO_PAGINA; // Calcula a página virtual
    int deslocamento = enderecoVirtual % TAMANHO_PAGINA; // Calcula o deslocamento

    // 1. Busca na TLB
    int quadroFisico = buscarNaTLB(paginaVirtual);
    if (quadroFisico != -1) {
        cout << "TLB HIT: Página " << paginaVirtual << " -> Quadro " << quadroFisico << endl;
        return quadroFisico * TAMANHO_PAGINA + deslocamento;
    }

    // 2. Busca na Tabela de Páginas
    if (tabelaPaginas[paginaVirtual].valido) {
        quadroFisico = tabelaPaginas[paginaVirtual].quadroFisico;
        adicionarNaTLB(paginaVirtual, quadroFisico); // Atualiza a TLB
        cout << "TLB MISS: Página " << paginaVirtual << " -> Quadro " << quadroFisico << endl;
        return quadroFisico * TAMANHO_PAGINA + deslocamento;
    }

    // 3. Página não mapeada
    cout << "Falha de página: Página " << paginaVirtual << " não está mapeada!" << endl;
    return -1; // Indica falha de página
}

// Função principal
int main() {
    inicializarTabelaPaginas(); // Inicializa a tabela de páginas

    // Loop para traduzir endereços virtuais fornecidos pelo usuário
    int enderecoVirtual;
    cout << "Digite um endereço virtual para traduzir (ou -1 para sair):" << endl;

    while (true) {
        cin >> enderecoVirtual;
        if (enderecoVirtual == -1) break;

        int enderecoFisico = traduzirEndereco(enderecoVirtual);
        if (enderecoFisico != -1) {
            cout << "Endereço Virtual: " << enderecoVirtual
                 << " -> Endereço Físico: " << enderecoFisico << endl;
        } else {
            cout << "Erro: Tradução falhou." << endl;
        }
    }

    return 0;
}
