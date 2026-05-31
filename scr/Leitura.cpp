#include "Leitura.h"
#include "TomasuloEstruturas.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Função auxiliar para remover o 'R' ou 'r' do nome do registrador (ex: "R1" -> 1)
int converteRegistrador(const string& regStr) {
    if (regStr[0] == 'R' || regStr[0] == 'r') {
        return stoi(regStr.substr(1));
    }
    return stoi(regStr); 
}

// Função para leitura do arquivo de instruções e fazer o parse
vector<Instrucao> lerArquivoInstrucoes(const string& nomeArquivo) {
    vector<Instrucao> filaInstrucoes;
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return filaInstrucoes;
    }
    string linha;
    // Enquanto tiver linhas no arquivo faz a leitura
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue; 
        stringstream ss(linha);
        string op, rdStr, rsStr, rtStr;
        ss >> op >> rdStr >> rsStr >> rtStr;
        // Faz o parse
        Instrucao inst;
        inst.op = op;
        inst.rd = converteRegistrador(rdStr);
        inst.rs = converteRegistrador(rsStr);
        inst.rt = converteRegistrador(rtStr);
        // Inicializa os ciclos com 0
        inst.cicloIssue = 0;
        inst.cicloExecuteInicio = 0;
        inst.cicloExecuteFim = 0;
        inst.cicloWriteback = 0;
        // Insere instrução na fila de instruções
        filaInstrucoes.push_back(inst);
    }
    // Fecha o arquivo e retorna a fila de instruções
    arquivo.close();
    return filaInstrucoes;
}