#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <map>
#include <iomanip> // Para alinhar textos (setw)

// ==========================================
// UTILITÁRIOS VISUAIS
// ==========================================
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// A NOVA LOGO!
void imprimirLogo() {
    std::cout << "\n";
    std::cout << "  _    _                  _                                     _            _           _ \n";
    std::cout << " | |  | |                | |                                   (_)          | |         | |\n";
    std::cout << " | |__| | __ _ _ __ ___  | |__  _   _ _ __ __ _ _   _  ___ _ __ _  __ _   __| | ___     | |\n";
    std::cout << " |  __  |/ _` | '_ ` _ \\ | '_ \\| | | | '__/ _` | | | |/ _ \\ '__| |/ _` | / _` |/ _ \\    | |\n";
    std::cout << " | |  | | (_| | | | | | || |_) | |_| | | | (_| | |_| |  __/ |  | | (_| || (_| | (_) |   |_|\n";
    std::cout << " |_|  |_|\\__,_|_| |_| |_||_.__/ \\__,_|_|  \\__, |\\__,_|\\___|_|  |_|\\__,_| \\__,_|\\___/    (_)\n";
    std::cout << "                                           __/ |                                           \n";
    std::cout << "                                          |___/                                            \n";
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
// 1. SISTEMA DE LOGIN E CADASTRO
// ==========================================
class SistemaAuth {
private:
    const std::string arquivoUsers = "usuarios.txt";
public:
    bool cadastrar(std::string user, std::string pass) {
        std::ifstream leitura(arquivoUsers);
        std::string u, p;
        while (leitura >> u >> p) if (u == user) return false;
        leitura.close();
        std::ofstream escrita(arquivoUsers, std::ios::app);
        escrita << user << " " << pass << "\n";
        escrita.close();
        return true;
    }
    bool logar(std::string user, std::string pass) {
        std::ifstream leitura(arquivoUsers);
        std::string u, p;
        if (!leitura.is_open()) return false;
        while (leitura >> u >> p) if (u == user && p == pass) return true;
        return false;
    }
};

// ==========================================
// 2. ESTOQUE
// ==========================================
class Estoque {
private:
    std::map<std::string, int> itens;
    const std::string arquivoDB = "estoque.txt";
public:
    Estoque() { carregarDados(); }
    void carregarDados() {
        std::ifstream arquivo(arquivoDB);
        if (!arquivo.is_open()) {
            itens["Brioche"] = 50; itens["Australiano"] = 50; itens["Gergelim"] = 50;
            itens["Parmesao"] = 30; itens["Batata"] = 30; itens["Integral"] = 20; itens["Pao_de_Alho"] = 25;
            itens["Angus"] = 50; itens["Frango"] = 40; itens["Vegano"] = 20;
            itens["Picanha"] = 30; itens["Costela"] = 25; itens["Peixe"] = 15; itens["Duplo_Smash"] = 30;
            itens["Cheddar"] = 100; itens["Bacon"] = 100; itens["Salada"] = 100;
            itens["Picles"] = 50; itens["Cebola_Roxa"] = 50; itens["Cebola_Caramelizada"] = 40;
            itens["Ovo"] = 50; itens["Calabresa"] = 40; itens["Gorgonzola"] = 30;
            itens["Catupiry"] = 40; itens["Maionese_Verde"] = 100; itens["Barbecue"] = 100; itens["Aneis_de_Cebola"] = 30;
            salvarDados(); return;
        }
        std::string nome; int qtd;
        while (arquivo >> nome >> qtd) itens[nome] = qtd;
        arquivo.close();
    }
    void salvarDados() {
        std::ofstream arquivo(arquivoDB);
        for (auto const& [nome, qtd] : itens) arquivo << nome << " " << qtd << "\n";
        arquivo.close();
    }
    bool consumirItem(std::string nome) {
        if (itens.find(nome) != itens.end() && itens[nome] > 0) {
            itens[nome]--; salvarDados(); return true;
        } return false;
    }
    int verQtd(std::string nome) {
        if (itens.find(nome) != itens.end()) return itens[nome]; return 0;
    }
};

// ==========================================
// 3. PRODUTO E BUILDER 
// ==========================================
class Hamburguer {
public:
    std::string nomeCliente;
    std::string pao;
    std::string carne;
    std::vector<std::string> extras;

    std::string gerarLinhaLog() const {
        std::string resumo = "CLIENTE: " + nomeCliente + " | " + pao + ", " + carne;
        if (!extras.empty()) {
            resumo += " + (";
            for (const auto& item : extras) resumo += item + " ";
            resumo += ")";
        } return resumo;
    }

    void imprimirNota() const {
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
            for (const auto& item : extras) std::cout << "  [+] " << item << "\n";
        }
        std::cout << "\n";
        std::cout << "****************************************************\n";
        std::cout << "         OBRIGADO E VOLTE SEMPRE!                   \n";
        std::cout << "****************************************************\n\n";
    }
};

