# Sobre o projeto

Space Invaders é um jogo proposto como forma de trabalho pelo professor
[Augusto Luengo Pereira Nunes](http://www.inf.ufrgs.br/~alpnunes/index.html)
no Instituto Federal do Paraná campus Londrina. O jogo é inspirado no [Space
Invaders Original](https://en.wikipedia.org/wiki/Space_Invaders) contendo os
mesmos elementos tirando as barreiras.

# Estrutura do código

## Funções de inicialização

Funções que começam com o nome "init" são usadas para inicializar os
componentes do jogo, por exemplo a função initAliens() aloca a matriz de
aliens e os coloca na posição inicial correta. A função de inicialização
principal é a initGame(), ao qual inicia o ncurses.

## Loop principal do jogo

Todas as funções são chamadas dentro de um loop um loop principal na função
startSpaceInvaders() e são divididas em duas categorias:

* Funções de lógica do jogo. Funções de renderização.

A função render() renderiza todos os componentes do jogo. As funções de lógica
são chamadas depois do render() dentro da função startSpaceInvaders(), isso
causa independência entre a renderização e física do jogo. O jogo funciona
mesmo tendo um ou todos os componentes não renderizados.

# Elementos Principais

## Bordas / Cenário

O caractere padrão da borda é o '\*'. As bordas são desenhadas centralizadas,
com base no tamanho da janela (terminal) ao executar o jogo. Caso a janela
seja menor do que o tamanho mínimo definido (90 caracteres de largura e 45
caracteres de altura), as bordas serão desenhadas de acordo com o espaço
mínimo.

A borda padrão é da cor amarela, caso o player perca uma vida a borda torna-se
vermelha e o caractere ‘\*’ é substituído por ‘X’. Em caso de vitória do
jogador, a borda torna-se verde e o caractere ‘\*’ é substituído por ‘+’.
Todos os caracteres do jogo são impressos na tela em negrito e cada componente
possui uma cor diferente.


## Aliens

Os aliens movem-se de forma conjunta e uniforme, automaticamente, após um
certo intervalo de tempo. Esse intervalo é baseado em uma variável, a qual
incrementa a cada loop do jogo. Os movimentos começam para a direita e mudam a
direção horizontal após cada colisão com as bordas laterais. Além disso, os
aliens descem uma posição no sentido vertical após as colisões laterais.

Cada alien tem uma variável do tipo boolean que informa seu estado de vida. Os
aliens possuem três lasers, os quais são gerados randomicamente com o mesmo
intervalo de tempo do movimento dos aliens. Apenas o último alien de cada
coluna pode atirar.


## Nave do Jogador

A posição inicial do player é centralizada, logo acima da borda inferior. A
nave do jogador é movimentada pelas setas do teclado, que controlam a posição
horizontal do player.

O laser do player é ativado com a tecla espaço e só poderá ser atirado se não
houver nenhum outro disparo do player em andamento. O disparo é finalizado ao
acertar um alien ou ao atingir a borda superior. Elementos Secundários Sons

## Sons

Existem diversos efeitos sonoros no jogo, os quais melhoram consideravelmente
a experiência do usuário e até mesmo a jogabilidade. A tecla ‘M’ é utilizada
para ativar ou desativar os sons do jogo. Os sons são tocados com o comando
play a partir da função playSound(). Por padrão o formato deles é em .wav e o
comando chamado para tocar tem o seguinte formato:

* play -q {caminho do executável}/sfx/{nome do arquivo}{extensão} &

A função getWorkingDirectory() é crucial para esse comando funcionar pois ela
encontra o caminho do executavel. A parte final desse comando '&' faz com que
ele seja executado em *background*, evitando que o jogo pause quando um som é
tocado. O comando play esta disponivel em diferentes distribuições linux pelo
pacote sox. Então o comando para fazer a instalação deste pacote é simples:

* sudo apt-get install sox

Os sons foram gerados pelo Gabriel Ranea a partir do site
[SuperFlashBros](http://www.superflashbros.net/as3sfxr/).

## Score

O score é incrementado em 10 pontos a cada alien eliminado. O jogador pode
conferir sua pontuação no canto superior direito do jogo e na tela final de
“Game Over”.

## Colisões

As colisões são detectadas a partir das posições X e Y dos itens (aliens vs
lasers) (player vs lasers). Por exemplo, todos os aliens fazem a verificação
se um laser do jogador encostou nele, caso isso ocorra, ele não é mais
renderizado e não se move mais.

## Vidas

O jogador começa com 3 vidas, que são mostradas no canto superior esquerdo da
tela. Toda vez que o jogador sofre um dano de algum laser sua vida é
decrementada.

## Game Over

O jogo pode ser encerrado de duas maneiras, com o jogador ganhando ou
perdendo. Em ambos os casos, uma mensagem animada de “Game Over” é mostrada na
tela, informando a pontuação do player e se ele venceu ou não o jogo. O
jogador pode escolher sair do jogo ou reiniciar. A forma como de como o jogo
reinicia é bem simples pois é só uma chamada para a função
startSpaceInvaders().

### Vitória

O jogador só vence se todos os aliens forem eliminados. A verificação é feita
com base no score do player em comparação com a quantidade de aliens
multiplicada pela pontuação de cada alien abatido. Ex: 25 aliens x 10 pontos =
250 pontos para vencer o jogo.

### Derrota

O jogador é derrotado se:

* O player não tiver mais vidas. Algum alien tocar a borda inferior. Algum
* alien tocar o player.

# Conclusão

O desenvolvimento deste jogo foi muito importante para a nossa experiência
pois tivemos o contato com tecnologias que não tinhamos o conhecimento prévio
como Makefile, CMake, Clion, Doxygen. Além disso, devido ao fato de que este
programa foi feito para o SO Linux, obtivemos um conhecimento mais aprofundado
sobre como esse sistema funciona e sobre as ferramentas via shell.
