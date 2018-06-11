
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


    
