
# 1	DESENVOLVIMENTO

   Este capítulo apresentará todo o desenvolvimento de um protótipo de medidor inteligente de energia elétrica residencial, do projeto a execução, tendo como base alguns conceitos apresentados nos capítulos anteriores.
    
## 1.1	   O PROTÓTIPO
    
   O protótipo de medidor inteligente de energia elétrica é um dispositivo portátil construído com objetivo de se obter em tempo real os valores de consumo elétrico através de leituras de corrente e tensão de um circuito monofásico residencial.
Para se obter os valores de corrente foi utilizado o medidor de corrente não invasivo SCT-013-030, que possui funcionamento similar a um alicate amperímetro e que descarta a necessidade de se abrir o circuito para realizar leituras. Basta somente que um dos fios, fase ou neutro, seja envolvido pelo sensor para que este entre em funcionamento, caso contrário o dispositivo pode não funcionar adequadamente.
Para os valores de tensão, elaboramos um circuito capaz de reduzir a tensão da rede a níveis seguros e aceitáveis pelo conversor analógico/digital do microcontrolador que garante uma relação da tensão de saída com a de entrada.

   Com os dados de corrente e tensão enviados ao microcontrolador é realizado o processamento e envio para plataforma ThingSpeak através do módulo de comunicação ESP8266-01. Ao receber os dados, a plataforma IoT irá representa-los em graficamente de forma a tornar as informações simples e fáceis para o consumidor. Todo o processo explicado anteriormente está representado na Figura 1 abaixo.
     
