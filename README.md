# AC3-trabalho2 - Algoritmo de Tomasulo
Projeto de AC3 do algoritmo de Tomasulo para sistemas superescalares de Fernando Valle Gusmão, Bruno Hermeto Magalhães e Enzo de Melo Bolognani

Executa-se por base do arquivo instrucoes.txt.

Classes

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
