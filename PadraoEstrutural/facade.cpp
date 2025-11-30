#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <map>
#include <iomanip> // Importante para o visual (setw)

// ==========================================
// 1. UTILITÁRIOS VISUAIS (UI HELPERS)
// ==========================================
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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
    std::cout << "                                          |___/           [SISTEMA FACADE v1.0]            \n";
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

// ==========================================
// SUBSISTEMAS (AUTH, ESTOQUE, BUILDER, HISTORICO)
// ==========================================

class SistemaAuth {
private: const std::string arquivoUsers = "usuarios.txt";
public:
    bool cadastrar(std::string user, std::string pass) {
        std::ifstream r(arquivoUsers); std::string u, p;
        while(r>>u>>p) if(u==user) return false;
        r.close(); std::ofstream w(arquivoUsers, std::ios::app);
        w<<user<<" "<<pass<<"\n"; w.close(); return true;
    }
    bool logar(std::string user, std::string pass) {
        std::ifstream r(arquivoUsers); std::string u, p;
        if(!r.is_open()) return false;
        while(r>>u>>p) if(u==user && p==pass) return true;
        return false;
    }
};

class Estoque {
private: std::map<std::string, int> itens; const std::string arquivoDB = "estoque.txt";
public:
    Estoque() { carregarDados(); }
    void carregarDados() {
        std::ifstream f(arquivoDB);
        if(!f.is_open()) {
            itens["Brioche"]=50; itens["Australiano"]=50; itens["Gergelim"]=50; itens["Parmesao"]=30;
            itens["Angus"]=50; itens["Frango"]=40; itens["Vegano"]=20; itens["Picanha"]=30;
            itens["Cheddar"]=100; itens["Bacon"]=100; itens["Salada"]=100; itens["Picles"]=50;
            itens["Maionese_Verde"]=100; itens["Barbecue"]=100; itens["Cebola_Caramelizada"]=40;
            salvarDados(); return;
        }
        std::string n; int q; while(f>>n>>q) itens[n]=q; f.close();
    }
    void salvarDados() { std::ofstream f(arquivoDB); for(auto const& [n,q]:itens) f<<n<<" "<<q<<"\n"; f.close(); }
    bool consumir(std::string n) { if(itens[n]>0) { itens[n]--; salvarDados(); return true; } return false; }
    int ver(std::string n) { return itens[n]; }
};

class Hamburguer {
public:
    std::string nomeCliente, pao, carne; std::vector<std::string> extras;
    std::string log() const {
        std::string s="CLIENTE: "+nomeCliente+" | "+pao+", "+carne;
        if(!extras.empty()) { s+=" + ("; for(auto& e:extras) s+=e+" "; s+=")"; } return s;
    }
    void nota() const {
        limparTela();
        std::cout << "\n";
        std::cout << "****************************************************\n";
        std::cout << "              NOTA FISCAL - LEOZAO                  \n";
        std::cout << "****************************************************\n\n";
        std::cout << " Cliente: " << nomeCliente << "\n";
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
        std::cout << "         OBRIGADO E VOLTE SEMPRE!                   \n";
        std::cout << "****************************************************\n\n";
    }
};

class HamburguerBuilder {
private: Hamburguer lanche;
public:
    void iniciar(std::string n) { lanche=Hamburguer(); lanche.nomeCliente=n; }
    void setPao(std::string t) { lanche.pao=t; }
    void setCarne(std::string t) { lanche.carne=t; }
    void addExtra(std::string t) { lanche.extras.push_back(t); }
    Hamburguer get() { return lanche; }
    void mostrar() {
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| STATUS DA MONTAGEM                               |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| Pao:   " << std::left << std::setw(35) << (lanche.pao.empty()?"[Escolhendo...]":lanche.pao) << " |\n";
        std::cout << "| Carne: " << std::left << std::setw(35) << (lanche.carne.empty()?"[Pendente]":lanche.carne) << " |\n";
        std::cout << "| Extras: " << std::left << std::setw(33) << (lanche.extras.empty()?"[Nenhum]":std::to_string(lanche.extras.size())+" itens") << " |\n";
        std::cout << "+--------------------------------------------------+\n\n";
    }
};

class HistoricoVendas {
private: std::string arq;
public:
    HistoricoVendas(std::string u) { arq="vendas_"+u+".txt"; }
    void registrar(const Hamburguer& h) { std::ofstream f(arq, std::ios::app); if(f.is_open()){f<<h.log()<<"\n"; f.close();} }
    void relatorio() {
        cabecalho("RELATORIO: "+arq); std::ifstream f(arq); std::string l;
        if(!f.is_open()) std::cout<<"(Sem vendas)\n";
        else { int i=1; while(std::getline(f,l)) { std::cout<<" #"<<i++<<" | "<<l<<"\n"; linha(); } f.close(); }
        std::cout<<"\n[Enter] voltar..."; std::cin.ignore(); std::cin.get();
    }
};