![1](https://user-images.githubusercontent.com/40185350/41261177-ee8048f4-6db0-11e8-92cc-0d597e2a7d34.png)

## 1.2	O HARDWARE

Para hardware foi utilizado os materiais listados no quadro 01 abaixo. Além disso, será apresentado a seguir todo o método utilizado para construção do protótipo.

![q1](https://user-images.githubusercontent.com/40185350/41261566-9f6e766c-6db2-11e8-878d-e1353b98f40c.png)

## 1.2.1	Condicionamento de sinal para o ATmega328P

Para o projeto foi o utilizado o microcontrolador Atmega328P da Atmel. Desta forma se fez necessário um estudo mais profundo sobre o conversor Analógico/Digital (AD) deste micro com o objetivo de obter os melhores resultados a partir do condicionamento dos sinais.

A Figura 2 demonstra a arquitetura interna de um Conversor Analógico-Digital (ADC). A partir dela pode-se observar que existem 8 canais de entradas analógicas multiplexadas na entrada do conversor, onde somente 4 serão utilizadas neste projeto. Entre outras característi- cas do conversor estão os 10 bits de resolução, 8 bits de precisão e taxa máxima de amostragem de 76,9KSPS( KiloAmostras por segundo). Para os bits de resolução é definido que a faixa de 5Vcc pode ser dividida em 1024 degraus discretos de medição, como mostrado na fórmula abaixo.

![f1](https://user-images.githubusercontent.com/40185350/41261822-e3ebc3de-6db3-11e8-8786-f32fa207b8d0.png)

Porém, se não forem calculadas médias a precisão do conversor estará em 8 bits. Isto siginifica que a unidade discreta de medição é de 5Vcc por 256 como pode ser visto na fórmula a seguir.

![f2](https://user-images.githubusercontent.com/40185350/41261885-31ee7658-6db4-11e8-87c2-f1e03a677665.png)

É importante ressaltar que segundo especificações do ADC, existe a opção que utilize referência interna de 1,1V, porém utilizaremos o valor de Vcc 5V. Além disso, é possível trabalhar com referências externas, porém não será abordado neste trabalho.(PUHLMANN, 2015)

![2](https://user-images.githubusercontent.com/40185350/41264277-3c2a4ff8-6dc2-11e8-983b-001faf495b2e.png)

## 1.2.2	O ESP8266

O módulo Esp8266 é um dispositivo System On Chip (SOC) que possui microcon- trolador de 32 bits com interface Wi-Fi e um chip de memória Flash.

Produzido pela empresa chinesa Espressif Systems, o módulo é amplamente utilizado em projetos de automação e IoT por possuir um sistema próprio de conexão à internet e ser uma solução barata e eficaz. Outra característica que torna o ESP8266 um aliado poderoso é a de proporcionar funcionalidade de acesso a rede Wi-Fi a vários tipos de microcontroladores através do protocolo TCP/IP integrado ao seu sistema. Além disso, este módulo possui a capacidade de hospedar aplicações assim como também descarregar funções de rede Wi-Fi a partir de outro processador de aplicação.

O ESP8266 está no mercado desde 2014 e atualmente conta com diversos modelos que, apesar de usarem o mesmo processador, se diferenciam em números de GPIO disponíveis, memória disponível e espaçamento entre os pinos.

Para este projeto, foi utilizado o modelo ESP8266-01, representado pela Figura 3. Este modelo possui como especificações os itens a baixo:

+	Possui 2 pinos GPIO ( General-purpose input/output);

+  Barramentos I2ˆC, SPI,UART;

+	CPU que opera em 80MHz, com possibilidade de operar em 160MHz;

+	Arquitetura RISC de 32 Bits;

+	Memória flash SPI de 512KBytes;

+	32Kbytes de RAM para instruções;

+	96KBytes de RAM para dados;

+	64KBytes de Rom para boot;

![3](https://user-images.githubusercontent.com/40185350/41264348-9af9aae2-6dc2-11e8-8253-5ca3def24fc3.png)

Os 8 pinos de conexão do ESP8266-01 são descritos no quadro 02 abaixo.

![q2](https://user-images.githubusercontent.com/40185350/41264648-3197a700-6dc4-11e8-83b1-373796fb652e.png)

É importante ressaltar que a tensão nominal de operação deste módulo é de 3.3V, podendo variar ± 0.3V, além disso, o consumo de corrente em operação pode chegar a 300 mA. Desta forma se faz necessário a construção de um circuito dedicado para alimentação do módulo.

Neste trabalho, utilizamos um adaptador como o mostrado na Figura 4, que nos permite utilizar alimentação de 5V, pois o mesmo se encarregará de regular a tensão 3.3V garantindo a operação adequada do módulo.

![4](https://user-images.githubusercontent.com/40185350/41264364-af222224-6dc2-11e8-91ca-025f2c132cf3.png)

Para a programação do ESP8266-01 utilizou-se a linguagem de comandos AT, desen- volvida na década de 80 pelo Americano Denis Hayes. Para cada configuração de parâmetros, consulta de status e execução de tarefas do módulo, existe um comando AT correspondente o que possibilita o uso em diversas aplicações. Segundo o fabricante, os comandos AT utilizados pelo ESP8266-01 podem ser divididos em três grupos: Comandos básicos, camada de WiFi e Camada TCP-IP. (MINATEL, 2018)

+	Os comandos básicos são os usados para verificar o funcionamento e outras funções do módulo.

+	Os comandos para WiFi são os responsáveis por listar as redes disponíveis, conectar e desconectar o módulo, entre outras.

+	Os comandos para camada TCP-IP, são utilizados por abrir e fechar conexões com o servidor, enviar dador, estabelecer múltiplas conexões, entre outras.

Os detalhes de cada comando AT podem ser verificados no datasheet disponível no site do fabricante.

## 1.2.3	Sensor de Corrente SCT-013-030

O sensor de corrente Split-core Currente Transformer 013-030 ( Transformador de corrente de núcleo dividido), produzido pela Beijing Yaohuadechang Eletronic Co. é uma boa opção para se medir correntes AC de forma não invasiva. Como qualquer transformador, o Transformador de Corrente (TC) possui um enrolamento primário e secundário. O enrolamento primário pode ser o fio condutor que se deseja medir a corrente, já o secundário é formado é formado pela bobina encontrada na caixa do sensor como mostrado na Figura 5.

![5](https://user-images.githubusercontent.com/40185350/41264391-ccf4bf1e-6dc2-11e8-927b-0509d207fff7.png)

Segundo datasheet, este sensor consegue medidas de corrente no primário de 0 a 30A, obtendo no secundário uma tensão que pode variar de 0 a 1V. Além disso, possui relação de espiras (N) de 1:1800, resistência de amostragem RL de 62Ω, faixa de operação não linear de 1% e trabalha com temperaturas que variam entre -25 C a + 70 C. (YHDC, 2015)

Com essas informações torna-se possível perceber que para cada 1A medido pelo primário será obtido uma tensão de 0,0333. . . volts no secundário.

Tendo-se conhecimento de que o Atmega328P possui faixa de operação de 0 à 5V e não realiza leituras negativas, foi elaborado um circuito de offset para elevar o sinal de saída acima de 0 volts. Conectado à 5Vcc, o circuito mostrado na Figura 6 é formado por um divisor de tensão e um capacitor que funcionará como uma bateria de 2,5 Volts. Desta forma o sinal enviado para o ADC do microcontrolador estará entre 1,5 Volts e 3,5 Volts como visto na Figura 11, simulada no Proteus.

Baseado no correto funcionamento no simulador, foi montado todo o circuito e comprovado o seu funcionamento. 

![6](https://user-images.githubusercontent.com/40185350/41264397-dbda1574-6dc2-11e8-994e-56ad399a4f2a.png)

## 1.2.4	Medindo de Tensão

A tensão que utilizamos em nossas residências sofre variações ao longo do dia, desta maneira para se obter uma maior precisão nos valores de potências e energia elétrica utilizada se faz necessário saber o valor real desta tensão a cada momento.

Sabendo que os valores de referência do conversor A/D do Atmega328P variam de 0 à 5V, foi projetado um circuito para condicionar o sinal, seguindo uma lógica similar à utilizada para o circuito de corrente, a fim de garantir a operacionalidade do sistema.
 
![7](https://user-images.githubusercontent.com/40185350/41264410-f19de5e8-6dc2-11e8-912a-1f97877baf95.png)

Antes de realizar a montagem física, todo o circuito foi simulado no software Proteus, obedecendo os passos a seguir:

Primeiramente, foi projetado um divisor que reduzisse a tensão de saída para va- lores dentro da faixa de aceitação do ADC. Utilizando o recurso de simulação, realizamos algumas combinações de resistores que nos levou a decidir por valores de R1=1MΩ e R2=4.7kΩ, mostrados na Figura 8.

![8](https://user-images.githubusercontent.com/40185350/41264418-fe421c56-6dc2-11e8-8191-4e0eb40dde0b.png)

A tensão de rede utilizada para esse projeto varia em torno de 220 volts rms, a qual chamaremos de Vin, porém, para calcular a tensão de saída nomeada de Vout, utilizaremos valores de pico a fim de garantir que os valores de saída não ultrapassem 5 V. Os cálculos utilizados são demonstrados na fórmula abaixo:

![f3](https://user-images.githubusercontent.com/40185350/41264690-726cbbf8-6dc4-11e8-93a3-bf69001abf64.png)

Como já comentado, o ADC não realiza leituras negativas do sinal. Portanto o segundo passo foi polarizar a saída de tensão através de um novo divisor com R3 e R4 iguais a 10KΩ ligados a 5Vcc, como mostra a Figura 9. O capacitor de 100uF também neste circuito funcionará como uma bateria de 2,5Vcc.

![9](https://user-images.githubusercontent.com/40185350/41264427-0b6706da-6dc3-11e8-80f0-985dbd0657d2.png)

Finalizada as simulações, montou-se o circuito e fez-se medições, conforme Figura 10, que comprovaram o funcionamento esperado.

![10](https://user-images.githubusercontent.com/40185350/41264444-208134e6-6dc3-11e8-8883-01d0fc349793.png)

 ## 1.2.5	RTC-Real Time Clock
 
 O Real Time Clock (RTC) utilizado no desenvolvimento do projeto é um dispositivo com uma função similar à de um relógio, sendo capaz de registrar valores de data e hora com alta precisão e baixo consumo de energia, além ainda de ajustar-se automaticamente em situações como meses com menos de 31 dias ou de anos bissextos.
 
Para o funcionamento adequado do módulo a tensão de operação deve estar 3.3V  e 5V. Caso o RTC perca a alimentação externa, um circuito de detecção de falhas acionará uma bateria acoplada que assumirá o funcionamento do módulo garantindo a configuração das informações estabelecidas inicialmente.(MADEIRA, 2017)

Para comunicação do RTC DS3231 com o microcontrolador ATmega328P é utilizado o protocolo I2C, onde a transferência dos dados ocorre através de um barramento bidirecional.

## 1.3	O SOFTWARE

Nesta etapa do trabalho é demonstrado a programação implementada para o projeto através de blocos de códigos e suas funções. Também será abordado, as ferramentas utilizadas para demonstração dos dados, simulação e elaboração do PCB. Além disso, será demonstrado um passo a passo da fabricação da placa de circuito impresso.

## 1.3.1	IDE Arduino

Para todo o código elaborado foi utilizado o ambiente de programação do Arduino, que utiliza linguagem de programação baseada em C e C++. Essa linguagem utiliza estruturas simples de programação e é bem difundida, o proporciona elaboração de programas até mesmo para pessoas sem conhecimento na área.(MOTA, 2017)

A IDE é encontrada gratuitamente no site oficial do Arduino e conta com várias opções de download que se adequam ao sistema operacional utilizado pelo seu computador.

Quando executada, a IDE exibida apresenta uma janela semelhante à Figura 11, onde apresenta funções que podem ser dividas em desenvolvimento do software, envio do código para o microcontrolador e interação com os dispositivos integrados ao micro.

![11](https://user-images.githubusercontent.com/40185350/41264456-30f259a4-6dc3-11e8-8fc4-0e1eb49a96e1.png)

## 1.3.2	Bibliotecas Utilizadas
 
Para o funcionamento adequando entre o programa elaborado e alguns dispositivos do hardware é necessário o uso de algumas bibliotecas.

Para o projeto utilizamos as bibliotecas demonstradas na Figura 12 abaixo:

![12](https://user-images.githubusercontent.com/40185350/41264473-40a65b52-6dc3-11e8-8873-5aa98f75a299.png)

A primeira biblioteca utilizada é a SoftwareSerial.h, que é utilizada para permitir comunicação serial em pinos digitais do Microcontrolador uma vez que o os pinos de serial nativo não estejam disponíveis. Através dela estabelecemos que os pinos digitais D2 e D3 seriam os responsáveis pela comunicação serial com o ESP8266-01.

A segunda biblioteca é a EmonLib.h, criada pela Open Energy Monitor. O uso desta biblioteca nos dar recurso para extrair os valores de corrente, tensão, fator de potência e os valores de cada tipo de potência de uma determinada aplicação. Para o projeto utilizaremos a Emonlib para obter os valores de corrente no sensor SCT-013-030 e os valores de tensão do circuito montado. Com esses valores se torna possível medir a potência utilizada e consequentemente o consumo de energia elétrica, que é o foco deste trabalho.

A terceira biblioteca é a Wire.h. Esta biblioteca é importante para todo trabalho que utiliza dispositivos I2C, no nosso caso o Real Time Clock (RTC).

O I2C é um protocolo de comunicação baseado no modelo mestre/escravo onde a função do mestre consiste em coordenar a comunicação enviando e recebendo informações dos escravos através de um barramento bidirecional.

A quarta e última biblioteca utilizada neste projeto é a DS3231.h que juntamente com a Wire.h atua sobre o protocolo I2C na troca de dados e processos de endereçamento. Com ela é possível obter os dados de data e hora como, ano, mês, hora, minuto etc. A aplicação dele no projeto se fez necessária para que seja possível enviar todos os dias no mesmo horário uma mensagem para o Twitter indicando os horários intermediários e de pico. Além disso, o consumo é baseado em KWh (Quilowatts-hora) o que torna o uso do RTC indispensável.
 
## 1.3.3	Configurando o ESP8266-01

Neste projeto o módulo ESP8266-01 é utilizado como ponte serial entre o AT- mega328 e a plataforma IoT. Para isso foi necessário programa-lo utilizando recursos de coman- dos AT.

Primeiramente deve-se verificar se nosso módulo está funcionando corretamente. Isso é feito através das linhas de código demonstrado na Figura 13 abaixo. O resultado deve ser observado através do monitor serial.

![13](https://user-images.githubusercontent.com/40185350/41264496-64729fa0-6dc3-11e8-9880-9383c692f56d.png)

Após checagem do ESP-01 e garantido o funcionamento, elaborou-se a função connectWifi (); como visto na Figura 14. Abaixo está descrito o objetivo de cada linha da função.

+	Linha 246 – é enviado o comando de reset para o módulo antes de conecta-lo a rede.

+	Na linha 247 – deve ser informado o nome e senha da rede nos campos SSID e PASSWORD respectivamente, para que o módulo possa se conectar à rede.

+	Na linha 249 – o comando CWMODE=1 configura o módulo como estação.

+	Na linha 250 – é definido que o módulo realizará uma única conexão. Este comando define a forma como os dados devem ser enviados.

+	Na linha 251 – O comando informará os dados IP referente a rede conectada.

![14](https://user-images.githubusercontent.com/40185350/41264509-6fe7943a-6dc3-11e8-8ef7-35de9e379484.png)

Como pode ser visto na figura acima, a função para conectar o módulo ESP8266-01 faz chamada da função sendData (); que poderá ser analisada no arquivo final que está em apêndice.

##  1.3.4	O ThingSpeak

Fabricada pela MathWorks, o ThingSpeak é uma plataforma IoT aberta que torna possível o armazenamento de dados em nuvem. Outras das funcionalidades permitida pela plataforma são a análise e visualização de fluxos de dados enviados pela internet que são representados em forma de gráficos.Para o protótipo foram criados 06 canais que recebem os valores de corrente, tensão, potência instantânea, consumo em KWh, gastos em reais e RTC como mostrado na Figura 15 (WORKS, 2015).
 
![15](https://user-images.githubusercontent.com/40185350/41264523-7d92e6de-6dc3-11e8-9a15-52f3a187aca9.png)

O ThingSpeak possui limite diário de oito mil mensagens e até 100 aplicações diferentes em 8 canais diferentes. Um protocolo de comunicação baseado em http é utilizado para envio e recebimento dos dados gerados a partir de qualquer dispositivo com recursos para comunicação em rede. É de grande importância ressaltar que para que não haja perda de dados no envio deve-se obedecer um intervalo de no mínimo 15 segundos entre envios.
Para estabelecer comunicação com a plataforma primeiramente é preciso iniciar conexão como mostrado na Figura 16 abaixo.

![16](https://user-images.githubusercontent.com/40185350/41264529-888ce65c-6dc3-11e8-92aa-e531a44726ba.png)

Entre as linhas 183 e 200 do código acima é enviado ao ESP o comando AT, IP e porta, necessário para abrir a comunicação com o ThingSpeak.

Em seguida é iniciado o envio dos dados através do campo “Get String” presente entre as linhas 131 e 154 do código mostrado na Figura 17. É importe ressaltar que deve-se enviar a chave de escrita gerada no momento do cadastro para que seja liberado a escrita nos canais criados.

O código acima poderá ser analisado junto ao código completo que está disponibili- zado em apêndice no final do trabalho.
 
![17](https://user-images.githubusercontent.com/40185350/41264539-96cdd83e-6dc3-11e8-8c4f-a01f172b49c3.png)

## 1.3.5	Calibração dos Sensores
    
O bloco de programação apresentado na Figura 18, mostra funções utilizadas no projeto e que são disponibilizadas pela biblioteca EmonLib.h já explanada no tópico de bibliotecas acima.

Através dessas funções são realizados cálculos dos valores de tensão, corrente, potências e fator de potência. Para que esses valores estejam de acordo com o real é preciso calibrar os sensores. Todas as técnicas para se obter os valores de calibração são baseadas em teorias demonstradas no site do Open Energy Monitor.

Para o sensor de corrente, a calibração é feita na linha 77 do código pela função emon1.current (x, y) onde o x representa o pino utilizado no microncontrolador e o y é a razão entre a relação de espiras e a resistência RL do sensor.

Após a calibração, a função emon1.calcIrms(1480) extrai o valor médio de corrente a partir de 1480 amostras, como pode ser visto na linha 285.

Para a leitura dos valores de tensão devemos primeiro garantir segurança, uma  vez que utilizaremos a tensão de rede, e que os sinais de tensão enviado para a porta do microcontrolador estão dentro da faixa aceitável pelo ADC.

Estando garantido estes requisitos podemos iniciar a calibração através da linha 78 do código com a função emon1.voltage (a, b, c) onde a é o pino utilizado no microcontrolador para receber o sinal, o b é o valor de calibração que deve ser ajustado pelo método de tentativa e erro até ser encontrado um valor que se adeque, e o c que é um valor padrão para o defasamento utilizado pela Open Energy.

Além disso, temos as a função emon1.calcVI(20,2000) na linha 299 significa o tamanho da janela de leitura definido pelo número de meias ondas por nós definidos, ou seja, 20 será o número de meias ondas e o 2000 o tempo de leitura em milissegundos.(MONITOR, 2016).

![18](https://user-images.githubusercontent.com/40185350/41264544-a0b5302c-6dc3-11e8-83e7-e58a23bbb711.png)

## 1.3.6	Desenvolvendo Alarmes utilizando o Twitter

Idealizado por Jack Dorsey, Evan Wiliams e Biz Stone em 2006, o twitter surgiu com o intuito de ser uma plataforma para troca de status em forma de mensagens curtas, com espaço para apenas 140 caracteres por envio.

Empregando está funcionalidade do twitter no projeto, utilizaremos essa rede social para informar o consumidor sobre início e fim de horários fora ponta, intermediário e ponta, afim de que seja possível, caso o consumidor deseje, transferir o uso de energia elétrica para horários que tornam o consumo mais baratos. Para realizar tal ação foi utilizado o React, que é um recurso disponibilizado pela plataforma ThingSpeak. Com ele pode-se realizar determinada ação pré-programada, como enviar uma mensagem ao Twitter através do monitoramento de um determinado canal.

Uma vez configurado para atender o projeto, o React irá monitora o canal RTC que nos horários de alarme apresentará graficamente o valor 1

Consequentemente o perfil TccIoT enviará uma mensagem para o perfil desejado informando os horários mais adequados para consumo.

5.	Para ajudar a manter o posicionamento do papel na placa, envolveu-se todo o material com outro papel comum e um pano.## 1.3.7	O ThingView

O ThingView é um aplicativo desenvolvido para sistemas Androide e IOS e tem sua aplicação voltada para apresentação dos dados de canais do ThingSpeak de forma rápida e fácil. Com ele possível fazer análise de dados em escalas de hora, dias, semanas, meses e ano, além de comparações com dados passados.

A introdução deste aplicativo no projeto tem como objetivo o monitoramento de forma fácil e a partir de qualquer local que permita acesso à internet.

Para utilizar o aplicativo é necessário entrar com os dados de Channel ID e API Key na interface A, como demostrado na Figura 19. Feito isso, é só aproveitar os benefícios de ter os valores de consumo na palma da mão, como visto na interface B.

![19](https://user-images.githubusercontent.com/40185350/41264556-b588b1a4-6dc3-11e8-95a0-bb6151445fd3.png)

## 1.3.8	Proteus

O simulador Proteus é uma ferramenta muita utilizada por aqueles que lidam com desenvolvimento eletrônico. O software permite desenhar circuitos e emprega um entorno gráfico, sendo possível aplicar os símbolos representativos dos componentes e assim realizar a simulação do seu funcionamento. Como comenta (Santos, 2014), a ferramenta possui a vantagem de não provocar danos aos circuitos, podendo incluir à simulação instrumentos de medição e inclusões gráficas que vem representar os sinais obtidos na simulação.

Ele é caracterizado pela facilidade de uso e compreensão através da sua interface e, por não oferecer o risco de danificar os equipamentos que são destinados aos ensaios e medidas de circuitos ou componentes, ele é capaz de substituir muito bem e com vantagens as experiências em laboratórios.

Iremos encontrar dentro do software Proteus o ISIS – Inteligente Schematic Input System (Sistema de Entrada Esquemático Inteligente), caracterizada por ser uma ferramenta para desenvolvimentos esquemáticos e encontraremos também o ARES – Advance Routing and Editing Software (Roteamento Avançado e Edição de Software), ferramente que permite o desenvolvimento de layouts (PCB) em que, através de uma interface própria, é possível importar o newtlist do ISIS, definir o padrão de trilhas, vias, pad’s, etc (SANTOS, 2014).

Para o projeto, o Proteus foi utilizado para realizar construção e simulação dos circuitos de tensão, corrente e offset.

## 1.3.9	Altium Designer

O Altium Designer é uma ferramenta com recursos avançados dedicada a elaboração esquemáticos e placas de circuitos elétricos/eletrônicos muito utilizado na indústria.

O software proporciona vantagens como a disponibilização de bibliotecas com diversos componentes para elaboração de projetos e a liberdade para criação de componentes específicos para uma determinada aplicação. Além disso, o software possui recursos que possibilitam a transferência de todo esquemático  para um ambiente de  construção de PCB  de forma profissional. É possível também realizar projetos em 3 dimensões, gerando maior visibilidade sobre o trabalho feito.

A Figura 20, demonstra o esquemático e o PCB elaborado para a construção do protótipo.
 
![20](https://user-images.githubusercontent.com/40185350/41264565-c30c2540-6dc3-11e8-934a-2deea1e9d644.png)

## 1.3.10	Fabricação da Placa

Depois de projetado, simulado e feito o layout de todo o circuito eletrônico, foi iniciado a confecção da placa de circuito impresso. De forma resumida, o processo para fabricação é explicado passo a passo abaixo:

1.	Com o layout da placa produzido, é preciso imprimi-lo. Para o projeto foi utilizado papel fotográfico com gramatura de 180. É importante informar que para se obter um melhor resultado, foi preciso realizar impressão a laser e em alta qualidade. Isso facilitou a transferência da tinta para a placa.

2.	Depois de impresso, determinou-se o tamanho da placa de cobre a ser utilizada. Foi utilizado para o projeto uma placa de cobre de tamanho 10x10cm.

3.	Definido o tamanho, é preciso limpar a placa de forma a retirar sujeira e outros elementos que prejudicariam o processo. Foi utilizado lã de aço para limpeza. É importante ressaltar que depois de limpa, não se deve tocar na parte de cobre da placa.

4.	Para transferir todo o circuito do papel para a placa, utilizou-se um ferro de passar comum, pré-aquecido com temperatura entre média e alta.

6.	O ferro aquecido foi passado com cuidado por 5 minutos, sobre a placa, de forma suave.Vale ressaltar que o tempo pode variar de caso para caso.

7.	Terminado o passo anterior, verificou-se com cuidado, se a tinta tinha sido transferida para o cobre.

8.	Garantida a transferência, iniciou-se o processo de corrosão da parte de cobre que não está com tinta. Nesta etapa do processo, mergulhou-se a placa em uma solução de percloreto de ferro diluído em água e aguardou-se entre 10 e 15 minutos.

9.	Após observar que a placa estava corroída, retirou-se cuidadosamente do recipiente com uma pinça e lavou-se em água corrente.

10.	O próximo passo realizado, foi limpar a placar com lã de aço para retirar a tinta que formou o circuito, a fim de expor o cobre que ficou após a corrosão.

O resultado de todo o processo é demonstrado abaixo na Figura 21.

![21](https://user-images.githubusercontent.com/40185350/41264568-ce6e4b48-6dc3-11e8-8dda-8a9d91066a1f.png)

Para finalizar a fabriação, realizou-se a furação e soldagem dos componentes na placa. O resultado pode ser visto no capítulo 2 deste trabalho.

## 1.3.11	Ubidots	

Uma empresa privada de serviços de engenharia que surgiu em 2012, Ubidots se especializou em soluções de hardware e software conectados para monitorar, controlar e automatizar remotamente processos para clients. 

Em 2018 a Ubidots expandiu e adaptou sua primeira versão em nuvem como “Ubidots for Education”. Hoje ela capacita cerca 35.000 estudantes para explorar e desenvolver tecnologia e soluções IoT, dando a todos os usuários um primeiro dispositivo gratuito.

Estas são algumas funções da versão “Ubidots for Education”:

+	Inclusão de até 10 widgets por painel para visualizar dados na nuvem em tempo real;

+	Com 10 variáveis por dispositivo, podemos exibir os dados brutos ou aplicando cálculos matemáticos para criar insights;

+	Podemos Compartilhar seus painéis e análises com links públicos e código incorporado;

+	Desenvolvimento de alertas por SMS, Email, Telegram e Webhook com base nos dados do seu sensor. Esses alertas são gerados mesmo que eles aconteçam ao mesmo tempo.

Para implementar a comunicação da ESP8266 com o Ubidots, é necessário que seja baixada a biblioteca Ubidots MQTT for ESP8266.Ela está disponivel na IDE do arduino,como Ubidots MQTT for ESP8266.

[22](https://user-images.githubusercontent.com/40185350/41264589-de1f0140-6dc3-11e8-9aac-109c6c426311.png)

Na imagem acima é mostrado em que parte do código configuramos a conexão com a rede wi-fi de sua escolha

Na imagem a seguir é enviado o valor da corrente para Ubidots, em seguida ele criará a variável automaticamente com o rótulo designado por você no código. Você pode enviar até 5 valores diferentes para Ubidots, se você deseja enviar mais valores apenas adicione esta linha client.add ("variable_name", value); ao seu código com os parâmetros necessários para você.

![23](https://user-images.githubusercontent.com/40185350/41264603-eae486c0-6dc3-11e8-81bb-864ff8b6ec8f.png)

# 2.	RESULTADOS

Após apresentado todo o desenvolvimento realizado para a construção do protótipo, explanação de conceitos e aplicação prática, se torna possível a análise dos resultados obtidos de forma a garantir a funcionalidade do equipamento dentro do que foi proposto: Monitoramento inteligente e em tempo quase real do consumo de energia elétrica. O físico do protótipo é mostrado na Figura abaixo:

![24](https://user-images.githubusercontent.com/40185350/41264613-f952a5de-6dc3-11e8-9cbe-889d8675131f.png)

## 2.1	CALIBRAÇÃO E TESTES

Para realizarmos a calibração e os testes do equipamento, utilizou-se uma carga com características resistivas a fim de obter o máximo de aproximação dos valores reais de corrente e um multímetro para se estabelecer o valor de tensão da rede.

Analisando os dados gerados, coletamos as informações produzidas pelo protótipo e realizamos algumas modificações via software para calibrar o equipamento a fim de reduzir o erro de medição.

Feito a calibração, iniciamos os testes para estabelecer as reais características de funcionamento do protótipo. Utilizou-se alguns equipamentos de uso residencial com o objetivo de realizar comparações e calcular o erro entre os valores de potência apresentados no protótipo e as características dos equipamentos. A Figura 26 abaixo, demostra os resultados obtidos e preenchidos em uma tabela no excel. É importante resaltar que o erro foi calculado baseado na fórmula abaixo, onde Pr = potência real, Pp = potência do protótipo e E(%) = erro percentual.

![f4](https://user-images.githubusercontent.com/40185350/41264718-94f05fb8-6dc4-11e8-8e59-0382956de93f.png)

![25](https://user-images.githubusercontent.com/40185350/41264623-0888e752-6dc4-11e8-8d00-b8150d32efd1.png)

## 2.2	ENVIO DOS RESULTADOS PARA A PLATAFORMA IOT THINGSPEAK E O APLI- CATIVO THINGVIEW

Para os resultados com a plataforma ThingSpeak, analisou-se a comunicação e o recebimento dos dados. Cada canal apresentou graficamente os dados enviados pelo equipamento. É importante ressaltar que para os valores de energia consumida os valores apresentados são uma somatória de todo o consumo. A Figura 27 abaixo, demonstra o momento do envio de dados pelo microcontrolador utilizando o ESP8266-01 como ponte serial.

![26](https://user-images.githubusercontent.com/40185350/41264633-15eef2ec-6dc4-11e8-8ce9-25a15d90d6d2.png)

Para o ThingView analisou-se somente se os dados apresentados no aplicativo eram os mesmos do ThingSpeak uma vez que o aplicativo é somente uma replicação da plataforma IoT.

## 2.3	ALERTAS COM O TWITTER

Conforme programação, verificamos que o funcionamento dos alarmes criados pelo RTC e enviados para o ThingSpeak geraram mensagens no twitter informando ao consumidor sobre os horários de pico e intermediário, alcançando assim o objetivo desejado. A Figura 28 mostra mensagens enviadas pela rede social.

![27](https://user-images.githubusercontent.com/40185350/41264639-2387f6ec-6dc4-11e8-86d7-1f0bc0962362.png)

## 2.4	DIFICULDADES ENCONTRAS

Do início até a conclusão do trabalho apareceram dificuldades que exigiam maior dedicação, conhecimento e pouco de sorte. Algumas destas são relatadas abaixo:

+	O primeiro desafio foi realizar o funcionamento do ESP8266-01 através dos comandos AT. Em grande parte das tentativas o módulo não respondia aos comandos enviados pela serial ou quando respondia, apresentava mensagens aleatórias e sem sentido. Só depois de muito estudo foi possível entender algumas funções e configurar parâmetros como velocidade de transmissão, modo de operação, conexão com a rede entre outros;

+	Elaboração do circuito para leitura de tensão. Houve até certo ponto várias tentativas de elaboração de um circuito ideal para a leitura de tensão de forma adequada e segura;

+	Leitura de ruídos pelas portas analógicas. Quando o protótipo estava sem tensão de entrada ou sem realizar leitura de corrente era possível através do monitor serial que valores aleatórios surgiam. Foi necessário medias tanto em hardware como software para eliminar este problema;

+	O pouco conhecimento com o software Altium designer e a grande variedade de comandos para uma determinada aplicação geraram dificuldades como rotear o circuito para gerar a PCB. Através de pesquisas foi possível corrigir detalhes como, espessura das trilhas, posição dos componentes, malha de terra entre outros.

## 2.5	CUSTOS PARA A CONSTRUÇÃO DO PROTÓTIPO

![q3](https://user-images.githubusercontent.com/40185350/41264673-6376dc3c-6dc4-11e8-8aa6-22c35cf37bc0.png)

#  REFERÊNCIAS


1. ASSIS, P. D. K. B. d. MICROCONTROLADOR. 17 f. Monografia (Graduação) — Universi- dade Presidente Antônio Carlos, Barbacena, 2004.

2. CASSIOLATO, C. Condicionamento de Sinais Analógicos e Sensores. 2018. Disponível em:
<http://www.smar.com/brasil/artigo-tecnico/condicionamento-de-sinais-analogicos-sensores>. Acesso em: 05-05-2018.

3. FORTES MáRCIO ZAMBOTI, R. A. T. G. D. A. V. S. M. A. R. F. M. M. Análise da adoção de
medidores inteligentes como instrumento da política pública de eficiência energética. Engevista, Engevista, v. 19, n. 2, p. 316–327, 2017.

4. MADEIRA, D. RTC- Registrando Data e Hora com Arduino. 2017. Disponível em: <https:
//portal.vidadesilicio.com.br/real-time-clock-rtc-ds3231/>. Acesso em: 09-05-2018.

5. MINATEL, P. Utilizando o ESP8266-01 com comandos AT. 2018. Disponível em: <http:http:
//pedrominatel.com.br/pt/esp8266/utilizando-o-modulo-esp8266-com-comandos-at/>. Acesso em: 05-05-2018.

6. MONITOR,      O.      E.      Installation      and      Calibration.       2016.       Disponível  em:	<https:https://learn.openenergymonitor.org/electricity-monitoring/ctac/ ct-and-ac-power-adaptor-installation-and-calibration-theory>. Acesso em: 10-05-201

7. MOTA, A. O que é o Arduino e como Funciona. 2017. Disponível em: <https://portal. vidadesilicio.com.br/o-que-e-arduino-e-como-funciona/>. Acesso em: 09-05-2018.

8. PUHLMANN, H. Trazendo o mundo real para dentro do processador - Condicio- namento de sinais analógicos. 2015. Disponível em: <https://www.embarcados.com.br/ condicionamento-de-sinais-analogicos/>. Acesso em: 05-05-2018.

9. SANTAELLA, L. Desvelando a internet das coisas. UFSCAR, 2013.

10. SANTOS, K. O software PROTEUS e sua visibilidade no processo de ensino em circuitos elétricos. 17 f. Monografia (Graduação) — Universidade Estadual da Paraíba, Campina Grande, 2014.

11. WORKS, M. Plataforma IoT ThingSpeak. 2015. Disponível em: <https://thingspeak.com/ pages/commercial_learn_more>. Acesso em: 07-05-2018.

12. YHDC. Sensor de Corrente SCT-013-030. 2015. Disponível em: <http://statics3.seeedstudio. com/assets/file/bazaar/product/101990028-SCT-013-030-Datasheet.pdf>. Acesso em: 07-05- 2018.

13.
 




 

 
