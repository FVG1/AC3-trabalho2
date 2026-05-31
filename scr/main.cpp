#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include "TomasuloEstruturas.hpp"
#include "Tomasulo.h"
#include "Mostrar.h"
#include "Leitura.h"

using namespace std;

// Função auxiliar para checar se ainda tem alguma instrução rodando
bool hardwareOcupado(const vector<EstacaoReserva>& estacoes) {
    for (const auto& er : estacoes) {
        if (er.busy) return true;
    }
    return false;
}

int main() {
    // Leitura e parse das instruções cria a fila de instruções
    string nomeArquivo = "instrucoes.txt";
    vector<Instrucao> fila = lerArquivoInstrucoes(nomeArquivo);
    // Verifica se a leitura foi feita
    if (fila.empty()) {
        cout << "Erro: Nenhuma instrucao carregada ou arquivo nao encontrado.\n";
        return 1;
    }
    // Criação do historico para guardar o ciclo de inicio ou fim de cada etapa
    vector<Instrucao> historico = fila;
    // Inicializar banco de registradores
    BancoRegistradores banco;
    banco.inicializar();
    // Inicialização das estações de reserva
    vector<EstacaoReserva> estacoes = {
        {"Add1"}, {"Add2"}, {"Add3"}, // 3 estações para ADD/SUB
        {"Mul1"}, {"Mul2"},           // 2 estações para MUL/DIV
        {"Ld1"}, {"Ld2"},             // 2 estações para LW
        {"St1"}, {"St2"}              // 2 estações para SW
    };
    // Criação arquivo de log
    ofstream arquivoLog("log.txt");
    if (!arquivoLog.is_open()) {
        cout << "Erro: Nao foi possivel criar o arquivo de log.\n";
        return 1;
    }
    int clock = 1;
    // Iniciando a simulação
    cout << "--- INICIANDO SIMULACAO DO ALGORITMO DE TOMASULO ---\n";
    // Roda enquanto tiver instruções na fila
    while (!fila.empty() || hardwareOcupado(estacoes)) {
        etapaWriteBack(clock, estacoes, banco, historico);
        etapaExecute(clock, estacoes, historico);
        etapaIssue(clock, fila, estacoes, banco, historico);
        // Mostra os dados desse ciclo de clock
        exibirEstadoDoSimulador(clock, cout, historico, estacoes, banco);
        exibirEstadoDoSimulador(clock, arquivoLog, historico, estacoes, banco);
        clock++;
    }
    arquivoLog.close();
    cout << "--- SIMULACAO CONCLUIDA COM SUCESSO EM " << clock - 1 << " CICLOS! ---\n";
    return 0;
}