# Projeto-EDL — Comparação de Estruturas de Dados

## Equipe

- João Vitor Carneiro
- Rodrigo Accioly

## Estrutura do projeto

- `include/`: arquivos `.h` com as definições dos TADs.
- `src/`: arquivos `.c` com as implementações e o programa principal (`main.c`).
- `resultados/`: arquivo `resultados.csv` gerado pelos experimentos.

## Implementações

Todos os módulos usam alocação dinâmica e armazenam apenas ponteiros para os registros carregados uma única vez na estrutura `Base`.

- **Lista unicamente encadeada (`lista.h` / `lista.c`)**
- **Hash com endereçamento aberto (`hash_aberta.h` / `hash_aberta.c`)**
- **Hash com encadeamento separado (`hash_encadeada.h` / `hash_encadeada.c`)**
- **Funções hash (`funcoes_hash.h` / `funcoes_hash.c`):** **Divisão** e **Polinomial:**

## Como compilar e executar

Antes de executar, certifique-se de que o arquivo da base de dados esteja salvo em `dados/estudantes.txt` na raiz do projeto.

### Opção 1: VS Code com MinGW (Windows)

1. Clone ou baixe o repositório e abra a pasta do projeto no VS Code (**File > Open Folder**).
2. Abra o terminal integrado (**Terminal > New Terminal** ou `Ctrl + '`).
3. Compile listando os arquivos da pasta `src/`:
   ```bash
   gcc -O2 -Iinclude src/main.c src/base.c src/lista.c src/funcoes_hash.c src/hash_aberta.c src/hash_encadeada.c -o projeto.exe
   ```
4. Execute o programa:
   ```bash
   .\projeto.exe
   ```

### Opção 2: GitHub Codespaces (no navegador)

1. Na página inicial deste repositório no GitHub, clique no botão verde **Code**.
2. Selecione a aba **Codespaces** e clique em **Create codespace on main**.
3. Aguarde o VS Code carregar no navegador e adicione o arquivo da base em `dados/estudantes.txt` (crie a pasta `dados` e arraste o arquivo pelo menu lateral).
4. No terminal integrado do Codespace, compile e execute:
   ```bash
   gcc -O2 -Iinclude src/*.c -o projeto
   ./projeto
   ```
