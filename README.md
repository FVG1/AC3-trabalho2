# AC3-trabalho2 - Algoritmo de Tomasulo
Projeto de AC3 do algoritmo de Tomasulo para sistemas superescalares de Fernando Valle Gusmão, Bruno Hermeto Guimarães e Enzo de Melo Bolognani

Executa-se por base do arquivo instrucoes.txt.

Código se encontra em src, results contém a saida com o arquivo teste realizado

# Classes
## Instrucao
Classe usada para instruções, contendo o tipo de operação, registradores usados e dados sobre o ciclo de ínicio/fim das etapas da simulação.

## EstacaoReserva
Classe utilizada para as estações de reserva, contendo o tipo de operação feita, valores dos operandos, ID da estação que vai produzir Vj e Vk, resultado da operação e quantidade de ciclos restantes.

## Registrador
Classe utilizada para os registradores, contendo o valor contido e ID da estação de reserva que vai escrever.

## BancoRegistradores
Classe utilizada como banco de registradores, contendo um vetor de 32 registradores.

# Implementação do Algoritmo
## `TomasuloEstruturas.hpp`
Contém todas as classes utilizadas no algoritmo.

## `Leitura.cpp`
Contém a função para fazer a leitura e o parse do arquivo `instrucoes.txt`, já colocando as instruções na fila.

## `Tomasulo.cpp`
Contém as funções das três etapas do algoritmo de Tomasulo: `etapaIssue()`, `etapaExecute()` e `etapaWriteBack()`.

## `Mostrar.cpp`
Contém a função que imprime no terminal e escreve no arquivo `log.txt`, as tabelas de instruções, banco de registradores e estações de reserva a cada ciclo.

# Operações suportadas
- LOAD (LW)
- STORE (SW)
- ADD
- SUB
- MUL
- DIV

# Arquitetura
- 32 registradores (Cada um inicia com um valor aleatória de 0 a 100)
- Estações de reserva configuráveis (ADD/SUB: 3 Estações, MUL/DIV: 2 Estações, LOAD: 2 Estações e STORE: 2 Estações)
- Ciclos por instrução:
    - SW: 1 Ciclo
    - ADD/SUB/LW: 2 Ciclos
    - MUL: 10 Ciclos
    - DIV: 40 Ciclos

# Como usar
1. Compilar usando `g++ main.cpp tomasulo.cpp mostrar.cpp leitura.cpp -o simulador`
2. Executar `./simulador`
  

struct Instrucao {
    string op;          // Operação: "ADD", "SUB", "MUL", "DIV", "LW", "SW"
    int rd;                  // ID do Registrador Destino
    int rs;                  // ID do Registrador Fonte J
    int rt;                  // ID do Registrador Fonte K
    // Controle de tempo/ciclos
    int cicloIssue;         // Ciclo em que a instrução foi despachada
    int cicloExecuteInicio; // Ciclo em que a instrução iniciou a execução
    int cicloExecuteFim;    // Ciclo em que a instrução terminou a execução
    int cicloWriteback;     // Ciclo em que escreveu o resultado no CDB

    // Construtor
    Instrucao(string _op = "", int _rd = 0, int _rs = 0, int _rt = 0)
        : op(_op), rd(_rd), rs(_rs), rt(_rt), 
          cicloIssue(-1), cicloExecuteInicio(0), cicloExecuteFim(0), cicloWriteback(0) {}
};


struct EstacaoReserva {
    string id;          // Identificador da estação (ex: "Add1", "Add2", "Mul1")
    bool busy;          // Indica se a estação está ocupada ou livre
    string op;          // Operação que está realizando (ex: "ADD", "MUL")
    int vj;             // Valor real do operando J
    int vk;             // Valor real do operando K
    string qj;          // ID da estação que vai produzir Vj (vazio "" se o valor já estiver pronto/válido)
    string qk;          // ID da estação que vai produzir Vk (vazio "" se o valor já estiver pronto/válido)
    int tempoRestante;  // Quantidade de ciclos que faltam para terminar a execução
    int resultado;      // Guarda o resultado da operação
    int idInstrucao;    // Variável para guardar os dados do ciclo para mostrar na saida

    // Construtor
    EstacaoReserva(string _id = "")
        : id(_id), busy(false), op(""), vj(0), vk(0), qj(""), qk(""), tempoRestante(-1), resultado(0), idInstrucao(-1) {}
};


struct Registrador {
    int valor;               // Valor numérico real contido no registrador
    string qi;          // ID da estação de reserva que vai escrever aqui (vazio "" se o dado for válido)

    // Construtor
    Registrador() : valor(0), qi("") {}
};


struct BancoRegistradores {
    Registrador regs[32];

    // Função para inicializar o banco de registradores
    void inicializar() {
        srand(time(0));
        regs[0].valor = 0;
        regs[0].qi = "";
        for (int i = 1; i < 32; ++i) {
            regs[i].valor = rand() % 101; // Valores aleatórios de 0 a 100
            regs[i].qi = "";
        }
    }
};

## Exemplo de Execução
### Entrada (`instrucoes.txt`)
```text
ADD R1 R2 R3
MUL R4 R1 R2
SUB R5 R4 R1
DIV R6 R8 R5
LW R9 0(R10)
ADD R11 R9 R12
SW R11 4(R10)
```
### Saída representando um ciclo da execução
```text
====================================================================================
 STATUS DO PROCESSADOR - CICLO: 1
====================================================================================

1. TABELA DE INSTRUCOES
-----------------------------------------------------------------------------
Instrucao   Suj. (RD) RS        RT        | Issue   Execute     Writeback 
-----------------------------------------------------------------------------
LW R1       R1        R2        R0        | 1       -           -         
ADD R3      R3        R1        R4        | -       -           -         
SW R3       R3        R2        R0        | -       -           -         

2. ESTACOES DE RESERVA
-----------------------------------------------------------------------------
Nome      Busy    Op      Vj      Vk      Qj      Qk      Tempo Restante 
-----------------------------------------------------------------------------
Add1      Nao     -       -       -       -       -       -              
Add2      Nao     -       -       -       -       -       -              
Add3      Nao     -       -       -       -       -       -              
Mul1      Nao     -       -       -       -       -       -              
Mul2      Nao     -       -       -       -       -       -              
Ld1       Sim     LW      80      0       -       -       2              
Ld2       Nao     -       -       -       -       -       -              
St1       Nao     -       -       -       -       -       -              
St2       Nao     -       -       -       -       -       -              

3. BANCO DE REGISTRADORES (QI / VALOR)
-----------------------------------------------------------------------------
R0       R1       R2       R3       R4       R5       R6       R7       
0        [Ld1]    80       5        23       33       72       24       
-----------------------------------------------------------------------------
R8       R9       R10      R11      R12      R13      R14      R15      
35       70       21       88       20       80       33       71       
-----------------------------------------------------------------------------
R16      R17      R18      R19      R20      R21      R22      R23      
13       55       63       60       6        84       2        7        
-----------------------------------------------------------------------------
R24      R25      R26      R27      R28      R29      R30      R31      
1        34       49       68       1        51       83       70       
-----------------------------------------------------------------------------
```