class HamburguerBuilder {
private:
    Hamburguer lanche;
public:
    void iniciar(std::string nome) { lanche = Hamburguer(); lanche.nomeCliente = nome; }
    void setPao(std::string t) { lanche.pao = t; }
    void setCarne(std::string t) { lanche.carne = t; }
    void addExtra(std::string t) { lanche.extras.push_back(t); }
    Hamburguer getResultado() { return lanche; }
    
    void mostrar() { 
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| STATUS DA MONTAGEM                               |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| Pao:   " << std::left << std::setw(35) << (lanche.pao.empty()?"[Escolhendo...]":lanche.pao) << " |\n";
        std::cout << "| Carne: " << std::left << std::setw(35) << (lanche.carne.empty()?"[Pendente]":lanche.carne) << " |\n";
        if(!lanche.extras.empty()) {
             std::cout << "| Extras: " << lanche.extras.size() << " itens selecionados                       |\n";
        } else {
             std::cout << "| Extras: [Nenhum]                                 |\n";
        }
        std::cout << "+--------------------------------------------------+\n\n";
    }
};

// ==========================================
// 4. HISTÓRICO INDIVIDUAL
// ==========================================
class HistoricoVendas {
private: std::string arquivoPessoal;
public:
    HistoricoVendas(std::string nomeAtendente) { arquivoPessoal = "vendas_" + nomeAtendente + ".txt"; }
    void registrarVenda(const Hamburguer& lanche) {
        std::ofstream arquivo(arquivoPessoal, std::ios::app);
        if (arquivo.is_open()) { arquivo << lanche.gerarLinhaLog() << "\n"; arquivo.close(); }
    }
    void exibirRelatorio() {
        cabecalho("RELATORIO DE VENDAS: " + arquivoPessoal);
        std::ifstream arquivo(arquivoPessoal);
        std::string linhaTxt;
        if (!arquivo.is_open()) { std::cout << "(Nenhuma venda registrada neste turno)\n"; } 
        else {
            int i = 1;
            while (std::getline(arquivo, linhaTxt)) {
                std::cout << " #" << i++ << " | " << linhaTxt << "\n";
                linha();
            }
            arquivo.close();
        }
        std::cout << "\n[Enter] para voltar..."; std::cin.ignore(); std::cin.get();
    }
};

// ==========================================
// 5. HELPER: MENU DINAMICO 
// ==========================================
std::string exibirMenu(std::string titulo, std::vector<std::string> lista, Estoque& est, bool voltar) {
    while (true) {
        std::vector<std::string> disp;
        for (auto& i : lista) if (est.verQtd(i) > 0) disp.push_back(i);

        if (disp.empty()) { std::cout << " (Categoria Esgotada!)\n"; return "NADA"; }

        std::cout << "=== " << titulo << " ===\n";
        for (size_t i = 0; i < disp.size(); i++) {
            // Formatação: alinha os números e os nomes
            std::cout << " [" << (i+1) << "] " << std::left << std::setw(25) << disp[i] << "\n";
        }
        linha();
        if (voltar) std::cout << " [0] PRONTO / FINALIZAR\n";

        std::cout << "\n>> Opcao: ";
        int op; std::cin >> op;

        if (voltar && op == 0) return "VOLTAR";
        if (op > 0 && op <= (int)disp.size()) {
            if (est.consumirItem(disp[op-1])) return disp[op-1];
            else std::cout << "\n[!] Erro de estoque! Tente novamente.\n";
        } else std::cout << "\n[!] Opcao invalida.\n";
        std::cout << "\n";
    }
}

