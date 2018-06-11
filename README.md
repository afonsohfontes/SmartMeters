

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



    
