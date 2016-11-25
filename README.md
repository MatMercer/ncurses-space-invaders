# Sobre o projeto

Space Invaders é um jogo proposto como forma de trabalho pelo professor
[Augusto Luengo Pereira Nunes](http://www.inf.ufrgs.br/~alpnunes/index.html)
no Instituto Federal do Paraná campus Londrina. O jogo é inspirado no [Space
Invaders Original](https://en.wikipedia.org/wiki/Space_Invaders) contendo os
mesmos elementos tirando as barreiras.


# Elementos Principais

Bordas / Cenário O caractere padrão da borda é  o ‘*’. As bordas são
desenhadas centralizadas, com base no tamanho da janela (terminal) ao executar
o jogo. Caso a janela seja menor do que o tamanho mínimo definido (90
caracteres de largura e 45 caracteres de altura), as bordas serão desenhadas
de acordo com o espaço mínimo.


A borda padrão é da cor amarela, caso o player perca uma vida a borda torna-se
vermelha e o caractere ‘*’ é substituído por ‘X’. Em caso de vitória do
jogador, a borda torna-se verde e o caractere ‘*’ é substituído por ‘+’. Todos
os caracteres do jogo são impressos na tela em negrito e cada componente
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
coluna pode atirar. Caso a coluna gerada esteja vazia, o laser será
redirecionado para o último alien da última coluna (mais próxima do player).


## Nave do Jogador

A posição inicial do player é centralizada, logo acima da borda inferior. A
nave do jogador é movimentada pelas setas do teclado, que controlam a posição
horizontal do player.

O laser do player é ativado com a tecla espaço e só poderá ser atirado se não
houver nenhum outro disparo do player em andamento. O disparo é finalizado ao
acertar um alien ou ao atingir a borda superior. Elementos Secundários Sons

## Sons

Existem diversos efeitos sonoros no jogo, os quais melhoram consideravelmente
a experiência do usuário e até mesmo a jogabilidade. A tecla **‘M’** é
utilizada para ativar ou desativar os sons do jogo.

Os sons foram gerados por Gabriel Ranea a partir do site
[SuperFlashBros](http://www.superflashbros.net/as3sfxr/).


## Score

O score é incrementado em 10 pontos a cada alien eliminado. O jogador pode
conferir sua pontuação no canto superior direito do jogo e na tela final de
“Game Over”.


## Colisões

As colisões são detectadas a partir das posições X e Y dos itens (aliens vs
lasers) (player vs lasers).


## Game Over

O jogo pode ser encerrado de duas maneiras. Em ambos os casos, uma mensagem
animada de “Game Over” é mostrada na tela, informando a pontuação do player e
se venceu ou não o jogo.

### Vitória

Caso o player tenha eliminado todos os aliens. A verificação é feita com base
no score do player em comparação com a quantidade de aliens multiplicada pela
pontuação de cada alien abatido. Ex: 25 aliens x 10 pontos = 250 pontos para
vencer o jogo.


### Derrota

O jogo é encerrado se o player não possuir nenhuma vida e sofrer dano ou se um
alien tocar a borda inferior ou se um alien tocar o player. Quando isto acontece
o jogador é notificado que ele perdeu.