// ==========================================
// 6. MAIN
// ==========================================
int main() {
    SistemaAuth auth;
    Estoque estoque;
    HamburguerBuilder builder;
    
    std::vector<std::string> lPaes = {"Brioche", "Australiano", "Gergelim", "Pao_de_Alho", "Parmesao", "Batata", "Integral"};
    std::vector<std::string> lCarnes = {"Angus", "Frango", "Vegano", "Picanha", "Costela", "Peixe", "Duplo_Smash"};
    std::vector<std::string> lExtras = {"Cheddar", "Catupiry", "Gorgonzola", "Bacon", "Calabresa", "Ovo", "Salada", "Picles", "Cebola_Roxa", "Cebola_Caramelizada", "Aneis_de_Cebola", "Maionese_Verde", "Barbecue"};

    while(true) {
        limparTela();
        imprimirLogo(); // <--- LOGO NOVA AQUI!
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| MENU INICIAL                                     |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "| 1. Entrar (Login)                                |\n";
        std::cout << "| 2. Cadastrar Funcionario                         |\n";
        std::cout << "| 3. Fechar Loja                                   |\n";
        std::cout << "+--------------------------------------------------+\n";
        std::cout << "\n>> Opcao: ";
        int opLogin; std::cin >> opLogin;

        if (opLogin == 3) {
            limparTela();
            std::cout << "Fechando o sistema... Ate amanha!\n";
            break;
        }

        if (opLogin == 2) {
            cabecalho("NOVO CADASTRO");
            std::string u, p;
            std::cout << "Novo Usuario: "; std::cin >> u;
            std::cout << "Nova Senha:   "; std::cin >> p;
            linha();
            if (auth.cadastrar(u, p)) std::cout << "[OK] Cadastrado com sucesso!\n";
            else std::cout << "[ERRO] Usuario ja existe!\n";
            std::cin.ignore(); std::cin.get(); continue;
        }

        if (opLogin == 1) {
            cabecalho("LOGIN NO SISTEMA");
            std::string usuarioLogado, senha;
            std::cout << "Usuario: "; std::cin >> usuarioLogado;
            std::cout << "Senha:   "; std::cin >> senha;

            if (auth.logar(usuarioLogado, senha)) {
                HistoricoVendas historicoPessoal(usuarioLogado);
                while(true) {
                    cabecalho("AREA DO ATENDENTE: " + usuarioLogado);
                    std::cout << " 1. Novo Pedido\n";
                    std::cout << " 2. Relatorio de Vendas\n";
                    std::cout << " 3. Deslogar\n";
                    linha();
                    std::cout << "\n>> Opcao: ";
                    int opSys; std::cin >> opSys;

                    if (opSys == 3) break;
                    if (opSys == 2) { historicoPessoal.exibirRelatorio(); continue; }

                    if (opSys == 1) {
                        cabecalho("INICIANDO NOVO PEDIDO");
                        std::string cli;
                        std::cout << "Nome do Cliente: "; std::cin >> cli;
                        builder.iniciar(cli);

                        limparTela(); builder.mostrar();
                        std::string p = exibirMenu("ESCOLHA O PAO OBRIGATORIO", lPaes, estoque, false);
                        if(p!="NADA") builder.setPao(p);

                        limparTela(); builder.mostrar();
                        std::string c = exibirMenu("ESCOLHA A CARNE OBRIGATORIA", lCarnes, estoque, false);
                        if(c!="NADA") builder.setCarne(c);

                        bool add = true;
                        while(add) {
                            limparTela(); builder.mostrar();
                            std::string e = exibirMenu("TURBINAR O LANCHE (OPCIONAL)", lExtras, estoque, true);
                            if(e=="VOLTAR" || e=="NADA") add = false;
                            else builder.addExtra(e);
                        }

                        Hamburguer final = builder.getResultado();
                        final.imprimirNota();
                        historicoPessoal.registrarVenda(final);
                        
                        std::cout << "[Enter] para proximo cliente...";
                        std::cin.ignore(); std::cin.get();
                    }
                }
            } else {
                std::cout << "\n[ERRO] Senha ou usuario incorretos!\n";
                std::cin.ignore(); std::cin.get();
            }
        }
    }
    return 0;
}