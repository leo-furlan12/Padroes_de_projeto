#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <map>
#include <iomanip> // Para alinhar textos (setw)
#include <limits>  // Para limpar buffer
#include <thread>  // Para dar um tempinho visual (sleep)
#include <chrono>

// ==========================================
// 1. UTILITÁRIOS VISUAIS
// ==========================================
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void imprimirLogo() {
    std::cout << "\n";
    std::cout << "  _    _                  _                                     _            _           _ \n";
    std::cout << " | |  | |                | |                                   (_)          | |         | |\n";
    std::cout << " | |__| | __ _ _ __ ___  | |__  _   _ _ __ __ _ _   _  ___ _ __ _  __ _   __| | ___     | |\n";
    std::cout << " |  __  |/ _` | '_ ` _ \\ | '_ \\| | | | '__/ _` | | | |/ _ \\ '__| |/ _` | / _` |/ _ \\    | |\n";
    std::cout << " | |  | | (_| | | | | | || |_) | |_| | | | (_| | |_| |  __/ |  | | (_| || (_| | (_) |   |_|\n";
    std::cout << " |_|  |_|\\__,_|_| |_| |_||_.__/ \\__,_|_|  \\__, |\\__,_|\\___|_|  |_|\\__,_| \\__,_|\\___/    (_)\n";
    std::cout << "                                           __/ |                                           \n";
    std::cout << "                                          |___/       [CHAIN OF RESPONSIBILITY]            \n";
    std::cout << "          >>> O MELHOR LANCHE ARTESANAL DA CIDADE <<<\n";
    std::cout << "\n";
}

void cabecalho(std::string titulo) {
    limparTela();
    std::cout << "+==================================================+\n";
    std::cout << "| " << std::left << std::setw(48) << titulo << " |\n";
    std::cout << "+==================================================+\n\n";
}

void linha() {
    std::cout << "----------------------------------------------------\n";
}

