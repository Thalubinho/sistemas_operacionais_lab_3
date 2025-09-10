# sistemas_operacionais_lab_3
* Terceira atividade prática da disciplina de sistemas operacionais 2025.2

* O código utiliza também um semáforo do tipo Mutex e Futex.

* O Futex é exclusivo de sistemas Linux, então para rodar no Windows, é necessário utilizar o WSL (Windows Subsystem for Linux).

## Passos para rodar no Windows

## Instale o WSL (Windows Subsystem for Linux)

1. Abra o Prompt de Comando ou o PowerShell como administrador.

2. Digite o seguinte comando e pressione Enter:

```PS
wsl --install
```

3. Siga as instruções para reiniciar o computador e criar um usuário e senha para a sua nova distribuição Linux (o padrão é o Ubuntu).

## Acesse o ambiente Linux pelo WSL

4. Abra o terminal do WSL (pesquise por "Ubuntu" no menu Iniciar).

## Instale as dependências

5. Dentro do terminal do WSL, atualize a lista de pacotes:

```bash
sudo apt-get update
```
6. Instale o compilador GCC e a biblioteca pthread (que contém o Mutex):

```bash
sudo apt-get install build-essential
```
* O pacote build-essential já inclui as ferramentas necessárias, como o GCC, e as bibliotecas de sistema, como pthreads, que permitem usar Mutex e Futex. Você não precisa baixar o pthread separadamente.

## Compile e execute o código

7. Certifique-se de que o arquivo de código C (estacionamento.c) está no ambiente do WSL.

8. Compile o código usando o gcc e a flag -pthread:

```bash
gcc estacionamento.c -o estacionamento -pthread
```

9. Execute o programa:

```bash
./estacionamento
```