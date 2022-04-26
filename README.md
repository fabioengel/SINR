# Simulador SINR
O simulador SINR é uma implementação feita sobre o simulador OMNeT++ e o arcabouço INET Framework. O OMNeT++ (acrônimo para Objective Modular Network Testbed in C++) é um simulador de eventos discretos baseado em C++. Seu código é aberto e está liberado para uso sob a licença Academic Public License, que o torna livre para o uso sem fins lucrativos. O OMNeT++ não é propriamente uma plataforma para simulação de redes de computadores. Entretanto, os autores do projeto mantém um arcabouço de modelos para redes cabeadas e sem fio chamada INET Framework. O INET possui modelos para pilha de protocolos da Internet, além de diversos outros protocolos e componentes de rede. Ainda que possua boa parte dos componentes e protocolos de rede, não há um módulo específico para que o modelo SINR seja utilizado. O simulador SINR, portanto, realiza algumas modificações em módulos do INET Framework para implementação do modelo SINR. A seguir, são descritos os passos necessários para instalação e exemplos de utilização da ferramenta.

## Instalação
- OMNeT++ 6.0 (15/04/2022) - [Linux](https://github.com/omnetpp/omnetpp/releases/download/omnetpp-6.0/omnetpp-6.0-linux-x86_64.tgz) / [Windows](https://github.com/omnetpp/omnetpp/releases/download/omnetpp-6.0/omnetpp-6.0-windows-x86_64.zip)
    - O guia de instalação está disponível em omnetpp-6.0/doc/InstallGuide.pdf.
    - Guia de instalação para o Ubuntu 21.10:
        - Instalar pacotes pré-requisitados.
        ```{r, engine='bash', descompact}
            $ sudo apt-get install build-essential clang lld gdb bison flex perl python3 python3-pip qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libqt5opengl5-dev libxml2-dev zlib1g-dev doxygen graphviz libwebkit2gtk-4.0-37 libopenscenegraph-dev
            $ python3 -m pip install --user --upgrade numpy pandas matplotlib scipy seaborn posix_ipc
        ```        
        - Descompactar o arquivo omnetpp-6.0-linux-x86_64.
       ```{r, engine='bash', descompact}
            $ tar xvfz omnetpp-6.0-linux-x86_64.tgz
        ```
        - O OMNeT++ requer que certas váriaveis de ambiente sejam definidas e que o diretório omnetpp-6.0/bin esteja em PATH. Executar o script setenv para configurar estas variáveis.
        ```{r, engine='bash', descompact}
            $ cd omnetpp-6.0
            $ source setenv
        ```
        - O script "configure" detecta software instalado e configura o sistema.
        ```{r, engine='bash', descompact}
            $ ./configure
        ```
        - Compilar OMNeT++.
        ```{r, engine='bash', descompact}
            $ make
        ```
        - O OMNeT++ já está instalado. Para facilitar seu uso, adicione o diretório no qual foi feita instalação em PATH. Por exemplo, seja "/home/fabio/Software/omnetpp-6.0" o diretório, basta então:
        ```{r, engine='bash', descompact}
            $ sudo gedit ~/.bashrc
        ```
        - Copiar para o final do arquivo: "export PATH=$PATH:/home/fabio/Software/omnetpp-6.0/bin"
        - O OMNeT++ pode ser iniciado no terminal:
        ```{r, engine='bash', descompact}
            $ omnetpp
        ```

-  [INET-4.3.6](https://github.com/inet-framework/inet/releases/download/v4.3.6/inet-4.3.6-src.tgz) (para OMNeT++ 6.0 - 15/04/2022)
    - Descompactar o arquivo em sua pasta de projetos.
     ```{r, engine='bash', descompact}
            $ tar xvfz inet-4.3.6-src.tgz
      ```
    - Importar o Inet como projeto (File/Import/General - Existing Projects into Workspace) e depois o compilar.
- Simulador SINR
    - Copiar os 3 arquivos de [ModelSINRPathLoss](https://github.com/fabioengel/SINR/blob/master/arquivos/ModelSINRPathLoss.zip) para "inet4.3/src/inet/physicallayer/wireless/common/pathloss". Este novo módulo assume que a potência do sinal transmitido decai proporcionalmente ao inverso da distância euclidiana entre emissor e receptor, elevados a um expoente denominado de \textit{path loss} (perda de percurso).
    - Copiar os 3 arquivos de [ModelSinrError](https://github.com/fabioengel/SINR/blob/master/arquivos/ModelSinrError.zip) para o seu projeto.
este modelo considera que se a SINR do sinal recebido for maior ou igual ao limite SINR, não devem existir erros nos bits recebidos, podendo, portanto, receber corretamente o sinal. Caso contrário, erros são inseridos de acordo com a relação sinal ruído mais interferência e considera-se a recepção impossível de ser feita.
    - Copiar os arquivos de [imagens](https://github.com/fabioengel/SINR/blob/master/arquivos/imagem.zip) para "inet4.3/images/device/". (Estas imagens são utilizadas nos exemplos).

## Exemplos

A seguir são apresentados dois [exemplos](https://github.com/fabioengel/SINR/tree/master/exemplos). Ambos são projetos do simulador SINR e estão disponíveis para download. Estes projetos buscam mostrar que o simulador SINR é capaz de simular o modelo em questão. Os valores utilizados em cada exemplo podem ser modificados por meio de cada respectivo arquivo omnetpp.ini. Bem como novas adaptações podem ser realizadas a partir destes exemplos. Um [vídeo](https://www.youtube.com/watch?v=Ts0pLwKRHnU) está disponível mostrando a execução.

### Exemplo 01
No [exemplo 01](https://github.com/fabioengel/SINR/tree/master/exemplos/Exemplo01) é considerado um cenário com três links: (a,b), (c,d) e (f,e). Considera-se o momento em que os dispositivos  "a" , "c" e "f" realizam uma transmissão. 

<p align="center">
    <img src="https://github.com/fabioengel/SINR/blob/master/arquivos/SINR.png"
</p>
    
Ao redor de cada dispositivo transmissor existe uma circunferência que indica o seu alcance de transmissão de 100m. Este alcance é obtido a partir da potência de transmissão, que é uniforme para os três transmissores e igual a 0.1mW; do nível de sensibilidade da antena de recepção, considerado neste exemplo igual a -90dBm; do ruído de fundo igual a -90dB e do path loss igual a 4. Estes valores permitem que a SINR seja calculada para cada respectivo dispositivo receptor do exemplo ("b", "d" e "e"). Um [script](https://github.com/fabioengel/SINR/blob/master/scripts/computoSINR.py) em Python foi utilizado para este cômputo. Este exemplo permite ao usuário utilizar diferentes valores, como por exemplo, potência de transmissão e SINR limite, e assim verificar o impacto na recepção dos sinais. A simulação e o script podem ser utilizados para verificação do resultado.

### Exemplo 02
Para o [exemplo 02](https://github.com/fabioengel/SINR/tree/master/exemplos/Exemplo02) são considerados três dispositivos. Dois transmissores (Host 0 e Host 2) e um receptor (Host 1). Os transmissores utilizam potência uniforme, igual a 0.1mW. Assume-que o sinal é recebido corretamente se a SINR no receptor for maior ou igual a 1dB. Enquanto os dispositivos 0 e 1 permanecem estáticos, o dispositivo 2 movimenta-se em direção ao dispositivo 1. Toda vez que o dispositivo 2 aproxima-se 10m em relação ao dispositivo 1, ele e o dispositivo 0 realizam uma transmissão simultânea. A tabela abaixo, cujo resultado é obtido por um [script](https://github.com/fabioengel/SINR/blob/master/scripts/computoSINREx02.py), mostra a relação SINR para cada um dos sinais transmitidos.

| Distancia entre Hosts 2 e 1 | 135m | 125m | 115m | 105m | 95m | 85m | 75m | 65m | 55m | 45m | 35m |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| SINR do sinal de Host 0 | **7.02dB** | **5.69dB** | **4.25dB** | **2.67dB** | 0.93dB | -0.99dB | -3.16dB | -5.65dB | -8.55dB | -12.4dB | -16.4dB |
| SINR do sinal de Host 2 | -7.04dB | -5.70dB | -4.26dB | -2.68dB | -0.94 | 0.99dB | **3.16dB** | **5.65dB** | **-8.55dB** | **12.0dB** | **16.4dB** |

 É possível observar três situações distintas:
 - Distância entre 1 e 2 maior que 105m -- O nível do sinal de 0 é maior no receptor, e o sinal de 2, tratado como interferência, não é o suficiente para impedir o recebimento do sinal de 0.
 - Distância entre 1 e 2 entre 85m e 95m -- Nesta situação nenhum dos sinais obtém SINR maior ou igual a SINR limite (1dB). Neste caso, nenhum dos sinais contendo mensagens pode ser recebido corretamente.
 - Distância entre 1 e 2 menor ou igual a 75m -- A potência recebida do sinal de 2 é maior e a potência do sinal de 0, desta vez considerada interferência, não é o suficiente para impedir o recebimento do sinal de 2.
 
<p align="center">
    <img src="https://github.com/fabioengel/SINR/blob/master/arquivos/exemplo02.gif"
</p>

 Assim como no exemplo anterior, os valores utilizados podem ser alterados e a simulação estendida para outros fins.    
    
## Considerações
O simulador SINR proposto é uma ferramenta que auxilia o desenvolvimento de algoritmos e estratégias de comunicação para redes sem fio sob o modelo SINR.  Sobre o simulador SINR foram realizados testes que mostram que o mesmo é capaz de simular o modelo SINR. Alguns trabalhos utilizando esta ferramenta foram publicados e outros encontram-se em andamento. A modularidade, característica herdada do OMNeT++, permite que alterações sejam realizadas de maneira simples. Por exemplo, modificações futuras podem ser feitas para que além do modelo SINR geométrico, modelos estocásticos sejam utilizados. 

Trabalhos que utilizaram o simulador SINR:
- [Argumentos para a Inviabilidade Prática de uma Estratégia de Escalonamento para Redes Sem Fio sob o Modelo SINR](https://sol.sbc.org.br/index.php/wtf/article/view/17200).
- [A Down-to-Earth Scheduling Strategy for Dense SINR Wireless Networks](https://ieeexplore.ieee.org/document/9672559).
 
    
### Contato
Fábio Engel -- fabioe@utfp.edu.br

## Licença
Este material está licenciado sob uma licença creative commons Atribuição-NãoComercial-CompartilhaIgual
4.0 Internacional. De acordo com os seguintes termos:
- **Atribuição** - Você deve dar o crédito apropriado, prover um link para a licença e indicar se mudanças foram feitas. Você deve fazê-lo em qualquer circunstância razoável, mas de maneira alguma que sugira ao licenciante a apoiar você ou o seu uso.
- **Não Comercial** - Você não pode usar o material para fins comerciais.
- **Compartilhamento de modo idêntico** - Se você remixar, transformar, ou criar a partir do material, tem de distribuir as suas contribuições sob a mesma licença que o original.

Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

