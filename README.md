# AC3-trabalho2 - Algoritmo de Tomasulo
Trabalho II de Arquitetura de Computadores III - Algoritmo de Tomasulo para sistemas superescalares
Código se encontra em src, e em results se encontra a saida com o arquivo teste realizado.

## Alunos
- Bruno Hermeto Guimarães
- Enzo de Melo Bolognani
- Fernando Valle Gusmão

## Classes
### Instrucao
Classe usada para instruções, contendo o tipo de operação, registradores usados e dados sobre o ciclo de ínicio/fim das etapas da simulação.

### EstacaoReserva
Classe utilizada para as estações de reserva, contendo o tipo de operação feita, valores dos operandos, ID da estação que vai produzir Vj e Vk, resultado da operação e quantidade de ciclos restantes.

### Registrador
Classe utilizada para os registradores, contendo o valor contido e ID da estação de reserva que vai escrever.

### BancoRegistradores
Classe utilizada como banco de registradores, contendo um vetor de 32 registradores.

## Implementação do Algoritmo
### `TomasuloEstruturas.hpp`
Contém todas as classes utilizadas no algoritmo.

### `Leitura.cpp`
Contém a função para fazer a leitura e o parse do arquivo `instrucoes.txt`, já colocando as instruções na fila.

### `Tomasulo.cpp`
Contém as funções das três etapas do algoritmo de Tomasulo: `etapaIssue()`, `etapaExecute()` e `etapaWriteBack()`.

### `Mostrar.cpp`
Contém a função que imprime no terminal e escreve no arquivo `log.txt`, as tabelas de instruções, banco de registradores e estações de reserva a cada ciclo.

## Operações suportadas
- LOAD (LW)
- STORE (SW)
- ADD
- SUB
- MUL
- DIV

## Arquitetura
- 32 registradores (Cada um inicia com um valor aleatória de 0 a 100)
- Estações de reserva configuráveis (ADD/SUB: 3 Estações, MUL/DIV: 2 Estações, LOAD: 2 Estações e STORE: 2 Estações)
- Ciclos por instrução:
    - SW: 1 Ciclo
    - ADD/SUB/LW: 2 Ciclos
    - MUL: 10 Ciclos
    - DIV: 40 Ciclos

## Como usar
1. Compilar usando `g++ main.cpp tomasulo.cpp mostrar.cpp leitura.cpp -o simulador`
2. Executar `./simulador`

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
