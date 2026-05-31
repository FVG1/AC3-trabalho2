#pragma once
#include "TomasuloEstruturas.hpp"
#include <vector>
#include <string>
#include <ostream>

using namespace std;

void exibirEstadoDoSimulador(int cicloAtual, ostream& out, const vector<Instrucao>& instrucoes, const vector<EstacaoReserva>& estacoes, const BancoRegistradores& banco);