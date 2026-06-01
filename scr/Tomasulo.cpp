#include "TomasuloEstruturas.hpp"
#include "Tomasulo.h"
#include <iostream>

using namespace std;

// Função da etapa Issue
void etapaIssue(int cicloAtual, vector<Instrucao>& fila, vector<EstacaoReserva>& estacoes, BancoRegistradores& banco, vector<Instrucao>& historico) {
    // Se não há mais instruções para despachar, sai da função
    if (fila.empty()) return;
    Instrucao proxInst = fila.front(); // Pega a próxima da fila
    // Acha a estação de reserva livre compatível
    int indiceEstacaoLivre = -1;
    for (int i = 0; i < estacoes.size(); i++) {
        // Estações para ADD e SUB
        if ((proxInst.op == "ADD" || proxInst.op == "SUB") && estacoes[i].id.find("Add") != string::npos && !estacoes[i].busy) {
            indiceEstacaoLivre = i;
            break;
        }
        // Estações para MUL e DIV
        if ((proxInst.op == "MUL" || proxInst.op == "DIV") && estacoes[i].id.find("Mul") != string::npos && !estacoes[i].busy) {
            indiceEstacaoLivre = i;
            break;
        }
        // Estações para LW
        if (proxInst.op == "LW" && estacoes[i].id.find("Ld") != string::npos && !estacoes[i].busy) {
            indiceEstacaoLivre = i;
            break;
        }
        // Estações para SW
        if (proxInst.op == "SW" && estacoes[i].id.find("St") != string::npos && !estacoes[i].busy) {
            indiceEstacaoLivre = i;
            break;
        }
    }
    // Se não achou estação livre, da Stall
    if (indiceEstacaoLivre == -1) {
        return; 
    }
    // Preenche a estação encontrada
    EstacaoReserva& er = estacoes[indiceEstacaoLivre];
    er.busy = true;
    er.op = proxInst.op;
    // Procura qual instrução no histórico corresponde a instrução que está saindo da fila
    for (int k = 0; k < historico.size(); k++) {
        if (historico[k].op == proxInst.op && historico[k].rd == proxInst.rd && historico[k].cicloIssue == 0) {
            historico[k].cicloIssue = cicloAtual; // Guarda o ciclo de Issue
            er.idInstrucao = k; // Guarda o id da instrução
            k = historico.size();
        }
    }
    // Checagem do operando RS
    if (banco.regs[proxInst.rs].qi == "") {
        er.vj = banco.regs[proxInst.rs].valor;
        er.qj = "";
    } else {
        er.qj = banco.regs[proxInst.rs].qi; // Copia a dependência
    }
    // Checagem do operando RT
    if (banco.regs[proxInst.rt].qi == "") {
        er.vk = banco.regs[proxInst.rt].valor;
        er.qk = "";
    } else {
        er.qk = banco.regs[proxInst.rt].qi; // Copia a dependência
    }
    // Renomeação do registrador de destino
    if (proxInst.op != "SW"){
        banco.regs[proxInst.rd].qi = er.id;
    }
    // Ajustar a latência das operações
    if (proxInst.op == "ADD" || proxInst.op == "SUB") er.tempoRestante = 2;
    if (proxInst.op == "MUL") er.tempoRestante = 10;
    if (proxInst.op == "DIV") er.tempoRestante = 40;
    if (proxInst.op == "LW") er.tempoRestante = 2;
    if (proxInst.op == "SW") er.tempoRestante = 1;
    // Avança a fila de instruções
    fila.erase(fila.begin());
}

