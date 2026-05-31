#pragma once
#include <vector>
#include <string>
#include "TomasuloEstruturas.hpp"

using namespace std;

void etapaIssue(int cicloAtual, vector<Instrucao>& fila, vector<EstacaoReserva>& estacoes, BancoRegistradores& banco, vector<Instrucao>& historico);
void etapaExecute(int cicloAtual, vector<EstacaoReserva>& estacoes, vector<Instrucao>& historico);
void etapaWriteBack(int cicloAtual, vector<EstacaoReserva>& estacoes, BancoRegistradores& banco, vector<Instrucao>& historico);