// ==========================================
// A FACHADA (FACADE) - AQUI FICA A COMPLEXIDADE
// ==========================================
class HamburgueriaFacade {
private:
    SistemaAuth auth;
    Estoque estoque;
    HamburguerBuilder builder;

    // Helper interno para menus bonitos
    std::string menuVisual(std::string titulo, std::vector<std::string> lista, bool voltar) {
        while(true) {
            std::vector<std::string> disp;
            for(auto& i:lista) if(estoque.ver(i)>0) disp.push_back(i);
            
            if(disp.empty()) { std::cout<<" (Esgotado!)\n"; return "NADA"; }
            
            std::cout << "=== " << titulo << " ===\n";
            for(size_t i=0; i<disp.size(); i++) {
                std::cout << " [" << (i+1) << "] " << std::left << std::setw(25) << disp[i] << "\n";
            }
            linha();
            if(voltar) std::cout << " [0] PRONTO / FINALIZAR\n";
            
            std::cout << "\n>> Opcao: "; int op; std::cin >> op;
            
            if(voltar && op==0) return "VOLTAR";
            if(op>0 && op<=(int)disp.size()) {
                if(estoque.consumir(disp[op-1])) return disp[op-1];
                else std::cout<<"\n[!] Erro de estoque!\n";
            } else std::cout<<"\n[!] Invalido.\n";
            std::cout<<"\n";
        }
    }

    void realizarPedido(HistoricoVendas& hist) {
        cabecalho("NOVO PEDIDO");
        std::string cli; std::cout << "Nome do Cliente: "; std::cin >> cli;
        builder.iniciar(cli);

        std::vector<std::string> paes = {"Brioche", "Australiano", "Gergelim", "Parmesao"};
        std::vector<std::string> carnes = {"Angus", "Frango", "Vegano", "Picanha"};
        std::vector<std::string> extras = {"Cheddar", "Bacon", "Salada", "Maionese_Verde", "Picles", "Barbecue", "Cebola_Caramelizada"};

        limparTela(); builder.mostrar();
        std::string p = menuVisual("ESCOLHA O PAO", paes, false);
        if(p!="NADA") builder.setPao(p);

        limparTela(); builder.mostrar();
        std::string c = menuVisual("ESCOLHA A CARNE", carnes, false);
        if(c!="NADA") builder.setCarne(c);

        while(true) {
            limparTela(); builder.mostrar();
            std::string e = menuVisual("ADICIONAIS", extras, true);
            if(e=="VOLTAR" || e=="NADA") break;
            builder.addExtra(e);
        }

        Hamburguer h = builder.get();
        h.nota();
        hist.registrar(h);
        std::cout << "[Enter] para proximo..."; std::cin.ignore(); std::cin.get();
    }

public:
    void iniciarSistema() {
        while(true) {
            limparTela();
            imprimirLogo(); // <--- LOGO AQUI
            std::cout << "+--------------------------------------------------+\n";
            std::cout << "| MENU INICIAL (Facade)                            |\n";
            std::cout << "+--------------------------------------------------+\n";
            std::cout << "| 1. Entrar (Login)                                |\n";
            std::cout << "| 2. Cadastrar Funcionario                         |\n";
            std::cout << "| 3. Fechar Loja                                   |\n";
            std::cout << "+--------------------------------------------------+\n";
            std::cout << "\n>> Opcao: "; int op; std::cin >> op;

            if(op==3) { limparTela(); std::cout<<"Encerrando...\n"; break; }
            if(op==2) {
                cabecalho("NOVO CADASTRO");
                std::string u,p; 
                std::cout<<"Usuario: "; std::cin>>u; std::cout<<"Senha: "; std::cin>>p; linha();
                if(auth.cadastrar(u,p)) std::cout<<"[OK] Sucesso!\n"; else std::cout<<"[ERRO] Ja existe!\n";
                std::cin.ignore(); std::cin.get();
            }
            if(op==1) {
                cabecalho("LOGIN");
                std::string u,p; 
                std::cout<<"Usuario: "; std::cin>>u; std::cout<<"Senha: "; std::cin>>p;
                if(auth.logar(u,p)) {
                    HistoricoVendas hist(u);
                    while(true) {
                        cabecalho("AREA DO ATENDENTE: "+u);
                        std::cout<<" 1. Novo Pedido\n 2. Relatorio\n 3. Deslogar\n"; linha();
                        std::cout<<"\n>> Opcao: "; int x; std::cin>>x;
                        if(x==3) break;
                        if(x==2) hist.relatorio();
                        if(x==1) realizarPedido(hist);
                    }
                } else {
                    std::cout<<"\n[ERRO] Login invalido.\n"; std::cin.ignore(); std::cin.get();
                }
            }
        }
    }
};

// ==========================================
// MAIN (CLIENTE)
// ==========================================
int main() {
    // Cliente continua simples, mas agora o sistema é bonito!
    HamburgueriaFacade sistema;
    sistema.iniciarSistema();
    return 0;
}