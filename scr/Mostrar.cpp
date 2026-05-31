#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Mostrar.h"
#include "TomasuloEstruturas.hpp"

using namespace std;

void exibirEstadoDoSimulador(int cicloAtual, ostream& out, const vector<Instrucao>& instrucoes, const vector<EstacaoReserva>& estacoes, const BancoRegistradores& banco) {
    out << "\n====================================================================================\n";
    out << " STATUS DO PROCESSADOR - CICLO: " << cicloAtual << "\n";
    out << "====================================================================================\n\n";
    // 1. Tabela de Status das Instruções
    out << "1. TABELA DE INSTRUCOES\n";
    out << "-----------------------------------------------------------------------------\n";
    out << left << setw(12) << "Instrucao" << setw(10) << "Suj. (RD)" << setw(10) << "RS" << setw(10) << "RT" 
         << "| " << setw(8) << "Issue" << setw(12) << "Execute" << setw(10) << "Writeback" << "\n";
    out << "-----------------------------------------------------------------------------\n";
    for (const auto& inst : instrucoes) {
        string chamada = inst.op + " R" + to_string(inst.rd);
        out << left << setw(12) << chamada 
             << setw(10) << ("R" + to_string(inst.rd)) 
             << setw(10) << ("R" + to_string(inst.rs)) 
             << setw(10) << ("R" + to_string(inst.rt)) << "| ";
        // Print dos ciclos (só mostra se já tiver acontecido, ou seja, > 0)
        if (inst.cicloIssue > 0) out << setw(8) << inst.cicloIssue; else out << setw(8) << "-";
        if (inst.cicloExecuteInicio > 0) {
            string execIntervalo = to_string(inst.cicloExecuteInicio) + ".." + to_string(inst.cicloExecuteFim);
            out << setw(12) << execIntervalo;
        } else {
            out << setw(12) << "-";
        }
        if (inst.cicloWriteback > 0) out << setw(10) << inst.cicloWriteback; else out << setw(10) << "-";
        out << "\n";
    }
    out << "\n";
    // 2. Tabela das Estações de Reserva
    out << "2. ESTACOES DE RESERVA\n";
    out << "-----------------------------------------------------------------------------\n";
    out << left << setw(10) << "Nome" << setw(8) << "Busy" << setw(8) << "Op" 
         << setw(8) << "Vj" << setw(8) << "Vk" << setw(8) << "Qj" << setw(8) << "Qk" 
         << setw(15) << "Tempo Restante" << "\n";
    out << "-----------------------------------------------------------------------------\n";
    for (const auto& er : estacoes) {
        out << left << setw(10) << er.id 
             << setw(8) << (er.busy ? "Sim" : "Nao")
             << setw(8) << (er.busy ? er.op : "-")
             << setw(8) << (er.busy && er.qj == "" ? to_string(er.vj) : "-")
             << setw(8) << (er.busy && er.qk == "" ? to_string(er.vk) : "-")
             << setw(8) << (!er.qj.empty() ? er.qj : "-")
             << setw(8) << (!er.qk.empty() ? er.qk : "-");
        if (er.busy && er.tempoRestante >= 0) {
            out << setw(15) << er.tempoRestante;
        } else {
            out << setw(15) << "-";
        }
        out << "\n";
    }
    out << "\n";
    // 3. Status dos Registradores
    out << "3. BANCO DE REGISTRADORES (QI / VALOR)\n";
    out << "-----------------------------------------------------------------------------\n";
    for (int bloco = 0; bloco < 32; bloco += 8) {
        // Linha dos nomes dos registradores
        for (int i = bloco; i < bloco + 8; ++i) {
            out << "R" << left << setw(8) << i;
        }
        out << "\n";
        // Linha do Qi (quem vai escrever) ou do valor armazenado
        for (int i = bloco; i < bloco + 8; ++i) {
            if (banco.regs[i].qi != "") {
                out << left << setw(9) << ("[" + banco.regs[i].qi + "]");
            } else {
                out << left << setw(9) << banco.regs[i].valor;
            }
        }
        out << "\n-----------------------------------------------------------------------------\n";
    }
}