// Função da etapa Execute
void etapaExecute(int cicloAtual, vector<EstacaoReserva>& estacoes, vector<Instrucao>& historico) {
    // Percorre todas as estações de reserva do processador (Add, Mult, Load, Store)
    for (int i = 0; i < estacoes.size(); i++) {   
        // Só mexe nas estações que estão ocupadas
        if (estacoes[i].busy) {
            // Só executa se NÃO houver dependências pendentes
            if (estacoes[i].qj == "" && estacoes[i].qk == "") {
                int idInst = estacoes[i].idInstrucao;        
                // Se a instrução ainda está processando
                if (estacoes[i].tempoRestante > 0) {
                    // Guarda o ciclo do inicio da etapa execute
                    if (historico[idInst].cicloExecuteInicio == 0) {
                        historico[idInst].cicloExecuteInicio = cicloAtual;
                    }                  
                    // Decrementa 1 do tempo restante representando a que passou 1 ciclo de clock
                    estacoes[i].tempoRestante--;       
                }
                // Se a instrução acabou de processar nesse ciclo faz a operação
                if (estacoes[i].tempoRestante == 0) {
                    historico[idInst].cicloExecuteFim = cicloAtual; // Guarda o ciclo do fim do execute
                    if (estacoes[i].op == "ADD") {
                        estacoes[i].resultado = estacoes[i].vj + estacoes[i].vk;
                    } 
                    else if (estacoes[i].op == "SUB") {
                        estacoes[i].resultado = estacoes[i].vj - estacoes[i].vk;
                    } 
                    else if (estacoes[i].op == "MUL") {
                        estacoes[i].resultado = estacoes[i].vj * estacoes[i].vk;
                    } 
                    else if (estacoes[i].op == "DIV") {
                        // Evitar divisão por 0
                        if (estacoes[i].vk != 0) {
                            estacoes[i].resultado = estacoes[i].vj / estacoes[i].vk;
                        } else {
                            estacoes[i].resultado = 0; 
                        }
                    }
                    else if (estacoes[i].op == "LW") {
                        estacoes[i].resultado = estacoes[i].vj + estacoes[i].vk; 
                    }
                    else if (estacoes[i].op == "SW") {
                        estacoes[i].resultado = estacoes[i].vj + estacoes[i].vk;
                    }
                }
            }
        }
    }
}

// Função da etapa WriteBack
void etapaWriteBack(int cicloAtual, vector<EstacaoReserva>& estacoes, BancoRegistradores& banco, vector<Instrucao>& historico) {
    // Procura qualquer estação de reserva que tenha terminado a execução
    for (int i = 0; i < estacoes.size(); i++) {
        // Tem que estar ocupado, com os operandos prontos e sem tempo restante
        if (estacoes[i].busy && estacoes[i].qj == "" && estacoes[i].qk == "" && estacoes[i].tempoRestante == 0) {
            string idEstacaoTerminou = estacoes[i].id; // ex: "Add1"
            // Pega o valor calculado no execute
            int valorCDB = estacoes[i].resultado;
            // Guarda o ciclo do write back
            int idInst = estacoes[i].idInstrucao;
            historico[idInst].cicloWriteback = cicloAtual;
            // Atualiza as outras estações que dependiam desta
            for (int j = 0; j < estacoes.size(); j++) {
                if (estacoes[j].busy) {
                    // Se o operando RS tinha dependência
                    if (estacoes[j].qj == idEstacaoTerminou) {
                        estacoes[j].vj = valorCDB; // Recebe o valor
                        estacoes[j].qj = "";       // Libera a dependência
                    }
                    // Se o operando RT tinha dependência
                    if (estacoes[j].qk == idEstacaoTerminou) {
                        estacoes[j].vk = valorCDB; // Recebe o valor
                        estacoes[j].qk = "";       // Libera a dependência
                    }
                }
            }
            // Atualiza o Banco de Registradores
            for (int r = 0; r < 32; r++) {
                if (banco.regs[r].qi == idEstacaoTerminou) {
                    banco.regs[r].valor = valorCDB; // Grava o valor
                    banco.regs[r].qi = "";          // Libera o registrador
                }
            }
            // Libera estação
            estacoes[i].busy = false;
            estacoes[i].op = "";
            return; 
        }
    }
}