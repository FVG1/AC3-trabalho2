#pragma once
#include "TomasuloEstruturas.hpp" // Para reconhecer a struct Instrucao
#include <vector>
#include <string>

std::vector<Instrucao> lerArquivoInstrucoes(const std::string& nomeArquivo);