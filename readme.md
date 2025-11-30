# 🍔 Hamburgueria Design Patterns (C++) 

Este repositório contém uma aplicação prática de Padrões de Projeto (Design Patterns) desenvolvida em C++. O sistema simula o gerenciamento de uma Hamburgueria Artesanal, onde a mesma regra de negócio foi reescrita três vezes para demonstrar diferentes abordagens arquiteturais.

Estrutura do Projeto

O projeto está dividido em três implementações distintas, cada uma focada em uma categoria de padrões do GoF (Gang of Four):

    PadraoCriacional/ (Foco no objeto): builder.cpp
    PadraoEstrutural/ (Foco na organização): facade.cpp
    PadraoComportamental/ (Foco no fluxo): Chain.cpp


### 1. Builder (Padrão Criacional) 

Para que serve?

O padrão Builder é utilizado para separar a construção de um objeto complexo da sua representação. Ele permite criar diferentes representações do mesmo objeto passo a passo.


Qual problema resolve?

Resolve o problema do "Construtor Telescópico" (ou Constructor Hell). Imagine ter que instanciar um hambúrguer assim: new Hamburguer("Brioche", "Angus", true, false, true, "Maionese", false...); É confuso e propenso a erros. Se o lanche tiver 10 ingredientes opcionais, o construtor fica gigante.


Implementação no Código

No arquivo builder.cpp, o cliente pode montar o lanche etapa por etapa.

    Product: Classe Hamburguer.

    Builder: Classe HamburguerBuilder (com métodos .setPao(), .addExtra()).

    Resultado: O código fica fluente e legível.



### 2. Facade (Padrão Estrutural)

Para que serve?

O Facade (Fachada) fornece uma interface simplificada para um conjunto de interfaces em um subsistema. Ele define uma interface de nível mais alto que torna o subsistema mais fácil de usar.


Qual problema resolve?

O alto acoplamento e a complexidade. No sistema original, a função main precisava conhecer e gerenciar: SistemaAuth (Login), Estoque (Banco de dados), Builder (Montagem) e HistoricoVendas (Log). O código cliente estava "sujo" com muita lógica de infraestrutura.


Implementação no Código

No arquivo facade.cpp, criamos a classe HamburgueriaFacade.

    A main (Cliente) agora tem apenas 3 linhas.

    A main chama apenas sistema.iniciarSistema().

    A classe Facade esconde toda a complexidade de verificar senha, checar estoque e salvar arquivos.




### 3. Chain of Responsibility (Padrão Comportamental)

Para que serve?

O Chain of Responsibility evita o acoplamento do remetente de uma solicitação ao seu receptor, dando a mais de um objeto a oportunidade de tratar a solicitação. Encadeia-se os objetos receptores e passa-se a solicitação ao longo da cadeia.


Qual problema resolve?

Resolve a rigidez de fluxos condicionais (if/else gigantes). Em vez de um "Gerente" decidir tudo, criamos uma "Linha de Produção" (Pipeline). Se precisarmos adicionar uma etapa nova (ex: Verificação de Pagamento) entre o Estoque e a Cozinha, basta adicionar um elo na corrente sem quebrar o código existente.


Implementação no Código

No arquivo Chain.cpp, o pedido (PedidoRequest) viaja por uma esteira:

    EstoqueHandler: Verifica disponibilidade. Se faltar ingrediente, aborta a cadeia.

    CozinhaHandler: Se aprovado pelo estoque, monta o objeto.

    LogHandler: Se montado com sucesso, salva no arquivo.




### Como Rodar?

Certifique-se de ter um compilador C++ (G++) instalado.

Executando o Builder

    Bash

    cd PadraoCriacional
    g++ builder.cpp -o app_builder
    ./app_builder

Executando o Facade

    Bash

    cd PadraoEstrutural
    g++ facade.cpp -o app_facade
    ./app_facade

Executando o Chain of Responsibility

    Bash

    cd PadraoComportamental
    g++ Chain.cpp -o app_chain
    ./app_chain



Funcionalidades Extras

Todos os exemplos possuem:

    Persistência de Dados: Uso de arquivos .txt para simular banco de dados de Estoque e Usuários.
    Autenticação: Sistema de Login e Cadastro.
    Interface: Menus interativos via terminal com ASCII Art.



##  Referências de Estudo
A base teórica e os diagramas conceituais utilizados para este projeto foram consultados no site **Refactoring Guru**.

* [Chain of Responsibility - Refactoring Guru]([https://refactoring.guru/pt-br/design-patterns/chain-of-responsibility](https://refactoring.guru/pt-br/design-patterns/chain-of-responsibility))
* [Builder - Refactoring Guru]([https://refactoring.guru/pt-br/design-patterns/builder](https://refactoring.guru/pt-br/design-patterns/builder))
* [Facade - Refactoring Guru]([https://refactoring.guru/pt-br/design-patterns/facade](https://refactoring.guru/pt-br/design-patterns/facade))



### *Desenvolvido por **Leonardo Furlan** com auxílio do **Google Gemini**.*
### **Linguagem:** C++
### * **IDE:** VS Code
