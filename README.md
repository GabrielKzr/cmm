<!--
Author: Gabriel Kaizer de Menezes & Nathan da Rosa Cidal
-->

# Analisador Léxico e Sintático — C--

Este projeto implementa um analisador léxico e sintático para uma linguagem de exemplo denominada **C--**.

O analisador é dividido em duas etapas:

* **Analisador léxico:** implementado utilizando [Flex](https://github.com/westes/flex).
* **Analisador sintático:** implementado manualmente utilizando um **Analisador Sintático Descendente Recursivo (ASDR)** em C.

O ponto de entrada do analisador sintático é a função:

```c
prog()
```

## Estrutura do projeto

```text
.
├── lexer/
│   └── lexer.flex
├── syntactic/
│   ├── asdr_sample.c
│   └── asdr_sample.h
├── tests/
│   └── main.cmm
├── main.c
├── Makefile
└── README.md
```

## Gramática

A gramática utilizada pelo analisador sintático é:

```text
Prog --> ListaDecl

ListaDecl --> Tipo IDENT RestoDecl
           | VOID IDENT RestoFunc

RestoDecl --> ';' ListaDecl
           | ',' ListaIdent ';'
           | RestoFunc

RestoFunc --> '(' FormalPar ')' '{' DeclVar ListaCmd '}' ListaDecl

DeclVar --> Tipo ListaIdent ';' DeclVar
          | ε

Tipo --> int
       | double
       | boolean

ListaIdent --> IDENT ',' ListaIdent
            | IDENT

FormalPar --> ParamList
            | ε

ParamList --> Tipo IDENT ',' ParamList
           | Tipo IDENT

Bloco --> '{' ListaCmd '}'

ListaCmd --> Cmd ListaCmd
           | ε

Cmd --> Bloco
     | while '(' E ')' Cmd
     | IDENT '=' E ';'
     | if '(' E ')' Cmd RestoIf

RestoIf --> else Cmd
         | ε

E --> E '+' T
    | E '-' T
    | T

T --> T '*' F
    | T '/' F
    | F

F --> IDENT
    | NUM
    | '(' E ')'
```

> `ε` representa uma produção vazia.

## Analisador léxico

O analisador léxico está localizado em:

```text
lexer/lexer.flex
```

Ele é processado pelo **Flex** para gerar o código C correspondente:

```text
build/lex.yy.c
```

Entre os tokens reconhecidos pelo léxico estão:

| Token         | Exemplos                           |
| ------------- | ---------------------------------- |
| `TYPE`        | `int`, `double`, `boolean`, `bool` |
| `VOID`        | `void`                             |
| `FUNC`        | `func`                             |
| `WHILE`       | `while`                            |
| `IF`          | `if`                               |
| `ELSE`        | `else`                             |
| `IDENT`       | `x`, `contador`, `resultado`       |
| `NUM`         | `10`, `3.14`, `true`, `false`      |
| operadores    | `+`, `-`, `*`, `/`, `=`            |
| delimitadores | `(`, `)`, `{`, `}`, `,`, `;`       |

No léxico atual, `true` e `false` são retornados como `NUM`:

```c
"true"  { return NUM; }
"false" { return NUM; }
```

Portanto, para o analisador sintático, ambos possuem o mesmo token que valores numéricos.

Operadores relacionais como:

```text
==
!=
<=
>=
<
>
```

são reconhecidos pelo léxico, mas produzem uma mensagem de erro léxico, pois não fazem parte da linguagem definida pela gramática.

A palavra reservada `return` também não é permitida.

## Analisador sintático

O analisador sintático está implementado como um **ASDR (Analisador Sintático Descendente Recursivo)** em:

```text
syntactic/asdr_sample.c
```

Cada produção da gramática é implementada por uma função correspondente.

O ponto de entrada da análise sintática é:

```c
prog()
```

A partir de `prog()`, o analisador percorre as produções da gramática de forma recursiva, consumindo os tokens fornecidos pelo analisador léxico.

## Requisitos

Para compilar o projeto são necessários:

* GCC
* Flex
* Make

Em sistemas baseados em Debian/Ubuntu, por exemplo:

```bash
sudo apt install gcc flex make
```

## Build

Para compilar o projeto, execute:

```bash
make
```

O `Makefile` executará as seguintes etapas:

1. Geração do analisador léxico com Flex.
2. Compilação do código gerado pelo Flex.
3. Compilação do ASDR e do programa principal.
4. Geração do executável.

O executável será criado em:

```text
build/lexer
```

## Executando um teste

O arquivo de teste padrão é:

```text
tests/main.cmm
```

Para executar o teste:

```bash
make test
```

O comando equivale a:

```bash
./build/lexer < tests/main.cmm
```

Também é possível executar o analisador diretamente sobre outro arquivo:

```bash
./build/lexer < arquivo.cmm
```

## Limpando os arquivos gerados

Para remover os arquivos gerados durante a compilação:

```bash
make clean
```

Isso remove o diretório:

```text
build/
```

## Exemplo

Um programa válido para a linguagem C--:

```c
int x, y;
double resultado;
boolean flag;

int calcula(int n, double fator, boolean ativo) {
    int i, temp;
    double valor;

    i = n;
    temp = 10;
    valor = fator * 2 + temp;

    if (true) {
        temp = temp + 1;
        valor = valor * 2;
    } else {
        temp = temp - 1;
    }

    while (ativo) {
        if (false) {
            temp = temp * 2;
        } else {
            temp = temp + i;
        }

        i = i - 1;
    }
}

void processa(int limite, double valor) {
    int contador;
    double parcial, total;

    contador = limite;
    parcial = valor * 2;
    total = parcial / (contador + 1);

    while (contador) {
        if (contador - 1) {
            total = total + parcial * contador;
            parcial = parcial + 1;
        } else {
            total = total / 2;
        }

        contador = contador - 1;
    }
}

void simples() {
    int a, b, c;

    a = 1;
    b = 2;
    c = (a + b) * (a - b) / 2;
}
```

Esse exemplo exercita diversas construções da linguagem, incluindo:

* declarações globais;
* declarações locais;
* `int`, `double` e `boolean`;
* funções com e sem parâmetros;
* parâmetros de diferentes tipos;
* funções `void` e funções com tipo de retorno;
* atribuições;
* `if` e `else`;
* `while`;
* comandos aninhados;
* expressões aritméticas;
* precedência de operadores;
* utilização de parênteses;
* identificadores e números;
* `true` e `false`.

## Comandos disponíveis

| Comando      | Descrição                        |
| ------------ | -------------------------------- |
| `make`       | Compila o projeto                |
| `make test`  | Compila e executa o teste padrão |
| `make clean` | Remove os arquivos gerados       |

## Autores

**Gabriel Kaizer de Menezes**
**Nathan da Rosa Cidal**