void pausa() {
    std::cout << "\n[Enter] para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// ==========================================
// 2. SUBSISTEMAS BÁSICOS
// ==========================================

class SistemaAuth {
private: const std::string arq = "usuarios.txt";
public:
    bool cadastrar(std::string u, std::string p) {
        std::ifstream r(arq); std::string tu, tp;
        while(r>>tu>>tp) if(tu==u) return false;
        r.close(); std::ofstream w(arq, std::ios::app);
        w<<u<<" "<<p<<"\n"; w.close(); return true;
    }
    bool logar(std::string u, std::string p) {
        std::ifstream r(arq); std::string tu, tp;
        if(!r.is_open()) return false;
        while(r>>tu>>tp) if(tu==u && tp==p) return true;
        return false;
    }
};

class Estoque {
private: std::map<std::string, int> itens; const std::string arq = "estoque.txt";
public:
    Estoque() { 
        std::ifstream f(arq);
        if(!f.is_open()) {
            itens["Brioche"]=50; itens["Australiano"]=50; itens["Gergelim"]=50; itens["Parmesao"]=30;
            itens["Angus"]=50; itens["Frango"]=40; itens["Vegano"]=20; itens["Picanha"]=30;
            itens["Cheddar"]=100; itens["Bacon"]=100; itens["Salada"]=100; itens["Maionese_Verde"]=100;
            itens["Picles"]=50; itens["Barbecue"]=100; itens["Cebola_Caramelizada"]=50;
            salvar(); return;
        }
        std::string n; int q; while(f>>n>>q) itens[n]=q; f.close();
    }
    void salvar() { std::ofstream f(arq); for(auto const& [n,q]:itens) f<<n<<" "<<q<<"\n"; f.close(); }
    bool temEstoque(std::string n) { return itens[n] > 0; }
    void baixarEstoque(std::string n) { if(itens[n]>0) itens[n]--; salvar(); }
    int ver(std::string n) { return itens[n]; }
};

class Hamburguer {
public:
    std::string cliente, pao, carne; std::vector<std::string> extras;
    std::string log() const {
        std::string s="CLIENTE: "+cliente+" | "+pao+", "+carne;
        if(!extras.empty()) { s+=" + ("; for(auto& e:extras) s+=e+" "; s+=")"; } return s;
    }
    void nota() const {
        std::cout << "\n";
        std::cout << "****************************************************\n";
        std::cout << "              NOTA FISCAL - LEOZAO                  \n";
        std::cout << "****************************************************\n\n";
        std::cout << " Cliente: " << cliente << "\n";
        linha();
        std::cout << " ITEM PRINCIPAL:\n";
        std::cout << "  [O] Pao " << pao << "\n";
        std::cout << "  [O] Carne " << carne << "\n\n";
        if(!extras.empty()) {
            std::cout << " ADICIONAIS:\n";
            for(auto& e:extras) std::cout << "  [+] " << e << "\n";
        }
        std::cout << "\n";
        std::cout << "****************************************************\n";
        std::cout << "         PEDIDO PROCESSADO COM SUCESSO!             \n";
        std::cout << "****************************************************\n\n";
    }
};

// ==========================================
// 3. ESTRUTURA CHAIN OF RESPONSIBILITY
// ==========================================

struct PedidoRequest {
    std::string nomeCliente;
    std::string nomeAtendente;
    std::string paoDesejado;
    std::string carneDesejada;
    std::vector<std::string> extrasDesejados;
    
    Hamburguer produtoFinal;
    bool abortado = false;
    std::string msgErro;
};

class Handler {
protected: Handler* proximo = nullptr;
public:
    virtual ~Handler() {}
    void setProximo(Handler* h) { proximo = h; }
    virtual void processar(PedidoRequest& req) {
        if (proximo && !req.abortado) {
            proximo->processar(req);
        }
    }
};

// ELO 1: ESTOQUE (Visual Melhorado)
class EstoqueHandler : public Handler {
private: Estoque* estoque;
public:
    EstoqueHandler(Estoque* e) : estoque(e) {}
    
    void processar(PedidoRequest& req) override {
        std::cout << " [1] Estoque  : Verificando disponibilidade... ";
        delay(600); // Efeito visual de processamento
        
        bool erro = false;
        if (!estoque->temEstoque(req.paoDesejado)) { req.msgErro = "Sem Pao: " + req.paoDesejado; erro=true; }
        else if (!estoque->temEstoque(req.carneDesejada)) { req.msgErro = "Sem Carne: " + req.carneDesejada; erro=true; }
        else {
            for (auto& extra : req.extrasDesejados) {
                if (!estoque->temEstoque(extra)) { req.msgErro = "Sem Extra: " + extra; erro=true; break; }
            }
        }

        if(erro) {
            req.abortado = true;
            std::cout << "[FALHOU] X\n";
        } else {
            estoque->baixarEstoque(req.paoDesejado);
            estoque->baixarEstoque(req.carneDesejada);
            for(auto& e : req.extrasDesejados) estoque->baixarEstoque(e);
            std::cout << "[OK] V\n";
            Handler::processar(req);
        }
    }
};

// ELO 2: COZINHA
class CozinhaHandler : public Handler {
public:
    void processar(PedidoRequest& req) override {
        std::cout << " [2] Cozinha  : Montando e embalando...        ";
        delay(600);
        
        req.produtoFinal.cliente = req.nomeCliente;
        req.produtoFinal.pao = req.paoDesejado;
        req.produtoFinal.carne = req.carneDesejada;
        req.produtoFinal.extras = req.extrasDesejados;
        
        std::cout << "[OK] V\n";
        Handler::processar(req);
    }
};

// ELO 3: CAIXA
class LogHandler : public Handler {
public:
    void processar(PedidoRequest& req) override {
        std::cout << " [3] Sistema  : Registrando venda no log...    ";
        delay(600);
        
        std::string arq = "vendas_" + req.nomeAtendente + ".txt";
        std::ofstream f(arq, std::ios::app);
        if (f.is_open()) {
            f << req.produtoFinal.log() << "\n";
            f.close();
            std::cout << "[OK] V\n";
        } else {
            std::cout << "[ERRO] X\n";
        }
    }
};

// ==========================================
// 4. HELPER DE MENU (O mesmo bonito do Facade)
// ==========================================
std::string menuVisual(std::string titulo, std::vector<std::string> lista, Estoque& est, bool voltar) {
    while(true) {
        std::vector<std::string> disp;
        for(auto& i:lista) if(est.ver(i)>0) disp.push_back(i);
        
        if(disp.empty()) { std::cout<<" (Esgotado!)\n"; return "NADA"; }
        
        std::cout << "=== " << titulo << " ===\n";
        for(size_t i=0; i<disp.size(); i++) {
            std::cout << " [" << (i+1) << "] " << std::left << std::setw(25) << disp[i] << "\n";
        }
        linha();
        if(voltar) std::cout << " [0] PRONTO / FINALIZAR\n";
        
        std::cout << "\n>> Opcao: "; int op;
        if(!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(); op=-1; } // Proteção
        
        if(voltar && op==0) return "VOLTAR";
        if(op>0 && op<=(int)disp.size()) {
            return disp[op-1]; // Retorna string, a validação final será no Handler do Estoque
        } else std::cout<<"\n[!] Invalido.\n\n";
    }
}

// ==========================================
// 5. MAIN
// ==========================================
int main() {
    SistemaAuth auth;
    Estoque estoque;
    
    // Configura Cadeia
    EstoqueHandler* elo1 = new EstoqueHandler(&estoque);
    CozinhaHandler* elo2 = new CozinhaHandler();
    LogHandler* elo3 = new LogHandler();
    elo1->setProximo(elo2);
    elo2->setProximo(elo3);

    // Listas
    std::vector<std::string> paes = {"Brioche", "Australiano", "Gergelim", "Parmesao"};
    std::vector<std::string> carnes = {"Angus", "Frango", "Vegano", "Picanha"};
    std::vector<std::string> extras = {"Cheddar", "Bacon", "Salada", "Maionese_Verde", "Picles", "Barbecue", "Cebola_Caramelizada"};

    while(true) {
        limparTela();
        imprimirLogo();
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| MENU INICIAL (Chain of Responsibility)           |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| 1. Entrar (Login)                                |\n";
        std::cout << "| 2. Cadastrar Funcionario                         |\n";
        std::cout << "| 3. Sair                                          |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "\n>> Opcao: "; int op;
        if(!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(); op=0; }

        if (op == 3) break;
        if (op == 2) {
            cabecalho("NOVO CADASTRO");
            std::string u, p;
            std::cout << "Novo Usuario: "; std::cin >> u;
            std::cout << "Nova Senha:   "; std::cin >> p;
            linha();
            if (auth.cadastrar(u, p)) std::cout << "[OK] Cadastrado com sucesso!\n";
            else std::cout << "[ERRO] Usuario ja existe!\n";
            pausa();
        }
        if (op == 1) {
            cabecalho("LOGIN");
            std::string u, s;
            std::cout << "Usuario: "; std::cin >> u;
            std::cout << "Senha:   "; std::cin >> s;

            if (auth.logar(u, s)) {
                while(true) {
                    cabecalho("AREA DO ATENDENTE: " + u);
                    std::cout << " 1. Novo Pedido (Disparar Cadeia)\n 2. Relatorio de Vendas\n 3. Deslogar\n";
                    linha();
                    std::cout << "\n>> Opcao: "; int x; 
                    if(!(std::cin >> x)) { std::cin.clear(); std::cin.ignore(); x=0; }
                    
                    if (x == 3) break;
                    if (x == 2) {
                        std::string arq = "vendas_" + u + ".txt";
                        cabecalho("LENDO ARQUIVO: " + arq);
                        std::ifstream f(arq); std::string l;
                        if(!f.is_open()) std::cout << "(Sem registros)\n";
                        while(std::getline(f,l)) std::cout << " - " << l << "\n";
                        pausa();
                    }
                    if (x == 1) {
                        PedidoRequest req;
                        req.nomeAtendente = u;
                        cabecalho("PREENCHENDO PEDIDO");
                        std::cout << "Nome do Cliente: "; std::cin >> req.nomeCliente;

                        limparTela();
                        req.paoDesejado = menuVisual("ESCOLHA O PAO", paes, estoque, false);
                        
                        limparTela();
                        req.carneDesejada = menuVisual("ESCOLHA A CARNE", carnes, estoque, false);

                        while(true) {
                            limparTela();
                            std::string e = menuVisual("ADICIONAR EXTRAS", extras, estoque, true);
                            if(e=="VOLTAR" || e=="NADA") break;
                            req.extrasDesejados.push_back(e);
                        }

                        // --- O MOMENTO MÁGICO DO CHAIN ---
                        limparTela();
                        std::cout << ">>> INICIANDO ESTEIRA DE PROCESSAMENTO <<<\n\n";
                        
                        elo1->processar(req); // Dispara no primeiro elo

                        if (req.abortado) {
                            std::cout << "\n+--------------------------------------------------+\n";
                            std::cout << "| [!] ERRO CRITICO NA CADEIA                       |\n";
                            std::cout << "| Motivo: " << std::left << std::setw(40) << req.msgErro << " |\n";
                            std::cout << "+--------------------------------------------------+\n";
                        } else {
                            req.produtoFinal.nota();
                        }
                        pausa();
                    }
                }
            } else {
                std::cout << "\n[ERRO] Login invalido.\n"; pausa();
            }
        }
    }
    
    delete elo1; delete elo2; delete elo3;
    return 0;
}