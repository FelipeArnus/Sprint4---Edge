# Sprint4 EdgeComputing:

Este é um projeto Arduino que utiliza um ESP32, sensores de temperatura, umidade e luz, e um display LCD para monitorar e exibir informações ambientais. Além disso, ele é capaz de publicar essas informações em um servidor MQTT para compartilhamento ou processamento remoto.

## Equipe:

- Felipe Arnus – RM: 550923
- João Pedro Vieira – RM: 99805
- João Pedro Chambrone – RM: 97857
- Vitor Hugo – RM: 97758

## Descrição do Projeto:

Este projeto tem como objetivo criar um sistema de monitoramento ambiental que coleta dados de temperatura, umidade e luminosidade, exibe essas informações em um display LCD e as envia para um servidor MQTT. Ele é adequado para aplicações como monitoramento de condições ambientais em espaços fechados, salas de cultivo, entre outros.

## Componentes Utilizados:

- 1 ESP32 (compatível com as bibliotecas e hardware utilizados)
- 1 Sensor de temperatura e umidade DHT11
- 1 Sensor de luminosidade LDR
- 1 Display LCD
- 2 Resistores 330 ohms
- 1 LED
- 1 Placa de prototipagem (breadboard)
- fios de conexão
- Acesso a uma rede Wi-Fi para comunicação MQTT

## Funcionamento do Código:

O código monitora os sensores continuamente e realiza as seguintes operações, alternando os itens 1 a 3 a cada 2 segundos:

1. **Monitoramento de Umidade:** verifica a umidade do ambiente e exibe-a no LCD. Se a umidade estiver acima de 70%, exibe "Umidade ALTA". Se estiver abaixo de 50%, exibe "Umidade BAIXA". Caso contrário, exibe "Umidade OK".

2. **Monitoramento de Luminosidade:** verifica a luminosidade do ambiente e exibe-a no LCD. Dependendo do valor lido, acende LEDs (verde, amarelo ou vermelho) para indicar o respectivo nível de luz.

3. **Monitoramento de Temperatura:** verifica a temperatura do ambiente e exibe-a no LCD. Se a temperatura estiver abaixo de 10°C, exibe "Temp: Baixa". Se estiver acima de 15°C, exibe "Temp: Alta". Caso contrário, exibe "Temp: OK".

4. **Comunicação MQTT:** O projeto utiliza o protocolo MQTT para enviar os dados coletados para um servidor MQTT externo. Isso permite que outros dispositivos recebam e processem essas informações.

## Configuração Wi-Fi e MQTT:

- SSID da rede Wi-Fi: "FIAP-IBM"
- Senha da rede Wi-Fi: "Challenge@23!"
- Servidor MQTT: "46.17.108.113"
- Porta MQTT: 1883
- Tópicos MQTT: "/TEF/hxh/cmd" (Subscribe) e "/TEF/hxh/attrs", "/TEF/hxh/attrs/humidity", "/TEF/hxh/attrs/temperature", "/TEF/hxh/attrs/light" (Publish)

## Instruções de Uso:

1. Carregue o código no seu ESP32.
2. Conecte os sensores de temperatura, umidade e luz ao ESP32 de acordo com as conexões definidas no código.
3. Certifique-se de que seu ESP32 está conectado à rede Wi-Fi correta.
4. Inicie um cliente MQTT para receber os dados publicados nos tópicos correspondentes.

## Caso deseje utilizar uma conexão via API segue as instruções de execução via Ubuntu: 

1. **Instalar o ifconfig para identificar o IP da máquina virtual:**
- sudo apt-get install net-tools

2. **Comando para ler o IP da VM (Virtual Machine):**
 - ifconfig

3. **Instalar o git**
- sudo apt install git

4. **Instale o docker(apenas o primeiro do site | ilustrado do 5. até 11.)**
- (https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-20-04)

5. **Primeiro, atualize sua lista existente de pacotes:**
- sudo apt update 

6. **Pacotes de pré-requisitos que permitem aptusar pacotes via HTTPS:**
- sudo apt install apt-transport-https ca-certificates curl software-properties-common 

7. **Adicionar a chave GPG do repositório oficial do Docker ao seu sistema:**
- curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

8. **Adicionar o repositório Docker às fontes APT:**
- sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"

9. **Certifique-se de instalar a partir do repositório Docker em vez do repositório padrão do Ubuntu:**
- apt-cache policy docker-ce

10. **Instale o Docker**
- sudo apt install docker-ce

11. **Verificação se o docker esta ativo**
- sudo systemctl status docker

12. **Copiar os arquivos do repositório de Fiware Descomplicado**
- git clone https://github.com/fabiocabrini/fiware

13. **Entrar na pasta do Fiware[Colocar Codigo]**
- cd fiware

14. **Rodar os containers[Colocar Codigo]**
- sudo docker compose up -d

15. **status dos containers[Colocar Codigo]**
- sudo docker stats

16. **faça o download desse arquivo**
- (github.com/fabiocabrini/fiware/blob/main/FIWARE.postman_collection.json)

17. **Entrar em myWorkspace**
- importe o arquivo do passo 16.

18. **Clique em Health Check e reabra o terminal[Colocar Codigo]**
- ifconfig
- encontre a linha inet xxx.xxx.x.xx(copie esse numero(os 'x'))

19. **Substitua( {{url}} ) do get em Health Check pelo copiado**
- Repita em IOT Agent MQTT, Orion Context Broker e STH-Comet


## Simulação:

Você pode simular este projeto no Wokwi usando o seguinte link: [Simulação no Wokwi](https://wokwi.com/projects/380871723922614273).
